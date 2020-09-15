local miniz = require "miniz"
local yxml = require "yxml"

-- Get first element  in table that match the id key
function get_elem(t, id)
	for _, elem in ipairs(t) do -- sweep elements in table (only numbered keys)
		if elem.id == id then return elem end -- check if match and return
	end
end

-- Get all elements  in table that match the id key. Return an array.
function get_elems(t, id)
	local elems = {} -- array with match elements
	for _, elem in ipairs(t) do  --sweep elements in table (only numbered keys)
		if elem.id == id then  --check if match
			elems[#elems + 1] = elem --append to array
		end
	end
	return elems
end

--Auxiliary function to convert letters reference (as in xlsx columns) in numeric
function letter2num(str)
	local sum = 0
	local idx = #str -- current character index in string
	local n = #str -- string length
	-- get each character in string, in reverse order
	while idx > 0 do
		val = string.byte(str:upper(), idx) - 64 -- subtract 64 to make 'A' -> 1
		-- multiply character value to relative positional value and add to result
		sum = sum + val * math.floor(26^(n-idx)) -- base 26 -> alphabet letters
		idx = idx -1 -- update index in reverse order
	end
	return sum
end

--Auxiliary function to convert numeric reference in letters (as in xlsx columns)
function num2letter(num)
	local t = {}
	
	-- get each 'algarism' by sucessive division
	local div = num
	repeat
		-- base 26 -> alphabet letters
		rem = div % 26
		div = div // 26
		-- store each 'algarism' in a array
		t[#t + 1] = rem -- 'algarism' is remainder
	until div <= 0
	
	local str = "" -- string to return
	
	-- convert array of 'algarisms' in a string (reverse order)
	local idx = #t
	while idx > 0 do
		str = str .. string.char(t[idx] + 64) -- add 64 to make 1 -> 'A'
		idx = idx -1 -- update index in reverse order
	end
	
	return str
end

-- Get XLSX sheet data. Arguments:
--    - file = xlsx file buffer, to read zipped data
--    - parser = xml parser object
--    - wb = pre-parsed xlsx Workbook, with shared strings
--    - name = string with name of the sheet, to locate in workbook
function get_sheet (file, parser, wb, name)
	-- try to locate sheet name in workbook
	if type(wb.sheets[name]) ~= "table" then return {} end -- fail
	
	-- get sheet index (in wb.sheets[name].id) and open relative file to buffer
	local s_str = file:read(("xl/worksheets/sheet%d.xml"):format(wb.sheets[name].id))
	local s_xml = parser:read(s_str) -- parse xml file data
	
	-- get dimension and sheetdata from root element
	local dim = get_elem(s_xml, "dimension")
	local data = get_elem(s_xml, "sheetData")
	-- get rows from sheetdata element
	local rows = get_elems(data, "row")
	
	local sheet = {} -- main table
	
	-- get sheet matrix dimension
	if type(dim) == "table" then
		-- parse expression like "A1:C5" to get rows and columns range
		local col_start, row_start, col_end, row_end = dim.attr.ref:match('(%a+)(%d+):(%a+)(%d+)')
		
		-- assemble columns identification array. Columns are identified by letters.
		columns = {}
		for col = letter2num(col_start), letter2num(col_end) do -- use auxiliary functions to convert letters to numbers
			columns[#columns + 1] = num2letter(col) -- and vice-versa
		end
		
		-- assemble rows identification array (numeric identification)
		row_s = {}
		for row = row_start, row_end do
			row_s[#row_s + 1] = row
		end
		
		-- store dimension information, with 'dim' key
		sheet.dim = {rows=row_s, cols=columns}
	end
	
	-- get sheet data -> iterate over rows
	for i, r in ipairs(rows) do
		r_idx = tonumber(r.attr.r) -- current row id (numeric)
		row = {} -- table to store current row data
		cells = get_elems(r, "c") -- look for 'c' elements in row, for cells data
		for j, c in ipairs(cells) do
			c_ref = c.attr.r:match('%a+') -- current column id (letter)
			value = get_elem(c, "v") -- look for 'v' element in cell, for cell's value
			if type(value) == "table" then 
				if c.attr.t == "s" then -- verify if value is a shared string
					-- value content is the index to look shared string in workbook
					row[c_ref] = wb.ss[tonumber(value.cont)+1]
				else
					-- regular value in content
					row[c_ref] = value.cont
				end
			end
		end
		-- store row in main table
		sheet[r_idx] = row
	end
	
	-- return main table
	return sheet
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

parser = yxml.new()

--zip = miniz.open("demo.xlsx")
zip = miniz.open("ESCOPO1.xlsm")


wb = zip:read("xl/workbook.xml")
ss = zip:read("xl/sharedStrings.xml")
--sty = zip:read("xl/styles.xml")

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
		local id = sheet.attr["r:id"]
		id = id:match('rId(%d+)')
		--workbook.sheets[sheet.attr.name].id = tonumber(sheet.attr.sheetId)
		workbook.sheets[sheet.attr.name].id = tonumber(id)
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
	--[[
	for r_i, row in ipairs(s_xml) do
		if type(row) == "table" then
			for c_i, cell in pairs (row) do
				print (c_i, cell)
			end
		else
			print (r_i, row)
		end
	end
	
	for i, col in ipairs(s_xml.dim.cols) do
		print (("%s = %s"):format(i, col))
	end
	--]]
	
	--print_xml (s_xml, "")
	for _, r in ipairs(s_xml.dim.rows) do
		str = ""
		for _, c in ipairs(s_xml.dim.cols) do
			str = str .. tostring(s_xml[r][c]) .. "\t"
		end
		print (str)
	end
	
end

--print(letter2num('umx'))
--print(num2letter(14558))

--[[
for key, ss in pairs(workbook.ss) do
	print (("%s = %s"):format(key, ss))
end
--]]

zip:close()
parser:close()