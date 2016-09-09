from dxf import *
import dxf_color


class dxf_render:
	def __init__(self, drawing):
		#gera a tabela de cores padrao Autocad em uma lista
		self.t_cor = dxf_color.init_color()
		self.tela=None
		
		#obtem as secoes basicas do DXF
		self.head, self.tabs, self.blocos, self.ents = dxf_des(drawing)
		
		#obtem as variaveis do cabecalho DXF
		self.l_var = dxf_vars(self.head)
		#print l_var
		
		#obtem os Layers cadastrados
		self.layers = dxf_item(self.tabs, 'TABLE', 'LAYER')
		
		#obtem os tipos de linha cadastrados
		self.ltypes = dxf_item(self.tabs, 'TABLE', 'LTYPE')
		
	def desenha_ents(self):
		if self.tela:
			self.tela.limpa()
			self.desenha(self.ents)
			self.tela.exibe()
	
	def desenha(self, mestre, offset=[0,0,0], rotacao=0):
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
							pattern = [15*x/maximo for x in valor]
					
					#muda o pattern do desenho
					if self.tela:
						self.tela.pattern = pattern
						self.tela.patt_a = 0
					
				if tipo == 'LINE':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 11)
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 21)
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 31)
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					if self.tela:
						self.tela.linha(pt1[:2], pt2[:2], self.t_cor[cor])
					
				elif tipo == 'POINT':
					pass
				elif tipo == 'CIRCLE':
					pt1 = [0,0,0]
					raio = 0
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: raio = valor[0] 
					
					if self.tela:
						self.tela.circulo(pt1[:2], raio, self.t_cor[cor])
					
				elif tipo == 'ARC':
					pt1 = [0,0,0]
					raio = 0
					ang_ini = 0
					ang_fim = 0
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: raio = valor[0]
					
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: ang_ini = valor[0]
					
					valor = dxf_proc_grupo(entidade, 51)
					if len(valor)>0: ang_fim = valor[0]
					
					if self.tela:
						self.tela.arco(pt1[:2], raio, ang_ini, ang_fim, self.t_cor[cor])
						
				elif tipo == 'TRACE':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					pt3 = [0,0,0]
					pt4 = [0,0,0]
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 11)
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 21)
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 31)
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 12)
					if len(valor)>0: pt3[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 22)
					if len(valor)>0: pt3[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 32)
					if len(valor)>0: pt3[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 13)
					if len(valor)>0: pt4[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 23)
					if len(valor)>0: pt4[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 33)
					if len(valor)>0: pt4[2] = valor[0] + offset[2]
					
					if self.tela:
						self.tela.linha(pt1[:2], pt2[:2], self.t_cor[cor])
						self.tela.linha(pt1[:2], pt3[:2], self.t_cor[cor])
						self.tela.linha(pt2[:2], pt4[:2], self.t_cor[cor])
						self.tela.linha(pt3[:2], pt4[:2], self.t_cor[cor])
						
				elif tipo == 'SOLID':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					pt3 = [0,0,0]
					pt4 = [0,0,0]
					quarto_ponto = 0
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 11)
					if len(valor)>0: pt2[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 21)
					if len(valor)>0: pt2[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 31)
					if len(valor)>0: pt2[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 12)
					if len(valor)>0: pt3[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 22)
					if len(valor)>0: pt3[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 32)
					if len(valor)>0: pt3[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 13)
					if len(valor)>0:
						pt4[0] = valor[0] + offset[0]
						quarto_ponto = 1
					
					valor = dxf_proc_grupo(entidade, 23)
					if len(valor)>0:
						pt4[1] = valor[0] + offset[1]
						quarto_ponto = 1
					
					valor = dxf_proc_grupo(entidade, 33)
					if len(valor)>0:
						pt4[2] = valor[0] + offset[2]
						quarto_ponto = 1
					
					if self.tela:
						self.tela.linha(pt1[:2], pt2[:2], self.t_cor[cor])
						self.tela.linha(pt1[:2], pt3[:2], self.t_cor[cor])
						if quarto_ponto:
							self.tela.linha(pt2[:2], pt4[:2], self.t_cor[cor])
							self.tela.linha(pt3[:2], pt4[:2], self.t_cor[cor])
						else:
							self.tela.linha(pt2[:2], pt3[:2], self.t_cor[cor])
				elif tipo == 'TEXT':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					segundo_ponto = 0
					tam = 0
					rot = 0
					alin = [0,1]
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
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
					
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: tam = valor[0]
					
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: rot = valor[0]
					
					valor = dxf_proc_grupo(entidade, 1)
					if len(valor)>0: texto = valor[0]
					
					valor = dxf_proc_grupo(entidade, 72)
					if len(valor)>0: alin[0] = valor[0]
					
					valor = dxf_proc_grupo(entidade, 73)
					if len(valor)>0: alin[1] = valor[0]
					
					if self.tela:
						if segundo_ponto:
							self.tela.texto(texto, pt1[:2], pt2[:2], tam, rot, self.t_cor[cor], alin)
						else:
							self.tela.texto(texto, pt1[:2], pt1[:2], tam, rot, self.t_cor[cor], alin)
						
					#print entidade.imprime() #---------------debug
					
				elif tipo == 'SHAPE':
					pass
				elif tipo == 'INSERT':
					if self.blocos:
						pt1 = [0,0,0]
						rot = 0
						nome_blk = ''
						
						valor = dxf_proc_grupo(entidade, 10)
						if len(valor)>0: pt1[0] = valor[0] + offset[0]
						
						valor = dxf_proc_grupo(entidade, 20)
						if len(valor)>0: pt1[1] = valor[0] + offset[1]
						
						valor = dxf_proc_grupo(entidade, 30)
						if len(valor)>0: pt1[2] = valor[0] + offset[2]
						
						valor = dxf_proc_grupo(entidade, 2)
						if len(valor)>0: nome_blk = valor[0]
						
						valor = dxf_proc_grupo(entidade, 50)
						if len(valor)>0: rot = valor[0]
						
						blk = dxf_item(self.blocos, 'BLOCK', nome_blk)
						self.desenha(blk, pt1, rot) #faz recursivamente os blocos
						#print pt1
						#print entidade.imprime()
					#faz recursivamente para os atributos fora da definicao do bloco
					self.desenha(entidade, offset)
						
				elif tipo == 'ATTRIB':
					pt1 = [0,0,0]
					pt2 = [0,0,0]
					segundo_ponto = 0
					tam = 0
					rot = 0
					alin = [0,1]
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
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
					
					valor = dxf_proc_grupo(entidade, 40)
					if len(valor)>0: tam = valor[0]
					
					valor = dxf_proc_grupo(entidade, 50)
					if len(valor)>0: rot = valor[0]
					
					valor = dxf_proc_grupo(entidade, 1)
					if len(valor)>0: texto = valor[0]
					
					valor = dxf_proc_grupo(entidade, 72)
					if len(valor)>0: alin[0] = valor[0]
					
					valor = dxf_proc_grupo(entidade, 74)
					if len(valor)>0: alin[1] = valor[0]
					
					if self.tela:
						if segundo_ponto:
							self.tela.texto(texto, pt1[:2], pt2[:2], tam, rot, self.t_cor[cor], alin)
						else:
							self.tela.texto(texto, pt1[:2], pt1[:2], tam, rot, self.t_cor[cor], alin)
				elif tipo == 'POLYLINE':
					self.desenha(entidade, offset) #faz recursivamente
					pt_ant = None
					pt_final = None
					
				elif tipo == 'VERTEX':
					
					pt1 = [0,0,0]
					
					valor = dxf_proc_grupo(entidade, 10)
					if len(valor)>0: pt1[0] = valor[0] + offset[0]
					
					valor = dxf_proc_grupo(entidade, 20)
					if len(valor)>0: pt1[1] = valor[0] + offset[1]
					
					valor = dxf_proc_grupo(entidade, 30)
					if len(valor)>0: pt1[2] = valor[0] + offset[2]
					
					valor = dxf_proc_grupo(entidade, 42)
					if len(valor)>0:
						bulge = valor[0]
					else:
						bulge = 0
					
					#verifica se nao eh o primeiro ponto
					if pt_ant != None and self.tela:
						if bulge_ant == 0:
							self.tela.linha(pt_ant[:2], pt1[:2], self.t_cor[cor])
						else:
							self.tela.arco_bulge(pt_ant[:2], pt1[:2], bulge_ant, self.t_cor[cor])
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
					#print entidade.imprime() #---------------debug
					
				elif tipo == '3DFACE':
					pass
				elif tipo == 'VIEWPORT':
					pass
				elif tipo == 'DIMENSION':
					pass
				else:
					pass
				#print tipo #-----------debug
				#print offset
			else: #apos o fechamento do loop, verifica se ha pontos pendentes de desenho (polylines fechadas)
				if pt_final != None and self.tela:
					if bulge_ant == 0:
						self.tela.linha(pt_ant[:2], pt_final[:2], self.t_cor[cor])
					else:
						self.tela.arco_bulge(pt_ant[:2], pt_final[:2], bulge_ant, self.t_cor[cor])