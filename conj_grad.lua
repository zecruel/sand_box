function conjugate_gradient (A, x, b)
-- credito: https://en.wikipedia.org/wiki/Conjugate_gradient_method

	--inicializa
	n = #A
	local r = {}
	local tmp = 0
	local erro = 0
	
	-- obtem r0
	for l = 1, n do
		tmp = 0
		for c = 1, n do
			tmp = tmp + A[l][c] * x[c]
		end
		r[l] = b[l] - tmp
		erro = erro + r[l] * r[l]
	end
	if erro < 1e-6 then return erro, 0 end -- verifica criterio de parada
	
	-- obtem p0
	local p = {}
	for l = 1, n do  p[l] = r[l] end
	
	-- comeca as iteracoes
	local alfa, beta
	local num, den
	for k = 1, 100 do -- max de 100 iteracoes
	
		-- calcula alfa -> escalar
		num = 0; den = 0
		for l = 1, n do
			tmp = 0
			for c = 1, n do
				tmp = tmp + A[l][c] * p[c]
			end
			den = den + p[l] * tmp
			num = num + r[l] * r[l]
		end
		alfa = num / den
		
		-- proximo vetor de resultado
		for l = 1, n do
			x[l] = x[l] + alfa * p[l]
		end
		
		-- calcula o vetor de restos r e os componentes de beta
		den = 0; erro = 0
		for l = 1, n do
			tmp = 0
			for c = 1, n do
				tmp = tmp + A[l][c] * p[c]
			end
			den = den +  r[l] * r[l]
			r[l] = r[l] - alfa * tmp
			erro = erro + r[l] * r[l]
		end
		
		if erro < 1e-6 then return erro, k end -- verifica criterio de parada
		
		-- calcula beta e aplica ao proximo vetor p
		beta = erro / den
		for l = 1, n do
			p[l] = r[l] + beta * p[l]
		end
	end
	
	return erro, k
end

A = {{4, 1}, {1, 3}}
b = {1,2}
x = {2,1}

erro, n = conjugate_gradient (A, x, b)
print ('erro =', erro, 'iteracoes =' , n)
io.write( "x = {  " )
		for l = 1, n do
			io.write( x[l], "  ,  " )
		end
		io.write( "}\n " )