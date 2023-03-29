-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'

-- ================= Esquemático ===============
function esquematico_dyn (event)
  if modal == 'ligacao' then
    -- funcao interativa para criacao de uma ligação
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Nova ligacao")
    
    cadzinho.nk_option(g_ligacao_tipo)
    
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
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      local prop = {color = 1, ltype = "Continuous", lw = 0}
      if g_ligacao_tipo.value ~= 1 then
        prop = {color = 20, ltype = "Continuous", lw = 17}
      end
      
      -- o elemento "ligacao" eh uma linha simples
      local ligacao = cadzinho.new_line(pts[1].x, pts[1].y, 0,
        pts[2].x, pts[2].y, 0, prop)
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
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  elseif modal == 'componente' then
    -- funcao interativa para acrescentar um componente ao desenho, de uma biblioteca

    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Adiciona um componente")
    
    -- armazena o ponto atual na lista
    pts[num_pt] = {}
    pts[num_pt].x = event.x
    pts[num_pt].y = event.y
    
    
    cadzinho.nk_layout(20, 1)
    if num_pt == 1 then
      cadzinho.nk_label('Escolha:')
      --cadzinho.nk_combo(g_tipo_comp)
      cadzinho.nk_layout(180, 1)
      if cadzinho.nk_group_begin("Biblioteca", false, true, true) then
        cadzinho.nk_layout(15, 1)
        obtem_lista_esq()
        for _, nome in ipairs(lista_esq_o) do      
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
        if type(lista_esq[g_componente.value]) == 'string' then
          local comp = cadzinho.new_insert(g_componente.value,
            pts[num_pt].x, pts[num_pt].y)
          if comp == nil then
            if cadzinho.new_block_file(lista_esq[g_componente.value],
              g_componente.value, "componente PELICAnU ".. g_componente.value,
              true, '#', '*', '$', '?', 0, 0, 0)
            then
              comp = cadzinho.new_insert(g_componente.value,
                pts[num_pt].x, pts[num_pt].y)
            end
          end
          if comp then
            local dados = pega_attrib(comp)
            local tipo = dados.TIPO
            if dados.SUB then
              g_sub.value = dados.SUB
            end
            local terminais = info_terminais(comp)
            g_terminais = {}
            for i, term in pairs(terminais) do
              g_terminais[i] = {value = term}
            end
            if tipo == 'ENGATE' then
              g_engate.value = dados.ENGATE
            else
              g_comp_id.value = pega_comp_id(comp)
            end
            if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
              local descricoes = info_descricoes(comp)
              g_descricoes = {}
              for i, descr in pairs(descricoes) do
                g_descricoes[i] = {value = descr}
              end
            end
            
            num_pt = 2
          end
        end
      end
      if cadzinho.nk_button("Cancela") then
        modal = ''
      end
      
    else
      local comp = cadzinho.new_insert(g_componente.value, pts[num_pt].x, pts[num_pt].y)
      if comp then cadzinho.ent_draw(comp) end
      local dados = pega_attrib(comp)
      local tipo = dados.TIPO
      cadzinho.nk_label(g_componente.value)
      cadzinho.nk_label('Entre o ponto')
      cadzinho.nk_layout(20, 2)
      if tipo == 'ENGATE' then
        cadzinho.nk_label("Engate:")
        cadzinho.nk_edit(g_engate)
      else
        cadzinho.nk_label("ID:")
        cadzinho.nk_edit(g_comp_id)
      end
      if dados.SUB then
        cadzinho.nk_label("Sub:")
        cadzinho.nk_edit(g_sub)
      end
      if tipo ~= 'BORNE_SEC' then
        cadzinho.nk_layout(20, 1)
        cadzinho.nk_label("Terminais:")
        cadzinho.nk_layout(20, 2)
        for i, term in pairs(g_terminais) do
          cadzinho.nk_label(tostring(i)..':')
          cadzinho.nk_edit(term)
        end
      end
      if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
        cadzinho.nk_layout(20, 1)
        cadzinho.nk_label("Descrições:")
        cadzinho.nk_layout(20, 2)
        for i, descr in pairs(g_descricoes) do
          cadzinho.nk_label(tostring(i)..':')
          cadzinho.nk_edit(descr)
        end
      end
      if event.type == 'enter' then
        if tipo == 'ENGATE' then
          muda_engate (comp, g_engate.value)
        else
          muda_comp_id (comp, g_comp_id.value)
        end
        local terminais = {}
        for i, term in pairs(g_terminais) do
          terminais[i] = term.value
        end
        muda_terminais(comp, terminais)
        if dados.SUB then muda_atrib(comp, {SUB = g_sub.value}) end
        if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
          local descricoes = {}
          for i, descr in pairs(g_descricoes) do
            descricoes[i] = descr.value
          end
          muda_descricoes(comp, descricoes)
        end
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        cadzinho.add_ext(comp, "PELICANU", {cadzinho.unique_id(), "COMPONENTE"})
        comp:write()
        cadzinho.clear_sel()
        --modal = ''
        --num_pt = 1
      elseif event.type == 'cancel' then
        num_pt = 1
      end
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  elseif modal == 'caixa' then
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
        --cadzinho.set_color("by layer")
        --cadzinho.set_lw("by layer")
        --cadzinho.set_ltype("bylayer")
        modal = ''
      end
    else
      cadzinho.nk_label('Proximo ponto')
      
      -- o elemento "caixa" principal -  eh uma polyline no formato retangulo
      --cadzinho.set_ltype("Dashdot") -- linha traco-ponto
      --cadzinho.set_color(3) -- cor verde
      local cor = g_caixa_tipo.value + 2
      local caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0,
        pts[2].x, pts[1].y, 0,
        {color = cor, ltype = "Dashdot"})
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
      local caixa_id = cadzinho.new_text(tx-0.6, ty-0.6, g_caixa_id.value,
        2.0, "right", "top", {color = cor, style = "ISO"})
      
      -- desenha um retangulo simples em volta do texto do identificador
      local retan_txt = nil
      if caixa_id then 
        cadzinho.ent_draw(caixa_id) 
        local cx_tx = cadzinho.get_bound(caixa_id)
        cx_tx.low.x = cx_tx.low.x - 0.3
        cx_tx.low.y = cx_tx.low.y - 0.3
        cx_tx.up.x = cx_tx.up.x + 0.3
        cx_tx.up.y = cx_tx.up.y + 0.3
        --cadzinho.set_ltype("Continuous") -- linha continua
        retan_txt = cadzinho.new_pline(cx_tx.low.x, cx_tx.low.y, 0,
        cx_tx.up.x, cx_tx.low.y, 0,
        {color = cor, ltype = "Continuous"})
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
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  elseif modal == 'edita' then
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
    local sub = nil
    if #sel > 0 then
      tipo = pega_comp_tipo(sel[1])
      local dados = pega_attrib(sel[1])
      if dados.SUB then
        sub = dados.SUB
      end
    end
    
    if #sel > 0 and  num_pt == 1 then
      if tipo and #sel == 1 then
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
        if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
          local descricoes = info_descricoes(sel[1])
          g_descricoes = {}
          for i, descr in pairs(descricoes) do
            g_descricoes[i] = {value = descr}
          end
        end
        if sub then
          g_sub.value = sub
        end
      elseif tipo and #sel > 1 and tipo == 'ENGATE' then
        cadzinho.clear_sel()
      elseif tipo and #sel > 1 then
        g_comp_id.value = pega_comp_id(sel[1])
        num_pt = 2
      else
        cadzinho.clear_sel()
      end
    end
    
    cadzinho.nk_layout(20, 1)
    if num_pt == 1 then
      cadzinho.nk_label('Selecione um componente')
      
      if event.type == 'cancel' then
        modal = ''
      end
    else
    
      cadzinho.nk_label('Confirme')
      cadzinho.nk_layout(20, 2)
      if #sel > 1 then
        cadzinho.nk_label("ID:")
        cadzinho.nk_edit(g_comp_id)
        if event.type == 'enter' then
          for i = 1, #sel do
            muda_comp_id (sel[i], g_comp_id.value)
            sel[i]:write()
          end
          cadzinho.clear_sel()
          num_pt = 1
        end
        
      else
        if tipo == 'ENGATE' then
          cadzinho.nk_label("Engate:")
          cadzinho.nk_edit(g_engate)
        else
          cadzinho.nk_label("ID:")
          cadzinho.nk_edit(g_comp_id)
        end
        
        if sub then
          cadzinho.nk_label("Sub:")
          cadzinho.nk_edit(g_sub)
        end
        
        if tipo ~= 'BORNE_SEC' then
          cadzinho.nk_layout(20, 1)
          cadzinho.nk_label("Terminais:")
          cadzinho.nk_layout(20, 2)
          for i, term in pairs(g_terminais) do
            cadzinho.nk_label(tostring(i)..':')
            cadzinho.nk_edit(term)
          end
        end
        
        if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
          cadzinho.nk_layout(20, 1)
          cadzinho.nk_label("Descrições:")
          cadzinho.nk_layout(20, 2)
          for i, descr in pairs(g_descricoes) do
            cadzinho.nk_label(tostring(i)..':')
            cadzinho.nk_edit(descr)
          end
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
          
          if tipo == 'ENT_DIG' or tipo == 'SAIDA_DIG' then
            local descricoes = {}
            for i, descr in pairs(g_descricoes) do
              descricoes[i] = descr.value
            end
            muda_descricoes(sel[1], descricoes)
          end
          
          if sub then muda_atrib(sel[1], {SUB = g_sub.value}) end
          
          sel[1]:write()
          
          cadzinho.clear_sel()
          --modal = ''
          num_pt = 1
        end
      end
      if event.type == 'cancel' then
        num_pt = 1
        cadzinho.clear_sel()
      end
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  elseif modal == 'formato' then
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
        modal = ''
      end
    else
      local fmt = cadzinho.new_insert(g_formato.value, pts[num_pt].x, pts[num_pt].y)
      if fmt then cadzinho.ent_draw(fmt) end
      cadzinho.nk_label(g_formato.value)
      cadzinho.nk_label('Entre o ponto')
      cadzinho.nk_layout(20, 2)
      cadzinho.nk_label("Número:")
      cadzinho.nk_edit(g_fmt_id)
      cadzinho.nk_label("Projeto:")
      cadzinho.nk_edit(g_fmt_prj)
      cadzinho.nk_label("Título:")
      cadzinho.nk_edit(g_fmt_tit)
      cadzinho.nk_label("Revisão:")
      cadzinho.nk_edit(g_fmt_rev)
      cadzinho.nk_label("Versão:")
      cadzinho.nk_edit(g_fmt_ver)
      cadzinho.nk_label("Folha:")
      cadzinho.nk_edit(g_fmt_fl)
      cadzinho.nk_label("Próx. fl.:")
      cadzinho.nk_edit(g_fmt_pfl)
      
      
      if event.type == 'enter' then
        local dados = {}
        dados.ident = g_fmt_id.value
        dados.titulo = g_fmt_tit.value
        dados.tipo = 'DIAGRAMA ESQUEMÁTICO'
        dados.projeto = g_fmt_prj.value
        dados.rev = g_fmt_rev.value
        dados.versao = g_fmt_ver.value
        dados.fl = g_fmt_fl.value
        dados.data = projeto.data
        dados.aplic = projeto.aplicacao
        dados.instal = projeto.instalacao
        dados.visto = projeto.visto
        dados.aprov = projeto.aprovacao
        dados.classif = 'RESERVADO'
        dados.p_fl = g_fmt_pfl.value
        muda_atrib (fmt, dados)
        
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        cadzinho.add_ext(fmt, "PELICANU", {cadzinho.unique_id(), "CAIXA", "DESENHO"})
        fmt:write()
        cadzinho.clear_sel()
        --modal = ''
        --num_pt = 1
      elseif event.type == 'cancel' then
        num_pt = 1
      end
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  elseif modal == 'refer' then
    -- funcao interativa para criacao de uma ligação
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Novo Quadro Ref.")
    
    if num_pt == 1 then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label('Escolha o item:')
      cadzinho.nk_layout(100, 1)
      if cadzinho.nk_group_begin("Típicos", false, true, true) then
        cadzinho.nk_layout(15, 2)
        
        for i = 1, #g_ref_le_num do      
          if cadzinho.nk_button(g_ref_le_num[i]) then
            g_ref_item.value = g_ref_le_num[i]
          end
        end
        cadzinho.nk_group_end()
      end
      cadzinho.nk_layout(20, 2)
      cadzinho.nk_label("Item:")
      cadzinho.nk_edit(g_ref_item)
      
      
      cadzinho.nk_layout(15, 1)
      if g_ref_le[g_ref_item.value] then
        if g_ref_le[g_ref_item.value].descr then
          cadzinho.nk_label(g_ref_le[g_ref_item.value].descr) end
        cadzinho.nk_layout(15, 2)
        if g_ref_le[g_ref_item.value].modelo then
          cadzinho.nk_label(g_ref_le[g_ref_item.value].modelo) end
        if g_ref_le[g_ref_item.value].fabr then
          cadzinho.nk_label(g_ref_le[g_ref_item.value].fabr) end
        cadzinho.nk_layout(20, 2)
        if cadzinho.nk_button("Insere") then
          num_pt = 2
        end
      end
    else
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label('Posicione o quadro')
      cadzinho.nk_propertyi("Altura", g_ref_alt, 3, 10)
      --cadzinho.nk_check("Oculta terminal", g_ref_term_ocul)
      cadzinho.nk_propertyi("Aplicação", g_ref_descr, 20, 100)
      --cadzinho.nk_check("Oculta Aplicação", g_ref_term_ocul)
      cadzinho.nk_propertyi("Desenho", g_ref_desenho, 20, 100)
      cadzinho.nk_check("Só Contatos", g_ref_contat)
      
      local quadro = quadro_ref (g_ref_item.value, event.x, event.y, g_ref_contat.value)
      for _, elem in ipairs(quadro) do
        cadzinho.ent_draw(elem)
      end
      if event.type == 'enter' then -- usuario entra o segundo ponto
        for _, elem in ipairs(quadro) do
          elem:write()
        end
      elseif event.type == 'cancel' then  -- usuario cancela
        num_pt = 1
      end
      
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  -- interface inicial
  else
    cadzinho.nk_layout(20, 1)
    if cadzinho.nk_button(" Ligacao") then
      num_pt = 1
      modal = 'ligacao'
      msg = ''
    end
    if cadzinho.nk_button(" Componente") then
      num_pt = 1
      obtem_tipo_comp()
      num_pt = 1
      modal = 'componente'
      msg = ''
    end
    if cadzinho.nk_button("麗 Caixa") then
      num_pt = 1
      modal = 'caixa'
      msg = ''
    end
    if cadzinho.nk_button(" Edita") then
      num_pt = 1
      modal = 'edita'
      msg = ''
    end
    if cadzinho.nk_button(" Formato") then
      num_pt = 1
      obtem_lista_formato()
      modal = 'formato'
      msg = ''
    end
    if cadzinho.nk_button(" Referência") then
      num_pt = 1
      modal = 'refer'
      g_ref_le_num, g_ref_le = tipico_itens()
      msg = ''
    end
  end
end