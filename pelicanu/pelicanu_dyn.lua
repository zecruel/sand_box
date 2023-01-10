-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'
  
  
-- ================= Gerenciamento do projeto ===============
function projeto_dyn ()
  
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
  
  -- Consolida o projeto (todos os desenhos) para não haver IDs conflitantes
  elseif modal == 'consolida' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Consolida o projeto")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
      --[[if abre_projeto(g_caminho.value) then
        modal = ''
      else
        msg = 'Erro'
      end--]]
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Atualiza o banco de dados
  elseif modal == 'atualiza' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Atualiza o banco de dados")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
      --[[if abre_projeto(g_caminho.value) then
        modal = ''
      else
        msg = 'Erro'
      end]]--
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Gera as planilhas auxiliares
  elseif modal == 'planilha' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Gera as planilhas auxiliares")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
      --[[if abre_projeto(g_caminho.value) then
        modal = ''
      else
        msg = 'Erro'
      end]]--
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
  -- Carrega os dados das planilhas auxiliares
  elseif modal == 'carrega' then
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Carrega planilhas aux")
    cadzinho.nk_label("Tem certeza?")
    cadzinho.nk_layout(15, 2)
    if cadzinho.nk_button("OK") then
      modal = ''
      --[[if abre_projeto(g_caminho.value) then
        modal = ''
      else
        msg = 'Erro'
      end]]--
    end
    if cadzinho.nk_button("Cancela") then
      modal = ''
    end
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
    if cadzinho.nk_button("") then
      modal = 'consolida'
      msg = ''
    end
    if cadzinho.nk_button("") then
      modal = 'atualiza'
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
    cadzinho.nk_layout(5, 1)
    cadzinho.nk_layout(15, 1)
    cadzinho.nk_label("Projeto atual:")
    cadzinho.nk_label(projeto.titulo)
    cadzinho.nk_label(projeto.descr)
    --cadzinho.nk_label("Caminho:")
    --cadzinho.nk_label(projeto.caminho)
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
    --outros ícones "罹ﮏ﮻"
  end
  --[[
  if modal == 'projeto' then
  elseif modal == 'esquematico' then
  
  elseif modal == 'logico' then
  
  elseif modal == 'unifilar' then
  
  elseif modal == 'fiacao' then
  
  elseif modal == 'interligacao' then
  
  elseif modal == 'analise' then
  
  elseif modal == 'biblioteca' then
  
  end]]--
end