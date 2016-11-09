# -*- coding: utf8 -*-
import os
import struct
import math


direcao=[(1, 0),
(1, 0.5),
(1, 1),
(0.5, 1),
(0, 1),
(-0.5, 1),
(-1, 1),
(-1, 0.5),
(-1, 0),
(-1, -0.5),
(-1, -1),
(-0.5, -1),
(0, -1),
(0.5, -1),
(1, -1),
(1, -0.5)]

octantes=[(1, 0),
(0.707106781, 0.707106781),
(0, 1),
(-0.707106781, 0.707106781),
(-1, 0),
(-0.707106781, -0.707106781),
(0, -1),
(0.707106781, -0.707106781)]
'''
octantes=[(1, 0),
(1, 1),
(0, 1),
(-1, 1),
(-1, 0),
(-1, -1),
(0, -1),
(1, -1)]
'''

class fonte_shx:
	comentario = ''
	num_fonte = None
	nome_fonte = ''
	dados_fonte = None
	lista_princ = []
	
	def __init__(self):
		self.abre_fonte()
	
	def abre_fonte(self):
		dir = os.path.dirname(os.path.abspath(__file__)).replace('\\','/') + '/'
		
		with open(dir+'simplex.shx', 'rb') as arq:
			conteudo = arq.read()
			#print conteudo[0:4]
			
		str_tmp = ''
		prox_index = 0
		valor_tmp = 0

		lista_dados = []
		cabecalho = None
		nome = None
		
		for index, byte_lido in enumerate(conteudo):
			#considera o byte lido como um inteiro nao negativo
			valor = struct.unpack('B', byte_lido)[0]
			if self.comentario == '':
				#busca os comentarios no inicio do arquivo
				if valor != 26:
					str_tmp += chr(valor)
					continue
				else: #termina quando encontra o valor 26 (1A hex)
					self.comentario += str_tmp
					str_tmp = ''
					
					#espera que o proximo campo tenha 6 bytes de comprimento (cabecalho do arquivo)
					prox_index = index + 6
					# prepara a lista para as letras serem adicionadas
					self.lista_princ.append([])
					cab_atual = self.lista_princ[-1]
					lista_tmp=[]
					continue
			
			#a partir deste ponto comeca a ler as definicoes de cada letra
			# as rotinas abaixo serao repetidas para cada letra
			elif not cabecalho:
				#busca o cabecalho da letra
				if index != prox_index:
					lista_tmp.append(valor)
				else: #termina quando atinge o comprimento de 4 bytes
					#verifica se o ultimo byte eh igual a zero, senao para por erro
					if (valor != 0): break #<=erro
					#o terceiro byte eh o comprimento do campo de dados da letra
					prox_index = index + lista_tmp[-1]
					# combina os dois primeiros bytes como um inteiro, que eh a especificacao unicode
					caracter= (lista_tmp[1] << 8) + lista_tmp[0]
					
					cab_atual.append(caracter)
					cabecalho = 1
					nome = None
					
			elif cabecalho:
				#busca os dados da letra (comandos de desenho)
				if index != prox_index:
					if not nome:
						if (valor != 0):
							str_tmp += chr(valor)
							
						else:
							nome = 1
							cab_atual.append(str_tmp)
							cab_atual.append([]) #adiciona
							coms_atual = cab_atual[-1]
							str_tmp = ''
					else:
						coms_atual.append(valor)
				else: #termina quando atinge o comprimento especificado
					#verifica se o ultimo byte eh igual a zero, senao para por erro
					if (valor != 0): break #<=erro
					
					# prepara a lista para a proxima letras
					prox_index = index + 4 #cabecalho com 4 bytes de compr
					self.lista_princ.append([])
					cab_atual = self.lista_princ[-1]
					lista_tmp=[]
					cabecalho = None
		
		if len(self.lista_princ)>0:
			dados = self.lista_princ.pop(0)
			self.num_fonte = dados[0]
			self.nome_fonte = dados[1]
			self.dados_fonte = dados[2]
			
			tam_fonte = abs(self.dados_fonte[0]+self.dados_fonte[1])
			cursor = [18, 'cursor', [1, 8, 0, tam_fonte, 2, 8, 0, struct.unpack('B', struct.pack('b',-tam_fonte))[0]]]
			self.lista_princ.insert(0,cursor)
			
		#print self.lista_princ
		#print self.comentario
		#print self.num_fonte
		#print self.nome_fonte
		#print self.dados_fonte
		
	def interpreta(self,txt):
		pre_x, pre_y = 0, 0
		px, py = 0, 0
		pena = 1
		executa = 0
		lista = []
		max_x = 0
		max_y = 0
		pilha = []
		
		for caracter in list(txt):
			letra = ord(caracter) #pega o codigo utf do caracter
			#procura a letra na fonte
			for i in self.lista_princ:
				if len(i)>1: 
					if i[0] == letra: break
			else: i = [42, 'asterisco', [0, 2, 14, 8, 254, 251, 33, 1, 68, 2, 46, 1, 72, 2, 65, 1, 74, 2, 68, 1, 78, 2, 47, 14, 8, 252, 253]]
			
			prox_index = 0
			comando = 0 #sem comando
			coord_y = 0 #indica que eh a coordenada y
			pula = 0
			bulge_f = 0
			bulge = 0
			arco_f = 0
			esc_tmp = 1.0
			escala = 1.0
			
			for index, j in enumerate(i[2]):
				if index == prox_index:
					executa = 0
					comando = j
					if comando > 14:
						#comando imediato de movimento
						prox_index = index + 1
						#obtem o comprimento do nibble maior
						compr = (comando & 240)/16
						#obtem o indice do vetor do nibble menor
						vetor = comando & 15
						# o vetor eh obtido da consultando o indice na tabela
						px = compr * direcao[vetor][0]
						py = compr * direcao[vetor][1]
						#print compr, vetor
						executa = 1
					elif comando == 1:
						#desenho ativado
						prox_index = index + 1
						if pula: 
							pula = 0
							continue
						pena = 1
						continue
					elif comando == 2:
						#desenho desligado
						prox_index = index + 1
						if pula: 
							pula = 0
							continue
						pena = 0
						continue
					elif comando == 3:
						#escala divide
						prox_index = index + 2
						continue
					elif comando == 4:
						#escala multiplica
						prox_index = index + 2
						continue
					elif comando == 5:
						#salva posicao atual
						prox_index = index + 1
						if pula: 
							pula = 0
							continue
						pilha.append((pre_x, pre_y))
						continue
					elif comando == 6:
						#restaura posicao atual
						prox_index = index + 1
						if pula: 
							pula = 0
							continue
						if len(pilha)>0:
							pre_x, pre_y = pilha.pop()
						continue
					elif comando == 7:
						#subshape
						prox_index = index + 3 #unicode pula 2 bytes
						if pula: 
							pula = 0
							continue
						#
						continue
					elif comando == 8:
						#uma coordenada (x,y)
						prox_index = index + 3
						coord_y = 0
						continue
					elif comando == 9:
						# sequencia de coordenadas (x,y), terminada em (0,0)
						prox_index = index + 3
						coord_y = 0
						continue
					elif comando == 10:
						#arco por octante
						prox_index = index + 3
						continue
					elif comando == 11:
						#arco fracionario
						prox_index = index + 6
						if pula: 
							pula = 0
							continue
						#
						continue
					elif comando == 12:
						#arco por bulge
						prox_index = index + 4
						if pula: 
							pula = 0
							continue
						#
						continue
					elif comando == 13:
						#sequencia de arcos por bulge, terminada em (0,0)
						prox_index = index + 3
						if pula: 
							pula = 0
							continue
						#
						continue
					elif comando == 14:
						# salta o proximo comando se eh um texto horizontal
						prox_index = index + 1
						pula = 1
						continue
				else:
					#print comando
					if comando == 3:
						if abs(j) > 0: esc_tmp = escala/j
						executa = 1
					if comando == 4:
						esc_tmp = escala*j
						executa = 1
					if comando == 8:
						if not coord_y:
							px = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 1
							continue
						else:
							py = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 0
							executa = 1
					elif comando == 9:
						if not coord_y:
							px = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 1
							continue
						else:
							py = struct.unpack('b', struct.pack('B', j))[0]
							if not((px==0) and (py==0)):
								coord_y = 0
								prox_index = index + 3
								executa = 1
								#print px, py
					if comando == 10:
						if not coord_y:
							raio = j
							coord_y = 1
						else:
							#obtem o primeiro octante e o sentido do nibble maior
							octante = (j & 112)/16
							sentido = (j & 128)/16
							if sentido: sentido = -1
							else: sentido =1
							
							#obtem a quantidade de octantes do nibble menor
							num_oct = j & 15
							if num_oct == 0: num_oct = 8 #circulo completo
							
							coord_y = 0
							arco_f = 1
							executa = 1
					if comando == 12:
						if not coord_y:
							px = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 1
							continue
						elif not bulge_f:
							py = struct.unpack('b', struct.pack('B', j))[0]
							bulge_f = 1
						else:
							bulge = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 0
							bulge_f = 0
							executa = 1
					elif comando == 13:
						if not coord_y:
							px = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 1
							continue
						elif not bulge_f:
							py = struct.unpack('b', struct.pack('B', j))[0]
							if not((px==0) and (py==0)):
								prox_index = index + 2
								bulge_f = 1
							continue
						else:
							bulge = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 0
							prox_index = index + 3
							executa = 1
							bulge_f = 0
				if executa:
					executa = 0
					#print comando
					if pula: 
						pula = 0
						esc_tmp = escala
						arco_f = 0
					else: 
						if escala != esc_tmp: escala = esc_tmp
						elif arco_f:
							arco_f = 0
							centro_x = pre_x - raio * octantes[octante][0]
							centro_y = pre_y - raio * octantes[octante][1]
							ang_ini = octante * math.pi/4
							for i in range(1, num_oct+1):
								px = centro_x + raio * math.cos(2 * math.pi * i * sentido/ 8 + ang_ini)
								py = centro_y + raio * math.sin(2 * math.pi * i * sentido/ 8 + ang_ini)
								lista.append(((pre_x,pre_y),(px,py)))
								pre_x=px
								pre_y=py
							#print centro_x, centro_y, raio
							#print octante, num_oct
						else:
							px *= escala
							py *= escala
							if pena:
								lista.append(((pre_x,pre_y),(pre_x+px,pre_y+py)))
								#lista.append((pre_x+px,pre_y+py))
								max_x = max([max_x, pre_x, pre_x+px])
								max_y = max([max_y, pre_y, pre_y+py])
							pre_x += px
							pre_y += py
							#print [pre_x,pre_y]
		
		return lista, max_x, max_y

if __name__ == "__main__":
	fonte = fonte_shx()
	for i in fonte.lista_princ:
		if len(i)> 1:
			a= i[0]
			#letra = struct.pack('H', a)
			letra = unichr(a)
			cod = ord(letra)
			print  i, '#', a, repr(letra), cod
	#print fonte.interpreta('teste')
	#print fonte.lista_princ[0]
	#print fonte.dados_fonte
	#print fonte.num_fonte
