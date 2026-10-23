#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "civetweb.h"

#define TXT_DIR "."

#define DOCUMENT_ROOT "."
#define PORT "8888,8884"
static int files_handler(struct mg_connection *conn, void *cbdata)
{
    DIR *dir;
    struct dirent *entry;
    int first = 1;

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Cache-Control: no-cache\r\n\r\n");

    mg_printf(conn, "[");

    dir = opendir(TXT_DIR);

    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            const char *name = entry->d_name;
            size_t len = strlen(name);

            if (len > 4 &&
                strcmp(name + len - 2, ".c") == 0)
            {
                if (!first)
                    mg_printf(conn, ",");

                mg_printf(conn, "\"%s\"", name);

                first = 0;
            }
        }

        closedir(dir);
    }

    mg_printf(conn, "]");

    return 200;
}

#include <limits.h>

static int view_handler(struct mg_connection *conn, void *cbdata)
{
    const struct mg_request_info *ri =
        mg_get_request_info(conn);

    char filename[PATH_MAX];
    FILE *fp;
    char buffer[4096];
    size_t n;

    const char *prefix = "/api/view/";

    snprintf(filename,
             sizeof(filename),
             "%s/%s",
             TXT_DIR,
             ri->local_uri + strlen(prefix));

    fp = fopen(filename, "rb");

    if (!fp)
    {
        mg_printf(conn,
            "HTTP/1.1 404 Not Found\r\n\r\n");
        return 404;
    }

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n\r\n");

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        mg_write(conn, buffer, n);
    }

    fclose(fp);

    return 200;
}

static int download_handler(struct mg_connection *conn,
                            void *cbdata)
{
    const struct mg_request_info *ri =
        mg_get_request_info(conn);

    char filename[PATH_MAX];
    FILE *fp;
    char buffer[4096];
    size_t n;

    const char *prefix = "/api/download/";
    const char *name =
        ri->local_uri + strlen(prefix);

    snprintf(filename,
             sizeof(filename),
             "%s/%s",
             TXT_DIR,
             name);

    fp = fopen(filename, "rb");

    if (!fp)
    {
        mg_printf(conn,
          "HTTP/1.1 404 Not Found\r\n\r\n");
        return 404;
    }

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Disposition: attachment; "
        "filename=\"%s\"\r\n\r\n",
        name);

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        mg_write(conn, buffer, n);
    }

    fclose(fp);

    return 200;
}


// Função do Handler que processa a requisição do arquivo binário
static int handler_arquivo_binario(struct mg_connection *conn, void *cbdata) {
    // 1. Simulação de um buffer binário na memória (pode ser um struct, leitura de sensor, etc.)
    // Aqui criamos um buffer de 64 bytes com valores incrementais para teste
    unsigned char dados_binarios[64];
    for (int i = 0; i < 64; i++) {
        dados_binarios[i] = (unsigned char)i; 
    }
    size_t tamanho_dados = sizeof(dados_binarios);

    // 2. Envia o cabeçalho HTTP de sucesso (Status 200 OK)
    mg_printf(conn, "HTTP/1.1 200 OK\r\n");
    
    // Define o tipo de conteúdo como binário bruto
    mg_printf(conn, "Content-Type: application/octet-stream\r\n");
    
    // Informa ao navegador o tamanho exato do buffer em bytes
    mg_printf(conn, "Content-Length: %lu\r\n", (unsigned long)tamanho_dados);
    
    // Cabeçalhos de CORS essenciais para o client JS em domínios/portas diferentes
    mg_printf(conn, "Access-Control-Allow-Origin: *\r\n");
    mg_printf(conn, "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n");
    mg_printf(conn, "Access-Control-Allow-Headers: Content-Type\r\n");
    
    // Fim dos cabeçalhos (linha em branco obrigatória)
    mg_printf(conn, "\r\n");

    // 3. Envia o bloco binário bruto diretamente para a conexão socket
    // mg_write escreve os bytes puros na rede sem interpretar formatação de string (%s, %d, etc.)
    int bytes_enviados = mg_write(conn, dados_binarios, tamanho_dados);

    // Retorna 1 para avisar ao Civetweb que a requisição foi completamente tratada por este handler
    return 1; 
}

int log_message(const struct mg_connection *conn, const char *message) {
  puts(message);
  return 1;
}
int main(int argc, char **argv) {

    const char *options[] = {"document_root",
                             DOCUMENT_ROOT,
                             "listening_ports",
                             PORT,
                             "request_timeout_ms",
                             "10000",
                             "error_log_file",
                             "error.log",
                             "enable_auth_domain_check",
                             "no",
                             0};
    struct mg_callbacks callbacks;
    struct mg_context *ctx;
    struct mg_server_port ports[32];
    int port_cnt, n;

    /* Start CivetWeb web server */
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.log_message = log_message;
    ctx = mg_start(&callbacks, 0, options);

    /* Check return value: */
    if (ctx == NULL) {
      fprintf(stderr, "Cannot start CivetWeb - mg_start failed.\n");
      return EXIT_FAILURE;
    }

mg_set_request_handler(ctx,
                       "/api/files",
                       files_handler,
                       NULL);

mg_set_request_handler(ctx,
                       "/api/view/",
                       view_handler,
                       NULL);

mg_set_request_handler(ctx,
                       "/api/download/",
                       download_handler,
                       NULL);

	/* List all listening ports */
	memset(ports, 0, sizeof(ports));
	port_cnt = mg_get_server_ports(ctx, 32, ports);
	printf("\n%i listening ports:\n\n", port_cnt);

	for (n = 0; n < port_cnt && n < 32; n++) {
		const char *proto = ports[n].is_ssl ? "https" : "http";
		const char *host;

		if ((ports[n].protocol & 1) == 1) {
			/* IPv4 */
			host = "127.0.0.1";
			printf("Browse files at %s://%s:%i/\n", proto, host, ports[n].port);
			
		}

	}

	/* Wait until the server should be closed */
	while (1) {
		sleep(1);
	}

	/* Stop the server */
	mg_stop(ctx);
	printf("Server stopped.\n");
	printf("Bye!\n");

	return EXIT_SUCCESS;
}

#if(0)
#include <sys/stat.h>
#include <time.h>

typedef struct
{
    char nome[256];
    time_t data;
} arquivo_info_t;


static int comparar_arquivos(const void *a, const void *b)
{
    const arquivo_info_t *fa = a;
    const arquivo_info_t *fb = b;

    if (fa->data < fb->data)
        return 1;   // mais recente primeiro

    if (fa->data > fb->data)
        return -1;

    return strcmp(fa->nome, fb->nome);
}

static int files_handler(struct mg_connection *conn, void *cbdata)
{
    DIR *dir;
    struct dirent *entry;

    arquivo_info_t arquivos[4096];
    int total = 0;
    int i;

    dir = opendir(TXT_DIR);

    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            const char *nome = entry->d_name;
            size_t len = strlen(nome);

            if (len < 4)
                continue;

            if (strcmp(nome + len - 4, ".txt") != 0)
                continue;

            char caminho[PATH_MAX];
            struct stat st;

            snprintf(caminho,
                     sizeof(caminho),
                     "%s/%s",
                     TXT_DIR,
                     nome);

            if (stat(caminho, &st) != 0)
                continue;

            strncpy(arquivos[total].nome,
                    nome,
                    sizeof(arquivos[total].nome) - 1);

            arquivos[total].nome[
                sizeof(arquivos[total].nome) - 1] = '\0';

            arquivos[total].data = st.st_mtime;

            total++;
        }

        closedir(dir);
    }

    qsort(arquivos,
          total,
          sizeof(arquivo_info_t),
          comparar_arquivos);

    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n\r\n");

    mg_printf(conn, "[");

    for (i = 0; i < total; i++)
    {
        if (i > 0)
            mg_printf(conn, ",");

        char data_str[32];

        strftime(data_str,
                 sizeof(data_str),
                 "%Y-%m-%d %H:%M:%S",
                 localtime(&arquivos[i].data));

        mg_printf(conn,
            "{\"name\":\"%s\","
            "\"date\":\"%s\"}",
            arquivos[i].nome,
            data_str);
    }

    mg_printf(conn, "]");

    return 200;
}
#endif

