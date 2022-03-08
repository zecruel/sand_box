-- globals
count = 1
component = {value = ''}
cont_id = {value = ''}
pts = {}

function in_polygon(pt, poly)
	-- Check if a point is inside a polygon
	
	-- Verify if passed argments are valid
	if type(pt) ~= "table" then return false end
	if type(poly) ~= "table" then return false end
	
	local check = false
	prev = #poly
	
	for i = 1, #poly do
		if ((poly[i].y > pt.y) ~= (poly[prev].y > pt.y)) and 
		(pt.x < (poly[prev].x - poly[i].x) * (pt.y - poly[i].y) / (poly[prev].y - poly[i].y) + poly[i].x) then
			check = not check
		end
		prev = i
	end
	
	return check
end

function wire_dyn(event)
	cadzinho.nk_layout(20, 1)
	cadzinho.nk_label("Place wire")
	
	pts[count] = {}
	pts[count].x = event.x
	pts[count].y = event.y
	
	cadzinho.nk_layout(20, 1)
	if count == 1 then
		cadzinho.nk_label('Enter start point')
		if event.type == 'enter' then
			count = count + 1
			
		elseif event.type == 'cancel' then
			cadzinho.stop_dynamic()
		end
	else
		cadzinho.nk_label('Enter end point')
		cadzinho.set_ltype("Continuous")
		cadzinho.set_color(1)
		wire = cadzinho.new_line(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0)
		cadzinho.ent_draw(wire)
		if event.type == 'enter' then
			if wire then
				cadzinho.add_ext(wire, "PELICANU", {"WIRE", "-"})
				wire:write()
				pts[1].x = event.x
				pts[1].y = event.y
			end
		elseif event.type == 'cancel' then
			count = 1
		end
	
	end
end

function container_dyn(event)
	cadzinho.nk_layout(20, 1)
	cadzinho.nk_label("Place container")
	cadzinho.nk_layout(20, 2)
	cadzinho.nk_label("ID:")
	cadzinho.nk_edit(cont_id)
	
	pts[count] = {}
	pts[count].x = event.x
	pts[count].y = event.y
	
	cadzinho.nk_layout(20, 1)
	if count == 1 then
		cadzinho.nk_label('Enter start point')
		if event.type == 'enter' then
			count = count + 1
		elseif event.type == 'cancel' then
			cadzinho.stop_dynamic()
		end
	else
		cadzinho.nk_label('Enter next point')
		cadzinho.set_ltype("Dashdot")
		cadzinho.set_color(3)
		
		pline = cadzinho.new_pline(pts[1].x, pts[1].y, 0, pts[2].x, pts[1].y, 0)
		cadzinho.pline_append(pline, pts[2].x, pts[2].y, 0)
		cadzinho.pline_append(pline, pts[1].x, pts[2].y, 0)
		cadzinho.pline_close(pline, true)
		--[[for i = 3, count do
			cadzinho.pline_append(pline, pts[i].x, pts[i].y, 0)
		end]]--
		cadzinho.ent_draw(pline)
		if event.type == 'enter' then
			if pline then
				cadzinho.add_ext(pline, "PELICANU", {"CONTAINER", cont_id.value})
				pline:write()
			end
			count = 1
		elseif event.type == 'cancel' then
			count = 1
		end
	
	end
end

function component_dyn(event)
	cadzinho.nk_layout(20, 1)
	cadzinho.nk_label("Place component")
	cadzinho.nk_layout(20, 2)
	cadzinho.nk_label("Component:")
	cadzinho.nk_edit(component)
	
	if event.type == 'cancel' then
		--cadzinho.stop_dynamic()
	end
end

function get_containers ()
	local cont = nil
	local wire = nil
	
	-- sweep all entities in drawing
	for i, ent in ipairs(cadzinho.get_all()) do
		-- look for extended data with appid "cadzinho"
		ext = cadzinho.get_ext (ent, "PELICANU")
		if #ext > 0 then
			if type(ext[1]) == "string" then
				descript = ext[1]:upper() -- ignore case
				
				-- identify elements
				-- container
				if descript == "CONTAINER" then
					-- get points and store in table
					cont = cadzinho.get_points(ent)
				
				-- wires
				elseif descript == "WIRE" then
					-- get points and store in table
					wire = cadzinho.get_points(ent)
				end
			end
		end
	end
	if cont ~= nil and wire ~= nil then
		if in_polygon(wire[1], cont) then cadzinho.db_print("inside 1") end
		if in_polygon(wire[2], cont) then cadzinho.db_print("inside 2") end
	end
end

function pelicanu_win()
	cadzinho.nk_layout(20, 1)
	if cadzinho.nk_button("APPID") then
		cadzinho.new_appid("PELICANU")
	end
	if cadzinho.nk_button("wire") then
		count = 1
		cadzinho.start_dynamic("wire_dyn")
	end
	if cadzinho.nk_button("component") then
		count = 1
		cadzinho.start_dynamic("component_dyn")
	end
	if cadzinho.nk_button("container") then
		count = 1
		cadzinho.start_dynamic("container_dyn")
	end
	
	if cadzinho.nk_button("get container") then
		get_containers()
	end
	
end

cadzinho.win_show("pelicanu_win", "Pelicanu", 200,200,200,200)