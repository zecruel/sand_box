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

function conjugate_gradient (A, x, b, parada, max_iter)
-- credito: https://en.wikipedia.org/wiki/Conjugate_gradient_method
  parada = parada or 1e-6
  max_iter = max_iter or 1000
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
	if erro < parada then return erro, 0 end -- verifica criterio de parada
	
	-- obtem p0
	local p = {}
	for l = 1, n do  p[l] = r[l] end
	
	-- comeca as iteracoes
	local alfa, beta
	local num, den
	for k = 1, max_iter do -- max de iteracoes
	
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
		
		if erro < parada then return false, erro, k end -- verifica criterio de parada
		
		-- calcula beta e aplica ao proximo vetor p
		beta = erro / den
		for l = 1, n do
			p[l] = r[l] + beta * p[l]
		end
    
    --cadzinho.gui_refresh()
    coroutine.yield(true, erro, k)
	end
	
	return false, erro, max_iter
end

function malha_dyn (event)

  if modal == 'limite' then
    -- funcao interativa para criacao de uma caixa limite
  
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label("Define limites")
    
    cadzinho.nk_propertyd("Espaçamento", g_malha_espacamento, 0.01)
    
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
      local passosx = math.ceil(math.abs(pts[2].x - pts[1].x) / g_malha_espacamento.value)
      local passosy = math.ceil(math.abs(pts[2].y - pts[1].y) / g_malha_espacamento.value)
      local sx = pts[2].x >= pts[1].x and 1 or -1
      local sy = pts[2].y >= pts[1].y and 1 or -1
      
      for i = 0, passosx do
        lims[#lims + 1] = cadzinho.new_line(pts[1].x + i * sx * g_malha_espacamento.value, 
          pts[1].y, 0, pts[1].x + i * sx * g_malha_espacamento.value,
          pts[1].y + sy * passosy * g_malha_espacamento.value, 0, prop)
      end
      for i = 0, passosy do
        lims[#lims + 1] = cadzinho.new_line(pts[1].x, pts[1].y + i * sy * g_malha_espacamento.value, 0,
          pts[1].x + passosx * sx * g_malha_espacamento.value,
          pts[1].y + i * sy * g_malha_espacamento.value, 0, prop)
      end
      
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
    cadzinho.nk_label("Cabos")
    
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
        
        malha_grade = {}
        local linha = {}
        local y_ant = pts[1].y
        for j = 1, #pts do
          if math.abs(pts[j].y - y_ant) > tolerancia then
            malha_grade[#malha_grade + 1] = linha
            linha = {}
            linha[#linha + 1] = pts[j]
            y_ant = pts[j].y
          else
            linha[#linha + 1] = pts[j]
          end
        end
        if #linha > 0 then malha_grade[#malha_grade + 1] = linha end
        
        
        
        for i = 1, #malha_grade do
          for j, pt in ipairs(malha_grade[i]) do
            for k = 1, #malha_cabos do
              if no_segmento2 (pt, malha_cabos[k]) then
                local dx = malha_cabos[k][2].x - malha_cabos[k][1].x
                local dy = malha_cabos[k][2].y - malha_cabos[k][1].y
                local t = 0
                if math.abs(dx) > tolerancia then
                  t = (pt.x - malha_cabos[k][1].x) / dx
                elseif math.abs(dy) > tolerancia then
                  t = (pt.y - malha_cabos[k][1].y) / dy
                else t = 0 end
                if not malha_cabos[k].pts then malha_cabos[k].pts = {} end
                ponto = {}
                ponto.i = i; ponto.j = j; ponto.t = t
                malha_cabos[k].pts[#malha_cabos[k].pts + 1] = ponto
                
                --[[local prop = {color = 1, ltype = "Continuous", lw = 0}
                local c = cadzinho.new_circle(pt.x, pt.y, 1, prop)
                c:write()]]--
              end
            end
          end
        end
        
        local lins = #malha_grade
        local cols = #malha_grade[1]
        local cams = #malha_camadas
        malha_max_iter = lins * cols * cams
        malha_crit_parada = 1e-4 * malha_i * malha_res_remoto
      
        sub_modal = 'espera'
        
      else
        msg = "Selecione dados válidos"
        modal = ''
        sub_modal = ''
      end
      
    elseif sub_modal == 'param' then
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label('Parâmetros')
      cadzinho.nk_propertyi("Camadas", g_malha_cams, 1, 8)
      
      cadzinho.nk_layout(15, 2)
      for i = 1, g_malha_cams.value do
        cadzinho.nk_propertyd("r" .. i, g_malha_res_cam[i], 0.01)
        cadzinho.nk_propertyd("p" .. i, g_malha_prof_cam[i], 0.01)
      end
      
      cadzinho.nk_layout(15, 1)
      if g_malha_cam_malha.value > g_malha_cams.value then g_malha_cam_malha.value = g_malha_cams.value end
      cadzinho.nk_propertyi("Cam_malha", g_malha_cam_malha, 1, g_malha_cams.value)
      
      cadzinho.nk_propertyd("I_malha", g_malha_i, 1.0)
      
      cadzinho.nk_propertyd("r_cabo", g_malha_res_cabo, 0.00001)
      
      cadzinho.nk_propertyd("R_remoto", g_malha_res_remoto, 0.00001)
      
      if cadzinho.nk_button(" Volta") then
        sub_modal = 'espera'
      end
      
    elseif sub_modal == 'salva' then
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label('Salva resultado')
      cadzinho.nk_label("Caminho:")
      cadzinho.nk_edit(g_malha_arq_res)
      
      if cadzinho.nk_button(" Salva") then
        --sub_modal = 'salva'
        local file = assert(io.open(g_malha_arq_res.value, "w"))
        
        local lins = #malha_grade
        local cols = #malha_grade[1]
        local cams = #malha_camadas
        local pos_c = lins * cols * cams + 1
        
        file:write( "Area (lin, col) =;" .. tostring(lins) .. ";" .. tostring(cols) .. "\n" )
        
        file:write( "Cabos =;" )
        local t = tostring(#malha_cabos)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Corrente =;" )
        local t = tostring(malha_i)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Res_cabo =;" )
        t = tostring(malha_res_cabo)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Res_remoto =;" )
        t = tostring(malha_res_remoto)
        file:write( t:gsub('%.', ',') .. "\n\n" )
        
        file:write( "Camadas = (indice, resistividade, profundidade) \n" )
        for k = 1, cams do
          file:write( ";" .. tostring(k) .. ";")
          t = tostring(malha_camadas[k].res)
          file:write( t:gsub('%.', ',') .. ";")
          t = tostring(malha_camadas[k].prof)
          file:write( t:gsub('%.', ',') .. "\n")
        end
        file:write( "\n" )
        
        file:write( "Cabos na camada =;" )
        t = tostring(malha_cam_cabos)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Max iter=;" )
        t = tostring(malha_max_iter)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Iteracoes =;" )
        t = tostring(malha_iter)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Criterio de parada =;" )
        t = tostring(malha_crit_parada)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "Erro =;" )
        t = tostring(malha_erro)
        file:write( t:gsub('%.', ',') .. "\n" )
        
        file:write( "V Terra remoto =;" )
        t = tostring(malha_v[pos_c])
        file:write( t:gsub('%.', ',') ..  "\n\n" )
        
        file:write( ";x;" )
        for j = 1, cols do
          t = tostring(malha_grade[1][j].x - malha_grade[1][1].x)
          file:write( t:gsub('%.', ',') .. ";")
        end
        file:write( "\ny\n" )
        
        for i = 1, lins do
          t = tostring(malha_grade[i][1].y - malha_grade[1][1].y)
          file:write( t:gsub('%.', ',') .. ";;")
          for j = 1, cols do
            t = tostring(malha_v[(i - 1) * cols + j] - malha_v[pos_c])
            file:write( t:gsub('%.', ',') .. "")
            if ( j < cols) then file:write( ";" ) end
          end
          file:write( "\n" ) 
        end

        
        --[[
        for k = 1, cams do
          for i = 1, lins do
            for j = 1, cols do
              t = tostring(malha_v[(k - 1) * lins + (i - 1) * cols + j] - malha_v[pos_c])
              file:write( t:gsub('%.', ','))
              if ( j < cols) then file:write( ";" ) end
            end
            file:write( "\n" ) 
          end
          file:write( "\n" ) 
        end
        
        t = tostring(malha_v[pos_c])
        file:write( t:gsub('%.', ','))
        ]]--
        
        file:close()
        sub_modal = 'espera'
      end
      if cadzinho.nk_button(" Volta") then
        sub_modal = 'espera'
      end
      
    elseif sub_modal == 'executa' then
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label('Calculando ...')
      cadzinho.nk_label('-Iterações = ' .. malha_iter)
      cadzinho.nk_label('-Max. iter = ' .. malha_max_iter)
      cadzinho.nk_label('-Erro = ' .. string.format("%.2e", malha_erro))
      cadzinho.nk_label('-Critério = ' .. malha_crit_parada)
      if malha_corr_exec then
        _ , malha_corr_exec, malha_erro, malha_iter = coroutine.resume(malha_corrotina, malha_matriz, malha_v, malha_b, malha_crit_parada, malha_max_iter)
      else
        sub_modal = 'espera'
      end
      cadzinho.nk_layout(20, 1)
      if cadzinho.nk_button(" Para") then
        sub_modal = 'espera'
        if malha_corr_exec then coroutine.close(malha_corrotina) end
        malha_corr_exec = false
      end
      
    elseif sub_modal == 'plota' then
    
      cadzinho.nk_layout(20, 1)
      cadzinho.nk_label("Plota resultado")
      
      -- armazena o ponto atual na lista
      --pts[num_pt] = {}
      --pts[num_pt].x = event.x
      --pts[num_pt].y = event.y
      
      cadzinho.nk_layout(20, 1)
      
      cadzinho.nk_label('Selecione o ponto')
      if event.type == 'enter' then
        local lins = #malha_grade
        local cols = #malha_grade[1]
        local cams = #malha_camadas
        local pos_c = lins * cols * cams + 1
        local cores = {}
        --[[cores[1] = 1
        cores[2] = 32
        cores[3] = 2
        cores[4] = 57
        cores[5] = 66
        cores[6] = 3
        cores[7] = 78
        cores[8] = 4
        cores[9] = 154
        cores[10] = 5
        cores[11] = 7
        ]]--
        
        
        cores[1] = 160
        cores[2] = 150
        cores[3] = 140
        cores[4] = 130
        cores[5] = 120
        cores[6] = 110
        cores[7] = 100
        cores[8] = 90
        cores[9] = 80
        cores[10] = 70
        cores[11] = 60
        cores[12] = 50
        cores[13] = 40
        cores[14] = 30
        cores[15] = 20
        cores[16] = 10
        cores[17] = 7
        
        local max_v = math.log(malha_v[1])
        local min_v = math.log(malha_v[1])
        local max_x = malha_grade[1][1].x
        local min_x = malha_grade[1][1].x
        local max_y = malha_grade[1][1].y
        local min_y = malha_grade[1][1].y
        
        for i = 1, lins do
          for j = 1, cols do
            local v = math.log(malha_v[(i - 1) * cols + j])
            local x = malha_grade[i][j].x
            local y = malha_grade[i][j].y
            max_v = v > max_v and v or max_v
            min_v = v < min_v and v or min_v
            max_x = x > max_x and x or max_x
            min_x = x < min_x and x or min_x
            max_y = y > max_y and y or max_y
            min_y = y < min_y and y or min_y
          end
        end
        
        local dv = (max_v - min_v)/16
        local dx = (max_x - min_x)
        local dy = (max_y - min_y)
        local f = dy / (dv * 50)
        if dx < dy then f = dx / (dv * 50) end
          
        
        
        for i = 1, lins -1 do
          for j = 1, cols - 1 do
            local x = malha_grade[i][j].x - malha_grade[1][1].x + event.x
            local y = malha_grade[i][j].y - malha_grade[1][1].y + event.y
            local y1 = malha_grade[i+1][j].y - malha_grade[1][1].y + event.y
            local x1 = malha_grade[i][j+1].x - malha_grade[1][1].x + event.x
            local z = math.log(malha_v[(i - 1) * cols + j])
            local zy = math.log(malha_v[(i) * cols + j])
            local zx = math.log(malha_v[(i - 1) * cols + j + 1])
            
            local cor_i = 1 + ((z + zx) / 2 - min_v) // dv
            
            local prop = {color = cores[cor_i], ltype = "Continuous", lw = 0}
            local l = cadzinho.new_line(x, y, (z - min_v)*f, x1, y, (zx - min_v)*f, prop)
            if l then
              cadzinho.add_ext(l, "PELICANU", {cadzinho.unique_id(),
                "ATERRAMENTO", "V1 = " .. malha_v[(i - 1) * cols + j] .. ", " ..
                "V2 = " .. malha_v[(i - 1) * cols + j + 1]})
              l:write()
            end
            
            cor_i = 1 + ((z + zy) / 2 - min_v) // dv
            
            prop = {color = cores[cor_i], ltype = "Continuous", lw = 0}
            l = cadzinho.new_line(x, y, (z - min_v)*f, x, y1, (zy - min_v)*f, prop)
            if l then
              cadzinho.add_ext(l, "PELICANU", {cadzinho.unique_id(),
                "ATERRAMENTO", "V1 = " .. malha_v[(i - 1) * cols + j] .. ", " ..
                "V2 = " .. malha_v[(i) * cols + j]})
              l:write()
            end
          end
           
        end
        
        
        t = tostring(malha_v[pos_c])
        
        sub_modal = 'espera'
      elseif event.type == 'cancel' then
        -- sai da funcao
        
        sub_modal = 'espera'
      end
    else
      cadzinho.nk_layout(15, 1)
      cadzinho.nk_label('-Cabos = ' .. #malha_cabos)
      cadzinho.nk_label('-Linhas = ' .. #malha_grade)
      cadzinho.nk_label('-Colunas = ' .. #malha_grade[1])
      
      cadzinho.nk_label('-Iterações = ' .. malha_iter)
      cadzinho.nk_label('-Max. iter = ' .. malha_max_iter)
      cadzinho.nk_label('-Erro = ' .. string.format("%.2e", malha_erro))
      cadzinho.nk_label('-Critério = ' .. malha_crit_parada)
      
      cadzinho.nk_layout(20, 1)
      
      if cadzinho.nk_button("璘 Parâmetros") then
        sub_modal = 'param'
        
        
      end
      
      if cadzinho.nk_button(" Executa") then
        sub_modal = 'executa'
        
        malha_camadas = {}
        
        for i = 1, g_malha_cams.value do
          malha_camadas[i] = {res = g_malha_res_cam[i].value, prof = g_malha_prof_cam[i].value}
        end
      
        malha_i = g_malha_i.value
        malha_res_cabo = g_malha_res_cabo.value
        malha_res_remoto = g_malha_res_remoto.value
        --local g_t_remoto = (lins * cols) / res_remoto
        malha_cam_cabos = g_malha_cam_malha.value

        
        local lins = #malha_grade
        local cols = #malha_grade[1]
        local cams = #malha_camadas
        
        --cadzinho.db_print('tam = ', lins, cols, cams)
        
        malha_matriz = {}
        malha_v = {}
        malha_b = {}
        for i = 1, lins * cols * cams + 1 do
          malha_matriz[i] = {}
          malha_v[i] = malha_i * malha_res_remoto
          malha_b[i] = 0
        end
        
        --cadzinho.db_print('matriz = ' .. #malha_matriz)
        
        local pos_l = 1
        local pos_c = 1
        local pt_corrente = false
        for i = 1, #malha_cabos do
          if malha_cabos[i].pts then
            table.sort(malha_cabos[i].pts,  compara_pts_cabo) -- organiza os pontos em ordem na linha do cabo
            for j = 1, #malha_cabos[i].pts - 1 do
              local lin1 = malha_cabos[i].pts[j].i
              local col1 = malha_cabos[i].pts[j].j
              local lin2 = malha_cabos[i].pts[j + 1].i
              local col2 = malha_cabos[i].pts[j + 1].j
              
              -- calcula a condutancia
              local g = 1/(distancia(malha_grade[lin1][col1], malha_grade[lin2][col2]) * malha_res_cabo)
              
              pos_l = (malha_cam_cabos - 1) * cols * lins + (lin1 - 1) * cols + col1
              pos_c = (malha_cam_cabos - 1) * cols * lins + (lin2 - 1) * cols + col2
              malha_matriz[pos_l][pos_c] = -g
              malha_matriz[pos_c][pos_l] = -g
              -- adiciona na diagonal
              if malha_matriz[pos_c][pos_c] then malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + g
              else malha_matriz[pos_c][pos_c] =  g end
              if malha_matriz[pos_l][pos_l] then malha_matriz[pos_l][pos_l] = malha_matriz[pos_l][pos_l] + g
              else malha_matriz[pos_l][pos_l] = g end
              
              if not pt_corrente then
                malha_b[pos_c] = malha_i
                pt_corrente = true
              end
              
            end
          end
        end
        pos_l = 1
        --cadzinho.db_print('matriz = ' .. #malha_matriz)
        
        local dx = malha_grade[1][2].x - malha_grade[1][1].x
        local dy = malha_grade[2][1].y - malha_grade[1][1].y
        local g = 1
        
        for k = 1, cams do
          for i = 1, lins - 1 do
            for j = 1, cols - 1 do
              dx = malha_grade[i][j + 1].x - malha_grade[i][j].x
              dy = malha_grade[i + 1][j].y - malha_grade[i][j].y
              
              -- calcula a condutancia -- arestas em x
              g = (dy * malha_camadas[k].prof) / (4 * dx * malha_camadas[k].res)
              local arestas = {{i = 0, k = 0}, {i = 1, k = 0},
                              {i = 0, k = 1}, {i = 1, k = 1} }
              if (k == cams) then arestas = {{i = 0, k = 0}, {i = 1, k = 0}} end
              for _, aresta in pairs(arestas) do
                pos_l = (k - 1 + aresta.k) * cols * lins + (i - 1 + aresta.i) * cols + j
                pos_c = (k - 1 + aresta.k) * cols * lins + (i - 1 + aresta.i) * cols + j + 1
                -- adiciona na malha_matriz - diagonal
                if malha_matriz[pos_c][pos_c] then malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + g
                else malha_matriz[pos_c][pos_c] =  g end
                if malha_matriz[pos_l][pos_l] then malha_matriz[pos_l][pos_l] = malha_matriz[pos_l][pos_l] + g
                else malha_matriz[pos_l][pos_l] = g end
                -- adiciona na malha_matriz - interconexoes
                if malha_matriz[pos_l][pos_c] then malha_matriz[pos_l][pos_c] = malha_matriz[pos_l][pos_c] - g
                else malha_matriz[pos_l][pos_c] =  -g end
                if malha_matriz[pos_c][pos_l] then malha_matriz[pos_c][pos_l] = malha_matriz[pos_c][pos_l] - g
                else malha_matriz[pos_c][pos_l] = -g end
              end
              
              -- calcula a condutancia -- arestas em y
              g = (dx * malha_camadas[k].prof) / (4 * dy * malha_camadas[k].res)
              arestas = {{j = 0, k = 0}, {j = 1, k = 0},
                          {j = 0, k = 1}, {j = 1, k = 1} }
              if (k == cams) then arestas = {{j = 0, k = 0}, {j = 1, k = 0}} end
              for _, aresta in pairs(arestas) do
                pos_l = (k - 1 + aresta.k) * cols * lins + (i - 1) * cols + j + aresta.j
                pos_c = (k - 1 + aresta.k) * cols * lins + (i) * cols + j + aresta.j
                -- adiciona na malha_matriz - diagonal
                if malha_matriz[pos_c][pos_c] then malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + g
                else malha_matriz[pos_c][pos_c] =  g end
                if malha_matriz[pos_l][pos_l] then malha_matriz[pos_l][pos_l] = malha_matriz[pos_l][pos_l] + g
                else malha_matriz[pos_l][pos_l] = g end
                -- adiciona na malha_matriz - interconexoes
                if malha_matriz[pos_l][pos_c] then malha_matriz[pos_l][pos_c] = malha_matriz[pos_l][pos_c] - g
                else malha_matriz[pos_l][pos_c] =  -g end
                if malha_matriz[pos_c][pos_l] then malha_matriz[pos_c][pos_l] = malha_matriz[pos_c][pos_l] - g
                else malha_matriz[pos_c][pos_l] = -g end
              end
              
              -- calcula a condutancia -- arestas em z
              g = (dx * dy) / (4 * malha_camadas[k].prof * malha_camadas[k].res)
              arestas = {{i = 0, j = 0}, {i = 1, j = 0},
                          {i = 0, j = 1}, {i = 1, j = 1} }
              for _, aresta in pairs(arestas) do
                pos_l = (k - 1) * cols * lins + (i - 1 + aresta.i) * cols + j + aresta.j
                if (k == cams) then -- na ultima camada, liga ao terra remoto
                  pos_c = lins * cols * cams + 1
                else
                  pos_c = (k) * cols * lins + (i - 1 + aresta.i) * cols + j + aresta.j
                end
                -- adiciona na malha_matriz - diagonal
                if malha_matriz[pos_c][pos_c] then malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + g
                else malha_matriz[pos_c][pos_c] =  g end
                if malha_matriz[pos_l][pos_l] then malha_matriz[pos_l][pos_l] = malha_matriz[pos_l][pos_l] + g
                else malha_matriz[pos_l][pos_l] = g end
                -- adiciona na malha_matriz - interconexoes
                if malha_matriz[pos_l][pos_c] then malha_matriz[pos_l][pos_c] = malha_matriz[pos_l][pos_c] - g
                else malha_matriz[pos_l][pos_c] =  -g end
                if malha_matriz[pos_c][pos_l] then malha_matriz[pos_c][pos_l] = malha_matriz[pos_c][pos_l] - g
                else malha_matriz[pos_c][pos_l] = -g end
                
                -- na ultima camada, liga ao terra remoto
                --if (k == cams) then
                --  malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + g_t_remoto 
                --end
              end
            
            end
          end
        end
        pos_c = lins * cols * cams + 1
        malha_matriz[pos_c][pos_c] = malha_matriz[pos_c][pos_c] + 1/malha_res_remoto
        
        
        malha_iter = 1
        malha_erro = 1000
        malha_max_iter = lins * cols * cams
        malha_crit_parada = 1e-4 * malha_i * malha_res_remoto
        malha_corr_exec = false
        malha_corrotina = coroutine.create (conjugate_gradient)
        _ , malha_corr_exec, malha_erro, malha_iter = coroutine.resume(malha_corrotina, malha_matriz, malha_v, malha_b, malha_crit_parada, malha_max_iter)
        
      end
    
      if cadzinho.nk_button(" Salva") then
        sub_modal = 'salva'
        
      end
    
      if cadzinho.nk_button(" Plota") then
        sub_modal = 'plota'
        
        
      end
    
      if cadzinho.nk_button(" Volta") then
        modal = ''
        sub_modal = ''
      end
    
    
      if event.type == 'cancel' then
        -- sai da funcao
        --cadzinho.set_color("by layer")
        --cadzinho.set_lw("by layer")
        --cadzinho.set_ltype("bylayer")
        modal = ''
        sub_modal = ''
      end
      
    end
    
    
    
    
    
    
    
    
    
    
    
    
    
  else
    cadzinho.nk_layout(20, 1)
    cadzinho.nk_label('Malha de aterramento')
    if cadzinho.nk_button("⚡ Cabo") then --
      num_pt = 1
      modal = 'cabo'
      msg = ''
    end
    if cadzinho.nk_button("麗 Limites") then
      num_pt = 1
      modal = 'limite'
      msg = ''
    end
    if cadzinho.nk_button(" Calcula") then
      modal = 'calcula_malha'
      sub_modal = 'ini'
      msg = ''
    end
    
    cadzinho.nk_label(msg) -- exibe mensagem de erro (se houver)
    
    
  end
end