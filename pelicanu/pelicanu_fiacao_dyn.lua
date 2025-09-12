--coding: utf-8
-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'


-- ================= Edição de Biblioteca de componentes ===============


function fiacao_dyn (event)

  if modal == 'chicote' then
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
        if g_fia_tipo_chic.value == 2 then
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
      
      local terms = novo_term_fiacao (cx, cy, '*')
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
    cadzinho.nk_label("Chicote")
    
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
    
    
    
  --[[elseif modal == 'calcula_malha' then
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
    if cadzinho.nk_button("⚡ Chicote") then --
      num_pt = 1
      modal = 'chicote'
      msg = ''
    end
    
    if cadzinho.nk_button("麗 Componente") then
      num_pt = 1
      modal = 'componente'
      msg = ''
    end
    
    if cadzinho.nk_button("麗 Ligação") then
      num_pt = 1
      modal = 'ligacao'
      msg = ''
    end
    
    if cadzinho.nk_button(" Calcula") then
      modal = 'calcula'
      sub_modal = ''
      msg = ''
    end
    
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
    
  end
end