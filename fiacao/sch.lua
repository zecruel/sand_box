wires = {}
components = {}
node_defs = {}

for i, ent in ipairs(cadzinho.get_all()) do
	ext = cadzinho.get_ext (ent, "cadzinho")
	if #ext > 0 then
		if type(ext[1] == "string") then
			descript = ext[1]:upper()
			if descript == "WIRE" then
				wires[#wires +1] = cadzinho.get_points(ent)
			elseif descript == "COMPONENT" then
				comp = {}
				for _, attr in ipairs(cadzinho.get_attribs(ent)) do
					if type(attr.tag == "string") then
						if attr.tag == "ID" then
							comp.id = attr.value
						end
					end
				end
				
				comp.blk = cadzinho.get_blk_name (ent)
				pts = cadzinho.get_points(ent)
				comp.ins_pt = pts[1]
				components[#components +1] = comp
			elseif descript == "NODE_DEF" then
				node_defs[#node_defs +1] = cadzinho.get_points(ent)
			end
		end
	end
	
end

cadzinho.db_print (("Wires=%d"):format(#wires))
cadzinho.db_print (("Components=%d"):format(#components))
cadzinho.db_print (("Node_defs=%d\n"):format(#node_defs))

for i, comp in ipairs(components) do
	cadzinho.db_print (("%d:%s, (%f,%f)"):format(i, comp.id, comp.ins_pt.x, comp.ins_pt.y))
end