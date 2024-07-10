-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8

function bd_novo(caminho, projeto)
  local bd = sqlite.open(caminho)
  local t = os.time()
  local jd = 2440587.5 + t / 86400
  if not bd then return nil end
  
  if type(projeto) == 'table' then
    bd:exec ('DROP TABLE IF EXISTS projeto')
    bd:exec ('CREATE TABLE projeto(chave TEXT, valor TEXT)')
    bd:exec ("INSERT INTO projeto VALUES('titulo', '" .. string.gsub(projeto.titulo, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('instalacao', '" .. string.gsub(projeto.instalacao, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('aplicacao', '" .. string.gsub(projeto.aplicacao, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('codigo', '" .. string.gsub(projeto.codigo, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('rev', '" .. string.gsub(projeto.rev, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('aprovacao', '" .. string.gsub(projeto.aprovacao, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('visto', '" .. string.gsub(projeto.visto, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('projetista', '" .. string.gsub(projeto.projetista, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('descr', '" .. string.gsub(projeto.descr, '\\', '\\\\') .. "');")
    bd:exec ("INSERT INTO projeto VALUES('data', '" .. string.gsub(projeto.data, '\\', '\\\\') .. "');")
  end
  
  bd:exec('DROP TABLE IF EXISTS arquivos')
  bd:exec('CREATE TABLE arquivos('..
    'caminho TEXT, modificado INTEGER, jd REAL, guid TEXT)')
  bd:exec ("INSERT INTO arquivos VALUES('projeto.db', "..
      string.format('%d', t) .. ", " .. string.format('%f', jd) .. ", NULL);")
  bd:exec('DROP TABLE IF EXISTS paineis')
  bd:exec('CREATE TABLE paineis('..
    'id TEXT, titulo TEXT, descr TEXT, fiacao INTEGER, x REAL, y REAL)')
  bd:exec('DROP TABLE IF EXISTS lista_equip')
  bd:exec('CREATE TABLE lista_equip('..
    'item TEXT, descr TEXT, modelo TEXT, fabr TEXT)')
  bd:exec('DROP TABLE IF EXISTS tipico_term')
  bd:exec('CREATE TABLE tipico_term('..
    'item TEXT, el_id INTEGER, elemento TEXT, ' ..
    't_id INTEGER, term TEXT, modulo TEXT, fiacao INTEGER)')
  bd:exec('DROP TABLE IF EXISTS regras_equip')
  bd:exec('CREATE TABLE regras_equip('..
    'tipo TEXT, item TEXT)')
  bd:exec('DROP TABLE IF EXISTS componentes')
  bd:exec('CREATE TABLE componentes('..
    'painel TEXT, id TEXT, tipo TEXT, item TEXT, id_fiacao TEXT)')
  bd:exec('DROP TABLE IF EXISTS componentes_esq')
  bd:exec('CREATE TABLE componentes_esq('..
    'unico INTEGER, tipo TEXT, '..
    'bloco TEXT, id TEXT, sub TEXT, pai INTEGER, x REAL, y REAL, arquivo TEXT)')
  bd:exec('DROP TABLE IF EXISTS caixas')
  bd:exec('CREATE TABLE caixas('..
    'unico INTEGER, '..
    'id TEXT, tipo TEXT, pai INTEGER, arquivo TEXT)')
  bd:exec('DROP TABLE IF EXISTS terminais_esq')
  bd:exec('CREATE TABLE terminais_esq('..
    'componente INTEGER, '..
    'id INTEGER, terminal TEXT)')
  bd:exec('DROP TABLE IF EXISTS barras_esq')
  bd:exec('CREATE TABLE barras_esq('..
    'id TEXT, '..
    'componente INTEGER, '..
    'terminal INTEGER)')
  bd:exec('DROP TABLE IF EXISTS desenhos')
  bd:exec('CREATE TABLE desenhos('..
    'unico INTEGER, '..
    'ident TEXT, titulo TEXT, tipo TEXT, projeto TEXT, '..
    'rev TEXT, versao TEXT, fl TEXT, data TEXT, ' ..
    'aplic TEXT, instal TEXT, visto TEXT, aprov TEXT,' ..
    'classif TEXT, pfl TEXT)')
  bd:exec('DROP TABLE IF EXISTS engates_esq')
  bd:exec('CREATE TABLE engates_esq('..
    'unico INTEGER, '..
    'engate TEXT)')
  bd:exec('DROP TABLE IF EXISTS referencias_esq')
  bd:exec('CREATE TABLE referencias_esq('..
    'unico INTEGER, '..
    'terminal TEXT, elemento TEXT)')
  bd:exec('DROP VIEW IF EXISTS hierarquia_esq')
  bd:exec("CREATE VIEW hierarquia_esq AS\n"..
    "SELECT componentes_esq.unico componente,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes_esq.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'COMPONENTE'\n"..
    ") pai,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes_esq.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'MODULO'\n"..
    ") modulo,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes_esq.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'PAINEL'\n"..
    ") painel,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes_esq.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'DESENHO'\n"..
    ") desenho,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes_esq.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'DESCRITIVO'\n"..
    ") descritivo\n"..
    "FROM componentes_esq\n")
  bd:exec('DROP VIEW IF EXISTS comp_term')
  bd:exec("CREATE VIEW comp_term AS\n"..
    "SELECT componentes_esq.unico,\n"..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.painel) painel,\n"..
    "(SELECT CASE WHEN hierarquia_esq.pai THEN \n"..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.pai)\n"..
    "ELSE componentes_esq.id END) componente,\n"..
    "CASE WHEN hierarquia_esq.modulo THEN \n"..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.modulo)\n"..
    "WHEN componentes_esq.sub THEN componentes_esq.sub end  modulo,\n"..
    "CASE WHEN hierarquia_esq.pai THEN componentes_esq.id END parte,\n"..
    "componentes_esq.bloco, componentes_esq.tipo, terminais_esq.id num, terminais_esq.terminal \n"..
    "FROM componentes_esq, hierarquia_esq\n"..
    "INNER JOIN terminais_esq ON terminais_esq.componente = componentes_esq.unico\n"..
    "WHERE componentes_esq.unico = hierarquia_esq.componente\n"..
    "AND NOT componentes_esq.tipo = 'ENGATE'\n"..
    "AND NOT componentes_esq.tipo = 'REFERENCIA'\n"..
    "ORDER BY painel ASC, componente ASC, modulo ASC, tipo ASC, hierarquia_esq.desenho ASC,\n"..
    "componentes_esq.x ASC, componentes_esq.y ASC, num ASC;")
  bd:exec('DROP VIEW IF EXISTS eng_term')
  bd:exec("CREATE VIEW eng_term AS\n"..
    "SELECT engates_esq.unico, engates_esq.engate, barras_esq.id barra, "..
    "terminais_esq.terminal, hierarquia_esq.desenho\n"..
    "FROM engates_esq, barras_esq, hierarquia_esq\n"..
    "INNER JOIN terminais_esq ON terminais_esq.componente = engates_esq.unico\n"..
    "WHERE engates_esq.unico = barras_esq.componente AND terminais_esq.id = barras_esq.terminal "..
    "AND engates_esq.unico = hierarquia_esq.componente\n"..
    "ORDER BY engates_esq.engate ASC, terminais_esq.terminal ASC;\n")
  bd:exec('DROP VIEW IF EXISTS eng_repetidos')
  bd:exec("CREATE VIEW eng_repetidos AS\n".. 
   "WITH cte_barras_rep AS (SELECT eng_term.barra barra,\n"..
   "count(eng_term.barra) quant FROM eng_term\n"..
   "GROUP BY eng_term.barra HAVING quant > 1)\n"..
   "SELECT eng_term.engate || '_' || eng_term.terminal node,\n"..
   "cte_barras_rep.barra FROM eng_term, cte_barras_rep\n"..
   "WHERE cte_barras_rep.barra = eng_term.barra\n"..
   "ORDER BY node ASC;")
  bd:exec('DROP VIEW IF EXISTS barra_consol')
  bd:exec("CREATE VIEW barra_consol AS\n".. 
    "SELECT CASE WHEN EXISTS (SELECT eng_term.barra\n"..
    "FROM eng_term WHERE barras_esq.id = eng_term.barra)\n"..
    "THEN (SELECT CASE WHEN EXISTS (\n"..
    "SELECT eng_repetidos.node FROM eng_repetidos\n"..
    "where eng_term.engate || '_' || eng_term.terminal = eng_repetidos.node)\n"..
    "THEN (SELECT eng_repetidos.barra FROM eng_repetidos \n"..
    "WHERE eng_term.engate || '_' || eng_term.terminal = eng_repetidos.node\n"..
    "LIMIT 1) ELSE eng_term.engate || '_' || eng_term.terminal END\n"..
    "FROM eng_term WHERE barras_esq.id = eng_term.barra)\n"..
    "ELSE barras_esq.id END\n"..
    "barra, componentes_esq.unico componente, barras_esq.terminal, hierarquia_esq.painel, "..
    "caixas.id nome_painel\n"..
    "FROM barras_esq, componentes_esq, hierarquia_esq, caixas\n"..
    "WHERE componentes_esq.unico = barras_esq.componente AND \n"..
    "NOT componentes_esq.tipo = 'ENGATE' AND barras_esq.componente = hierarquia_esq.componente\n"..
    "AND NOT componentes_esq.tipo = 'REFERENCIA'\n"..
    "AND hierarquia_esq.painel = caixas.unico\n"..
    "ORDER BY barra ASC\n")
  bd:exec('DROP VIEW IF EXISTS fiacao_interna')
  bd:exec("CREATE VIEW fiacao_interna AS\n"..
    "SELECT barra_consol.barra,\n"..
    "barra_consol.componente elemento,\n"..
    "barra_consol.terminal term,\n"..
    "LAG(barra_consol.componente) OVER (PARTITION BY barra_consol.nome_painel,\n"..
    "barra_consol.barra ORDER BY barra_consol.barra) comp_ant,\n"..
    "LAG(barra_consol.terminal) OVER (PARTITION BY barra_consol.nome_painel,\n"..
    "barra_consol.barra ORDER BY barra_consol.barra) term_ant,\n"..
    "LEAD(barra_consol.componente) OVER (PARTITION BY barra_consol.nome_painel,\n"..
    "barra_consol.barra ORDER BY barra_consol.barra) comp_pos,\n"..
    "LEAD(barra_consol.terminal) OVER (PARTITION BY barra_consol.nome_painel,\n"..
    "barra_consol.barra ORDER BY barra) term_pos,\n"..
    "barra_consol.nome_painel painel\n"..
    "FROM barra_consol\n"..
    "ORDER BY barra_consol.nome_painel,\n"..
    "barra_consol.barra\n")
  bd:exec('DROP VIEW IF EXISTS fiacao_tabela')
  bd:exec("CREATE VIEW fiacao_tabela AS\n"..
    "SELECT ( SELECT comp_term.componente FROM comp_term\n" ..
    "WHERE comp_term.unico = fiacao_interna.elemento) componente,\n" ..
    "(SELECT comp_term.terminal FROM comp_term\n" ..
    "WHERE comp_term.num = fiacao_interna.term AND\n" ..
    "comp_term.unico = fiacao_interna.elemento) terminal,\n" ..
    "CASE WHEN fiacao_interna.comp_ant AND fiacao_interna.comp_pos\n" ..
    "THEN ( SELECT comp_term.componente FROM comp_term\n" ..
    "WHERE comp_term.unico = fiacao_interna.comp_ant)\n" ..
    "|| '.' || ( SELECT comp_term.terminal\n" ..
    "FROM comp_term WHERE comp_term.num = fiacao_interna.term_ant\n" ..
    "AND comp_term.unico = fiacao_interna.comp_ant )\n" ..
    "|| '  /  ' || ( SELECT comp_term.componente FROM comp_term\n" ..
    "WHERE comp_term.unico = fiacao_interna.comp_pos)\n" ..
    "|| '.' || ( SELECT comp_term.terminal FROM comp_term\n" ..
    "WHERE comp_term.num = fiacao_interna.term_pos AND\n" ..
    "comp_term.unico = fiacao_interna.comp_pos)\n" ..
    "WHEN fiacao_interna.comp_ant THEN (\n" ..
    "SELECT comp_term.componente FROM comp_term\n" ..
    "WHERE comp_term.unico = fiacao_interna.comp_ant)\n" ..
    "|| '.' || (SELECT comp_term.terminal FROM comp_term\n" ..
    "WHERE comp_term.num = fiacao_interna.term_ant AND\n" ..
    "comp_term.unico = fiacao_interna.comp_ant)\n" ..
    "WHEN fiacao_interna.comp_pos THEN (\n" ..
    "SELECT comp_term.componente FROM comp_term\n" ..
    "WHERE comp_term.unico = fiacao_interna.comp_pos)\n" ..
    "|| '.' || ( SELECT comp_term.terminal FROM comp_term\n" ..
    "WHERE comp_term.num = fiacao_interna.term_pos AND\n" ..
    "comp_term.unico = fiacao_interna.comp_pos) END ligacao,\n" ..
    "fiacao_interna.painel FROM fiacao_interna\n" ..
    "WHERE fiacao_interna.comp_ant OR  fiacao_interna.comp_pos\n" ..
    "ORDER BY painel ASC, componente ASC, terminal ASC;")
  bd:exec('DROP VIEW IF EXISTS interlig')
  bd:exec("CREATE VIEW interlig AS\n"..
    "WITH cte_interlig AS (\n" ..
    "SELECT DISTINCT barra_consol.barra,\n" ..
    "barra_consol.nome_painel painel\n" ..
    "FROM (SELECT barra_consol.barra barra,\n" ..
    "count(DISTINCT barra_consol.nome_painel) quant\n" ..
    "FROM barra_consol GROUP BY barra_consol.barra\n" ..
    "HAVING quant > 1) passante, barra_consol\n" ..
    "WHERE barra_consol.barra = passante.barra\n" ..
    "ORDER BY barra_consol.barra, barra_consol.nome_painel)\n" ..
    "SELECT interlig1.barra, interlig1.painel p1,\n" ..
    "(SELECT comp_term.componente\n" ..
    "FROM componentes_esq, hierarquia_esq, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes_esq.unico = barra_consol.componente AND\n" ..
    "componentes_esq.unico = comp_term.unico AND\n" ..
    "barra_consol.barra = interlig1.barra AND\n" ..
    "(componentes_esq.tipo = 'BORNE' OR\n" ..
    "componentes_esq.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig1.painel AND\n" ..
    "hierarquia_esq.componente = componentes_esq.unico AND\n" ..
    "caixas.unico = hierarquia_esq.painel) borne_p1,\n" ..
    "(SELECT comp_term.terminal\n" ..
    "FROM componentes_esq, hierarquia_esq, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes_esq.unico = barra_consol.componente AND\n" ..
    "componentes_esq.unico = comp_term.unico AND\n" ..
    "barra_consol.terminal = comp_term.num AND\n" ..
    "barra_consol.barra = interlig1.barra AND\n" ..
    "(componentes_esq.tipo = 'BORNE' OR\n" ..
    "componentes_esq.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig1.painel AND\n" ..
    "hierarquia_esq.componente = componentes_esq.unico AND\n" ..
    "caixas.unico = hierarquia_esq.painel) term_p1,\n" ..
    "interlig2.painel p2,\n" ..
    "(SELECT comp_term.componente\n" ..
    "FROM componentes_esq, hierarquia_esq, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes_esq.unico = barra_consol.componente AND\n" ..
    "componentes_esq.unico = comp_term.unico AND\n" ..
    "barra_consol.barra = interlig2.barra AND\n" ..
    "(componentes_esq.tipo = 'BORNE' OR\n" ..
    "componentes_esq.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig2.painel AND\n" ..
    "hierarquia_esq.componente = componentes_esq.unico AND\n" ..
    "caixas.unico = hierarquia_esq.painel) borne_p2,\n" ..
    "(SELECT comp_term.terminal\n" ..
    "FROM componentes_esq, hierarquia_esq,\n" ..
    "barra_consol, caixas, comp_term\n" ..
    "WHERE componentes_esq.unico = barra_consol.componente AND\n" ..
    "componentes_esq.unico = comp_term.unico AND\n" ..
    "barra_consol.terminal = comp_term.num AND\n" ..
    "barra_consol.barra = interlig2.barra AND\n" ..
    "(componentes_esq.tipo = 'BORNE' OR\n" ..
    "componentes_esq.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig2.painel AND\n" ..
    "hierarquia_esq.componente = componentes_esq.unico AND\n" ..
    "caixas.unico = hierarquia_esq.painel)term_p2\n" ..
    "FROM cte_interlig interlig1\n" ..
    "JOIN cte_interlig interlig2 ON\n" ..
    "interlig1.barra = interlig2.barra AND p1 > p2\n"..
    "ORDER BY p1 ASC, p2 ASC;")
  bd:exec('DROP VIEW IF EXISTS engate_par')
  bd:exec("CREATE VIEW engate_par AS\n"..
    "WITH cte_eng AS (select *, ROW_NUMBER() OVER (PARTITION BY desenho\n"..
    "ORDER BY ini, folha) e_num\n"..
    "FROM (WITH cte_engate AS (\n"..
    "SELECT engates_esq.unico unico, engates_esq.engate engate,\n"..
    "(SELECT desenhos.ident FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia_esq.desenho) desenho,\n"..
    "(SELECT desenhos.fl FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia_esq.desenho) folha\n"..
    "FROM engates_esq, hierarquia_esq\n"..
    "WHERE engates_esq.unico = hierarquia_esq.componente\n"..
    "ORDER BY engates_esq.engate, desenho ASC, folha ASC)\n"..
    "SELECT num, engate, unico, desenho, folha,\n"..
    "CASE WHEN prox THEN prox ELSE ant END AS par,\n"..
    "CASE WHEN ant IS NULL THEN 1 WHEN prox THEN 1 ELSE 0 END AS ini\n"..
    "FROM (\n"..
    "SELECT ROW_NUMBER() OVER (PARTITION BY engate\n"..
    "ORDER BY desenho, folha) num,\n"..
    "engate, unico, desenho, folha,\n"..
    "LAG(unico) OVER (PARTITION BY engate ORDER BY engate ASC) ant,\n"..
    "LEAD(unico) OVER (PARTITION BY engate ORDER BY engate ASC) prox\n"..
    "FROM cte_engate)\n"..
    "WHERE num % 2 = 1\n"..
    "UNION\n"..
    "SELECT num, engate, unico, desenho, folha, ant, 0 ini\n"..
    "FROM (\n"..
    "SELECT ROW_NUMBER() OVER (PARTITION BY engate\n"..
    "ORDER BY desenho, folha) num,\n"..
    "engate, unico, desenho, folha,\n"..
    "LAG(unico) OVER (PARTITION BY engate ORDER BY engate ASC) ant,\n"..
    "LEAD(unico) OVER (PARTITION BY engate ORDER BY engate ASC) prox\n"..
    "FROM cte_engate)\n"..
    "WHERE num % 2 = 0\n"..
    "ORDER BY engate, desenho, folha)\n"..
    "WHERE ini = 1\n"..
    "UNION\n"..
    "SELECT *, 0 e_num\n"..
    "FROM (WITH cte_engate AS (\n"..
    "SELECT engates_esq.unico unico, engates_esq.engate engate,\n"..
    "(SELECT desenhos.ident FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia_esq.desenho) desenho,\n"..
    "(SELECT desenhos.fl FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia_esq.desenho) folha\n"..
    "FROM engates_esq, hierarquia_esq\n"..
    "WHERE engates_esq.unico = hierarquia_esq.componente\n"..
    "ORDER BY engates_esq.engate, desenho ASC, folha ASC)\n"..
    "SELECT num, engate, unico, desenho, folha,\n"..
    "CASE WHEN prox THEN prox ELSE ant END AS par,\n"..
    "CASE WHEN ant IS NULL THEN 1 WHEN prox THEN 1 ELSE 0 END AS ini\n"..
    "FROM (\n"..
    "SELECT ROW_NUMBER() OVER (PARTITION BY engate\n"..
    "ORDER BY desenho, folha) num,\n"..
    "engate, unico, desenho, folha,\n"..
    "LAG(unico) OVER (PARTITION BY engate ORDER BY engate ASC) ant,\n"..
    "LEAD(unico) OVER (PARTITION BY engate ORDER BY engate ASC) prox\n"..
    "FROM cte_engate)\n"..
    "WHERE num % 2 = 1\n"..
    "UNION\n"..
    "SELECT num, engate, unico, desenho, folha, ant, 0 ini\n"..
    "FROM (\n"..
    "SELECT ROW_NUMBER() OVER (PARTITION BY engate\n"..
    "ORDER BY desenho, folha) num,\n"..
    "engate, unico, desenho, folha,\n"..
    "LAG(unico) OVER (PARTITION BY engate ORDER BY engate ASC) ant,\n"..
    "LEAD(unico) OVER (PARTITION BY engate ORDER BY engate ASC) prox\n"..
    "FROM cte_engate)\n"..
    "WHERE num % 2 = 0\n"..
    "ORDER BY engate, desenho, folha)\n"..
    "WHERE ini = 0\n"..
    "ORDER BY engate, desenho, folha)\n"..
    "SELECT eng.unico, eng.desenho, eng.folha, \n"..
    "CASE WHEN e_num > 0 THEN e_num ELSE \n"..
    "(SELECT eng2.e_num FROM cte_eng eng2 WHERE eng2.unico = eng.par)\n"..
    "END e_num, CASE WHEN \n"..
    "(SELECT par.desenho FROM cte_eng par WHERE par.unico = eng.par) = eng.desenho THEN\n"..
    "'NESTE' ELSE \n"..
    "(SELECT par.desenho FROM cte_eng par WHERE par.unico = eng.par)\n"..
    "END vai_des, CASE WHEN \n"..
    "(SELECT par.folha FROM cte_eng par WHERE par.unico = eng.par) = eng.folha THEN \n"..
    "'NESTA FL.' ELSE\n"..
    "(SELECT par.folha FROM cte_eng par WHERE par.unico = eng.par)\n"..
    "END vai_fl\n"..
    "FROM cte_eng eng\n"..
    "ORDER BY eng.desenho, eng.folha")
  bd:exec('DROP VIEW IF EXISTS descr_comp')
  bd:exec("CREATE VIEW descr_comp AS\n"..
    "SELECT componentes_esq.unico,\n"..
  "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.painel) painel,\n"..
  "(SELECT CASE WHEN hierarquia_esq.pai THEN \n"..
  "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.pai)\n"..
  "ELSE componentes_esq.id END) componente,\n"..
  "CASE WHEN hierarquia_esq.modulo THEN \n"..
  "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia_esq.modulo)\n"..
  "WHEN componentes_esq.sub THEN componentes_esq.sub end  modulo,\n"..
  "CASE WHEN hierarquia_esq.pai THEN componentes_esq.id END parte, componentes_esq.tipo,\n"..
  "(SELECT desenhos.ident FROM desenhos WHERE desenhos.unico = hierarquia_esq.desenho) desenho,\n"..
  "(SELECT desenhos.fl FROM desenhos WHERE desenhos.unico = hierarquia_esq.desenho) fl,\n"..
  "componentes_esq.arquivo FROM componentes_esq, hierarquia_esq \n"..
  "WHERE componentes_esq.unico = hierarquia_esq.componente AND NOT componentes_esq.tipo = 'ENGATE'\n"..
  "ORDER BY painel ASC, componente ASC, modulo ASC, tipo ASC, hierarquia_esq.desenho ASC,\n"..
  "componentes_esq.x ASC, componentes_esq.y ASC;")
  bd:exec('DROP VIEW IF EXISTS tipico_aplic')
  bd:exec("CREATE VIEW tipico_aplic AS\n"..
    "SELECT efetivo.unico, efetivo.painel, efetivo.componente,\n" ..
    "efetivo.modulo, efetivo.parte, efetivo.tipo, efetivo.num,\n" ..
    "componentes.item, comp_term.num t_id, tip.term FROM\n" ..
    "(SELECT unico, painel, componente, modulo, parte, tipo,\n" ..
    "ROW_NUMBER() OVER\n" ..
    "(PARTITION BY painel, componente, modulo, tipo) num\n" ..
    "FROM descr_comp WHERE NOT tipo = 'REFERENCIA') efetivo\n" ..
    "LEFT JOIN comp_term\n" ..
    "ON efetivo.unico = comp_term.unico\n" ..
    "LEFT JOIN componentes\n" ..
    "ON efetivo.painel = componentes.painel AND\n" ..
    "efetivo.componente = componentes.id AND\n" ..
    "NOT efetivo.tipo = 'BORNE' AND\n" ..
    "NOT efetivo.tipo = 'BORNE_SEC'\n" ..
    "LEFT JOIN (SELECT tipico.item,\n" ..
    "tipico.modulo, tipico.elemento,\n" ..
    "tipico.num, tipico_term.t_id, tipico_term.term\n" ..
    "FROM (SELECT item, modulo, el_id, elemento,\n" ..
    "ROW_NUMBER() OVER\n" ..
    "(PARTITION BY item, modulo, elemento) num\n" ..
    "FROM tipico_term\n" ..
    "GROUP BY item, modulo, elemento, el_id) tipico\n" ..
    "INNER JOIN tipico_term\n" ..
    "ON tipico.item = tipico_term.item AND\n" ..
    "tipico.el_id = tipico_term.el_id AND (\n" ..
    "tipico.modulo = tipico_term.modulo \n" ..
    "OR (tipico.modulo IS NULL AND  \n" ..
    "tipico_term.modulo IS NULL))\n" ..
    "ORDER BY tipico.item, tipico.modulo,\n" ..
    "tipico.elemento, tipico.num, \n" ..
    "tipico_term.t_id) tip\n" ..
    "ON efetivo.tipo = tip.elemento AND\n" ..
    "efetivo.num = tip.num AND\n" ..
    "componentes.item = tip.item AND\n" ..
    "NOT efetivo.tipo = 'BORNE' AND\n" ..
    "NOT efetivo.tipo = 'BORNE_SEC' AND\n" ..
    "comp_term.num = tip.t_id AND ( \n" ..
    "(tip.modulo IS NULL AND efetivo.modulo IS NULL) \n" ..
    "OR efetivo.modulo = tip.modulo);")
  bd:exec('DROP VIEW IF EXISTS estima_comp_esq')
  bd:exec("CREATE VIEW estima_comp_esq AS\n"..
    "SELECT painel, componente, sum(num) elementos, sum(DISTINCT est) est,\n" ..
    "  CASE WHEN est = 1 THEN 'DIODO'\n" ..
    "  WHEN est = 1 THEN 'DIODO'\n" ..
    "  WHEN est = 2 THEN 'RÉGUA'\n" ..
    "  WHEN est = 4 THEN 'MINI DJ'\n" ..
    "  WHEN est = 20 THEN 'MINI DJ'\n" ..
    "  WHEN est = 8 THEN 'CT TENSÃO'\n" ..
    "  WHEN est = 16 THEN 'CT CORRENTE'\n" ..
    "  WHEN est = 24 THEN 'CT TENSÃO+CORRENTE'\n" ..
    "  WHEN est = 32 THEN 'CONTATO EXTERNO'\n" ..
    "  WHEN est >= 64 AND est < 128 THEN\n" ..
    "    CASE WHEN componente LIKE '%94%' THEN 'RELÉ TRIP'\n" ..
    "    WHEN componente LIKE '%83%' THEN 'RELÉ BIEST CTRL'\n" ..
    "    WHEN componente LIKE '%86%' THEN 'RELÉ BIEST BLOQ'\n" ..
    "    ELSE 'RELÉ AUX' END\n" ..
    "  WHEN est >= 128 AND est < 417 THEN\n" ..
    "    CASE WHEN componente LIKE '%21%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%87%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%50%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%51%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%67%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%59%' THEN 'IED RELÉ PROT'\n" ..
    "    WHEN componente LIKE '%61%' THEN 'IED RELÉ PROT'\n" ..
    "    ELSE 'IED' END ELSE 'ERRO' END\n" ..
    "  estimado, item\n" ..
    "FROM ( SELECT painel, componente, tipo, COUNT(tipo) num,\n" ..
    "  CASE tipo WHEN 'DIODO' THEN 1\n" ..
    "  WHEN 'BORNE_SEC' THEN 2\n" ..
    "  WHEN 'BORNE' THEN 2\n" ..
    "  WHEN 'MINI_DISJ' THEN 4\n" ..
    "  WHEN 'CT_TENSAO' THEN 8\n" ..
    "  WHEN 'CT_CORR' THEN 16\n" ..
    "  WHEN 'CONTATO_NA' THEN 32\n" ..
    "  WHEN 'CONTATO_NF' THEN 32\n" ..
    "  WHEN 'BOBINA' THEN 64\n" ..
    "  WHEN 'ALIM_CC' THEN 128\n" ..
    "  WHEN 'ENT_DIG' THEN 256\n" ..
    "  WHEN 'SAIDA_DIG' THEN 256\n" ..
    "  ELSE 0 END est\n" ..
    "  FROM descr_comp WHERE NOT tipo = 'REFERENCIA' AND \n" ..
    "  NOT componente = '' AND componente NOT NULL\n" ..
    "  GROUP BY painel, componente, tipo)\n" ..
    "LEFT JOIN regras_equip ON estimado = regras_equip.tipo\n" ..
    "GROUP BY painel, componente;")
  
  bd:close()
  return true
end

function deleta_arq_db(bd, arquivo)
  local comando = "DELETE FROM engates_esq\n" ..
    "WHERE engates_esq.unico IN (\n" ..
    "SELECT unico\n" ..
    "FROM componentes_esq\n" ..
    "WHERE componentes_esq.arquivo = '" .. arquivo .. "');"

  bd:exec(comando)
  
  local comando = "DELETE FROM referencias_esq\n" ..
    "WHERE referencias_esq.unico IN (\n" ..
    "SELECT unico\n" ..
    "FROM componentes_esq\n" ..
    "WHERE componentes_esq.arquivo = '" .. arquivo .. "');"

  bd:exec(comando)
    
  comando = "DELETE FROM barras_esq\n" ..
    "WHERE barras_esq.componente IN (\n" ..
    "SELECT unico\n" ..
    "FROM componentes_esq\n" ..
    "WHERE componentes_esq.arquivo = '" .. arquivo .. "');"

  bd:exec(comando)
    
  comando = "DELETE FROM terminais_esq\n" ..
    "WHERE terminais_esq.componente IN (\n" ..
    "SELECT unico\n" ..
    "FROM componentes_esq\n" ..
    "WHERE componentes_esq.arquivo = '" .. arquivo .. "');"

  bd:exec(comando)
    
  comando =  "DELETE FROM desenhos\n" ..
    "WHERE desenhos.unico IN (\n" ..
    "SELECT unico\n" ..
    "FROM caixas\n" ..
    "WHERE caixas.arquivo = '" .. arquivo .. "');"

  bd:exec(comando)
    
  comando = "DELETE FROM componentes_esq\n" ..
    "WHERE componentes_esq.arquivo = '" .. arquivo .. "';"

  bd:exec(comando)
    
  comando = "DELETE FROM caixas\n" ..
    "WHERE caixas.arquivo = '" .. arquivo .. "';"

  bd:exec(comando)
    
  comando = "DELETE FROM arquivos\n" ..
    "WHERE arquivos.caminho = '" .. arquivo .. "';"
    

  bd:exec(comando)
  
end

function atualiza_db(bd, arquivo)

  cadzinho.open_drwg (arquivo, true)
  -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
  atualiza_unicos()
  -- atualiza a lista principal com os elementos
  atualiza_elems()
  
  cadzinho.save_drwg (arquivo, true)
  
  local caixas = obtem_caixas()
  atualiza_db_caixas(bd, arquivo, caixas)
  atualiza_db_esq(bd, arquivo, caixas)
  
end

function atualiza_db_caixas (bd, arquivo, caixas)

  for id, caixa in pairs(caixas) do
    local pai = id
    if type(pai) == 'number' then
      pai = string.format('%d', pai)
    else
      pai = '0'
    end
    if not pai then pai = 'NULL' end
    for el_id, _ in pairs(caixa.conteudo) do
      local el = elems_pelicanu[el_id]

      if el.tipo == "CAIXA" then
        local sub_caixa = caixas[el_id]
        if sub_caixa then
          local nome = sub_caixa.nome
          if not nome then nome = 'NULL' end
          local tipo = sub_caixa.tipo
          if not tipo then tipo = 'NULL' end

          
          bd:exec ("INSERT INTO caixas VALUES("..
            string.format('%d', el_id) ..", '"..
            nome .."', '"..
            tipo .."', "..
            pai..
            ", '".. arquivo .. "'" ..
          ");")

          if sub_caixa.tipo == "DESENHO" then
            local dados = pega_attrib(el.ent)
            if dados.ident then dados.ident = "'" .. dados.ident .. "'" 
            else dados.ident = 'NULL' end
            if dados.titulo then dados.titulo = "'" .. dados.titulo .. "'" 
            else dados.titulo = 'NULL' end
            if dados.tipo then dados.tipo = "'" .. dados.tipo .. "'" 
            else dados.tipo = 'NULL' end
            if dados.projeto then dados.projeto = "'" .. dados.projeto .. "'" 
            else dados.projeto = 'NULL' end
            if dados.rev then dados.rev = "'" .. dados.rev .. "'" 
            else dados.rev = 'NULL' end
            if dados.versao then dados.versao = "'" .. dados.versao .. "'" 
            else dados.versao = 'NULL' end
            if dados.fl then dados.fl = "'" .. dados.fl .. "'" 
            else dados.fl = 'NULL' end
            if dados.data then dados.data = "'" .. dados.data .. "'" 
            else dados.data = 'NULL' end
            if dados.aplic then dados.aplic = "'" .. dados.aplic .. "'" 
            else dados.aplic = 'NULL' end
            if dados.instal then dados.instal = "'" .. dados.instal .. "'" 
            else dados.instal = 'NULL' end
            if dados.visto then dados.visto = "'" .. dados.visto .. "'" 
            else dados.visto = 'NULL' end
            if dados.aprov then dados.aprov = "'" .. dados.aprov .. "'" 
            else dados.aprov = 'NULL' end
            if dados.classif then dados.classif = "'" .. dados.classif .. "'" 
            else dados.classif = 'NULL' end
            if dados.p_fl then dados.p_fl = "'" .. dados.p_fl .. "'" 
            else dados.p_fl = 'NULL' end
            bd:exec ("INSERT INTO desenhos VALUES("..
             string.format('%d', el_id) ..", "..
             dados.ident ..", "..
             dados.titulo ..", "..
             dados.tipo ..", "..
             dados.projeto ..", "..
             dados.rev ..", "..
             dados.versao ..", "..
             dados.fl ..", "..
             dados.data ..", "..
             dados.aplic ..", "..
             dados.instal ..", "..
             dados.visto ..", "..
             dados.aprov ..", "..
             dados.classif ..", "..
             dados.p_fl ..
            ");")
          end
        end
      end
      
    end
  end
end

function atualiza_db_esq(bd, arquivo, caixas)
  
  local fila = nova_fila()
  local desenhos = obtem_desenhos (caixas, {'ESQUEM', 'FUNCIONA', 'TRIFIL'})
  
  for id, desenho in pairs(desenhos) do
    -- insere a desenho na fila de caixas
    desenho.id = id
    fila.insere(desenho)
  end
  
  -- processa a fila de caixas
  local caixa = fila.remove()
  while caixa do
    local pai = caixa.id
    if type(pai) == 'number' then
      pai = string.format('%d', pai)
    else
      pai = '0'
    end
    if not pai then pai = 'NULL' end
    -- varre o conteúdo do caixa corrente
    for el_id, _ in pairs(caixa.conteudo) do
      local el = elems_pelicanu[el_id]

      if el.tipo == "COMPONENTE" then
        local terms = info_terminais (el.ent)
        local bloco = cadzinho.get_blk_name (el.ent)
        local dados = cadzinho.get_ins_data (el.ent)
        if not bloco then bloco = 'NULL'
        else bloco = "'"..bloco.."'" end
        local comp_id = pega_comp_id(el.ent)
        if not comp_id then comp_id = 'NULL'
        elseif string.len(comp_id) == 0 then comp_id = 'NULL'
        else comp_id = "'"..comp_id.."'" end
        local comp_tipo = pega_comp_tipo(el.ent)
        if not comp_tipo then comp_tipo = 'NULL'
        else comp_tipo = "'"..comp_tipo.."'" end
        local comp_sub = pega_comp_sub(el.ent)
        if not comp_sub then comp_sub = 'NULL'
        else comp_sub = "'"..comp_sub.."'" end
        
        bd:exec ("INSERT INTO componentes_esq VALUES("..
          string.format('%d', el_id) ..", "..
          comp_tipo ..", "..
          bloco ..", "..
          comp_id ..", "..
          comp_sub ..", "..
          pai..", "..
          string.format('%f', dados.pt.x) .. ", "..
          string.format('%f', dados.pt.y) ..
          ", '".. arquivo .. "'" ..
          ");")
        for t_id, t in pairs(terms) do
          bd:exec ("INSERT INTO terminais_esq VALUES("..
            string.format('%d', el_id) ..", "..
            string.format('%d', t_id) ..", '"..
            t .."');")
        end

        if comp_tipo == "'ENGATE'" then
          local engate = pega_engate(el.ent)
          if not engate then engate = 'NULL'
          else engate = "'"..engate.."'" end
          bd:exec ("INSERT INTO engates_esq VALUES("..
          string.format('%d', el_id) ..", "..engate..")")
        end
        if comp_tipo == "'REFERENCIA'" then
          local dados = pega_attrib(el.ent)
          if dados.ELEMENTO then
            bd:exec ("INSERT INTO referencias_esq VALUES("..
            string.format('%d', el_id) .. ", '".. dados.TERMINAL  .. "', '".. 
              dados.ELEMENTO .. "')"
            )
          end
        end
      elseif el.tipo == "CAIXA" then
        local sub_caixa = caixas[el_id]
        if sub_caixa then
          -- insere a sub_caixa na fila de caixas
          sub_caixa.id = el_id
          fila.insere(sub_caixa)
        end
      end
      
    end
    
    -- pega a próxima caixa da fila
    caixa = fila.remove()
  end
  
  -- obtem as barras (ligações do desenho)
  local barras = obtem_barras(desenhos, caixas)

  for i, barra in pairs(barras) do
    if not barra.id then
      barra.id = cadzinho.unique_id()
    end

    for j, term in ipairs(barra.terminais) do

      bd:exec ("INSERT INTO barras_esq VALUES('"..
        barra.id .."', "..
        string.format('%d', term.comp) ..", "..
        string.match(term.term, "^T(%d)")..
        ");")
    end
  end
  
end

function atualiza_lista_arq_bd (bd, lista_arq)
  bd:exec("DELETE FROM arquivos WHERE caminho <> 'projeto.db';")
  for i = 1, #lista_arq do
    bd:exec ("INSERT INTO arquivos VALUES('"..
      lista_arq[i].name .."', "..
      string.format('%d', lista_arq[i].modified) ..", NULL, NULL);")
  end
end

function atualiza_db_paineis(bd)
  -- obtém a lista de painéis efetivamente utilizados no banco de dados
  local paineis = {}
  for linha in bd:cols('SELECT DISTINCT painel FROM descr_comp ORDER BY painel') do -- para cada linha do BD
    if linha.painel then paineis[linha.painel] = 1 end
  end
  
  -- atualiza a tabela do bd com o descritivo dos painéis (acrescenta os novos)
  for painel, _ in pairs(paineis) do
    bd:exec (
    "INSERT INTO paineis(id, titulo, descr, fiacao, x, y)" .. 
    "SELECT '"..painel.."', NULL, NULL, 1, 0, 0 WHERE NOT EXISTS"..
    "(SELECT 1 FROM paineis WHERE id = '"..painel.."');")
  
  end
end

function atualiza_db_componentes(bd)
  
  -- obtém a lista de componentes efetivamente utilizados no banco de dados
  local comps = {}
  local regras = {}
  for linha in bd:cols('SELECT * FROM regras_equip') do
    regras[linha.tipo] = linha.item
  end
  for linha in bd:cols(
    'SELECT painel, componente, sum(num) elementos, ' ..
    'GROUP_CONCAT(tipo, ";") tipos ' ..
    'FROM ( SELECT painel, componente, tipo, COUNT(tipo) num ' ..
    "FROM descr_comp WHERE NOT tipo = 'REFERENCIA' " ..
    'GROUP BY painel, componente, tipo) ' ..
    'GROUP BY painel, componente;'
  ) do
    -- estima qual o tipo do componente
    local tipo = estima_tipo_comp (linha.componente, linha.tipos)
    
    local item_le = regras[tipo]
    
    comps[#comps + 1] = {
      painel = linha.painel,
      comp = linha.componente,
      tipo = tipo,
      item = item_le
    }
  end
  
  -- atualiza a tabela do bd com o descritivo dos componentes (acrescenta os novos)
  for _, comp in ipairs(comps) do
    local item_le = 'NULL'
    if comp.item then item_le = "'" .. comp.item .. "'" end
    bd:exec (
    "INSERT INTO componentes(painel, id, tipo, item, id_fiacao)" .. 
    "SELECT '".. comp.painel .."', '"..
    comp.comp .."', '".. comp.tipo .."', ".. item_le ..
    ", NULL  WHERE NOT EXISTS"..
    "(SELECT 1 FROM componentes WHERE painel = '".. comp.painel .."' AND id = '"..
    comp.comp .."');")
  
  end
end

--[[
select unico, painel, componente, modulo,
parte, tipo, num, item, t_id,
case when tipico_aplic.term IS NOT NULL then tipico_aplic.term else 
(case when not tipo = 'BORNE' then (select terminal from comp_term
where tipico_aplic.unico = comp_term.unico and
tipico_aplic.t_id = comp_term.num) else num end) end terminal
from tipico_aplic;

select unico, painel, componente, modulo,
parte, tipo, num, item, t_id,
case when tipico_aplic.term IS NOT NULL then tipico_aplic.term else 
(case when not tipo = 'BORNE' then num||t_id else num end) end terminal
from tipico_aplic;

select unico, painel, componente, modulo,
parte, tipo, num, item, t_id,
case when tipico_aplic.term IS NOT NULL then tipico_aplic.term else 
(case when not tipo = 'BORNE' then num||t_id else num end) end terminal,
case when item is not null and tipico_aplic.term IS NULL and not tipo = 'BORNE' then 'incompleto' else
(case when not tipo = 'BORNE' and tipico_aplic.term IS NULL then 'sem tip' end) end alerta
from tipico_aplic
]]--