
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int sample_idx; 
    int value;      
} DigitalEvent;

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

int main(void) {
    int i, s;
    int num_analog = 3;  /* Combinando com o config_canais.lua */
    int num_digital = 2; /* Combinando com o config_canais.lua */
    int total_samples = 1000;
    double sample_rate = 1000.0;

    /* Aloca e preenche canais Analógicos (VA, VB, VC) */
    double **analog_matrix = (double **)malloc(num_analog * sizeof(double *));
    for (i = 0; i < num_analog; i++) {
        analog_matrix[i] = (double *)malloc(total_samples * sizeof(double));
        double phase = i * (2.0 * M_PI / 3.0);
        for (s = 0; s < total_samples; s++) {
            analog_matrix[i][s] = 120.0 * sin(2.0 * M_PI * 60.0 * ((double)s / sample_rate) - phase);
        }
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

    lua_State *L = luaL_newstate();
    if (L == NULL) return 1;
    luaL_openlibs(L);

    /* 1. Executa ANTES o arquivo de configuração para injetar a tabela 'canais_config' */
    printf("Carregando mapa de canais do arquivo externo...\n");
    if (luaL_dofile(L, "config_canais.lua") != LUA_OK) {
        fprintf(stderr, "Aviso: Nao foi possivel ler 'config_canais.lua'. Usando nomes padrao. Detalhe: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    /* 2. Carrega a string do engine COMTRADE */
    if (luaL_dostring(L, lua_comtrade_script) != LUA_OK) {
        fprintf(stderr, "Erro ao carregar script de engine Lua: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    /* 3. Exporta */
    c_export_comtrade(L, tempo_disparo, analog_matrix, num_analog, digital_events, num_events_per_channel, num_digital, sample_rate, total_samples);

    for (i = 0; i < num_analog; i++) free(analog_matrix[i]);
    free(analog_matrix);
    for (i = 0; i < num_digital; i++) free(digital_events[i]);
    free(digital_events);
    lua_close(L);

    return 0;
}
