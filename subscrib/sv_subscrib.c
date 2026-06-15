/*
 * sv_subscrib.c
 *
 * Program for Sampled Values (SV) subscriber
 * This program capture SV streams and write their data to a shared memory space.
 * The configuration file, in Lua laguage, must inform the AppId and desired dataset
 * objects indexes to store data. We assume a dataset always formed by a INT32, with 
 * sample data, and its quality word (4 bytes).
 * Optionaly inform in config the samples per sec and circular buffer size
 * in seconds (default = 4800 samples and 2 seconds).
 *
 * Used libs:
 *   - libiec61850
 *   - Lua
 *
 * Compile:
 * gcc -O3 -std=c17 sv_subscrib.c -pthread -Iinclude -Llib -liec61850 -llua -lm -o sv_subscrib
 *
 * To execute without sudo permission, change program's capabilities by:
 * sudo setcap "cap_sys_nice=ep cap_ipc_lock=ep cap_net_raw+eip" ./sv_subscrib
 *
 * if not work, try:
 * sudo sysctl -w kernel.sched_rt_runtime_us=-1
 * 
 */

#include <stdint.h>  // For `uint8_t`, `int8_t`, etc.
#include "hal_thread.h"
#include <signal.h>
#include <stdio.h>
#include "sv_subscriber.h"
#include <pthread.h>
#include "goose_receiver.h"
#include "goose_subscriber.h"
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
/* shared memory */
#include <sys/mman.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define MAX_SV 100
#define MAX_DS 20


struct SV_stream{
  int AppId;
  int ds_size;
  int ds_idx[MAX_DS];
  int payload_size;
  int smp_max;
  int buf_max;
  int32_t *data;
  int32_t *pos;
  int lock;
};

static bool running = true;

void sigint_handler(int signalId)
{
  running = 0;
}

/* Callback handler for received SV messages */
static void svUpdateListener (SVSubscriber subscriber,
  void* parameter, SVSubscriber_ASDU asdu) {
  struct SV_stream *stream = parameter; 
  //int app_id = stream->AppId;

  int smp_cnt = SVSubscriber_ASDU_getSmpCnt(asdu);

  if (*(stream->pos) % stream->smp_max != smp_cnt){
    printf ("pos=%d, cnt=%d\n", *(stream->pos), smp_cnt);
    *(stream->pos) = smp_cnt;
  }

   /*
    * Access to the data requires a priori knowledge of the data set.
    * We assume a data set consisting of INT32 values encoded as 4 bytes.
    * Follow each value, a word of 4 bytes with qualitiy information (ignored).
    * You can find the first INT32 value at byte position 0, the second value 
    * at byte position 8, the third value at byte position 16, and so on.
    *
    * To prevent damages due configuration, please check the length of the
    * data block of the SV message before accessing the data.
    */
  if (SVSubscriber_ASDU_getDataSize(asdu) >= stream->payload_size) {
    int i;
    for (i = 0; i < stream->ds_size; i++){
      int idx = *(stream->pos) * stream->ds_size + i;
      stream->data[idx] = SVSubscriber_ASDU_getINT32(asdu, stream->ds_idx[i]);
      //printf("%d,", stream->data[idx]);
    }
    //printf("\n");
  }
  
  *(stream->pos) = *(stream->pos) + 1;
  if (*(stream->pos) >= stream->buf_max) *(stream->pos) = 0;
}


/* Aux function for qsort */
int comp_asc(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return (val_a > val_b) - (val_a < val_b); /* avoid overflow */
}

int main(int argc, char** argv) {
  SVReceiver SV_receiver = SVReceiver_create();
  
  char *interface;
  char *config_file;
  int streams_len = 0;
  struct SV_stream streams[MAX_SV];
  SVSubscriber subs[MAX_SV];

  if (argc > 1) {
    SVReceiver_setInterfaceId(SV_receiver, argv[1]);
    printf("Set interface id: %s\n", argv[1]);
    interface = argv[1];
  }
  else {
    printf("Using interface eth0\n");
    SVReceiver_setInterfaceId(SV_receiver, "eth0");
    interface = "eth0";
  }

  if (argc > 2) {
    printf("Config file: %s\n", argv[2]);
    config_file = argv[2];
  }
  else {
    printf("Using default config file - sv_config.lua\n");
    config_file = "sv_config.lua";
  }
 
  /* Read config file with Lua interpreter */
  lua_State *L = luaL_newstate(); /* opens Lua */
  luaL_openlibs(L); /* opens the standard libraries */
  /* execute Lua config file */
  if (luaL_loadfile(L, config_file) || lua_pcall(L, 0, 0, 0)){
    printf("cannot run config. file: %s", lua_tostring(L, -1));
    return 1; /* error */
  }
  /* -------------------- parse config file  -------------------*/
	lua_getglobal(L, "sampled_values");
	if (lua_istable(L, -1)){
		
		/* iterate over table */
		lua_pushnil(L);  /* first key */
		while (lua_next(L, -2) != 0) { /* table index are shifted*/
			/* uses 'key' (at index -2) and 'value' (at index -1) */
			
			if (lua_istable(L, -1) && streams_len < MAX_SV - 1){
        int ok = 1;
        lua_getfield(L, -1, "AppId");
        if (lua_isinteger(L, -1)){
          streams[streams_len].AppId = lua_tointeger(L, -1);
          ok &= 1;
        } else ok = 0;
        lua_pop(L, 1);

        lua_getfield(L, -1, "samples_sec");
        if (lua_isinteger(L, -1)){
          streams[streams_len].smp_max = lua_tointeger(L, -1);
        } else streams[streams_len].smp_max = 4800;
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "seconds");
        if (lua_isinteger(L, -1)){
          streams[streams_len].buf_max = lua_tointeger(L, -1) *
            streams[streams_len].smp_max;
        } else streams[streams_len].buf_max =
           2 * streams[streams_len].smp_max;
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "dataset");
        if (lua_istable(L, -1)){
          int ds_size = 0, max = 0;
		      /* iterate over table */
          lua_pushnil(L);  /* first key */
		      while (lua_next(L, -2) != 0) { /* table index are shifted*/
			      /* uses 'key' (at index -2) and 'value' (at index -1) */
            if (lua_istable(L, -1)){
              lua_getfield(L, -1, "idx");
              if (lua_isinteger(L, -1)){
                int idx = lua_tointeger(L, -1);
                if (idx >= 0 && ds_size < MAX_DS-1) {
                  max = (idx > max)? idx : max;
                  streams[streams_len].ds_idx[ds_size] = idx * 8;
                  ds_size++;
                }
              }
              lua_pop(L, 1);
            }
            /* removes 'value'; keeps 'key' for next iteration */
			      lua_pop(L, 1);
		      }
          lua_pop(L, 1);
          if (ds_size == 0) ok = 0;
          else {
            qsort(streams[streams_len].ds_idx, ds_size, sizeof(int), comp_asc);
            streams[streams_len].ds_size = ds_size;
            streams[streams_len].payload_size = max * 8 + 8;
          }
        } else ok = 0;
        //lua_pop(L, 1);
        
        if (ok) {
          streams[streams_len].lock = 0;
          
          /* shared memory config */
          int oflags = O_RDWR | O_CREAT;  //O_RDONLY
	        int length = (1 + streams[streams_len].buf_max * streams[streams_len].ds_size) * sizeof(int32_t);
	        char name[40] = "/sv_subscriber.";
          strncat(name, lua_tostring(L, -2), 25);
	        int fd = shm_open(name, oflags, 0644 );
          if (fd < 1) return 1; /* error */
	        ftruncate(fd, length);
          int32_t *ptr = (int32_t *) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
          memset(ptr, 0, length); /* init all with 0 */
          streams[streams_len].pos = &ptr[0];
          streams[streams_len].data = &ptr[1];

          printf("stream name: %s, dataSet size: %d\n", name, streams[streams_len].ds_size);
          
          /* create subscriber, link callback and attach to receiver */
          subs[streams_len] = SVSubscriber_create(NULL, streams[streams_len].AppId);
          SVSubscriber_setListener(subs[streams_len], svUpdateListener, &streams[streams_len]);
          SVReceiver_addSubscriber(SV_receiver, subs[streams_len]);

          streams_len++;
			  }
      }
			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(L, 1);
		}
	}
  else {
    printf("Error in config. file: missing 'sampled_values' table");
    return 1; /* error */
  }
	lua_pop(L, 1);
  lua_close(L);

  /* Start listening to SV messages - starts a new receiver background thread */
  SVReceiver_start(SV_receiver);

  if (SVReceiver_isRunning(SV_receiver)) {
    signal(SIGINT, sigint_handler);

    while (running){ /* infinite loop */
         Thread_sleep(1000);
    }
    /* Stop listening to SV messages */
    SVReceiver_stop(SV_receiver);
  }
  else {
    printf("Failed to start SV subscriber. Reason can be that the Ethernet interface doesn't exist or root permission are required.\n");
  }
  
  /* Cleanup and free resources */
  SVReceiver_destroy(SV_receiver);
  //munmap(ptr, length); // needed ?
  //close(fd);
  //shm_unlink(name); //needed ?

  return 0;
}
