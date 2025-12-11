local function read_header(file)
  
  -- Read the header
  local data = file:read(4)
  if not data or string.unpack("<c4", data) ~= 'LASF' then
      return "Not valid LAS file"
  end
  file:seek("cur", 20)
  data = file:read(2)
  if not data then return "Not valid LAS file" end
  local major, minor = string.unpack("<BB", data)
  file:seek("cur", 68)
  data = file:read(6)
  if not data then return "Not valid LAS file" end
  local h_sz, of_data = string.unpack("<HL", data)
  file:seek("cur", 4)
  data = file:read(3)
  if not data then return "Not valid LAS file" end
  local format, len = string.unpack("<BH", data)
  file:seek("cur", 24)
  data = file:read(24)
  if not data then return "Not valid LAS file" end
  local sx, sy, sz = string.unpack("<ddd", data)
  data = file:read(24)
  if not data then return "Not valid LAS file" end
  local ox, oy, oz = string.unpack("<ddd", data)
  data = file:read(16)
  if not data then return "Not valid LAS file" end
  local max_x, min_x = string.unpack("<dd", data)
  data = file:read(16)
  if not data then return "Not valid LAS file" end
  local max_y, min_y = string.unpack("<dd", data)
  data = file:read(16)
  if not data then return "Not valid LAS file" end
  local max_z, min_z = string.unpack("<dd", data)

  -- Remove trailing nulls from name
  --name = name:gsub("%z+$", "")

  return {
      major = major,
      minor = minor,
      h_sz = h_sz,
      of_data = of_data,
      format = format, len = len,
      scale = {x = sx, y = sy, z = sz},
      of = {x = ox, y = oy, z = oz},
      max = {x = max_x, y = max_y, z = max_z},
      min = {x = min_x, y = min_y, z = min_z}
  }
  
end

local function read_point(file)
  
  -- Read the point
  local data = file:read(14)
  if not data then return nil end
  local x, y, z, i = string.unpack("<lllH", data)
  file:seek("cur", 14)
  data = file:read(6)
  if not data then return nil end
  local r, g, b = string.unpack("<HHH", data)

  return {
    x = x, y = y, z = z, i = i , r = r, g = g, b = b
  }
  
end


local file_path = "C:\\Users\\c055897\\OneDrive - CEMIG\\bim\\nuvem\\20251202103028494.las"
--local file_path = "C:\\Users\\c055897\\OneDrive - CEMIG\\bim\\nuvem\\20251202093149425.las"

local file, err = io.open(file_path, "rb")
if not file then
  print ("Failed to open file: " .. tostring(err))
  
else

  local head = read_header(file)
  if type(head) == 'string' then print (head)
  elseif type(head) == 'table' then
    local response = "Version=" .. head.major .. "." .. head.minor .. "\n" ..
    "header size=" .. head.h_sz .. "\n" ..
    "Data of=" .. head.of_data .. "\n" ..
    "Format=" .. head.format .. " len=" .. head.len .. "\n" ..
    "Scale=" .. head.scale.x .. " , " .. head.scale.y .. " , " .. head.scale.z .. "\n" ..
    "of=" .. head.of.x .. " , " .. head.of.y .. " , " .. head.of.z .. "\n" ..
    "max=" .. head.max.x .. " , " .. head.max.y .. " , " .. head.max.z .. "\n" ..
    "min=" .. head.min.x .. " , " .. head.min.y .. " , " .. head.min.z .. "\n"
    print (response)
  
  
    file:seek("set", head.of_data)
    
    cadzinho.set_timeout(120)
    
    for i=1,100000000 do 
      local pt = read_point (file)
      if type(pt) ~= 'table' then break end
      local x1 = pt.x * head.scale.x
      local y1 = pt.y * head.scale.y
      local z1 = pt.z * head.scale.z
      
      if z1 > -19 then
        local p = cadzinho.new_point(x1, y1, z1)
        if p then p:write() end
      end
      --down sampling
      file:seek("cur", 50*head.len)
      
      --local response = "( " .. pt.x * head.scale.x .. " , " ..
      --  pt.y * head.scale.y .. " , " ..
      --  pt.z * head.scale.z .. " )"
      --print (response)
    end
    
  end
end



