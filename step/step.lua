local file = io.open("DIM.stp", "r")
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
      -- print(id, func, c, level)
    end
    
    
    
    
    if ld == ')' then 
      level = level - 1
      curr = prev
    end
  end
  
  if level > 0 then
  
  end
end

file:close()

local serpent = require("serpent")



for k, t in pairs(data) do
  --print(k, serpent.line(t,{comment = false}))
  print('[' .. k .. '] = '.. serpent.line(t, {comment = false}) .. ',')
end
print ('OK')