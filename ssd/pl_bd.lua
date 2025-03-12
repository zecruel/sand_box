leitor = require 'xlsx_lua'

path = "61850_cemig.xlsm"

planilha = leitor.open(path)

bd = sqlite.open('61850_cemig2.db')

-- limpa o banco de dados
bd:exec('DROP TABLE IF EXISTS geral')
bd:exec('CREATE TABLE geral('..
  'ld TEXT, pre TEXT, ln TEXT, inst INTEGER, d_o TEXT, d_a TEXT, ' ..
  'uso TEXT, goose INTEGER, descr TEXT, obs TEXT)')

for nome_aba, aba in pairs(planilha.sheets) do
	cadzinho.db_print (("aba: %s, index = %s"):format(nome_aba, aba.idx))
	--expande as celulas mescladas
  leitor.expand_merge (aba)

	for _, r in ipairs(aba.dim.rows) do
    if r > 1 and aba.data[r]['B'] then
      pre = 'NULL'
      inst = 'NULL'
      d_o = 'NULL'
      d_a = 'NULL'
      uso = 'NULL'
      goose = 'NULL'
      descr = 'NULL'
      obs = 'NULL'
      
      if aba.data[r]['A'] then pre = "'".. tostring(aba.data[r]['A']) .. "'" end
      if aba.data[r]['C'] then inst = tonumber(aba.data[r]['C']) end
      if aba.data[r]['D'] then d_o = "'".. tostring(aba.data[r]['D']) .. "'" end
      if aba.data[r]['E'] then d_a = "'".. tostring(aba.data[r]['E']) .. "'" end
      if aba.data[r]['F'] then uso = "'".. tostring(aba.data[r]['F']) .. "'" end
      if aba.data[r]['G'] then goose = '1' end
      if aba.data[r]['H'] then descr = "'".. tostring(aba.data[r]['H']) .. "'" end
      if aba.data[r]['I'] then obs = "'".. tostring(aba.data[r]['I']) .. "'" end
      
      bd:exec ("INSERT INTO geral VALUES('".. tostring(nome_aba) .."', "..
        pre ..", '".. tostring(aba.data[r]['B']) .."', "..
        inst ..", ".. d_o ..", ".. d_a  ..", "..
        uso ..", ".. goose ..", ".. descr ..", ".. obs ..");")
      
      --[[
      str = ""
      for _, c in ipairs(aba.dim.cols) do
        str = str .. tostring(aba.data[r][c]) .. "    "
      end
      print (str)
      ]]--
    end
	end
	
end

bd:close()

--[[
create table log_nodes (ln TEXT, d_o TEXT, tipo text, descr text, obs text);

WITH cte_ln AS (
    SELECT DISTINCT ln, d_o
             FROM geral
             ORDER BY ln, d_o
)
SELECT DISTINCT ln, "Beh" as d_o, "ENS" as tipo, 'Status do modo (N/T/B)' as descr  FROM cte_ln
union all SELECT ln, d_o, CASE d_o WHEN 'Str' then 'ACD' 
WHEN 'Loc' then 'SPS' WHEN 'LocKey' then 'SPS'  WHEN 'Pos' then 'DPC' 
WHEN 'OpCnt' then 'INS' WHEN 'Auto' then 'SPC' 
WHEN 'BlkOpn' then 'SPC' WHEN 'BlkCls' then 'SPC'
WHEN 'EnaOpn' then 'SPS' WHEN 'EnaCls' then 'SPS' 
when 'Op' then 'ACT' end as tipo,
CASE d_o WHEN 'Str' then 'Partida' 
WHEN 'Loc' then 'Chave local/remoto' WHEN 'LocKey' then 'Chave local/remoto'   WHEN 'Pos' then 'Aberto/Fechado' 
WHEN 'OpCnt' then 'Contador de operacoes' WHEN 'Auto' then 'Automatico/Manual' 
WHEN 'BlkOpn' then 'Bloqueio abertura' WHEN 'BlkCls' then 'Bloqueio fechamento'
WHEN 'EnaOpn' then 'Habilita abertura' WHEN 'EnaCls' then 'Habilita fechamento' 
when 'Op' then 'Trip' 
else (select descr from geral where geral.ln = cte_ln.ln and geral.d_o = cte_ln.d_o limit 1)
end as descr 
FROM cte_ln ORDER BY ln
]]--