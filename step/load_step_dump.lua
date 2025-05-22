dofile('dump_teste.lua')
print(step, 'OK')


local funcs = {}
for k, t in pairs(step) do
  if t.func then
    if not funcs[t.func] then
      funcs[t.func] = {use = 1, }--args = #t[1]}
    else
      funcs[t.func].use = funcs[t.func].use + 1
    end
  else
    print ("failed= " .. k) 
  end
end

for k, t in pairs(funcs) do
  print(k, t.use, t.args)
end