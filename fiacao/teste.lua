local miniz = require "miniz"
local yxml = require "yxml"

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

s = zip:read("xl/worksheets/sheet1.xml")

--print (s)

parser = yxml.new()

t = parser:read(s)

print_xml (t, "")

zip:close()
parser:close()