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
	
	cadzinho.db_print(#grade, #grade[1])
	
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
	for i = 1, #cabos do
		if cabos[i].pts then
			table.sort(cabos[i].pts,  compara_pts_cabo) -- organiza os pontos em ordem na linha do cabo
			--[[for j = 1, #cabos[i].pts do
				cadzinho.db_print(i, cabos[i].pts[j].i, cabos[i].pts[j].j)
			end]]--
		end
	end
      end
  end
end