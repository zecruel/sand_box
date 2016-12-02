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
					atual.conteudo.append([imediato, grupo, valor]) #acrecenta os campos ao conteudo do objeto atual
					nivel = atual #muda o nivel
				else:
					atual.conteudo.append([imediato, grupo, valor]) #acrecenta os campos ao conteudo do objeto atual
				
				#if type(valor) is str:
	return drawing
	
def dxf_escreve(arquivo, objeto_dxf, recur=1 ):
	#with open(arquivo) as f:
	#presume-se que o arquivo esteja aberto
	if isinstance(objeto_dxf, obj_dxf):
		#Escreve o nome da entidade, se houver
		if objeto_dxf.nome != '':
			arquivo.write('0' + '\n')
			arquivo.write(str(objeto_dxf.nome).encode('ascii') + '\n')
		
		sequencia = 0
		
		for i in objeto_dxf.conteudo: #varre o conteudo do obj
			if isinstance(i, list):
				arquivo.write(str(i[1]).encode('ascii') + '\n')
				arquivo.write(str(i[2]).encode('ascii') + '\n')
				if (i[0] == 'ef_flag') and (i[2] != 0):
					sequencia = 1
			elif (isinstance(i, obj_dxf)) and recur: #caso recursivo
				dxf_escreve(arquivo, i, recur) #passa a escrever os filhos
		else:
			#Termina os objetos compostos
			if objeto_dxf.nome == 'SECTION': #secoes
				arquivo.write('0' + '\n')
				arquivo.write('ENDSEC' + '\n')
			elif objeto_dxf.nome == 'TABLE': #tabelas
				arquivo.write('0' + '\n')
				arquivo.write('ENDTAB' + '\n')
			elif objeto_dxf.nome == 'BLOCK': #blocos
				arquivo.write('0' + '\n')
				arquivo.write('ENDBLK' + '\n')
			elif (objeto_dxf.nome != '') and (sequencia == 1): #outros que tiverem a flag de sequencia
				arquivo.write('0' + '\n')
				arquivo.write('SEQEND' + '\n')
		#termina o dxf, se for um drawing
		#if objeto_dxf.nome == '':
		#	arquivo.write('0' + '\n')
		#	arquivo.write('EOF' + '\n')

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
		
def dxf_itens(mestre, tipo):
	'''Lista o nome dos itens dentro de um mestre DXF, que pode der uma secao, tabela ou bloco.
	Deve-se especificar as strings de tipo (conforme especificacao DXF) '''
	
	lista = [] #lista mestre de retorno
	if isinstance(mestre, obj_dxf):
		for i in mestre.conteudo: #varre o conteudo da tabela
			if isinstance(i, obj_dxf):
				if i.nome == tipo: #verifica o tipo
					j = dxf_proc_grupo(i, 2)
					if len(j) > 0:
						lista.append(j[0])
					#else:
					#	lista.append('')
	return lista

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