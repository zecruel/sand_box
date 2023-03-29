-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'


-- ================= Edição de Biblioteca de componentes ===============
function biblioteca_dyn (event)

  if modal == 'terminal' then
    -- funcao interativa para criacao de um terminal, no modo de edição de componente
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Adiciona um terminal")
    cadzinho.nk_option(g_ligacao_tipo)
    
    local sel = cadzinho.get_sel()
    if #sel < 1 then
      num_pt = 1
      cadzinho.enable_sel()
    elseif num_pt == 1 then
      num_pt = 2
    end
    
    cadzinho.nk_layout(20, 1)
    if num_pt == 1 then
      cadzinho.nk_label('Selecione um elemento')
      
      if event.type == 'cancel' then
        modal = ''
      end
    else
      local tipo = 'T'
      if g_ligacao_tipo ~= 1 then tipo = 'H' end
      -- armazena o ponto atual na lista
      pts[num_pt] = {}
      pts[num_pt].x = event.x
      pts[num_pt].y = event.y
      
      cadzinho.nk_propertyi("Numero", g_term_num, 0, 100)
      cadzinho.nk_layout(20, 2)
      cadzinho.nk_label("Nome:")
      cadzinho.nk_edit(g_term_nome)

      local texto = '#' .. tipo .. string.format('%d', g_term_num.value) .. '$' .. g_term_nome.value
      local term_id = cadzinho.new_text(pts[2].x, pts[2].y, texto, 2.0, "left", "middle")
      if term_id then cadzinho.ent_draw(term_id) end
      
      cadzinho.nk_layout(20, 1)
      if num_pt == 2 then
        cadzinho.nk_label('Posicione o texto')
        if event.type == 'enter' then
          num_pt = num_pt + 1
        elseif event.type == 'cancel' then
          modal = ''
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
                cadzinho.add_ext(sel[i], "PELICANU", {cadzinho.unique_id(), "TERMINAL", tipo .. string.format('%d', g_term_num.value)})
                sel[i]:write()
              end
            end
            term_id:write()
          end
          cadzinho.clear_sel()
          modal = ''
          num_pt = 1
        elseif event.type == 'cancel' then
          num_pt = 1
        end
      end
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
    
  --elseif modal == 'formato' then
  --  cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  
  
  -- interface inicial
  else
    cadzinho.nk_layout(20, 1)
    if cadzinho.nk_button(" Terminal") then
      num_pt = 1
      obtem_lista_formato()
      modal = 'terminal'
      msg = ''
    end
  end
end