--coding: utf-8
-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar
-- Autor: Ezequiel Rabelo de Aguiar - 2023
-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8

-- ============= variáveis globais =====================
elems_pelicanu = {} -- lista principal dos elementos
tolerancia = 0.1 -- tolerancia para comparacoes de numeros não inteiros
biblioteca = "" -- diretorio onde estao os elementos (componentes, formatos, etc)

cadzinho.set_timeout (600)

require('pelicanu_util')
require('pelicanu_bd')
require('pelicanu_dyn')
require('pelicanu_dyn_prj')
require('pelicanu_dyn_esq')
require('pelicanu_malha_dyn')
require('pelicanu_fiacao_dyn')

dizeres = {
  pelicanu = '-- PELICAnU - Projeto Elétrico, Lógico, Interligação, Controle, Automação & Unifilar',
  autor = '-- Autor: Ezequiel Rabelo de Aguiar - 2023',
  lua = '-- Utiliza a sintaxe padrao da linguagem Lua 5.4. Codificado em UTF-8',
  alerta = '-- ========!! Altere-o com parcimonia !!========',
  proj = '-- Arquivo de configuracao de um projeto no PELICAnU, criado automaticamente',
  ini = '-- Arquivo de inicialização do PELICAnU, criado automaticamente',
  config = '-- Arquivo de configuracao geral do PELICAnU'
}

abreviacoes = {}
abreviacoes['DISJUNTOR'] = 'DISJ'
abreviacoes['ABERTURA'] = 'AB'
abreviacoes['PROTEÇÃO'] = 'PROT'
abreviacoes['PROTEçãO'] = 'PROT'
abreviacoes['PROTEçAO'] = 'PROT'
abreviacoes['PROTECãO'] = 'PROT'
abreviacoes['PROTECAO'] = 'PROT'
abreviacoes['PRINCIPAL'] = 'P'
abreviacoes['SUPLEMENTAR'] = 'S'
abreviacoes['TRANSFORMADOR'] = 'TRAFO'

function abrevia (texto)
  local t = string.upper (texto)
  
  for chave, subst in pairs (abreviacoes) do
    t = string.gsub(t, chave, subst)
  end
  
  return t
end



icone = "<svg width=\"80.0000\" height=\"80.0000\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">" ..
"<path stroke=\"rgb(254, 254, 254)\" fill=\"none\" d=\"M2.863 41.8 L36.42 41.96 L26.32 31.86 L25.22 30.52 L24.28 29.06 L23.52 27.51 L22.95 25.87 L22.58 24.18 L22.41 22.45 L22.45 20.72 L22.69 19 L23.14 17.33 L23.78 15.72 L24.6 14.19 L25.6 12.78 L26.76 11.49 L28.07 10.35 L29.5 9.377 L31.04 8.577 L32.66 7.964 L34.34 7.549 L36.06 7.335 L37.8 7.327 L39.52 7.525 L41.2 7.925 L42.83 8.522 L44.37 9.308 L45.82 10.27 L47.13 11.4 L48.3 12.67 L49.32 14.08 L50.16 15.6 L50.81 17.2 L51.27 18.87 L51.53 20.59 L39.37 20.59 L39.12 20.57 L38.87 20.58 L38.63 20.59 L38.38 20.63 L38.13 20.67 L37.89 20.73 L37.65 20.81 L37.42 20.89 L37.19 21 L36.97 21.11 L36.76 21.24 L36.55 21.38 L36.35 21.53 L36.16 21.69 L35.98 21.86 L35.81 22.04 L35.66 22.24 L35.51 22.44 L35.37 22.65 L35.25 22.86 L35.14 23.09 L35.04 23.32 L34.96 23.55 L34.89 23.79 L34.83 24.03 L34.79 24.28 L34.77 24.53 L34.75 24.78 L34.75 25.02 L34.77 25.27 L34.8 25.52 L34.85 25.77 L51.56 42.67 L51.77 42.86 L51.97 43.04 L52.18 43.22 L52.4 43.39 L52.61 43.57 L52.84 43.73 L53.06 43.9 L53.29 44.05 L53.51 44.21 L53.75 44.36 L53.98 44.51 L54.22 44.65 L54.46 44.78 L54.7 44.92 L54.95 45.05 L55.19 45.17 L55.44 45.29 L55.7 45.4 L55.95 45.51 L56.21 45.62 L56.46 45.72 L56.72 45.81 L56.99 45.9 L57.25 45.99 L57.51 46.07 L57.78 46.14 L58.05 46.21 L58.32 46.28 L58.59 46.34 L58.86 46.4 L59.13 46.45 L59.4 46.49 L60.1 47.28 L60.75 48.11 L61.34 48.98 L61.87 49.9 L62.34 50.84 L62.74 51.82 L63.08 52.82 L63.35 53.84 L63.55 54.87 L63.67 55.92 L63.73 56.97 L63.72 58.03 L63.63 59.08 L63.48 60.12 L63.25 61.15 L62.95 62.16 L62.59 63.15 L62.16 64.12 L61.67 65.05 L61.11 65.95 L60.5 66.8 L59.83 67.62 L59.11 68.39 L58.33 69.1 L57.51 69.76 L56.65 70.37 L55.75 70.92 L54.81 71.4 L53.84 71.82 L52.85 72.17 L51.83 72.46 L50.8 72.67 L24.67 72.67 L23.39 72.39 L22.14 72.05 L20.9 71.64 L19.68 71.18 L18.48 70.67 L17.31 70.09 L16.17 69.46 L15.06 68.78 L13.98 68.04 L12.94 67.26 L11.94 66.43 L10.98 65.54 L10.06 64.62 L9.192 63.65 L8.367 62.64 L7.59 61.6 L6.863 60.51 L6.189 59.4 L5.569 58.25 L5.004 57.08 L4.496 55.88 L4.045 54.66 L3.653 53.41 L3.32 52.15 L3.048 50.88 L2.836 49.59 L2.686 48.3 L2.598 47 L2.571 45.7 L2.607 44.39 L2.704 43.09 L2.863 41.8 \"/>" ..
"<path stroke=\"rgb(255, 128, 0)\" fill=\"none\" d=\"M34.85 25.77 L34.8 25.52 L34.77 25.27 L34.75 25.02 L34.75 24.78 L34.77 24.53 L34.79 24.28 L34.83 24.03 L34.89 23.79 L34.96 23.55 L35.04 23.32 L35.14 23.09 L35.25 22.86 L35.37 22.65 L35.51 22.44 L35.66 22.24 L35.81 22.04 L35.98 21.86 L36.16 21.69 L36.35 21.53 L36.55 21.38 L36.76 21.24 L36.97 21.11 L37.19 21 L37.42 20.89 L37.65 20.81 L37.89 20.73 L38.13 20.67 L38.38 20.63 L38.63 20.59 L38.87 20.58 L39.12 20.57 L39.37 20.59 L72.91 20.59 L73.15 20.61 L73.38 20.65 L73.61 20.7 L73.83 20.75 L74.06 20.82 L74.28 20.9 L74.5 20.99 L74.71 21.09 L74.92 21.2 L75.12 21.33 L75.31 21.45 L75.5 21.59 L75.69 21.74 L75.86 21.9 L76.03 22.06 L76.19 22.23 L76.34 22.41 L76.48 22.6 L76.62 22.79 L76.74 22.99 L76.86 23.2 L76.96 23.41 L77.06 23.62 L77.14 23.84 L77.21 24.07 L77.28 24.29 L77.33 24.52 L77.37 24.75 L77.4 24.98 L77.42 25.22 L77.43 25.45 L77.43 25.69 L77.43 31.77 L77.35 32.87 L77.2 33.96 L76.97 35.03 L76.66 36.09 L76.28 37.12 L75.83 38.12 L75.3 39.09 L74.71 40.02 L74.06 40.9 L73.34 41.73 L72.57 42.52 L71.74 43.24 L70.86 43.9 L69.94 44.5 L68.98 45.04 L67.98 45.5 L66.95 45.89 L65.9 46.2 L64.83 46.45 L63.74 46.61 L62.64 46.7 L61.54 46.7 L60.45 46.63 L59.36 46.49 L58.28 46.26 L57.22 45.96 L56.19 45.58 L55.19 45.14 L54.22 44.62 L53.29 44.03 L52.4 43.38 L51.56 42.67 L34.85 25.77 \"/>" ..
"<path stroke=\"rgb(192, 192, 192)\" fill=\"none\" d=\"M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 \"/>" ..
"<path stroke=\"rgb(254, 254, 254)\" fill=\"none\" d=\"M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>" ..
"<path fill=\"rgb(254, 254, 254)\" stroke=\"none\" d=\"M2.863 41.8 L36.42 41.96 L26.32 31.86 L25.22 30.52 L24.28 29.06 L23.52 27.51 L22.95 25.87 L22.58 24.18 L22.41 22.45 L22.45 20.72 L22.69 19 L23.14 17.33 L23.78 15.72 L24.6 14.19 L25.6 12.78 L26.76 11.49 L28.07 10.35 L29.5 9.377 L31.04 8.577 L32.66 7.964 L34.34 7.549 L36.06 7.335 L37.8 7.327 L39.52 7.525 L41.2 7.925 L42.83 8.522 L44.37 9.308 L45.82 10.27 L47.13 11.4 L48.3 12.67 L49.32 14.08 L50.16 15.6 L50.81 17.2 L51.27 18.87 L51.53 20.59 L39.37 20.59 L39.12 20.57 L38.87 20.58 L38.63 20.59 L38.38 20.63 L38.13 20.67 L37.89 20.73 L37.65 20.81 L37.42 20.89 L37.19 21 L36.97 21.11 L36.76 21.24 L36.55 21.38 L36.35 21.53 L36.16 21.69 L35.98 21.86 L35.81 22.04 L35.66 22.24 L35.51 22.44 L35.37 22.65 L35.25 22.86 L35.14 23.09 L35.04 23.32 L34.96 23.55 L34.89 23.79 L34.83 24.03 L34.79 24.28 L34.77 24.53 L34.75 24.78 L34.75 25.02 L34.77 25.27 L34.8 25.52 L34.85 25.77 L51.56 42.67 L51.77 42.86 L51.97 43.04 L52.18 43.22 L52.4 43.39 L52.61 43.57 L52.84 43.73 L53.06 43.9 L53.29 44.05 L53.51 44.21 L53.75 44.36 L53.98 44.51 L54.22 44.65 L54.46 44.78 L54.7 44.92 L54.95 45.05 L55.19 45.17 L55.44 45.29 L55.7 45.4 L55.95 45.51 L56.21 45.62 L56.46 45.72 L56.72 45.81 L56.99 45.9 L57.25 45.99 L57.51 46.07 L57.78 46.14 L58.05 46.21 L58.32 46.28 L58.59 46.34 L58.86 46.4 L59.13 46.45 L59.4 46.49 L60.1 47.28 L60.75 48.11 L61.34 48.98 L61.87 49.9 L62.34 50.84 L62.74 51.82 L63.08 52.82 L63.35 53.84 L63.55 54.87 L63.67 55.92 L63.73 56.97 L63.72 58.03 L63.63 59.08 L63.48 60.12 L63.25 61.15 L62.95 62.16 L62.59 63.15 L62.16 64.12 L61.67 65.05 L61.11 65.95 L60.5 66.8 L59.83 67.62 L59.11 68.39 L58.33 69.1 L57.51 69.76 L56.65 70.37 L55.75 70.92 L54.81 71.4 L53.84 71.82 L52.85 72.17 L51.83 72.46 L50.8 72.67 L24.67 72.67 L23.39 72.39 L22.14 72.05 L20.9 71.64 L19.68 71.18 L18.48 70.67 L17.31 70.09 L16.17 69.46 L15.06 68.78 L13.98 68.04 L12.94 67.26 L11.94 66.43 L10.98 65.54 L10.06 64.62 L9.192 63.65 L8.367 62.64 L7.59 61.6 L6.863 60.51 L6.189 59.4 L5.569 58.25 L5.004 57.08 L4.496 55.88 L4.045 54.66 L3.653 53.41 L3.32 52.15 L3.048 50.88 L2.836 49.59 L2.686 48.3 L2.598 47 L2.571 45.7 L2.607 44.39 L2.704 43.09 L2.863 41.8 M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>" ..
"<path fill=\"rgb(255, 128, 0)\" stroke=\"none\" d=\"M34.85 25.77 L34.8 25.52 L34.77 25.27 L34.75 25.02 L34.75 24.78 L34.77 24.53 L34.79 24.28 L34.83 24.03 L34.89 23.79 L34.96 23.55 L35.04 23.32 L35.14 23.09 L35.25 22.86 L35.37 22.65 L35.51 22.44 L35.66 22.24 L35.81 22.04 L35.98 21.86 L36.16 21.69 L36.35 21.53 L36.55 21.38 L36.76 21.24 L36.97 21.11 L37.19 21 L37.42 20.89 L37.65 20.81 L37.89 20.73 L38.13 20.67 L38.38 20.63 L38.63 20.59 L38.87 20.58 L39.12 20.57 L39.37 20.59 L72.91 20.59 L73.15 20.61 L73.38 20.65 L73.61 20.7 L73.83 20.75 L74.06 20.82 L74.28 20.9 L74.5 20.99 L74.71 21.09 L74.92 21.2 L75.12 21.33 L75.31 21.45 L75.5 21.59 L75.69 21.74 L75.86 21.9 L76.03 22.06 L76.19 22.23 L76.34 22.41 L76.48 22.6 L76.62 22.79 L76.74 22.99 L76.86 23.2 L76.96 23.41 L77.06 23.62 L77.14 23.84 L77.21 24.07 L77.28 24.29 L77.33 24.52 L77.37 24.75 L77.4 24.98 L77.42 25.22 L77.43 25.45 L77.43 25.69 L77.43 31.77 L77.35 32.87 L77.2 33.96 L76.97 35.03 L76.66 36.09 L76.28 37.12 L75.83 38.12 L75.3 39.09 L74.71 40.02 L74.06 40.9 L73.34 41.73 L72.57 42.52 L71.74 43.24 L70.86 43.9 L69.94 44.5 L68.98 45.04 L67.98 45.5 L66.95 45.89 L65.9 46.2 L64.83 46.45 L63.74 46.61 L62.64 46.7 L61.54 46.7 L60.45 46.63 L59.36 46.49 L58.28 46.26 L57.22 45.96 L56.19 45.58 L55.19 45.14 L54.22 44.62 L53.29 44.03 L52.4 43.38 L51.56 42.67 L34.85 25.77 \"/>" ..
"<path fill=\"rgb(192, 192, 192)\" stroke=\"none\" d=\"M15 46.91 L39.47 46.91 L40.33 47.07 L41.18 47.29 L42.01 47.59 L42.8 47.96 L43.56 48.41 L44.28 48.91 L44.95 49.48 L45.56 50.1 L46.13 50.78 L46.62 51.5 L47.06 52.26 L47.42 53.06 L47.72 53.89 L47.94 54.74 L48.08 55.6 L48.15 56.48 L48.14 57.36 L48.05 58.23 L47.88 59.09 L47.65 59.94 L47.33 60.76 L46.95 61.55 L46.5 62.3 L45.98 63.01 L45.41 63.67 L44.77 64.28 L44.09 64.83 L43.36 65.32 L42.59 65.74 L41.79 66.1 L40.96 66.38 L40.1 66.58 L38.98 66.85 L37.84 67.05 L36.7 67.19 L35.54 67.26 L34.39 67.26 L33.24 67.19 L32.09 67.06 L30.95 66.87 L29.83 66.6 L28.72 66.27 L27.63 65.88 L26.57 65.43 L25.54 64.91 L24.53 64.34 L23.57 63.71 L22.64 63.02 L21.75 62.29 L20.9 61.5 L20.11 60.66 L19.36 59.78 L18.67 58.86 L18.03 57.9 L17.44 56.9 L16.92 55.87 L16.45 54.81 L16.05 53.73 L15.71 52.63 L15.44 51.5 L15.23 50.37 L15.09 49.22 L15.01 48.07 L15 46.91 \"/>" ..
"<path fill=\"rgb(128, 128, 128)\" stroke=\"none\" d=\"M38.44 17.03 L38.44 17.13 L38.45 17.23 L38.47 17.33 L38.51 17.43 L38.55 17.52 L38.6 17.61 L38.66 17.69 L38.73 17.77 L38.8 17.84 L38.88 17.9 L38.97 17.96 L39.06 18 L39.16 18.04 L39.25 18.07 L39.35 18.08 L39.46 18.09 L39.56 18.09 L39.66 18.08 L39.76 18.05 L39.86 18.02 L39.95 17.98 L40.04 17.93 L40.12 17.87 L40.2 17.8 L40.27 17.73 L40.33 17.65 L40.39 17.56 L40.44 17.47 L40.47 17.37 L40.5 17.27 L40.52 17.17 L40.52 17.07 L40.52 16.97 L40.51 16.87 L40.48 16.77 L40.45 16.67 L40.41 16.58 L40.36 16.49 L40.3 16.4 L40.23 16.33 L40.16 16.26 L40.08 16.19 L39.99 16.14 L39.9 16.09 L39.8 16.06 L39.7 16.03 L39.6 16.01 L39.5 16 L39.4 16.01 L39.3 16.02 L39.2 16.04 L39.1 16.08 L39.01 16.12 L38.92 16.17 L38.83 16.23 L38.76 16.29 L38.69 16.37 L38.62 16.45 L38.57 16.54 L38.52 16.63 L38.49 16.72 L38.46 16.82 L38.44 16.92 L38.44 17.03 \"/>" ..
"<path stroke=\"rgb(0, 0, 0)\" fill=\"none\" d=\"M34.85 25.77 L77.43 25.69 \"/>" ..
"</svg>"

icon = cadzinho.svg_image(icone,20,20)

projeto = { 
  titulo = '',
  instalacao = '',
  aplicacao = '',
  codigo = '',
  rev = '',
  aprovacao = '',
  visto = '',
  projetista = '',
  descr = '',
  data = '',
  caminho = ''
}

-- para funcoes dinamicas
num_pt = 1 -- numero de pontos
pts = {} -- lista de pontos de entrada
lista_comp = {}
lista_comp_o = {}
lista_comp_tipo = {}
lista_formato = {}
lista_formato_o = {}
lista_esq = {}
lista_esq_o = {}

-- para a interface grafica
component = {value = ''}
g_caixa_id = {value = ''}
g_caixa_tipo = {value = 1, 'COMPONENTE', 'MODULO', 'PAINEL', 'DESCRITIVO', 'REFERENCIA', 'GENERICO'}
g_editor_abas = {value = 1, "Esquematico", "Biblioteca"}
g_term_num = {value = 1}
g_term_nome = {value = "1"}
g_eng_num = {value = 1}
g_eng_nome = {value = "E1"}
g_engate = {value = ""}
g_componente = {value = ""}
g_tipo_comp = {value = 1, ""}
g_comp_id = {value = ""}
g_terminais = {}
g_formato = {value = ""}
modal = ''
sub_modal = ''
g_caminho = {value = ""}
msg = ''
g_titulo = {value = ""}
g_instalacao = {value = ""}
g_aplicacao = {value = ""}
g_codigo = {value = ""}
g_rev = {value = ""}
g_aprovacao = {value = ""}
g_visto = {value = ""}
g_projetista = {value = ""}
g_descr = {value = ""}
g_data = {value = ""}
g_sub = {value = ""}


g_descricoes = {}

g_fmt_id = {value = "XXXXXYYYYZZZZ"}
g_fmt_prj = {value = "Vão XX"}
g_fmt_tit = {value = "Circuito XX"}
g_fmt_rev = {value = "a"}
g_fmt_ver = {value = "1"}
g_fmt_fl = {value = "1"}
g_fmt_pfl = {value = "1"}

g_num_auto = {value = false}

g_ref_alt = {value = 5}
g_ref_descr = {value = 32}
g_ref_desenho = {value = 32}
g_ref_term = {value = ""}
g_ref_term_ocul = {value = false}
g_ref_descr_ocul = {value = false}
g_ref_modo = {value = 1, "LE", "Manual"}
g_ref_le = {}
g_ref_le_num = {}
g_ref_item = {value = ""}
g_ref_elem = {value = 1, 'CONTATO_NA', 'CONTATO_NF', 'BOBINA'}
g_ref_contat = {value = false}

g_ligacao_tipo = {value = 1, "Baixa", "Alta"}

g_malha_espacamento = {value = 1.0}
g_malha_i = {value = 1000}
g_malha_res_remoto = {value = 0.0001}
g_malha_res_cabo = {value = 0.0001}

g_malha_cams = {value = 4}
g_malha_cam_malha = {value = 2}

g_malha_arq_res = {value = "malha.csv"}

g_malha_res_cam = {{value = 700},
  {value = 600},
  {value = 500},
  {value = 400},
  {value = 300},
  {value = 200},
  {value = 100},
  {value = 50}}

g_malha_prof_cam = {{value = 0.5},
  {value = 0.1},
  {value = 1.0},
  {value = 1.0},
  {value = 1.0},
  {value = 1.0},
  {value = 1.0},
  {value = 1.0}}




excel = require "xlsxwriter.workbook"

cores_claras = {
  [1] = '#FFBC37', -- laranja
  [2] = '#66FF66', -- verde
  [3] = '#FF7C80', -- vermelho
  [4] = '#66CCFF', -- azul
  [5] = '#CC66FF', -- roxo
  [6] = '#FFFF66', -- amarelo
  [7] = '#C68C52', -- marrom
  [8] = '#DDDDDD' -- cinza
}

malha_corrotina = false
malha_corr_exec = false
malha_grade = {}
malha_cabos = {}
malha_camadas = {}
malha_matriz = {}
malha_v = {}
malha_b = {}
malha_iter = 1
malha_erro = 1000
malha_max_iter = 1000
malha_crit_parada = 1e-6

malha_camadas[1] = {res = 760, prof = 0.6}
malha_camadas[2] = {res = 760, prof = 0.1}
malha_camadas[3] = {res = 191, prof = 2}

malha_i = 3200
malha_res_cabo = 0.0001
malha_res_remoto = 0.001
--local g_t_remoto = (lins * cols) / res_remoto
malha_cam_cabos = 2

-- ============================================


function abre_projeto(caminho)
  if type(caminho) ~= 'string' then return nil end
  local config = format_dir(caminho) .. '_dados' .. fs.dir_sep .. 'projeto.lua'
  local bd = format_dir(caminho) .. '_dados'.. fs.dir_sep .. 'projeto.db'
  local log = format_dir(caminho) .. '_dados'.. fs.dir_sep .. 'log.txt'
  local saida = format_dir(caminho) .. '_saida' .. fs.dir_sep
  local aux = format_dir(caminho) .. '_aux' .. fs.dir_sep
  local proj, err = carrega_config(config)
  if not proj then return nil end
  
  projeto.caminho = caminho
  projeto.config = config
  --if exists(bd) then 
    projeto.bd = bd
  --else
  --  projeto.bd = nil
  --end
  --if exists(log) then 
    projeto.log = log
  --else
  --  projeto.log = nil
  --end
  if exists(saida) then 
    projeto.saida = saida
  else
    projeto.saida = nil
  end
  if type(proj.titulo) == 'string' then
    projeto.titulo = proj.titulo
  else
    projeto.titulo = ''
  end
  if type(proj.instalacao) == 'string' then
    projeto.instalacao = proj.instalacao
  else
    projeto.instalacao = ''
  end
  if type(proj.aplicacao) == 'string' then
    projeto.aplicacao = proj.aplicacao
  else
    projeto.aplicacao = ''
  end
  if type(proj.codigo) == 'string' then
    projeto.codigo = proj.codigo
  else
    projeto.codigo = '' 
  end
  if type(proj.rev) == 'string' then
    projeto.rev = proj.rev
  else
    projeto.rev = ''
  end
  if type(proj.aprovacao) == 'string' then
    projeto.aprovacao = proj.aprovacao
  else
    projeto.aprovacao = ''
  end
  if type(proj.visto) == 'string' then
    projeto.visto = proj.visto
  else
    projeto.visto = ''
  end
  if type(proj.projetista) == 'string' then
    projeto.projetista = proj.projetista
  else
    projeto.projetista = ''
  end
  if type(proj.descr) == 'string' then
    projeto.descr = proj.descr
  else
    projeto.descr = ''
  end
  if type(proj.data) == 'string' then
    projeto.data = proj.data
  else
    projeto.data = ''
  end
  
  local arq_ini = io.open(diretorio(fs.script_path()) .. 'ini.lua', 'w+')
  if arq_ini then
    arq_ini:write(dizeres.pelicanu..'\n')
    arq_ini:write(dizeres.autor..'\n')
    arq_ini:write(dizeres.ini..'\n')
    arq_ini:write(dizeres.lua..'\n')
    arq_ini:write(dizeres.alerta..'\n\n')
    arq_ini:write('projeto = "'.. string.gsub(caminho, '\\', '\\\\') .. '"')
    
    arq_ini:close()
  end
  
  fs.chdir(caminho)
  
  return true
end

function novo_projeto(caminho, proj)
  if type(caminho) ~= 'string' then return nil end
  if type(proj) ~= 'table' then return nil end
  local dados = format_dir(caminho) .. '_dados' .. fs.dir_sep
  local saida = format_dir(caminho) .. '_saida' .. fs.dir_sep
  local aux = format_dir(caminho) .. '_aux' .. fs.dir_sep
  local config = dados .. 'projeto.lua'
  local bd = dados .. 'projeto.db'
  local log = dados .. 'log.txt'
  
  if not exists (caminho) then
    cria_pasta(caminho)
  end
  
  if not exists (caminho) then return nil end
  
  if not exists (dados) then
    cria_pasta(dados)
  end
  if not exists (saida) then
    cria_pasta(saida)
  end
  
  if not exists (aux) then
    cria_pasta(aux)
  end
  
  local pl_base = aux .. 'base.xlsm'
  if not exists(pl_base) then
    copia_arq(diretorio(fs.script_path()) .. 'base.xlsm', pl_base)
  end
  
  local arq_prj, err = io.open(config, 'w+')
  if not arq_prj then return nil end
  arq_prj:write(dizeres.pelicanu..'\n')
  arq_prj:write(dizeres.autor..'\n')
  arq_prj:write(dizeres.proj..'\n')
  arq_prj:write(dizeres.lua..'\n')
  arq_prj:write(dizeres.alerta..'\n\n')
  
  arq_prj:write('titulo = "'.. string.gsub(proj.titulo, '\\', '\\\\') .. '"\n')
  arq_prj:write('instalacao = "'.. string.gsub(proj.instalacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('aplicacao = "'.. string.gsub(proj.aplicacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('codigo = "'.. string.gsub(proj.codigo, '\\', '\\\\') .. '"\n')
  arq_prj:write('rev = "'.. string.gsub(proj.rev, '\\', '\\\\') .. '"\n')
  arq_prj:write('aprovacao = "'.. string.gsub(proj.aprovacao, '\\', '\\\\') .. '"\n')
  arq_prj:write('visto = "'.. string.gsub(proj.visto, '\\', '\\\\') .. '"\n')
  arq_prj:write('projetista = "'.. string.gsub(proj.projetista, '\\', '\\\\') .. '"\n')
  arq_prj:write('descr = "'.. string.gsub(proj.descr, '\\', '\\\\') .. '"\n')
  arq_prj:write('data = "'.. string.gsub(proj.data, '\\', '\\\\') .. '"\n')
  
  arq_prj:close()
  
  local arq_log = io.open(log, 'a+')
  if arq_log then
    arq_log:write(os.date('%Y/%m/%d-%H:%M:%S') .. ' => Criação do projeto\n')
    
    arq_log:close()
  end
  
  bd_novo(bd, proj)
  
  return abre_projeto(caminho)
end

function lista_proj()
  local lista_arq = {}
  local fila = nova_fila()
  
  -- insere a pasta do projeto na fila
  fila.insere(format_dir(projeto.caminho))
  
  -- processa a fila de pastas
  local pasta = fila.remove()
  while pasta do
    local dir = fs.dir(pasta)
    -- varre o conteúdo da pasta corrente
    for i = 1, #dir do
      local item = pasta .. dir[i].name
      if dir[i].is_dir then
        -- se o item for uma subpasta
        if not string.find(dir[i].name, '^_') then -- e não for ignorável
          fila.insere(format_dir(item)) -- coloca na fila
        end
      else
        -- verifica se é um arquivo DXF
        local ext = extensao(dir[i].name)
        if type(ext) == "string" then
          if ext:upper() == ".DXF" then
            dir[i].name = item -- caminho completo do arquivo
            -- adiciona na lista de arquivos
            lista_arq[#lista_arq +1] = dir[i]
          end
        end
      end
    end
    
    -- pega a próxima pasta da fila
    pasta = fila.remove()
  end
  
  return lista_arq
end

function no_segmento (pt, linha)
  -- Verifica se um ponto esta num segmento de reta
  
  -- confere os parametros passados
  if type(pt) ~= "table" then return false end
  if type(linha) ~= "table" then return false end
  
  -- primeiro, checa se o ponto pertence a reta
  if math.abs((pt.x - linha[1].x) / (linha[2].x - linha[1].x) - 
    (pt.y - linha[1].y) / (linha[2].y - linha[1].y)) > tolerancia then
    return false
  end
  
  -- agora, verifica se esta dentro do segmento
  if ( pt.x > linha[1].x and pt.x < linha[2].x or 
    pt.x < linha[1].x and pt.x > linha[2].x) or 
    ( pt.y > linha[1].y and pt.y < linha[2].y or 
    pt.y < linha[1].y and pt.y > linha[2].y) then
    return true
  end
  
  -- nao estah no segmento
  return false
end

function no_segmento2 (pt, linha)
  -- Verifica se um ponto esta num segmento de reta
  
  -- confere os parametros passados
  if type(pt) ~= "table" then return false end
  if type(linha) ~= "table" then return false end
  
  local dxc = pt.x - linha[1].x
  local dyc = pt.y - linha[1].y
  local dxl = linha[2].x - linha[1].x
  local dyl = linha[2].y - linha[1].y
  -- calcula o produto vetorial
  local prod = dxc * dyl - dyc * dxl
  
  -- verifica se esta na reta
  if math.abs(prod) > tolerancia then return false end
  
  -- calcula o produto escalar
  prod = dxc * dxl + dyc * dyl
   -- calcula a distancia quadratica
  local dist = dxl * dxl + dyl * dyl
   -- agora, verifica se esta dentro do segmento
   if prod < -tolerancia  or prod > dist + tolerancia then return false end
   
  return true
end

function intersec(l1, l2)
	-- Verifica se há interseção entre dois segmentos de reta e calcula o ponto

	-- confere os parametros passados
	if type(l1) ~= "table" then return false end
	if type(l2) ~= "table" then return false end
	
	-- verifica se as linhas não são paralelas
	local den = (l1[1].x - l1[2].x) * (l2[1].y - l2[2].y) - (l1[1].y - l1[2].y) * (l2[1].x - l2[2].x)
	if math.abs(den) < tolerancia then return false end
	
	t = ((l1[1].x - l2[1].x) * (l2[1].y - l2[2].y) - (l1[1].y - l2[1].y) * (l2[1].x - l2[2].x)) / den
	
	if t > 1 or t < 0 then return false end
	
	local pt = {}
	pt.x = l1[1].x + t * (l1[2].x - l1[1].x)
	pt.y = l1[1].y + t * (l1[2].y - l1[1].y)
	return pt
end

function linha_conect (a, b)
  -- Verifica se dois segmentos de linha estão conectados
  
  -- Verify if passed argments are valid
  if type(a) ~= "table" then return false end
  if type(b) ~= "table" then return false end
  
  -- testa pra ver se nao sao o mesmo objeto
  if a == b then return false end
  
  -- trivialmente, testa a coincidencia de vertices
  for _, pt1 in ipairs(a) do
    for _, pt2 in ipairs(b) do
      if math.abs(pt1.x - pt2.x) < tolerancia and math.abs(pt1.y - pt2.y) < tolerancia then
        return true
      end
    end
  end
  
  -- Senão, testa se algum vertice toca a outra linha
  for _, pt1 in ipairs(a) do
    if no_segmento(pt1, b) then return true end
  end
  for _, pt1 in ipairs(b) do
    if no_segmento(pt1, a) then return true end
  end
  
  -- nao tem conexao
  return false
end

function circulo_conect (linha, circulo)
  -- Verifica se um circulo e uma linha estão conectados
  
  -- Verify if passed argments are valid
  if type(linha) ~= "table" then return false end
  if type(circulo) ~= "table" then return false end
  
  -- testa pra ver se nao sao o mesmo objeto
  if linha == circulo then return false end
  
  -- trivialmente, testa se algum vertice da linha esta dentro ou toca o circulo
  local raio = (math.abs(circulo.radius) + tolerancia) ^ 2
  for _, pt in ipairs(linha) do
    local dist = (circulo.center.x - pt.x)^2 + (circulo.center.y - pt.y)^2
    if dist < raio then
      return true
    end
  end
  
  -- nao tem conexao
  return false
end

function term_conect (terminal, ligacao)
  -- Verifica a conexao entre um terminal (componente) e uma ligacao (fio)
  
  for _, figura in ipairs(terminal) do
    -- cadzinho.db_print(figura.tipo)
    if figura.linha then
      if linha_conect(figura.linha, ligacao) then
        return true
      end
    elseif figura.circulo then
      if circulo_conect (ligacao, figura.circulo) then
        return true
      end
    end
  end
  
  -- nao tem conexao
  return false
end

function dentro_poligono(pt, polig)
  -- Verifica se um ponto esta dendro do poligono
  
  -- Verifica os parametros passados
  if type(pt) ~= "table" then return false end
  if type(polig) ~= "table" then return false end
  
  local dentro = false
  local ant = #polig -- ponto anterior (indice)
  
  -- metodo de escaneamento por linha horizontal
  for i = 1, #polig do
    if ((polig[i].y > pt.y) ~= (polig[ant].y > pt.y)) and 
    (pt.x < (polig[ant].x - polig[i].x) * (pt.y - polig[i].y) /
    (polig[ant].y - polig[i].y) + polig[i].x) then
      dentro = not dentro
    end
    ant = i
  end
  
  return dentro
end

function dentro_contorno (ent, contorno)
  -- verifica se um objeto do desenho esta dentro de um contorno
  local tol = 0.1 -- tolerância
  local limite = cadzinho.get_bound(ent) -- pega os limites do objeto (retangulo)
  -- verifica se o retangulo está dentro do contorno
  limite.low.x = limite.low.x + tol
  limite.low.y = limite.low.y + tol
  limite.up.x = limite.up.x - tol
  limite.up.y = limite.up.y - tol
  return dentro_poligono(limite.low, contorno) and dentro_poligono(limite.up, contorno)
end

function atualiza_elems()
  elems_pelicanu = {}
  for i, ent in ipairs(cadzinho.get_all()) do -- varre todos os objetos do desenho
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 0 then
      -- elemento do PELICAnU
      local elem = {}
      local unico = "0"
      local tipo = "NADA"
      local especifico = ""
      if type(ext[1]) == "string" then
        unico = ext[1]:upper() -- identificador unico (muda p/ maiusculo)
      end
      if #ext > 1 then
        tipo = ext[2]:upper() -- tipo de elemento PELICAnU (muda p/ maiusculo)
      end
      if #ext > 2 then
        especifico = ext[3] -- dado especifico do tipo (opcional)
      end
      local unic = tonumber(unico,16)
      elem.id = unic
      elem.ent = ent
      elem.tipo = tipo
      elem.esp = especifico
      
      elems_pelicanu[unic] = elem -- armazena na lista principal
    end
  end
end

function pega_conteudo(id)
  -- pega o conteudo de uma caixa indicada pelo ID
  
  local caixa = elems_pelicanu[id] -- busca o elemento da lista principal
  if caixa == nil then
    return nil
  end
  
  local limite = cadzinho.get_bound(caixa.ent)
  local contorno = {{x=limite.low.x, y=limite.low.y, z=limite.low.z},
    {x=limite.up.x, y=limite.low.y, z=limite.low.z},
    {x=limite.up.x, y=limite.up.y, z=limite.up.z},
    {x=limite.low.x, y=limite.up.y, z=limite.low.z}}

  local tipo = cadzinho.get_ent_typ(caixa.ent)

  if tipo == 'LWPOLYLINE' then
    contorno = cadzinho.get_points(caixa.ent)
  end
  
  -- contorno da caixa, considerando ser uma polyline
  local conteudo = {}
  
  -- varredura em todos elementos
  for el_id, el in pairs(elems_pelicanu) do
    if el ~= caixa then
      -- verifica se o elemento atual está dentro da caixa
      if dentro_contorno(el.ent, contorno) then
        conteudo[#conteudo+1] = el_id -- adiciona-o a lista de retorno
      end
    end
  end
  return conteudo -- retorna o conteudo da caixa
end

function conteudo_todo()
  -- pega o conteúdo (elementos PELICAnU) do desenho inteiro
  
  local conteudo = {}
  for el_id in pairs(elems_pelicanu) do
    conteudo[#conteudo+1] = el_id
  end
  return conteudo
end

function atualiza_unicos()
  -- atualiza os identificadores únicos nos elementos do desenho
  
  for i, ent in ipairs(cadzinho.get_all()) do -- varre todos os objetos do desenho
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 1 then
      -- muda o id unico e grava o no desenho
      cadzinho.edit_ext_i(ent, "PELICANU", 1, cadzinho.unique_id())
      ent:write()
    end
  end
end

function rotulo_caixa(conteudo)
  -- busca rotulo da caixa
  
  local rotulo = nil
  for el_id in pairs(conteudo) do -- varredura do conteudo da caixa
    local el = elems_pelicanu[el_id]
    if el.tipo == "ROTULO" and el.esp == "CAIXA" then -- se for o tipo procurado
      --pega seu texto (considerando que é uma entidade tipo TEXT)
      rotulo = cadzinho.get_text_data(el.ent)
      break
    end
  end
  if rotulo then return rotulo.text
  else return nil
  end
end

function muda_comp_id (comp, id)
-- altera a identificação de um componente do PELICAnU

  -- decompoe o texto para o formato fracao
  local id1 = ""
  local id2 = ""
  if id then
    id1 = string.match(id, '^([^/]*)/?.*$')
    id2 = string.match(id, '/(.*)$')
  end
  local idx1 = 0
  local idx2 = 0
  local ocul1 = false
  local ocul2 = false
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "ID*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag']:upper(), "^ID1") then
      idx1 = i
      ocul1 = attr['hidden']
    -- "denominador"
    elseif string.find(attr['tag']:upper(), "^ID2") then
      idx2 = i
      ocul2 = attr['hidden']
    end
  end
  
  if idx1 > 0 and idx2 > 0 and id1 and id2 then
    -- formato fracao completa
    id1 = '%%U' .. id1 -- o texto do "numerador" eh subinhado
    cadzinho.edit_attr(comp, idx1, 'ID1', id1, ocul1)
    cadzinho.edit_attr(comp, idx2, 'ID2', id2, ocul2)
    
  -- ID somente no "numerador"
  elseif idx1 > 0 then
    cadzinho.edit_attr(comp, idx1, 'ID1', id, ocul1)
  end
  if idx2 > 0 and not id2 then -- se houver o "denominador", deixa ele vazio
    cadzinho.edit_attr(comp, idx2, 'ID2', '', ocul2)
  end
end

function pega_comp_id (comp)
-- obtem a identificacao de um componente PELICAnU. O retorno eh um texto unico combinado
  local id = false
  local id1 = ''
  local id2 = ''
  local idx1 = 0
  local idx2 = 0
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "ID*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag']:upper(), "^ID1") then 
      idx1 = i
      id1 = attr['value']
      id1 = string.gsub(id1, '%%%%%a', '') -- retira as marcas de formatacao, se houver
    end
    -- "denominador"
    if string.find(attr['tag']:upper(), "^ID2") then
      idx2 = i
      id2 = attr['value']
      id2 = string.gsub(id2, '%%%%%a', '') -- retira as marcas de formatacao, se houver
    end
  end
  
  if idx1 > 0 then
    if idx2 > 0 and #id2 > 0 then
      id = id1 .. '/' .. id2 -- combina as duas IDs num texto unico
    else
      id = id1 -- so possui a ID principal
    end
  end
  
  return id
end

function pega_attrib (ent)
-- obtem os dados de uma entidade tipo INSERT (bloco). Retorna uma tabela
  local dados = {}
  local tipo = cadzinho.get_ent_typ(ent)
  -- soh aceita INSERT
  if tipo ~= 'INSERT' then
    return dados
  end
  -- varre os elementos ATTRIB da entidade, cadastrando-as na tabela
  local attrs = cadzinho.get_attribs(ent)
  for i, attr in ipairs(attrs) do
    dados[attr.tag:upper()] = attr.value
  end
  
  return dados
end

function muda_atrib (ent, dados)
-- altera as informacoes textuais dos terminais de um componente
-- entrada: tabela com pares indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1

  local ocul = false
  -- varre os elementos ATTRIB da entidade
  local attrs = cadzinho.get_attribs(ent)
  for i, attr in ipairs(attrs) do
    -- confronta o marcador do atributo com a tabela
    if dados[attr['tag']:upper()] then
      ocul = attr['hidden'] -- mantem a configuração de "oculto"
      -- modifica o indice encontrado
      cadzinho.edit_attr(ent, i, attr['tag']:upper(), dados[attr['tag']:upper()], ocul)
    end
  end
end

function pega_comp_tipo(comp)
  local dados = pega_attrib(comp)
  return dados.TIPO
end

function pega_comp_sub(comp)
  local dados = pega_attrib(comp)
  return dados.SUB
end

function pega_engate(comp)
  local dados = pega_attrib(comp)
  return dados.ENGATE
end

function muda_engate(comp, engate, num, desenho, folha, descr)
  local eng_o = false
  local eng_i = 0
  local num_o = false
  local num_i = 0
  local des_o = false
  local des_i = 0
  local fl_o = false
  local fl_i = 0
  local descr_o = false
  local descr_i = 0

  -- varre os elementos ATTRIB da entidade, buscando a etiqueta "ENGATE"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    -- "numerador' ou ID principal
    if string.find(attr['tag']:upper(), "ENGATE") then
      eng_i = i
      eng_o = attr['hidden']
    elseif string.find(attr['tag']:upper(), "E1") then
      num_i = i
      num_o = attr['hidden']
    elseif string.find(attr['tag']:upper(), "REF1") then
      des_i = i
      des_o = attr['hidden']
    elseif string.find(attr['tag']:upper(), "REF2") then
      fl_i = i
      fl_o = attr['hidden']
    elseif string.find(attr['tag']:upper(), "DESCR1") then
      descr_i = i
      descr_o = attr['hidden']
  end
  end
  
  if eng_i > 0 and engate then
    cadzinho.edit_attr(comp, eng_i, 'ENGATE', engate, eng_o)
  end
  if num_i > 0 and num then
    cadzinho.edit_attr(comp, num_i, 'E1', num, num_o)
  end
  if des_i > 0 and desenho then
    cadzinho.edit_attr(comp, des_i, 'REF1', desenho, des_o)
  end
  if fl_i > 0 and folha then
    cadzinho.edit_attr(comp, fl_i, 'REF2', folha, fl_o)
  end
  if descr_i > 0 and descr then
    cadzinho.edit_attr(comp, descr_i, 'DESCR1', descr, descr_o)
  end

end

function rotacao (pt, ang)
--funcao de rotacao de um ponto no plano cartesiano, angulo em graus
  local cos = math.cos(ang * math.pi / 180)
  local sen = math.sin(ang * math.pi / 180)
  
  local x = pt.x * cos - pt.y * sen
  local y = pt.x * sen + pt.y * cos
  pt.x = x
  pt.y = y
end

function pega_terminais (comp)
  -- pega os pontos de conexao de um componente - terminais
  local conexoes = {}
  
  -- nome do bloco do componente
  local nome_b = cadzinho.get_blk_name (comp)
  if not nome_b then return conexoes end -- erro
  
  -- dados do componente (ponto de insercao, escalas, rotacao)
  local dados_comp = cadzinho.get_ins_data (comp)
  if not dados_comp then return conexoes end -- erro
  
  -- varre os elementos internos do bloco, procurando conexoes
  for _, ent in ipairs(cadzinho.get_blk_ents(nome_b)) do
    local ext = cadzinho.get_ext (ent, "PELICANU") -- procura pelo marcador extendido
    if #ext > 2 then
      local tipo = ext[2]:upper() -- tipo de elemento PELICAnU (muda p/ maiusculo)
      if tipo == "TERMINAL" then
        --identificacao do terminal
        local t_id = ext[3]:upper() -- dado especifico do tipo (muda p/ maiusculo)
        
        local terminal = conexoes[t_id]
        if not terminal then terminal = {} end
        local figura = cadzinho.get_ent_typ(ent)
        -- soh aceita linha ou circulo como terminal
        if figura == 'LINE' then
          local dados = {}
          dados.tipo = 'linha'
          --pega os pontos da linha
          local pontos = cadzinho.get_points(ent)
          for _, pt in ipairs(pontos) do
            -- executa a trasformações geometricas, conforme o componente
            rotacao(pt, dados_comp.rot)
            pt.x = pt.x * dados_comp.scale.x
            pt.y = pt.y * dados_comp.scale.y
            
            pt.x = pt.x + dados_comp.pt.x
            pt.y = pt.y + dados_comp.pt.y
          end
          
          dados.linha = pontos
          terminal[#terminal + 1] = dados
        elseif figura == 'CIRCLE' then
          local dados = {}
          dados.tipo = 'circulo'
          -- pega os parametros do circulo
          local circulo = cadzinho.get_circle_data(ent)
          -- executa a trasformações geometricas, conforme o componente
          rotacao(circulo.center, dados_comp.rot)
          circulo.center.x = circulo.center.x * dados_comp.scale.x
          circulo.center.y = circulo.center.y * dados_comp.scale.y
          circulo.radius = math.abs(circulo.radius * dados_comp.scale.x)
          circulo.center.x = circulo.center.x + dados_comp.pt.x
          circulo.center.y = circulo.center.y + dados_comp.pt.y
          
          dados.circulo = circulo
          terminal[#terminal + 1] = dados
        end
        
        conexoes[t_id] = terminal
      end
    end
  end
  
  return conexoes
end

function info_terminais (comp)
-- obtem as informacoes textuais dos terminais de um componente
-- saida: par indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1
  local terminais = {}
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "T*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local t_num = string.match(attr['tag']:upper(), "^T(%d)")
    if t_num then
      terminais[tonumber(t_num)] = attr['value']
    end
  end
  
  return terminais
end

function muda_terminais (comp, terminais)
-- altera as informacoes textuais dos terminais de um componente
-- entrada: tabela com pares indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1

  local ocul = false
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "T*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local t_num = string.match(attr['tag']:upper(), "^T(%d)")
    if t_num then
      -- confronta o indice encontrado com a tabela
      local term = terminais[tonumber(t_num)]
      if term then
        -- modifica o indice encontrado
        ocul = attr['hidden'] -- mantem a configuração de "oculto"
        cadzinho.edit_attr(comp, i, 'T' .. t_num, term, ocul)
      end
    end
  end
end

function info_descricoes (comp)
-- obtem as informacoes textuais das descrições de um componente
-- saida: par indice-texto da descrição, onde o indice eh um numero inteiro comecando em 1
  local descr = {}
  
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "DESCR*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local d_num = string.match(attr['tag']:upper(), "^DESCR(%d)")
    if d_num then
      descr[tonumber(d_num)] = attr['value']
    end
  end
  
  return descr
end

function muda_descricoes (comp, descricoes)
-- altera as informacoes textuais dos descricoes de um componente
-- entrada: tabela com pares indice-texto do terminal, onde o indice eh um numero inteiro comecando em 1

  local ocul = false
  -- varre os elementos ATTRIB da entidade, buscando as etiquetas "T*"
  local attrs = cadzinho.get_attribs(comp)
  for i, attr in ipairs(attrs) do
    local d_num = string.match(attr['tag']:upper(), "^DESCR(%d)")
    if d_num then
      -- confronta o indice encontrado com a tabela
      local descr = descricoes[tonumber(d_num)]
      if descr then
        -- modifica o indice encontrado
        ocul = attr['hidden'] -- mantem a configuracao de "oculto"
        cadzinho.edit_attr(comp, i, 'DESCR' .. d_num, descr, ocul)
      end
    end
  end
end

function obtem_caixas()
  local caixas = {}
  local SetLib = require("Set") -- biblioteca para matematica de conjuntos
  local caixa
  local conteudo
  
  -- atualiza os identificadores unicos, para evitar elementos repetidos (com mesmo id)
  --atualiza_unicos()
  -- atualiza a lista principal com os elementos
  --atualiza_elems()
  
  -- caixa "ARQUIVO" para armazenar os elementos órfãos
  caixa = {}
  conteudo = conteudo_todo()
  caixa['nome'] = ""
  caixa['conteudo'] = SetLib.new(conteudo)
  caixa['filhas'] = {}
  caixa['tipo'] = 'ARQUIVO'
  caixas[0] = caixa
  
  -- varre os elementos, cadastrando as caixas existentes no desenho
  for el_id, el in pairs(elems_pelicanu) do
    if el.tipo == "CAIXA" then
      caixa = {}
      conteudo = pega_conteudo(el_id)
      caixa['nome'] = ""
      caixa['conteudo'] = SetLib.new(conteudo)
      caixa['filhas'] = {}
      caixa['tipo'] = el.esp
      caixas[el_id] = caixa
    end
  end
  
  -- repassa as caixas, cadastrando as caixas aninhadas
  for _, caixa in pairs(caixas) do
    for el_id in pairs(caixa.conteudo) do
      local el = elems_pelicanu[el_id]
      if el.tipo == "CAIXA" then
        caixa.filhas[#caixa.filhas+1] = caixas[el_id]
      end
    end
  end
  
  -- com as caixas cadastradas, os conteúdos estarao sobrepostos ("repetidos")
  -- ajusta os conteudos, ficando cada caixa com seus elementos exclusivos
  for _, caixa in pairs(caixas) do
    for _,filha in ipairs(caixa.filhas) do
      caixa.conteudo = caixa.conteudo - filha.conteudo
    end
  end
  
  -- por fim, pega o nome de cada caixa
  for id, caixa in pairs(caixas) do
    caixa.nome = rotulo_caixa(caixa.conteudo)
  end
  
  return caixas
end

function obtem_barras (desenhos, caixas)
  local componentes = {}
  local fios = {}
  local barras = {}
  local fila = nova_fila()
  
  for id, desenho in pairs(desenhos) do
    -- insere a desenho na fila de caixas
    desenho.id = id
    fila.insere(desenho)
  end
  
  -- processa a fila de caixas
  local caixa = fila.remove()
  while caixa do
    -- varre o conteúdo do caixa corrente
    for el_id, _ in pairs(caixa.conteudo) do
      local el = elems_pelicanu[el_id]
      
      if el.tipo == "CAIXA" then
        local sub_caixa = caixas[el_id]
        if sub_caixa then
          -- insere a sub_caixa na fila de caixas
          sub_caixa.id = el_id
          fila.insere(sub_caixa)
        end
      -- cadastra os componentes e os fios
      elseif el.tipo == "COMPONENTE" then
        local componente = {}
        componente.el = el
        componente.term =  pega_terminais (el.ent)
        componente.conexoes = {}
        componente.bloco = cadzinho.get_blk_name (el.ent)
        componentes[el_id] = componente
      elseif el.tipo == "LIGACAO" then
        local fio = {}
        fio.el = el
        fio.pontos = cadzinho.get_points(el.ent)
        fios[el_id] = fio
      end
      
    end
    -- pega a próxima caixa da fila
    caixa = fila.remove()
  end
  
  -- varre os componentes para cadastrar as conexoes dos terminais e fios
  for comp_id, componente in pairs(componentes) do
    for t_id, terminal in pairs(componente.term) do
      for l_id, fio in pairs(fios) do
        if term_conect (terminal, fio.pontos) then
          local conexoes ={}
          if componente.conexoes[t_id] then
            conexoes = componente.conexoes[t_id]
          end
          conexoes[#conexoes + 1] = l_id
          componente.conexoes[t_id] = conexoes
        end
      end
    end
  end
  
  for comp_id, componente in pairs(componentes) do
    for t_id, cnx in pairs(componente.conexoes) do
      local fio_ant = false
      for i, fio_id in ipairs(cnx) do
        local fio = fios[fio_id]
        if fio_ant then
          -- adiciona o fio a barra existente (mesmo terminal)
          fio.barra = fio_ant.barra
          fio.barra.fios[#fio.barra.fios+1] = fio
        else
          if fio.barra then
            -- adiciona o terminal a barra existente (fio ligado a outro componente)
            local terminal = {comp = comp_id, term = t_id}
            fio.barra.terminais[#fio.barra.terminais+1] = terminal
          else
            -- Nova barra
            fio.barra = {}
            fio.barra.fios = {fio}
            fio.barra.terminais = {{comp = comp_id, term = t_id}}
            barras[fio.barra] = fio.barra
          end
        end
        fio_ant = fio
      end
    end
  end
  
  for fio_id1, fio1 in pairs(fios) do
    for fio_id2, fio2 in pairs(fios) do
      if fio1 ~= fio2 and linha_conect (fio1.pontos, fio2.pontos) then
        if fio1.barra and not fio2.barra then
          fio1.barra.fios[#fio1.barra.fios+1] = fio2
          fio2.barra = fio1.barra
        elseif not fio1.barra and fio2.barra then
          fio2.barra.fios[#fio2.barra.fios+1] = fio1
          fio1.barra = fio2.barra
        elseif fio1.barra and fio2.barra and fio1.barra ~= fio2.barra then
          barras[fio2.barra] = nil
          -- transfere os fios e terminais
          for _, fio in pairs(fio2.barra.fios) do
            fio1.barra.fios[#fio1.barra.fios+1] = fio
          end
          for _, fio in pairs(fio2.barra.terminais) do
            fio1.barra.terminais[#fio1.barra.terminais+1] = fio
          end
          for _, fio in pairs(fio1.barra.fios) do
            fio.barra = fio1.barra
          end
        elseif not fio1.barra and not fio2.barra then
          -- Nova barra
          fio1.barra = {}
          fio1.barra.fios = {fio1, fio2}
          fio1.barra.terminais = {}
          fio2.barra = fio1.barra
          barras[fio1.barra] = fio1.barra
        end
      end
    end
  end
  
  return barras
end


function obtem_desenhos (caixas, tipos)
  local desenhos = {}
  if type(tipos) ~= 'table' then return desenhos end
  
  --local caixas = obtem_caixas()
  for id, caixa in pairs(caixas) do
    for el_id, _ in pairs(caixa.conteudo) do
      local el = elems_pelicanu[el_id]
      if el.tipo == "CAIXA" then
        local sub_caixa = caixas[el_id]
        if sub_caixa then
          if sub_caixa.tipo == "DESENHO" then
            local dados = pega_attrib(el.ent)
            local des_tipo = string.upper(dados.TIPO)
            for _, tipo in pairs(tipos) do
              if string.find(des_tipo, tipo) then
                desenhos[el_id] = sub_caixa
              end
            end
          end
        end
      end
    end
  end
  return desenhos
end

--[[
function grava_pl_comp_ant ()
  local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  if not exists (aux) then
    if not cria_pasta(aux) then
      return false
    end
  end
  
  local cam_pl = aux .. "componentes.xlsx"
  
  -- cria o arquivo de planilha
  if exists (cam_pl) then
    if not os.remove(cam_pl) then -- tenta deletar o arquivo existente
      return false
    end
  end
  
  local planilha = excel:new(cam_pl)
  local aba = planilha:add_worksheet("Componentes")
  aba:protect("", {["objects"] = true, ["scenarios"] = true}) -- aba protegida
  local protegido = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    })
  local desprotegido = planilha:add_format({locked = false, border = 1})
  
  local m_p = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    valign = "vcenter",
    })
  local m_d = planilha:add_format({locked = false, valign = "vcenter", border = 1})
  
  -- tamanho das colunas
  aba:set_column(0, 0, 20)
  aba:set_column(1, 1, 11)
  aba:set_column(2, 2, 20)
  aba:set_column(3, 4, 11)
  aba:set_column(5, 5, 18)
  aba:set_column(6, 6, 4)
  aba:set_column(7, 7, 11)
  
  -- primeira linha de titulo
  local tit_p = planilha:add_format({
    border = 6,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    bold = true
    })
  local tit_d = planilha:add_format({locked = false, bold = true, border = 6})
  aba:write(0, 0, 'ID Unico', tit_p)
  aba:write(0, 1, 'Painel', tit_p)
  aba:write(0, 2, 'Componente', tit_p)
  aba:write(0, 3, 'Modulo', tit_p)
  aba:write(0, 4, 'Parte', tit_d)
  aba:write(0, 5, 'Tipo', tit_p)
  aba:write(0, 6, 'T id', tit_p)
  aba:write(0, 7, 'Terminal', tit_d)
  
  local lin = 1
  
  -- variaveis para agupamento (merge) das celulas repetidas
  local ini_comp = 1
  local ini_unico = 1
  local ini_modulo = 1
  local ini_painel = 1
  local comp_ant = false
  local tipo_ant = false
  local unico_ant = false
  local parte_ant = false
  local modulo_ant = nil
  local painel_ant = nil
  
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then return false end
  
  for linha in bd:cols('select * from comp_term') do -- para cada linha do BD
    -- grava na planilha cada celula separada, a principio
    aba:write(lin, 0, string.format('%X', linha.unico), protegido)
    aba:write(lin, 1, linha. painel, protegido)
    aba:write(lin, 2, linha.componente, protegido)
    aba:write(lin, 3, linha.modulo, protegido)
    if linha.parte then
      aba:write(lin, 4, linha.parte, desprotegido)
    else aba:write(lin, 4, linha.parte, protegido) end
    aba:write(lin, 5, linha.tipo, protegido)
    aba:write(lin, 6, linha.num, protegido)
    aba:write(lin, 7, linha.terminal, desprotegido)
    
    -- agrupa as celulas repetidas
    if painel_ant ~= linha.painel then
      if (lin - ini_painel) > 1 then
        aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
      end
      ini_painel = lin
    end
    if comp_ant ~= linha.componente then
      if (lin - ini_comp) > 1 then
        aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
      end
      ini_comp = lin
    end
    if modulo_ant ~= linha.modulo or comp_ant and comp_ant ~= linha.componente then
      if (lin - ini_modulo) > 1 then
        aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
      end
      ini_modulo = lin
    end
    if unico_ant ~= linha.unico then -- o ID unico eh o criterio para agrupar blocos e partes
      if (lin - ini_unico) > 1 then
        aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
        aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
        if parte_ant then
          aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
        else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
      end
      ini_unico = lin
    end
    
    comp_ant = linha.componente
    unico_ant = linha.unico
    tipo_ant = linha.tipo
    parte_ant = linha.parte
    modulo_ant = linha.modulo
    painel_ant = linha.painel
    
    -- proxima linha
    lin = lin + 1
  end
  -- finaliza os agrupamentos, se necessario
  if (lin - ini_painel) > 1 then
    aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
  end
  if (lin - ini_comp) > 1 then
    aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
  end

  if (lin - ini_modulo) > 1 then
    aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
  end
  
  if (lin - ini_unico) > 1 then
    aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
    aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
    if parte_ant then
      aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
    else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
  end
  --
  bd:close()
  
  planilha:close()
  return true
end
]]--

function estima_tipo_comp (nome, elementos)
  local tipo = 0
  
  for el in string.gmatch(elementos, "[^;]+") do
    if el == 'DIODO' then
      tipo = tipo | 1
    elseif el == 'BORNE_SEC' or el == 'BORNE' then
      tipo = tipo | 2
    elseif el == 'MINI_DISJ' then
      tipo = tipo | 4
    elseif el == 'CT_TENSAO' then
      tipo = tipo | 8
    elseif el == 'CT_CORR' then
      tipo = tipo | 16
    elseif el == 'CONTATO_NA' or el == 'CONTATO_NF' then
      tipo = tipo | 32
      
    elseif el == 'BOBINA' then
      tipo = tipo | 64
    
    elseif el == 'ALIM_CC' then
      tipo = tipo | 128
    
    elseif el == 'ENT_DIG' or el == 'SAIDA_DIG' then
      tipo = tipo | 256
    end
  end
  
  local ret = 'DESCONHECIDO'
  if tipo == 0 then
    ret = 'DESCONHECIDO'
  elseif tipo == 1 then
    ret = 'DIODO'
  elseif tipo == 2 then
    ret = 'RÉGUA'
  elseif tipo == 4 or tipo == 20 then
    ret = 'MINI DJ'
  elseif tipo == 8 then
    ret = 'CT TENSÃO'
  elseif tipo == 16 then
    ret = 'CT CORRENTE'
  elseif tipo == 24 then
    ret = 'CT TENSÃO+CORRENTE'
  elseif tipo == 32 then
    ret = 'CONTATO EXTERNO'
  elseif tipo >= 64 and tipo < 128 then
    ret = 'RELÉ AUX'
    if string.find(nome, '94') then
      ret = 'RELÉ TRIP'
    elseif string.find(nome, '83') then
      ret = 'RELÉ BIEST CTRL'
    elseif string.find(nome, '86') then
      ret = 'RELÉ BIEST BLOQ'
    end
  elseif tipo >= 128 and tipo <= 417 then
    ret = 'IED'
    if string.find(nome, '21') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '87') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '51') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '67') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '50') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '59') then
      ret = 'IED RELÉ PROT'
    elseif string.find(nome, '61') then
      ret = 'IED RELÉ PROT'
    end
  else
    ret = 'ERRO'
  end
  
  return ret
end

function grava_pl_comp ()
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then -- erro na abertura do bd
    return false
  end
  
  local cor = 1
  
  -- verifica se há necessidade de criar a pasta de saída
  local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  if not exists (aux) then
    if not cria_pasta(aux) then
      return false -- erro com a pasta de saída
    end
  end
  
  -- cria o arquivo de planilha
  local cam_pl = aux .. "componentes.xlsx"
  if exists (cam_pl) then -- se há um arquivo antigo
    if not os.remove(cam_pl) then -- tenta deletar o arquivo existente
      return false -- erro ao apagar o arquivo antigo
    end
  end
  
  local planilha = excel:new(cam_pl)
  if not planilha then return false end -- erro na criacao da planilha
  
  -- cria a primeira aba, listando os painéis
  local aba_paineis = planilha:add_worksheet("Painéis")
  --aba_paineis:protect("", {["objects"] = true, ["scenarios"] = true}) -- aba protegida
  local protegido = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    })
  local desprotegido = planilha:add_format({locked = false, border = 1})
  
  local m_p = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    valign = "vcenter",
    })
  local m_d = planilha:add_format({locked = false, valign = "vcenter", border = 1})
  
  -- tamanho das colunas
  aba_paineis:set_column(0, 0, 20)
  aba_paineis:set_column(1, 1, 40)
  aba_paineis:set_column(2, 2, 60)
  aba_paineis:set_column(3, 3, 11)
  aba_paineis:set_column(4, 5, 18)
  
  -- primeira linha como titulo
  local tit_p = planilha:add_format({
    border = 6,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    bold = true
    })
  local tit_d = planilha:add_format({locked = false, bold = true, border = 6})
  aba_paineis:write(0, 0, 'Painel', tit_p)
  aba_paineis:write(0, 1, 'Título', tit_p)
  aba_paineis:write(0, 2, 'Descr.', tit_p)
  aba_paineis:write(0, 3, 'Interno', tit_p)
  aba_paineis:write(0, 4, 'X', tit_p)
  aba_paineis:write(0, 5, 'Y', tit_p)
  aba_paineis:set_tab_color('red')
  
  atualiza_db_paineis(bd)
  
  -- grava na planilha a lista atualizada de painéis
  local lin = 1
  local paineis = {}
  for linha in bd:cols('SELECT * FROM paineis ORDER BY id') do -- para cada linha do BD
    -- grava na planilha cada celula separada, a principio
    aba_paineis:write(lin, 0, linha.id, protegido)
    aba_paineis:write(lin, 1, linha.titulo, desprotegido)
    aba_paineis:write(lin, 2, linha.descr, desprotegido)
    if linha.fiacao > 0 then
      aba_paineis:write(lin, 3, 'X', desprotegido)
    else
      aba_paineis:write(lin, 3, '', desprotegido)
    end
    aba_paineis:write(lin, 4, linha.x, desprotegido)
    aba_paineis:write(lin, 5, linha.y, desprotegido)
    
    local painel = {}
    -- adiciona uma aba para cada painel
    painel.aba = planilha:add_worksheet(linha.id)
    painel.aba:set_tab_color(cores_claras[cor])
    painel.aba:set_column(0, 0, 20)
    painel.aba:set_column(1, 1, 25)
    local p_fmt = planilha:add_format({
      border = 6,
      locked = true,
      pattern = 1,
      bg_color = cores_claras[cor],
      bold = true
      })
    
    -- linha de identificação do painel
    painel.aba:write(0, 0, 'PAINEL:', p_fmt)
    painel.aba:write(0, 1, linha.id, p_fmt)
    -- linha de título da aba
    painel.aba:write(1, 0, 'Componente', tit_p)
    painel.aba:write(1, 1, 'Tipo (estimado)', tit_p)
    painel.aba:write(1, 2, 'Item LE', tit_p)
    painel.aba:write(1, 3, 'Id Fiação', tit_p)
    painel.lin = 2
    
    paineis[linha.id] = painel
    -- proxima linha
    lin = lin + 1
    if cor >= 8 then
      cor = 1
    else
      cor = cor + 1
    end
  end
  
  atualiza_db_componentes(bd)
  
  local alerta_fmt = planilha:add_format({locked = false, border = 1, bg_color = 'yellow'})

  local erro_fmt = planilha:add_format({locked = false, border = 1, bg_color = 'red'})
  print('planilha')
  -- grava a lista de componentes na planilha
  for linha in bd:cols(
    'SELECT * FROM componentes ORDER BY painel, id, tipo') do
    
    -- grava na planilha correspondente ao painel
    local painel = paineis[linha.painel]
    
    painel.aba:write(painel.lin, 0, linha.id, protegido)
    
    local fmt = desprotegido
    if linha.tipo == 'ERRO' then
      fmt = erro_fmt
    elseif linha.tipo == 'CONTATO EXTERNO' then
      fmt = alerta_fmt
    end
    painel.aba:write(painel.lin, 1, linha.tipo, fmt)
    
    fmt = desprotegido
    painel.aba:write(painel.lin, 2, linha.item, fmt)
    
    fmt = desprotegido
    --[[
    if linha.tipo == 'IED' then 
      fmt = alerta_fmt
    elseif linha.tipo == 'IED RELÉ PROT' then
      fmt = alerta_fmt
    end]]--
    painel.aba:write(painel.lin, 3, '', fmt)
    
    -- proxima linha
    paineis[linha.painel].lin = paineis[linha.painel].lin + 1
  end
  
  --
  bd:close()
  
  planilha:close()
  return true
end



function grava_pl_term (num_autom)
  local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  if not exists (aux) then
    if not cria_pasta(aux) then
      return false
    end
  end
  
  local cam_pl = aux .. "terminais.xlsx"
  
  -- cria o arquivo de planilha
  if exists (cam_pl) then
    if not os.remove(cam_pl) then -- tenta deletar o arquivo existente
      return false
    end
  end
  
  local planilha = excel:new(cam_pl)
  local aba = planilha:add_worksheet("Terminais")
  aba:protect("", {["objects"] = true, ["scenarios"] = true}) -- aba protegida
  local protegido = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    })
  local desprotegido = planilha:add_format({locked = false, border = 1})
  
  local m_p = planilha:add_format({
    border = 1,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    valign = "vcenter",
    })
  local m_d = planilha:add_format({locked = false, valign = "vcenter", border = 1})
  
  -- tamanho das colunas
  aba:set_column(0, 0, 20)
  aba:set_column(1, 1, 11)
  aba:set_column(2, 2, 20)
  aba:set_column(3, 4, 11)
  aba:set_column(5, 5, 18)
  aba:set_column(6, 6, 4)
  aba:set_column(7, 8, 11)
  
  -- primeira linha de titulo
  local tit_p = planilha:add_format({
    border = 6,
    locked = true,
    pattern = 1,
    bg_color = '#D8E4BC',
    bold = true
    })
  local tit_d = planilha:add_format({locked = false, bold = true, border = 6})
  aba:write(0, 0, 'ID Unico', tit_p)
  aba:write(0, 1, 'Painel', tit_p)
  aba:write(0, 2, 'Componente', tit_p)
  aba:write(0, 3, 'Modulo', tit_d)
  aba:write(0, 4, 'Parte', tit_d)
  aba:write(0, 5, 'Tipo', tit_p)
  aba:write(0, 6, 'T id', tit_p)
  aba:write(0, 7, 'Terminal', tit_d)
  
  local lin = 1
  
  -- variaveis para agupamento (merge) das celulas repetidas
  local ini_comp = 1
  local ini_unico = 1
  local ini_modulo = 1
  local ini_painel = 1
  local comp_ant = false
  local tipo_ant = false
  local unico_ant = false
  local parte_ant = false
  local modulo_ant = nil
  local painel_ant = nil
  
  local alerta_fmt = planilha:add_format({locked = false, border = 1, bg_color = 'yellow'})
  local erro_fmt = planilha:add_format({locked = false, border = 1, bg_color = 'red'})
  local fmt = desprotegido
  
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then return false end
  
  
  
  --local cmd = 'select * from comp_term'
  local cmd = "select unico, painel, comp componente, modulo, parte, bloco, tipo,\n" ..
    "t_num num, terminal, desenho, fl from comp_term_esq\n" ..
    "ORDER BY painel ASC, componente ASC, modulo ASC, parte asc, tipo ASC,\n" ..
    "desenho ASC, fl asc, x ASC, y DESC, unico asc, num asc;"
  if num_autom then
    cmd = "SELECT tip.unico, tip.painel, tip.componente,\n"..
      "CASE WHEN tip.tipo = 'BORNE_SEC' THEN tip.num ELSE tip.modulo END modulo,\n"..
      "tip.parte, tip.tipo, tip.item, tip.t_id num,\n"..
      "CASE WHEN tip.term IS NOT NULL THEN tip.term ELSE \n"..
      "(CASE WHEN tipo = 'BORNE' THEN num \n"..
      "WHEN tipo = 'BORNE_SEC' THEN  (CASE WHEN tip.t_id = 1 THEN 'E' ELSE 'S' END)\n"..
      "ELSE num || t_id END) END terminal,\n"..
      "CASE WHEN item IS NOT NULL AND tip.term IS NULL AND \n"..
      "NOT tipo = 'BORNE' THEN 'Incompleto' ELSE (CASE WHEN NOT tipo = 'BORNE' AND \n"..
      "NOT tipo = 'BORNE_SEC' AND tip.term IS NULL THEN 'Sem tipico' END) END alerta\n"..
      "FROM tipico_aplic tip;"
  end
  for linha in bd:cols(cmd) do -- para cada linha do BD
    -- grava na planilha cada celula separada, a principio
    aba:write(lin, 0, string.format('%X', linha.unico), protegido)
    aba:write(lin, 1, linha. painel, protegido)
    aba:write(lin, 2, linha.componente, protegido)
    if linha.tipo == 'BORNE_SEC' then
      aba:write(lin, 3, linha.modulo, desprotegido)
    else
      aba:write(lin, 3, linha.modulo, protegido)
    end
    if linha.parte then
      aba:write(lin, 4, linha.parte, desprotegido)
    else aba:write(lin, 4, linha.parte, protegido) end
    aba:write(lin, 5, linha.tipo, protegido)
    aba:write(lin, 6, linha.num, protegido)
    aba:write(lin, 7, linha.terminal, desprotegido)
    
    if linha.alerta == 'Incompleto' then
      aba:write(lin, 8, linha.alerta, erro_fmt)
    elseif linha.alerta then
      aba:write(lin, 8, linha.alerta, alerta_fmt)
    end
    
    
    -- agrupa as celulas repetidas
    if painel_ant ~= linha.painel then
      if (lin - ini_painel) > 1 then
        aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
      end
      ini_painel = lin
    end
    if comp_ant ~= linha.componente then
      if (lin - ini_comp) > 1 then
        aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
      end
      ini_comp = lin
    end
    if modulo_ant ~= linha.modulo or comp_ant and comp_ant ~= linha.componente then
      if (lin - ini_modulo) > 1 then
        aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
      end
      ini_modulo = lin
    end
    if unico_ant ~= linha.unico then -- o ID unico eh o criterio para agrupar blocos e partes
      if (lin - ini_unico) > 1 then
        aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
        aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
        if parte_ant then
          aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
        else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
      end
      ini_unico = lin
    end
    
    comp_ant = linha.componente
    unico_ant = linha.unico
    tipo_ant = linha.tipo
    parte_ant = linha.parte
    modulo_ant = linha.modulo
    painel_ant = linha.painel
    
    -- proxima linha
    lin = lin + 1
  end
  -- finaliza os agrupamentos, se necessario
  if (lin - ini_painel) > 1 then
    aba:merge_range(ini_painel, 1, lin - 1, 1, painel_ant, m_p)
  end
  if (lin - ini_comp) > 1 then
    aba:merge_range(ini_comp, 2, lin - 1, 2, comp_ant, m_p)
  end

  if (lin - ini_modulo) > 1 then
    aba:merge_range(ini_modulo, 3, lin - 1, 3, modulo_ant, m_p)
  end
  
  if (lin - ini_unico) > 1 then
    aba:merge_range(ini_unico, 0, lin - 1, 0, string.format('%X', unico_ant), m_p)
    aba:merge_range(ini_unico, 5, lin - 1, 5, tipo_ant, m_p)
    if parte_ant then
      aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_d)
    else aba:merge_range(ini_unico, 4, lin - 1, 4, parte_ant, m_p) end
  end
  --
  bd:close()
  
  planilha:close()
  return true
end

function le_pl_base()
  local leitor = require 'xlsx_lua'
  
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then -- erro na abertura do bd
    return false
  end
  
  -- caminho da planilha
  local arq = format_dir(projeto.caminho) .. '_aux'.. fs.dir_sep .. 'base.xlsm'
  
  local pl_term = false
  local pl_equip = false
  local pl_regras = false
  -- le o arquivo excel
  local workbook = leitor.open(arq)
  -- procura pelas abas 'Equipamentos' e 'Terminais'
  if type(workbook) == 'table' then 
    pl_equip = workbook.sheets['Equipamentos']
    pl_term = workbook.sheets['Terminais']
    pl_regras = workbook.sheets['Regras']
  else
    return nil -- erro na abertura do arquivo excel
  end
  
  -- verifica a existência das abas
  if type(pl_equip) ~= 'table' or type(pl_term) ~= 'table' then return nil end
  
  -- limpa o banco de dados
  bd:exec('DROP TABLE IF EXISTS lista_equip')
  bd:exec('CREATE TABLE lista_equip('..
    'item TEXT, descr TEXT, modelo TEXT, fabr TEXT)')
  bd:exec('DROP TABLE IF EXISTS tipico_term')
  bd:exec('CREATE TABLE tipico_term('..
    'item TEXT, el_id INTEGER, elemento TEXT, ' ..
    't_id INTEGER, term TEXT, modulo TEXT, fiacao INTEGER)')
  bd:exec('DROP TABLE IF EXISTS regras_equip')
  bd:exec('CREATE TABLE regras_equip('..
    'tipo TEXT, item TEXT)')
  
  -- insere os dados da aba 'Equipamentos' no banco de dados
  for lin = 2, #pl_equip.dim.rows do
    if pl_equip.data[lin]['A'] then
      if tostring(pl_equip.data[lin]['A']) ~= '' then
        bd:exec ("INSERT INTO lista_equip VALUES('"..
            tostring(pl_equip.data[lin]['A']) .."', '"..
            tostring(pl_equip.data[lin]['B']) .."', '"..
            tostring(pl_equip.data[lin]['C']) .."', '"..
            tostring(pl_equip.data[lin]['D']) .."');")
      end
    end
  end
  
  -- para a aba 'Terminais' é necessário processar as células mescladas
  local elems = {}
  
  -- varre os conjuntos mesclados
  for _, merge in ipairs(pl_term.merged) do
		-- pega o 'range' do conjunto mesclado
		local col_start, row_start, col_end, row_end = merge:match('(%a+)(%d+):(%a+)(%d+)')
		-- a primeira célula do conjunto é a que contém o valor das demais 
		local value = pl_term.data[tonumber(row_start)][col_start]
		if tonumber(row_start) > 1 then -- ignora a primeira linha, como título
      -- gera a tabela de elementos
      if col_start == 'B' and col_end == 'B' then -- elementos na coluna 'B'
        elems[tonumber(row_start)] = 1
      end
    end
	end
  
  -- expande as celulas mescladas (replica o valor pra todas celulas do grupo)
  leitor.expand_merge (pl_term)
  
  -- grava o dados de terminais no BD
  local t_id = 1 -- identificação sequencial do terminal dentro do elemento
  local el_id = 0
  local item_ant = nil
  local mod_ant = nil
  -- varre a planilha
  local final = tonumber(pl_term.dim.rows[#pl_term.dim.rows])
  for i = 2, final do
    local item = pl_term.data[i]['A']
    local elem = pl_term.data[i]['B']
    local term = pl_term.data[i]['C']
    local mod = pl_term.data[i]['D']
    
    if mod then
      mod = "'" .. mod .. "'"
    else
      mod = 'NULL'
    end
    local mod_fia = pl_term.data[i]['E']
    if mod_fia then
      mod_fia = '1'
    else
      mod_fia = 'NULL'
    end
    
    if elems[i] then
      el_id = el_id + 1
      t_id = 1
    end
    
    if mod_ant ~= mod then
      el_id = 1
      t_id = 1
    end
    
    if item_ant ~= item then
      el_id = 1
      t_id = 1
    end
    
    if item and term then
      -- finalmente, grava no BD
      bd:exec ("INSERT INTO tipico_term VALUES('"..
        item .. "', " .. el_id .. ", '" .. elem .. "', " ..
        t_id .. ", '".. term .. "', ".. mod ..", ".. mod_fia .. ");")
    end
    
    mod_ant = mod
    item_ant = item
    t_id = t_id + 1
  end
  
  final = tonumber(pl_regras.dim.rows[#pl_regras.dim.rows])
  for i = 2, final do
    local tipo = pl_regras.data[i]['A']
    local item = pl_regras.data[i]['B']
    
    if item then
      item = "'" .. item .. "'"
    else
      item = 'NULL'
    end
    
    if tipo then
      bd:exec ("INSERT INTO regras_equip VALUES('"..
        tipo .. "', " .. item ..  ");")
      
      -- forca a atualização das regras nos componenetes
      bd:exec ("UPDATE componentes SET item = "..
        item .. " WHERE tipo = '" .. tipo ..  "';")
    end
  end
  
  
  atualiza_db_componentes(bd)
  
  
  bd:close()
  
  return true
end

--[[function le_pl_comp()
  cadzinho.db_print ("----- Atualiza componentes  ------")
  local leitor = require 'xlsx_lua'
  
  local pl_comp = false
  -- le o arquivo excel
  local workbook = leitor.open('componentes.xlsx')
  -- procura pela aba 'Componentes'
  if type(workbook) == 'table' then pl_comp = workbook.sheets['Componentes'] end
  
  if type(pl_comp) == 'table' then
    -- expande as celulas mescladas (replica o valor pra todas celulas do grupo)
    leitor.expand_merge (pl_comp)
    
    -- abre o banco de dados
    local bd = sqlite.open('pelicanu.db')
    
    -- atualiza a lista principal com os elementos
    atualiza_elems()
    
    -- cria uma lista com os componentes
    local componentes = {}
    for el_id, el in pairs(elems_pelicanu) do
      if el.tipo == "COMPONENTE" then
        local componente = {}
        componente.ent = el.ent
        componente.id = pega_comp_id(el.ent)
        componente.term =  info_terminais (el.ent)
        componentes[el_id] = componente
      end
    end
    
    -- varre a planilha
    for _, lin in ipairs(pl_comp.dim.rows) do
      if lin ~= 1 then -- ignora a primeira linha com o titulo
        local unico = tonumber(pl_comp.data[lin]['A'],16) -- coluna A eh o id unico
        -- atualiza o banco de dados com as informacoes lidas
        bd:exec("UPDATE terminais_esq SET terminal = '" ..
          tostring(pl_comp.data[lin]['H']) ..
          "' WHERE componente = " .. string.format('%d', unico) ..
          " AND id = " .. tostring(pl_comp.data[lin]['G']) .. ";")

        if pl_comp.data[lin]['E'] then
          bd:exec("UPDATE componentes_esq SET id = '" ..
          tostring(pl_comp.data[lin]['E']) ..
          "' WHERE unico = " .. string.format('%d', unico) .. ";")
        end
        -- atualiza na lista de componentes
        local componente = componentes[unico]
        if type(componente) == 'table' then
          if pl_comp.data[lin]['E'] then
            componente.id = tostring(pl_comp.data[lin]['E'])
          end
          componente.term[tonumber(pl_comp.data[lin]['G'])] = tostring(pl_comp.data[lin]['H'])
        end
      end
    end
    
    -- atualiza no desenho
    for _, componente in pairs(componentes) do
      muda_comp_id (componente.ent, componente.id)
      muda_terminais(componente.ent, componente.term)
      componente.ent:write()
    end
    
    bd:close()
  end
  
  cadzinho.db_print ("----- Concluido  ------")
end]]--

function terminais_pl_bd()
  local leitor = require 'xlsx_lua'
  local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  local cam_pl = aux .. "terminais.xlsx"
  
  local pl_term = false
  -- le o arquivo excel
  local workbook = leitor.open(cam_pl)
  -- procura pela aba 'Componentes'
  if type(workbook) == 'table' then pl_term = workbook.sheets['Terminais']
  else return false end
  
  if type(pl_term) ~= 'table' then return false end
  
  -- expande as celulas mescladas (replica o valor pra todas celulas do grupo)
  leitor.expand_merge (pl_term)
  
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then -- erro na abertura do bd
    return false
  end
    
  -- varre a planilha
  for _, lin in ipairs(pl_term.dim.rows) do
    if lin ~= 1 then -- ignora a primeira linha com o titulo
      local unico = tonumber(pl_term.data[lin]['A'],16) -- coluna A eh o id unico
      -- atualiza o banco de dados com as informacoes lidas
      bd:exec("UPDATE terminais_esq SET terminal = '" ..
        tostring(pl_term.data[lin]['H']) ..
        "' WHERE componente = " .. string.format('%d', unico) ..
        " AND id = " .. tostring(pl_term.data[lin]['G']) .. ";")

      if pl_term.data[lin]['E'] then
        bd:exec("UPDATE componentes_esq SET id = '" ..
        tostring(pl_term.data[lin]['E']) ..
        "' WHERE unico = " .. string.format('%d', unico) .. ";")
      end
      
      if pl_term.data[lin]['F'] == 'BORNE_SEC' then
        bd:exec("UPDATE componentes_esq SET sub = '" ..
        tostring(pl_term.data[lin]['D']) ..
        "' WHERE unico = " .. string.format('%d', unico) .. ";")
      end
    end
  end
  
  -- atualiza o comp_term
  atualiza_comp_term(bd)
  
  bd:close()
  
  return true
 
end

function atualiza_comp (bd, cam)
  local cmd =  "SELECT t.unico, t.parte, t.tipo, t.num, t.terminal, " ..
    "t.modulo FROM comp_term t INNER JOIN " ..
    "componentes_esq c ON t.unico = c.unico " ..
    "WHERE c.arquivo = '" .. cam .. "';"
  
  local comps = {}
  
  for linha in bd:cols(cmd) do -- para cada linha do BD
  
    local unico = tonumber(linha.unico)
    if not comps[unico] then
      comps[unico] = {}
      comps[unico].terms = {}
    end
    local terms = comps[unico].terms
    terms['T' .. linha.num] = linha.terminal
    if linha.tipo == 'BORNE_SEC' and linha.modulo then
      comps[unico].parte = nil
      terms['SUB'] = linha.modulo
    else
      comps[unico].parte = linha.parte
    end
    
  end
  
  for unico, comp in pairs(comps) do
    local el = elems_pelicanu[unico]
    if comp.parte then
      muda_comp_id (el.ent, comp.parte)
    end
    
    muda_atrib (el.ent, comp.terms)
    el.ent:write()
  end
  
end

function atualiza_ref(bd, cam)
  
  local cmd = "SELECT r.unico, des.projeto, des.titulo, " ..
    "CASE WHEN r.des_ref = c.desenho THEN 'NESTE' " ..
    "ELSE c.desenho END desenho, " ..
    "CASE WHEN r.des_ref = c.desenho AND " ..
    "r.fl_ref = c.fl THEN 'NESTA FL.' ELSE c.fl END fl " ..
    "FROM (SELECT rf.unico, comp.painel, comp.componente, " ..
    "comp.modulo, comp.arquivo, rf.elemento, rf.terminal, " ..
    "comp.desenho des_ref, comp.fl fl_ref " ..
    "FROM referencias_esq rf " ..
    "LEFT JOIN descr_comp comp ON rf.unico = comp.unico " ..
    "ORDER BY painel, componente, modulo, terminal) r " ..
    "LEFT JOIN (SELECT DISTINCT dc.unico, dc.painel, " ..
    "dc.componente, dc.modulo, dc.tipo, dc.desenho, " ..
    "dc.fl, group_concat(t.terminal, '-') OVER " ..
    "(PARTITION BY t.componente) terms " ..
    "FROM descr_comp dc " ..
    "INNER JOIN (SELECT componente, terminal  " ..
    "FROM terminais_esq " ..
    "ORDER BY componente, terminal) t " ..
    "ON dc.unico = t.componente) c " ..
    "ON r.painel = c.painel AND r.elemento = c.tipo " ..
    "AND r.componente = c.componente " ..
    "AND (r.modulo = c.modulo " ..
    "OR (r.modulo IS NULL AND c.modulo IS NULL)) " ..
    "AND r.terminal LIKE '%' || c.terms || '%' " ..
    "LEFT JOIN desenhos des " ..
    "ON c.desenho = des.ident AND c.fl = des.fl " ..
    "WHERE r.arquivo = '" .. cam .. "';"
    
  for linha in bd:cols(cmd) do -- para cada linha do BD
    local unico = tonumber(linha.unico)
    local el = elems_pelicanu[unico]
    local dados = {APLIC = '-', DESENHO = '-'}
    if linha.desenho and linha.fl then
      if linha.fl == 'NESTA FL.' then
        dados.DESENHO = linha.fl
      else
        dados.DESENHO = linha.desenho .. ' FL.' .. linha.fl
      end
    end
    if linha.projeto and linha.titulo then
      dados.APLIC = linha.projeto .. ' ' .. linha.titulo
    end
    
    dados.APLIC = abrevia(dados.APLIC)
    
    muda_atrib (el.ent, dados)
    el.ent:write()
  end

end


function atualiza_engates(bd, cam)
    
  local cmd =  "SELECT e.unico, e.e_num, e.vai_des, e.vai_fl " ..
    "FROM engate_par e INNER JOIN " ..
    "componentes_esq c ON e.unico = c.unico " ..
    "WHERE c.arquivo = '" .. cam .. "';"
  
  for linha in bd:cols(cmd) do -- para cada linha do BD
    local unico = tonumber(linha.unico)
    local el = elems_pelicanu[unico]
    local num = 'E' .. linha.e_num
    local des = linha.vai_des
    local fl = linha.vai_fl
    local descr = nil
    
    if des == 'NESTE' then
      descr = ''
    end
    if fl == 'NESTA FL.' then
      des = fl
      fl = ''
    elseif fl then
      fl = 'FL. ' .. fl
    end
    muda_engate(el.ent, nil, num, des, fl, descr)
    el.ent:write()
  end
  
end


function atualiza_ref_desenho()
  -- abre e le o banco de dados
  local bd = sqlite.open(projeto.bd)
  if not bd then -- erro na abertura do bd
    return false
  end
  
  -- pega o caminho do documento atual
  local drwg, dir = cadzinho.get_drwg_path()
  
  if drwg == '' then
    return false
  end
  
  -- atualiza a lista principal com os elementos
  atualiza_elems()
  
  atualiza_comp (bd, dir .. drwg)
  atualiza_engates (bd, dir .. drwg)
  atualiza_ref (bd, dir .. drwg)
  
  
  --[[
  -- cria uma lista com as referencias, provenientes do banco de dados
  local referencias = {}
  local cmd = "SELECT r.unico, des.projeto, des.titulo, " ..
    "CASE WHEN r.des_ref = dc.desenho THEN 'NESTE' ELSE dc.desenho END desenho, " ..
    "CASE WHEN r.des_ref = dc.desenho AND r.fl_ref = dc.fl THEN 'NESTA FL.' ELSE dc.fl END fl " ..
    "FROM (SELECT rf.unico, comp.painel, comp.componente, comp.modulo, " ..
    "comp.arquivo, rf.terminal, comp.desenho des_ref, comp.fl fl_ref " ..
    "FROM referencias_esq rf " ..
    "INNER JOIN descr_comp comp ON rf.unico = comp.unico " ..
    "ORDER BY painel, componente, modulo, terminal) r " ..
    "INNER JOIN comp_term c ON r.painel = c.painel AND " ..
    "r.componente = c.componente AND (r.modulo = c.modulo " ..
    "OR (r.modulo IS NULL AND c.modulo IS NULL)) " ..
    "AND r.terminal = c.terminal " ..
    "INNER JOIN descr_comp dc ON c.unico = dc.unico " ..
    "INNER JOIN desenhos des ON dc.desenho = des.ident  " ..
    "AND dc.fl = des.fl " ..
    "WHERE r.arquivo = '" .. dir .. drwg .. "';"
  
  for linha in bd:cols(cmd) do -- para cada linha do BD
    local referencia = {}
    referencia.projeto = linha.projeto
    referencia.titulo = linha.titulo
    referencia.desenho = linha.desenho
    referencia.fl = linha.fl
    referencias[linha.unico] = referencia
  end
  
  
  for el_id, el in pairs(elems_pelicanu) do
    if el.tipo == "COMPONENTE" then
      local comp_tipo = pega_comp_tipo(el.ent)
      if comp_tipo == "REFERENCIA" then
        -- atualiza no desenho
        local dados = {aplicacao = '-', desenho = '-'}
        if type(referencias[el_id]) == 'table' then
          dados.aplicacao = referencias[el_id].projeto .. ' ' .. referencias[el_id].titulo
          dados.desenho = referencias[el_id].desenho .. ' FL.' .. referencias[el_id].fl
          if referencias[el_id].fl == 'NESTA FL.' then
            dados.desenho = 'NESTA FL.'
          end
        end
        muda_atrib (el.ent, dados)
        el.ent:write()
      end
    end
  end
  ]]--
  
  --[[
  cadzinho.save_drwg (arquivo, true)
-- grava a estrutura atual do projeto na tabela do bd
local lista_arq = lista_proj() -- obtem as informações atualizadas
atualiza_lista_arq_bd (bd, lista_arq)
  ]]--
  
  
  bd:close()
  
end

--[[function atualiza_engates()
  -- atualiza a lista principal com os elementos
  atualiza_elems()
  
  --abre o banco de dados
  local bd = sqlite.open('pelicanu.db')
  for linha in bd:cols('select * from engate_par') do -- para cada linha do BD
    local unico = tonumber(linha.unico)
    local el = elems_pelicanu[unico]
    local num = 'E' .. linha.e_num
    local des = linha.vai_des
    local fl = linha.vai_fl
    local descr = nil
    
    if des == 'NESTE' then
      descr = ''
    end
    if fl == 'NESTA FL.' then
      des = fl
      fl = ''
    elseif fl then
      fl = 'FL. ' .. fl
    end
    muda_engate(el.ent, nil, num, des, fl, descr)
    el.ent:write()
  end
  bd:close()
end]]--

function obtem_tipo_comp ()
  lista_comp_tipo = {}
  g_tipo_comp = {value = g_tipo_comp.value, ""}

  -- diretorio para buscar os componentes na biblioteca (se chama "componentes" :P)
  local dir_comp = biblioteca .. "componentes"

  -- lista os subdiretorios como tipo de componentes
  local dir = fs.dir(dir_comp)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    if dir[i].is_dir then
      lista_comp_tipo[#lista_comp_tipo+1] = dir[i].name
      g_tipo_comp[#g_tipo_comp+1] = dir[i].name
    end
  end
  if g_tipo_comp.value > #lista_comp_tipo+1 then
    g_tipo_comp.value = 1
  end
end

function obtem_lista_comp ()
  lista_comp = {}
  lista_comp_o = {}

  -- diretorio para buscar os componentes na biblioteca (se chama "componentes" :P)
  local dir_comp = biblioteca .. "componentes" ..
        fs.dir_sep .. g_tipo_comp[g_tipo_comp.value]

  local dir = fs.dir(dir_comp)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    local ext = extensao(dir[i].name)
    if type(ext) == "string" and dir[i].is_dir == false then
      if ext:upper() == ".DXF" then
        lista_comp[nome_arq(dir[i].name)] = format_dir(dir_comp) .. dir[i].name
        lista_comp_o[#lista_comp_o+1] = nome_arq(dir[i].name)
      end
    end
  end
  table.sort(lista_comp_o)

end

function obtem_lista_formato ()
  lista_formato = {}
  lista_formato_o = {}

  -- diretorio para buscar os formatos na biblioteca (se chama "formatos" :P)
  local dir_fmt = biblioteca .. "formatos"

  local dir = fs.dir(dir_fmt)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    local ext = extensao(dir[i].name)
    if type(ext) == "string" and dir[i].is_dir == false then
      if ext:upper() == ".DXF" then
        lista_formato[nome_arq(dir[i].name)] = format_dir(dir_fmt) .. dir[i].name
        lista_formato_o[#lista_formato_o+1] = nome_arq(dir[i].name)
      end
    end
  end
  table.sort(lista_formato_o)

end

function obtem_lista_esq ()
  lista_esq = {}
  lista_esq_o = {}

  -- diretorio para buscar os componentes de esquemático
  -- na biblioteca (se chama "componentes/esquematico" :P)
  local dir_comp = biblioteca .. "componentes" ..
        fs.dir_sep .. "esquematico"

  local dir = fs.dir(dir_comp)
  if not dir then return end -- sai em caso de erro
  for i = 1, #dir do
    local ext = extensao(dir[i].name)
    if type(ext) == "string" and dir[i].is_dir == false then
      if ext:upper() == ".DXF" then
        lista_esq[nome_arq(dir[i].name)] = format_dir(dir_comp) .. dir[i].name
        lista_esq_o[#lista_esq_o+1] = nome_arq(dir[i].name)
      end
    end
  end
  table.sort(lista_esq_o)

end


function tipico_itens()
  local itens = {}
  local itens_num = {}
  local bd = sqlite.open(projeto.bd)
  if not bd then return itens end
  
  local cmd = 'select distinct t.item, l.descr, l.modelo, l.fabr ' ..
    'from tipico_term t ' ..
    'left join lista_equip l '..
    'on t.item = l.item'
  for linha in bd:cols(cmd) do -- para cada linha do BD
    itens[linha.item] = linha
    itens_num[#itens_num + 1] = linha.item
  end
  
  bd:close()
  table.sort(itens_num)
  return itens_num, itens
end

function nova_ref (x, y)
  local nome_bloco = 'referencia_' .. g_ref_alt.value .. '_' ..
    g_ref_descr.value ..  '_' .. g_ref_desenho.value
    
  local ref_blc = cadzinho.new_insert(nome_bloco, x, y)
  if ref_blc == nil then
  
    --cadzinho.set_color("by block")
    --cadzinho.set_lw("by block")
    --cadzinho.set_ltype("byblock")
    
    local param = {color = "by block", lw = "by block",
      ltype = "byblock", style = "ISO"}
    
    local elems = {}
    local caixa = cadzinho.new_pline(0, 0, 0, g_ref_descr.value, 0, 0, param)
    cadzinho.pline_append(caixa, g_ref_descr.value, -g_ref_alt.value, 0)
    cadzinho.pline_append(caixa, 0, -g_ref_alt.value, 0)
    cadzinho.pline_close(caixa, true)
    elems[#elems + 1] = caixa
    
    caixa = cadzinho.new_pline(g_ref_descr.value, 0, 0,
      g_ref_descr.value + g_ref_desenho.value, 0, 0, param)
    cadzinho.pline_append(caixa, g_ref_descr.value + g_ref_desenho.value, -g_ref_alt.value, 0)
    cadzinho.pline_append(caixa, g_ref_descr.value, -g_ref_alt.value, 0)
    cadzinho.pline_close(caixa, true)
    elems[#elems + 1] = caixa
    
    local txt = cadzinho.new_text(0, 0, 
      '#*TERMINAL$??', 0.5, "left", "top", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(0, -g_ref_alt.value/2, 
      '#*ELEMENTO$CONTATO_NA', 0.5, "left", "middle", param)
    elems[#elems + 1] = txt
    
    local txt = cadzinho.new_text(0, -g_ref_alt.value, 
      '#*TIPO$REFERENCIA', 0.5, "left", "bottom", param)
    elems[#elems + 1] = txt
    
    txt = cadzinho.new_text(1, -g_ref_alt.value/2, '#APLIC$APLIC??', 2.0, 
      "left", "middle", param)
    elems[#elems + 1] = txt
    
    txt = cadzinho.new_text(g_ref_descr.value + 1, -g_ref_alt.value/2,
      '#DESENHO$DES?? fl. ??', 2.0, "left", "middle", param)
    elems[#elems + 1] = txt

    --cadzinho.set_color("by layer")
    --cadzinho.set_lw("by layer")
    --cadzinho.set_ltype("bylayer")
    
    param.color = "by layer"
    param.lw = "by layer"
    param.ltype = "bylayer"
  
  
    if cadzinho.new_block(elems, nome_bloco,
      "referencia esquematico PELICAnU ".. nome_bloco,
      true, '#', '*', '$', '?', 0, 0, 0) then

      ref_blc = cadzinho.new_insert(nome_bloco, x, y, 1, 1, 0, param)
    end
  end
  
  if ref_blc then
    muda_atrib (ref_blc, {TERMINAL = g_ref_term.value})
    cadzinho.new_appid("PELICANU") -- garante que o desenho tenha a marca do aplicativo
    cadzinho.add_ext(ref_blc, "PELICANU", 
      {cadzinho.unique_id(), "COMPONENTE"})
    return ref_blc
  end
  
  return nil
end

function retangulo (x, y, largura, altura)
  local retan = cadzinho.new_pline(x, y, 0, x + largura, y, 0)
  cadzinho.pline_append(retan, x + largura, y - altura, 0)
  cadzinho.pline_append(retan, x, y - altura, 0)
  cadzinho.pline_close(retan, true)
  return retan
end

function quadro_ref (item, x, y, so_contatos)
  local elems = {}
  
  if type(item) ~= 'string' then return elems end
  local bd = sqlite.open(projeto.bd)
  if not bd then return elems end
  
  local pos_x = x + 2 + g_ref_descr.value
  local pos_y = y - 3 * g_ref_alt.value
  
  local cmd_cmpl = ''
  if so_contatos then
    cmd_cmpl = " AND elemento LIKE 'CONTATO%' AND modulo IS NULL"
  end
  
  local cmd = "SELECT modulo, COUNT(*) elems FROM " ..
    "(SELECT DISTINCT el_id, elemento, modulo FROM tipico_term " ..
    "WHERE item = '".. item .."'" .. cmd_cmpl ..
    ") GROUP BY modulo ORDER BY modulo;"
  
  -- planeja o tamanho do quadro, por módulo
  local modulos = {}
  local total = 1
  for linha in bd:cols(cmd) do -- para cada linha do BD
    local modulo = {}
    local id = linha.modulo
    if not id then id = 1 end
    modulo.elems = linha.elems
    modulo.ini = total
    total = total + 2
    modulo.pos = total
    total = total + linha.elems
    modulos[id] = modulo
  end
  
  cmd = "SELECT distinct el_id, elemento, modulo, " ..
    "group_concat(term, '-') OVER " ..
    "(PARTITION BY el_id, modulo) terms " ..
    "FROM (SELECT el_id, elemento, modulo, term from tipico_term " ..
    "WHERE item = '".. item .."'" .. cmd_cmpl ..
    " ORDER BY el_id, modulo, term);"
  
  for linha in bd:cols(cmd) do -- para cada linha do BD
    local id = linha.modulo
    if not id then id = 1 end
    
    pos_y = y - modulos[id].pos * g_ref_alt.value
    modulos[id].pos = modulos[id].pos + 1
  
    local ref = nova_ref(pos_x, pos_y)
    if ref then
      muda_atrib (ref, {TERMINAL = linha.terms, ELEMENTO = linha.elemento})
    end
    elems[#elems + 1] = ref
    
    
    local retan = retangulo (x, pos_y,
      g_ref_descr.value, g_ref_alt.value)
    elems[#elems + 1] = retan
    
    local txt = cadzinho.new_text(x + 2,
      pos_y - 0.5 * g_ref_alt.value,
      linha.elemento, 2.0, "left", "middle")
    elems[#elems + 1] = txt
    
    txt = cadzinho.new_text(x + g_ref_descr.value - 2,
      pos_y - 0.5 * g_ref_alt.value,
      linha.terms, 2.0, "right", "middle")
    elems[#elems + 1] = txt
    
    --pos_y = pos_y - g_ref_alt.value
  end
  
  if #elems == 0 then return elems end
  
  local retan = retangulo (pos_x, y - 2 * g_ref_alt.value,
    g_ref_descr.value, g_ref_alt.value)
  elems[#elems + 1] = retan
  
  local txt = cadzinho.new_text(pos_x + g_ref_descr.value/2,
    y - 2.5 * g_ref_alt.value, 'APLICAÇÃO', 2.0, "center", "middle")
  elems[#elems + 1] = txt
  
  retan = retangulo (pos_x + g_ref_descr.value, y - 2 * g_ref_alt.value,
    g_ref_desenho.value, g_ref_alt.value)
  elems[#elems + 1] = retan
  
  txt = cadzinho.new_text(pos_x + g_ref_descr.value + g_ref_desenho.value/2,
    y - 2.5 * g_ref_alt.value, 'DESENHO', 2.0, "center", "middle")
  elems[#elems + 1] = txt
  
  retan = retangulo (pos_x, y,
    g_ref_descr.value + g_ref_desenho.value, total * g_ref_alt.value)
  cadzinho.add_ext(retan, "PELICANU", {cadzinho.unique_id(),
    "CAIXA", "COMPONENTE"})
  elems[#elems + 1] = retan
  
  txt = cadzinho.new_text(pos_x + (g_ref_descr.value + g_ref_desenho.value)/2,
    y - g_ref_alt.value, 'ID ??', 2.5, "center", "middle")
  cadzinho.add_ext(txt, "PELICANU", {cadzinho.unique_id(), "ROTULO", "CAIXA"})
  elems[#elems + 1] = txt
  
  txt = cadzinho.new_text(pos_x + g_ref_descr.value + g_ref_desenho.value - 2,
    y - 0.7 * g_ref_alt.value, '(' .. item .. ')', 2.0, "right", "middle")
  cadzinho.add_ext(txt, "PELICANU", {cadzinho.unique_id(), "ROTULO", "ITEM_LE"})
  elems[#elems + 1] = txt
  
  
  for id, modulo in pairs(modulos) do
    if id ~= 1 then
      retan = retangulo (pos_x, y - modulo.ini * g_ref_alt.value,
        g_ref_descr.value + g_ref_desenho.value, (modulo.elems + 2) * g_ref_alt.value)
      cadzinho.add_ext(retan, "PELICANU", {cadzinho.unique_id(),
        "CAIXA", "MODULO"})
      elems[#elems + 1] = retan
    
      txt = cadzinho.new_text(pos_x + (g_ref_descr.value + 0.0 * g_ref_desenho.value)/2,
        y - modulo.ini * g_ref_alt.value - 1.5 * g_ref_alt.value, id, 2.5, "center", "middle")
      cadzinho.add_ext(txt, "PELICANU", {cadzinho.unique_id(), "ROTULO", "CAIXA"})
      elems[#elems + 1] = txt
    
    end
  end
  
  --[[
  local caixa = cadzinho.new_pline(x, y - g_ref_alt.value, 0, x + g_ref_descr.value, y - g_ref_alt.value, 0)
  cadzinho.pline_append(caixa, x + g_ref_descr.value, y - 2 * g_ref_alt.value, 0)
  cadzinho.pline_append(caixa, x, y - 2 * g_ref_alt.value, 0)
  cadzinho.pline_close(caixa, true)
  elems[#elems + 1] = caixa
  
  caixa = cadzinho.new_pline(x + g_ref_descr.value, y - g_ref_alt.value, 0, x + g_ref_descr.value + g_ref_desenho.value, y - g_ref_alt.value, 0)
  cadzinho.pline_append(caixa, x + g_ref_descr.value + g_ref_desenho.value, y - 2 * g_ref_alt.value, 0)
  cadzinho.pline_append(caixa, x + g_ref_descr.value, y - 2 * g_ref_alt.value, 0)
  cadzinho.pline_close(caixa, true)
  elems[#elems + 1] = caixa]]--
  
  bd:close()
  
  return elems
end


--============== Janela Principal =======================
function pelicanu_win()
  cadzinho.nk_layout(20, 1)
  if cadzinho.nk_button(" Projeto:") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('projeto_dyn')
  end
  
  cadzinho.nk_layout(20, 1)
  cadzinho.nk_label(projeto.titulo)

  cadzinho.nk_layout(20, 3)
  if cadzinho.nk_button("﴾ Esquemático") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('esquematico_dyn')
  end
  if cadzinho.nk_button(" Lógico") then
    
  end
  if cadzinho.nk_button(" Unifilar") then
    
  end
  if cadzinho.nk_button("練 Fiação") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('fiacao_dyn')    
  end
  if cadzinho.nk_button(" Interligação") then
    
  end
  if cadzinho.nk_button(" Análise") then
    --[[local aux = format_dir(projeto.caminho) .. '_aux' .. fs.dir_sep
  
    if not exists (aux) then
      cria_pasta(aux)
    end
    local pl_base = aux .. 'base.xlsm'
    if not exists(pl_base) then
      copia_arq(diretorio(fs.script_path()) .. 'base.xlsm', pl_base)
    end]]--
    cadzinho.set_param({layer = 0, color = 'by layer', ltype = 'bylayer', lw = 'by layer'})
  end
  if cadzinho.nk_button(" Biblioteca") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('biblioteca_dyn')
  end
if cadzinho.nk_button(" Malha") then
    modal = ''
    sub_modal = ''
    cadzinho.start_dynamic('malha_dyn')
  end
  if cadzinho.nk_button_img(icon) then
	
  end
  
  
  
  --[[
  cadzinho.nk_layout(400, 1)
  if cadzinho.nk_tab_begin("modo_ed", g_editor_abas) then
    if g_editor_abas.value == 2 then -- biblioteca
      cadzinho.nk_layout(20, 1)
    
    
      if cadzinho.nk_button("terminal") then
        num_pt = 1
        local sel = cadzinho.get_sel()
        if (#sel > 0) then
          cadzinho.start_dynamic("terminal_dyn")
        end
      end
      
      if cadzinho.nk_button("engate") then
        num_pt = 1
        local sel = cadzinho.get_sel()
        if (#sel > 0) then
          cadzinho.start_dynamic("engate_dyn")
        end
      end
      
      
      if cadzinho.nk_button("test") then
        novo_projeto('C:\\util\\pelicanu\\teste', {})
      end
      
    else -- padrao - esquematico
      cadzinho.nk_layout(20, 1)
      if cadzinho.nk_button(" Ligacao") then
        num_pt = 1
        cadzinho.start_dynamic("ligacao_dyn")
      end
      if cadzinho.nk_button(" Componente") then
        num_pt = 1
        obtem_tipo_comp()
        cadzinho.start_dynamic("componente_dyn")
      end
      if cadzinho.nk_button("麗 Caixa") then
        num_pt = 1
        cadzinho.start_dynamic("caixa_dyn")
      end
      if cadzinho.nk_button(" Edita") then
        num_pt = 1
        cadzinho.start_dynamic("edita_dyn")
      end
      if cadzinho.nk_button(" Formato") then
        num_pt = 1
        obtem_lista_formato()
        cadzinho.start_dynamic("formato_dyn")
      end

      if cadzinho.nk_button(" Banco de dados") then
        teste()
      end
      if cadzinho.nk_button(" Grava pl") then
        grava_pl_comp()
      end
      if cadzinho.nk_button(" Le pl") then
        le_pl_comp()
      end
      if cadzinho.nk_button(" Engates") then
        atualiza_engates()
      end
    end
    cadzinho.nk_tab_end()
  end
  ]]--
end

-- inicia a janela quando o script eh executado
local config, err = carrega_config(diretorio(fs.script_path()) .. 'config.lua')
if config then
  biblioteca = config.biblioteca
end
local ini, err = carrega_config(diretorio(fs.script_path()) .. 'ini.lua')
if ini then
  abre_projeto(ini.projeto)
end
--cadzinho.win_show("pelicanu_win", "Pelicanu", 220,120,250,200)
cadzinho.win_show("pelicanu_win", "Pelicanu", 2, 540, 210, 165)
