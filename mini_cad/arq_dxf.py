class ponto:
	def __init__(self, x = 0.0, y = 0.0, z = 0.0):
		self.x = x
		self.y = y
		self.z = z

def ini_dxf(blocos=''):
	buffer = ''
	buffer += '999\n'
	buffer += 'Criado por ZeCruel\n'
	#------------ Cabecalho -------------------
	buffer += '999\n'
	buffer += '----------------- Cabecalho -------------------\n'
	buffer += '0\n'
	buffer += 'SECTION\n'
	buffer += '2\n'
	buffer += 'HEADER\n'
	# variaveis
	buffer += '9\n'
	buffer += '$ACADVER\n' #versao do AutoCad
	buffer += '1\n'
	buffer += 'AC1009\n' #R12
	'''
	buffer += '9\n'
	buffer += '$LIMMIN\n'
	buffer += '10\n'
	buffer += '0\n'
	buffer += '20\n'
	buffer += '0\n'
	#buffer += '30\n'
	#buffer += '0\n'
	buffer += '9\n'
	buffer += '$LIMMAX\n'
	buffer += '10\n'
	buffer += '5.0\n'
	buffer += '20\n'
	buffer += '5.0\n'
	#buffer += '30\n'
	#buffer += '0\n'
	
	buffer += '9\n'
	buffer += '$INSBASE\n'
	buffer += '10\n'
	buffer += '0\n'
	buffer += '20\n'
	buffer += '0\n'
	['$ACADVER', ['n_def', 1, 'AC1014']
	'''
	buffer += '0\n'
	buffer += 'ENDSEC\n'
	
	#-------------- Tabelas de estilo ---------------
	buffer += '999\n'
	buffer += '----------------- Tabelas de estilo -------------------\n'
	buffer += '0\n'
	buffer += 'SECTION\n'
	buffer += '2\n'
	buffer += 'TABLES\n'
	buffer += '0\n'
	buffer += 'TABLE\n'
	buffer += '2\n'
	buffer += 'STYLE\n'
	buffer += '70\n'
	buffer += '2\n'
	buffer += '0\n'
	buffer += 'STYLE\n'
	buffer += '2\n'
	buffer += 'STANDARD\n'
	buffer += '70\n'
	buffer += '0\n'
	buffer += '40\n'
	buffer += '0.0\n'
	buffer += '41\n'
	buffer += '1.0\n'
	buffer += '50\n'
	buffer += '0.0\n'
	buffer += '71\n'
	buffer += '0\n'
	buffer += '42\n'
	buffer += '1\n'
	buffer += '3\n'
	buffer += 'INTL_ISO\n'
	buffer += '0\n'
	buffer += 'ENDTAB\n'
	buffer += '0\n'
	buffer += 'ENDSEC\n'
	
	#--------------------- Blocos ----------------
	buffer += '999\n'
	buffer += '----------------- Blocos -------------------\n'
	buffer += '0\n'
	buffer += 'SECTION\n'
	buffer += '2\n'
	buffer += 'BLOCKS\n'
	buffer += blocos
	buffer += '0\n'
	buffer += 'ENDSEC\n'
	
	#--------------------- Entidades ----------------
	buffer += '999\n'
	buffer += '----------------- Entidades -------------------\n'
	buffer += '0\n'
	buffer += 'SECTION\n'
	buffer += '2\n'
	buffer += 'ENTITIES\n'
	return buffer

def fim_dxf():
	buffer = ''
	buffer += '0\n'
	buffer += 'ENDSEC\n'
	buffer += '999\n'
	buffer += '----------------- FIM -------------------\n'
	buffer += '0\n'
	buffer += 'EOF\n'
	return buffer
	
def linha_dxf(pt1, pt2, layer, cor):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Linha ****\n'
	buffer += '0\n'
	buffer += 'LINE\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '11\n'
	buffer += str(pt2.x) + '\n'
	buffer += '21\n'
	buffer += str(pt2.y) + '\n'
	buffer += '31\n'
	buffer += str(pt2.z) + '\n'
	return buffer

def texto_dxf(pt1, tam, texto, layer, cor, alin_h=0, alin_v=1):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Texto ****\n'
	buffer += '0\n'
	buffer += 'TEXT\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '40\n'
	buffer += str(tam) + '\n'
	buffer += '11\n'
	buffer += str(pt1.x) + '\n'
	buffer += '21\n'
	buffer += str(pt1.y) + '\n'
	buffer += '31\n'
	buffer += str(pt1.z) + '\n'
	buffer += '1\n'
	buffer += texto + '\n'
	buffer += '72\n'
	buffer += str(alin_h) + '\n'   #alinhamento horizontal (0=left, 1=center, 2=right)
	buffer += '73\n'
	buffer += str(alin_v) + '\n'   #alinhamento vertical (0=baseline, 1=bottom, 2=midle, 3=top)
	return buffer
	
def bloco(pt1, nome, entid, layer, atrib=''):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Bloco ****\n'
	buffer += '0\n'
	buffer += 'BLOCK\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	
	buffer += '2\n'
	buffer += nome + '\n'           # nome do bloco
	
	buffer += '3\n'
	buffer += nome + '\n'           # nome do bloco
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	if atrib != '':
		buffer += '70\n' #flags
		buffer += '2\n'
		buffer += atrib
		#buffer += '0\n'
		#buffer += 'SEQEND\n'
	else:
		buffer += '70\n' #flags
		buffer += '0\n'
	buffer += entid
	buffer += '0\n'
	buffer += 'ENDBLK\n'
	return buffer

def ins_bloco(pt1, nome, layer, atrib=''):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Insere Bloco ****\n'
	buffer += '0\n'
	buffer += 'INSERT\n'
	
	buffer += '2\n'
	buffer += nome + '\n'           # nome do bloco
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	if atrib != '':
		buffer += '66\n' #flags
		buffer += '1\n'
		buffer += atrib
		buffer += '0\n'
		buffer += 'SEQEND\n'
	return buffer

def atrib_def(pt1, tam, texto, tag, layer, cor, alin_h=0, alin_v=1):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Def. Atributo ****\n'
	buffer += '0\n'
	buffer += 'ATTDEF\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '40\n'
	buffer += str(tam) + '\n'
	buffer += '11\n'
	buffer += str(pt1.x) + '\n'
	buffer += '21\n'
	buffer += str(pt1.y) + '\n'
	buffer += '31\n'
	buffer += str(pt1.z) + '\n'
	buffer += '1\n'
	buffer += texto + '\n'
	buffer += '72\n'
	buffer += str(alin_h) + '\n'   #alinhamento horizontal (0=left, 1=center, 2=right)
	buffer += '74\n'
	buffer += str(alin_v) + '\n'   #alinhamento vertical (0=baseline, 1=bottom, 2=midle, 3=top)
	buffer += '3\n'
	buffer += tag + '\n'
	buffer += '2\n'
	buffer += tag + '\n'
	buffer += '70\n'
	buffer += '0\n'
	return buffer
	
def atrib(pt1, tam, texto, tag, layer, cor, alin_h=0, alin_v=1):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Ins. Atributo ****\n'
	buffer += '0\n'
	buffer += 'ATTRIB\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '40\n'
	buffer += str(tam) + '\n'
	buffer += '11\n'
	buffer += str(pt1.x) + '\n'
	buffer += '21\n'
	buffer += str(pt1.y) + '\n'
	buffer += '31\n'
	buffer += str(pt1.z) + '\n'
	buffer += '1\n'
	buffer += texto + '\n'
	buffer += '72\n'
	buffer += str(alin_h) + '\n'   #alinhamento horizontal (0=left, 1=center, 2=right)
	buffer += '74\n'
	buffer += str(alin_v) + '\n'   #alinhamento vertical (0=baseline, 1=bottom, 2=midle, 3=top)
	buffer += '2\n'
	buffer += tag + '\n'
	buffer += '70\n'
	buffer += '8\n'
	return buffer
	
def poli(layer, cor, vertices):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Poli-line ****\n'
	buffer += '0\n'
	buffer += 'POLYLINE\n'
	
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	
	buffer += '10\n'
	buffer += '0\n'
	buffer += '20\n'
	buffer += '0\n'
	buffer += '30\n'
	buffer += '0\n'
	buffer += '70\n' #flags
	buffer += '0\n'
	buffer += '66\n'
	buffer += '1\n'
	buffer += vertices
	buffer += '0\n'
	buffer += 'SEQEND\n'
	return buffer
	
def vertice(pt1, layer, cor):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Vertice ****\n'
	buffer += '0\n'
	buffer += 'VERTEX\n'
	
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '70\n' #flags
	buffer += '0\n'
	return buffer
	
def trace(pt1, pt2, pt3, pt4, layer, cor):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Trace ****\n'
	buffer += '0\n'
	buffer += 'TRACE\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '11\n'
	buffer += str(pt2.x) + '\n'
	buffer += '21\n'
	buffer += str(pt2.y) + '\n'
	buffer += '31\n'
	buffer += str(pt2.z) + '\n'
	buffer += '12\n'
	buffer += str(pt3.x) + '\n'
	buffer += '22\n'
	buffer += str(pt3.y) + '\n'
	buffer += '32\n'
	buffer += str(pt3.z) + '\n'
	buffer += '13\n'
	buffer += str(pt4.x) + '\n'
	buffer += '23\n'
	buffer += str(pt4.y) + '\n'
	buffer += '33\n'
	buffer += str(pt4.z) + '\n'
	return buffer
	
def solid(pt1, pt2, pt3, pt4, layer, cor):
	buffer = ''
	buffer += '999\n'
	buffer += '**** Solid ****\n'
	buffer += '0\n'
	buffer += 'SOLID\n'
	buffer += '8\n'
	buffer += layer + '\n'         # layer
	buffer += '62\n'
	buffer += cor + '\n'           # cor
	buffer += '10\n'
	buffer += str(pt1.x) + '\n'
	buffer += '20\n'
	buffer += str(pt1.y) + '\n'
	buffer += '30\n'
	buffer += str(pt1.z) + '\n'
	buffer += '11\n'
	buffer += str(pt2.x) + '\n'
	buffer += '21\n'
	buffer += str(pt2.y) + '\n'
	buffer += '31\n'
	buffer += str(pt2.z) + '\n'
	buffer += '12\n'
	buffer += str(pt3.x) + '\n'
	buffer += '22\n'
	buffer += str(pt3.y) + '\n'
	buffer += '32\n'
	buffer += str(pt3.z) + '\n'
	buffer += '13\n'
	buffer += str(pt4.x) + '\n'
	buffer += '23\n'
	buffer += str(pt4.y) + '\n'
	buffer += '33\n'
	buffer += str(pt4.z) + '\n'
	return buffer

if __name__ == "__main__":
	layer = 'principal'
	cor = '0'
	pt1 = ponto(0,0,0)
	pt2 = ponto(1,1,0)
	pt3 = ponto(1,0,0)
	
	pt4 = ponto(2,0,0)
	pt5 = ponto(3,1,0)
	pt6 = ponto(4,0,0)
	
	blk1 = bloco(pt1, 'ret', 
			linha_dxf(pt1, pt2, layer, cor) +
			linha_dxf(pt2, pt3, layer, cor) +
			linha_dxf(pt1, pt3, layer, cor), layer,
			atrib_def(pt1, 0.1, 'a1', 'a', layer, cor))
	poli1 = poli(layer, cor,
			vertice(pt4, layer, cor) +
			vertice(pt5, layer, cor) +
			vertice(pt6, layer, cor))
	trace1 = trace(ponto(0,3,0),
			ponto(4,3,0),
			ponto(0.5,3.5,0),
			ponto(4.5,3.5,0),
			layer, cor)
	solid1 = solid(ponto(0,2,0),
			ponto(3,3,0),
			ponto(0.5,2.5,0),
			ponto(4.5,2.5,0),
			layer, cor)
	with open('saida.dxf', 'w') as arq:
		arq.write(ini_dxf(blk1) +
			texto_dxf(pt2, 0.25, 'teste', layer, cor) +
			ins_bloco(pt1, 'ret', layer, atrib(pt1, 0.1, 'a1', 'a', layer, cor)) +
			ins_bloco(pt2, 'ret', layer, atrib(pt2, 0.1, 'a2', 'a', layer, cor)) +
			ins_bloco(pt3, 'ret', layer, atrib(pt3, 0.1, 'a3', 'a', layer, cor) )+
			poli1 + trace1 + solid1 +
			fim_dxf())
	print 'Pronto!'
	