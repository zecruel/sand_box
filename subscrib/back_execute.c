#include "civetweb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

static pid_t process_pid = -1;

static int start_process(void)
{
    if (process_pid > 0)
        return 0; // já executando

    pid_t pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0)
    {
        // Processo filho

        setsid();

        freopen("/dev/null", "r", stdin);
        freopen("/tmp/meuprocesso.log", "a", stdout);
        freopen("/tmp/meuprocesso.log", "a", stderr);

        execl(
            "/opt/minhaapp/minhaapp",
            "minhaapp",
            NULL);

        exit(EXIT_FAILURE);
    }

    process_pid = pid;

    return 0;
}

static int stop_process(void)
{
    if (process_pid <= 0)
        return -1;

    kill(process_pid, SIGTERM);

    process_pid = -1;

    return 0;
}

static int process_handler(
    struct mg_connection *conn,
    void *cbdata)
{
    char body[1024];
    int len;

    len = mg_read(conn, body, sizeof(body)-1);
    body[len] = '\0';

    if (strstr(body, "\"start\""))
    {
        if (start_process() == 0)
        {
            mg_printf(conn,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n\r\n"
                "{\"status\":\"executando\",\"pid\":%d}",
                process_pid);
        }
        else
        {
            mg_printf(conn,
                "HTTP/1.1 500 Internal Server Error\r\n"
                "Content-Type: application/json\r\n\r\n"
                "{\"error\":\"falha ao iniciar\"}");
        }
    }
    else if (strstr(body, "\"stop\""))
    {
        stop_process();

        mg_printf(conn,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n\r\n"
            "{\"status\":\"parado\"}");
    }

    return 200;
}

static int is_running(void)
{
    if (process_pid <= 0)
        return 0;

    return (kill(process_pid, 0) == 0);
}

static int status_handler(
    struct mg_connection *conn,
    void *cbdata)
{
    mg_printf(conn,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n\r\n"
        "{"
        "\"running\":%s,"
        "\"pid\":%d"
        "}",
        is_running() ? "true" : "false",
        process_pid);
    
    return 200;
}


#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "civetweb.h"

#define TXT_DIR "/opt/app/textos"

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
                strcmp(name + len - 4, ".txt") == 0)
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


