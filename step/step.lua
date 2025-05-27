local file = io.open("DIM.stp", "r")
local curr = {}
local prev = curr
local data = {}
local level = 0
local stack = {}

local init = false

local hier = {}
local curr_hier = {}

for line in file:lines() do
  local next_char = nil
  
  _, next_char, id = string.find(line, "#(%d+)%s*=%s*", next_char)
  if id then
    init = true
    level = 0
    curr = {}
    data [tonumber(id)] = curr
    next_char = next_char + 1
    
    curr_hier = {}
    hier [tonumber(id)] = curr_hier
    curr_hier.child = {}
    
    _, next_char, lu, func = string.find(line, "([%(]?)%s*([%w_]+)", next_char)
    
    if lu == '(' then
      prev = curr
      curr = {}
      prev[#prev+1] = curr
      stack[level+1] = prev
      level = level + 1
    end
    
    if func then 
      curr.func = func
      next_char = next_char + 1
    end
  end
  
  if init then
    for lu, c, ld in string.gmatch(line, "([,%(%)]?)%s*([%w_#%.%*%-%+']*)%s*([,%(%)]?)", next_char) do
      --print(id, lu, c, ld, level)
      
      if lu == '(' or ld == '(' then
        prev = curr
        curr = {}
        prev[#prev+1] = curr
        stack[level+1] = prev
        level = level + 1
      end
      if level > 0 and string.len(c) > 0 then
        local n = tonumber(c)
        if n then curr[#curr+1] = n
        elseif c == '.F.' then curr[#curr+1] = false
        elseif c == '.T.' then curr[#curr+1] = true
        else curr[#curr+1] = c end
        --print(id, func, c, level)
        
        _, _, n = string.find(c, "#(%d+)")
        if n then 
          curr_hier.child[tonumber(n)] = {pos = #curr, ref=curr}
          
        end
        
      end
      
      
      
      
      if lu == ')' or ld == ')' then 
        level = level - 1
        curr = stack[level+1]
      end
    end
  end
end

file:close()

local serpent = require("serpent")



--for k, t in pairs(data) do
  --print(k, serpent.line(t,{comment = false}))
  --print('[' .. k .. '] = '.. serpent.line(t, {comment = false}) .. ',')
--end


if nil == true then
local del_child = {}

for _, t in pairs(hier) do
  for c, _ in pairs(t.child) do
    t[c] = hier[c]
    del_child[#del_child+1] = c
  end
  t.child = nil
end

for i = 1, #del_child do
  hier[del_child[i]] = nil
end

function dump_write(data, hier, file, level)
  local indent = ''
  for i = 1, level do indent = indent .. "  " end
  for k, t in pairs(hier) do
    local d = data[k].func or 'nil'
    file:write(indent .. '[' .. k .. ']='.. d ..'\n')
    dump_write(data, t, file, level+1)
  end
end

f = io.open("dump_teste_hier.lua", "w+")
dump_write(data, hier, f, 0)


f:close()

end


--local del_child = {}

for k, t in pairs(hier) do
  for kc, c in pairs(t.child) do
    c.ref[c.pos] = data[kc]
    --del_child[#del_child+1] = kc
  end
  --t.child = nil
end

--for i = 1, #del_child do
  --hier[del_child[i]] = nil
  --data[del_child[i]] = nil
--end

local data_len = #data

for k, t in pairs(hier) do
  for kc, c in pairs(t.child) do
    data[kc] = nil
  end
end


function dump_write(data, file, level)
  local indent = ''
  for i = 1, level do indent = indent .. " " end
  
  file:write(indent .. '{')
  if data.func then file:write('func=' .. data.func .. ', ') end
  for i = 1, #data do
    if type (data[i]) == 'table' then
      file:write('\n')
      dump_write(data[i], file, level+1)
    else
      file:write(tostring(data[i]) .. ', ') end
  end
  
  file:write(' },\n' .. indent)
end

f = io.open("dump_teste_hier.lua", "w+")

for i = 1, data_len do
  --print (data[i])
  if data[i] then
    f:write('[' .. i .. '] = ')
    dump_write(data[i], f, 1)
  end
end
--f:write(serpent.dump(data, {comment = false, indent = ' '}))


f:close()

print ('OK')