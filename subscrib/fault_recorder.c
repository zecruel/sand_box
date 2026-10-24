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


#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>



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

typedef struct {
    int sample_idx; 
    int value;      
} DigitalEvent;


int streams_len = 0;
struct SV_stream streams[MAX_SV];

lua_State *L;


/* ------------------------------------------------------------------------- */
/* STRING DO SCRIPT LUA (C89) - INTEGRADO COM TABELA EXTERNA DE CONFIGURAÇÃO */
/* ------------------------------------------------------------------------- */
const char* lua_comtrade_script = 
"function salvar_comtrade(timestamp_inicial_ms, analogicos, digitais_transicoes, frequencia_amostragem, total_amostras)\n"
"    local seg_totais = math.floor(timestamp_inicial_ms / 1000)\n"
"    local msec = math.floor(timestamp_inicial_ms % 1000)\n"
"    local t = os.date(\"!*t\", seg_totais)\n"
"    local sufixo_tempo = string.format(\"%04d_%02d_%02d-%02d%02d%02d_%03d\", t.year, t.month, t.day, t.hour, t.min, t.sec, msec)\n"
"    local nome_cfg = sufixo_tempo .. \".cfg\"\n"
"    local nome_dat = sufixo_tempo .. \".dat\"\n"
"    local timestamp_final_ms = timestamp_inicial_ms + ((total_amostras - 1) / frequencia_amostragem) * 1000\n"
"    local t_fim = os.date(\"!*t\", math.floor(timestamp_final_ms / 1000))\n"
"    local msec_fim = math.floor(timestamp_final_ms % 1000)\n"
"    local str_data_ini = string.format(\"%02d/%02d/%04d,%02d:%02d:%02d.%06d\", t.day, t.month, t.year, t.hour, t.min, t.sec, msec * 1000)\n"
"    local str_data_fim = string.format(\"%02d/%02d/%04d,%02d:%02d:%02d.%06d\", t_fim.day, t_fim.month, t_fim.year, t_fim.hour, t_fim.min, t_fim.sec, msec_fim * 1000)\n"
"    local num_analog = #analogicos\n"
"    local num_digital = #digitais_transicoes\n"
"    \n"
"    -- Recupera metadados da tabela de configuracao lida anteriormente ou cria fallback\n"
"    local cfg_ext = canais_config or {}\n"
"    local analog_meta = cfg_ext.analogicos or {}\n"
"    local digital_meta = cfg_ext.digitais or {}\n"
"    \n"
"    local analog_cfg = {}\n"
"    local fatores = {}\n"
"    for i = 1, num_analog do\n"
"        local canal = analogicos[i]\n"
"        local min_val = canal[1] or 0.0\n"
"        local max_val = canal[1] or 0.0\n"
"        for s = 1, total_amostras do\n"
"            local v = canal[s] or 0.0\n"
"            if v < min_val then min_val = v end\n"
"            if v > max_val then max_val = v end\n"
"        end\n"
"        local ganho, offset\n"
"        if max_val == min_val then ganho = 1.0; offset = min_val\n"
"        else ganho = (max_val - min_val) / 65534; offset = (max_val + min_val) / 2 end\n"
"        fatores[i] = {ganho = ganho, offset = offset}\n"
"        \n"
"        local m = analog_meta[i] or {}\n"
"        local nome = m.nome or string.format(\"ChA_%d\", i)\n"
"        local fase = m.fase or \"\"\n"
"        local unidade = m.unidade or \"V\"\n"
"        analog_cfg[i] = string.format(\"%d,%s,%s,,%s,%f,%f,0.0,-32768,32767,1.0,1.0,S\", i, nome, fase, unidade, ganho, offset)\n"
"    end\n"
"    local cfg = {}\n"
"    table.insert(cfg, \"Subestacao Teste,Dispositivo IED,1999\")\n"
"    table.insert(cfg, string.format(\"%d,%dA,%dD\", num_analog + num_digital, num_analog, num_digital))\n"
"    for i = 1, num_analog do table.insert(cfg, analog_cfg[i]) end\n"
"    for i = 1, num_digital do\n"
"        local m = digital_meta[i] or {}\n"
"        local nome = m.nome or string.format(\"ChD_%d\", i)\n"
"        local fase = m.fase or \"\"\n"
"        local ccbm = m.ccbm or \"\"\n"
"        table.insert(cfg, string.format(\"%d,%s,%s,%s,0\", i, nome, fase, ccbm))\n"
"    end\n"
"    table.insert(cfg, \"60\")\n"
"    table.insert(cfg, \"1\")\n"
"    table.insert(cfg, string.format(\"%f,%d\", frequencia_amostragem, total_amostras))\n"
"    table.insert(cfg, str_data_ini)\n"
"    table.insert(cfg, str_data_fim)\n"
"    table.insert(cfg, \"BINARY\")\n"
"    table.insert(cfg, \"1\")\n"
"    local f_cfg = io.open(nome_cfg, \"w\")\n"
"    f_cfg:write(table.concat(cfg, \"\\n\") .. \"\\n\")\n"
"    f_cfg:close()\n"
"    local f_dat = io.open(nome_dat, \"wb\")\n"
"    local estado_digital = {}\n"
"    for d = 1, num_digital do\n"
"        estado_digital[d] = digitais_transicoes[d] or 0\n"
"    end\n"
"    local palavras_digitais = math.ceil(num_digital / 16)\n"
"    for s = 1, total_amostras do\n"
"        for d = 1, num_digital do\n"
"            if digitais_transicoes[d][s] ~= nil then estado_digital[d] = digitais_transicoes[d][s] end\n"
"        end\n"
"        local tempo_offset_us = math.floor(((s - 1) / frequencia_amostragem) * 1000000)\n"
"        f_dat:write(string.pack(\"<I4I4\", s, tempo_offset_us))\n"
"        for a = 1, num_analog do\n"
"            local v_real = analogicos[a][s] or 0.0\n"
"            local f = fatores[a]\n"
"            local v_int = math.floor(((v_real - f.offset) / f.ganho) + 0.5)\n"
"            v_int = math.max(-32768, math.min(32767, v_int))\n"
"            f_dat:write(string.pack(\"<i2\", v_int))\n"
"        end\n"
"        for p = 1, palavras_digitais do\n"
"            local palavra = 0\n"
"            for bit = 0, 15 do\n"
"                local d_idx = (p - 1) * 16 + bit + 1\n"
"                if d_idx <= num_digital and estado_digital[d_idx] == 1 then\n"
"                    palavra = palavra + (1 << bit)\n"
"                end\n"
"            end\n"
"            f_dat:write(string.pack(\"<I2\", palavra))\n"
"        end\n"
"    end\n"
"    f_dat:close()\n"
"    print(\"\\n[LUA] Arquivos COMTRADE salvos com sucesso: \" .. sufixo_tempo .. \".cfg/.dat\")\n"
"end\n";

void c_export_comtrade(lua_State *L, 
                        struct timespec start_time, 
                        double **analog_matrix, int num_analog_channels,
                        DigitalEvent **digital_events, const int *num_events_per_channel, int num_digital_channels,
                        double sample_rate, int total_samples) 
{
    int i, s;
    double timestamp_ms = (double)start_time.tv_sec * 1000.0 + (double)start_time.tv_nsec / 1000000.0;

    lua_getglobal(L, "salvar_comtrade");
    if (!lua_isfunction(L, -1)) {
        fprintf(stderr, "Erro C API: Funcao 'salvar_comtrade' nao encontrada.\n");
        return;
    }

    lua_pushnumber(L, timestamp_ms);

    /* Monta canais Analógicos */
    lua_createtable(L, num_analog_channels, 0); 
    for (i = 0; i < num_analog_channels; i++) {
        lua_createtable(L, total_samples, 0); 
        for (s = 0; s < total_samples; s++) {
            lua_pushnumber(L, analog_matrix[i][s]);
            lua_rawseti(L, -2, s + 1);
        }
        lua_rawseti(L, -2, i + 1);
    }

    /* Monta canais Digitais */
    lua_createtable(L, num_digital_channels, 0);
    for (i = 0; i < num_digital_channels; i++) {
        lua_createtable(L, 0, num_events_per_channel[i]); 
        for (s = 0; s < num_events_per_channel[i]; s++) {
            DigitalEvent ev = digital_events[i][s];
            lua_pushinteger(L, ev.value);
            lua_rawseti(L, -2, ev.sample_idx);
        }
        lua_rawseti(L, -2, i + 1);
    }

    lua_pushnumber(L, sample_rate);
    lua_pushinteger(L, total_samples);

    if (lua_pcall(L, 5, 0, 0) != LUA_OK) {
        fprintf(stderr, "Erro de Execucao no Lua: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

int save_capture(/*struct mg_connection *conn, void *cbdata */) {

  
	for(int i = 0; i < streams_len; i++){
	  if(i > 0) {/*mg_printf(conn, ",");*/}
    int32_t pos = *(streams[i].pos);
	  //mg_printf(conn, "\"%s\":{\"pos\": %d, \"data\":[", streams[i].name, pos);
		for(int j = 0; j < streams[i].ds_size; j++){
	    // if(j > 0) mg_printf(conn, ",");
	    // mg_printf(conn, "[");
      for(int k = 0; k < streams[i].buf_max; k++){
	      // if(k > 0) mg_printf(conn, ",");
        int idx = k * streams[i].ds_size + j;
        int32_t data = streams[i].data[idx];
        // mg_printf(conn, "%d", data);
      }


	    // mg_printf(conn, "]");
    }
    
	  // mg_printf(conn, "]}");
  }
	// mg_printf(conn, "}\r\n");

	return 1;
}


int PostResponser(/*struct mg_connection *conn, void *cbdata*/) {
	long long r_total = 0;
	int r, s;
  int sel_streams[MAX_SV], sel_str_n = 0;
  int ds_idx[MAX_SV][MAX_DS], ds_idx_n[MAX_SV];

	char buf[2048];
  memset(buf, 0, 2048);

  /*
	while (r > 0) {
		r_total += r;
		
    printf("%s\n", buf);
		r = mg_read(conn, buf, sizeof(buf));
	}
	*/
	luaL_dostring(L, buf);
	lua_getglobal(L, "sel_channels");
	if (lua_istable(L, -1)){
		
		/* iterate over table */
		lua_pushnil(L);  /* first key */
		while (lua_next(L, -2) != 0) { /* table index are shifted*/
			/* uses 'key' (at index -2) and 'value' (at index -1) */
      const char *str_name = lua_tostring(L, -2);
      int str_idx = -1;
			for(int i = 0; i < streams_len; i++){
        if(strcmp(str_name, streams[i].name) == 0 && sel_str_n < MAX_SV - 1) {
          sel_streams[sel_str_n] = i;
          ds_idx_n[sel_str_n] = 0;
          str_idx = sel_str_n;
          sel_str_n++;
          break;
        }
      }
      if (str_idx > -1) {
			  if (lua_istable(L, -1) && sel_str_n < MAX_SV){
          int len = lua_rawlen(L, -1);
          ds_idx_n[str_idx] = len;
          for(int i = 0; i < len; i++){
            lua_rawgeti(L, -1, i + 1);
            ds_idx[str_idx][i] = lua_tointeger(L, -1);
            lua_pop(L, 1);
          }
        }
      }
    lua_pop(L, 1);
    }
  }
  lua_pop(L, 1);
 

  
  
	for(int i = 0; i < sel_str_n; i++){
    int sel = sel_streams[i];
	  // if(i > 0) mg_printf(conn, ",");
    int32_t pos = *(streams[sel].pos);
    int rem = pos % 80;
	  // mg_printf(conn, "\"%s\":[", streams[sel].name);
		for(int j = 0; j < ds_idx_n[i]; j++){
	    // if(j > 0) mg_printf(conn, ",");
	    // mg_printf(conn, "[");
      for(int k = 0; k <= 240; k++){
	      // if(k > 0) mg_printf(conn, ",");
        int buf_pos = pos + k - 300 - rem;
        if (buf_pos < 0) buf_pos += streams[sel].buf_max;
        if (!(buf_pos < streams[sel].buf_max)) buf_pos -= streams[sel].buf_max;
        int idx = buf_pos * streams[sel].ds_size + ds_idx[i][j];
        int32_t data = streams[sel].data[idx];
        // mg_printf(conn, "%d", data);
      }


	    // mg_printf(conn, "]");
    }
    
	  // mg_printf(conn, "]");
  }
	// mg_printf(conn, "}\r\n");

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
  

    /* 2. Carrega a string do engine COMTRADE */
    if (luaL_dostring(L, lua_comtrade_script) != LUA_OK) {
        fprintf(stderr, "Erro ao carregar script de engine Lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

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


	// while (!exitNow) {
		sleep(1);
	// }

    int i, j, k, s;
    int num_analog = 8;  /* Combinando com o config_canais.lua */
    int num_digital = 2; /* Combinando com o config_canais.lua */
    int total_samples = 9600;
    double sample_rate = 4800.0;

    /* Aloca e preenche canais Analógicos (VA, VB, VC) */
    double **analog_matrix = (double **)malloc(num_analog * sizeof(double *));
    for (i = 0; i < num_analog; i++) {
        analog_matrix[i] = (double *)malloc(total_samples * sizeof(double));
        // double phase = i * (2.0 * M_PI / 3.0);
        // for (s = 0; s < total_samples; s++) {
        //     analog_matrix[i][s] = 120.0 * sin(2.0 * M_PI * 60.0 * ((double)s / sample_rate) - phase);
        // }
    }

	for(int i = 0; i < streams_len; i++){
	  if(i > 0) {/*mg_printf(conn, ",");*/}
    int32_t pos = *(streams[i].pos);
	  //mg_printf(conn, "\"%s\":{\"pos\": %d, \"data\":[", streams[i].name, pos);
		for(int j = 0; j < streams[i].ds_size; j++){
	    // if(j > 0) mg_printf(conn, ",");
	    // mg_printf(conn, "[");
      for(int k = 0; k < streams[i].buf_max; k++){
	      // if(k > 0) mg_printf(conn, ",");
        int buf_pos = pos + k;
        if (buf_pos < 0) buf_pos += streams[i].buf_max;
        if (!(buf_pos < streams[i].buf_max)) buf_pos -= streams[i].buf_max;
        int idx = buf_pos * streams[i].ds_size + j;
        // int32_t data = streams[i].data[idx];
        analog_matrix[j][k] = 0.001 * streams[i].data[idx];
        // mg_printf(conn, "%d", data);
      }


	    // mg_printf(conn, "]");
    }
    
	  // mg_printf(conn, "]}");
  }
    /* Configura Eventos Digitais Esparsos */
    int num_events_per_channel[] = { 2, 2 }; 
    DigitalEvent **digital_events = (DigitalEvent **)malloc(num_digital * sizeof(DigitalEvent *));
    
    digital_events[0] = (DigitalEvent *)malloc(2 * sizeof(DigitalEvent));
    digital_events[0][0].sample_idx = 1;   digital_events[0][0].value = 0;
    digital_events[0][1].sample_idx = 450; digital_events[0][1].value = 1;

    digital_events[1] = (DigitalEvent *)malloc(2 * sizeof(DigitalEvent));
    digital_events[1][0].sample_idx = 1;   digital_events[1][0].value = 1;
    digital_events[1][1].sample_idx = 600; digital_events[1][1].value = 0;

    struct timespec tempo_disparo;
    tempo_disparo.tv_sec = 1774831200; 
    tempo_disparo.tv_nsec = 0;
/* Captura o tempo real da máquina em sistemas POSIX (Linux/macOS) */
if (clock_gettime(CLOCK_REALTIME, &tempo_disparo) == -1) {
tempo_disparo.tv_sec = time(NULL);
tempo_disparo.tv_nsec = 0;
}
    /* 1. Executa ANTES o arquivo de configuração para injetar a tabela 'canais_config' */
    printf("Carregando mapa de canais do arquivo externo...\n");
    if (luaL_dofile(L, "config_canais.lua") != LUA_OK) {
        fprintf(stderr, "Aviso: Nao foi possivel ler 'config_canais.lua'. Usando nomes padrao. Detalhe: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
    /* 3. Exporta */
    c_export_comtrade(L, tempo_disparo, analog_matrix, num_analog, digital_events, num_events_per_channel, num_digital, sample_rate, total_samples);

    for (i = 0; i < num_analog; i++) free(analog_matrix[i]);
    free(analog_matrix);
    for (i = 0; i < num_digital; i++) free(digital_events[i]);
    free(digital_events);
	/* Stop the server */
  lua_close(L);
	printf("Server stopped.\n");
	printf("Bye!\n");

	return EXIT_SUCCESS;
}
