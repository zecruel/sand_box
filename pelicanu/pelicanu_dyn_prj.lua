-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'
  
  
-- ================= Gerenciamento do projeto ===============
function projeto_dyn (event)
  
  -- abre um projeto
  if modal == 'abre' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Abre um projeto")
    cadzinho.nk_label("Caminho:")
    cadzinho.nk_edit(g_caminho)
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      if abre_projeto(g_caminho.value) then
        modal = ''
      else
        msg = 'Erro'
      end
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- cria um novo projeto
  elseif modal == 'novo' then
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label("Novo projeto")
    cadzinho.nk_label("Caminho:")
    cadzinho.nk_edit(g_caminho)
    cadzinho.nk_layout(15, 2)
    cadzinho.nk_label("Título:")
    cadzinho.nk_edit(g_titulo)
    cadzinho.nk_label("Instalação:")
    cadzinho.nk_edit(g_instalacao)
    cadzinho.nk_label("Aplicação:")
    cadzinho.nk_edit(g_aplicacao)
    cadzinho.nk_label("Código:")
    cadzinho.nk_edit(g_codigo)
    cadzinho.nk_label("Revisão:")
    cadzinho.nk_edit(g_rev)
    cadzinho.nk_label("Aprovação:")
    cadzinho.nk_edit(g_aprovacao)
    cadzinho.nk_label("Visto:")
    cadzinho.nk_edit(g_visto)
    cadzinho.nk_label("Projetista:")
    cadzinho.nk_edit(g_projetista)
    cadzinho.nk_label("Descrição:")
    cadzinho.nk_edit(g_descr)
    cadzinho.nk_label("Data:")
    cadzinho.nk_edit(g_data)
    
    if cadzinho.nk_button("OK") then
      local proj = {}
      proj.titulo = g_titulo.value
      proj.instalacao = g_instalacao.value
      proj.aplicacao = g_aplicacao.value
      proj.codigo = g_codigo.value
      proj.rev = g_rev.value
      proj.aprovacao = g_aprovacao.value
      proj.visto = g_visto.value
      proj.projetista = g_projetista.value
      proj.descr = g_descr.value
      proj.data = g_data.value
      if novo_projeto(g_caminho.value, proj) then
        g_titulo.value = ""
        g_instalacao.value = ""
        g_aplicacao.value = ""
        g_codigo.value = ""
        g_rev.value = ""
        g_aprovacao.value = ""
        g_visto.value = ""
        g_projetista.value = ""
        g_descr.value = ""
        g_data.value = ""
        modal = ''
      else
        msg = 'Erro'
      end
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  
  -- Consolida o projeto (todos os desenhos)
  elseif modal == 'consolida' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Consolida o projeto")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Atualiza o banco de dados
  elseif modal == 'atualiza' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Atualiza o banco de dados")
    cadzinho.nk_layout(5, 1)
    if sub_modal == 'executa' then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label("As alterações atuais")
      cadzinho.nk_label("não poderão ser desfeitas")
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Tem certeza?")
      cadzinho.nk_layout(15, 2)
      if cadzinho.nk_button("OK") then
        -- "salva" o documento atual para restaurar depois
        local drwg, dir = cadzinho.get_drwg_path()
      
        local arqs_mod = {} -- arquivos que foram modificados desde a última atualização
        local arqs_bd = {} -- registro dos arquivos antigos
        local arqs_del = 0
        
        -- lê a estrutura atual da pasta de projeto
        local lista_arq = lista_proj()
        
        -- abre o banco de dados
        local bd = sqlite.open(projeto.bd)
        if bd then
          -- pega as informações antigas dos arquivos (caminho e modificação)
          for linha in bd:cols("SELECT * FROM arquivos WHERE caminho <> 'projeto.db';") do -- para cada linha do BD
            arqs_bd[linha.caminho] = linha.modificado
          end
        
          for i = 1, #lista_arq do
            -- verifica se os arquivos da pasta foram modificados
            if arqs_bd[lista_arq[i].name] ~= lista_arq[i].modified then
              -- lista dos modificados
              arqs_mod[#arqs_mod + 1] = lista_arq[i].name
            else
              -- os arquivos que não sofreram alteração são retirados da lista
              -- sobram apenas os arquivos a serem apagados do bd (modificados ou deletados)
              arqs_bd[lista_arq[i].name] = nil
            end
          end
          
          -- processa primeiro as informações a ser apagados do bd
          for arq,_ in pairs(arqs_bd) do
            deleta_arq_db(bd, arq)
            arqs_del = arqs_del + 1
          end
          
          -- Processa os arquivos modificados
          for i = 1, #arqs_mod do
            atualiza_db(bd, arqs_mod[i])
          end
          
          -- grava a estrutura atual do projeto na tabela do bd
          lista_arq = lista_proj() -- obtem as informações atualizadas
          atualiza_lista_arq_bd (bd, lista_arq)
          
          bd:close()
          msg = 'Concluído: ' .. #arqs_mod .. ' desenhos'
          
          local arq_log = io.open(projeto.log, 'a+')
          if arq_log then
            arq_log:write(os.date('%Y/%m/%d-%H:%M:%S') .. 
              ' => Atualização do banco de dados- ' .. 
              #arqs_mod .. ' desenhos modificados e ' ..
              arqs_del ..' excluídos\n')
            
            arq_log:close()
          end
          
        else
          msg = 'Erro no banco de dados'
        end
        sub_modal = ''
        
        -- restaura o documento anterior
        if drwg ~= '' then
          cadzinho.open_drwg (dir .. drwg, true)
        end
        
      end
      if cadzinho.nk_button("Cancela") then
        sub_modal = ''
      end
    else
      cadzinho.nk_layout(20, 2)
      if cadzinho.nk_button("Atualiza") then
        sub_modal = 'executa'
      end
      if cadzinho.nk_button("Volta") then
        modal = ''
      end
    end
    cadzinho.nk_layout(5, 1)
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Gera as planilhas auxiliares
  elseif modal == 'planilha' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Planilhas auxiliares")
    cadzinho.nk_layout(5, 1)
    if sub_modal == 'pl_p_bd' then
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("O banco de dados")
      cadzinho.nk_label("central será atualizado")
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Tem certeza?")
      cadzinho.nk_layout(15, 2)
      if cadzinho.nk_button("OK") then
        sub_modal = ''
      end
      if cadzinho.nk_button("Cancela") then
        sub_modal = ''
      end
    elseif sub_modal == 'bd_p_pl_comp' then
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Grava componentes")
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("A planilha existente")
      cadzinho.nk_label("será apagada")
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Tem certeza?")
      cadzinho.nk_layout(15, 2)
      if cadzinho.nk_button("OK") then
        if not grava_pl_comp () then
          msg = 'Erro Ger. Planilhas'
        else
          sub_modal = ''
        end
      end
      if cadzinho.nk_button("Cancela") then
        sub_modal = ''
      end
    elseif sub_modal == 'bd_p_pl_term' then
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Grava Terminais")
     
      cadzinho.nk_check("Numera Auto.", g_num_auto)
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("A planilha existente")
      cadzinho.nk_label("será apagada")
      cadzinho.nk_layout(5, 1)
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label("Tem certeza?")
      cadzinho.nk_layout(15, 2)
      if cadzinho.nk_button("OK") then
        if not grava_pl_term (g_num_auto.value) then
          msg = 'Erro Ger. Planilhas'
        else
          sub_modal = ''
        end
      end
      if cadzinho.nk_button("Cancela") then
        sub_modal = ''
      end
    else
      cadzinho.nk_layout(20, 1)
      if cadzinho.nk_button("BD  Pl. Base") then
        msg = ''
        if le_pl_base() then
          msg = 'Sucesso'
        else
          msg = 'Erro Leit. Plan.'
        end
      end
      if cadzinho.nk_button("BD  Componentes ") then
        msg = ''
        sub_modal = 'bd_p_pl_comp'
      end
      if cadzinho.nk_button(" BD  Componentes") then
        msg = ''
        sub_modal = 'pl_p_bd'
      end
      if cadzinho.nk_button("BD  Terminais ") then
        msg = ''
        sub_modal = 'bd_p_pl_term'
      end
      if cadzinho.nk_button(" BD  Terminais") then
        msg = ''
       -- sub_modal = 'pl_p_bd'
        
        if terminais_pl_bd() then
          msg = 'Sucesso'
        else
          msg = 'Erro Leit. Plan.'
        end
      end
      
      if cadzinho.nk_button("Volta") then
        msg = ''
        modal = ''
      end
    end
      
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Carrega os dados das planilhas auxiliares
  elseif modal == 'carrega' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Atualiza desenho atual")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      atualiza_ref_desenho()
      modal = ''
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_layout(5, 1)
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Limpa o banco de dados
  elseif modal == 'limpa' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Limpa o banco de dados")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
      if bd_novo(projeto.bd) then
        local arq_log = io.open(projeto.log, 'a+')
        if arq_log then
          arq_log:write(os.date('%Y/%m/%d-%H:%M:%S')..' => Banco de dados reiniciado\n')
          
          arq_log:close()
        end
        modal = ''
      else
        msg = 'Erro'
      end
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- interface inicial
  else
    sub_modal = ''
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Gerencia o Projeto")
    -- barra de botões
    cadzinho.nk_layout(20, 4)
    if cadzinho.nk_button("") then
      g_caminho.value = ""
      msg = ''
      modal = 'abre'
    end
    if cadzinho.nk_button("") then
      g_caminho.value = ""
      msg = ''
      g_titulo.value = ""
      g_instalacao.value = ""
      g_aplicacao.value = ""
      g_codigo.value = ""
      g_rev.value = ""
      g_aprovacao.value = ""
      g_visto.value = ""
      g_projetista.value = ""
      g_descr.value = ""
      g_data.value = ""
      modal = 'novo'
    end
    if cadzinho.nk_button("") then
      modal = 'atualiza'
      msg = ''
    end
    if cadzinho.nk_button("") then
      modal = 'consolida'
      msg = ''
    end
    if cadzinho.nk_button("") then
      modal = 'planilha'
      msg = ''
    end
    if cadzinho.nk_button("") then
      modal = 'carrega'
      msg = ''
    end
    if cadzinho.nk_button("") then
      modal = 'limpa'
      msg = ''
    end
    if cadzinho.nk_button("拓") then
      local exe = io.popen("start C:\\util\\webui-windows-gcc-x64\\main.exe --proj " .. projeto.caminho .. "_dados")
      if not exe then
        msg = 'erro'
      else
        exe:close()
        msg =""
      end
    end
    cadzinho.nk_layout(5, 1)
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label("Projeto atual:")
    cadzinho.nk_label(projeto.titulo)
    cadzinho.nk_label(projeto.descr)
    cadzinho.nk_layout(15, 2)
    cadzinho.nk_label("Instalação:")
    cadzinho.nk_label(projeto.instalacao)
    cadzinho.nk_label("Aplicação:")
    cadzinho.nk_label(projeto.aplicacao)
    cadzinho.nk_label("Código:")
    cadzinho.nk_label(projeto.codigo)
    cadzinho.nk_label("Revisão:")
    cadzinho.nk_label(projeto.rev)
    cadzinho.nk_label("Aprovação:")
    cadzinho.nk_label(projeto.aprovacao)
    cadzinho.nk_label("Visto:")
    cadzinho.nk_label(projeto.visto)
    cadzinho.nk_label("Projetista:")
    cadzinho.nk_label(projeto.projetista)
    cadzinho.nk_label("Data:")
    cadzinho.nk_label(projeto.data)
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label("Caminho:")
    cadzinho.nk_label(projeto.caminho)
    --outros ícones "罹 ﮏ  ﮻    ﯨ ﯱ     "
  end
end