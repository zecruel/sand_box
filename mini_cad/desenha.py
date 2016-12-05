from dxf import *
import dxf_color
import math
import re

def sub_ctrl(cod):
	#print cod.group(0), cod.group(1)
	if cod.group(1).upper() == 'D': return unichr(176)
	elif cod.group(1).upper() == 'P': return unichr(177)
	elif cod.group(1).upper() == 'C': return unichr(216) #simbolo de diametro
		
	return ''

class dxf_render:
	def __init__(self, drawing, selecao):
		#gera a tabela de cores padrao Autocad em uma lista
		self.t_cor = dxf_color.init_color()
		self.tela=None
		self.selecao = selecao
		
		#obtem as secoes basicas do DXF
		self.head, self.tabs, self.blocos, self.ents = dxf_des(drawing)
		
		#obtem as variaveis do cabecalho DXF
		self.l_var = dxf_vars(self.head)
		#print l_var
		
		#obtem os Layers cadastrados
		self.layers = dxf_item(self.tabs, 'TABLE', 'LAYER')
		
		#obtem os tipos de linha cadastrados
		self.ltypes = dxf_item(self.tabs, 'TABLE', 'LTYPE')
		
		self.t_linhas = {}
		
		
	def desenha_ents(self):
		if self.tela:
			self.tela.limpa()
			#print 'teste'
			#self.desenha(self.ents, [0,0,0], 30) #teste
			self.desenha(self.ents, [0,0,0], 0)
			
			#self.desenha(self.selecao.lista_selecao, [0,0,0], 0, None, 1)
			self.tela.lib.desenha_lista()
			self.tela.exibe()
	
	def desenha_selec(self, selec):
		if self.tela:
			if isinstance(selec, obj_dxf):
				#print selec
				self.tela.limpa_selec()
				self.desenha(selec, [0,0,0], 0, None, 1)
				self.tela.exibe()
	
	def rotaciona(self, pt1, pivo, ang):
		pt_rot = [0, 0, 0]
		pt_rot[0] = math.cos(ang)*(pt1[0]-pivo[0]) - math.sin(ang)*(pt1[1]-pivo[1]) + pivo[0]
		pt_rot[1] = math.sin(ang)*(pt1[0]-pivo[0]) + math.cos(ang)*(pt1[1]-pivo[1]) + pivo[1]
		pt_rot[2] = pt1[2]
		return pt_rot
		
	def tipos_linha(self):
		self.t_linhas = {}
		lista = dxf_itens(self.ltypes, 'LTYPE')
		for ltype_n in lista:
			#busca o patern da linha
			ltype = dxf_item(self.ltypes, 'LTYPE', ltype_n) #objeto ltype
			valor = dxf_proc_grupo(ltype, 49)
			#print ltype_n, valor
			if len(valor)>0:
				#escalona o pattern (minimo 4 pixels)
				#minimo = min([abs(x) for x in valor])
				#pattern = [4*x/minimo for x in valor]
				
				#escalona o pattern (pelo maximo = 15 pixels)
				maximo = max([abs(x) for x in valor])
				if maximo >= 0:
					pattern = [int(15*x/maximo) for x in valor]
			else:
				pattern = [1]
			#muda o pattern do desenho
			if self.tela:
				self.t_linhas[ltype_n] = self.tela.add_pattern(pattern)
	
	def desenha(self, mestre, offset=[0,0,0], rotacao=0, contido=None, camada=0):
		if isinstance(mestre, obj_dxf):
			pt_ant = None #ponto anterior -> para polylines
			pt_final = None #ponto final -> para polylines fechadas
			bulge_ant = 0 #para arcos em polylines
			
			for idx, i in enumerate(mestre.conteudo): #varre o conteudo do obj em busca de entidades
				tipo = ''
				entidade = None
				
				#verifica se eh uma entidade DXF
				if isinstance(i, obj_dxf):
					tipo =  i.nome
					entidade = i
					ltype_n = ''
					cor = 1 #define a cor 1 (vermelho) como padrao
					pattern = [1] #linha solida
					if not contido:
						dono = entidade
					else:
						dono = contido
					
					#busca o layer da entidade
					valor = dxf_proc_fixo(entidade, 'layer')
					if len(valor)>0: 
						n_layer = valor[0] #nome do layer
						layer = dxf_item(self.layers, 'LAYER', n_layer) #objeto layer
						
						#busca a cor padrao do layer
						valor = dxf_proc_fixo(layer, 'color')
						if len(valor)>0: cor = valor[0]
							
						#busca o tipo de linha padrao do layer
						valor = dxf_proc_fixo(layer, 'l_type')
						if len(valor)>0:
							ltype_n = valor[0] #nome do tipo de linha
					
					#cor do objeto, se houver, sobrepoe a do layer
					valor = dxf_proc_fixo(entidade, 'color')
					if len(valor)>0: 
						if valor[0] < 256: cor = valor[0]
					
					#tipo de linha do objeto, se houver, sobrepoe o do layer
					valor = dxf_proc_fixo(entidade, 'l_type')
					if len(valor)>0:
						if valor[0].upper() != 'BYLAYER': ltype_n = valor[0] #nome do tipo de linha	
					
					'''
					#busca o patern da linha
					ltype = dxf_item(self.ltypes, 'LTYPE', ltype_n) #objeto ltype
					valor = dxf_proc_grupo(ltype, 49)
					if len(valor)>0:
						#escalona o pattern (minimo 4 pixels)
						#minimo = min([abs(x) for x in valor])
						#pattern = [4*x/minimo for x in valor]
						
						#escalona o pattern (pelo maximo = 15 pixels)
						maximo = max([abs(x) for x in valor])
						if maximo > 0:
							pattern = [int(15*x/maximo) for x in valor]
					'''
					
					#muda o pattern do desenho
					if self.tela:
						try:
							self.tela.lib.muda_pattern(self.t_linhas[ltype_n])
						except KeyError:
							self.tela.lib.muda_pattern(1)
						#print self.t_linhas[ltype_n]
						#self.tela.patt_a = 0
				
				if tipo == 'LINE':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					
					#procura o primeiro ponto
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#procura o segundo ponto
					valor = dxf_proc_grupo(entidade, 11) #x
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 21) #y
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 31) #z
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						pt2 = self.rotaciona(pt2, offset, rotacao*math.pi/180)
					
					#desenha, se houver onde
					if self.tela:
						self.tela.linha(dono, camada, pt1[:2], pt2[:2], self.t_cor[cor])
					
				elif tipo == 'POINT':
					pass
					
				elif tipo == 'CIRCLE':
					pt1 = [0,0,0]
					raio = 0
					
					#procura o centro
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#procura o raio
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: raio = valor[0]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
					
					#desenha, se houver onde
					if self.tela:
						self.tela.circulo(dono, camada, pt1[:2], raio, self.t_cor[cor])
					
				elif tipo == 'ARC':
					pt1 = [0,0,0]
					raio = 0
					ang_ini = 0
					ang_fim = 0
					
					#procura o centro
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#procura o raio
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: raio = valor[0]
					
					#angulos de inicio e fim
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: ang_ini = valor[0]
					valor = dxf_proc_grupo(entidade, 51)
					if len(valor)>0: ang_fim = valor[0]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						ang_ini += rotacao
						ang_fim += rotacao
					
					#desenha, se houver onde
					if self.tela:
						self.tela.arco(dono, camada, pt1[:2], raio, ang_ini, ang_fim, self.t_cor[cor])
						
				elif tipo == 'TRACE':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					pt3 = [0,0,0]
					pt4 = [0,0,0]
					
					#primeiro ponto
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#segundo ponto
					valor = dxf_proc_grupo(entidade, 11) #x
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 21) #y
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 31) #z
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					#terceiro ponto
					valor = dxf_proc_grupo(entidade, 12) #x
					if len(valor)>0: pt3[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 22) #y
					if len(valor)>0: pt3[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 32) #z
					if len(valor)>0: pt3[2] = valor[0] + offset[2]
					
					#quarto ponto
					valor = dxf_proc_grupo(entidade, 13) #x
					if len(valor)>0: pt4[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 23) #y
					if len(valor)>0: pt4[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 33) #z
					if len(valor)>0: pt4[2] = valor[0] + offset[2]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						pt2 = self.rotaciona(pt2, offset, rotacao*math.pi/180)
						pt3 = self.rotaciona(pt3, offset, rotacao*math.pi/180)
						pt4 = self.rotaciona(pt4, offset, rotacao*math.pi/180)
					
					if self.tela:
						self.tela.linha(dono, camada, pt1[:2], pt2[:2], self.t_cor[cor])
						self.tela.linha(dono, camada, pt1[:2], pt3[:2], self.t_cor[cor])
						self.tela.linha(dono, camada, pt2[:2], pt4[:2], self.t_cor[cor])
						self.tela.linha(dono, camada, pt3[:2], pt4[:2], self.t_cor[cor])
						
				elif tipo == 'SOLID':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					pt3 = [0,0,0]
					pt4 = [0,0,0]
					quarto_ponto = 0
					
					#primeiro ponto
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#segundo ponto
					valor = dxf_proc_grupo(entidade, 11) #x
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 21) #y
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 31) #z
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					#terceiro ponto
					valor = dxf_proc_grupo(entidade, 12) #x
					if len(valor)>0: pt3[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 22) #y
					if len(valor)>0: pt3[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 32) #z
					if len(valor)>0: pt3[2] = valor[0] + offset[2]
					
					#quarto ponto -> opcional
					valor = dxf_proc_grupo(entidade, 13) #x
					if len(valor)>0:
						pt4[0] = valor[0] + offset[0]
						quarto_ponto = 1
					valor = dxf_proc_grupo(entidade, 23) #y
					if len(valor)>0:
						pt4[1] = valor[0] + offset[1]
						quarto_ponto = 1
					valor = dxf_proc_grupo(entidade, 33) #z
					if len(valor)>0:
						pt4[2] = valor[0] + offset[2]
						quarto_ponto = 1
						
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						pt2 = self.rotaciona(pt2, offset, rotacao*math.pi/180)
						pt3 = self.rotaciona(pt3, offset, rotacao*math.pi/180)
						pt4 = self.rotaciona(pt4, offset, rotacao*math.pi/180)
					
					if self.tela:
						self.tela.linha(dono, camada, pt1[:2], pt2[:2], self.t_cor[cor])
						self.tela.linha(dono, camada, pt1[:2], pt3[:2], self.t_cor[cor])
						if quarto_ponto: #se houver quarto ponto
							self.tela.linha(dono, camada, pt2[:2], pt4[:2], self.t_cor[cor])
							self.tela.linha(dono, camada, pt3[:2], pt4[:2], self.t_cor[cor])
						else:
							self.tela.linha(dono, camada, pt2[:2], pt3[:2], self.t_cor[cor])
				
				elif tipo == 'TEXT':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					segundo_ponto = 0
					tam = 0
					rot = 0
					alin = [0,1]
					
					#primeiro ponto
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#segundo ponto -> opcional
					valor = dxf_proc_grupo(entidade, 11)
					if len(valor)>0: 
						pt2[0] = valor[0] + offset[0]
						segundo_ponto = 1
					valor = dxf_proc_grupo(entidade, 21)
					if len(valor)>0: 
						pt2[1] = valor[0] + offset[1]
						segundo_ponto = 1
					valor = dxf_proc_grupo(entidade, 31)
					if len(valor)>0: 
						pt2[2] = valor[0] + offset[2]
						segundo_ponto = 1
					
					#tamanho do texto
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: tam = valor[0]
					
					#rotacao do texto
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: rot = valor[0]
					
					#string do texto
					valor = dxf_proc_grupo(entidade, 1)
					if len(valor)>0: texto = valor[0]
					
					#alinhamento horizontal
					valor = dxf_proc_grupo(entidade, 72)
					if len(valor)>0: alin[0] = valor[0]
					
					#alinhamento vertical
					valor = dxf_proc_grupo(entidade, 73)
					if len(valor)>0: alin[1] = valor[0]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						pt2 = self.rotaciona(pt2, offset, rotacao*math.pi/180)
						rot -= rotacao
					
					#substitui os codigos de controle %% por caracteres unicode
					texto = re.sub('%%(\w)', sub_ctrl, texto)
					
					
					
					if self.tela:
						if segundo_ponto:
							self.tela.texto_shx(dono, camada, texto, pt1[:2], pt2[:2], tam, rot, self.t_cor[cor], alin)
						else:
							self.tela.texto_shx(dono, camada, texto, pt1[:2], pt1[:2], tam, rot, self.t_cor[cor], alin)
						
					#print entidade.imprime() #---------------debug
					
				elif tipo == 'SHAPE':
					pass
				elif tipo == 'INSERT':
					if self.blocos:
						pt1 = [0,0,0]
						rot = 0
						nome_blk = ''
						
						#ponto de insercao do bloco
						valor = dxf_proc_grupo(entidade, 10)
						if len(valor)>0: pt1[0] = valor[0] + offset[0]
						valor = dxf_proc_grupo(entidade, 20)
						if len(valor)>0: pt1[1] = valor[0] + offset[1]
						valor = dxf_proc_grupo(entidade, 30)
						if len(valor)>0: pt1[2] = valor[0] + offset[2]
						
						# nome do bloco
						valor = dxf_proc_grupo(entidade, 2)
						if len(valor)>0: nome_blk = valor[0]
						
						#rotacao
						valor = dxf_proc_grupo(entidade, 50)
						if len(valor)>0: rot = valor[0]
						
						#aplica rotacao
						if rotacao != 0:
							pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						
						#busca o bloco na biblioteca
						blk = dxf_item(self.blocos, 'BLOCK', nome_blk)
						self.desenha(blk, pt1, rotacao+rot, dono, camada) #faz recursivamente os blocos
						
					#faz recursivamente para os atributos fora da definicao do bloco
					self.desenha(entidade, offset, rotacao, dono, camada)
						
				elif tipo == 'ATTRIB':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					segundo_ponto = 0
					tam = 0
					rot = 0
					alin = [0,1]
					
					#primeiro ponto
					valor = dxf_proc_grupo(entidade, 10) #x
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20) #y
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30) #z
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#segundo ponto -> opcional
					valor = dxf_proc_grupo(entidade, 11)
					if len(valor)>0: 
						pt2[0] = valor[0] + offset[0]
						segundo_ponto = 1
					valor = dxf_proc_grupo(entidade, 21)
					if len(valor)>0: 
						pt2[1] = valor[0] + offset[1]
						segundo_ponto = 1
					valor = dxf_proc_grupo(entidade, 31)
					if len(valor)>0: 
						pt2[2] = valor[0] + offset[2]
						segundo_ponto = 1
					
					#tamanho do texto
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: tam = valor[0]
					
					#rotacao do texto
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: rot = valor[0]
					
					#string do texto
					valor = dxf_proc_grupo(entidade, 1)
					if len(valor)>0: texto = valor[0]
					
					#alinhamento horizontal
					valor = dxf_proc_grupo(entidade, 72)
					if len(valor)>0: alin[0] = valor[0]
					
					#alinhamento vertical
					valor = dxf_proc_grupo(entidade, 74)
					if len(valor)>0: alin[1] = valor[0]
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
						pt2 = self.rotaciona(pt2, offset, rotacao*math.pi/180)
						rot -= rotacao
					
					if self.tela:
						if segundo_ponto:
							self.tela.texto_shx(dono, camada, texto, pt1[:2], pt2[:2], tam, rot, self.t_cor[cor], alin)
						else:
							self.tela.texto_shx(dono, camada, texto, pt1[:2], pt1[:2], tam, rot, self.t_cor[cor], alin)
				elif tipo == 'POLYLINE':
					self.desenha(entidade, offset, rotacao, dono, camada) #faz recursivamente
					pt_ant = None
					pt_final = None
					
				elif tipo == 'VERTEX':
					
					pt1 = [0,0,0]
					
					#ponto do vertice
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					#valor do bulge -> usado em segmentos circulares
					valor = dxf_proc_grupo(entidade, 42)
					if len(valor)>0:
						bulge = valor[0]
					else:
						bulge = 0
					
					#aplica rotacao
					if rotacao != 0:
						pt1 = self.rotaciona(pt1, offset, rotacao*math.pi/180)
					
					#verifica se nao eh o primeiro ponto
					if pt_ant != None and self.tela:
						if bulge_ant == 0:
							self.tela.linha(dono, camada, pt_ant[:2], pt1[:2], self.t_cor[cor])
						else:
							self.tela.arco_bulge(dono, camada, pt_ant[:2], pt1[:2], bulge_ant, self.t_cor[cor])
					else: 
						pt_ant = [0,0,0]
						#verifica se a a polyline eh fechada
						valor = dxf_proc_grupo(mestre, 70) #busca a flag no objeto mestre
						if len(valor)>0:
							if (valor[0] & 1):
								pt_final = [0,0,0]
								#define o primeiro ponto como ponto final para fechar o caminho
								pt_final[0], pt_final[1], pt_final [2] = pt1[0], pt1[1], pt1[2]
					#prepara para o proximo ponto
					pt_ant[0], pt_ant[1], pt_ant[2] = pt1[0], pt1[1], pt1[2]
					bulge_ant = bulge
				
				if tipo == 'LWPOLYLINE':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					
					#procura a lista de pontos
					lista_x = dxf_proc_grupo(entidade, 10) #x
					lista_y = dxf_proc_grupo(entidade, 20) #y
					lista_z = dxf_proc_grupo(entidade, 30) #z
					lista_bulge = dxf_proc_grupo(entidade, 42)
					
					#busca o numero de vertices
					valor = dxf_proc_grupo(entidade, 90)
					if len(valor)>0:
						vertices = int(valor[0])
					else: vertices = 0
					
					vertices = max(len(lista_x), len(lista_y), len(lista_z), vertices)
						
					if vertices > 1: #continua se houver mais de 1 vertice
						
						#pega o primeiro ponto
						if len(lista_x) > 0: pt1[0] = lista_x[0] + offset[0]
						else: pt1[0] = offset[0]
						if len(lista_y) > 0: pt1[1] = lista_y[0] + offset[1]
						else: pt1[1] = offset[1]
						if len(lista_z) > 0: pt1[2] = lista_z[0] + offset[2]
						else: pt1[2] = offset[2]
						if len(lista_bulge) > 0: bulge = lista_bulge[0]
						else: bulge = 0
						
						#verifica se a a polyline eh fechada
						pt_final = None
						valor = dxf_proc_grupo(entidade, 70) #busca a flag no objeto mestre
						if len(valor)>0:
							if (valor[0] & 1):
								pt_final = [0,0,0]
								#define o primeiro ponto como ponto final para fechar o caminho
								pt_final[0], pt_final[1], pt_final [2] = pt1[0], pt1[1], pt1[2]
						
						for i in range(1,vertices):
							
							if i < len(lista_x): pt2[0] = lista_x[i] + offset[0]
							else: pt2[0] = offset[0]
							if i < len(lista_y): pt2[1] = lista_y[i] + offset[1]
							else: pt2[1] = offset[1]
							if i < len(lista_z): pt2[2] = lista_z[i] + offset[2]
							else: pt2[2] = offset[2]
							
							if self.tela:
								if bulge == 0:
									self.tela.linha(dono, camada, pt1[:2], pt2[:2], self.t_cor[cor])
								else:
									self.tela.arco_bulge(dono, camada, pt1[:2], pt2[:2], bulge, self.t_cor[cor])
							
							pt1[0], pt1[1], pt1[2] = pt2[0], pt2[1], pt2[2]
							if i < len(lista_bulge): bulge = lista_bulge[i]
							else: bulge = 0
						
						#se for uma plyline fechada
						if pt_final and self.tela:
							if bulge == 0:
								self.tela.linha(dono, camada, pt1[:2], pt_final[:2], self.t_cor[cor])
							else:
								self.tela.arco_bulge(dono, camada, pt1[:2], pt_final[:2], bulge, self.t_cor[cor])
						pt_final = None
				
				elif tipo == '3DFACE':
					pass
				elif tipo == 'VIEWPORT':
					pass
				elif tipo == 'DIMENSION':
					pass
				else:
					pass
			else: #apos o fechamento do loop, verifica se ha pontos pendentes de desenho (polylines fechadas)
				if pt_final != None and self.tela:
					if bulge_ant == 0:
						self.tela.linha(dono, camada, pt_ant[:2], pt_final[:2], self.t_cor[cor])
					else:
						self.tela.arco_bulge(dono, camada, pt_ant[:2], pt_final[:2], bulge_ant, self.t_cor[cor])