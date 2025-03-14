-- Write a XML structure in a string
function xml_write_rec(xml_t, level) -- Recursive function (lazy...)
  local indent = ''
  local multi = false -- flag to explicit closing tag
  local str = ""
  
  -- indentation, according level in structure
  for i = 1, level do indent = indent .. "  " end
  
  -- init tag
  str = str .. indent .. "<" .. xml_t.id
  
  -- write attributes
  if xml_t.attr then
    for attr, value in pairs(xml_t.attr) do
      str = str .. " " .. attr .. '="' .. value ..'"'
    end
  end
  
  if #xml_t > 0 or xml_t.cont then
    -- needs a explicit closing tag
    str = str .. ">"
    multi = true
  end
  
  -- if exists sub elements, then write in a new line
  if #xml_t > 0 then str = str .. "\n"
  else indent = "" end -- write in same line without indentation
  
  -- write sub elements, recursively
  for i = 1, #xml_t do
    str = str .. xml_write_rec(xml_t[i], level + 1) -- up the level
  end
  
  -- write text contents
  if xml_t.cont then str = str .. xml_t.cont end
  
  -- close tag
  if multi then
    str = str .. indent .. "</" .. xml_t.id .. ">\n" -- explicit close
  else
    str = str .. " />\n" -- self close
  end
  
  return str
end

function xml_write(xml_t)
  local str = '<?xml version="1.0" encoding="utf-8"?>\n'
  str = str .. xml_write_rec(xml_t, 0)
  return str
end

bd = sqlite.open('61850_cemig.db')

local tipos_enum = {}
for linha in bd:cols('SELECT * FROM enums ORDER BY enum_id, ord') do
  if not tipos_enum[linha.enum_id] then tipos_enum[linha.enum_id] = {} end
  local t_enum = tipos_enum[linha.enum_id]
  t_enum[linha.ord] = {
    valor = linha.valor, descr = linha.descr, obs = linha.obs}
end

local tipos_da = {}
for linha in bd:cols('SELECT * FROM structs ORDER BY d_a') do
  if not tipos_da[linha.d_a] then tipos_da[linha.d_a] = {} end
  local t_da = tipos_da[linha.d_a]
  t_da[linha.bda] = {
    t_bas = linha.t_bas,
    tipo = linha.tipo, descr = linha.descr, obs = linha.obs}
end

local tipos_do = {}
for linha in bd:cols('SELECT * FROM do_types ORDER BY d_o') do
  if not tipos_do[linha.d_o] then tipos_do[linha.d_o] = {[1] = linha.classe} end
  local t_do = tipos_do[linha.d_o]
  t_do[linha.d_a] = {
    t_bas = linha.t_bas, fc = linha.fc, trg = linha.trg,
    tipo = linha.tipo, descr = linha.descr, obs = linha.obs}
end

local tipos_ln = {}
for linha in bd:cols('SELECT * FROM log_nodes ORDER BY ln') do
  if not tipos_ln[linha.ln] then tipos_ln[linha.ln] = {} end
  local t_ln = tipos_ln[linha.ln]
  t_ln[linha.d_o] = {tipo = linha.tipo, descr = linha.descr, obs = linha.obs}
end


bd:close()














local i = 1

local ssd = {id = 'SCL', attr = {
  xmlns="http://www.iec.ch/61850/2003/SCL",
  ["xmlns:sxy"] = "http://www.iec.ch/61850/2003/SCLcoordinates",
  version = "2007", revision = "B", release = "4"
  }}

local history = {id = 'History'}
history[1] = {id = 'Hitem', attr = {
  version="1",
  revision="A",
  when="2025-03-14",
  who="Zecruel",
  what="Versão inaugural do arquivo SSD",
  why="Padronização de SEs digitais na Cemig GT"
  }}
  
ssd[#ssd+1] = {id = 'Header', attr = {
  id = "SSD_SE_teste",
  version="1",
  revision="A",
  toolID="CadZinho-PELICAnU",
  nameStructure="IEDName",
  },
  [1] = history
}

local setor_500 = {id = 'VoltageLevel', attr = {
  name="U",
  ["sxy:x"] = "0",["sxy:y"] = "0",
  desc="Setor de 500kV - Arranjo disjuntor e meio"},
  [1] = {id = 'Voltage', attr = {unit="V", multiplier="k"},
  cont = '500'
  }
}
vao_BU1 =  {id ='Bay', attr = {
  name="BU1", ['sxy:x']="0",['sxy:y']="22", desc="Barra 1 500kV"},
  
  [1] = {id = 'ConnectivityNode', attr = {
    name="BU1", pathName="SE_Teste/U/BU1/BU1", -- SE/setor/vao/nó
    ['sxy:x']="0", ['sxy:y']="0", ['sxy:dir']="horizontal" }
  }
}
setor_500[#setor_500] = vao_BU1

ssd[#ssd+1] = {id = 'Substation', attr = {
  name = "SE_Teste",
  desc = "Teste de criação de SSD"
  },
  
  [1] = setor_500
}

--ssd[#ssd+1] = {id = 'Communication'}

--ssd[#ssd+1] = {id = 'IED', attr = { name = ""}}

dt = {id = 'DataTypeTemplates'}

for id, t in pairs(tipos_ln) do
  local ln = {id = 'LNodeType', attr = {id = id .. '_CEMIG', lnClass = id}}
  i = 1
  for id2, t2 in pairs(t) do
    ln[i] = {id = 'DO', attr = {name = id2,
      desc = t2.descr, type = t2.tipo}
    }
    i = i + 1
  end
  dt[#dt+1] = ln
end

for id, t in pairs(tipos_do) do
  local d_o = {id = 'DOType', attr = {id = id, cdc = t[1]}}
  i = 1
  for id2, t2 in pairs(t) do
    if id2 ~= 1 then
      local tipo = 'DA'
      if not t2.t_bas then tipo = 'SDO' end
      d_o[i] = {id = tipo, attr = {name = id2, fc = t2.fc, desc = t2.descr,
        bType = t2.t_bas, type = t2.tipo}
      }
      if type(t2.trg) == 'string' then
        if string.find(t2.trg, 'qchg') then d_o[i].attr.qchg = 'true' end
        if string.find(t2.trg, 'dchg') then d_o[i].attr.dchg = 'true' end
        if string.find(t2.trg, 'dupd') then d_o[i].attr.dupd = 'true' end
      end
      i = i + 1
    end
  end
  dt[#dt+1] = d_o
end
for id, t in pairs(tipos_da) do
  local d_a = {id = 'DAType', attr = {id = id}}
  i = 1
  for id2, t2 in pairs(t) do
    d_a[i] = {id = 'BDA', attr = {name = id2,
      bType = t2.t_bas, type = t2.tipo}
    }
    i = i + 1
  end
  dt[#dt+1] = d_a
end
for id, t in pairs(tipos_enum) do
  local enum = {id = 'EnumType', attr = {id = id}}
  for i = 1, #t do
    enum[i] = {id = 'EnumVal', attr = {ord = i}, cont = t[i].valor}
  end
  dt[#dt+1] = enum
end

ssd[#ssd+1] = dt

f = io.open("ssd_teste.xml", "w+")
  f:write(xml_write(ssd))
f:close()

