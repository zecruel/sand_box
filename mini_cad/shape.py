# -*- coding: utf8 -*-
import os
import struct


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
		
		with open(dir+'txt.shx', 'rb') as arq:
			conteudo = arq.read()
			#print conteudo[0:4]
			
		str_tmp = ''
		prox_index = 0
		valor_tmp = 0

		lista_dados = []
		cabecalho = None

		for index, byte_lido in enumerate(conteudo):
			#considera o byte lido como um inteiro nao negativo
			valor = struct.unpack('B', byte_lido)[0]
			if self.comentario == '':
				#busca os comentarios no inicio do arquivo
				if valor != 0:
					str_tmp += chr(abs(valor))
					continue
				else: #termina quando encontra o valor zero
					self.comentario += str_tmp
					prox_index = index + 1
					str_tmp = ''
					continue
			elif not self.num_fonte:
				#busca o numero da fonte
				if (prox_index == index):
					if (valor != 0): break #<=erro, terminou inesperadamente
					elif valor_tmp:
						self.num_fonte = valor_tmp
						continue
					else: continue
				elif (valor != 0): #termina quando encontra o valor zero
					valor_tmp = valor
					prox_index = index + 1
					continue
			elif self.nome_fonte == '':
				#busca o nome da fonte
				# os caracteres depois do espaco devem ser ignorados
				if valor != 0:
					str_tmp += chr(abs(valor))
					continue
				else: #termina quando encontra o valor zero
					self.nome_fonte += str_tmp
					#espera que o proximo campo tenha 6 bytes de comprimento (fonte Unicode)
					prox_index = index + 6
					str_tmp = ''
					continue
			elif not self.dados_fonte:
				#dados gerais da fonte conforme lista abaixo (fonte Unicode)
				#[above, below, modes (0 = horizontal, 2= dual), encoding, type(licence)]
				if index != prox_index:
					lista_dados.append(valor)
					continue
				else: 
					if (valor != 0): break #<=erro, terminou inesperadamente
					self.dados_fonte = lista_dados
					#espera que o proximo campo tenha 4 bytes de comprimento (cabecalhos de cada letra)
					prox_index = index + 4
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
					prox_index = index + lista_tmp[2]
					# combina os dois primeiros bytes como um inteiro, que eh a especificacao unicode
					caracter= (lista_tmp[1] << 8) + lista_tmp[0]
					
					cab_atual.append(caracter)
					cab_atual.append([]) #adiciona
					coms_atual = cab_atual[-1]
					cabecalho = 1
					
			elif cabecalho:
				#busca os dados da letra (comandos de desenho)
				if index != prox_index:
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
		'''
		print self.lista_princ
		print self.comentario
		print self.num_fonte
		print self.nome_fonte
		print self.dados_fonte
		'''
	def interpreta(self,txt):
		pre_x, pre_y = 0, 0
		px, py = 0, 0
		pena = 1
		executa = 0
		lista = []
		max_x = 0
		max_y = 0
		
		for caracter in list(txt):
			letra = ord(caracter) #pega o codigo utf do caracter
			#procura a letra na fonte
			for i in self.lista_princ:
				if len(i)>1: 
					if i[0] == letra: break
			else: i = [42, [0, 2, 14, 8, 254, 251, 33, 1, 68, 2, 46, 1, 72, 2, 65, 1, 74, 2, 68, 1, 78, 2, 47, 14, 8, 252, 253]]
			
			prox_index = 1 # ja comeca do indice 1
			comando = 0 #sem comando
			coord_y = 0 #indica que eh a coordenada y
			pula = 0
			bulge_f = 0
			esc_tmp = 1.0
			escala = 1.0
			
			for index, j in enumerate(i[1]):
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
						#
						continue
					elif comando == 6:
						#restaura posicao atual
						prox_index = index + 1
						if pula: 
							pula = 0
							continue
						#
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
						if pula: 
							pula = 0
							continue
						#
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
						prox_index = index + 4
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
					if comando == 3:
						esc_tmp = escala/j
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
							if not((px==0) and (px==0)):
								coord_y = 0
								prox_index = index + 3
								executa = 1
								#print px, py
					elif comando == 13:
						if not coord_y:
							px = struct.unpack('b', struct.pack('B', j))[0]
							coord_y = 1
							continue
						elif not bulge_f:
							py = struct.unpack('b', struct.pack('B', j))[0]
							if not((px==0) and (px==0)):
								coord_y = 0
								prox_index = index + 4
								#executa = 1
							bulge_f = 1
							continue
						else:
							coord_y = 0
							prox_index = index + 3
							#executa = 1
							bulge_f = 0
				if executa:
					executa = 0
					#print comando
					if pula: 
						pula = 0
						esc_tmp = escala
					else: 
						if escala != esc_tmp: escala = esc_tmp
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
			#print  i, '#', a, repr(letra), cod
	print fonte.interpreta('!')
