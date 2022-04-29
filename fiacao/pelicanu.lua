-- globals
count = 1
component = {value = ''}
cont_id = {value = ''}
pts = {}
pelicanu ={}
pelicanu.elems = {}

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

function check_in (ent, fence)
	bound = cadzinho.get_bound(ent)
	return in_polygon(bound.low, fence) and in_polygon(bound.up, fence)
end

function pelicanu.get_all()
	pelicanu.elems = {}
	for i, ent in ipairs(cadzinho.get_all()) do
		local ext = cadzinho.get_ext (ent, "PELICANU")
		if #ext > 0 then
			local elem = {}
			local unique = "0"
			local typ = "NONE"
			if type(ext[1]) == "string" then
				unique = ext[1]:upper() -- ignore case
			end
			if type(ext[2]) == "string" then
				typ = ext[2]:upper() -- ignore case
			end
			local uniq = tonumber(unique,16)
			elem.id = uniq
			elem.ent = ent
			elem.type = typ
			
			pelicanu.elems[uniq] = elem
		end
	end
end

function pelicanu.get_content(id)
	local container = pelicanu.elems[id]
	if container == nil then
		return nil
	end
	fence = cadzinho.get_points(container.ent)
	content = {}
	for el_id, el in pairs(pelicanu.elems) do
		if el ~= container then
			if check_in(el.ent, fence) then
				content[#content+1] = el_id
			end
		end
	end
	return content
end

function update_all_unique()
	for i, ent in ipairs(cadzinho.get_all()) do
		ext = cadzinho.get_ext (ent, "PELICANU")
		if #ext > 1 then
			cadzinho.edit_ext_i(ent, "PELICANU", 1, cadzinho.unique_id())
			ent:write()
		end
	end
	return content
end

function find_cont_id (content)
	id = nil
	for i, ent in ipairs(content) do
		-- look for extended data with appid "cadzinho"
		ext = cadzinho.get_ext (ent, "PELICANU")
		if #ext > 1 then
			if type(ext[2]) == "string" then
				descript = ext[2]:upper() -- ignore case
				
				-- identify elements
				-- container
				if descript == "CONT_ID" then
					id = cadzinho.get_text_data(ent)
					break
				end
			end
		end
	end
	if id then return id.text
	else return nil
	end
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
				cadzinho.add_ext(wire, "PELICANU", {cadzinho.unique_id(), "WIRE", "-"})
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
		--pline = cadzinho.new_pline(pts[1].x, pts[1].y, 0, pts[2].x, pts[2].y, 0)
		cadzinho.pline_close(pline, true)
		--[[for i = 3, count do
			cadzinho.pline_append(pline, pts[i].x, pts[i].y, 0)
		end]]--
		if (pline) then cadzinho.ent_draw(pline) end
		tx = pts[2].x
		if tx < pts[1].x then tx = pts[1].x end
		ty = pts[2].y
		if ty < pts[1].y then ty = pts[1].y end
		text = cadzinho.new_text(tx-0.4, ty-0.2, cont_id.value, 2.0, "right", "top")
		if (text) then cadzinho.ent_draw(text) end
		if event.type == 'enter' then
			if pline then
				cadzinho.add_ext(pline, "PELICANU", {cadzinho.unique_id(), "CONTAINER"})
				pline:write()
			end
			if text then
				cadzinho.add_ext(text, "PELICANU", {cadzinho.unique_id(), "CONT_ID"})
				--data = cadzinho.get_text_data(text)
				--cadzinho.db_print(data.align.h)
				text:write()
			end
			count = 1
			--count = count + 1
		elseif event.type == 'cancel' then
			--[[if pline then
				cadzinho.add_ext(pline, "PELICANU", {"CONTAINER", cont_id.value})
				pline:write()
			end]]--
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

function sort_inner_aux (a, b)
	if type(a) == table and type(b) == table then
		for i, test in ipair(a.inner) do
			if b == test then
				return true
			end
		end
	end
	return false
end

function sort_num_inner_aux (a, b)
	if type(a) == table and type(b) == table then
		return #a.inner > #b.inner
	end
	return false
end

function get_containers ()
	local conts = {}
	local wires = {}
	
	update_all_unique()
	pelicanu.get_all()
	
	for el_id, el in pairs(pelicanu.elems) do
		if el.type == "CONTAINER" then
			conts[#conts+1] = el_id
		end
	end
	
	local SetLib = require("Set")
	
	local containeres = {}
	for i, container in ipairs(conts) do
		cont = {}
		content = pelicanu.get_content(container)
		cont["unique"] = container
		cont["type"] = "CONTAINER"
		cont['content'] = SetLib.new(content)
		cont['id'] = i
		cont["inner"] = {}
		
		containeres[container] = cont
	end
	
	sorted={}
	
	--adjust content
	for k, cont in pairs(containeres) do
		for el_id in pairs(cont.content) do
			el = pelicanu.elems[el_id]
			if el.type == "CONTAINER" then
				cont.inner[#cont.inner+1] = containeres[el_id]
			end
		end
		table.sort(cont.inner, sort_inner_aux)
		sorted[#sorted+1] = cont
	end
	
	table.sort(sorted, sort_inner_aux)
	
	for _, cont in ipairs(sorted) do
		
		for _,inner in ipairs(cont.inner) do
			cont.content = cont.content - inner.content
		end
	end
	
	db = sqlite.open('pelicanu.db')
	db:exec('DROP TABLE IF EXISTS elements')
	db:exec('CREATE TABLE elements(uniq INTEGER NOT NULL PRIMARY KEY, type VARCHAR(100) NOT NULL, id VARCHAR(100), parent VARCHAR(100))')
	
	for _, container in ipairs(sorted) do
		
		db:exec("INSERT INTO elements VALUES("..
			string.format('%d', container.unique) ..", '"..
			container.type .."', '"..
			string.format('%d', container.unique) .."', '"..
			container.id..
			"');")
		cadzinho.db_print(string.format('%X', container.unique), container.type, #container.content, container.id )
	end
	
	db:close()
end

function test()
	--for i = 1, 100 do
	--	cadzinho.db_print(cadzinho.unique_id())
	--end
	cadzinho.db_print(cadzinho.last_blk("*D"))
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
	
	if cadzinho.nk_button("test") then
		test()
	end
	
end

cadzinho.win_show("pelicanu_win", "Pelicanu", 200,200,200,200)