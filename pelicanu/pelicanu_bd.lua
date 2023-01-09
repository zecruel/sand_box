local pelicanu_bd = {}
function pelicanu_bd:novo(caminho)
  local bd = sqlite.open(caminho)
  bd:exec('DROP TABLE IF EXISTS componentes')
  bd:exec('CREATE TABLE componentes('..
    'unico INTEGER, tipo TEXT, '..
    'bloco TEXT, id TEXT, pai INTEGER, x REAL, y REAL)')
  bd:exec('DROP TABLE IF EXISTS caixas')
  bd:exec('CREATE TABLE caixas('..
    'unico INTEGER, '..
    'id TEXT, tipo TEXT, pai INTEGER)')
  bd:exec('DROP TABLE IF EXISTS terminais')
  bd:exec('CREATE TABLE terminais('..
    'componente INTEGER, '..
    'id INTEGER, terminal TEXT)')
  bd:exec('DROP TABLE IF EXISTS barras')
  bd:exec('CREATE TABLE barras('..
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
  bd:exec('DROP TABLE IF EXISTS engates')
  bd:exec('CREATE TABLE engates('..
    'unico INTEGER, '..
    'engate TEXT)')
  bd:exec('DROP VIEW IF EXISTS hierarquia')
  bd:exec("CREATE VIEW hierarquia AS\n"..
    "SELECT componentes.unico componente,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'COMPONENTE'\n"..
    ") pai,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'MODULO'\n"..
    ") modulo,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'PAINEL'\n"..
    ") painel,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'DESENHO'\n"..
    ") desenho,\n"..
    "(WITH RECURSIVE cte_caixas (unico, tipo, pai) AS (\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas WHERE caixas.unico = componentes.pai\n"..
    "UNION ALL\n"..
    "SELECT caixas.unico, caixas.tipo, caixas.pai\n"..
    "FROM caixas, cte_caixas\n"..
    "WHERE cte_caixas.pai = caixas.unico)\n"..
    "SELECT unico FROM cte_caixas WHERE cte_caixas.tipo = 'DESCRITIVO'\n"..
    ") descritivo\n"..
    "FROM componentes\n")
  bd:exec('DROP VIEW IF EXISTS comp_term')
  bd:exec("CREATE VIEW comp_term AS\n"..
    "SELECT componentes.unico,\n"..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.painel) painel,\n"..
    "(SELECT CASE WHEN hierarquia.pai\n"..
    "THEN (SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.pai)\n"..
    "ELSE componentes.id\n"..
    "END) componente,\n"..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.modulo) modulo,\n"..
    "(SELECT CASE WHEN hierarquia.pai\n"..
    "THEN componentes.id\n"..
    "END) parte,\n"..
    "componentes.bloco, componentes.tipo, terminais.id num, terminais.terminal \n"..
    "FROM componentes, hierarquia \n"..
    "INNER JOIN terminais ON terminais.componente = componentes.unico\n"..
    "WHERE componentes.unico = hierarquia.componente "..
    "AND NOT componentes.tipo = 'ENGATE'\n"..
    "ORDER BY painel ASC, componente ASC, modulo ASC, tipo ASC, hierarquia.desenho ASC, "..
    "componentes.x ASC, componentes.y ASC, num ASC;\n")
  bd:exec('DROP VIEW IF EXISTS eng_term')
  bd:exec("CREATE VIEW eng_term AS\n"..
    "SELECT engates.unico, engates.engate, barras.id barra, "..
    "terminais.terminal, hierarquia.desenho\n"..
    "FROM engates, barras, hierarquia\n"..
    "INNER JOIN terminais ON terminais.componente = engates.unico\n"..
    "WHERE engates.unico = barras.componente AND terminais.id = barras.terminal "..
    "AND engates.unico = hierarquia.componente\n"..
    "ORDER BY engates.engate ASC, terminais.terminal ASC;\n")
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
    "FROM eng_term WHERE barras.id = eng_term.barra)\n"..
    "THEN (SELECT CASE WHEN EXISTS (\n"..
    "SELECT eng_repetidos.node FROM eng_repetidos\n"..
    "where eng_term.engate || '_' || eng_term.terminal = eng_repetidos.node)\n"..
    "THEN (SELECT eng_repetidos.barra FROM eng_repetidos \n"..
    "WHERE eng_term.engate || '_' || eng_term.terminal = eng_repetidos.node\n"..
    "LIMIT 1) ELSE eng_term.engate || '_' || eng_term.terminal END\n"..
    "FROM eng_term WHERE barras.id = eng_term.barra)\n"..
    "ELSE barras.id END\n"..
    "barra, componentes.unico componente, barras.terminal, hierarquia.painel, "..
    "caixas.id nome_painel\n"..
    "FROM barras, componentes, hierarquia, caixas\n"..
    "WHERE componentes.unico = barras.componente AND \n"..
    "NOT componentes.tipo = 'ENGATE' AND barras.componente = hierarquia.componente\n"..
    "AND hierarquia.painel = caixas.unico\n"..
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
    "FROM componentes, hierarquia, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes.unico = barra_consol.componente AND\n" ..
    "componentes.unico = comp_term.unico AND\n" ..
    "barra_consol.barra = interlig1.barra AND\n" ..
    "(componentes.tipo = 'BORNE' OR\n" ..
    "componentes.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig1.painel AND\n" ..
    "hierarquia.componente = componentes.unico AND\n" ..
    "caixas.unico = hierarquia.painel) borne_p1,\n" ..
    "(SELECT comp_term.terminal\n" ..
    "FROM componentes, hierarquia, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes.unico = barra_consol.componente AND\n" ..
    "componentes.unico = comp_term.unico AND\n" ..
    "barra_consol.terminal = comp_term.num AND\n" ..
    "barra_consol.barra = interlig1.barra AND\n" ..
    "(componentes.tipo = 'BORNE' OR\n" ..
    "componentes.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig1.painel AND\n" ..
    "hierarquia.componente = componentes.unico AND\n" ..
    "caixas.unico = hierarquia.painel) term_p1,\n" ..
    "interlig2.painel p2,\n" ..
    "(SELECT comp_term.componente\n" ..
    "FROM componentes, hierarquia, barra_consol,\n" ..
    "caixas, comp_term\n" ..
    "WHERE componentes.unico = barra_consol.componente AND\n" ..
    "componentes.unico = comp_term.unico AND\n" ..
    "barra_consol.barra = interlig2.barra AND\n" ..
    "(componentes.tipo = 'BORNE' OR\n" ..
    "componentes.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig2.painel AND\n" ..
    "hierarquia.componente = componentes.unico AND\n" ..
    "caixas.unico = hierarquia.painel) borne_p2,\n" ..
    "(SELECT comp_term.terminal\n" ..
    "FROM componentes, hierarquia,\n" ..
    "barra_consol, caixas, comp_term\n" ..
    "WHERE componentes.unico = barra_consol.componente AND\n" ..
    "componentes.unico = comp_term.unico AND\n" ..
    "barra_consol.terminal = comp_term.num AND\n" ..
    "barra_consol.barra = interlig2.barra AND\n" ..
    "(componentes.tipo = 'BORNE' OR\n" ..
    "componentes.tipo = 'BORNE_SEC') AND\n" ..
    "caixas.id = interlig2.painel AND\n" ..
    "hierarquia.componente = componentes.unico AND\n" ..
    "caixas.unico = hierarquia.painel)term_p2\n" ..
    "FROM cte_interlig interlig1\n" ..
    "JOIN cte_interlig interlig2 ON\n" ..
    "interlig1.barra = interlig2.barra AND p1 > p2\n"..
    "ORDER BY p1 ASC, p2 ASC;")
  bd:exec('DROP VIEW IF EXISTS engate_par')
  bd:exec("CREATE VIEW engate_par AS\n"..
    "WITH cte_eng AS (select *, ROW_NUMBER() OVER (PARTITION BY desenho\n"..
    "ORDER BY ini, folha) e_num\n"..
    "FROM (WITH cte_engate AS (\n"..
    "SELECT engates.unico unico, engates.engate engate,\n"..
    "(SELECT desenhos.ident FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia.desenho) desenho,\n"..
    "(SELECT desenhos.fl FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia.desenho) folha\n"..
    "FROM engates, hierarquia\n"..
    "WHERE engates.unico = hierarquia.componente\n"..
    "ORDER BY engates.engate, desenho ASC, folha ASC)\n"..
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
    "SELECT engates.unico unico, engates.engate engate,\n"..
    "(SELECT desenhos.ident FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia.desenho) desenho,\n"..
    "(SELECT desenhos.fl FROM desenhos\n"..
    "WHERE desenhos.unico = hierarquia.desenho) folha\n"..
    "FROM engates, hierarquia\n"..
    "WHERE engates.unico = hierarquia.componente\n"..
    "ORDER BY engates.engate, desenho ASC, folha ASC)\n"..
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
    "SELECT componentes.unico,\n" ..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.painel) painel,\n" ..
    "(SELECT CASE WHEN hierarquia.pai\n" ..
    "THEN (SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.pai)\n" ..
    "ELSE componentes.id\n" ..
    "END) componente,\n" ..
    "(SELECT caixas.id FROM caixas WHERE caixas.unico = hierarquia.modulo) modulo,\n" ..
    "(SELECT CASE WHEN hierarquia.pai\n" ..
    "THEN componentes.id\n" ..
    "END) parte, componentes.tipo,\n" ..
    "(SELECT desenhos.ident FROM desenhos WHERE desenhos.unico = hierarquia.desenho) desenho,\n" ..
    "(SELECT desenhos.fl FROM desenhos WHERE desenhos.unico = hierarquia.desenho) fl\n" ..
    "FROM componentes, hierarquia \n" ..
    "WHERE componentes.unico = hierarquia.componente AND NOT componentes.tipo = 'ENGATE'\n" ..
    "ORDER BY painel ASC, componente ASC, modulo ASC, tipo ASC, hierarquia.desenho ASC, componentes.x ASC, componentes.y ASC;")
  bd:close()
  return true
end
return pelicanu_bd