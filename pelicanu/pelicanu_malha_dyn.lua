--coding: utf-8
-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8


-- Maquina de estados para funções dinâmicas (GUI - CadZinho) do PELICANU
-- Estado principal - variável global 'modal'
-- Estado secundário - variável global 'sub_modal'


-- ================= Edição de Biblioteca de componentes ===============
function compara_pts(pt1, pt2)
	if (pt1.y < pt2.y) then return true
	elseif (pt1.y == pt2.y) then
		if (pt1.x < pt2.x) then return true
		else return false end
	else return false end
end

function compara_pts_cabo(pt1, pt2)
	if (pt1.t < pt2.t) then return true
	else return false end
end

function distancia(pt1, pt2)
	local dx = pt2.x - pt1.x
	local dy = pt2.y - pt1.y
	return math.sqrt(dx * dx + dy * dy)
end

function conjugate_gradient (A, x, b)
-- credito: https://en.wikipedia.org/wiki/Conjugate_gradient_method

	--inicializa
	n = #A
	local r = {}
	local tmp = 0
	local erro = 0
	
	-- obtem r0
	for l = 1, n do
		tmp = 0
		for c, v in pairs(A[l]) do
			tmp = tmp + v * x[c]
		end
		r[l] = b[l] - tmp
		erro = erro + r[l] * r[l]
	end
	if erro < 1e-6 then return erro, 0 end -- verifica criterio de parada
	
	-- obtem p0
	local p = {}
	for l = 1, n do  p[l] = r[l] end
	
	-- comeca as iteracoes
	local alfa, beta
	local num, den
	for k = 1, 2000 do -- max de 2000 iteracoes
	
		-- calcula alfa -> escalar
		num = 0; den = 0
		for l = 1, n do
			tmp = 0
			for c, v in pairs(A[l]) do
				tmp = tmp + v * p[c]
			end
			den = den + p[l] * tmp
			num = num + r[l] * r[l]
		end
		alfa = num / den
		
		-- proximo vetor de resultado
		for l = 1, n do
			x[l] = x[l] + alfa * p[l]
		end
		
		-- calcula o vetor de restos r e os componentes de beta
		den = 0; erro = 0
		for l = 1, n do
			tmp = 0
			for c, v in pairs(A[l]) do
				tmp = tmp + v * p[c]
			end
			den = den +  r[l] * r[l]
			r[l] = r[l] - alfa * tmp
			erro = erro + r[l] * r[l]
		end
		
		if erro < 1e-6 then return erro, k end -- verifica criterio de parada
		
		-- calcula beta e aplica ao proximo vetor p
		beta = erro / den
		for l = 1, n do
			p[l] = r[l] + beta * p[l]
		end
	end
	
	return erro, k
end

function malha_dyn (event)

  if modal == 'limite' then
    -- funcao interativa para criacao de uma caixa limite
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Caixa limite")
    
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
      local prop = {color = 4, ltype = "Hidden", lw = 0}
      local lims = {}
      lims[1] = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[1].y, 0, prop)
      lims[2] = cadzinho.new_line(pts[2].x, pts[1].y, 0, pts[2].x, pts[2].y, 0, prop)
      lims[3] = cadzinho.new_line(pts[2].x, pts[2].y, 0, pts[1].x, pts[2].y, 0, prop)
      lims[4] = cadzinho.new_line(pts[1].x, pts[2].y, 0, pts[1].x, pts[1].y, 0, prop)
      
      lims[5] = cadzinho.new_line(pts[1].x, pts[1].y + (pts[2].y - pts[1].y) / 3, 0, pts[2].x, pts[1].y + (pts[2].y - pts[1].y) / 3, 0, prop)
      lims[6] = cadzinho.new_line(pts[1].x, pts[1].y + (pts[2].y - pts[1].y) * 2/3, 0, pts[2].x, pts[1].y + (pts[2].y - pts[1].y) * 2/3, 0, prop)
      lims[7] = cadzinho.new_line(pts[1].x + (pts[2].x - pts[1].x) * 1/3, pts[1].y, 0, pts[1].x + (pts[2].x - pts[1].x) * 1/3, pts[2].y, 0, prop)
      lims[8] = cadzinho.new_line(pts[1].x + (pts[2].x - pts[1].x) * 2/3, pts[1].y, 0, pts[1].x + (pts[2].x - pts[1].x) * 2/3, pts[2].y, 0, prop)
      for _, l in pairs(lims) do cadzinho.ent_draw(l) end
      

      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        for _, l in pairs(lims) do
          cadzinho.add_ext(l, "PELICANU", {cadzinho.unique_id(), "ATERRAMENTO", "LIMITE"})
          l:write()
        end
        
        
        num_pt = 1
      
        
      elseif event.type == 'cancel' then

        num_pt = 1
      end
    
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
    elseif modal == 'cabo' then
    -- funcao interativa para criacao dos cabos
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Caixa limite")
    
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
      local prop = {color = 30, ltype = "Continuous", lw = 11}
      local cabo = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0, prop)
      if cabo then cadzinho.ent_draw(cabo) end
      

      if event.type == 'enter' then
        cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
        
        if cabo then
          cadzinho.add_ext(cabo, "PELICANU", {cadzinho.unique_id(), "ATERRAMENTO", "CABO"})
          cabo:write()
        end
        
        
        num_pt = 1
      
        
      elseif event.type == 'cancel' then

        num_pt = 1
      end
    
    end
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
  else
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label('Malha de aterramento')
    if cadzinho.nk_button("cabo") then
      num_pt = 1
      modal = 'cabo'
      msg = ''
    end
    if cadzinho.nk_button("麗 Limite") then
      num_pt = 1
      modal = 'limite'
      msg = ''
    end
    if cadzinho.nk_button("calcula") then
	cabos = {}
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
					cabos[#cabos +1] = cadzinho.get_points(ent)
				end
			end
			--local unic = tonumber(unico,16)
			--elem.id = unic
			--elem.ent = ent
			--elem.tipo = tipo
			--elem.esp = especifico
		end
	end
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
	
	local grade = {}
	local linha = {}
	local y_ant = pts[1].y
	for j = 1, #pts do
		if math.abs(pts[j].y - y_ant) > tolerancia then
			grade[#grade +1] = linha
			linha = {}
			linha[#linha+1] = pts[j]
			y_ant = pts[j].y
		else
			linha[#linha+1] = pts[j]
		end
	end
	if #linha > 0 then grade[#grade +1] = linha end
	
	
	
	for i = 1, #grade do
		for j, pt in ipairs(grade[i]) do
			for k = 1, #cabos do
				if no_segmento2 (pt, cabos[k]) then
					local dx = cabos[k][2].x - cabos[k][1].x
					local dy = cabos[k][2].y - cabos[k][1].y
					local t = 0
					if math.abs(dx) > tolerancia then
						t = (pt.x - cabos[k][1].x) / dx
					elseif math.abs(dy) > tolerancia then
						t = (pt.y - cabos[k][1].y) / dy
					else t = 0 end
					if not cabos[k].pts then cabos[k].pts = {} end
					ponto = {}
					ponto.i = i; ponto.j = j; ponto.t = t
					cabos[k].pts[#cabos[k].pts + 1] = ponto
					
					--[[local prop = {color = 1, ltype = "Continuous", lw = 0}
					local c = cadzinho.new_circle(pt.x, pt.y, 1, prop)
					c:write()]]--
				end
			end
		end
	end
	
	local camadas = {}
	camadas[1] = {res = 10, prof = 0.1}
	camadas[2] = {res = 10, prof = 0.1}
	camadas[3] = {res = 10, prof = 0.1}
	camadas[4] = {res = 10, prof = 0.1}
	
	local lins = #grade
	local cols = #grade[1]
	local cams = #camadas
	
	local i_malha = 2
	local res_cabo = 0.001
	local res_remoto = 1
	local g_t_remoto = res_remoto / (lins * cols)
	local cam_cabos = 1
	
	
	matriz = {}
	v = {}
	b = {}
	for i = 1, lins * cols * cams do
		matriz[i] = {}
		v[i] = 2
		b[i] = 0
		--for j = 1, lins * cols * cams do
		--	matriz[i][j] = 0
		--end
	end
	local cam = 1
	--local lin = 1
	--local col = 1
	cadzinho.db_print(#grade, #grade[1])
	
	local pos_l = 1
	local pos_c = 1
	local pt_corrente = false
	for i = 1, #cabos do
		if cabos[i].pts then
			table.sort(cabos[i].pts,  compara_pts_cabo) -- organiza os pontos em ordem na linha do cabo
			for j = 1, #cabos[i].pts - 1 do
				local lin1 = cabos[i].pts[j].i
				local col1 = cabos[i].pts[j].j
				local lin2 = cabos[i].pts[j + 1].i
				local col2 = cabos[i].pts[j + 1].j
				
				-- calcula a condutancia
				local g = 1/(distancia(grade[lin1][col1], grade[lin2][col2]) * res_cabo)
				
				pos_l = (cam - 1) * cols * lins + (lin1 - 1) * cols + col1
				pos_c = (cam - 1) * cols * lins + (lin2 - 1) * cols + col2
				matriz[pos_l][pos_c] = -g
				matriz[pos_c][pos_l] = -g
				-- adiciona na diagonal
				if matriz[pos_c][pos_c] then matriz[pos_c][pos_c] = matriz[pos_c][pos_c] + g
				else matriz[pos_c][pos_c] =  g end
				if matriz[pos_l][pos_l] then matriz[pos_l][pos_l] = matriz[pos_l][pos_l] + g
				else matriz[pos_l][pos_l] = g end
				
				if not pt_corrente then
					b[pos_c] = i_malha
					pt_corrente = true
				end
				
			end
		end
	end
	pos_l = 1
	for k = 1, cams do
		for i = 1, lins do
			for j = 1, cols do
				
				if (j < cols) then
					pos_c = (k - 1) * cols * lins + (i - 1) * cols + j + 1
					if k ~= cam_cabos or not matriz[pos_l][pos_c] then
						-- calcula a condutancia
						local g = 1/(distancia(grade[i][j], grade[i][j + 1]) * camadas[k].res)
						matriz[pos_l][pos_c] = -g
						matriz[pos_c][pos_l] = -g
						-- adiciona na diagonal
						if matriz[pos_c][pos_c] then matriz[pos_c][pos_c] = matriz[pos_c][pos_c] + g
						else matriz[pos_c][pos_c] =  g end
						if matriz[pos_l][pos_l] then matriz[pos_l][pos_l] = matriz[pos_l][pos_l] + g
						else matriz[pos_l][pos_l] = g end
					end
				else
					pos_c = (k - 1) * cols * lins + (i - 1) * cols + j - 1
					if k ~= cam_cabos or not matriz[pos_l][pos_c] then
						-- calcula a condutancia
						local g = 1/(distancia(grade[i][j], grade[i][j - 1]) * camadas[k].res)
						
						matriz[pos_l][pos_c] = -g
						matriz[pos_c][pos_l] = -g
					end
				end
				
				if (i < lins) then
					pos_c = (k - 1) * cols * lins + (i) * cols + j
					if k ~= cam_cabos or not matriz[pos_l][pos_c] then
						-- calcula a condutancia
						local g = 1/(distancia(grade[i][j], grade[i + 1][j]) * camadas[k].res)
						
						matriz[pos_l][pos_c] = -g
						matriz[pos_c][pos_l] = -g
						-- adiciona na diagonal
						if matriz[pos_c][pos_c] then matriz[pos_c][pos_c] = matriz[pos_c][pos_c] + g
						else matriz[pos_c][pos_c] =  g end
						if matriz[pos_l][pos_l] then matriz[pos_l][pos_l] = matriz[pos_l][pos_l] + g
						else matriz[pos_l][pos_l] = g end
					end
				else
					pos_c = (k - 1) * cols * lins + (i - 2) * cols + j
					if k ~= cam_cabos or not matriz[pos_l][pos_c] then
						-- calcula a condutancia
						local g = 1/(distancia(grade[i][j], grade[i - 1][j]) * camadas[k].res)
						
						matriz[pos_l][pos_c] = -g
						matriz[pos_c][pos_l] = -g
					end
				end
				
				if (k < cams) then
					pos_c = (k) * cols * lins + (i - 1) * cols + j
						if k ~= cam_cabos or not matriz[pos_l][pos_c] then
						-- calcula a condutancia
						local g = 1/(camadas[k].prof * camadas[k].res)
						
						matriz[pos_l][pos_c] = -g
						matriz[pos_c][pos_l] = -g
						-- adiciona na diagonal
						if matriz[pos_c][pos_c] then matriz[pos_c][pos_c] = matriz[pos_c][pos_c] + g
						else matriz[pos_c][pos_c] =  g end
						if matriz[pos_l][pos_l] then matriz[pos_l][pos_l] = matriz[pos_l][pos_l] + g
						else matriz[pos_l][pos_l] = g end
					end
				else
					matriz[pos_l][pos_l] = matriz[pos_l][pos_l] + g_t_remoto
				end
			
			
				--pos_c = (k - 1) * cols * lins + (i - 1) * cols + j
				
				
				pos_l = pos_l + 1
			end
		end
	end
	
	erro, n = conjugate_gradient (matriz, v, b)
	
	local file = assert(io.open('teste.csv', "w"))
	for k = 1, cams do
		for i = 1, lins do
			for j = 1, cols do
				t = tostring(v[(k - 1) * lins + (i - 1) * cols + j])
				file:write( t:gsub('%.', ','))
				if ( j < cols) then file:write( ";" ) end
			end
			file:write( "\n" ) 
		end
		file:write( "\n" ) 
	end
	file:close()
	
	cadzinho.db_print ('erro =', erro, 'iteracoes =' , n)
	
      end
  end
end