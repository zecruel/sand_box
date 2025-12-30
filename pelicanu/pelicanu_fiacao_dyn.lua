--coding: utf-8
-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'


-- ================= Edição de Biblioteca de componentes ===============
function novo_terminal_fiacao (x, y, term)
  term = term or "1"
  
  local nome_bloco = 'plcn_term_fia'
    
  local ref_blc = cadzinho.new_insert(nome_bloco, x, y)
  
  
  if ref_blc == nil then
  
    
    local param = {color = "by block", lw = "by block",
      ltype = "byblock", style = "ISO"}
    
    local elems = {}
    elems[#elems + 1] = cadzinho.new_line(-1, 0, 0, 1, 0, 0, param)
    
    local txt = cadzinho.new_text(-2, 0,
      '#TERMINAL$1', 2.5, "right", "middle", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(2, 0,
      '#LIGACAO$??.?/??.??.?', 2.5, "left", "middle", param)
    elems[#elems + 1] = txt
    
    txt = cadzinho.new_text(0, 0, 
      '#*TIPO$TERMINAL_FIA', 0.5, "left", "bottom", param)
    elems[#elems + 1] = txt
    
    param.color = "by layer"
    param.lw = 0
    param.ltype = "Continuous"
  
  
    if cadzinho.new_block(elems, nome_bloco,
      "Terminal fiacao PELICAnU ",
      true, '#', '*', '$', '?', 0, 0, 0) then

      ref_blc = cadzinho.new_insert(nome_bloco, x, y, 1, 1, 0, param)
    end
  end
  
  if ref_blc then
    muda_atrib (ref_blc, {TERMINAL = term})
    cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
    cadzinho.add_ext(ref_blc, "PELICANU", 
      {cadzinho.unique_id(), "FIACAO", "TERMINAL"})
    return ref_blc
  end
  
  return nil
end

function novo_borne_fiacao (x, y, term)
  term = term or "1"
  
  local nome_bloco = 'plcn_borne_fia'
    
  local ref_blc = cadzinho.new_insert(nome_bloco, x, y)
  
  if ref_blc == nil then
    
    local param = {color = "by block", lw = "by block",
      ltype = "byblock", style = "ISO"}
    
    local elems = {}
    elems[#elems + 1] = cadzinho.new_line(-6, 3, 0, 6, 3, 0, param)
    elems[#elems + 1] = cadzinho.new_line(-9, 0, 0, -6, 0, 0, param)
    elems[#elems + 1] = cadzinho.new_line(6, 0, 0, 9, 0, 0, param)
    
    local txt = cadzinho.new_text(0, 0,
      '#TERMINAL$1', 2.5, "center", "middle", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(-10, 0,
      '#LIGACAO$??.?/??.??.?', 2.5, "right", "middle", param)
    elems[#elems + 1] = txt
    
    param.color = "by layer"
    param.lw = 0
    param.ltype = "Continuous"
  
    if cadzinho.new_block(elems, nome_bloco,
      "Terminal fiacao PELICAnU ",
      true, '#', '*', '$', '?', 0, 0, 0) then

      ref_blc = cadzinho.new_insert(nome_bloco, x, y, 1, 1, 0, param)
    end
  end
  
  if ref_blc then
    muda_atrib (ref_blc, {TERMINAL = term})
    cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
    cadzinho.add_ext(ref_blc, "PELICANU", 
      {cadzinho.unique_id(), "FIACAO", "TERMINAL"})
    return ref_blc
  end
  
  return nil
end

function novo_sec_e_fiacao (x, y)
  
  local nome_bloco = 'plcn_sec_e_fia'
    
  local ref_blc = cadzinho.new_insert(nome_bloco, x, y)
  
  if ref_blc == nil then
    
    local param = {color = "by block", lw = "by block",
      ltype = "byblock", style = "ISO"}
    
    local elems = {}
    elems[#elems + 1] = cadzinho.new_line(0, -3, 0, 0, 3, 0, param)
    elems[#elems + 1] = cadzinho.new_line(-9, 0, 0, -6, 0, 0, param)
    
    local txt = cadzinho.new_text(-3, 0,
      '#TERMINAL$E', 2.5, "center", "middle", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(-10, 0,
      '#LIGACAO$??.?/??.??.?', 2.5, "right", "middle", param)
    elems[#elems + 1] = txt
    
    param.color = "by layer"
    param.lw = 0
    param.ltype = "Continuous"
  
  
    if cadzinho.new_block(elems, nome_bloco,
      "Terminal fiacao PELICAnU ",
      true, '#', '*', '$', '?', 0, 0, 0) then

      ref_blc = cadzinho.new_insert(nome_bloco, x, y, 1, 1, 0, param)
    end
  end
  
  if ref_blc then
    cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
    cadzinho.add_ext(ref_blc, "PELICANU", 
      {cadzinho.unique_id(), "FIACAO", "TERMINAL"})
    return ref_blc
  end
  
  return nil
end

function novo_sec_s_fiacao (x, y)
  
  local nome_bloco = 'plcn_sec_s_fia'
    
  local ref_blc = cadzinho.new_insert(nome_bloco, x, y)
  
  if ref_blc == nil then
    
    local param = {color = "by block", lw = "by block",
      ltype = "byblock", style = "ISO"}
    
    local elems = {}
    elems[#elems + 1] = cadzinho.new_line(0, -3, 0, 0, 3, 0, param)
    elems[#elems + 1] = cadzinho.new_line(6, 0, 0, 9, 0, 0, param)
    
    local txt = cadzinho.new_text(3, 0,
      '#TERMINAL$S', 2.5, "center", "middle", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(10, 0,
      '#LIGACAO$??.?/??.??.?', 2.5, "left", "middle", param)
    elems[#elems + 1] = txt
    
    param.color = "by layer"
    param.lw = 0
    param.ltype = "Continuous"
  
  
    if cadzinho.new_block(elems, nome_bloco,
      "Terminal fiacao PELICAnU ",
      true, '#', '*', '$', '?', 0, 0, 0) then

      ref_blc = cadzinho.new_insert(nome_bloco, x, y, 1, 1, 0, param)
    end
  end
  
  if ref_blc then
    cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
    cadzinho.add_ext(ref_blc, "PELICANU", 
      {cadzinho.unique_id(), "FIACAO", "TERMINAL"})
    return ref_blc
  end
  
  return nil
end


function exec_fiacao_bd(painel)
  local barras = {}
  local fiacao = {}
  local b_terra = {}
  local componentes = {}
  local log = ""
  local bd = sqlite.open(projeto.bd)
  if bd then
    local ant = ""
    local ant_t = false
    local term_ant = nil
    local barra_ant = ""


    local terra = "select painel, componente, modulo, terminal, " ..
              "ROW_NUMBER() OVER (partition by painel " ..
              "order by painel, componente, modulo, terminal) as bt from ( " ..
              "select bbb.barra, d.painel, d.componente, d.tipo, " ..
              "d.modulo, t.terminal, ROW_NUMBER() OVER ( " ..
              "PARTITION BY bbb.barra ORDER BY bbb.barra, " ..
              "case when d.tipo = 'BORNE' then 1 " ..
              "when d.tipo = 'CT_CORR' then 2 " ..
              "when d.tipo = 'CT_TENSAO' then 2 " ..
              "when d.tipo = 'BORNE_SEC' then 7 " ..
              "else 6 end, d.painel,  " ..
              "d.componente, d.modulo, t.terminal) as rn from  " ..
              "(select distinct bb.id barra from engates_esq e " ..
              "inner join barras_esq bb on e.unico = bb.componente " ..
              "where engate = 'TERRA') bbb " ..
              "left join barras_esq b on b.id = bbb.barra " ..
              "inner join descr_comp d on d.unico = b.componente " ..
              "left join comp_term t on t.unico = b.componente " ..
              "and t.num = b.terminal) " ..
              "where rn = 1 and painel = '" .. painel .. "';"
    for linha in bd:cols(terra) do -- para cada linha do BD
      local val = ""
      if linha.modulo then
        val = linha.componente .. "." .. linha.modulo .. "." .. linha.terminal
      else
        val = linha.componente .. "." .. linha.terminal
      end
      b_terra[val] = linha.bt
    end

    local cmd = "select b.barra barra_id, d.componente, c.item, d.tipo, " ..
                "case when c.id_fiacao is NULL then d.componente else c.id_fiacao end fia, " ..
                "d.modulo, t.terminal from barra_consol b " ..
                "left join descr_comp d on d.unico = b.componente " ..
                "left join comp_term t on t.unico = b.componente and t.num = b.terminal " ..
                "left join componentes c on c.painel = b.nome_painel and c.id = d.componente " ..
                "where b.nome_painel = '" .. painel .. "' " ..
                "order by b.barra, case when instr(d.componente, '27') > 0 and d.tipo = 'BOBINA' then 1 " ..
                "when d.tipo = 'CT_TENSAO' then 1 " ..
                "when d.tipo = 'CT_CORR' then 1 " ..
                "when d.tipo = 'ALIM_CC' then 2 " ..
                "when d.tipo = 'BOBINA' then 3 " ..
                "when d.tipo = 'MINI_DISJ' then 6 " ..
                "else 5 end, fia desc, " .. 
                "case when cast(d.modulo as int) then cast(d.modulo as int) else d.modulo end, " ..
                "case when cast(t.terminal as int) then cast(t.terminal as int) else t.terminal end;"
    for linha in bd:cols(cmd) do -- para cada linha do BD
      if barras[linha.barra_id] == nil then
        barras[linha.barra_id] = {corr = false}
      end
      local barra = barras[linha.barra_id]
      barra[#barra+1] = {c = linha.componente, f = linha.fia, m = linha.modulo, t = linha.terminal}
      if linha.tipo == "CT_CORR" or linha.tipo == "TOMADA" then
        barra.corr = true
      end
      
      if linha.barra_id ~= barra_ant then
        ant = ""
        ant_t = false
        term_ant = nil
        
      end
      
      local comp_id = linha.fia
      --print (comp_id)
      if fiacao[comp_id] == nil then
        fiacao[comp_id] = {}
      end
      
      if componentes[comp_id] == nil then
        componentes[comp_id] = {esq = linha.componente, le = linha.item}
      elseif componentes[comp_id].esq ~= linha.componente then
        log = log .. "ERRO: Os componentes " .. componentes[comp_id].esq ..
          " e " .. linha.componente .. " estão com mesmo cod de fiação\n"
      elseif componentes[comp_id].le ~= linha.item then
        log = log .. "ERRO: O componente " .. componentes[comp_id].esq ..
          " está com conflito de item de lista de equipamento\n"
      end
      
      if linha.modulo and linha.modulo ~= "" then
        if fiacao[comp_id][linha.modulo] == nil then
          fiacao[comp_id][linha.modulo] = {tipo_ = 'modulo'}
        end
        
        fiacao[comp_id][linha.modulo][linha.terminal] = {l = ant, c = barra.corr, t = false}
        ant = comp_id .. '.' .. linha.modulo .. '.' .. linha.terminal
        
        if ant_t and term_ant and term_ant ~= "" then
          if ant_t[term_ant].l ~= "" then ant_t[term_ant].l = ant_t[term_ant].l .. '|' end
          ant_t[term_ant].l = ant_t[term_ant].l .. ant
        end
        
        ant_t = fiacao[comp_id][linha.modulo]
        term_ant = linha.terminal
        
      else
        fiacao[comp_id][linha.terminal] = {l = ant, c = barra.corr, t = false}
        ant = comp_id .. '.' .. linha.terminal
        
        if ant_t and term_ant and term_ant ~= "" then
          if ant_t[term_ant].l ~= "" then ant_t[term_ant].l = ant_t[term_ant].l .. '|' end
          ant_t[term_ant].l = ant_t[term_ant].l .. ant
        end
        
        ant_t = fiacao[comp_id]
        term_ant = linha.terminal
      end
       
      barra_ant = linha.barra_id
      
      if b_terra[ant] then
        ant_t[term_ant].t = true
      end
    end
    
    bd:close()
  end
  return componentes, fiacao, barras, b_terra, log
end

function grava_pl_fiacao ()
  
  local cor = 1
  
  -- verifica se há necessidade de criar a pasta de saída
  local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  if not exists (aux) then
    if not cria_pasta(aux) then
      return false -- erro com a pasta de saída
    end
  end
  
  -- cria o arquivo de planilha
  local cam_pl = aux .. "fiacao_".. g_fia_bd.painel ..".xlsx"
  if exists (cam_pl) then -- se há um arquivo antigo
    if not os.remove(cam_pl) then -- tenta deletar o arquivo existente
      return false -- erro ao apagar o arquivo antigo
    end
  end
  
  local planilha = excel:new(cam_pl)
  if not planilha then return false end -- erro na criacao da planilha
  
  local protegido = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    })
  local desprotegido = planilha:add_format({locked = false, border = 1})
  
  local m_p = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    valign = "vcenter",
    })
  local m_d = planilha:add_format({locked = false, valign = "vcenter", border = 1})
  
  -- primeira linha como titulo
  local tit_p = planilha:add_format({
    border = 6,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    bold = true
    })
  local tit_d = planilha:add_format({locked = false, bold = true, border = 6})
  
   -- cria a primeira aba
  local aba_be = planilha:add_worksheet("BarrasEsq")
  -- tamanho das colunas
  aba_be:set_column(0, 0, 20)
  aba_be:set_column(1, 1, 3)
  aba_be:set_column(2, 50, 15)
  aba_be:write(0, 0, 'Barra', tit_d)
  aba_be:write(0, 1, '(*)', tit_d)
  aba_be:write(0, 2, 'Ligações', tit_d)
  
  aba_be:set_tab_color('red')

  local lin = 1
  local col = 1
  for id, barra in pairs(g_fia_bd.barras) do
    -- grava na planilha cada celula separada, a principio
    aba_be:write(lin, 0, id)
    if barra.corr then aba_be:write(lin, 1, 'x') end

    for col = 1, #barra do
      local val = barra[col].c
      if barra[col].m then val = val .. "." .. barra[col].m end
      if barra[col].t then val = val .. "." .. barra[col].t end
      aba_be:write(lin, col + 1, val, desprotegido)
    
    end
    -- proxima linha
    lin = lin + 1
  end
  
   -- cria a segunda aba
  local aba_bf = planilha:add_worksheet("BarrasFia")
  -- tamanho das colunas
  aba_bf:set_column(0, 0, 20)
  aba_bf:set_column(1, 1, 3)
  aba_bf:set_column(2, 20, 15)
  aba_bf:write(0, 0, 'Barra', tit_d)
  aba_bf:write(0, 1, '(*)', tit_d)
  aba_bf:write(0, 2, 'Ligações', tit_d)
  
  aba_bf:set_tab_color('blue')
  
  lin = 1
  col = 1
  for id, barra in pairs(g_fia_bd.barras) do
    -- grava na planilha cada celula separada, a principio
    aba_bf:write(lin, 0, id)
    if barra.corr then aba_bf:write(lin, 1, 'x') end
    for col = 1, #barra do
      local val = barra[col].f
      if barra[col].m then val = val .. "." .. barra[col].m end
      if barra[col].t then val = val .. "." .. barra[col].t end
      aba_bf:write(lin, col + 1, val, desprotegido)
    
    end
    -- proxima linha
    lin = lin + 1
  end
  
  -- cria a terceira aba
  local aba_terra = planilha:add_worksheet("Terra")
  -- tamanho das colunas
  aba_terra:set_column(0, 0, 18)
  aba_terra:set_column(1, 1, 32)
  aba_terra:write(0, 0, 'Componente', tit_d)
  aba_terra:write(0, 1, 'Ligação', tit_d)
  
  aba_terra:set_tab_color('green')
  
  lin = 1

  for lig, bt in pairs(g_fia_bd.b_terra) do
    aba_terra:write(lin, 0, lig, desprotegido)
    aba_terra:write(lin, 1, bt, desprotegido)
    lin = lin + 1
  end

  -- cria a quarta aba
  local aba_fia = planilha:add_worksheet("Fiacao")
  -- tamanho das colunas
  aba_fia:set_column(0, 0, 18)
  aba_fia:set_column(1, 1, 32)
  aba_fia:write(0, 0, 'Componente', tit_d)
  aba_fia:write(0, 1, 'Ligação', tit_d)
  
  aba_fia:set_tab_color('gray')
  
  lin = 1
  col = 1
  
  for c_id, comp in pairs (g_fia_bd.fiacao) do
    for m_id, m in pairs(comp) do
      if m.tipo_ and m.tipo_ == 'modulo' then
        for t_id, t in pairs (m) do
          if t_id ~= 'tipo_' then
            local val = c_id .. '.' .. m_id .. '.' .. t_id
            aba_fia:write(lin, 0, val, desprotegido)
            val = t.l
            if t.t then val = val .. '|BT' end
            if t.c then val = val .. '(*)' end
            aba_fia:write(lin, 1, val, desprotegido)
            lin = lin + 1
          end
        end
      else
        local val = c_id .. '.' .. m_id
        aba_fia:write(lin, 0, val, desprotegido)
        val = m.l
        if m.t then val = val .. '|BT' end
        if m.c then val = val .. '(*)' end 
        aba_fia:write(lin, 1, val, desprotegido)
        lin = lin + 1
      end
    end
    lin = lin + 1
  end
  
  planilha:close()
  return true
end


function obtem_fia_comp()
  local caixas = {}
  local SetLib = require("Set") -- biblioteca para matematica de conjuntos
  local caixa
  local conteudo
  local componentes = {}
  
  -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
  --atualiza_unicos()
  -- atualiza a lista principal com os elementos
  --atualiza_elems()
  
  -- varre os elementos, cadastrando as caixas existentes no desenho
  for el_id, el in pairs(elems_pelicanu) do
    if el.tipo == "FIACAO" and (el.esp == "COMPONENTE" or el.esp == "MODULO") then
      caixa = {}
      conteudo = pega_conteudo(el_id, true)
      caixa['conteudo'] = SetLib.new(conteudo)
      caixa['tipo'] = el.esp
      caixas[el_id] = caixa
    end
  end
  
  -- repassa as caixas, cadastrando as caixas aninhadas
  for c_id, caixa in pairs(caixas) do
    if caixa['tipo'] == "COMPONENTE" then
      local comp = {}
      comp.mod = {}
      comp.terms = {}
      for el_id in pairs(caixa.conteudo) do
        
        local el = elems_pelicanu[el_id]
        if el.tipo == "FIACAO" and el.esp == "MODULO" then
          local mod = {}
          mod.terms = {}
          caixa.conteudo = caixa.conteudo - caixas[el_id].conteudo
          for sub_id in pairs(caixas[el_id].conteudo) do
            local sub = elems_pelicanu[sub_id]
            if sub.tipo == "FIACAO" and sub.esp == "ID_MOD" then
              mod.id = sub_id
            elseif sub.tipo == "FIACAO" and sub.esp == "TERMINAL" then
              mod.terms[#mod.terms+1] = sub_id
            end
          end
          --mod.cont = caixas[el_id].conteudo
          comp.mod[el_id] = mod
        elseif el.tipo == "FIACAO" and el.esp == "ID_FIA" then
          comp.fia = el_id
          local rotulo = cadzinho.get_text_data(el.ent)
          if rotulo then caixa.nome = rotulo.text end
        elseif el.tipo == "FIACAO" and el.esp == "ID_ESQ" then
          comp.esq = el_id
        elseif el.tipo == "FIACAO" and el.esp == "ID_LE" then
          comp.le = el_id
          
        end
      end
      --comp.cont = caixa.conteudo
      for el_id in pairs(caixa.conteudo) do
        local el = elems_pelicanu[el_id]
        if el.tipo == "FIACAO" and el.esp == "TERMINAL" then
          comp.terms[#comp.terms+1] = el_id
        end
      end
      componentes[c_id] = comp
    end
  end
  
  return componentes
end

function texto_el(id)
  if not id then return nil end
  local el = elems_pelicanu[id]
  if not el then return nil end
  local t = cadzinho.get_text_data(el.ent)
  if not t then return nil end
  return t.text
end

function pega_term(id)
  if not id then return nil end
  local el = elems_pelicanu[id]
  if not el then return nil end
  
  local term, lig
  
  local attrs = cadzinho.get_attribs(el.ent)
  for i, attr in ipairs(attrs) do
    -- terminal
    if string.find(attr['tag']:upper(), "^TERMINAL") then
      term = attr['value']
    -- ligação
    elseif string.find(attr['tag']:upper(), "^LIGACAO") then
      lig = attr['value']
    end
  end
  return term, lig
end

function muda_lig (id, lig)
  if not id then return nil end
  local el = elems_pelicanu[id]
  if not el then return nil end
  
  local idx1 = 0
  local ocul1 = false
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas
  local attrs = cadzinho.get_attribs(el.ent)
  for i, attr in ipairs(attrs) do
    if string.find(attr['tag']:upper(), "^LIGACAO") then
      idx1 = i
      ocul1 = attr['hidden']
    end
  end
  
  if idx1 > 0 then
    cadzinho.edit_attr(el.ent, idx1, 'LIGACAO', lig, ocul1)
  end
  
  el.ent:write()
end

function fiacao_dyn (event)
  if modal == 'executa' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Carrega fiação")
    
    if num_pt == 1 then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label('Escolha o painel:')
      cadzinho.nk_layout(100, 1)
      if cadzinho.nk_group_begin("Paineis", false, true, true) then
        cadzinho.nk_layout(15, 2)
        
        for i = 1, #g_fia_paineis do      
          if cadzinho.nk_button(g_fia_paineis[i]) then
            g_fia_painel.value = g_fia_paineis[i]
          end
        end
        cadzinho.nk_group_end()
      end
      cadzinho.nk_layout(20, 2)
      cadzinho.nk_label("Painel:")
      cadzinho.nk_edit(g_fia_painel)
      
      
      cadzinho.nk_layout(15, 1)
      if g_fia_painel.value and g_fia_painel.value ~= "" then
        if cadzinho.nk_button("Executa") then
          if sub_modal == 'planilha' then
            --[[   -- le o arquivo excel
      local leitor = require 'xlsx_lua'
  local workbook = leitor.open(arq)
  -- procura pelas abas 'Equipamentos' e 'Terminais'
  if type(workbook) == 'table' then 
    pl_equip = workbook.sheets['Equipamentos']
    pl_term = workbook.sheets['Terminais']
    pl_regras = workbook.sheets['Regras']
  else
    return nil -- erro na abertura do arquivo excel
  end
  
  -- verifica a existência das abas
  if type(pl_equip) ~= 'table' or type(pl_term) ~= 'table' then return nil end
  
    -- insere os dados da aba 'Equipamentos' no banco de dados
  for lin = 2, #pl_equip.dim.rows do
    if pl_equip.data[lin]['A'] then
    end]]--
  
          else
            local componentes, fiacao, barras, b_terra, log = exec_fiacao_bd(g_fia_painel.value)
          --end
          g_fia_bd = {}
          g_fia_bd.painel = g_fia_painel.value
          g_fia_bd.componentes = componentes
          g_fia_bd.fiacao = fiacao
          g_fia_bd.barras = barras
          g_fia_bd.b_terra = b_terra
          g_fia_bd.log = log
          
          print('===============teste=======================')
          --[[for c_id, comp in pairs (fiacao) do
            for m_id, m in pairs(comp) do
              if type(m) == 'table' then
                for t_id, t in pairs (m) do
                  print(c_id .. '.' .. m_id .. '.' .. t_id .. '-' .. t)
                end
              else
                print(c_id .. '.' .. m_id .. '-' .. m)
              end
            end
          end
          
          print('***************** ESQUEMATICO ******************')
          for c_id, comp in pairs (componentes) do
            print(c_id .. ' - ' .. comp.esq .. ' - ' .. tostring(comp.le))
          end]]--
          print('=============== LOG =======================')
          print(log)
          end --
          
          modal = ''
        end
      end
    end
    
  elseif modal == 'componente' then
    -- funcao interativa para criacao de uma caixa
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Novo componente")
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_fia_id_fia)
    cadzinho.nk_label("Esq:")
    cadzinho.nk_edit(g_fia_id_esq)
    cadzinho.nk_label("LE:")
    cadzinho.nk_edit(g_fia_id_le)
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_propertyi("Ini", g_fia_ini, 0)
    cadzinho.nk_propertyi("Lin", g_fia_lin, 0)
    cadzinho.nk_propertyi("Col", g_fia_col, 0)
    
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
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      --cadzinho.set_color(3) -- cor verde
      local cor = 4
      local caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0,
        pts[2].x, pts[1].y, 0,
        {color = cor, ltype = "Continuous"})
      cadzinho.pline_append(caixa, pts[2].x, pts[2].y, 0)
      cadzinho.pline_append(caixa, pts[1].x, pts[2].y, 0)
      cadzinho.pline_close(caixa, true)
      if (caixa) then cadzinho.ent_draw(caixa) end
      
      -- identificador da caixa - eh uma entidade TEXT e um elemento tipo "rotulo"
      -- posicao do texto - canto superior direito
      local tx = pts[2].x
      if tx < pts[1].x then tx = pts[1].x end
      local ty = pts[2].y
      if ty < pts[1].y then ty = pts[1].y end
      
      local w = pts[2].x - pts[1].x
      local h = pts[2].y - pts[1].y
      
      local cx = w/2.0 + pts[1].x
      local cy = h/2.0 + pts[1].y
      
      local id_fia = cadzinho.new_text(cx, ty-2, g_fia_id_fia.value,
        3.5, "center", "top", {color = cor, style = "ISO"})
      if id_fia then cadzinho.ent_draw(id_fia) end
      
      local id_esq = cadzinho.new_text(cx, ty-math.abs(h)+2, g_fia_id_esq.value,
        2, "center", "bottom", {color = cor, style = "ISO"})
      if id_esq then cadzinho.ent_draw(id_esq) end
      
      local id_le = cadzinho.new_text(tx-2, ty-2, "(" .. g_fia_id_le.value .. ")",
        2, "right", "top", {color = cor, style = "ISO"})
      if id_le then cadzinho.ent_draw(id_le) end
      
      local terms = {}
      for j = 1, g_fia_col.value do
        for i = 1, g_fia_lin.value do
          terms[#terms+1] = novo_terminal_fiacao ((tx-math.abs(w)+8)+(j-1)*36, (ty-12)-(i-1)*4, tostring(#terms+g_fia_ini.value))
        end
      end
      for i = 1, #terms do
        if terms[i] then cadzinho.ent_draw(terms[i]) end
      end
      
      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        if caixa then
          cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "FIACAO", 'COMPONENTE'})
          caixa:write()
        end
        if id_fia then
          cadzinho.add_ext(id_fia, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_FIA"})
          id_fia:write()
        end
        if id_esq then
          cadzinho.add_ext(id_esq, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_ESQ"})
          id_esq:write()
        end
        if id_le then
          cadzinho.add_ext(id_le, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_LE"})
          id_le:write()
        end
        
        for i = 1, #terms do
          if terms[i] then terms[i]:write() end
        end
        
        
        num_pt = 1
        
      elseif event.type == 'cancel' then
        num_pt = 1
      end
    
    end 
  elseif modal == 'modulo' then
    -- funcao interativa para criacao de uma caixa
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Novo modulo")
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_fia_id_mod)
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_propertyi("Ini", g_fia_ini, 0)
    cadzinho.nk_propertyi("Lin", g_fia_lin, 0)
    cadzinho.nk_propertyi("Col", g_fia_col, 0)
    
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
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      --cadzinho.set_color(3) -- cor verde
      local cor = 3
      local caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0,
        pts[2].x, pts[1].y, 0,
        {color = cor, ltype = "Dashdot"})
      cadzinho.pline_append(caixa, pts[2].x, pts[2].y, 0)
      cadzinho.pline_append(caixa, pts[1].x, pts[2].y, 0)
      cadzinho.pline_close(caixa, true)
      if (caixa) then cadzinho.ent_draw(caixa) end
      
      -- identificador da caixa - eh uma entidade TEXT e um elemento tipo "rotulo"
      -- posicao do texto - canto superior direito
      local tx = pts[2].x
      if tx < pts[1].x then tx = pts[1].x end
      local ty = pts[2].y
      if ty < pts[1].y then ty = pts[1].y end
      
      local w = pts[2].x - pts[1].x
      local h = pts[2].y - pts[1].y
      
      local cx = w/2.0 + pts[1].x
      local cy = h/2.0 + pts[1].y
      
      local id_fia = cadzinho.new_text(cx, ty-2, g_fia_id_mod.value,
        3, "center", "top", {color = cor, style = "ISO"})
      if id_fia then cadzinho.ent_draw(id_fia) end
      
      local terms = {}
      for j = 1, g_fia_col.value do
        for i = 1, g_fia_lin.value do
          terms[#terms+1] = novo_terminal_fiacao ((tx-math.abs(w)+8)+(j-1)*36, (ty-12)-(i-1)*4, tostring(#terms+g_fia_ini.value))
        end
      end
      for i = 1, #terms do
        if terms[i] then cadzinho.ent_draw(terms[i]) end
      end
      
      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        if caixa then
          cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "FIACAO", 'MODULO'})
          caixa:write()
        end
        if id_fia then
          cadzinho.add_ext(id_fia, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_MOD"})
          id_fia:write()
        end
        
        for i = 1, #terms do
          if terms[i] then terms[i]:write() end
        end
        
        
        num_pt = 1
        
      elseif event.type == 'cancel' then
        num_pt = 1
      end
    
    end
    
  
  elseif modal == 'regua' then
    -- funcao interativa para criacao de uma caixa
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Nova régua")
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_fia_id_fia)
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_propertyi("Ini", g_fia_ini, 0)
    cadzinho.nk_propertyi("Lin", g_fia_lin, 1)
    
    -- armazena o ponto atual na lista
    pts[num_pt] = {}
    pts[num_pt].x = event.x - 1
    pts[num_pt].y = event.y - 1
    
    cadzinho.nk_layout(20, 1)
  
    cadzinho.nk_label('Ponto de origem')
    
    local w = 12
    local h = 6 * g_fia_lin.value + 6
    
    --cadzinho.set_color(3) -- cor verde
    local cor = 4
    local caixa = cadzinho.new_pline(pts[1].x-6, pts[1].y, 0,
      pts[1].x+6, pts[1].y, 0,
      {color = cor, ltype = "Continuous"})
    cadzinho.pline_append(caixa, pts[1].x+6, pts[1].y-h, 0)
    cadzinho.pline_append(caixa, pts[1].x-6, pts[1].y-h, 0)
    cadzinho.pline_close(caixa, true)
    if (caixa) then cadzinho.ent_draw(caixa) end
    
    
    
    local id_fia = cadzinho.new_text(pts[1].x, pts[1].y-3, g_fia_id_fia.value,
      3.5, "center", "middle", {color = cor, style = "ISO"})
    if id_fia then cadzinho.ent_draw(id_fia) end
    
    local terms = {}
    for i = 1, g_fia_lin.value do
      terms[#terms+1] = novo_borne_fiacao (pts[1].x, (pts[1].y-9)-(i-1)*6, tostring(#terms+g_fia_ini.value))
    end
    for i = 1, #terms do
      if terms[i] then cadzinho.ent_draw(terms[i]) end
    end
    
    if event.type == 'enter' then
      cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
      
      if caixa then
        cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "FIACAO", 'COMPONENTE'})
        caixa:write()
      end
      if id_fia then
        cadzinho.add_ext(id_fia, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_FIA"})
        id_fia:write()
      end
      
      for i = 1, #terms do
        if terms[i] then terms[i]:write() end
      end
      
      
      num_pt = 1
      
    elseif event.type == 'cancel' then
      modal = ''
    end
    
  elseif modal == 'secion' then
    -- funcao interativa para criacao de uma caixa
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Nova régua Sec")
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_fia_id_fia)
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_propertyi("Ini", g_fia_ini, 0)
    cadzinho.nk_propertyi("Lin", g_fia_lin, 1)
    
    -- armazena o ponto atual na lista
    pts[num_pt] = {}
    pts[num_pt].x = event.x - 1
    pts[num_pt].y = event.y - 1
    
    cadzinho.nk_layout(20, 1)
  
    cadzinho.nk_label('Ponto de origem')
    
    local w = 24
    local h = 6 * g_fia_lin.value + 6
    
    --cadzinho.set_color(3) -- cor verde
    local cor = 4
    local caixa = cadzinho.new_pline(pts[1].x-12, pts[1].y, 0,
      pts[1].x+12, pts[1].y, 0,
      {color = cor, ltype = "Continuous"})
    cadzinho.pline_append(caixa, pts[1].x+12, pts[1].y-h, 0)
    cadzinho.pline_append(caixa, pts[1].x-12, pts[1].y-h, 0)
    cadzinho.pline_close(caixa, true)
    if (caixa) then cadzinho.ent_draw(caixa) end
    
    
    
    local id_fia = cadzinho.new_text(pts[1].x, pts[1].y-3, g_fia_id_fia.value,
      3.5, "center", "middle", {color = cor, style = "ISO"})
    if id_fia then cadzinho.ent_draw(id_fia) end
    
    local terms = {}
    for i = 1, g_fia_lin.value do
      cor = 3
      terms[#terms+1] = novo_sec_e_fiacao (pts[1].x-6, (pts[1].y-9)-(i-1)*6)
      terms[#terms+1] = novo_sec_s_fiacao (pts[1].x+6, (pts[1].y-9)-(i-1)*6)
      local id_mod = cadzinho.new_text(pts[1].x, (pts[1].y-9)-(i-1)*6,
        tostring(i+g_fia_ini.value-1),
        2.5, "center", "middle", {color = cor, style = "ISO"})
      local mod = cadzinho.new_pline(pts[1].x-12, (pts[1].y-6)-(i-1)*6, 0,
        pts[1].x+12, (pts[1].y-6)-(i-1)*6, 0,
        {color = cor, ltype = "Continuous"})
      cadzinho.pline_append(mod, pts[1].x+12, (pts[1].y-12)-(i-1)*6, 0)
      cadzinho.pline_append(mod, pts[1].x-12, (pts[1].y-12)-(i-1)*6, 0)
      cadzinho.pline_close(mod, true)
      if mod then
        cadzinho.add_ext(mod, "PELICANU", 
          {cadzinho.unique_id(), "FIACAO", 'MODULO'})
        terms[#terms+1] = mod
      end
      if id_mod then
        cadzinho.add_ext(id_mod, "PELICANU",
          {cadzinho.unique_id(), "FIACAO", "ID_MOD"})
        terms[#terms+1] = id_mod
      end
    end
    for i = 1, #terms do
      if terms[i] then cadzinho.ent_draw(terms[i]) end
    end
    
    if event.type == 'enter' then
      cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
      
      if caixa then
        cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "FIACAO", 'COMPONENTE'})
        caixa:write()
      end
      if id_fia then
        cadzinho.add_ext(id_fia, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_FIA"})
        id_fia:write()
      end
      
      for i = 1, #terms do
        if terms[i] then terms[i]:write() end
      end
      
      
      num_pt = 1
      
    elseif event.type == 'cancel' then
      modal = ''
    end
    
    
    
    
    --cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  
  --[[
  elseif modal == 'chicote' then
    -- funcao interativa para criacao dos cabos
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Chicote tipo:")
    
    cadzinho.nk_option(g_fia_tipo_chic)
    
    if g_fia_tipo_chic.value ~= g_fia_tipo_chic_ant.value then
      num_pt = 1
    end
    
    if g_fia_tipo_chic.value == 2 then
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label("Comprimento:")
      cadzinho.nk_propertyd("mm", g_fia_compr_chic)
    end
    
    -- armazena o ponto atual na lista
    pts[num_pt] = {}
    pts[num_pt].x = event.x
    pts[num_pt].y = event.y
    
    cadzinho.nk_layout(5, 1)
    cadzinho.nk_layout(20, 1)
    if num_pt == 1 then
      cadzinho.nk_label('Primeiro ponto')
      if event.type == 'enter' then
        num_pt = num_pt + 1
      elseif event.type == 'cancel' then
        -- sai da funcao
        --cadzinho.set_color("by layer")
        --cadzinho.set_lw("by layer")
        --cadzinho.set_ltype("bylayer")
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      -- o elemento "caixa" principal -  eh uma polyline no formato retangulo
      --cadzinho.set_ltype("Hidden") -- linha tracejada
      --cadzinho.set_color(4) -- ciano
      local prop = {color = 4, ltype = "Continuous", lw = 21}
      local chic = false
      if g_fia_tipo_chic.value == 2 then
      
        prop.color = 146
        
        chic = cadzinho.new_pline(pts[1].x, pts[1].y, 0.2,
          pts[2].x, pts[2].y, 0.2, prop)
        for i = 3, num_pt do
          cadzinho.pline_append(chic, pts[i].x, pts[i].y, 0.2)
        end
      else
        chic = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0, prop)
        
      end
      if chic then cadzinho.ent_draw(chic) end

      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        
        
        if g_fia_tipo_chic.value == 2 then
          num_pt = num_pt + 1
        else
          if chic then
            cadzinho.add_ext(chic, "PELICANU", {cadzinho.unique_id(), "FIACAO", "CANALETA"})
            chic:write()
          end
          pts[1].x = event.x
          pts[1].y = event.y
        end
      
        
      elseif event.type == 'cancel' then
        if g_fia_tipo_chic.value == 2 and num_pt > 2 then
          prop.color = 146
        
          chic = cadzinho.new_pline(pts[1].x, pts[1].y, 0.2,
            pts[2].x, pts[2].y, 0.2, prop)
          for i = 3, num_pt - 1 do
            cadzinho.pline_append(chic, pts[i].x, pts[i].y, 0.2)
          end
          if chic then
            cadzinho.add_ext(chic, "PELICANU", {cadzinho.unique_id(), "FIACAO", "CHICOTE", g_fia_compr_chic.value})
            chic:write()
          end
        end
        num_pt = 1
      end
    
    end
    g_fia_tipo_chic_ant.value = g_fia_tipo_chic.value
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  elseif modal == 'componente' then
    -- funcao interativa para criacao de uma caixa
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Novo componente")
    cadzinho.nk_layout(20, 2)
    cadzinho.nk_label("ID:")
    cadzinho.nk_edit(g_fia_id_fia)
    cadzinho.nk_label("Esq:")
    cadzinho.nk_edit(g_fia_id_esq)
    
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
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      --cadzinho.set_color(3) -- cor verde
      local cor = 3
      local caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0,
        pts[2].x, pts[1].y, 0,
        {color = cor, ltype = "Dashdot"})
      cadzinho.pline_append(caixa, pts[2].x, pts[2].y, 0)
      cadzinho.pline_append(caixa, pts[1].x, pts[2].y, 0)
      cadzinho.pline_close(caixa, true)
      if (caixa) then cadzinho.ent_draw(caixa) end
      
      -- identificador da caixa - eh uma entidade TEXT e um elemento tipo "rotulo"
      -- posicao do texto - canto superior direito
      local tx = pts[2].x
      if tx < pts[1].x then tx = pts[1].x end
      local ty = pts[2].y
      if ty < pts[1].y then ty = pts[1].y end
      
      local w = pts[2].x - pts[1].x
      local h = pts[2].y - pts[1].y
      
      local cx = w/2.0 + pts[1].x
      local cy = h/2.0 + pts[1].y
      
      local id_fia = cadzinho.new_text(cx, ty-1, g_fia_id_fia.value,
        8, "center", "top", {color = cor, style = "ISO"})
      if id_fia then cadzinho.ent_draw(id_fia) end
      
      local id_esq = cadzinho.new_text(cx, ty-10, g_fia_id_esq.value,
        4, "center", "top", {color = cor, style = "ISO"})
      if id_esq then cadzinho.ent_draw(id_esq) end
      
      local terms = novo_terms_fiacao (cx, cy, '*')
      if terms then cadzinho.ent_draw(terms) end
      
      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        if caixa then
          cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "FIACAO", 'COMPONENTE'})
          caixa:write()
        end
        if id_fia then
          cadzinho.add_ext(id_fia, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_FIA"})
          id_fia:write()
        end
        if id_esq then
          cadzinho.add_ext(id_esq, "PELICANU", {cadzinho.unique_id(), "FIACAO", "ID_ESQ"})
          id_esq:write()
        end
        
        if terms then terms:write() end
        
        
        num_pt = 1
        
      elseif event.type == 'cancel' then
        num_pt = 1
      end
    
    end
    --cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  elseif modal == 'ligacao' then
    -- funcao interativa para criacao dos cabos
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Ligação")
    
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
        --cadzinho.set_color("by layer")
        --cadzinho.set_lw("by layer")
        --cadzinho.set_ltype("bylayer")
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      -- o elemento "caixa" principal -  eh uma polyline no formato retangulo
      --cadzinho.set_ltype("Hidden") -- linha tracejada
      --cadzinho.set_color(4) -- ciano
      local prop = {color = 9, ltype = "Continuous", lw = 10}
      local chic = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0, prop)
      if chic then cadzinho.ent_draw(chic) end
      

      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        if chic then
          cadzinho.add_ext(chic, "PELICANU", {cadzinho.unique_id(), "FIACAO", "LIGACAO"})
          chic:write()
        end
        num_pt = 1
      
        
      elseif event.type == 'cancel' then

        num_pt = 1
      end
    
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  elseif modal == 'calcula' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label('Cálculo fiação')
      chicotes = {}
      lims = {}
      pts = {}
      for i, ent in ipairs(cadzinho.get_all()) do -- varre os objetos selecionados do desenho
        local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
        if #ext > 0 then
          -- elemento do PELICAnU
          --local elem = {}
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
          if tipo == 'FIACAO' then
            if especifico == 'CHICOTE' then
              chicotes[#chicotes +1] = cadzinho.get_points(ent)
            elseif especifico == 'CABO' then
              chicotes[#chicotes +1] = cadzinho.get_points(ent)
            end
          end
          --local unic = tonumber(unico,16)
          --elem.id = unic
          --elem.ent = ent
          --elem.tipo = tipo
          --elem.esp = especifico
        end
      end
    
    
    
  elseif modal == 'calcula_malha' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label('Cálculo tensões')
    
    -- ========================================
    
    if sub_modal == 'ini' then
    
      malha_cabos = {}
      lims = {}
      pts = {}
      for i, ent in ipairs(cadzinho.get_sel()) do -- varre os objetos selecionados do desenho
        local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
        if #ext > 0 then
          -- elemento do PELICAnU
          --local elem = {}
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
          if tipo == 'ATERRAMENTO' then
            if especifico == 'LIMITE' then
              lims[#lims +1] = cadzinho.get_points(ent)
            elseif especifico == 'CABO' then
              malha_cabos[#malha_cabos +1] = cadzinho.get_points(ent)
            end
          end
          --local unic = tonumber(unico,16)
          --elem.id = unic
          --elem.ent = ent
          --elem.tipo = tipo
          --elem.esp = especifico
        end
      end
      
      if #lims > 0 and #malha_cabos > 0 then
        
        for j = 1, #lims do
          for k = j, #lims do
            local pt = intersec(lims[j], lims[k])
            if pt then
              pts[#pts +1] = pt
              --cadzinho.db_print(pt.x, pt.y) 
              
            end
          end
        end
        
        
        table.sort(pts, compara_pts) -- organiza os pontos em ordem
        ]]--
    
    
    
    
    
    
    
    
    
    
    
    
  else
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label('Fiacao')
     
    if cadzinho.nk_button("⚡ Carrega do BD") then
      modal = 'executa'
      sub_modal = 'bd'
      msg = ''
      
      g_fia_paineis = {}
      local bd = sqlite.open(projeto.bd)
      if bd then
        
        local cmd = 'SELECT id FROM paineis ORDER BY id'
        for linha in bd:cols(cmd) do -- para cada linha do BD
          g_fia_paineis[#g_fia_paineis + 1] = linha.id
        end
        
        bd:close()
      end
    end
    

    if cadzinho.nk_button("⚡ Carrega da PL") then
      modal = 'executa'
      sub_modal = 'planilha'
      msg = ''
      
      local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep

      g_fia_paineis = {}
      local dir = fs.dir(aux)
      if not dir then 
        modal = ''
        sub_modal = ''
        msg = 'Erro: Diretório'
      end -- sai em caso de erro
      for i = 1, #dir do
        local painel = string.match(dir[i].name, 'fiacao_(%w+).xlsx')
        if painel then
          g_fia_paineis[#g_fia_paineis+1] = painel
        end
      end
      table.sort(lista_esq_o)
    end
   
    --[[if cadzinho.nk_button(" Chicote") then --
      num_pt = 1
      modal = 'chicote'
      msg = ''
    end]]--
    
    if cadzinho.nk_button(" Componente") then
      num_pt = 1
      modal = 'componente'
      msg = ''
    end 
    
    if cadzinho.nk_button(" Módulo") then
      num_pt = 1
      modal = 'modulo'
      msg = ''
    end
    
    if cadzinho.nk_button(" Régua Bornes") then
      num_pt = 1
      modal = 'regua'
      msg = ''
    end
    
    if cadzinho.nk_button(" Régua Sec") then
      num_pt = 1
      modal = 'secion'
      msg = ''
    end
    
    if cadzinho.nk_button(" Limpa Ligações") then
      -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
      atualiza_unicos()
      -- atualiza a lista principal com os elementos
      atualiza_elems()
      local componentes = obtem_fia_comp()
      
      for id, comp in pairs(componentes) do
        for i = 1, #comp.terms do
          muda_lig(comp.terms[i], '-')
        end
        
        for m, mod in pairs(comp.mod)do
          for i = 1, #mod.terms do
            muda_lig(mod.terms[i], '-')
          end
        end
      end
    end
    
    --[[if cadzinho.nk_button(" Ligação") then
      num_pt = 1
      modal = 'ligacao'
      msg = ''
    end
    
    if cadzinho.nk_button("⚡ Calcula") then
      modal = 'calcula'
      sub_modal = ''
      msg = ''
    end]]--
    
    if g_fia_bd.painel ~= nil then
      cadzinho.nk_label("Carregado: " .. g_fia_bd.painel)
      if cadzinho.nk_button("Salva  ") then
        if grava_pl_fiacao() then msg = 'Sucesso'
        else  msg = 'Falha' end
      end
      
      if cadzinho.nk_button("Fiação  Desenho") then
        -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
        atualiza_unicos()
        -- atualiza a lista principal com os elementos
        atualiza_elems()
        local componentes = obtem_fia_comp()
        
        for id, comp in pairs(componentes) do
          local id_fia = tostring(texto_el(comp.fia))
          
          local t_esq = g_fia_bd.componentes[id_fia]
          if t_esq and comp.esq then
            local el = elems_pelicanu[comp.esq]
            if el then 
              cadzinho.text_gsub(el.ent, ".*", t_esq.esq)
              el.ent:write()
            end
          end
          if t_esq and t_esq.le and comp.le then
            local el = elems_pelicanu[comp.le]
            if el then 
              cadzinho.text_gsub(el.ent, ".*", "(" .. t_esq.le .. ")")
              el.ent:write()
            end
          end
          
          
          --g_fia_bd.fiacao = fiacao
          
          print('===============teste=======================')
          --[[for c_id, comp in pairs (fiacao) do
            for m_id, m in pairs(comp) do
              if type(m) == 'table' then
                for t_id, t in pairs (m) do
                  print(c_id .. '.' .. m_id .. '.' .. t_id .. '-' .. t)
                end
              else
                print(c_id .. '.' .. m_id .. '-' .. m)
              end
            end
          end
          
          print('***************** ESQUEMATICO ******************')
          for c_id, comp in pairs (componentes) do
            print(c_id .. ' - ' .. comp.esq .. ' - ' .. tostring(comp.le))
          end]]--
          
          
          for i = 1, #comp.terms do
            local term, lig = pega_term(comp.terms[i])
            
            local bd_comp = g_fia_bd.fiacao[id_fia]
            if type(bd_comp) == 'table' and term and bd_comp[term] and bd_comp[term].tipo_ ~= 'modulo' then
              val = bd_comp[term].l
              if bd_comp[term].t then val = val .. '|BT' end
              if bd_comp[term].c then val = val .. '(*)' end

              muda_lig(comp.terms[i], val)
            else muda_lig(comp.terms[i], '-') end
          end
          
          --print (cadzinho.text_gsub(el.ent, "87", "21", 2))
          --el.ent:write()
          
          for m, mod in pairs(comp.mod)do
            local id_modulo = texto_el(mod.id)
            for i = 1, #mod.terms do
              local term, lig = pega_term(mod.terms[i])
              local bd_comp = g_fia_bd.fiacao[id_fia]
              if type(bd_comp) == 'table' and id_modulo and 
                type(bd_comp[id_modulo]) == 'table' and term and
                bd_comp[id_modulo][term] and
                type(bd_comp[id_modulo][term].l) == 'string' then
                val = bd_comp[id_modulo][term].l
                if bd_comp[id_modulo][term].t then val = val .. '|BT' end
                if bd_comp[id_modulo][term].c then val = val .. '(*)' end
                muda_lig(mod.terms[i], val)
              else muda_lig(mod.terms[i], '-') end
            end
          end
        end
      end
      
    end
    
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
    
  end
end
