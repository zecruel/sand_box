local file = io.open("0538-4_latch_3d.stp", "r")
local curr = {}
local prev = curr
local data = {}
local level = 0

for line in file:lines() do
  local next_char = nil
  
  _, next_char, id = string.find(line, "#(%d+)%s*=%s*", next_char)
  if id then 
    level = 0
    curr = {}
    next_char = next_char + 1
    
    _, next_char, func = string.find(line, "([%w_]+)", next_char)
    
    data [tonumber(id)] = curr
    if func then 
      curr.func = func
      next_char = next_char + 1
    end
  end
  
  for lu, c, ld in string.gmatch(line, "([,%(]?)%s*([%w_#%.%*%-%+']+)%s*([,%)]?)", next_char) do
    if lu == '(' then
      prev = curr
      curr = {}
      prev[#prev+1] = curr
      level = level + 1
    end
    if level > 0 then
      local n = tonumber(c)
      if n then curr[#curr+1] = n
      else curr[#curr+1] = c end
    end
    
    if ld == ')' then 
      level = level - 1
      curr = prev
    end
  end
end

file:close()

local serpent = require("serpent")

f = io.open("dump_teste.lua", "w+")

f:write('step = {\n')

for k, t in pairs(data) do
  --print(k, serpent.line(t,{comment = false}))
  f:write('  [' .. k .. '] = '.. serpent.line(t, {comment = false}) .. ',\n')
end
f:write('}\n')

f:close()
print ('OK')