-- Floyd-Warshall algorithm
-- https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm

function floydWarshall(grafo)
  local dist = {}
  local prev = {}
  local numVertices = #grafo

  -- Inicializa a matriz de distância
  for i = 1, numVertices do
    dist[i] = {}
    prev[i] = {}
    for j = 1, numVertices do
      if i == j then
        dist[i][j] = 0
        --prev[i][j] = i
      elseif grafo[i] and grafo[i][j] then
        dist[i][j] = grafo[i][j]
        prev[i][j] = i
      else
        dist[i][j] = math.huge -- infinito para distâncias não conectadas
        --prev[i][j] = j
      end
    end
  end

  -- Aplica o algoritmo de Floyd-Warshall
  for k = 1, numVertices do
    for i = 1, numVertices do
      for j = 1, numVertices do
        if dist[i][j] > dist[i][k] + dist[k][j] then
            dist[i][j] = dist[i][k] + dist[k][j]
            prev[i][j] = prev[k][j]
        end
      end
    end
  end

  return dist, prev
end

-- Exemplo de uso
-- Representação do grafo como uma matriz de adjacência
-- grafo[i][j] contém o peso da aresta de i para j, ou nil se não houver aresta
local grafo = {
  {0, 1, 3, nil, 6},
  {nil, 0, 1, 3, nil},
  {1, 2, 0, 1, nil},
  {3, nil, nil, 0, 2},
  {nil, nil, nil, 1, 0}
}

local distancias, prevs = floydWarshall(grafo)

-- Imprime as distâncias
for i = 1, #distancias do
  for j = 1, #distancias do
    if distancias[i][j] == math.huge then
      io.write("inf ")
    else
      io.write(distancias[i][j] .. " ")
    end
  end
  io.write("\n")
end

 io.write("\n")
 
-- Imprime as distâncias
for i = 1, #distancias do
  for j = 1, #distancias do
    if distancias[i][j] == math.huge then
      io.write("inf ")
    else
      io.write(tostring(prevs[i][j]) .. " ")
    end
  end
  io.write("\n")
end