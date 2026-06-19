/*

compile:
gcc emb_server.c civetweb.c -I. -Iinclude -Llib -DNO_SSL -llua -lm -o emb_server
 * Copyright (c) 2013-2021 the CivetWeb developers
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

/* Note: This example omits some error checking and input validation for a
 * better clarity/readability of the code. Example codes undergo less quality
 * management than the main source files of this project. */


/* Simple example program on how to use CivetWeb embedded into a C program. */
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
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

#define MAX_SV 100
#define MAX_DS 20


struct SV_stream{
  char name[25];
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

int streams_len = 0;
struct SV_stream streams[MAX_SV];

lua_State *L;

int ExitHandler(struct mg_connection *conn, void *cbdata) {
	mg_printf(conn,
	          "HTTP/1.1 200 OK\r\nContent-Type: "
	          "text/plain\r\nConnection: close\r\n\r\n");
	mg_printf(conn, "Server will shut down.\n");
	mg_printf(conn, "Bye!\n");
	exitNow = 1;
	return 1;
}

int FileHandler(struct mg_connection *conn, void *cbdata) {
	/* In this handler, we ignore the req_info and send the file "fileName". */
	const char *fileName = (const char *)cbdata;

	mg_send_file(conn, fileName);
	return 1;
}

int PostResponser(struct mg_connection *conn, void *cbdata) {
	long long r_total = 0;
	int r, s;

  int32_t data[240];
  int32_t pos = *(streams[0].pos);

  for(int i = 0; i < 240; i++){
    int buf_pos = pos + i - 300;
    if (buf_pos < 0) pos += streams[0].buf_max;
    if (!(buf_pos < streams[0].buf_max)) pos -= streams[0].buf_max;
    int ch_idx = 0;
    int idx = buf_pos * streams[0].ds_size + ch_idx;
    data[i] = streams[0].data[idx];
  }

	char buf[2048];
  memset(buf, 0, 2048);

	const struct mg_request_info *ri = mg_get_request_info(conn);

	if (0 != strcmp(ri->request_method, "POST")) {
		/* Not a POST request */
		int ret = mg_get_request_link(conn, buf, sizeof(buf));

		mg_printf(conn,
		          "HTTP/1.1 405 Method Not Allowed\r\nConnection: close\r\n");
		mg_printf(conn, "Content-Type: text/plain\r\n\r\n");
		mg_printf(conn,
		          "%s method not allowed in the POST handler\n",
		          ri->request_method);
		if (ret >= 0) {
			mg_printf(conn,
			          "use a web tool to send a POST request to %s\n",
			          buf);
		}
		return 1;
	}

	if (ri->content_length >= 0) {
		/* We know the content length in advance */
    //printf ("ri->content_length=%d\n", ri->content_length);
	} else {
		/* We must read until we find the end (chunked encoding
		 * or connection close), indicated my mg_read returning 0 */
	}
  
	r = mg_read(conn, buf, sizeof(buf));
  int pass = 0;
  if (r > 0) pass = atoi(buf);
  
  /*
	while (r > 0) {
		r_total += r;
		
    printf("%s\n", buf);
		r = mg_read(conn, buf, sizeof(buf));
	}
	*/
  
  mg_printf(conn, "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "Content-Type: Content-Type: application/json\r\n\r\n");
	mg_printf(conn, "[");
  mg_printf(conn, "[%g,%g]", 0.0, 0.01 * data[0]);
  for (int i = 1; i < 240; i++){
    mg_printf(conn, ",[%g,%g]", i * 0.00020833, 0.01 * data[i]);
  }
	mg_printf(conn, "]\r\n");
  
	return 1;
}

int get_config(struct mg_connection *conn, void *cbdata) {
  lua_getglobal(L, "get_streams");
  lua_newtable(L);
  for(int i = 0; i < streams_len; i++){
    lua_pushstring(L, streams[i].name);
    lua_rawseti(L, -2, i+1);
  }
  int r = lua_pcall(L, 1, 1, 0);
  if (r == LUA_OK) {
	  mg_printf(conn, "HTTP/1.1 200 OK\r\n");
    mg_printf(conn, "Content-Type: Content-Type: application/json\r\n\r\n");
    mg_printf(conn, "%s", lua_tostring(L, -1));
    mg_printf(conn, "\r\n");
    lua_pop(L, 1);
  }
  
	return 1;
}

int get_ch_data(struct mg_connection *conn, void *cbdata) {

  int32_t data[240];
  int32_t pos = *(streams[0].pos);

  for(int i = 0; i < 240; i++){
    int buf_pos = pos + i - 300;
    if (buf_pos < 0) pos += streams[0].buf_max;
    if (!(buf_pos < streams[0].buf_max)) pos -= streams[0].buf_max;
    int ch_idx = 0;
    int idx = buf_pos * streams[0].ds_size + ch_idx;
    data[i] = streams[0].data[idx];
  }

	  mg_printf(conn, "HTTP/1.1 200 OK\r\n");
	mg_printf(conn, "Content-Type: Content-Type: application/json\r\n\r\n");
	mg_printf(conn, "[");
  mg_printf(conn, "[%g,%g]", 0.0, 0.01 * data[0]);
  for (int i = 1; i < 240; i++){
    mg_printf(conn, ",[%g,%g]", i * 0.00020833, 0.01 * data[i]);
  }
	mg_printf(conn, "]\r\n");
  
	return 1;
}


int log_message(const struct mg_connection *conn, const char *message) {
	puts(message);
	return 1;
}

/* Aux function for qsort */
int comp_asc(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return (val_a > val_b) - (val_a < val_b); /* avoid overflow */
}

int main(int argc, char *argv[]) {
  char *config_file;
  
  if (argc > 1) {
    printf("Config file: %s\n", argv[1]);
    config_file = argv[1];
  }
  else {
    printf("Using default config file - sv_config.lua\n");
    config_file = "sv_config.lua";
  }
 
  /* Start Lua interpreter */
  L = luaL_newstate(); /* opens Lua */
  luaL_openlibs(L); /* opens the standard libraries */
  const char *lfuncs =
    "function table_to_json(tbl)\n"
    "  local result = {}\n"
    "  for key, value in pairs(tbl) do\n"
    "    local formatted_value = type(value) == 'table' and "
    "table_to_json(value) or string.format('\\\"%s\\\"', tostring(value))\n"
    "    table.insert(result, string.format('\\\"%s\\\":%s', key, formatted_value))\n"
    "  end\n"
    "  return '{' .. table.concat(result, ',') .. '}'\n"
    "end\n"

    "function get_streams (tbl)\n"
    "  local result = {}\n"
    "  for key, value in pairs(tbl) do\n"
    "    result[value] = sampled_values[value]\n"
    "  end\n"
    "  return table_to_json(result)\n"
    "end\n";
		luaL_dostring(L, lfuncs);
  

  /* -------------------- parse config file  -------------------*/
  /* execute Lua config file */
  if (luaL_loadfile(L, config_file) || lua_pcall(L, 0, 0, 0)){
    printf("cannot run config. file: %s", lua_tostring(L, -1));
    return 1; /* error */
  }
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
          int oflags = O_RDONLY;
	        int length = (1 + streams[streams_len].buf_max * streams[streams_len].ds_size) * sizeof(int32_t);
	        char name[40] = "/sv_subscriber.";
          memset(streams[streams_len].name, 0, 25);
          strncpy(streams[streams_len].name, lua_tostring(L, -2), 25);
          strncat(name, streams[streams_len].name, 25);
	        int fd = shm_open(name, oflags, 0644 );
          if (fd >= 0) {
            int32_t *ptr = (int32_t *) mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
            streams[streams_len].pos = &ptr[0];
            streams[streams_len].data = &ptr[1];

            printf("stream name: %s, dataSet size: %d\n", name, streams[streams_len].ds_size);
            
            streams_len++;
          }
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
  printf("top stack = %d\n", lua_gettop(L));





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
	mg_set_request_handler(ctx, "/postresponse", PostResponser, 0);
	mg_set_request_handler(ctx, "/getchdata", get_ch_data, 0);
	mg_set_request_handler(ctx, "/getconfig", get_config, 0);

	
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

	return EXIT_SUCCESS;
}
