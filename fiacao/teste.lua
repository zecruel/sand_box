local miniz = require "miniz"
local yxml = require "yxml"

function get_elem(t, id)
	for _, elem in ipairs(t) do
		if elem.id == id then return elem end
	end
end

function get_elems(t, id)
	elems = {}
	for _, elem in ipairs(t) do
		if elem.id == id then
			elems[#elems + 1] = elem
		end
	end
	return elems
end

function letter2num(str)
	sum = 0
	local idx = #str
	local n = #str
	while idx > 0 do
		val = string.byte(str:upper(), idx) - 64
		sum = sum + val * math.floor(26^(n-idx))
		idx = idx -1
	end
	return sum
end

function get_sheet (file, parser, wb, name)
	if type(wb.sheets[name]) == "table" then
		local s_str = file:read(("xl/worksheets/sheet%d.xml"):format(wb.sheets[name].id))
		local s_xml = parser:read(s_str)
		
		local dim = get_elem(s_xml, "dimension")
		local data = get_elem(s_xml, "sheetData")
		local rows = get_elems(data, "row")
		
		local sheet = {}
		if type(dim) == "table" then
			local col_start, row_start, col_end, row_end = dim.attr.ref:match('(%a+)(%d+):(%a+)(%d+)')
			sheet.dim = {rows={row_start, row_end}, cols={col_start, col_end}}
		end
		
		for i, r in ipairs(rows) do
			r_idx = tonumber(r.attr.r)
			row = {}
			cells = get_elems(r, "c")
			for j, c in ipairs(cells) do
				c_ref = c.attr.r:match('%a+')
				value = get_elem(c, "v")
				if type(value) == "table" then 
					if c.attr.t == "s" then
						--print(wb.ss[tonumber(value.cont)+1])
						row[c_ref] = wb.ss[tonumber(value.cont)+1]
					else
						--print(value.cont)
						row[c_ref] = value.cont
					end
				end
			end
			sheet[r_idx] = row
		end
		
		
		return sheet
	end
end


function print_xml(t, n)
	print (("%s%s"):format(n, t.id))
	if t.cont then print (("  %s%s"):format(n, t.cont)) end
	if t.attr then
		for key, value in pairs(t.attr) do
			print ((" %s%s=%s"):format(n, key, value))
			
		end
	end

	for key, value in ipairs(t) do
		print_xml (value, n.."  ")
		
	end
end


zip = miniz.open("demo.xlsx")
--zip = miniz.open("ESCOPO1.xlsm")

wb = zip:read("xl/workbook.xml")
ss = zip:read("xl/sharedStrings.xml")
--sty = zip:read("xl/styles.xml")


--print (s)

parser = yxml.new()

--[[
sheet = zip:read("xl/worksheets/sheet1.xml")
t = parser:read(sheet)
print_xml (t, "")
print("")
--]]

workbook = {}
workbook.sheets = {}
workbook.ss = {}

wb_t = parser:read(wb)
sheets = get_elem(wb_t, "sheets")
--print_xml (sheets, "")
sheets_t = get_elems(sheets, "sheet")
for _, sheet in ipairs(sheets_t) do
	if type(sheet.attr) == "table" then 
		workbook.sheets[sheet.attr.name] = {}
		workbook.sheets[sheet.attr.name].id = tonumber(sheet.attr.sheetId)
	end
end

ss_t = parser:read(ss)
ssi = get_elems(ss_t, "si")

for i, s in ipairs(ssi) do
	str = get_elem(s, "t")
	if type(str) == "table" then 
		workbook.ss[i] = str.cont
	end
end

for key, sheet in pairs(workbook.sheets) do
	print (("%s = %s"):format(key, sheet.id))
	
	--print(("xl/worksheets/sheet%s.xml"):format(sheet.id))
	
	--s_str = zip:read(("xl/worksheets/sheet%s.xml"):format(sheet.id))
	--s_xml = parser:read(s_str)
	s_xml = get_sheet (zip, parser, workbook, key)
	for r_i, row in pairs(s_xml) do
		if type(row) == "table" then
			for c_i, cell in pairs (row) do
				print (c_i, cell)
			end
		else
			print (r_i, row)
		end
	end
	
	
	--print_xml (s_xml, "")
end

print(letter2num('umx'))

--[[
for key, ss in pairs(workbook.ss) do
	print (("%s = %s"):format(key, ss))
end
--]]

zip:close()
parser:close()