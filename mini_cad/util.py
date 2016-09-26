from threading import Lock
from dxf import obj_dxf

def intersect(lin_pt1, lin_pt2, rect_pt1, rect_pt2):
	'''testa se uma linha intercepta um retangulo'''
	#ordena os cantos do retangulo
	r_bl_x = min(rect_pt1[0], rect_pt2[0])
	r_bl_y = min(rect_pt1[1], rect_pt2[1])
	r_tr_x = max(rect_pt1[0], rect_pt2[0])
	r_tr_y = max(rect_pt1[1], rect_pt2[1])
	
	#ordena os cantos do retangulo da linha
	l_bl_x = min(lin_pt1[0], lin_pt2[0])
	l_bl_y = min(lin_pt1[1], lin_pt2[1])
	l_tr_x = max(lin_pt1[0], lin_pt2[0])
	l_tr_y = max(lin_pt1[1], lin_pt2[1])
	
	#verifica se a linha esta fora de alcance do retangulo
	if ((lin_pt1[0] > r_tr_x) and (lin_pt2[0] > r_tr_x)) or ( #linha a direita
	(lin_pt1[1] > r_tr_y) and (lin_pt2[1] > r_tr_y)) or (  #linha acima
	(lin_pt1[0] < r_bl_x) and (lin_pt2[0] < r_bl_x)) or ( #linha a esquerda
	(lin_pt1[1] < r_bl_y) and (lin_pt2[1] < r_bl_y)): #linha abaixo
		return 0
	else: #verifica se a linha cruza o retangulo
		#calcula as polarizacoes dos quatro cantos do retangulo
		a = lin_pt2[1]-lin_pt1[1]
		b = lin_pt1[0]-lin_pt2[0]
		c = lin_pt2[0]*lin_pt1[1] - lin_pt1[0]*lin_pt2[1]
		
		pol1 = a*rect_pt1[0] + b*rect_pt1[1] + c
		pol2 = a*rect_pt1[0] + b*rect_pt2[1] + c
		pol3 = a*rect_pt2[0] + b*rect_pt1[1] + c
		pol4 = a*rect_pt2[0] + b*rect_pt2[1] + c
		if ((pol1>=0) and (pol2>=0) and (pol3>=0) and (pol4>=0)) or(
		(pol1<0) and (pol2<0) and (pol3<0) and (pol4<0)):
			return 0
		return 1

class selecao:
	lista_busca = []
	tam_area = 5 #tamando da area de busca em pixels
	trava_busca = Lock()
	
	lista_selecao = obj_dxf()
	trava_selec = Lock()
	
	modo = 'toggle'
	
	def add_selec(self, selec):
		self.trava_selec.acquire()
		if isinstance(selec, obj_dxf):
			if self.lista_selecao.conteudo.count(selec) == 0:
				self.lista_selecao.conteudo.append(selec)
				#print selec.nome
			elif self.modo == 'toggle':
				indice = self.lista_selecao.conteudo.index(selec)
				self.lista_selecao.conteudo.pop(indice)
		self.trava_selec.release()
	
	#funcoes para busca grafica
	def add_line(self, entidade, vertices):
		self.trava_busca.acquire()
		lista = []
		lista.append(entidade)
		lista.append(vertices)
		self.lista_busca.append(lista)
		self.trava_busca.release()
		
	def busca(self, rect_pt1, rect_pt2):
		self.trava_busca.acquire()
		for i in self.lista_busca:
			if intersect(i[1][0], i[1][1], rect_pt1, rect_pt2):
				self.trava_busca.release()
				return i[0]
		self.trava_busca.release()
		return None
		
	def graf_add_pt(self, pt1):
		#retangulo de busca
		rect_pt1 = [0,0]
		rect_pt2 = [0,0]
		rect_pt1[0] = pt1[0]-self.tam_area
		rect_pt1[1] = pt1[1]-self.tam_area
		rect_pt2[0] = pt1[0]+self.tam_area
		rect_pt2[1] = pt1[1]+self.tam_area
		selec = self.busca(rect_pt1, rect_pt2)
		self.add_selec(selec)