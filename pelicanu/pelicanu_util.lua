-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8

function exists(name)
    if type(name)~="string" then return false end
    return os.rename(name,name) and true or false
end

function isFile(name)
    if type(name)~="string" then return false end
    if not exists(name) then return false end
    local f = io.open(name)
    if f then
      f:close()
      return true
    end
    return false
end

function isDir(name)
    return (exists(name) and not isFile(name))
end

function nome_arq(url)
  return url:match("(.+)%..+$")
end

function extensao(url)
  return url:match("^.+(%..+)$")
end

function diretorio(url)
  url = string.gsub(url, "(.[^"..fs.dir_sep.."]+)$", "") .. fs.dir_sep
  return url
end

function format_dir(url)
  url = string.gsub(url, "(.[^"..fs.dir_sep.."])$", '%1' .. fs.dir_sep)
  return url
end

function cria_pasta(caminho)
   if (os.getenv("oS") or ""):match("^Windows") then
      return os.execute('mkdir "' .. caminho .. '" 1>nul: 2>&1')
   else
      return os.execute('mkdir -p "' .. caminho .. '" 2>/dev/null')
   end
end

function copia_arq(origem, destino)
   if (os.getenv("oS") or ""):match("^Windows") then
      return os.execute('copy "' .. origem .. '" "' .. destino .. '"')
   else
      return os.execute('cp "' .. origem .. '" "' .. destino .. '"')
   end
   return exists(destino)
end

function carrega_config(arq)
  local configEnv = {} -- to keep it separate from the global env
  local f,err = loadfile(arq, "t", configEnv)
  if f then
    f() -- run the chunk
    -- now configEnv should contain your data
    return configEnv
  else
    return nil, err
  end
end

--Implementação de filas em Lua. 
-- Adaptado de: https://www.tutorialspoint.com/how-to-implement-a-queue-in-lua-programming
function nova_fila()
  -- variáveis internas
  local self = {
    prim = 1,
    ult = 0,
    dados = {}
  }
  
  -- insere um elemento no fim da fila
  local insere = function (valor)
    self.ult = self.ult + 1
    self.dados[self.ult] = valor
  end
  
  -- pega e retira o primeiro elemento da fila
  local remove = function ()
    -- verifica se a fila está vazia
    if self.prim > self.ult then
      return nil 
    else
      -- se houver alguém na fila, devolve o primeiro elemento
      local valor = self.dados[self.prim]
      self.dados[self.prim] = nil -- limpa a posição da fila
      self.prim = self.prim + 1
      return valor
    end
  end
  
  -- métodos retornados
  return {
    insere = insere,
    remove = remove
  }
  
end

--[[
<dataValidations count="1">
<dataValidation type="list" allowBlank="1" showInputMessage="1" showErrorMessage="1" sqref="C3" xr:uid="{D84B114C-348A-49C0-B93D-922C2EAD8ABA}">
<formula1>"1,3,5,7,11,13,17"</formula1>
</dataValidation>
</dataValidations>

<formula1>$A$1:$A$5</formula1>


]]--