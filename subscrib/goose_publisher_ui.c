/*
 * goose_publisher_example.c
 */

/* compile with:
* gcc -O2 goose_publisher_ui.c civetweb.c -DNO_SSL -pthread -Iinclude -Llib -liec61850 -llua -lm -lc -o goose_ui
*/ 

#include <unistd.h>
#include <signal.h>

// Linux includes
#include <math.h>

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include "mms_value.h"
#include "goose_publisher.h"
#include "hal_thread.h"

#include <fcntl.h>

#include "civetweb.h"


#define DOCUMENT_ROOT "."
#define PORT "8888,8884"
#define EXIT_URI "/exit"

volatile int exitNow = 0;
/* shared memory */
#include <sys/mman.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
static int running = 0;

struct goose_param {
  GoosePublisher publisher;
  LinkedList dataSetValues;
  pthread_mutex_t mutex; // = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond; // Inicializada dinamicamente no main()
  int update;
};
struct data_ctx {
  struct goose_param *param;
  MmsValue * bin1;
  MmsValue * bin2;
  MmsValue * bin3;
  MmsValue * bin4;

};

lua_State *L;

int log_message(const struct mg_connection *conn, const char *message) {
	puts(message);
	return 1;
}
int ExitHandler(struct mg_connection *conn, void *cbdata) {
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: "
	          "text/plain\r\nConnection: close\r\n\r\n");
	mg_printf(conn, "Server will shut down.\n");
	mg_printf(conn, "Bye!\n");
	exitNow = 1;
  running = 0;
	return 1;
}

int FileHandler(struct mg_connection *conn, void *cbdata) {
	/* In this handler, we ignore the req_info and send the file "fileName". */
	const char *fileName = (const char *)cbdata;

	mg_send_file(conn, fileName);
	return 1;
}

// Função auxiliar para calcular o tempo absoluto futuro usando o relógio Monotônico
void get_fut_t(struct timespec *ts, long ms) {
    // CLOCK_MONOTONIC garante que o tempo apenas avança e ignora ajustes do sistema (NTP/Hora de Verão)
    clock_gettime(CLOCK_MONOTONIC, ts);
    
    ts->tv_sec += ms / 1000;
    ts->tv_nsec += (ms % 1000) * 1000000;

    if (ts->tv_nsec >= 1000000000) {
        ts->tv_sec += 1;
        ts->tv_nsec -= 1000000000;
    }
}

void * goose_thread(void * argument) {
  struct goose_param *param = argument;
  if (running) {
    long wait_t[] = {2, 4, 20, 100, 500, 1000}; // Máximo de 5 segundos se não for infinito
    int t_idx = 0;
    struct timespec timeout;
    
    pthread_mutex_lock(&(param->mutex));
    

    // O loop while protege contra Spurious Wakeups em ambos os modos
    while (running) {
        param->update = 0;
        get_fut_t(&timeout, wait_t[t_idx]);
        while(!param->update) {
            int res = pthread_cond_timedwait(&(param->cond), &(param->mutex), &timeout);
            
            if (res == ETIMEDOUT) {
                //printf("[Consumidora] TIMEOUT atingido no relógio monotônico!\n");
                GoosePublisher_publish(param->publisher, param->dataSetValues);
                if (t_idx < 5) t_idx++;
                break;
            } else if (res == 0) {
                //printf("[Consumidora] Acordei com sinal dentro do prazo do timeout!\n");
                GoosePublisher_increaseStNum(param->publisher);
                GoosePublisher_publish(param->publisher, param->dataSetValues);
                t_idx = 0;
                break;

            } else {
                perror("[Consumidora] Erro no timedwait");
                break;
            }
        }
    }

    pthread_mutex_unlock(&(param->mutex));
  }
  return NULL;
}

// Thread que envia o sinal para acordar
void* goose_update(void* arg) {
    
    struct goose_param *param = arg;
    pthread_mutex_lock(&(param->mutex));
    param->update = 1;
    //printf("[Produtora] Evento pronto! Enviando pthread_cond_signal...\n");
    pthread_cond_signal(&(param->cond)); 
    
    pthread_mutex_unlock(&(param->mutex));
    return NULL;
}
int goose_toggle_handler(struct mg_connection *conn, void *cbdata) {

    static bool state = false;
    struct data_ctx *ctx = cbdata;
    state = !state;
    MmsValue_setBoolean(ctx->bin1, state);

    char json_buffer[256];

    // 3. Geração da string JSON usando snprintf
    // O operador ternário converte o tipo 'bool' nas strings "true" ou "false" exigidas pelo JSON
    snprintf(json_buffer, sizeof(json_buffer),
        "{"
        "\"bin1\":%s,"
        "\"bin2\":%s,"
        "\"bin3\":%s,"
        "\"bin4\":%s"
        "}",
        MmsValue_getBoolean(ctx->bin1) ? "true" : "false",
        MmsValue_getBoolean(ctx->bin2)   ? "true" : "false",
        MmsValue_getBoolean(ctx->bin3)  ? "true" : "false",
        MmsValue_getBoolean(ctx->bin4) ? "true" : "false"
    );


	  mg_printf(conn, "HTTP/1.1 200 OK\r\n");
    mg_printf(conn, "Content-Type: Content-Type: application/json\r\n\r\n");
    mg_printf(conn, "%s", json_buffer);
    mg_printf(conn, "\r\n");
    goose_update((void *)ctx->param);
	return 1;
}

/* has to be executed as root! */
int
main(int argc, char **argv)
{
    char *interface;
    if (argc > 1)
        interface = argv[1];
    else
        interface = "eth0";

    struct data_ctx d_ctx;
    d_ctx.bin1 = MmsValue_newBoolean(false);
    d_ctx.bin2 = MmsValue_newBoolean(false);
    d_ctx.bin3 = MmsValue_newBoolean(false);
    d_ctx.bin4 = MmsValue_newBoolean(false);


    printf("Using interface %s\n", interface);
    Quality q = QUALITY_VALIDITY_GOOD;
    LinkedList dataSetValues = LinkedList_create();

    // LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
    // LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));
    // LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

    LinkedList_add(dataSetValues, d_ctx.bin1);
    LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));
    LinkedList_add(dataSetValues, d_ctx.bin2);
    LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));
    LinkedList_add(dataSetValues, d_ctx.bin3);
    LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));
    LinkedList_add(dataSetValues, d_ctx.bin4);
    LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));




    CommParameters gooseCommParameters;

    gooseCommParameters.appId = 1000;
    gooseCommParameters.srcAddress[0] = 0x00;
    gooseCommParameters.srcAddress[1] = 0x07;
    gooseCommParameters.srcAddress[2] = 0x44;
    gooseCommParameters.srcAddress[3] = 0xca;
    gooseCommParameters.srcAddress[4] = 0xce;
    gooseCommParameters.srcAddress[5] = 0x1e;
    gooseCommParameters.dstAddress[0] = 0x01;
    gooseCommParameters.dstAddress[1] = 0x0c;
    gooseCommParameters.dstAddress[2] = 0xcd;
    gooseCommParameters.dstAddress[3] = 0x01;
    gooseCommParameters.dstAddress[4] = 0x00;
    gooseCommParameters.dstAddress[5] = 0x01;
    gooseCommParameters.vlanId = 0;
    gooseCommParameters.vlanPriority = 4;


    printf("Activating realtime scheduler.\n");
    //use_realtime_scheduler();
    /*
     * Create a new GOOSE publisher instance. As the second parameter the interface
     * name can be provided (e.g. "eth0" on a Linux system). If the second parameter
     * is NULL the interface name as defined with CONFIG_ETHERNET_INTERFACE_ID in
     * stack_config.h is used.
     */
    GoosePublisher publisher = GoosePublisher_create(&gooseCommParameters, interface);

    if (publisher) {
        GoosePublisher_setGoCbRef(publisher, "Zecruel_Mestre/LLN0$GO$gcbGoose");
        GoosePublisher_setConfRev(publisher, 1);
        GoosePublisher_setDataSetRef(publisher, "Zecruel_Mestre/LLN0$cbGoose");
        GoosePublisher_setTimeAllowedToLive(publisher, 2000);
        pthread_t rec_thread;
        int curr_up = 0;
        struct goose_param param;
        param.publisher = publisher;
        param.dataSetValues = dataSetValues;
        param.update = 0;
        d_ctx.param = &param;
        pthread_mutex_init(&param.mutex, NULL);
// --- Inicialização da Variável de Condição com CLOCK_MONOTONIC ---
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    pthread_condattr_setclock(&attr, CLOCK_MONOTONIC); // Vincula ao relógio monotônico
    pthread_cond_init(&param.cond, &attr);
    pthread_condattr_destroy(&attr); // O objeto de atributos não é mais necessário
    // -----------------------------------------------------------------
        int retcode = pthread_create(&rec_thread, NULL, goose_thread, (void*)&param);
        running = 1;
        if (retcode != 0){
          printf("Failed to create pthread. retcode = %i: %s\n", retcode, strerror(retcode));
          return 1;
        }
        // usleep(5500 * 1000); 
        // goose_update((void*)&param);
        // usleep(500 * 1000); 
        // goose_update((void*)&param);
        // usleep(100 * 1000); 
        // goose_update((void*)&param);
        // usleep(5500 * 1000); 
        // goose_update((void*)&param);
        //

	const char *options[] = {
		"document_root", DOCUMENT_ROOT,
		"listening_ports", PORT,
		"request_timeout_ms", "10000",
		"error_log_file", "error.log",
		"enable_auth_domain_check", "no",
		0
	};
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

	mg_set_request_handler(ctx, EXIT_URI, ExitHandler, 0);

	mg_set_request_handler(ctx, "/plot", FileHandler, (void *)"./teste_plot.html");

	/* Add handler for /postresponse example */
	// mg_set_request_handler(ctx, "/postresponse", PostResponser, 0);
	// mg_set_request_handler(ctx, "/getconfig", get_config, 0);
	// mg_set_request_handler(ctx, "/savecapture", save_capture, 0);
	mg_set_request_handler(ctx, "/toggle", goose_toggle_handler, (void *)&d_ctx);

	
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
			
			printf(
			    "Exit at %s://%s:%i%s\n", proto, host, ports[n].port, EXIT_URI);
			printf("\n");
		}

	}

	/* Wait until the server should be closed */
	while (!exitNow) {
#ifdef _WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
	}

	/* Stop the server */
	mg_stop(ctx);
  lua_close(L);
	printf("Server stopped.\n");
	printf("Bye!\n");

	// return EXIT_SUCCESS;

        const char * return_message;
        retcode = pthread_join(rec_thread, (void**)&return_message);
        if (retcode != 0)
        {
          printf("Failed to join (terminate) pthread. retcode = %i: %s\n",
           retcode, strerror(retcode));
        }
        GoosePublisher_destroy(publisher);
    }
    else {
        printf("Failed to create GOOSE publisher. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
    }

    LinkedList_destroyDeep(dataSetValues, (LinkedListValueDeleteFunction) MmsValue_delete);

    return 0;
}




