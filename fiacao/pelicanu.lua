-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar

-- ============= variáveis globais =====================
pelicanu ={}  -- tabela principal
pelicanu.elems = {} -- lista principal dos elementos

-- para funcoes dinamicas
num_pt = 1 -- numero de pontos
pts = {} -- lista de pontos de entrada

-- para a interface grafica
component = {value = ''}
g_caixa_id = {value = ''}
g_editor_abas = {value = 1, "Esquematico", "Biblioteca"}

-- ============================================

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
	
	limite = cadzinho.get_bound(ent) -- pega os limites do objeto (retangulo)
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
	
	-- contorno da caixa, considerando ser uma polyline
	contorno = cadzinho.get_points(caixa.ent)
	conteudo = {}
	
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
	
	conteudo = {}
	for el_id in pairs(pelicanu.elems) do
		conteudo[#conteudo+1] = el_id
	end
	return conteudo
end

function pelicanu.atualiza_unicos()
	-- atualiza os identificadores únicos nos elementos do desenho
	
	for i, ent in ipairs(cadzinho.get_all()) do -- varre todos os objetos do desenho
		ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
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
		el = pelicanu.elems[el_id]
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
		ligacao = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0)
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
	cadzinho.nk_layout(20, 1)
	cadzinho.nk_label("Nova caixa")
	cadzinho.nk_layout(20, 2)
	cadzinho.nk_label("ID:")
	cadzinho.nk_edit(g_caixa_id)
	
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
		caixa = cadzinho.new_pline(pts[1].x, pts[1].y, 0, pts[2].x, pts[1].y, 0)
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
		tx = pts[2].x
		if tx < pts[1].x then tx = pts[1].x end
		ty = pts[2].y
		if ty < pts[1].y then ty = pts[1].y end
		caixa_id = cadzinho.new_text(tx-0.6, ty-0.6, g_caixa_id.value, 2.0, "right", "top")
		
		-- desenha um retangulo simples em volta do texto do identificador
		local retan_txt = nil
		if caixa_id then 
			cadzinho.ent_draw(caixa_id) 
			cx_tx = cadzinho.get_bound(caixa_id)
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
				cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "CAIXA"})
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


function ordena_cont (a, b)
	--função auxiliar para ordenar tabelas
	-- verifica se b esta contida em a
	if type(a) == table and type(b) == table then
		for i, test in ipair(a.inner) do
			if b == test then
				return true
			end
		end
	end
	return false
end

function obtem_caixas()
	local caixas = {}
	local SetLib = require("Set") -- biblioteca para matematica de conjuntos
	local caixa
	local conteudo
	
	-- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
	pelicanu.atualiza_unicos()
	-- atualiza a lista principal com os elementos
	pelicanu.atualiza_elems()
	
	-- caixa "desenho" para armazenar os elementos órfãos
	caixa = {}
	conteudo = pelicanu.conteudo_todo()
	caixa['nome'] = ""
	caixa['conteudo'] = SetLib.new(conteudo)
	caixa['filhas'] = {}
	caixas['desenho'] = caixa
	
	-- varre os elementos, cadastrando as caixas existentes no desenho
	for el_id, el in pairs(pelicanu.elems) do
		if el.tipo == "CAIXA" then
			caixa = {}
			conteudo = pelicanu.conteudo(el_id)
			caixa['nome'] = ""
			caixa['conteudo'] = SetLib.new(conteudo)
			caixa['filhas'] = {}
			caixas[el_id] = caixa
		end
	end
	
	-- repassa as caixas, cadastrando as caixas aninhadas
	for _, caixa in pairs(caixas) do
		for el_id in pairs(caixa.conteudo) do
			el = pelicanu.elems[el_id]
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

function teste()
	cadzinho.db_print ("teste")
	local caixas = obtem_caixas()
	for id, caixa in pairs(caixas) do
		cadzinho.db_print (caixa.nome)
		for el_id, _ in pairs(caixa.conteudo) do
			el = pelicanu.elems[el_id]
			cadzinho.db_print ("    " .. el.tipo)
		end
	end
end

function get_containers ()
	local caixas = {}
	local ligacoes = {}
	
	caixas[#caixas+1] = 'main'
	
	for el_id, el in pairs(pelicanu.elems) do
		if el.tipo == "CAIXA" then
			caixas[#caixas+1] = el_id
		end
	end
	
	local SetLib = require("Set") -- biblioteca para matematica de conjuntos
	
	local containeres = {}
	for i, container in ipairs(caixas) do
		cont = {}
		if container == 'main' then
			conteudo = pelicanu.conteudo_todo()
		else
			conteudo = pelicanu.conteudo(container)
		end
		cont["unico"] = container
		cont["tipo"] = "CAIXA"
		cont['conteudo'] = SetLib.new(conteudo)
		cont['id'] = i
		cont["inner"] = {}
		
		containeres[container] = cont
	end
	
	sorted={}
	
	--adjust conteudo
	for k, cont in pairs(containeres) do
		for el_id in pairs(cont.conteudo) do
			el = pelicanu.elems[el_id]
			if el.tipo == "CAIXA" then
				cont.inner[#cont.inner+1] = containeres[el_id]
			end
		end
		table.sort(cont.inner, ordena_cont)
		sorted[#sorted+1] = cont
	end
	
	table.sort(sorted, ordena_cont)
	
	for _, cont in ipairs(sorted) do
		
		for _,inner in ipairs(cont.inner) do
			cont.conteudo = cont.conteudo - inner.conteudo
		end
	end
	
	for _, cont in ipairs(sorted) do
		cont.id = pelicanu.rotulo_caixa(cont.conteudo)
		el = pelicanu.elems[cont.unico]
		if el then
			el.esp = cont.id
		end
	end
	
	db = sqlite.open('pelicanu.db')
	db:exec('DROP TABLE IF EXISTS elements')
	db:exec('CREATE TABLE elements('..
		'unic INTEGER, '..
		'tipo TEXT, id TEXT, parent INTEGER)')
	
	for _, container in ipairs(sorted) do
		local parent
		if type(container.unico) == 'number' then
			parent = string.format('%d', container.unico)
		else
			parent = 'NULL'
		end
		for el_id in pairs(container.conteudo) do
			el = pelicanu.elems[el_id]
			if el.tipo == "CAIXA" then
				cont.inner[#cont.inner+1] = containeres[el_id]
			end
			
			db:exec("INSERT INTO elements VALUES("..
			string.format('%d', el_id) ..", '"..
			el.tipo .."', '"..
			el.esp .."', "..
			parent..
			");")
		end
		
		
		cadzinho.db_print(parent, #container.conteudo, container.id )
	end
	
	db:close()
end

--============== Janela Principal =======================
function pelicanu_win()
	cadzinho.nk_layout(200, 1)
	if cadzinho.nk_tab_begin("modo_ed", g_editor_abas) then
		if g_editor_abas.value == 2 then -- biblioteca
			cadzinho.nk_layout(20, 1)
			if cadzinho.nk_button("APPID") then
				cadzinho.new_appid("PELICANU")
			end
			
			
			if cadzinho.nk_button("test") then
				--test()
				--co = coroutine.create(test)
				--coroutine.resume(co)
				cadzinho.print_drwg("teste.pdf", 210, 297, "mm", 5, 0,0)
			end
			
		else -- padrao - esquematico
			cadzinho.nk_layout(20, 1)
			if cadzinho.nk_button("ligacao") then
				num_pt = 1
				cadzinho.start_dynamic("ligacao_dyn")
			end
			if cadzinho.nk_button("component") then
				num_pt = 1
				cadzinho.start_dynamic("component_dyn")
			end
			if cadzinho.nk_button("caixa") then
				num_pt = 1
				cadzinho.start_dynamic("caixa_dyn")
			end
			
			if cadzinho.nk_button("get container") then
				get_containers()
			end
			
			if cadzinho.nk_button("caixas") then
				teste()
			end
		end
		cadzinho.nk_tab_end()
	end
end

-- inicia a janela quando o script eh executado
cadzinho.win_show("pelicanu_win", "Pelicanu", 900,100,300,500)