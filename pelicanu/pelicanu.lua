-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar

-- ============= variáveis globais =====================
pelicanu ={}  -- tabela principal
pelicanu.elems = {} -- lista principal dos elementos
tolerancia = 0.001 -- tolerancia para comparacoes de numeros não inteiros

-- para funcoes dinamicas
num_pt = 1 -- numero de pontos
pts = {} -- lista de pontos de entrada
lista_comp = {}
lista_comp_o = {}

-- para a interface grafica
component = {value = ''}
g_caixa_id = {value = ''}
g_caixa_tipo = {value = 'COMPONENTE'}
g_editor_abas = {value = 1, "Esquematico", "Biblioteca"}
g_biblioteca = {value = 'C:\\util\\pelicanu\\'}
--g_biblioteca = {value = '/home/ezequiel/pelicanu/'}
g_term_num = {value = 1}
g_term_nome = {value = "1"}
g_eng_num = {value = 1}
g_eng_nome = {value = "E1"}
g_componente = {value = ""}
g_comp_id = {value = ""}
g_terminais = {}

-- ============================================
function nome_arq(url)
  return url:match("(.+)%..+$")
end

function extensao(url)
  return url:match("^.+(%..+)$")
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
	-- funcao interativa para criacao de uma caixa
	
	cadzinho.nk_layout(20, 1)
	cadzinho.nk_label("Nova caixa")
	cadzinho.nk_layout(20, 2)
	cadzinho.nk_label("ID:")
	cadzinho.nk_edit(g_caixa_id)
	cadzinho.nk_label("Tipo:")
	cadzinho.nk_edit(g_caixa_tipo)
	
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
				cadzinho.add_ext(caixa, "PELICANU", {cadzinho.unique_id(), "CAIXA", g_caixa_tipo.value})
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
	
	texto = '#T' .. string.format('%d', g_term_num.value) .. '$' .. g_term_nome.value
	term_id = cadzinho.new_text(pts[1].x, pts[1].y, texto, 2.0, "left", "middle")
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
						ext = cadzinho.get_ext (sel[i], "PELICANU") -- procura pelo marcador extendido
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
	
	texto = '#E' .. string.format('%d', g_eng_num.value) .. '$' .. g_eng_nome.value
	eng_id = cadzinho.new_text(pts[1].x, pts[1].y, texto, 2.0, "center", "middle")
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
					ext = cadzinho.get_ext (sel[i], "PELICANU") -- procura pelo marcador extendido
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

function muda_comp_id (comp, id)
	local id1 = string.match(id, '^([^/]*)/?.*$')
	local id2 = string.match(id, '/(.*)$')
	local idx1 = 0
	local idx2 = 0
	local ocul1 = false
	local ocul2 = false
	
	local attrs = cadzinho.get_attribs(comp)
	
	for i, attr in ipairs(attrs) do
		if string.find(attr['tag'], "^ID1") then
			idx1 = i
			ocul1 = attr['hidden']
		elseif string.find(attr['tag'], "^ID2") then
			idx2 = i
			ocul2 = attr['hidden']
		end
	end
	
	if idx1 > 0 and idx2 > 0 and id1 and id2 then
		id1 = '%%U' .. id1
		cadzinho.edit_attr(comp, idx1, 'ID1', id1, ocul1)
		cadzinho.edit_attr(comp, idx2, 'ID2', id2, ocul2)
	elseif idx1 > 0 then
		
		cadzinho.edit_attr(comp, idx1, 'ID1', id, ocul1)
	end
	
	if idx2 > 0 and not id2 then
		cadzinho.edit_attr(comp, idx2, 'ID2', '', ocul2)
	end
end

function pega_comp_id (comp)
	local id = ''
	local id1 = ''
	local id2 = ''
	local idx1 = 0
	local idx2 = 0
	local attrs = cadzinho.get_attribs(comp)
	
	for i, attr in ipairs(attrs) do
		if string.find(attr['tag'], "^ID1") then 
			idx1 = i
			id1 = attr['value']
			id1 = string.gsub(id1, '(%%%%%a)', '')
		end
		if string.find(attr['tag'], "^ID2") then
			idx2 = i
			id2 = attr['value']
			id2 = string.gsub(id2, '(%%%%%a)', '')
		end
	end
	
	if idx1 > 0 then
		if idx2 > 0 and #id2 > 0 then
			id = id1 .. '/' .. id2
		else
			id= id1
		end
	end
	
	return id
end

function rotacao (pt, ang)
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
	local terminais = {}
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
	local ocul = false
	local attrs = cadzinho.get_attribs(comp)
	
	for i, attr in ipairs(attrs) do
		local t_num = string.match(attr['tag'], "^T(%d)")
		if t_num then
			local term = terminais[tonumber(t_num)]
			if term then
				ocul = attr['hidden']
				cadzinho.edit_attr(comp, i, 'T' .. t_num, term, ocul)
			end
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
		cadzinho.nk_layout(150, 1)
		if cadzinho.nk_group_begin("Biblioteca", true, true, true) then
			cadzinho.nk_layout(20, 1)
			--[[for nome, caminho in pairs(lista_comp) do			
				if cadzinho.nk_button(nome) then
					g_componente.value = nome
				end
			end]]--
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
				local comp = cadzinho.new_insert(g_componente.value, pts[num_pt].x, pts[num_pt].y)
				if comp == nil then
					if cadzinho.new_block_file(lista_comp[g_componente.value], g_componente.value, "componente PELICAnU ".. g_componente.value, true, '#', '*', '$', '?', 0, 0, 0) then
						comp = cadzinho.new_insert(g_componente.value, pts[num_pt].x, pts[num_pt].y)
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

		
		if event.type == 'enter' then
			num_pt = num_pt + 1
		elseif event.type == 'cancel' then
			cadzinho.stop_dynamic()
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
			terminais = {}
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
	
	if #sel > 0 and  num_pt == 1 then
		num_pt = 2
		g_comp_id.value = pega_comp_id(sel[1])
		local terminais = info_terminais(sel[1])
		g_terminais = {}
		for i, term in pairs(terminais) do
			g_terminais[i] = {value = term}
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
		cadzinho.nk_edit(g_comp_id)
		
		cadzinho.nk_layout(20, 1)
		cadzinho.nk_label("Terminais:")
		cadzinho.nk_layout(20, 2)
		for i, term in pairs(g_terminais) do
			cadzinho.nk_label(tostring(i)..':')
			cadzinho.nk_edit(term)
		end
		
		if event.type == 'enter' then
			muda_comp_id (sel[1], g_comp_id.value)
			terminais = {}
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
	caixa['tipo'] = 'FOLHA'
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
	
	db = sqlite.open('pelicanu.db')
	db:exec('DROP TABLE IF EXISTS componentes')
	db:exec('CREATE TABLE componentes('..
		'unico INTEGER, '..
		'bloco TEXT, id TEXT, pai INTEGER)')
	db:exec('DROP TABLE IF EXISTS caixas')
	db:exec('CREATE TABLE caixas('..
		'unico INTEGER, '..
		'id TEXT, tipo TEXT, pai INTEGER)')
	db:exec('DROP TABLE IF EXISTS terminais')
	db:exec('CREATE TABLE terminais('..
		'componente INTEGER, '..
		'id INTEGER, texto TEXT)')
		
	db:exec('DROP TABLE IF EXISTS barras')
	db:exec('CREATE TABLE barras('..
		'id TEXT, '..
		'componente INTEGER, '..
		'terminal INTEGER)')
	
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
			el = pelicanu.elems[el_id]
			--cadzinho.db_print ("    " .. el.tipo)
			if el.tipo == "COMPONENTE" then
				local terms = info_terminais (el.ent)
				local bloco = cadzinho.get_blk_name (el.ent)
				if not bloco then bloco = 'NULL' end
				local comp_id = pega_comp_id(el.ent)
				if not comp_id then comp_id = 'NULL' end
				--cadzinho.db_print ("    " .. bloco, comp_id)
				
				db:exec ("INSERT INTO componentes VALUES("..
					string.format('%d', el_id) ..", '"..
					bloco .."', '"..
					comp_id .."', "..
					pai..
					");")
				for t_id, t in pairs(terms) do
					db:exec ("INSERT INTO terminais VALUES("..
						string.format('%d', el_id) ..", "..
						string.format('%d', t_id) ..", '"..
						t .."');")
				end
			elseif el.tipo == "CAIXA" then
				local sub_caixa = caixas[el_id]
				if sub_caixa then
					local nome = sub_caixa.nome
					if not nome then nome = 'NULL' end
					local tipo = sub_caixa.tipo
					if not tipo then tipo = 'NULL' end
					--cadzinho.db_print ("    " .. nome, tipo)
					
					db:exec ("INSERT INTO caixas VALUES("..
						string.format('%d', el_id) ..", '"..
						nome .."', '"..
						tipo .."', "..
						pai..
						");")
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
			db:exec ("INSERT INTO barras VALUES('"..
				barra.id .."', "..
				string.format('%d', term.comp) ..", "..
				string.match(term.term, "^T(%d)")..
				");")
		end
	end
	
	--
	db:close()
	cadzinho.db_print ("----- Teste concluido  ------")
end


--============== Janela Principal =======================
function pelicanu_win()
	cadzinho.nk_layout(200, 1)
	if cadzinho.nk_tab_begin("modo_ed", g_editor_abas) then
		if g_editor_abas.value == 2 then -- biblioteca
			cadzinho.nk_layout(20, 1)
			cadzinho.nk_label("Caminho:")
			cadzinho.nk_edit(g_biblioteca)
		
		
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
			if cadzinho.nk_button("componente") then
				num_pt = 1
				lista_comp = {}
				lista_comp_o = {}
				dir = fs.dir(g_biblioteca.value)
				for i = 1, #dir do
					local ext = extensao(dir[i].name)
					if type(ext) == "string" and dir[i].is_dir == false then
						if ext:upper() == ".DXF" then
							lista_comp[nome_arq(dir[i].name)] = g_biblioteca.value .. dir[i].name
							lista_comp_o[#lista_comp_o+1] = nome_arq(dir[i].name)
						end
					end
				end
				table.sort(lista_comp_o)
				cadzinho.start_dynamic("componente_dyn")
			end
			if cadzinho.nk_button("caixa") then
				num_pt = 1
				cadzinho.start_dynamic("caixa_dyn")
			end
			if cadzinho.nk_button("Edita Comp") then
				num_pt = 1
				cadzinho.start_dynamic("edita_dyn")
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
cadzinho.win_show("pelicanu_win", "Pelicanu", 220,300,200,250)