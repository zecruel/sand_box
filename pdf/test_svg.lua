local s = ""
--local file = "C:\\util\\CadZinho-opengl\\script\\teste.svg"
--local file = "C:\\util\\CadZinho-opengl\\samples\\da.svg"
local file = "C:\\util\\teste.svg"

--[[f = io.open(file, 'r')
s = f:read('*a')
f:close()]]--

for s in io.lines(file, 'l') do
  com = s:match('<(%a+)')
  if com == 'rect' then
    x = s:match(' x="([^"]+)"')
    y = s:match(' y="([^"]+)"')
    w = s:match(' width="([^"]+)"')
    h = s:match(' height="([^"]+)"')
    rx = s:match(' rx="([^"]+)"')
    ry = s:match(' ry="([^"]+)"')
    
    cadzinho.db_print(com, x, y, w, h, rx, ry)
  elseif com == 'circle' then
    x = s:match(' cx="([^"]+)"')
    y = s:match(' cy="([^"]+)"')
    r = s:match(' r="([^"]+)"')
    
    circle = cadzinho.new_circle(tonumber(x),-tonumber(y),tonumber(r))
    circle:write()
    
    cadzinho.db_print(com, x, y, r)
  elseif com == 'ellipse' then
    x = s:match(' cx="([^"]+)"')
    y = s:match(' cy="([^"]+)"')
    rx = s:match(' rx="([^"]+)"')
    ry = s:match(' ry="([^"]+)"')
    
    cadzinho.db_print(com, x, y, rx, ry)
  elseif com == 'line' then
    x1 = s:match(' x1="([^"]+)"')
    y1 = s:match(' y1="([^"]+)"')
    x2 = s:match(' x2="([^"]+)"')
    y2 = s:match(' y2="([^"]+)"')
    
    cadzinho.db_print(com, x1, y1, x2, y2)
  elseif com == 'polyline' then
    points = s:match(' points="([^"]+)"')
    
    cadzinho.db_print(com)
  elseif com == 'polygon' then
    points = s:match(' points="([^"]+)"')
    
    cadzinho.db_print(com, points)
  elseif com == 'path' then
    d = s:match(' d="([^"]+)"')
    if d then
      local modal = nil
      local coords = {}
      local base_x = 0
      local base_y = 0
      local pline = nil
      d = d:gsub("([^%s,;])([%a])", "%1 %2"); -- Convert "100D" to "100 D"
      d = d:gsub("([%a])([^%s,;])", "%1 %2"); -- Convert "D100" to "D 100"
      for ds in d:gmatch("([^%s,;]+)") do
        if tonumber(ds) then
          coords[#coords+1] = tonumber(ds)
        else
          if modal == 'm' then
            if pline then pline:write() end
            pline = nil
            for i, v in ipairs(coords) do
              if i % 2 == 1 then
                base_x = base_x + v
              else
                base_y = base_y - v
              end
            end
          elseif modal == 'M' then
            if pline then pline:write() end
            pline = nil
            for i, v in ipairs(coords) do
              if i % 2 == 1 then
                base_x = v
              else
                base_y = -v
              end
            end
          elseif modal == 'l' then
            local x = 0
            local y = 0
            for i, v in ipairs(coords) do
              if i % 2 == 1 then
                x = base_x + v
              else
                y = base_y - v
                if pline then
                  cadzinho.pline_append(pline, x, y, 0)
                else
                  pline = cadzinho.new_pline(base_x, base_y, 0, x, y, 0)
                end
                
                base_x = x
                base_y = y
              end
            end
          elseif modal == 'L' then
            local x = 0
            local y = 0
            for i, v in ipairs(coords) do
              if i % 2 == 1 then
                x = v
              else
                y = -v
                if pline then
                  cadzinho.pline_append(pline, x, y, 0)
                else
                  pline = cadzinho.new_pline(base_x, base_y, 0, x, y, 0)
                end
                
                base_x = x
                base_y = y
              end
            end
          elseif modal == 'h' then
            for i, v in ipairs(coords) do
              x = base_x + v
              if pline then
                cadzinho.pline_append(pline, x, base_y, 0)
              else
                pline = cadzinho.new_pline(base_x, base_y, 0, x, base_y, 0)
              end
              base_x = x
            end
          elseif modal == 'H' then
            for i, v in ipairs(coords) do
              x = v
              if pline then
                cadzinho.pline_append(pline, x, base_y, 0)
              else
                pline = cadzinho.new_pline(base_x, base_y, 0, x, base_y, 0)
              end
              base_x = x
            end
          elseif modal == 'v' then
            for i, v in ipairs(coords) do
              y = base_y - v
              if pline then
                cadzinho.pline_append(pline, base_x, y, 0)
              else
                pline = cadzinho.new_pline(base_x, base_y, 0, base_x, y, 0)
              end
              base_y = y
            end
          elseif modal == 'V' then
            for i, v in ipairs(coords) do
              y = -v
              if pline then
                cadzinho.pline_append(pline, base_x, y, 0)
              else
                pline = cadzinho.new_pline(base_x, base_y, 0, base_x, y, 0)
              end
              base_y = y
            end
          elseif modal == 'z' then
            if pline then cadzinho.pline_close(pline, true) end
          elseif modal == 'Z' then
            if pline then cadzinho.pline_close(pline, true) end
          elseif modal == 'c' then
            modal = ds
          elseif modal == 'C' then
            modal = ds
          elseif modal == 's' then
            modal = ds
          elseif modal == 'S' then
            modal = ds
          elseif modal == 'q' then
            modal = ds
          elseif modal == 'Q' then
            modal = ds
          elseif modal == 't' then
            modal = ds
          elseif modal == 'T' then
            modal = ds
          elseif modal == 'a' then
            modal = ds
          elseif modal == 'A' then
            modal = ds
          end
          modal = ds
          coords = {}
        end
      end
      if modal == 'm' then
        if pline then pline:write() end
        pline = nil
        for i, v in ipairs(coords) do
          if i % 2 == 1 then
            base_x = base_x + v
          else
            base_y = base_y - v
          end
        end
      elseif modal == 'M' then
        if pline then pline:write() end
        pline = nil
        for i, v in ipairs(coords) do
          if i % 2 == 1 then
            base_x = v
          else
            base_y = -v
          end
        end
      elseif modal == 'l' then
        local x = 0
        local y = 0
        for i, v in ipairs(coords) do
          if i % 2 == 1 then
            x = base_x + v
          else
            y = base_y - v
            if pline then
              cadzinho.pline_append(pline, x, y, 0)
            else
              pline = cadzinho.new_pline(base_x, base_y, 0, x, y, 0)
            end
            
            base_x = x
            base_y = y
          end
        end
      elseif modal == 'L' then
        local x = 0
        local y = 0
        for i, v in ipairs(coords) do
          if i % 2 == 1 then
            x = v
          else
            y = -v
            if pline then
              cadzinho.pline_append(pline, x, y, 0)
            else
              pline = cadzinho.new_pline(base_x, base_y, 0, x, y, 0)
            end
            
            base_x = x
            base_y = y
          end
        end
      elseif modal == 'h' then
        for i, v in ipairs(coords) do
          x = base_x + v
          if pline then
            cadzinho.pline_append(pline, x, base_y, 0)
          else
            pline = cadzinho.new_pline(base_x, base_y, 0, x, base_y, 0)
          end
          base_x = x
        end
      elseif modal == 'H' then
        for i, v in ipairs(coords) do
          x = v
          if pline then
            cadzinho.pline_append(pline, x, base_y, 0)
          else
            pline = cadzinho.new_pline(base_x, base_y, 0, x, base_y, 0)
          end
          base_x = x
        end
      elseif modal == 'v' then
        for i, v in ipairs(coords) do
          y = base_y - v
          if pline then
            cadzinho.pline_append(pline, base_x, y, 0)
          else
            pline = cadzinho.new_pline(base_x, base_y, 0, base_x, y, 0)
          end
          base_y = y
        end
      elseif modal == 'V' then
        for i, v in ipairs(coords) do
          y = -v
          if pline then
            cadzinho.pline_append(pline, base_x, y, 0)
          else
            pline = cadzinho.new_pline(base_x, base_y, 0, base_x, y, 0)
          end
          base_y = y
        end
      elseif modal == 'z' then
        if pline then cadzinho.pline_close(pline, true) end
      elseif modal == 'Z' then
        if pline then cadzinho.pline_close(pline, true) end
      elseif modal == 'c' then
        modal = ds
      elseif modal == 'C' then
        modal = ds
      elseif modal == 's' then
        modal = ds
      elseif modal == 'S' then
        modal = ds
      elseif modal == 'q' then
        modal = ds
      elseif modal == 'Q' then
        modal = ds
      elseif modal == 't' then
        modal = ds
      elseif modal == 'T' then
        modal = ds
      elseif modal == 'a' then
        modal = ds
      elseif modal == 'A' then
        modal = ds
      end
      if pline then pline:write() end
      pline = nil
    end
    
    --cadzinho.db_print(com, d)
    
  elseif com == 'text' then
    x = s:match(' x="([^"]+)"')
    y = s:match(' y="([^"]+)"')
    
    text = s:match('>(.*)</text>')
    
    --cadzinho.db_print(com, x, y, text)
  end
end
