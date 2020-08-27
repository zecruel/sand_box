tolerance = 0.001 -- tolerance to calculate float point comparassion

function lie_segment (pt, line)
	-- Check if a point lies on line segment
	
	-- Verify if passed argments are valid
	if type(pt) ~= "table" then return false end
	if type(line) ~= "table" then return false end
	
	-- first, check if point is on line
	if math.abs((pt.x - line[1].x) / (line[2].x - line[1].x) - (pt.y - line[1].y) / (line[2].y - line[1].y)) > tolerance then
		return false
	end
	
	-- check if the point is contained in the segment
	if ( pt.x > line[1].x and pt.x < line[2].x or pt.x < line[1].x and pt.x > line[2].x) or ( pt.y > line[1].y and pt.y < line[2].y or pt.y < line[1].y and pt.y > line[2].y) then
		return true
	end
	
	-- all tests fail
	return false
end

function test_connect (a, b)
	-- verify if line segments are connected
	
	-- Verify if passed argments are valid
	if type(a) ~= "table" then return false end
	if type(b) ~= "table" then return false end
	
	-- check if lines are the same object
	if a == b then return false end
	
	-- trivially, tests whether the vertices coincide
	for _, pt1 in ipairs(a) do
		for _, pt2 in ipairs(b) do
			if math.abs(pt1.x - pt2.x) < tolerance and math.abs(pt1.y - pt2.y) < tolerance then
				return true
			end
		end
	end
	
	--So test if any vertex touches the other line
	for _, pt1 in ipairs(a) do
		if lie_segment(pt1, b) then return true end
	end
	for _, pt1 in ipairs(b) do
		if lie_segment(pt1, a) then return true end
	end
	
	-- all tests fail
	return false
end

function get_term (ent)
	-- get component terminals from DXF block
	-- passed arguments: DXF INSERT entity
	-- returns: a table with terminals
	
	-- terminals are DXF LINE entities marked with extended data 
	--     mark = "terminal", appid = "cadzinho"
	--     second ext data = terminal id
	
	local terms = {} -- create main table
	
	-- get block name
	local name = cadzinho.get_blk_name (ent)
	if not name then return terms end -- fail to find block name
	
	-- get insert point to offset
	local pts = cadzinho.get_points(ent)
	if type(pts) ~= "table" then return terms end -- fail to get points
	local ins_pt = pts[1]
	if type(ins_pt) ~= "table" then return terms end -- fail to get insert point
	
	-- sweep entities from DXF BLOCK table
	for _, inside in ipairs(cadzinho.get_blk_ents(name)) do
		-- get extended data table
		local ext = cadzinho.get_ext (inside, "cadzinho")
		if #ext > 0 then
			-- try to find terminal mark
			if type(ext[1] == "string") then
				local descript = ext[1]:upper()
				if descript == "TERMINAL" then
					--found mark then get points and id
					local term_pts = {}
					for i, pt in ipairs(cadzinho.get_points(inside)) do
						-- offset points with insert point
						term_pts[i] = {}
						term_pts[i].x = ins_pt.x + pt.x
						term_pts[i].y = ins_pt.y + pt.y
						term_pts[i].z = ins_pt.z + pt.z
					end
					-- store terminal points in table, with id as key
					terms[ext[2]] = term_pts
				end
			end
		end
	end
	return terms
end


-- ================ Main routine =====================

-- wires are DXF LINE entities marked with extended data 
--     mark = "wire", appid = "cadzinho"
wires = {}

-- components are DXF INSERT entities marked with extended data 
--     mark = "component", appid = "cadzinho"
-- components are refered to BLOCKs with terminals definition, and have ATTRIBs linked to terminals
-- components have ATTRIBs with individualized information (ID, value, type, etc.)
components = {}

-- node_defs are used to identify a node or net with a label and can define connections between remote wires
-- node_defs are DXF INSERT entities marked with extended data 
--     mark = "node_def", appid = "cadzinho"
--     second ext data = node id (label)
-- node_defs are refered to BLOCKs with one terminal definition
node_defs = {}

-- sweep all entities in drawing
for i, ent in ipairs(cadzinho.get_all()) do
	-- look for extended data with appid "cadzinho"
	ext = cadzinho.get_ext (ent, "cadzinho")
	if #ext > 0 then
		if type(ext[1]) == "string" then
			descript = ext[1]:upper() -- ignore case
			
			-- identify elements
			-- wires
			if descript == "WIRE" then
				-- get points and store in table
				wire = {}
				wire.pts = cadzinho.get_points(ent)
				wires[#wires +1] = wire
				
			--components
			elseif descript == "COMPONENT" then
				comp = {}
				-- get tagged informations
				for _, attr in ipairs(cadzinho.get_attribs(ent)) do
					if type(attr.tag) == "string" then
						if attr.tag == "ID" then
							comp.id = attr.value:upper()
						end
					end
				end
				-- get terminals
				comp.terms = get_term(ent)
				
				comp.blk = cadzinho.get_blk_name (ent)
				pts = cadzinho.get_points(ent)
				comp.ins_pt = pts[1]
				
				-- store in table
				components[#components +1] = comp
			
			-- node labels
			elseif descript == "NODE_DEF" then
				-- get node id
				if type(ext[2]) == "string" then
					node_def = {}
					node_def.id = ext[2]:upper() -- ignore case
					-- get terminal
					node_def.terms = get_term(ent)
					-- store in table
					node_defs[#node_defs +1] = node_def
				end
			end
		end
	end
	
end

cadzinho.db_print (("Wires=%d"):format(#wires))
cadzinho.db_print (("Components=%d"):format(#components))
cadzinho.db_print (("Node_defs=%d\n"):format(#node_defs))

for i, comp in ipairs(components) do
	--cadzinho.db_print (("%d:%s"):format(i, comp.id))
	for t, pts in pairs(comp.terms) do
		--cadzinho.db_print (("    %s-(%.2f,%.2f)"):format(t, pts[1].x, pts[1].y))
	end
end

for i, n_def in ipairs(node_defs) do
	--cadzinho.db_print (("%d:%s"):format(i, n_def.id))
	for t, pts in pairs(n_def.terms) do
		--cadzinho.db_print (("    %s-(%.2f,%.2f)"):format(t, pts[1].x, pts[1].y))
		for _, w in pairs (wires) do
			if test_connect(pts, w.pts) then
				w.id = n_def.id
			end
		end
	end
end



--[[
for _, w in pairs (wires) do
	if type(w.id) == "nil" then
		w.id = ("WIRE%d"):format(j)
		j = j +1
	end
end
--]]

nodes = {}
for _, w in pairs (wires) do
	no_own = true
	if w.owner then
		no_own = false
	else
		w.owner = {}
		w.owner.wires = {w}
	end
	
	if not w.owner.id and type(w.id) == "string" then w.owner.id = w.id end
	
	for _, w2 in pairs (wires) do
		if w ~= w2 and  type(w.id) == "string" and type(w2.id) == "string" and w.id == w2.id then
			if w2.owner then
				if no_own then
					no_own = false
					for i = 1, #w.owner.wires do
						w2.owner.wires[#w2.owner.wires +1] = w.owner.wires[i]
					end
					w.owner = w2.owner
				end
			else
				w.owner.wires[#w.owner.wires +1] = w2
				w2.owner = w.owner
			end
		end
	
		if w ~= w2 and test_connect(w2.pts, w.pts) then
			if w2.owner then
				if no_own then
					no_own = false
					for i = 1, #w.owner.wires do
						w2.owner.wires[#w2.owner.wires +1] = w.owner.wires[i]
					end
					w.owner = w2.owner
					if type(w.id) == "string" and type(w2.owner.id) == "string" and w.id ~= w2.owner.id then
						cadzinho.db_print ("error")
					end
				end
			else
				w.owner.wires[#w.owner.wires +1] = w2
				w2.owner = w.owner
				if type(w.owner.id) == "string" and type(w2.id) == "string" and w.owner.id ~= w2.id then
					cadzinho.db_print ("error")
				end
			end
			if not w.owner.id and type(w2.id) == "string" then w.owner.id = w2.id end
		end
	end
	
	if no_own then
		nodes[#nodes + 1] = w.owner
	end
end

--[[
nodes = {}
for _, w in pairs (wires) do
	no_own = true
	if w.owner then
		no_own = false
	else
		w.owner = {w}
	end
		for _, w2 in pairs (wires) do
			if w ~= w2 and test_connect(w2.pts, w.pts) then
				if w2.owner then
					if no_own then
						no_own = false
						for i = 1, #w.owner do
							w2.owner[#w2.owner +1] = w.owner[i]
						end
						w.owner = w2.owner
					end
				else
					w.owner[#w.owner +1] = w2
					w2.owner = w.owner
				end
			end
		end
	
	if no_own then
		nodes[#nodes + 1] = w.owner
	end
end


for _, w in pairs (wires) do
	cadzinho.db_print (w.id)
end
--]]
cadzinho.db_print (("Nodes=%d\n"):format(#nodes))
j = 1
for i, n in ipairs(nodes) do
	if not n.id then
		n.id = ("NODE%d"):format(j)
		j = j +1
	end
	cadzinho.db_print (("%d:%s"):format(i, n.id))
	for j = 1, #n.wires do
		cadzinho.db_print (("    %s"):format(n.wires[j].id))
	end
end