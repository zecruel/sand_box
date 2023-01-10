-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8

-- ============= variáveis globais =====================
pelicanu = {}  -- tabela principal
pelicanu.elems = {} -- lista principal dos elementos
tolerancia = 0.1 -- tolerancia para comparacoes de numeros não inteiros
biblioteca = "" -- diretorio onde estao os elementos (componentes, formatos, etc)

require('pelicanu_bd')
require('pelicanu_dyn')

dizeres = {
  pelicanu = '-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar',
  autor = '-- Autor: Ezequiel Rabelo de Aguiar - 2023',
  lua = '-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8',
  alerta = '-- ========!! Altere-o com parcimonia !!========',
  proj = '-- Arquivo de configuracao de um projeto no PELICAnU, criado automaticamente',
  ini = '-- Arquivo de inicialização do PELICAnU, criado automaticamente',
  config = '-- Arquivo de configuracao geral do PELICAnU'
}

projeto = { 
  titulo = '',
  instalacao = '',
  aplicacao = '',
  codigo = '',
  rev = '',
  aprovacao = '',
  visto = '',
  projetista = '',
  descr = '',
  data = '',
  caminho = ''
}

-- para funcoes dinamicas
num_pt = 1 -- numero de pontos
pts = {} -- lista de pontos de entrada
lista_comp = {}
lista_comp_o = {}
lista_comp_tipo = {}
lista_formato = {}
lista_formato_o = {}

-- para a interface grafica
component = {value = ''}
g_caixa_id = {value = ''}
g_caixa_tipo = {value = 1, 'COMPONENTE', 'MODULO', 'PAINEL', 'GENERICO'}
g_editor_abas = {value = 1, "Esquematico", "Biblioteca"}
g_term_num = {value = 1}
g_term_nome = {value = "1"}
g_eng_num = {value = 1}
g_eng_nome = {value = "E1"}
g_engate = {value = ""}
g_componente = {value = ""}
g_tipo_comp = {value = 1, ""}
g_comp_id = {value = ""}
g_terminais = {}
g_formato = {value = ""}
modal = ''
sub_modal = ''
g_caminho = {value = ""}
msg = ''
g_titulo = {value = ""}
g_instalacao = {value = ""}
g_aplicacao = {value = ""}
g_codigo = {value = ""}
g_rev = {value = ""}
g_aprovacao = {value = ""}
g_visto = {value = ""}
g_projetista = {value = ""}
g_descr = {value = ""}
g_data = {value = ""}

excel = require "xlsxwriter.workbook"

-- ============================================
function exists(name)
    if type(name)~="string" then return false end
    return os.rename(name,name) and true or false
end

function isFile(name)
    if type(name)~="string" then return false end
    if not exists(name) then return false end
    local f = io.open(name)
    if f then
      f:close()
      return true
    end
    return false
end

function isDir(name)
    return (exists(name) and not isFile(name))
end

function nome_arq(url)
  return url:match("(.+)%..+$")
end

function extensao(url)
  return url:match("^.+(%..+)$")
end

function diretorio(url)
  url = string.gsub(url, "(.[^"..fs.dir_sep.."]+)$", "") .. fs.dir_sep
  return url
end

function format_dir(url)
  url = string.gsub(url, "(.[^"..fs.dir_sep.."])$", '%1' .. fs.dir_sep)
  return url
end

function cria_pasta(caminho)
   if (os.getenv("oS") or ""):match("^Windows") then
      os.execute('mkdir "' .. caminho .. '" 1>nul: 2>&1')
   else
      os.execute('mkdir -p "' .. caminho .. '" 2>/dev/null')
   end
end

function carrega_config(arq)
  local configEnv = {} -- to keep it separate from the global env
  local f,err = loadfile(arq, "t", configEnv)
  if f then
    f() -- run the chunk
    -- now configEnv should contain your data
    return configEnv
  else
    return nil, err
  end
end

function abre_projeto(caminho)
  if type(caminho) ~= 'string' then return nil end
  local config = format_dir(caminho) .. '_dados' .. fs.dir_sep .. 'projeto.lua'
  local bd = format_dir(caminho) .. '_dados'.. fs.dir_sep .. 'projeto.db'
  local log = format_dir(caminho) .. '_dados'.. fs.dir_sep .. 'log.txt'
  local saida = format_dir(caminho) .. '_saida' .. fs.dir_sep
  local proj, err = carrega_config(config)
  if not proj then return nil end
  
  projeto.caminho = caminho
  projeto.config = config
  --if exists(bd) then 
    projeto.bd = bd
  --else
  --  projeto.bd = nil
  --end
  --if exists(log) then 
    projeto.log = log
  --else
  --  projeto.log = nil
  --end
  if exists(saida) then 
    projeto.saida = saida
  else
    projeto.saida = nil
  end
  if type(proj.titulo) == 'string' then
    projeto.titulo = proj.titulo
  else
    projeto.titulo = ''
  end
  if type(proj.instalacao) == 'string' then
    projeto.instalacao = proj.instalacao
  else
    projeto.instalacao = ''
  end
  if type(proj.aplicacao) == 'string' then
    projeto.aplicacao = proj.aplicacao
  else
    projeto.aplicacao = ''
  end
  if type(proj.codigo) == 'string' then
    projeto.codigo = proj.codigo
  else
    projeto.codigo = '' 
  end
  if type(proj.rev) == 'string' then
    projeto.rev = proj.rev
  else
    projeto.rev = ''
  end
  if type(proj.aprovacao) == 'string' then
    projeto.aprovacao = proj.aprovacao
  else
    projeto.aprovacao = ''
  end
  if type(proj.visto) == 'string' then
    projeto.visto = proj.visto
  else
    projeto.visto = ''
  end
  if type(proj.projetista) == 'string' then
    projeto.projetista = proj.projetista
  else
    projeto.projetista = ''
  end
  if type(proj.descr) == 'string' then
    projeto.descr = proj.descr
  else
    projeto.descr = ''
  end
  if type(proj.data) == 'string' then
    projeto.data = proj.data
  else
    projeto.data = ''
  end
  
  local arq_ini = io.open(diretorio(fs.script_path()) .. 'ini.lua', 'w+')
  if arq_ini then
    arq_ini:write(dizeres.pelicanu..'\n')
    arq_ini:write(dizeres.autor..'\n')
    arq_ini:write(dizeres.ini..'\n')
    arq_ini:write(dizeres.lua..'\n')
    arq_ini:write(dizeres.alerta..'\n\n')
    arq_ini:write('projeto = "'.. string.gsub(caminho, '\\', '\\\\') .. '"')
    
    arq_ini:close()
  end
  
  fs.chdir(caminho)
  
  return true
end

function novo_projeto(caminho, proj)
  if type(caminho) ~= 'string' then return nil end
  if type(proj) ~= 'table' then return nil end
  local dados = format_dir(caminho) .. '_dados' .. fs.dir_sep
  local saida = format_dir(caminho) .. '_saida' .. fs.dir_sep
  local config = dados .. 'projeto.lua'
  local bd = dados .. 'projeto.db'
  local log = dados .. 'log.txt'
  
  if not exists (caminho) then
    cria_pasta(caminho)
  end
  
  if not exists (caminho) then return nil end
  
  if not exists (dados) then
    cria_pasta(dados)
  end
  if not exists (saida) then
    cria_pasta(saida)
  end
  
  local arq_prj, err = io.open(config, 'w+')
  if not arq_prj then return nil end
  arq_prj:write(dizeres.pelicanu..'\n')
  arq_prj:write(dizeres.autor..'\n')
  arq_prj:write(dizeres.proj..'\n')
  arq_prj:write(dizeres.lua..'\n')
  arq_prj:write(dizeres.alerta..'\n\n')
  
  arq_prj:write('titulo = "'.. string.gsub(proj.titulo, '\\', '\\\\') .. '"\n')
  arq_prj:write('instalacao = "'.. string.gsub(proj.instalacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('aplicacao = "'.. string.gsub(proj.aplicacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('codigo = "'.. string.gsub(proj.codigo, '\\', '\\\\') .. '"\n')
  arq_prj:write('rev = "'.. string.gsub(proj.rev, '\\', '\\\\') .. '"\n')
  arq_prj:write('aprovacao = "'.. string.gsub(proj.aprovacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('visto = "'.. string.gsub(proj.visto, '\\', '\\\\') .. '"\n')
  arq_prj:write('projetista = "'.. string.gsub(proj.projetista, '\\', '\\\\') .. '"\n')
  arq_prj:write('descr = "'.. string.gsub(proj.descr, '\\', '\\\\') .. '"\n')
  arq_prj:write('data = "'.. string.gsub(proj.data, '\\', '\\\\') .. '"\n')
  
  arq_prj:close()
  
  local arq_log = io.open(log, 'a+')
  if arq_log then
    arq_log:write(os.date()..' => Criação do projeto\n')
    
    arq_log:close()
  end
  
  bd_novo(bd)
  
  return abre_projeto(caminho)
end

function no_segmento (pt, linha)
  -- Verifica se um ponto esta num segmento de reta
  
  -- confere os parametros passados
  if type(pt) ~= "table" then return false end
  if type(linha) ~= "table" then return false end
  
  -- primeiro, checa se o ponto pertence a reta
  if math.abs((pt.x - linha[1].x) / (linha[2].x - linha[1].x) - 
    (pt.y - linha[1].y) / (linha[2].y - linha[1].y)) > tolerancia then
    return false
  end
  
  -- agora, verifica se esta dentro do segmento
  if ( pt.x > linha[1].x and pt.x < linha[2].x or 
    pt.x < linha[1].x and pt.x > linha[2].x) or 
    ( pt.y > linha[1].y and pt.y < linha[2].y or 
    pt.y < linha[1].y and pt.y > linha[2].y) then
    return true
  end
  
  -- nao estah no segmento
  return false
end

function linha_conect (a, b)
  -- Verifica se dois segmentos de linha estão conectados
  
  -- Verify if passed argments are valid
  if type(a) ~= "table" then return false end
  if type(b) ~= "table" then return false end
  
  -- testa pra ver se nao sao o mesmo objeto
  if a == b then return false end
  
  -- trivialmente, testa a coincidencia de vertices
  for _, pt1 in ipairs(a) do
    for _, pt2 in ipairs(b) do
      if math.abs(pt1.x - pt2.x) < tolerancia and math.abs(pt1.y - pt2.y) < tolerancia then
        return true
      end
    end
  end
  
  -- Senão, testa se algum vertice toca a outra linha
  for _, pt1 in ipairs(a) do
    if no_segmento(pt1, b) then return true end
  end
  for _, pt1 in ipairs(b) do
    if no_segmento(pt1, a) then return true end
  end
  
  -- nao tem conexao
  return false
end

function circulo_conect (linha, circulo)
  -- Verifica se um circulo e uma linha estão conectados
  
  -- Verify if passed argments are valid
  if type(linha) ~= "table" then return false end
  if type(circulo) ~= "table" then return false end
  
  -- testa pra ver se nao sao o mesmo objeto
  if linha == circulo then return false end
  
  -- trivialmente, testa se algum vertice da linha esta dentro ou toca o circulo
  local raio = (math.abs(circulo.radius) + tolerancia) ^ 2
  for _, pt in ipairs(linha) do
    local dist = (circulo.center.x - pt.x)^2 + (circulo.center.y - pt.y)^2
    if dist < raio then
      return true
    end
  end
  
  -- nao tem conexao
  return false
end

function term_conect (terminal, ligacao)
  -- Verifica a conexao entre um terminal (componente) e uma ligacao (fio)
  
  for _, figura in ipairs(terminal) do
    -- cadzinho.db_print(figura.tipo)
    if figura.linha then
      if linha_conect(figura.linha, ligacao) then
        return true
      end
    elseif figura.circulo then
      if circulo_conect (ligacao, figura.circulo) then
        return true
      end
    end
  end
  
  -- nao tem conexao
  return false
end

function dentro_poligono(pt, polig)
  -- Verifica se um ponto esta dendro do poligono
  
  -- Verifica os parametros passados
  if type(pt) ~= "table" then return false end
  if type(polig) ~= "table" then return false end
  
  local dentro = false
  local ant = #polig -- ponto anterior (indice)
  
  -- metodo de escaneamento por linha horizontal
  for i = 1, #polig do
    if ((polig[i].y > pt.y) ~= (polig[ant].y > pt.y)) and 
    (pt.x < (polig[ant].x - polig[i].x) * (pt.y - polig[i].y) / (polig[ant].y - polig[i].y) + polig[i].x) then
      dentro = not dentro
    end
    ant = i
  end
  
  return dentro
end

function dentro_contorno (ent, contorno)
  -- verifica se um objeto do desenho esta dentro de um contorno
  
  local limite = cadzinho.get_bound(ent) -- pega os limites do objeto (retangulo)
  -- verifica se o retangulo está dentro do contorno
  return dentro_poligono(limite.low, contorno) and dentro_poligono(limite.up, contorno)
end

function pelicanu.atualiza_elems()
  pelicanu.elems = {}
  for i, ent in ipairs(cadzinho.get_all()) do -- varre todos os objetos do desenho
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 0 then
      -- elemento do PELICAnU
      local elem = {}
      local unico = "0"
      local tipo = "NADA"
      local especifico = ""
      if type(ext[1]) == "string" then
        unico = ext[1]:upper() -- identificador unico (muda p/ maiusculo)
      end
      if #ext > 1 then
        tipo = ext[2]:upper() -- tipo de elemento PELICAnU (muda p/ maiusculo)
      end
      if #ext > 2 then
        especifico = ext[3] -- dado especifico do tipo (opcional)
      end
      local unic = tonumber(unico,16)
      elem.id = unic
      elem.ent = ent
      elem.tipo = tipo
      elem.esp = especifico
      
      pelicanu.elems[unic] = elem -- armazena na lista principal
    end
  end
end

function pelicanu.conteudo(id)
  -- pega o conteudo de uma caixa indicada pelo ID
  
  local caixa = pelicanu.elems[id] -- busca o elemento da lista principal
  if caixa == nil then
    return nil
  end
  
  local limite = cadzinho.get_bound(caixa.ent)
  local contorno = {{x=limite.low.x, y=limite.low.y, z=limite.low.z},
    {x=limite.up.x, y=limite.low.y, z=limite.low.z},
    {x=limite.up.x, y=limite.up.y, z=limite.up.z},
    {x=limite.low.x, y=limite.up.y, z=limite.low.z}}

  local tipo = cadzinho.get_ent_typ(caixa.ent)

  if tipo == 'LWPOLYLINE' then
    contorno = cadzinho.get_points(caixa.ent)
  end
  
  -- contorno da caixa, considerando ser uma polyline
  local conteudo = {}
  
  -- varredura em todos elementos
  for el_id, el in pairs(pelicanu.elems) do
    if el ~= caixa then
      -- verifica se o elemento atual está dentro da caixa
      if dentro_contorno(el.ent, contorno) then
        conteudo[#conteudo+1] = el_id -- adiciona-o a lista de retorno
      end
    end
  end
  return conteudo -- retorna o conteudo da caixa
end

function pelicanu.conteudo_todo()
  -- pega o conteúdo (elementos PELICAnU) do desenho inteiro
  
  local conteudo = {}
  for el_id in pairs(pelicanu.elems) do
    conteudo[#conteudo+1] = el_id
  end
  return conteudo
end

function pelicanu.atualiza_unicos()
  -- atualiza os identificadores únicos nos elementos do desenho
  
  for i, ent in ipairs(cadzinho.get_all()) do -- varre todos os objetos do desenho
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 1 then
      -- muda o id unico e grava o no desenho
      cadzinho.edit_ext_i(ent, "PELICANU", 1, cadzinho.unique_id())
      ent:write()
    end
  end
end

function pelicanu.rotulo_caixa(conteudo)
  -- busca rotulo da caixa
  
  local rotulo = nil
  for el_id in pairs(conteudo) do -- varredura do conteudo da caixa
    local el = pelicanu.elems[el_id]
    if el.tipo == "ROTULO" and el.esp == "CAIXA" then -- se for o tipo procurado
      --pega seu texto (considerando que é uma entidade tipo TEXT)
      rotulo = cadzinho.get_text_data(el.ent)
      break
    end
  end
  if rotulo then return rotulo.text
  else return nil
  end
end

function muda_comp_id (comp, id)
-- altera a identificação de um componente do PELICAnU

  -- decompoe o texto para o formato fracao
  local id1 = ""
  local id2 = ""
  if id then
    id1 = string.match(id, '^([^/]*)/?.*$')
    id2 = string.match(id, '/(.*)$')
  end
  local idx1 = 0
  local idx2 = 0
  local ocul1 = false
  local ocul2 = false
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "ID*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag'], "^ID1") then
      idx1 = i
      ocul1 = attr['hidden']
    -- "denominador"
    elseif string.find(attr['tag'], "^ID2") then
      idx2 = i
      ocul2 = attr['hidden']
    end
  end
  
  if idx1 > 0 and idx2 > 0 and id1 and id2 then
    -- formato fracao completa
    id1 = '%%U' .. id1 -- o texto do "numerador" eh subinhado
    cadzinho.edit_attr(comp, idx1, 'ID1', id1, ocul1)
    cadzinho.edit_attr(comp, idx2, 'ID2', id2, ocul2)
    
  -- ID somente no "numerador"
  elseif idx1 > 0 then
    cadzinho.edit_attr(comp, idx1, 'ID1', id, ocul1)
  end
  if idx2 > 0 and not id2 then -- se houver o "denominador", deixa ele vazio
    cadzinho.edit_attr(comp, idx2, 'ID2', '', ocul2)
  end
end

function pega_comp_id (comp)
-- obtem a identificacao de um componente PELICAnU. O retorno eh um texto unico combinado
  local id = false
  local id1 = ''
  local id2 = ''
  local idx1 = 0
  local idx2 = 0
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "ID*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag'], "^ID1") then 
      idx1 = i
      id1 = attr['value']
      id1 = string.gsub(id1, '(%%%%%a)', '') -- retira as marcas de formatacao, se houver
    end
    -- "denominador"
    if string.find(attr['tag'], "^ID2") then
      idx2 = i
      id2 = attr['value']
      id2 = string.gsub(id2, '(%%%%%a)', '') -- retira as marcas de formatacao, se houver
    end
  end
  
  if idx1 > 0 then
    if idx2 > 0 and #id2 > 0 then
      id = id1 .. '/' .. id2 -- combina as duas IDs num texto unico
    else
      id = id1 -- so possui a ID principal
    end
  end
  
  return id
end

function pega_attrib (ent)
-- obtem os dados de uma entidade tipo INSERT (bloco). Retorna uma tabela
  local dados = {}
  local tipo = cadzinho.get_ent_typ(ent)
  -- soh aceita INSERT
  if tipo ~= 'INSERT' then
    return dados
  end
  -- varre os elementos ATTRIB da entidade, cadastrando-as na tabela
  local attrs = cadzinho.get_attribs(ent)
  for i, attr in ipairs(attrs) do
    dados[attr.tag] = attr.value
  end
  
  return dados
end

function pega_comp_tipo(comp)
  local dados = pega_attrib(comp)
  return dados.TIPO
end

function pega_engate(comp)
  local dados = pega_attrib(comp)
  return dados.ENGATE
end

function muda_engate(comp, engate, num, desenho, folha, descr)
  local eng_o = false
  local eng_i = 0
  local num_o = false
  local num_i = 0
  local des_o = false
  local des_i = 0
  local fl_o = false
  local fl_i = 0
  local descr_o = false
  local descr_i = 0

  -- varre os elementos ATTRIB da entidade, buscando a etiqueta "ENGATE"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag'], "ENGATE") then
      eng_i = i
      eng_o = attr['hidden']
    elseif string.find(attr['tag'], "E1") then
      num_i = i
      num_o = attr['hidden']
    elseif string.find(attr['tag'], "REF1") then
      des_i = i
      des_o = attr['hidden']
    elseif string.find(attr['tag'], "REF2") then
      fl_i = i
      fl_o = attr['hidden']
    elseif string.find(attr['tag'], "DESCR1") then
      descr_i = i
      descr_o = attr['hidden']
  end
  end
  
  if eng_i > 0 and engate then
    cadzinho.edit_attr(comp, eng_i, 'ENGATE', engate, eng_o)
  end
  if num_i > 0 and num then
    cadzinho.edit_attr(comp, num_i, 'E1', num, num_o)
  end
  if des_i > 0 and desenho then
    cadzinho.edit_attr(comp, des_i, 'REF1', desenho, des_o)
  end
  if fl_i > 0 and folha then
    cadzinho.edit_attr(comp, fl_i, 'REF2', folha, fl_o)
  end
  if descr_i > 0 and descr then
    cadzinho.edit_attr(comp, descr_i, 'DESCR1', descr, descr_o)
  end

end

function rotacao (pt, ang)
--funcao de rotacao de um ponto no plano cartesiano, angulo em graus
  local cos = math.cos(ang * math.pi / 180)
  local sen = math.sin(ang * math.pi / 180)
  
  local x = pt.x * cos - pt.y * sen
  local y = pt.x * sen + pt.y * cos
  pt.x = x
  pt.y = y
end

function pega_terminais (comp)
  -- pega os pontos de conexao de um componente - terminais
  local conexoes = {}
  
  -- nome do bloco do componente
  local nome_b = cadzinho.get_blk_name (comp)
  if not nome_b then return conexoes end -- erro
  
  -- dados do componente (ponto de insercao, escalas, rotacao)
  local dados_comp = cadzinho.get_ins_data (comp)
  if not dados_comp then return conexoes end -- erro
  
  -- varre os elementos internos do bloco, procurando conexoes
  for _, ent in ipairs(cadzinho.get_blk_ents(nome_b)) do
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 2 then
      local tipo = ext[2]:upper() -- tipo de elemento PELICAnU (muda p/ maiusculo)
      if tipo == "TERMINAL" then
        --identificacao do terminal
        local t_id = ext[3]:upper() -- dado especifico do tipo (muda p/ maiusculo)
        
        local terminal = conexoes[t_id]
        if not terminal then terminal = {} end
        local figura = cadzinho.get_ent_typ(ent)
        -- soh aceita linha ou circulo como terminal
        if figura == 'LINE' then
          local dados = {}
          dados.tipo = 'linha'
          --pega os pontos da linha
          local pontos = cadzinho.get_points(ent)
          for _, pt in ipairs(pontos) do
            -- executa a trasformações geometricas, conforme o componente
            rotacao(pt, dados_comp.rot)
            pt.x = pt.x * dados_comp.scale.x
            pt.y = pt.y * dados_comp.scale.y
            
            pt.x = pt.x + dados_comp.pt.x
            pt.y = pt.y + dados_comp.pt.y
          end
          
          dados.linha = pontos
          terminal[#terminal + 1] = dados
        elseif figura == 'CIRCLE' then
          local dados = {}
          dados.tipo = 'circulo'
          -- pega os parametros do circulo
          local circulo = cadzinho.get_circle_data(ent)
          -- executa a trasformações geometricas, conforme o componente
          rotacao(circulo.center, dados_comp.rot)
          circulo.center.x = circulo.center.x * dados_comp.scale.x
          circulo.center.y = circulo.center.y * dados_comp.scale.y
          circulo.radius = math.abs(circulo.radius * dados_comp.scale.x)
          circulo.center.x = circulo.center.x + dados_comp.pt.x
          circulo.center.y = circulo.center.y + dados_comp.pt.y
          
          dados.circulo = circulo
          terminal[#terminal + 1] = dados
        end
        
        conexoes[t_id] = terminal
      end
    end
  end
  
  return conexoes
end

function info_terminais (comp)
-- obtem as informacoes textuais dos terminais de um componente
-- par indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1
  local terminais = {}
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "T*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local t_num = string.match(attr['tag'], "^T(%d)")
    if t_num then
      terminais[tonumber(t_num)] = attr['value']
    end
  end
  
  return terminais
end

function muda_terminais (comp, terminais)
-- altera as informacoes textuais dos terminais de um componente
-- entrada: tabela com pares indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1

  local ocul = false
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "T*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local t_num = string.match(attr['tag'], "^T(%d)")
    if t_num then
      -- confronta o indice encontrado com a tabela
      local term = terminais[tonumber(t_num)]
      if term then
        -- modifica o indice encontrado
        ocul = attr['hidden'] -- mantem a conficuracao de "oculto"
        cadzinho.edit_attr(comp, i, 'T' .. t_num, term, ocul)
      end
    end
  end
end

function obtem_caixas()
  local caixas = {}
  local SetLib = require("Set") -- biblioteca para matematica de conjuntos
  local caixa
  local conteudo
  
  -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
  --pelicanu.atualiza_unicos()
  -- atualiza a lista principal com os elementos
  --pelicanu.atualiza_elems()
  
  -- caixa "desenho" para armazenar os elementos órfãos
  caixa = {}
  conteudo = pelicanu.conteudo_todo()
  caixa['nome'] = ""
  caixa['conteudo'] = SetLib.new(conteudo)
  caixa['filhas'] = {}
  caixa['tipo'] = 'ARQUIVO'
  caixas[0] = caixa
  
  -- varre os elementos, cadastrando as caixas existentes no desenho
  for el_id, el in pairs(pelicanu.elems) do
    if el.tipo == "CAIXA" then
      caixa = {}
      conteudo = pelicanu.conteudo(el_id)
      caixa['nome'] = ""
      caixa['conteudo'] = SetLib.new(conteudo)
      caixa['filhas'] = {}
      caixa['tipo'] = el.esp
      caixas[el_id] = caixa
    end
  end
  
  -- repassa as caixas, cadastrando as caixas aninhadas
  for _, caixa in pairs(caixas) do
    for el_id in pairs(caixa.conteudo) do
      local el = pelicanu.elems[el_id]
      if el.tipo == "CAIXA" then
        caixa.filhas[#caixa.filhas+1] = caixas[el_id]
      end
    end
  end
  
  -- com as caixas cadastradas, os conteúdos estarao sobrepostos ("repetidos")
  -- ajusta os conteudos, ficando cada caixa com seus elementos exclusivos
  for _, caixa in pairs(caixas) do
    for _,filha in ipairs(caixa.filhas) do
      caixa.conteudo = caixa.conteudo - filha.conteudo
    end
  end
  
  -- por fim, pega o nome de cada caixa
  for id, caixa in pairs(caixas) do
    caixa.nome = pelicanu.rotulo_caixa(caixa.conteudo)
  end
  
  return caixas
end

function obtem_barras ()
  local componentes = {}
  local fios = {}
  local barras = {}
  
  -- varre os elementos, cadastrando os componentes e os fios
  for el_id, el in pairs(pelicanu.elems) do
    if el.tipo == "COMPONENTE" then
      local componente = {}
      componente.el = el
      componente.term =  pega_terminais (el.ent)
      componente.conexoes = {}
      componente.bloco = cadzinho.get_blk_name (el.ent)
      componentes[el_id] = componente
    elseif el.tipo == "LIGACAO" then
      local fio = {}
      fio.el = el
      fio.pontos = cadzinho.get_points(el.ent)
      fios[el_id] = fio
    end
  end
  
  -- varre os componentes para cadastrar as conexoes dos terminais e fios
  for comp_id, componente in pairs(componentes) do
    for t_id, terminal in pairs(componente.term) do
      for l_id, fio in pairs(fios) do
        if term_conect (terminal, fio.pontos) then
          local conexoes ={}
          if componente.conexoes[t_id] then
            conexoes = componente.conexoes[t_id]
          end
          conexoes[#conexoes + 1] = l_id
          componente.conexoes[t_id] = conexoes
        end
      end
    end
  end
  
  for comp_id, componente in pairs(componentes) do
    for t_id, cnx in pairs(componente.conexoes) do
      local fio_ant = false
      for i, fio_id in ipairs(cnx) do
        local fio = fios[fio_id]
        if fio_ant then
          -- adiciona o fio a barra existente (mesmo terminal)
          fio.barra = fio_ant.barra
          fio.barra.fios[#fio.barra.fios+1] = fio
        else
          if fio.barra then
            -- adiciona o terminal a barra existente (fio ligado a outro componente)
            local terminal = {comp = comp_id, term = t_id}
            fio.barra.terminais[#fio.barra.terminais+1] = terminal
          else
            -- Nova barra
            fio.barra = {}
            fio.barra.fios = {fio}
            fio.barra.terminais = {{comp = comp_id, term = t_id}}
            barras[fio.barra] = fio.barra
          end
        end
        fio_ant = fio
      end
    end
  end
  
  for fio_id1, fio1 in pairs(fios) do
    for fio_id2, fio2 in pairs(fios) do
      if fio1 ~= fio2 and linha_conect (fio1.pontos, fio2.pontos) then
        if fio1.barra and not fio2.barra then
          fio1.barra.fios[#fio1.barra.fios+1] = fio2
          fio2.barra = fio1.barra
        elseif not fio1.barra and fio2.barra then
          fio2.barra.fios[#fio2.barra.fios+1] = fio1
          fio1.barra = fio2.barra
        elseif fio1.barra and fio2.barra and fio1.barra ~= fio2.barra then
          barras[fio2.barra] = nil
          -- transfere os fios e terminais
          for _, fio in pairs(fio2.barra.fios) do
            fio1.barra.fios[#fio1.barra.fios+1] = fio
          end
          for _, fio in pairs(fio2.barra.terminais) do
            fio1.barra.terminais[#fio1.barra.terminais+1] = fio
          end
          for _, fio in pairs(fio1.barra.fios) do
            fio.barra = fio1.barra
          end
        elseif not fio1.barra and not fio2.barra then
          -- Nova barra
          fio1.barra = {}
          fio1.barra.fios = {fio1, fio2}
          fio1.barra.terminais = {}
          fio2.barra = fio1.barra
          barras[fio1.barra] = fio1.barra
        end
      end
    end
  end
  
  return barras
end

function teste()
  cadzinho.db_print ("Teste de criacao de banco de dados")
  
  -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
  pelicanu.atualiza_unicos()
  -- atualiza a lista principal com os elementos
  pelicanu.atualiza_elems()
  
  bd_novo('pelicanu.db')
  local bd = sqlite.open('pelicanu.db')
  local caixas = obtem_caixas()
  for id, caixa in pairs(caixas) do
    --cadzinho.db_print (caixa.nome)
    local pai = id
    if type(pai) == 'number' then
      pai = string.format('%d', pai)
    else
      pai = '0'
    end
    if not pai then pai = 'NULL' end
    for el_id, _ in pairs(caixa.conteudo) do
      local el = pelicanu.elems[el_id]
      --cadzinho.db_print ("    " .. el.tipo)
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
        --cadzinho.db_print ("    " .. bloco, comp_id)
        
        bd:exec ("INSERT INTO componentes VALUES("..
          string.format('%d', el_id) ..", "..
          comp_tipo ..", "..
          bloco ..", "..
          comp_id ..", "..
          pai..", "..
          string.format('%f', dados.pt.x) ..", "..
          string.format('%f', dados.pt.y) ..
          ");")
        for t_id, t in pairs(terms) do
          bd:exec ("INSERT INTO terminais VALUES("..
            string.format('%d', el_id) ..", "..
            string.format('%d', t_id) ..", '"..
            t .."');")
        end

        if comp_tipo == "'ENGATE'" then
          local engate = pega_engate(el.ent)
          if not engate then engate = 'NULL'
          else engate = "'"..engate.."'" end
          bd:exec ("INSERT INTO engates VALUES("..
          string.format('%d', el_id) ..", "..engate..")")
        end
      elseif el.tipo == "CAIXA" then
        local sub_caixa = caixas[el_id]
        if sub_caixa then
          local nome = sub_caixa.nome
          if not nome then nome = 'NULL' end
          local tipo = sub_caixa.tipo
          if not tipo then tipo = 'NULL' end
          --cadzinho.db_print ("    " .. nome, tipo)
          
          bd:exec ("INSERT INTO caixas VALUES("..
            string.format('%d', el_id) ..", '"..
            nome .."', '"..
            tipo .."', "..
            pai..
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
            if dados.pfl then dados.pfl = "'" .. dados.pfl .. "'" 
            else dados.pfl = 'NULL' end
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
             dados.pfl ..
            ");")
          end
        end
      end
      
    end
  end
  
  
  
  
  local barras = obtem_barras()
  --cadzinho.db_print ("Num barras=", #barras)
  for i, barra in pairs(barras) do
    if not barra.id then
      barra.id = cadzinho.unique_id()
    end
    --cadzinho.db_print ("Barra", i, "fios=", #barra.fios)
    for j, term in ipairs(barra.terminais) do
      --cadzinho.db_print ("  " .. string.format('%x', term.comp), term.term)
      bd:exec ("INSERT INTO barras VALUES('"..
        barra.id .."', "..
        string.format('%d', term.comp) ..", "..
        string.match(term.term, "^T(%d)")..
        ");")
    end
  end
  
  
  --
  bd:close()
  cadzinho.db_print ("----- Teste concluido  ------")
end

function grava_pl_comp ()
  cadzinho.db_print ("----- Grava componentes  ------")
  -- cria o arquivo de planilha
  os.remove("componentes.xlsx") -- deleta o arquivo existente
  local planilha = excel:new("componentes.xlsx")
  local aba = planilha:add_worksheet("Componentes")
  --aba:protect() -- aba protegida
  local protegido = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = 'silver',
    })
  local desprotegido = planilha:add_format({locked = false, border = 1})
  
  local m_p = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = 'silver',
    valign = "vcenter",
    })
  local m_d = planilha:add_format({locked = false, valign = "vcenter", border = 1})
  
  -- tamanho das colunas
  aba:set_column(0, 0, 20)
  aba:set_column(1, 1, 11)
  aba:set_column(2, 2, 20)
  aba:set_column(3, 4, 11)
  aba:set_column(5, 5, 18)
  aba:set_column(6, 6, 4)
  aba:set_column(7, 7, 11)
  
  -- primeira linha de titulo
  local tit_p = planilha:add_format({
    border = 6,
    locked = true,
    pattern = 1,
    bg_color = 'silver',
    bold = true
    })
  local tit_d = planilha:add_format({locked = false, bold = true, border = 6})
  aba:write(0, 0, 'ID Unico', tit_p)
  aba:write(0, 1, 'Painel', tit_p)
  aba:write(0, 2, 'Componente', tit_p)
  aba:write(0, 3, 'Modulo', tit_p)
  aba:write(0, 4, 'Parte', tit_d)
  aba:write(0, 5, 'Tipo', tit_p)
  aba:write(0, 6, 'T id', tit_p)
  aba:write(0, 7, 'Terminal', tit_d)
  
  local lin = 1
  
  -- variaveis para agupamento (merge) das celulas repetidas
  local ini_comp = 1
  local ini_unico = 1
  local ini_modulo = 1
  local ini_painel = 1
  local comp_ant = false
  local tipo_ant = false
  local unico_ant = false
  local parte_ant = false
  local modulo_ant = nil
  local painel_ant = nil
  
  -- abre e le o banco de dados
  local bd = sqlite.open('pelicanu.db')
  for linha in bd:cols('select * from comp_term') do -- para cada linha do BD
    -- grava na planilha cada celula separada, a principio
    aba:write(lin, 0, string.format('%X', linha.unico), protegido)
    aba:write(lin, 1, linha. painel, protegido)
    aba:write(lin, 2, linha.componente, protegido)
    aba:write(lin, 3, linha.modulo, protegido)
    if linha.parte then
      aba:write(lin, 4, linha.parte, desprotegido)
    else aba:write(lin, 4, linha.parte, protegido) end
    aba:write(lin, 5, linha.tipo, protegido)
    aba:write(lin, 6, linha.num, protegido)
    aba:write(lin, 7, linha.terminal, desprotegido)
    
    -- agrupa as celulas repetidas
    if painel_ant ~= linha.painel then
      if (lin - ini_painel) > 1 then
        aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
      end
      ini_painel = lin
    end
    if comp_ant ~= linha.componente then
      if (lin - ini_comp) > 1 then
        aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
      end
      ini_comp = lin
    end
    if modulo_ant ~= linha.modulo or comp_ant and comp_ant ~= linha.componente then
      if (lin - ini_modulo) > 1 then
        aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
      end
      ini_modulo = lin
    end
    if unico_ant ~= linha.unico then -- o ID unico eh o criterio para agrupar blocos e partes
      if (lin - ini_unico) > 1 then
        aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
        aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
        if parte_ant then
          aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
        else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
      end
      ini_unico = lin
    end
    
    comp_ant = linha.componente
    unico_ant = linha.unico
    tipo_ant = linha.tipo
    parte_ant = linha.parte
    modulo_ant = linha.modulo
    painel_ant = linha.painel
    
    -- proxima linha
    lin = lin + 1
  end
  -- finaliza os agrupamentos, se necessario
  if (lin - ini_painel) > 1 then
    aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
  end
  if (lin - ini_comp) > 1 then
    aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
  end

  if (lin - ini_modulo) > 1 then
    aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
  end
  
  if (lin - ini_unico) > 1 then
    aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
    aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
    if parte_ant then
      aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
    else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
  end
  --
  bd:close()
  
  planilha:close()
  cadzinho.db_print ("----- Concluido  ------")
end


function le_pl_comp()
  cadzinho.db_print ("----- Atualiza componentes  ------")
  local leitor = require 'xlsx_lua'
  
  local pl_comp = false
  -- le o arquivo excel
  local workbook = leitor.open('componentes.xlsx')
  -- procura pela aba 'Componentes'
  if type(workbook) == 'table' then pl_comp = workbook.sheets['Componentes'] end
  
  if type(pl_comp) == 'table' then
    -- expande as celulas mescladas (replica o valor pra todas celulas do grupo)
    leitor.expand_merge (pl_comp)
    
    -- abre o banco de dados
    local bd = sqlite.open('pelicanu.db')
    
    -- atualiza a lista principal com os elementos
    pelicanu.atualiza_elems()
    
    -- cria uma lista com os componentes
    local componentes = {}
    for el_id, el in pairs(pelicanu.elems) do
      if el.tipo == "COMPONENTE" then
        local componente = {}
        componente.ent = el.ent
        componente.id = pega_comp_id(el.ent)
        componente.term =  info_terminais (el.ent)
        componentes[el_id] = componente
      end
    end
    
    -- varre a planilha
    for _, lin in ipairs(pl_comp.dim.rows) do
      if lin ~= 1 then -- ignora a primeira linha com o titulo
        local unico = tonumber(pl_comp.data[lin]['A'],16) -- coluna A eh o id unico
        -- atualiza o banco de dados com as informacoes lidas
        bd:exec("UPDATE terminais SET terminal = '" ..
          tostring(pl_comp.data[lin]['H']) ..
          "' WHERE componente = " .. string.format('%d', unico) ..
          " AND id = " .. tostring(pl_comp.data[lin]['G']) .. ";")

        if pl_comp.data[lin]['E'] then
          bd:exec("UPDATE componentes SET id = '" ..
          tostring(pl_comp.data[lin]['E']) ..
          "' WHERE unico = " .. string.format('%d', unico) .. ";")
        end
        -- atualiza na lista de componentes
        local componente = componentes[unico]
        if type(componente) == 'table' then
          if pl_comp.data[lin]['E'] then
            componente.id = tostring(pl_comp.data[lin]['E'])
          end
          componente.term[tonumber(pl_comp.data[lin]['G'])] = tostring(pl_comp.data[lin]['H'])
        end
      end
    end
    
    -- atualiza no desenho
    for _, componente in pairs(componentes) do
      muda_comp_id (componente.ent, componente.id)
      muda_terminais(componente.ent, componente.term)
      componente.ent:write()
    end
    
    bd:close()
  end
  
  cadzinho.db_print ("----- Concluido  ------")
end

function atualiza_engates()
  -- atualiza a lista principal com os elementos
  pelicanu.atualiza_elems()
  
  --abre o banco de dados
  local bd = sqlite.open('pelicanu.db')
  for linha in bd:cols('select * from engate_par') do -- para cada linha do BD
    local unico = tonumber(linha.unico)
  local el = pelicanu.elems[unico]
  local num = 'E' .. linha.e_num
  local des = linha.vai_des
  local fl = linha.vai_fl
  local descr = nil
  
  if des == 'NESTE' then
    descr = ''
  end
  if fl == 'NESTA FL.' then
    des = fl
    fl = ''
  elseif fl then
    fl = 'FL. ' .. fl
  end
  muda_engate(el.ent, nil, num, des, fl, descr)
  el.ent:write()
  end
  bd:close()
end

function obtem_tipo_comp ()
  lista_comp_tipo = {}
  g_tipo_comp = {value = g_tipo_comp.value, ""}

  -- diretorio para buscar os componentes na biblioteca (se chama "componentes" :P)
  local dir_comp = biblioteca .. "componentes"

  -- lista os subdiretorios como tipo de componentes
  local dir = fs.dir(dir_comp)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    if dir[i].is_dir then
      lista_comp_tipo[#lista_comp_tipo+1] = dir[i].name
      g_tipo_comp[#g_tipo_comp+1] = dir[i].name
    end
  end
  if g_tipo_comp.value > #lista_comp_tipo+1 then
    g_tipo_comp.value = 1
  end
end

function obtem_lista_comp ()
  lista_comp = {}
  lista_comp_o = {}

  -- diretorio para buscar os componentes na biblioteca (se chama "componentes" :P)
  local dir_comp = biblioteca .. "componentes" ..
        fs.dir_sep .. g_tipo_comp[g_tipo_comp.value]

  local dir = fs.dir(dir_comp)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    local ext = extensao(dir[i].name)
    if type(ext) == "string" and dir[i].is_dir == false then
      if ext:upper() == ".DXF" then
        lista_comp[nome_arq(dir[i].name)] = diretorio(dir_comp) .. dir[i].name
        lista_comp_o[#lista_comp_o+1] = nome_arq(dir[i].name)
      end
    end
  end
  table.sort(lista_comp_o)

end

function obtem_lista_formato ()
  lista_formato = {}
  lista_formato_o = {}

  -- diretorio para buscar os formatos na biblioteca (se chama "formatos" :P)
  local dir_fmt = biblioteca .. "formatos"

  local dir = fs.dir(dir_fmt)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    local ext = extensao(dir[i].name)
    if type(ext) == "string" and dir[i].is_dir == false then
      if ext:upper() == ".DXF" then
        lista_formato[nome_arq(dir[i].name)] = diretorio(dir_fmt) .. dir[i].name
        lista_formato_o[#lista_formato_o+1] = nome_arq(dir[i].name)
      end
    end
  end
  table.sort(lista_formato_o)

end


-- ============= Funções dinâmicas GUI ==================
function ligacao_dyn(event)
  -- funcao interativa para criacao de uma ligação
  
  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Nova ligacao")
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Primeiro ponto')
    if event.type == 'enter' then -- usuario entra o primeiro ponto
      num_pt = num_pt + 1
      
    elseif event.type == 'cancel' then  -- usuario cancela
      -- sai da funcao
      cadzinho.set_color("by layer")
      cadzinho.set_lw("by layer")
      cadzinho.set_ltype("bylayer")
      cadzinho.stop_dynamic()
    end
  else
    cadzinho.nk_label('Proximo ponto')
    cadzinho.set_ltype("Continuous") -- linha continua
    cadzinho.set_color(1) -- cor vermelha
    
    -- o elemento "ligacao" eh uma linha simples
    local ligacao = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0)
    cadzinho.ent_draw(ligacao) -- mostra o desenho temporario
    
    if event.type == 'enter' then -- usuario entra o segundo ponto
      if ligacao then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        -- grava o elemento no desenho
        cadzinho.add_ext(ligacao, "PELICANU", {cadzinho.unique_id(), "LIGACAO", "-"})
        ligacao:write()
        
        -- continua - considera o ponto atual como o primeiro da proxima ligação
        pts[1].x = event.x
        pts[1].y = event.y
      end
    elseif event.type == 'cancel' then -- usuario cancela
      -- volta ao primeiro ponto
      num_pt = 1
    end
  
  end
end

function caixa_dyn(event)
  -- funcao interativa para criacao de uma caixa
  
  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Nova caixa")
  cadzinho.nk_layout(20, 2)
  cadzinho.nk_label("ID:")
  cadzinho.nk_edit(g_caixa_id)
  cadzinho.nk_label("Tipo:")
  cadzinho.nk_combo(g_caixa_tipo)
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Primeiro ponto')
    if event.type == 'enter' then
      num_pt = num_pt + 1
    elseif event.type == 'cancel' then
      -- sai da funcao
      cadzinho.set_color("by layer")
      cadzinho.set_lw("by layer")
      cadzinho.set_ltype("bylayer")
      cadzinho.stop_dynamic()
    end
  else
    cadzinho.nk_label('Proximo ponto')
    
    -- o elemento "caixa" principal -  eh uma polyline no formato retangulo
    cadzinho.set_ltype("Dashdot") -- linha traco-ponto
    cadzinho.set_color(3) -- cor verde
    local caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0, pts[2].x, pts[1].y, 0)
    cadzinho.pline_append(caixa, pts[2].x, pts[2].y, 0)
    cadzinho.pline_append(caixa, pts[1].x, pts[2].y, 0)
    cadzinho.pline_close(caixa, true)
    --[[
    -- outro formato: poligono arbitrario fechado
    for i = 3, num_pt do
      cadzinho.pline_append(caixa, pts[i].x, pts[i].y, 0)
    end]]--
    if (caixa) then cadzinho.ent_draw(caixa) end
    
    -- identificador da caixa - eh uma entidade TEXT e um elemento tipo "rotulo"
    -- posicao do texto - canto superior direito
    local tx = pts[2].x
    if tx < pts[1].x then tx = pts[1].x end
    local ty = pts[2].y
    if ty < pts[1].y then ty = pts[1].y end
    local caixa_id = cadzinho.new_text(tx-0.6, ty-0.6, g_caixa_id.value, 2.0, "right", "top")
    
    -- desenha um retangulo simples em volta do texto do identificador
    local retan_txt = nil
    if caixa_id then 
      cadzinho.ent_draw(caixa_id) 
      local cx_tx = cadzinho.get_bound(caixa_id)
      cx_tx.low.x = cx_tx.low.x - 0.3
      cx_tx.low.y = cx_tx.low.y - 0.3
      cx_tx.up.x = cx_tx.up.x + 0.3
      cx_tx.up.y = cx_tx.up.y + 0.3
      cadzinho.set_ltype("Continuous") -- linha continua
      retan_txt = cadzinho.new_pline(cx_tx.low.x, cx_tx.low.y, 0, cx_tx.up.x, cx_tx.low.y, 0)
      cadzinho.pline_append(retan_txt, cx_tx.up.x, cx_tx.up.y, 0)
      cadzinho.pline_append(retan_txt, cx_tx.low.x, cx_tx.up.y, 0)
      cadzinho.pline_close(retan_txt, true)
    end
    if retan_txt then cadzinho.ent_draw(retan_txt) end
    
    if event.type == 'enter' then
      cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
      
      if caixa then
        cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "CAIXA", g_caixa_tipo[g_caixa_tipo.value]})
        caixa:write()
      end
      if caixa_id then
        cadzinho.add_ext(caixa_id, "PELICANU", {cadzinho.unique_id(), "ROTULO", "CAIXA"})
        caixa_id:write()
      end
      if retan_txt then retan_txt:write() end
      
      num_pt = 1
      --[[
      -- outro formato: poligono arbitrario fechado
      num_pt = num_pt + 1]]--
      
    elseif event.type == 'cancel' then
      --[[
      -- outro formato: poligono arbitrario fechado
      if caixa then
        cadzinho.add_ext(caixa, "PELICANU", {"CAIXA", g_caixa_id.value})
        caixa:write()
      end]]--
      num_pt = 1
    end
  
  end
end

function terminal_dyn(event)
  -- funcao interativa para criacao de um terminal, no modo de edição de componente

  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Adiciona um terminal")
  cadzinho.nk_propertyi("Numero", g_term_num, 0, 100)
  cadzinho.nk_layout(20, 2)
  cadzinho.nk_label("Nome:")
  cadzinho.nk_edit(g_term_nome)
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  local texto = '#T' .. string.format('%d', g_term_num.value) .. '$' .. g_term_nome.value
  local term_id = cadzinho.new_text(pts[1].x, pts[1].y, texto, 2.0, "left", "middle")
  if term_id then cadzinho.ent_draw(term_id) end
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Posicione o texto')
    if event.type == 'enter' then
      num_pt = num_pt + 1
    elseif event.type == 'cancel' then
      cadzinho.stop_dynamic()
    end
  else
    cadzinho.nk_label('Confirme')
    if event.type == 'enter' then
      local sel = cadzinho.get_sel()
      if (#sel > 0) and term_id then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        for i = 1, #sel do
          local tipo = cadzinho.get_ent_typ(sel[i])
          -- soh aceita linha ou circulo como terminal
          if tipo == 'LINE' or tipo == 'CIRCLE' then
            --verifica se o elemento jah possui marcador
            local ext = cadzinho.get_ext (sel[i], "PELICANU") -- procura pelo marcador extendido
            if #ext > 1 then
              cadzinho.del_ext_all (sel[i], "PELICANU") -- apaga os dados existentes
            end
            cadzinho.add_ext(sel[i], "PELICANU", {cadzinho.unique_id(), "TERMINAL", 'T' .. string.format('%d', g_term_num.value)})
            sel[i]:write()
          end
        end
        term_id:write()
      end
      cadzinho.clear_sel()
      cadzinho.stop_dynamic()
      num_pt = 1
    elseif event.type == 'cancel' then
      num_pt = 1
    end
  end
end

function engate_dyn(event)
  -- funcao interativa para criacao de um engate, no modo de edição de componente

  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Adiciona um engate")
  cadzinho.nk_propertyi("Numero", g_eng_num, 0, 100)
  cadzinho.nk_layout(20, 2)
  cadzinho.nk_label("Nome:")
  cadzinho.nk_edit(g_eng_nome)
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  local texto = '#E' .. string.format('%d', g_eng_num.value) .. '$' .. g_eng_nome.value
  local eng_id = cadzinho.new_text(pts[1].x, pts[1].y, texto, 2.0, "center", "middle")
  if eng_id then cadzinho.ent_draw(eng_id) end
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Posicione o texto')
    if event.type == 'enter' then
      num_pt = num_pt + 1
    elseif event.type == 'cancel' then
      cadzinho.stop_dynamic()
    end
  else
    cadzinho.nk_label('Confirme')
    if event.type == 'enter' then
      local sel = cadzinho.get_sel()
      if (#sel > 0) and eng_id then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        for i = 1, #sel do
          --verifica se o elemento jah possui marcador
          local ext = cadzinho.get_ext (sel[i], "PELICANU") -- procura pelo marcador extendido
          if #ext > 1 then
            cadzinho.del_ext_all (sel[i], "PELICANU") -- apaga os dados existentes
          end
          cadzinho.add_ext(sel[i], "PELICANU", {cadzinho.unique_id(), "ENGATE", 'E' .. string.format('%d', g_term_num.value)})
          sel[i]:write()
        end
        eng_id:write()
      end
      cadzinho.clear_sel()
      cadzinho.stop_dynamic()
      num_pt = 1
    elseif event.type == 'cancel' then
      num_pt = 1
    end
  end
end

function componente_dyn(event)
  -- funcao interativa para acrescentar um componente ao desenho, de uma biblioteca

  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Adiciona um componente")
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Escolha o componente')
    cadzinho.nk_combo(g_tipo_comp)
    cadzinho.nk_layout(150, 1)
    if cadzinho.nk_group_begin("Biblioteca", false, true, true) then
      cadzinho.nk_layout(20, 1)
      --[[for nome, caminho in pairs(lista_comp) do      
        if cadzinho.nk_button(nome) then
          g_componente.value = nome
        end
      end]]--
      obtem_lista_comp()
      for _, nome in ipairs(lista_comp_o) do      
        if cadzinho.nk_button(nome) then
          g_componente.value = nome
        end
      end
      cadzinho.nk_group_end()
    end
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("Nome:")
    cadzinho.nk_edit(g_componente)
    if cadzinho.nk_button("Insere") then
      if type(lista_comp[g_componente.value]) == 'string' then
        local comp = cadzinho.new_insert(g_componente.value,
          pts[num_pt].x, pts[num_pt].y)
        if comp == nil then
          if cadzinho.new_block_file(lista_comp[g_componente.value],
            g_componente.value, "componente PELICAnU ".. g_componente.value,
            true, '#', '*', '$', '?', 0, 0, 0)
          then
            comp = cadzinho.new_insert(g_componente.value,
              pts[num_pt].x, pts[num_pt].y)
          end
        end
        if comp then
          local terminais = info_terminais(comp)
          g_terminais = {}
          for i, term in pairs(terminais) do
            g_terminais[i] = {value = term}
          end
          num_pt = 2
        end
      end
    end
  else
    local comp = cadzinho.new_insert(g_componente.value, pts[num_pt].x, pts[num_pt].y)
    if comp then cadzinho.ent_draw(comp) end
    cadzinho.nk_label(g_componente.value)
    cadzinho.nk_label('Entre o ponto')
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_comp_id)
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Terminais:")
    cadzinho.nk_layout(20, 2)
    for i, term in pairs(g_terminais) do
      cadzinho.nk_label(tostring(i)..':')
      cadzinho.nk_edit(term)
    end
    if event.type == 'enter' then
      muda_comp_id (comp, g_comp_id.value)
      local terminais = {}
      for i, term in pairs(g_terminais) do
        terminais[i] = term.value
      end
      muda_terminais(comp, terminais)
      cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
      cadzinho.add_ext(comp, "PELICANU", {cadzinho.unique_id(), "COMPONENTE"})
      comp:write()
      cadzinho.clear_sel()
      --cadzinho.stop_dynamic()
      --num_pt = 1
    elseif event.type == 'cancel' then
      num_pt = 1
    end
  end
  
end

function formato_dyn(event)
  -- funcao interativa para acrescentar um componente ao desenho, de uma biblioteca

  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Adiciona um formato")
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Escolha o formato')
    cadzinho.nk_layout(150, 1)
    if cadzinho.nk_group_begin("Formatos", true, true, true) then
      cadzinho.nk_layout(20, 1)
      for _, nome in ipairs(lista_formato_o) do      
        if cadzinho.nk_button(nome) then
          g_formato.value = nome
        end
      end
      cadzinho.nk_group_end()
    end
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("Nome:")
    cadzinho.nk_edit(g_formato)
    if cadzinho.nk_button("Insere") then
      if type(lista_formato[g_formato.value]) == 'string' then
        local fmt = cadzinho.new_insert(g_formato.value, pts[num_pt].x, pts[num_pt].y)
        if fmt == nil then
          if cadzinho.new_block_file(lista_formato[g_formato.value],
            g_formato.value, "formato PELICAnU ".. g_formato.value,
            true, '#', '*', '$', '?', 0, 0, 0) then

            fmt = cadzinho.new_insert(g_formato.value, pts[num_pt].x, pts[num_pt].y)
          end
        end
        if fmt then num_pt = 2 end
      end
    end

    
    if event.type == 'enter' then
      num_pt = num_pt + 1
    elseif event.type == 'cancel' then
      cadzinho.stop_dynamic()
    end
  else
    local fmt = cadzinho.new_insert(g_formato.value, pts[num_pt].x, pts[num_pt].y)
    if fmt then cadzinho.ent_draw(fmt) end
    cadzinho.nk_label(g_formato.value)
    cadzinho.nk_label('Entre o ponto')
    if event.type == 'enter' then
      cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
      cadzinho.add_ext(fmt, "PELICANU", {cadzinho.unique_id(), "CAIXA", "DESENHO"})
      fmt:write()
      cadzinho.clear_sel()
      --cadzinho.stop_dynamic()
      --num_pt = 1
    elseif event.type == 'cancel' then
      num_pt = 1
    end
  end
  
end


function edita_dyn(event)
  -- funcao interativa para editar o id de um componente

  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Edita a ID")
  
  -- armazena o ponto atual na lista
  pts[num_pt] = {}
  pts[num_pt].x = event.x
  pts[num_pt].y = event.y
  
  local sel = cadzinho.get_sel()
  if #sel < 1 then
    num_pt = 1
    cadzinho.enable_sel()
  end

  local tipo = nil
  if #sel > 0 then tipo = pega_comp_tipo(sel[1]) end
  
  if #sel > 0 and  num_pt == 1 then
    if tipo then
      num_pt = 2
      if tipo == 'ENGATE' then
        g_engate.value = pega_engate(sel[1])
      else
        g_comp_id.value = pega_comp_id(sel[1])
      end
      local terminais = info_terminais(sel[1])
      g_terminais = {}
      for i, term in pairs(terminais) do
        g_terminais[i] = {value = term}
      end
    else
      cadzinho.clear_sel()
    end
  end
  
  cadzinho.nk_layout(20, 1)
  if num_pt == 1 then
    cadzinho.nk_label('Selecione um componente')
    
    if event.type == 'cancel' then
      cadzinho.stop_dynamic()
    end
  else
    cadzinho.nk_label('Confirme')
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    if tipo == 'ENGATE' then
      cadzinho.nk_edit(g_engate)
    else
      cadzinho.nk_edit(g_comp_id)
    end

    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Terminais:")
    cadzinho.nk_layout(20, 2)
    for i, term in pairs(g_terminais) do
      cadzinho.nk_label(tostring(i)..':')
      cadzinho.nk_edit(term)
    end
    
    if event.type == 'enter' then
      if tipo == 'ENGATE' then
        muda_engate (sel[1], g_engate.value)
      else
        muda_comp_id (sel[1], g_comp_id.value)
      end
      local terminais = {}
      for i, term in pairs(g_terminais) do
        terminais[i] = term.value
      end
      muda_terminais(sel[1], terminais)
      
      sel[1]:write()
      
      cadzinho.clear_sel()
      --cadzinho.stop_dynamic()
      num_pt = 1
    elseif event.type == 'cancel' then
      num_pt = 1
      cadzinho.clear_sel()
    end
  end
  
end

function component_dyn(event)
  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label("Place component")
  cadzinho.nk_layout(20, 2)
  cadzinho.nk_label("Component:")
  cadzinho.nk_edit(component)
  
  
  cadzinho.nk_layout(100, 1)
  if cadzinho.nk_group_begin("Grupo", true, true, true) then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_slide_i(inteiro2, 0, 10)
    cadzinho.nk_slide_f(numero2, -1.5, 20.8)
    
    cadzinho.nk_combo(combo, 100, 50)
    cadzinho.nk_option(option)
    cadzinho.nk_check("check", check)
    cadzinho.nk_group_end()
  end
  
  cadzinho.nk_layout(100, 1)
  if cadzinho.nk_tab_begin("tabdin", tab) then
    if tab.value == 1 then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_slide_i(inteiro2, 0, 10)
      cadzinho.nk_slide_f(numero2, -1.5, 20.8)
    elseif tab.value == 2 then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_combo(combo, 100, 50)
      cadzinho.nk_option(option)
      cadzinho.nk_check("check", check)
      
      
    else
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_propertyi("inteiro", inteiro, 0, 10, 2)
      cadzinho.nk_propertyd("numero", numero, -1, 40, 3)
    end
    cadzinho.nk_tab_end()
  end
  
  if event.type == 'cancel' then
    --cadzinho.stop_dynamic()
  end
end

--============== Janela Principal =======================
function pelicanu_win()
  cadzinho.nk_layout(20, 1)
  if cadzinho.nk_button("Projeto:") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('projeto_dyn')
  end
  cadzinho.nk_label(projeto.titulo)
  
  
  cadzinho.nk_layout(400, 1)
  if cadzinho.nk_tab_begin("modo_ed", g_editor_abas) then
    if g_editor_abas.value == 2 then -- biblioteca
      cadzinho.nk_layout(20, 1)
    
    
      if cadzinho.nk_button("terminal") then
        num_pt = 1
        local sel = cadzinho.get_sel()
        if (#sel > 0) then
          cadzinho.start_dynamic("terminal_dyn")
        end
      end
      
      if cadzinho.nk_button("engate") then
        num_pt = 1
        local sel = cadzinho.get_sel()
        if (#sel > 0) then
          cadzinho.start_dynamic("engate_dyn")
        end
      end
      
      
      if cadzinho.nk_button("test") then
        novo_projeto('C:\\util\\pelicanu\\teste', {})
      end
      
    else -- padrao - esquematico
      cadzinho.nk_layout(20, 1)
      if cadzinho.nk_button(" Ligacao") then
        num_pt = 1
        cadzinho.start_dynamic("ligacao_dyn")
      end
      if cadzinho.nk_button(" Componente") then
        num_pt = 1
        obtem_tipo_comp()
        cadzinho.start_dynamic("componente_dyn")
      end
      if cadzinho.nk_button("麗 Caixa") then
        num_pt = 1
        cadzinho.start_dynamic("caixa_dyn")
      end
      if cadzinho.nk_button(" Edita") then
        num_pt = 1
        cadzinho.start_dynamic("edita_dyn")
      end
      if cadzinho.nk_button(" Formato") then
        num_pt = 1
        obtem_lista_formato()
        cadzinho.start_dynamic("formato_dyn")
      end

      if cadzinho.nk_button(" Banco de dados") then
        teste()
      end
      if cadzinho.nk_button(" Grava pl") then
        grava_pl_comp()
      end
      if cadzinho.nk_button(" Le pl") then
        le_pl_comp()
      end
      if cadzinho.nk_button(" Engates") then
        atualiza_engates()
      end
    end
    cadzinho.nk_tab_end()
  end
end

-- inicia a janela quando o script eh executado
local config, err = carrega_config(diretorio(fs.script_path()) .. 'config.lua')
if config then
  biblioteca = config.biblioteca
end
local ini, err = carrega_config(diretorio(fs.script_path()) .. 'ini.lua')
if ini then
  abre_projeto(ini.projeto)
end
cadzinho.win_show("pelicanu_win", "Pelicanu", 220,120,200,450)
