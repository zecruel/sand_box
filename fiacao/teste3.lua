-- teste da linguagem Lua
-- execute no interpretador com: dofile("teste.lua")

function split(str, sep)
	local sep, fields = sep or ":", {}
	local pattern = string.format("([^%s]+)", sep)
	str:gsub(pattern, function(c) fields[#fields+1] = c end)
	return fields
end   
      

dofile("teste_macro_Plan1.lua")

pares = {}

for chave, vetor in pairs(fiacao) do
	anterior = nil
	for indice, elem in ipairs(vetor) do
		if anterior ~= nil then
			table.insert(pares, {anterior, elem})
		end
		anterior = elem
	end
end

componentes = {}

for i, par in ipairs(pares) do
	campos = split(par[1], "%.")
	componente = string.upper(campos[1])
	terminal = string.upper(campos[2])
	if componentes[componente] == nil then
		componentes[componente] = {}
	end
	if componentes[componente][terminal] == nil then
		componentes[componente][terminal] = string.upper(par[2])
	else
		componentes[componente][terminal] = componentes[componente][terminal].."/"..string.upper(par[2])
	end
	
	campos = split(par[2], "%.")
	componente = string.upper(campos[1])
	terminal = string.upper(campos[2])
	if componentes[componente] == nil then
		componentes[componente] = {}
	end
	if componentes[componente][terminal] == nil then
		componentes[componente][terminal] = string.upper(par[1])
	else
		componentes[componente][terminal] = componentes[componente][terminal].."/"..string.upper(par[1])
	end
end


local file,err = io.open( "teste_fiacao.csv", "wb" )
file:write( "sep=;\r\n" )

local comp_ord = {}

for chave in pairs(componentes) do
    table.insert(comp_ord, chave)
end

table.sort(comp_ord)

for i = 1, #comp_ord do
	local componente, ligacoes = comp_ord[i], componentes[comp_ord[i]]
	print (componente)
	file:write( componente.."\r\n" )
	
	local term_ord = {}

	for chave in pairs(ligacoes) do
	    table.insert(term_ord, chave)
	end

	table.sort(term_ord)
	
	--for terminal, ligacao in pairs(ligacoes) do
	for j = 1, #term_ord do
		local terminal, ligacao =  term_ord[j], ligacoes[term_ord[j]]
		print (terminal.."-"..ligacao)
		file:write( ";"..terminal..";"..ligacao.."\r\n" )
	end
end

--file:write( "}" )
file:close()
