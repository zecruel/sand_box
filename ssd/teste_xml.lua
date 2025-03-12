function serialize(tbl)
    local result = {}
    local stack = {{tbl, "root"}}
    local visited = {}

    while #stack > 0 do
        local current = table.remove(stack)
        local t = current[1]
        local path = current[2]

        if visited[t] then
            table.insert(result, string.format("%s = %s", path, visited[t]))
        else
            visited[t] = path
            table.insert(result, string.format("%s = {}", path))

            for k, v in pairs(t) do
                local key = type(k) == "string" and string.format("%q", k) or tostring(k)
                local newPath = string.format("%s[%s]", path, key)

                if type(v) == "table" then
                    table.insert(stack, {v, newPath})
                else
                    local value = type(v) == "string" and string.format("%q", v) or tostring(v)
                    table.insert(result, string.format("%s = %s", newPath, value))
                end
            end
        end
    end

    return table.concat(result, "\n")
end


function xml_write(xml_t, file, level)
  local indent = ''
  local multi = false
  for i = 1, level do indent = indent .. "  " end
  file:write(indent .. "<" .. xml_t.id)
  if xml_t.attr then
    for attr, value in pairs(xml_t.attr) do
      file:write(" " .. attr .. '="' .. value ..'"')
    end
  end
  if #xml_t > 0 or xml_t.cont then
    file:write(">")
    multi = true
  end
  if #xml_t > 0 then file:write("\n") 
  else indent = "" end
  for i = 1, #xml_t do
    xml_write(xml_t[i], file, level + 1)
  end
  --if #xml_t > 0 then file:write("\n") end
  
  if xml_t.cont then file:write(xml_t.cont) end
  if multi then
    file:write(indent .. "</" .. xml_t.id .. ">\n")
  else
    file:write(" />\n")
  end
end


local serpent = require("serpent")
local parser = yxml.new()

local f = io.open("teste_timoteo.cid", "r")
local xml_str = f:read("*a")
f:close()

local xml_t = parser:read(xml_str) -- convert xml string to Lua table

f = io.open("dump_teste.lua", "w+")
f:write(serpent.dump(xml_t,  {nocode = true, indent = ' '}))
--f:write(serialize(xml_t))
f:close()

f = io.open("dump_teste.xml", "w+")
  f:write('<?xml version="1.0" encoding="utf-8"?>\n')
  xml_write(xml_t, f, 0)
f:close()

parser:close()