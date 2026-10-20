/*
 * goose_publisher_example.c
 */

/* compile with:
* gcc -O2 goose_publisher_example.c -pthread -Iinclude -Llib -liec61850 -llua -lm -o goose
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

static int running = 0;
struct goose_param {
  GoosePublisher publisher;
  LinkedList dataSetValues;
  pthread_mutex_t mutex; // = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t cond; // Inicializada dinamicamente no main()
  int update;
};

void handle_signal(int signal) {
  printf("Signal received: %d\n", signal);
  running = 0;
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


/* has to be executed as root! */
int
main(int argc, char **argv)
{
    char *interface;
    if (argc > 1)
        interface = argv[1];
    else
        interface = "eth0";

    printf("Using interface %s\n", interface);
    Quality q = QUALITY_VALIDITY_GOOD;
    LinkedList dataSetValues = LinkedList_create();

    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(1234));
    LinkedList_add(dataSetValues, Quality_toMmsValue(&q, NULL));
    LinkedList_add(dataSetValues, MmsValue_newIntegerFromInt32(5678));

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

    signal(SIGINT, handle_signal); // Register signal handler for SIGINT

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
        GoosePublisher_setGoCbRef(publisher, "simpleIOGenericIO/LLN0$GO$gcbAnalogValues");
        GoosePublisher_setConfRev(publisher, 1);
        GoosePublisher_setDataSetRef(publisher, "simpleIOGenericIO/LLN0$AnalogValues");
        GoosePublisher_setTimeAllowedToLive(publisher, 500);
        pthread_t rec_thread;
        int curr_up = 0;
        struct goose_param param;
        param.publisher = publisher;
        param.dataSetValues = dataSetValues;
        param.update = 0;
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
        usleep(5500 * 1000); 
        goose_update((void*)&param);
        usleep(500 * 1000); 
        goose_update((void*)&param);
        usleep(100 * 1000); 
        goose_update((void*)&param);
        usleep(5500 * 1000); 
        goose_update((void*)&param);




        printf("Waiting for signal...\n");
        pause(); // Suspend program execution until a signal arrives
        printf("Program resumed after signal.\n");

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




