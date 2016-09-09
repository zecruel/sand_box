class ponto:
	def __init__(self, x = 0.0, y = 0.0, z = 0.0):
		self.x = x
		self.y = y
		self.z = z

class obj_dxf:
	def __init__(self, x = 0, nome = ''):
		self.up = x
		self.nome = nome
		self.conteudo = []
	def imprime(self,indent=''):
		buffer = ''
		for i in self.conteudo:
			if isinstance(i, obj_dxf):
				buffer += indent + i.nome + ':\n'
				buffer += i.imprime(indent +'    ')
			else:
				buffer += indent + str(i) + '\n'
		return buffer

def dxf_le(arquivo):
	with open(arquivo) as f:
		#funcao para conversao de tipo de acordo com o valor de grupo DXF
		tipo = str
		grupo = 0 #grupo DXF
		imediato = 'n_def'#grupo DXF fixo
		drawing = obj_dxf()
		atual = drawing
		nivel = drawing
		
		for num_lin, lin in enumerate(f):
			# retira os separadores de linha e caracteres de espaco
			lin = lin.strip(' \t\n\r')
			if (num_lin % 2 == 0):
				grupo = int(lin)
				
				#identifica o tipo pela faixa de valores do grupo
				if (grupo >= 0) and (grupo < 10):
					tipo = str #o tipo item eh uma string
				elif (grupo >= 10) and (grupo < 60):
					tipo = float #o tipo item eh um float
				elif (grupo >= 60) and (grupo < 80):
					tipo = int #o tipo item eh um int
				elif (grupo >= 140) and (grupo < 148):
					tipo = float #o tipo item eh um float
				elif (grupo >= 170) and (grupo < 176):
					tipo = int #o tipo item eh um int
				elif (grupo >= 210) and (grupo < 240):
					tipo = float #o tipo item eh um float
				elif (grupo >= 999) and (grupo < 1010):
					tipo = str #o tipo item eh uma string
				elif (grupo >= 1010) and (grupo < 1060):
					tipo = float #o tipo item eh um float
				elif (grupo >= 1060) and (grupo < 1080):
					tipo = int #o tipo item eh um int
				else: #tipo nao definido
					tipo = str #o tipo item eh uma string
				
				#identifica os grupos de funcao DXF fixos
				if grupo == 0:
					imediato = 'start'
				elif grupo == 5:
					imediato = 'handle'
				elif grupo == 6:
					imediato = 'l_type'
				elif grupo == 7:
					imediato = 't_style'
				elif grupo == 8:
					imediato = 'layer'
				elif grupo == 9:
					imediato = 'var'
				elif grupo == 38:
					imediato = 'elev'
				elif grupo == 39:
					imediato = 'tick'
				elif grupo == 62:
					imediato = 'color'
				elif grupo == 66:
					imediato = 'ef_flag'
				elif grupo == 67:
					imediato = 'model_paper'
				elif grupo == 210:
					imediato = 'extr_x'
				elif grupo == 220:
					imediato = 'extr_y'
				elif grupo == 230:
					imediato = 'extr_z'
				elif grupo == 999:
					imediato = 'comment'
				elif grupo == 1001:
					imediato = 'app'
				elif grupo == 1002:
					imediato = 'ctrl'
				#----------- coordenadas
				elif (grupo >= 10) and (grupo < 19):
					imediato = 'x'
				elif (grupo >= 20) and (grupo < 29):
					imediato = 'y'
				elif (grupo >= 30) and (grupo < 38):
					imediato = 'z'
				#----------
				else:
					imediato = 'n_def'
				
			else:
				# pega o valor do grupo conforme tipo definido anteriormente
				valor = tipo(lin)
				
				if imediato == 'start': #se eh o inicio de um objeto
					chave = valor.upper() #coloca em caixa alta para evitar erros de comparacao
					#acrescenta um nivel de hierarquia
					if (chave == 'SECTION') or (
					chave == 'TABLE') or (
					chave == 'BLOCK'):
						atual.conteudo.append(obj_dxf(atual, chave)) #cria um novo objeto e o acrescenta na lista pai
						atual = atual.conteudo[-1] #o obj pai passa a ser ultimo obj filho
						nivel = atual #muda o nivel
					#volta a um nivel superior
					elif (chave == 'ENDSEC') or (
					chave == 'ENDTAB') or (
					chave == 'ENDBLK') or (
					chave == 'SEQEND'):
						atual = nivel #vai pra o nivel atual
						try:
							atual = atual.up #volta ao nivel pai
						except AttributeError: #caso o arquivo DXF nao for bem formatado
							atual = drawing #volta ao nivel mestre
						nivel = atual
					else: #se for um objeto ordinario
						atual = nivel #dentro do nivel atual
						atual.conteudo.append(obj_dxf(atual, chave)) #cria um novo objeto e o acrescenta na lista pai
						atual = atual.conteudo[-1] #o obj pai passa a ser ultimo obj filho
				#quando a flag "Entities follow" for ativada, acrescenta um nivel de hierarquia no objeto atual
				elif (imediato == 'ef_flag') and (valor != 0):
						nivel = atual #muda o nivel
				else:
					atual.conteudo.append([imediato, grupo, valor]) #acrecenta os campos ao conteudo do objeto atual
				
				#if type(valor) is str:
	return drawing

def dxf_proc_fixo(objeto_dxf, fixo, recur = 0):
	'''Busca um grupo fixo (string) dentro de um objeto DXF e retorna seus valores numa lista.
	Pode ser passada a flag de recursividade, onde ira buscar o valor dentro dos subobjetos.'''
	
	lista = []
	if isinstance(objeto_dxf, obj_dxf):
		for i in objeto_dxf.conteudo: #varre o conteudo do obj
			if isinstance(i, list):
				if i[0] == fixo: #procura o fixo
					lista += [i[2]] #acrescenta o valor a lista de retorno (pode ser recursivo)
			elif (isinstance(i, obj_dxf)) and recur: #caso recursivo
				lista += dxf_proc_fixo(i, fixo, recur) #procura nos filhos
	return lista
		
def dxf_proc_grupo(objeto_dxf, grupo, recur = 0):
	'''Busca um grupo inominado dentro de um objeto DXF e seus valores numa lista.
	Pode ser passada a flag de recursividade, onde ira buscar o valor dentro dos subobjetos.'''
	
	lista = []
	if isinstance(objeto_dxf, obj_dxf):
		for i in objeto_dxf.conteudo: #varre o conteudo do obj
			if isinstance(i, list):
				if i[1] == grupo: #procura o grupo
					lista += [i[2]] #acrescenta o valor a lista de retorno (pode ser recursivo)
			elif (isinstance(i, obj_dxf)) and recur: #caso recursivo
				lista += dxf_proc_grupo(i, grupo, recur) #procura nos filhos
	return lista

def dxf_item(mestre, tipo, item):
	'''Busca um item dentro de um mestre DXF, que pode der uma secao, tabela ou bloco.
	Deve-se especificar as strings de tipo (conforme especificacao DXF) 
	e com o nome do item buscado (que sera buscado no valor do grupo 2)'''
	if isinstance(mestre, obj_dxf):
		for i in mestre.conteudo: #varre o conteudo da tabela
			if isinstance(i, obj_dxf):
				if i.nome == tipo: #verifica o tipo
					j = dxf_proc_grupo(i, 2)
					if len(j) > 0:
						if j[0] == item:
							return i #sucesso: retorna o objeto
		else:
			return None #se chegou ao final, entao nao encontrou						return i #sucesso: retorna o objeto
	else:
		return None #se chegou ao final, entao nao encontrou

def dxf_vars(head):
	''' Lista as variaveis dentro da secao HEADER DXF.
	Retorna uma lista onde cada elemento eh tambem uma lista para cada variavel e seus valores'''
	
	lista = [] #lista mestre de retorno
	atual = None
	if isinstance(head, obj_dxf):
		for idx, i in enumerate(head.conteudo): #varre o conteudo do obj
			if isinstance(i, list):
				if i[0] == 'var': #procura o grupo
					exec('lis%d = []' % idx) #cria uma lista derivada (lis0, lis1, lis2, ...)
					exec('atual = lis%d' % idx) #e a define como a lista atual
					lista.append(atual) #acrescenta a lista derivada a lista mestre de retorno
					atual.append(i[2]) #o primeiro item da lista derivada eh o nome da variavel
				else:
					if isinstance(atual, list): atual.append(i)
	return lista

def dxf_des(drawing):
	'''desmonta o DXF em suas secoes principais '''
	
	head = dxf_item(drawing, 'SECTION', 'HEADER')
	tabs = dxf_item(drawing, 'SECTION', 'TABLES')
	blks = dxf_item(drawing, 'SECTION', 'BLOCKS')
	ents = dxf_item(drawing, 'SECTION', 'ENTITIES')
	return head, tabs, blks, ents
	
def dxf_tabs(tabs):
	'''desmonta as tabelas DXF em seus tipos principais '''
	
	t_ltype = dxf_item(tabs, 'TABLE', 'LTYPE')
	t_layer = dxf_item(tabs, 'TABLE', 'LAYER')
	t_style = dxf_item(tabs, 'TABLE', 'STYLE')
	t_view = dxf_item(tabs, 'TABLE', 'VIEW')
	t_ucs = dxf_item(tabs, 'TABLE', 'UCS')
	t_vport = dxf_item(tabs, 'TABLE', 'VPORT')
	t_dimstyle = dxf_item(tabs, 'TABLE', 'DIMSTYLE')
	t_appid = dxf_item(tabs, 'TABLE', 'APPID')
	return t_vport, t_layer, t_style, t_view, t_ucs, t_vport, t_dimstyle, t_appid
	
def dxf_desenha(mestre, t_cor, desenho=None, blocos=None, layers=None, ltypes=None, offset = [0,0,0]):
	
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
					layer = dxf_item(layers, 'LAYER', n_layer) #objeto layer
					
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
				ltype = dxf_item(ltypes, 'LTYPE', ltype_n) #objeto ltype
				valor = dxf_proc_grupo(ltype, 49)
				if len(valor)>0:
					#escalona o pattern (minimo 4 pixels)
					minimo = min([abs(x) for x in valor])
					pattern = [4*x/minimo for x in valor]
				
				#muda o pattern do desenho
				if desenho:
					desenho.pattern = pattern
					desenho.patt_a = 0
				
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
				
				if desenho:
					desenho.linha(pt1[:2], pt2[:2], t_cor[cor])
				
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
				
				if desenho:
					desenho.circulo(pt1[:2], raio, t_cor[cor])
				
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
				
				if desenho:
					desenho.arco(pt1[:2], raio, ang_ini, ang_fim, t_cor[cor])
					
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
				
				if desenho:
					desenho.linha(pt1[:2], pt2[:2], t_cor[cor])
					desenho.linha(pt1[:2], pt3[:2], t_cor[cor])
					desenho.linha(pt2[:2], pt4[:2], t_cor[cor])
					desenho.linha(pt3[:2], pt4[:2], t_cor[cor])
					
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
				
				if desenho:
					desenho.linha(pt1[:2], pt2[:2], t_cor[cor])
					desenho.linha(pt1[:2], pt3[:2], t_cor[cor])
					if quarto_ponto:
						desenho.linha(pt2[:2], pt4[:2], t_cor[cor])
						desenho.linha(pt3[:2], pt4[:2], t_cor[cor])
					else:
						desenho.linha(pt2[:2], pt3[:2], t_cor[cor])
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
				
				if desenho:
					if segundo_ponto:
						desenho.texto(texto, pt1[:2], pt2[:2], tam, rot, t_cor[cor], alin)
					else:
						desenho.texto(texto, pt1[:2], pt1[:2], tam, rot, t_cor[cor], alin)
					
				#print entidade.imprime() #---------------debug
				
			elif tipo == 'SHAPE':
				pass
			elif tipo == 'INSERT':
				if blocos:
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
					
					blk = dxf_item(blocos, 'BLOCK', nome_blk)
					dxf_desenha(blk, t_cor, desenho, None, layers, ltypes, pt1) #faz recursivamente os blocos
					#print pt1
					#print entidade.imprime()
				#faz recursivamente para os atributos fora da definicao do bloco
				dxf_desenha(entidade, t_cor, desenho, None, layers, ltypes)
					
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
				
				if desenho:
					if segundo_ponto:
						desenho.texto(texto, pt1[:2], pt2[:2], tam, rot, t_cor[cor], alin)
					else:
						desenho.texto(texto, pt1[:2], pt1[:2], tam, rot, t_cor[cor], alin)
			elif tipo == 'POLYLINE':
				dxf_desenha(entidade, t_cor, desenho, None, layers, ltypes, offset) #faz recursivamente
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
				if pt_ant != None and desenho:
					if bulge_ant == 0:
						desenho.linha(pt_ant[:2], pt1[:2], t_cor[cor])
					else:
						desenho.arco_bulge(pt_ant[:2], pt1[:2], bulge_ant, t_cor[cor])
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
			if pt_final != None and desenho:
				#desenho.linha(pt_ant[:2], pt_final[:2], t_cor[cor])
				if bulge_ant == 0:
					desenho.linha(pt_ant[:2], pt_final[:2], t_cor[cor])
				else:
					desenho.arco_bulge(pt_ant[:2], pt_final[:2], bulge_ant, t_cor[cor])
				

def init_color():
	'''Gera a tabela de cores padrao Autocad em uma lista,
	com indices de 0 a 255. Os elementos sao tuplas
	com valores RGB.'''
	color_table = []
	color_table.append((0, 0, 0))
	color_table.append((255, 0, 0))
	color_table.append((255, 255, 0))
	color_table.append((0, 255, 0))
	color_table.append((0, 255, 255))
	color_table.append((0, 0, 255))
	color_table.append((255, 0, 255))
	color_table.append((255, 255, 255))
	color_table.append((65, 65, 65))
	color_table.append((128, 128, 128))
	color_table.append((255, 0, 0))
	color_table.append((255, 170, 170))
	color_table.append((189, 0, 0))
	color_table.append((189, 126, 126))
	color_table.append((129, 0, 0))
	color_table.append((129, 86, 86))
	color_table.append((104, 0, 0))
	color_table.append((104, 69, 69))
	color_table.append((79, 0, 0))
	color_table.append((79, 53, 53))
	color_table.append((255, 63, 0))
	color_table.append((255, 191, 170))
	color_table.append((189, 46, 0))
	color_table.append((189, 141, 126))
	color_table.append((129, 31, 0))
	color_table.append((129, 96, 86))
	color_table.append((104, 25, 0))
	color_table.append((104, 78, 69))
	color_table.append((79, 19, 0))
	color_table.append((79, 59, 53))
	color_table.append((255, 127, 0))
	color_table.append((255, 212, 170))
	color_table.append((189, 94, 0))
	color_table.append((189, 157, 126))
	color_table.append((129, 64, 0))
	color_table.append((129, 107, 86))
	color_table.append((104, 52, 0))
	color_table.append((104, 86, 69))
	color_table.append((79, 39, 0))
	color_table.append((79, 66, 53))
	color_table.append((255, 191, 0))
	color_table.append((255, 234, 170))
	color_table.append((189, 141, 0))
	color_table.append((189, 173, 126))
	color_table.append((129, 96, 0))
	color_table.append((129, 118, 86))
	color_table.append((104, 78, 0))
	color_table.append((104, 95, 69))
	color_table.append((79, 59, 0))
	color_table.append((79, 73, 53))
	color_table.append((255, 255, 0))
	color_table.append((255, 255, 170))
	color_table.append((189, 189, 0))
	color_table.append((189, 189, 126))
	color_table.append((129, 129, 0))
	color_table.append((129, 129, 86))
	color_table.append((104, 104, 0))
	color_table.append((104, 104, 69))
	color_table.append((79, 79, 0))
	color_table.append((79, 79, 53))
	color_table.append((191, 255, 0))
	color_table.append((234, 255, 170))
	color_table.append((141, 189, 0))
	color_table.append((173, 189, 126))
	color_table.append((96, 129, 0))
	color_table.append((118, 129, 86))
	color_table.append((78, 104, 0))
	color_table.append((95, 104, 69))
	color_table.append((59, 79, 0))
	color_table.append((73, 79, 53))
	color_table.append((127, 255, 0))
	color_table.append((212, 255, 170))
	color_table.append((94, 189, 0))
	color_table.append((157, 189, 126))
	color_table.append((64, 129, 0))
	color_table.append((107, 129, 86))
	color_table.append((52, 104, 0))
	color_table.append((86, 104, 69))
	color_table.append((39, 79, 0))
	color_table.append((66, 79, 53))
	color_table.append((63, 255, 0))
	color_table.append((191, 255, 170))
	color_table.append((46, 189, 0))
	color_table.append((141, 189, 126))
	color_table.append((31, 129, 0))
	color_table.append((96, 129, 86))
	color_table.append((25, 104, 0))
	color_table.append((78, 104, 69))
	color_table.append((19, 79, 0))
	color_table.append((59, 79, 53))
	color_table.append((0, 255, 0))
	color_table.append((170, 255, 170))
	color_table.append((0, 189, 0))
	color_table.append((126, 189, 126))
	color_table.append((0, 129, 0))
	color_table.append((86, 129, 86))
	color_table.append((0, 104, 0))
	color_table.append((69, 104, 69))
	color_table.append((0, 79, 0))
	color_table.append((53, 79, 53))
	color_table.append((0, 255, 63))
	color_table.append((170, 255, 191))
	color_table.append((0, 189, 46))
	color_table.append((126, 189, 141))
	color_table.append((0, 129, 31))
	color_table.append((86, 129, 96))
	color_table.append((0, 104, 25))
	color_table.append((69, 104, 78))
	color_table.append((0, 79, 19))
	color_table.append((53, 79, 59))
	color_table.append((0, 255, 127))
	color_table.append((170, 255, 212))
	color_table.append((0, 189, 94))
	color_table.append((126, 189, 157))
	color_table.append((0, 129, 64))
	color_table.append((86, 129, 107))
	color_table.append((0, 104, 52))
	color_table.append((69, 104, 86))
	color_table.append((0, 79, 39))
	color_table.append((53, 79, 66))
	color_table.append((0, 255, 191))
	color_table.append((170, 255, 234))
	color_table.append((0, 189, 141))
	color_table.append((126, 189, 173))
	color_table.append((0, 129, 96))
	color_table.append((86, 129, 118))
	color_table.append((0, 104, 78))
	color_table.append((69, 104, 95))
	color_table.append((0, 79, 59))
	color_table.append((53, 79, 73))
	color_table.append((0, 255, 255))
	color_table.append((170, 255, 255))
	color_table.append((0, 189, 189))
	color_table.append((126, 189, 189))
	color_table.append((0, 129, 129))
	color_table.append((86, 129, 129))
	color_table.append((0, 104, 104))
	color_table.append((69, 104, 104))
	color_table.append((0, 79, 79))
	color_table.append((53, 79, 79))
	color_table.append((0, 191, 255))
	color_table.append((170, 234, 255))
	color_table.append((0, 141, 189))
	color_table.append((126, 173, 189))
	color_table.append((0, 96, 129))
	color_table.append((86, 118, 129))
	color_table.append((0, 78, 104))
	color_table.append((69, 95, 104))
	color_table.append((0, 59, 79))
	color_table.append((53, 73, 79))
	color_table.append((0, 127, 255))
	color_table.append((170, 212, 255))
	color_table.append((0, 94, 189))
	color_table.append((126, 157, 189))
	color_table.append((0, 64, 129))
	color_table.append((86, 107, 129))
	color_table.append((0, 52, 104))
	color_table.append((69, 86, 104))
	color_table.append((0, 39, 79))
	color_table.append((53, 66, 79))
	color_table.append((0, 63, 255))
	color_table.append((170, 191, 255))
	color_table.append((0, 46, 189))
	color_table.append((126, 141, 189))
	color_table.append((0, 31, 129))
	color_table.append((86, 96, 129))
	color_table.append((0, 25, 104))
	color_table.append((69, 78, 104))
	color_table.append((0, 19, 79))
	color_table.append((53, 59, 79))
	color_table.append((0, 0, 255))
	color_table.append((170, 170, 255))
	color_table.append((0, 0, 189))
	color_table.append((126, 126, 189))
	color_table.append((0, 0, 129))
	color_table.append((86, 86, 129))
	color_table.append((0, 0, 104))
	color_table.append((69, 69, 104))
	color_table.append((0, 0, 79))
	color_table.append((53, 53, 79))
	color_table.append((63, 0, 255))
	color_table.append((191, 170, 255))
	color_table.append((46, 0, 189))
	color_table.append((141, 126, 189))
	color_table.append((31, 0, 129))
	color_table.append((96, 86, 129))
	color_table.append((25, 0, 104))
	color_table.append((78, 69, 104))
	color_table.append((19, 0, 79))
	color_table.append((59, 53, 79))
	color_table.append((127, 0, 255))
	color_table.append((212, 170, 255))
	color_table.append((94, 0, 189))
	color_table.append((157, 126, 189))
	color_table.append((64, 0, 129))
	color_table.append((107, 86, 129))
	color_table.append((52, 0, 104))
	color_table.append((86, 69, 104))
	color_table.append((39, 0, 79))
	color_table.append((66, 53, 79))
	color_table.append((191, 0, 255))
	color_table.append((234, 170, 255))
	color_table.append((141, 0, 189))
	color_table.append((173, 126, 189))
	color_table.append((96, 0, 129))
	color_table.append((118, 86, 129))
	color_table.append((78, 0, 104))
	color_table.append((95, 69, 104))
	color_table.append((59, 0, 79))
	color_table.append((73, 53, 79))
	color_table.append((255, 0, 255))
	color_table.append((255, 170, 255))
	color_table.append((189, 0, 189))
	color_table.append((189, 126, 189))
	color_table.append((129, 0, 129))
	color_table.append((129, 86, 129))
	color_table.append((104, 0, 104))
	color_table.append((104, 69, 104))
	color_table.append((79, 0, 79))
	color_table.append((79, 53, 79))
	color_table.append((255, 0, 191))
	color_table.append((255, 170, 234))
	color_table.append((189, 0, 141))
	color_table.append((189, 126, 173))
	color_table.append((129, 0, 96))
	color_table.append((129, 86, 118))
	color_table.append((104, 0, 78))
	color_table.append((104, 69, 95))
	color_table.append((79, 0, 59))
	color_table.append((79, 53, 73))
	color_table.append((255, 0, 127))
	color_table.append((255, 170, 212))
	color_table.append((189, 0, 94))
	color_table.append((189, 126, 157))
	color_table.append((129, 0, 64))
	color_table.append((129, 86, 107))
	color_table.append((104, 0, 52))
	color_table.append((104, 69, 86))
	color_table.append((79, 0, 39))
	color_table.append((79, 53, 66))
	color_table.append((255, 0, 63))
	color_table.append((255, 170, 191))
	color_table.append((189, 0, 46))
	color_table.append((189, 126, 141))
	color_table.append((129, 0, 31))
	color_table.append((129, 86, 96))
	color_table.append((104, 0, 25))
	color_table.append((104, 69, 78))
	color_table.append((79, 0, 19))
	color_table.append((79, 53, 59))
	color_table.append((51, 51, 51))
	color_table.append((80, 80, 80))
	color_table.append((105, 105, 105))
	color_table.append((130, 130, 130))
	color_table.append((190, 190, 190))
	color_table.append((255, 255, 255))
	return color_table

if __name__ == "__main__":
	import tkFileDialog
	
	import window
	jan = window.viewer(args=(1,2,3),
		kwargs = {
		'largura': 800,
		'altura': 600,
		'titulo': 'Visualisador DXF'
		})
	
	arquivo = tkFileDialog.askopenfilename(title='Abrir')
	if arquivo:
		#le o arquivo em uma estrutura DXF
		drawing = dxf_le(arquivo)
		
		#imprime a estrutura DXF indentada
		#print drawing.imprime()
		
		#obtem as secoes basicas do DXF
		head, tabs, blks, ents = dxf_des(drawing)
		
		#obtem as variaveis do cabecalho DXF
		l_var = dxf_vars(head)
		#print l_var
		
		#obtem os Layers cadastrados
		layers = dxf_item(tabs, 'TABLE', 'LAYER')
		
		#obtem os tipos de linha cadastrados
		ltypes = dxf_item(tabs, 'TABLE', 'LTYPE')
		
		#procura um layer chamado FRAME
		l_frame = dxf_item(layers, 'LAYER', 'FRAME')
		#if isinstance(l_frame, obj_dxf): print l_frame.conteudo
		
		#procura um bloco chamado *D6
		blk_d6 = dxf_item(blks, 'BLOCK', '*D6')
		#if isinstance(blk_d6, obj_dxf): print blk_d6.conteudo
		
		#gera a tabela de cores padrao Autocad em uma lista
		color_t = init_color()
		
		#busca os valores do grupo 40 dentro da secao ents, com recursividade
		valor = dxf_proc_grupo(ents, 40, 1) #testa a recursividade
		#print valor
		
		#busca os valores de x dentro das Entidades, com recursividade
		lista_x = dxf_proc_fixo(ents, 'x', 1)
		#print lista_x
		
		#listas das outras coordenadas
		lista_y = dxf_proc_fixo(ents, 'y', 1)
		lista_z = dxf_proc_fixo(ents, 'z', 1)
		
		#encontra os limites dos objetos
		maior = max(lista_x)
		menor = min(lista_x)
		lim_x = [menor, maior]
		
		maior = max(lista_y)
		menor = min(lista_y)
		lim_y = [menor, maior]
		
		#maior = max(lista_z)
		#menor = min(lista_z)
		#lim_z = [menor, maior]
		
		zoom_x = 800/(lim_x[1] - lim_x[0])
		zoom_y = 600/(lim_y[1] - lim_y[0])
		
		jan.zoom = min([zoom_x, zoom_y])
		jan.offset = [i * jan.zoom for i in [lim_x[0], lim_y[0]]]
		
		'''
		print lim_x
		print lim_y
		print jan.offset
		print jan.zoom
		'''
		
		
		class desenho:
			def __init__(self, ents, color_t, jan, blks, layers, ltypes):
				self.ents = ents
				self.color_t = color_t
				self.jan = jan
				self.blks = blks
				self.layers = layers
				self.ltypes = ltypes
			def redesenha(self):
				self.jan.limpa()
				#self.jan.screen.fill((0, 0, 0))
				dxf_desenha(self.ents, self.color_t, self.jan, self.blks, self.layers, self.ltypes)
				#print jan.zoom, jan.offset
				self.jan.exibe()
		
		des = desenho(ents, color_t, jan, blks, layers, ltypes)
		des.redesenha()
		jan.redesenha = des.redesenha