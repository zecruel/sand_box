# -*- coding: cp1252 -*-
import threading
import math
import sdl_event
import sdl
from ctypes import *
import util
import dxf
import shape


class viewer(threading.Thread):
	def __init__(self, group=None, target=None, name=None,
				args=(), kwargs={}, verbose=None):
		''' Deriva a classe threading, permitindo a passagem
		de argumentos na inicializacao da classe '''
		threading.Thread.__init__(self, group=group, target=target,
							name=name, verbose=verbose)
		
		#os argumentos passados sao armaz. em variaveis internas
		#args eh uma tupla e kwargs eh um dicionario
		self.args = args
		self.kwargs = kwargs
		
		#------------ parametros passados -----------
		self.largura = kwargs.pop('largura', 400)
		self.altura = kwargs.pop('altura', 400)
		self.fundo = kwargs.pop('fundo', (0,0,0))
		self.titulo = kwargs.pop('titulo', 'SDL 2.0')
	
		
		#------------- variaveis internas ---------------
		self.zoom = 1
		self.offset = [0,0]
		self.redesenha = None #funcao externa para redesenho
		
		self.plano = 'xy'
		
		self.sdl = sdl.sdl_dll() #carrega o dll do SDL 2.0
		self.lib = self.sdl.lib
		self.lib_ttf = self.sdl.lib_ttf
		self.pronto = threading.Condition() #informa que esta pronto
		
		self.fonte_shx = shape.fonte_shx()
		
		self.selec = util.selecao() #objeto auxiliar na selecao de elementos na tela
		
		#tipo de linhas
		self.pix_count = 0
		self.pattern = [1]
		self.patt_a = 0
		
		self.camadas=[]
		
		self.cor_hi = (255,0,255,120)
		
		self.lista_selecao = dxf.obj_dxf()
		
		#------------------------------------------------------------
		
		self.start()

	def run(self):
		''' Modifica o metodo run da classe threading'''
		
		#------------------------------------
		#inicializacao e criacao da janela SDL
		
		evento =sdl_event.SDL_Event()
		self.lib.SDL_Init(32)
		self.lib_ttf.TTF_Init()
		
		#cria as entidades SDL basicas
		self.window = self.lib.SDL_CreateWindow(self.titulo, 536805376, 536805376, self.largura, self.altura, 0) #janela com posicao indeterminada
		self.renderer = self.lib.SDL_CreateRenderer(self.window, -1, 0) #tela principal
		self.t_fundo = self.lib.SDL_CreateTexture(self.renderer, 373694468, 2, self.largura, self.altura) #imagem de fundo
		self.camadas.append(self.t_fundo)
		
		self.t_hilite = self.lib.SDL_CreateTexture(self.renderer, 373694468, 2, self.largura, self.altura)
		self.lib.SDL_SetTextureBlendMode(self.t_hilite, 1)
		self.camadas.append(self.t_hilite)
		
		self.fonte = self.lib_ttf.TTF_OpenFont(self.sdl.dir + 'SourceCodePro-ExtraLight.ttf', 20)
		
		#limpa a imagem de fundo com a cor determinada
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_fundo)
		self.lib.SDL_SetRenderDrawColor(self.renderer, self.fundo[0], self.fundo[1], self.fundo[2], 255)
		self.lib.SDL_RenderClear(self.renderer)
		
		#exibe a imagem de fundo na tela
		self.lib.SDL_SetRenderTarget(self.renderer, None)
		self.lib.SDL_RenderCopy(self.renderer, self.t_fundo, None, None)
		self.lib.SDL_RenderPresent(self.renderer)
		
		#depois de tudo pronto, notifica os processos em espera
		with self.pronto:
			self.pronto.notify()
		
		self.lib.SDL_StartTextInput()
		
		while 1:
			if self.lib.SDL_PollEvent(byref(evento)):
				if evento.type == 256: #evento de sair
					break
				elif (evento.type >= 768)&(evento.type < 1024): #eventos de teclado
					if evento.type == 771:
						print evento.text.text
					pass
					#print evento.type
				elif (evento.type >= 1024)&(evento.type < 1028): #eventos de mouse
					if evento.type == 1025: #SDL_MOUSEBUTTONDOWN
						print evento.button.button
						_, mouse_x, mouse_y = self.ver_mouse()
						selec = self.selec.busca([mouse_x,self.altura-mouse_y])
						if isinstance(selec, dxf.obj_dxf):
							self.lista_selecao.conteudo.append(selec)
							print selec.nome
							#principal = selec
							#while (principal.up.nome != 'SECTION'):
							#	principal = principal.up
							#print principal.nome
					elif evento.type == 1027: #SDL_MOUSEWHEEL
						#print evento.wheel.x, evento.wheel.y
						#print self.ver_mouse()
						_, mouse_x, mouse_y = self.ver_mouse()
						zoom_ant = self.zoom
						self.zoom = self.zoom + evento.wheel.y*0.2*self.zoom
					
						self.offset[0] *= self.zoom/zoom_ant
						self.offset[1] *= self.zoom/zoom_ant
						self.offset[0] += float(mouse_x)*(self.zoom/zoom_ant-1)
						self.offset[1] += (self.altura-float(mouse_y))*(self.zoom/zoom_ant-1)
							
						if self.redesenha != None:
							self.redesenha()
		
		self.sai()
		
	def limpa(self):
		#limpa a imagem de fundo com a cor determinada
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_fundo)
		self.lib.SDL_SetRenderDrawColor(self.renderer, self.fundo[0], self.fundo[1], self.fundo[2], 255)
		self.lib.SDL_RenderClear(self.renderer)
		
		#limpa a imagem de hilite com a cor determinada e totalmente transparente
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_hilite)
		self.lib.SDL_SetRenderDrawColor(self.renderer, self.fundo[0], self.fundo[1], self.fundo[2], 0)
		self.lib.SDL_RenderClear(self.renderer)
		
		#zera a lista de selecao
		self.selec.lista_busca = []
	
	def exibe(self):
		#exibe a imagem de fundo na tela
		self.lib.SDL_SetRenderTarget(self.renderer, None)
		for i in self.camadas:
			self.lib.SDL_RenderCopy(self.renderer, i, None, None)
		self.lib.SDL_RenderPresent(self.renderer)

	def ver_mouse(self):
		x = c_int(0)
		y = c_int(0)
		botoes = self.lib.SDL_GetMouseState(byref(x), byref(y))
		return botoes, x.value, y.value
	
	def sai(self):
		#saida segura do SDL
		for i in self.camadas:
			self.lib.SDL_DestroyTexture(i)
		self.lib.SDL_DestroyRenderer(self.renderer)
		self.lib.SDL_DestroyWindow(self.window)
		self.lib_ttf.TTF_CloseFont(self.fonte)
		self.lib_ttf.TTF_Quit()
		self.lib.SDL_Quit()
	
	def set_camada(self, camada=0, color=(0,0,0)):
		cor = [0,0,0]
		cor[0] = color[0]
		cor[1] = color[1]
		cor[2] = color[2]
		
		#verifica se eh a mesma cor do fundo de tela
		if (cor[0] == self.fundo[0]) and (
		cor[1] == self.fundo[1]) and (
		cor[2] == self.fundo[2]):
			cor[0] ^= 255 #faz um XOR na cor
			cor[1] ^= 255 #faz um XOR na cor
			cor[2] ^= 255 #faz um XOR na cor
		
		#Busca a camada na lista de camadas existentes
		try:
			self.lib.SDL_SetRenderTarget(self.renderer, self.camadas[camada])
		except: #se nao existir a camada, determina a camada de fundo
			self.lib.SDL_SetRenderTarget(self.renderer, self.camadas[0])
		
		#determina a cor de desenho para cada camada
		if camada == 0: #para a camada de fundo, a cor eh a propria do objeto
			self.lib.SDL_SetRenderDrawColor(self.renderer, cor[0], cor[1], cor[2], 0)
		elif camada == 1: #para a camada de hilite, a cor eh a propria do hilite
			self.lib.SDL_SetRenderDrawColor(self.renderer, self.cor_hi[0], self.cor_hi[1], self.cor_hi[2], self.cor_hi[3])
		else: #outros casos, a cor eh a propria do objeto
			self.lib.SDL_SetRenderDrawColor(self.renderer, cor[0], cor[1], cor[2], 0)
		
	def set_pt(self, x, y, esp=4):
		'''desenha um pixel na camada SDL'''
		if (0 < x < self.largura) & (0< y < self.altura):
			if esp<2:
				self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
			else:
				rect = sdl.SDL_Rect()
				rect.h = int(round(esp))
				rect.w = int(round(esp))
				rect.x = int(round(x-esp/2))
				rect.y = int(round(y-esp/2))
				self.lib.SDL_RenderFillRect(self.renderer, byref(rect))
	
	def linha(self, entidade, camada, ponto1, ponto2, color=(0,0,0), esp=1):
		
		#determina a camada e a cor
		self.set_camada(camada, color)
		
		if camada == 1: #para a camada de hilite, aumenta a espessura
			esp += 2
		
		pt1 = [0,0]
		pt2 = [0,0]
		pt1[0] = ponto1[0]
		pt1[1] = ponto1[1]
		pt2[0] = ponto2[0]
		pt2[1] = ponto2[1]
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt2[0] *= self.zoom
		pt2[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		pt2[0] -= self.offset[0]
		pt2[1] -= self.offset[1]
		
		pt1[0] = int(round(pt1[0]))
		pt1[1] = int(round(pt1[1]))
		pt2[0] = int(round(pt2[0]))
		pt2[1] = int(round(pt2[1]))
		
		#desenha somente se a linha esta dentro da area de desenho
		if util.intersect(pt1, pt2, [0,0], [self.largura, self.altura]):
			
			self.selec.add_line(entidade, [[pt1[0], pt1[1]], [pt2[0], pt2[1]]])
			
			#corrige a origem em y para emular um plano cartesiano
			pt1[1] = self.altura - pt1[1]
			pt2[1] = self.altura - pt2[1]
			
			dx = abs(pt2[0] - pt1[0])
			dy = abs(pt2[1] - pt1[1])
			x, y = pt1[0], pt1[1]
			sx = -1 if pt1[0] > pt2[0] else 1
			sy = -1 if pt1[1] > pt2[1] else 1
			
			if abs(dx+dy) > 0:
				'''otimizacao para linhas verticais e horizontais.
				traduzido da linguagem C da pagina
				http://www.willperone.net/Code/codeline.php'''
				if dx == 0:  # verticais
					#print 'linha vertical'
					if 0 <= x < self.largura:
						if (dy*sy) > 0:
							#y = pt1[1]
							while y <= pt2[1]:
								self.pix_count += 1
								if self.pattern[self.patt_a] >=0:
									self.set_pt(x, y, esp)
								if self.pix_count >= abs(self.pattern[self.patt_a]):
									self.pix_count = 0
									self.patt_a += 1
									if self.patt_a >= len(self.pattern):
										self.patt_a = 0
								y += 1
						else:
							y = pt2[1]
							while y <= pt1[1]:
								self.pix_count += 1
								if self.pattern[self.patt_a] >=0:
									self.set_pt(x, y, esp)
								if self.pix_count >= abs(self.pattern[self.patt_a]):
									self.pix_count = 0
									self.patt_a += 1
									if self.patt_a >= len(self.pattern):
										self.patt_a = 0
								y += 1
				elif dy == 0:  # horizontais
					#print 'linha horizontal'
					if 0 <= y < self.altura:
						if (dx*sx) > 0:
							#x = pt1[0]
							while x <= pt2[0]:
								self.pix_count += 1
								if self.pattern[self.patt_a] >=0:
									self.set_pt(x, y, esp)
								if self.pix_count >= abs(self.pattern[self.patt_a]):
									self.pix_count = 0
									self.patt_a += 1
									if self.patt_a >= len(self.pattern):
										self.patt_a = 0
								x += 1
						else:
							x = pt2[0]
							while x <= pt1[0]:
								self.pix_count += 1
								if self.pattern[self.patt_a] >=0:
									self.set_pt(x, y, esp)
								if self.pix_count >= abs(self.pattern[self.patt_a]):
									self.pix_count = 0
									self.patt_a += 1
									if self.patt_a >= len(self.pattern):
										self.patt_a = 0
								x += 1
				else:
					'''Bresenham's line algorithm
					http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#Python'''
					#print 'linha em angulo'
					if dx > dy:
						err = dx / 2.0
						while x != pt2[0]:
							self.pix_count += 1
							if self.pattern[self.patt_a] >=0:
								self.set_pt(x, y, esp)
							if self.pix_count >= abs(self.pattern[self.patt_a]):
								self.pix_count = 0
								self.patt_a += 1
								if self.patt_a >= len(self.pattern):
									self.patt_a = 0
							err -= dy
							if err < 0:
								y += sy
								err += dx
							x += sx
					else:
						err = dy / 2.0
						while y != pt2[1]:
							self.pix_count += 1
							if self.pattern[self.patt_a] >=0:
								self.set_pt(x, y, esp)
							if self.pix_count >= abs(self.pattern[self.patt_a]):
								self.pix_count = 0
								self.patt_a += 1
								if self.patt_a >= len(self.pattern):
									self.patt_a = 0
							err -= dx
							if err < 0:
								x += sx
								err += dy
							y += sy        
					self.pix_count += 1
					if self.pattern[self.patt_a] >=0:
						self.set_pt(x, y, esp)
					if self.pix_count >= abs(self.pattern[self.patt_a]):
						self.pix_count = 0
						self.patt_a += 1
						if self.patt_a >= len(self.pattern):
							self.patt_a = 0
	
	
	def circulo(self, entidade, camada, pt1, raio, cor=(0,0,0), esp=1):
		self.arco(entidade, camada, pt1, raio, 0, 0, cor, esp)
	
	def arco(self, entidade, camada, pt1, raio, ang_ini, ang_fim, cor=(0,0,0), esp=1, sentido=1):
		
		n = 32 #numero de vertices do polígono regular que aproxima o circulo ->bom numero 

		#sentido = 1#o sentido padrão do algoritmo eh contra relógio
		#sentido = -1 # para sentido do relógio sentido eh negativo
		
		centro_x = pt1[0]
		centro_y = pt1[1]
		
		ang_ini *= math.pi/180
		ang_fim *= math.pi/180
		
		angulo = (ang_fim-ang_ini) * sentido #angulo do arco
		if angulo <= 0: angulo = angulo + 2*math.pi
		
		passo = angulo*n/(2*math.pi)	#descobre quantos passos para o laço a seguir

		passos = abs(int(passo)) #numero de vertices do arco
		
		pre_x = centro_x + raio * math.cos(ang_ini)
		pre_y = centro_y + raio * math.sin(ang_ini)
		
		#já começa do segundo vértice
		for i in range(1,passos):
			px = centro_x + raio * math.cos(2 * math.pi * i * sentido/ n + ang_ini)
			py = centro_y + raio * math.sin(2 * math.pi * i * sentido/ n + ang_ini)
			
			#self.lista.append(ponto.l_usin(ponto.ponto(pre_x,pre_y,pre_z),ponto.ponto(px,py,pz), self.velocidade))
			self.linha(entidade, camada, [pre_x,pre_y], [px,py], cor, esp)
			pre_x=px
			pre_y=py
			
		# o ultimo vertice do arco eh o ponto final, nao calculado no laço
		#self.lista.append(ponto.l_usin(ponto.ponto(pre_x,pre_y,pre_z),ponto.ponto(atual_x,atual_y,atual_z), self.velocidade))
		px = centro_x + raio * math.cos(ang_fim)
		py = centro_y + raio * math.sin(ang_fim)
		self.linha(entidade, camada, [pre_x,pre_y], [px,py], cor, esp)
		
	def arco_bulge(self, entidade, camada, pt1, pt2, bulge, cor=(0,0,0), esp=1):
		
		theta = 2 * math.atan(bulge)
		alfa = math.atan2(pt2[1]-pt1[1], pt2[0]-pt1[0])
		d = math.sqrt((pt2[1]-pt1[1])**2 + (pt2[0]-pt1[0])**2) / 2
		raio = d*(bulge**2 + 1)/(2*bulge)
		
		ang_c = math.pi+(alfa - math.pi/2 - theta)
		centro = [raio*math.cos(ang_c) + pt1[0],raio*math.sin(ang_c) + pt1[1]]
		
		#angulo inicial e final obtidos das coordenadas iniciais
		ang_ini = math.atan2(pt1[1]-centro[1],pt1[0]-centro[0])
		ang_final = math.atan2(pt2[1]-centro[1],pt2[0]-centro[0])
		
		sentido = 1
		if bulge < 0:
			ang_ini += math.pi
			ang_final += math.pi
			sentido = -1
		
		#converte para garus
		ang_ini *= 180/math.pi
		ang_final *= 180/math.pi
		
		self.arco(entidade, camada, centro, raio, ang_ini, ang_final, cor, esp, sentido)

				
	def line(self, entidade, camada, x0, y0, x1, y1, color = (0,0,0)):
		cor = [0,0,0]
		cor[0] = color[0]
		cor[1] = color[1]
		cor[2] = color[2]
		
		#verifica se eh a mesma cor do fundo de tela
		if (cor[0] == self.fundo[0]) and (
		cor[1] == self.fundo[1]) and (
		cor[2] == self.fundo[2]):
			cor[0] ^= 255 #faz um XOR na cor
			cor[1] ^= 255 #faz um XOR na cor
			cor[2] ^= 255 #faz um XOR na cor
		
		pt1 = [0,0]
		pt2 = [0,0]
		pt1[0] = x0
		pt1[1] = y0
		pt2[0] = x1
		pt2[1] = y1
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt2[0] *= self.zoom
		pt2[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		pt2[0] -= self.offset[0]
		pt2[1] -= self.offset[1]
		
		pt1[0] = int(round(pt1[0]))
		pt1[1] = int(round(pt1[1]))
		pt2[0] = int(round(pt2[0]))
		pt2[1] = int(round(pt2[1]))
		
		#desenha uma linha SDL na cor determinada
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_fundo)
		self.lib.SDL_SetRenderDrawColor(self.renderer, cor[0], cor[1], cor[2], 0)
		self.lib.SDL_RenderDrawLine(self.renderer, pt1[0], self.altura-pt1[1], pt2[0], self.altura-pt2[1])
		
	def texto(self, entidade, camada, txt, pt1, pt2, tam=1, rot=0, color=(0,0,0), alin=(0,1)):
		cor = [0,0,0]
		cor[0] = color[0]
		cor[1] = color[1]
		cor[2] = color[2]
		
		#verifica se eh a mesma cor do fundo de tela
		if (cor[0] == self.fundo[0]) and (
		cor[1] == self.fundo[1]) and (
		cor[2] == self.fundo[2]):
			cor[0] ^= 255 #faz um XOR na cor
			cor[1] ^= 255 #faz um XOR na cor
			cor[2] ^= 255 #faz um XOR na cor
		
		
		cor_sdl = sdl.SDL_Color()
		#determina a cor de desenho para cada camada
		if camada == 0: #para a camada de fundo, a cor eh a propria do objeto
			cor_sdl.r = cor[0]
			cor_sdl.g = cor[1]
			cor_sdl.b = cor[2]
			cor_sdl.a = 255
		elif camada == 1: #para a camada de hilite, a cor eh a do hilite
			cor_sdl.r = self.cor_hi[0]
			cor_sdl.g = self.cor_hi[1]
			cor_sdl.b = self.cor_hi[2]
			cor_sdl.a = self.cor_hi[3]
		else:
			cor_sdl.r = cor[0]
			cor_sdl.g = cor[1]
			cor_sdl.b = cor[2]
			cor_sdl.a = 255
		
		pos = [0, 0]
		
		#renderiza o texto em uma nova camada (textura)
		s_texto =self.lib_ttf.TTF_RenderUTF8_Solid(self.fonte, str(txt), cor_sdl)
		t_texto = self.lib.SDL_CreateTextureFromSurface(self.renderer, s_texto)
		self.lib.SDL_SetTextureBlendMode(t_texto, 1)
		self.lib.SDL_FreeSurface(s_texto)
		
		#descobre as dimensoes da textura
		largura = c_int(0)
		altura = c_int(0)
		self.lib.SDL_QueryTexture(t_texto, None, None, byref(largura), byref(altura))
		
		#print altura. value, largura.value, txt
		if altura.value<=0: altura.value = 1 #evita divisao por zero
		
		#determina a posicao de insercao do texto em funcao de seu alinhamento
		centro_x = alin[0] * (float(largura.value)/altura.value * tam/2)
		centro_y = (alin[1]-3)* (float(tam)/ 2)
		base_x =  alin[0] * (float(pt2[0]) - pt1[0])/2
		base_y =  alin[0] * (float(pt2[1]) - pt1[1])/2
		pos[0] += float(pt1[0]) + base_x - centro_x
		pos[1] += float(pt1[1]) + base_y - centro_y
		#print pos, [base_x, base_y], [centro_x, centro_y]
		
		#aplica zoom e offset aos pontos
		pos[0] *= self.zoom
		pos[1] *= self.zoom
		pos[0] -= self.offset[0]
		pos[1] -= self.offset[1]
		tam *= self.zoom
		
		#corrige a origem em y para emular um plano cartesiano
		pos[1] = float(self.altura) - pos[1]
		
		#determina o retangulo SDL de destino do texto
		r_destino = sdl.SDL_Rect()
		r_destino.x = int(round(pos[0]))
		r_destino.y = int(round(pos[1]))
		r_destino.h = int(round(tam))
		r_destino.w = int(round(float(largura.value)/altura.value * tam))
		
		#determina o pivo de rotacao (ponto SDL)
		pivo = sdl.SDL_Point()
		pivo.x = int(round(alin[0] * r_destino.w/2))
		pivo.y = int(round((3-alin[1])* r_destino.h/2))
		
		#acrescenta o ponto de insercao do texto na lista de busca para selecao da tela			
		self.selec.add_line(entidade, [[pos[0], float(self.altura) - pos[1]], [pos[0], float(self.altura) - pos[1]]])
		
		#Busca a camada na lista de camadas existentes
		try:
			self.lib.SDL_SetRenderTarget(self.renderer, self.camadas[camada])
		except: #se nao existir a camada, determina a camada de fundo
			self.lib.SDL_SetRenderTarget(self.renderer, self.camadas[0])
		
		#copia o texto na camada com a rotacao
		self.lib.SDL_RenderCopyEx(self.renderer, t_texto, None, byref(r_destino), rot, byref(pivo), 0)
		
		#exclui a camada temporaria
		self.lib.SDL_DestroyTexture(t_texto)
	
	def texto_shx(self, entidade, camada, txt, pt1, pt2, tam=1, rot=0, cor=(0,0,0), alin=(0,1)):
		
		#renderiza o texto
		linhas, largura, altura = self.fonte_shx.interpreta(txt)
		
		if (len(linhas)>0) and (altura!=0):
			pos = [0, 0]
			#tam_fonte = abs(self.fonte_shx.dados_fonte[0]+self.fonte_shx.dados_fonte[1])
			tam_fonte = self.fonte_shx.dados_fonte[0]
			tamanho = float(tam)/tam_fonte
			
			#determina a posicao de insercao do texto em funcao de seu alinhamento
			centro_x = alin[0] * (float(largura)/altura * tam/2)
			centro_y = (alin[1]-1)* (float(tam)/ 2)
			base_x =  alin[0] * (float(pt2[0]) - pt1[0])/2
			base_y =  alin[0] * (float(pt2[1]) - pt1[1])/2
			pos[0] += float(pt1[0]) + base_x - centro_x
			pos[1] += float(pt1[1]) + base_y - centro_y
			if rot != 0:
				cosseno = math.cos(rot*math.pi/180)
				seno = math.sin(rot*math.pi/180)
			
			for lin in linhas:
				p1_x = tamanho * lin[0][0] + pos[0]
				p1_y = tamanho * lin[0][1] + pos[1]
				p2_x = tamanho * lin[1][0] + pos[0]
				p2_y = tamanho * lin[1][1] + pos[1]
				
				#aplica rotacao
				if rot != 0:
					p1r_x = cosseno*(p1_x-pos[0]) - seno*(p1_y-pos[1]) + pos[0]
					p1r_y = seno*(p1_x-pos[0]) + cosseno*(p1_y-pos[1]) + pos[1]
					p2r_x = cosseno*(p2_x-pos[0]) - seno*(p2_y-pos[1]) + pos[0]
					p2r_y = seno*(p2_x-pos[0]) + cosseno*(p2_y-pos[1]) + pos[1]
					p1_x = p1r_x
					p1_y = p1r_y
					p2_x = p2r_x
					p2_y = p2r_y
				self.linha(entidade, camada, [p1_x,p1_y], [p2_x,p2_y], cor)
			#self.linha(None, 0, [pre_x,pre_y], [pre_x+px,pre_y+py], (0,0,0), 3)
		
	
	def des_teste(self):
		texto_teste ='Lorem ipsum dolor sit amet, consectetur adipiscing elit, '+\
		'sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ' +\
		'Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ' +\
		'nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ' +\
		'reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla ' +\
		'pariatur. Excepteur sint occaecat cupidatat non proident, sunt in ' +\
		'culpa qui officia deserunt mollit anim id est laborum.'
		texto_teste2 ='ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðõôóòöúùûü.,;:!@#$%¨&*(){}[]?><|/\\^~-=+°º²³¹ª'
		
		self.limpa()
		self.pattern = [10, -5, 0, -5]
		
		self.line(None, 0, 100,90,100,110) #--------teste
		self.line(None, 0, 90,100,110,100) #--------teste
		self.texto(None, 0, texto_teste[:150],(0,100),(100,100), 30, 0, (0,0,255), (2,2))
		
		self.texto(None, 0, 'teste',(100,100),(100,100), 30, 0, (0,0,0), (1,2))
		self.texto_shx(None, 0, 'teste',(100,100),(100,100), 30, 30, (0,0,100), (1,2))
		self.texto_shx(None, 0, 'teste',(100,100),(100,100), 30, 60, (0,0,255), (1,2))
		self.texto_shx(None, 0,'teste',(100,100),(100,100), 30, 90, (0,0,0), (1,2))
		self.texto(None, 0,'teste',(100,100),(100,100), 30, -120, (0,0,0), (1,2))
		
		self.line(None, 0, 200,180,200,220) #--------teste
		self.line(None, 0, 180,200,220,200) #--------teste
		self.texto(None, 0, 'testezão',(200,200),(200,200), 30, 0, (0,0,0))
		self.texto(None, 0, 'teste',(200,200),(200,200), 30, -30, (0,0,0))
		self.texto(None, 0, 'teste',(200,200),(200,200), 30, -60, (0,0,0))
		self.texto(None, 0, 'teste',(200,200),(200,200), 30, -90, (0,0,0))
		
		self.line(None, 0, 250,80,250,120) #--------teste
		self.line(None, 0, 230,100,270,100) #--------teste
		self.texto(None, 0, 'BL', (250,100), (250,100), 40, 0, (255,0,0), (0,1))
		self.texto(None, 0, 'MC', [250,100], [250,100], 40, 0, (0,255,0), (1,2))
		self.texto(None, 0, 'TR', [250,100], [250,100], 40, 0, (0,0,255), (2,3))
		self.linha(None, 0, [100, 400], [200, 100], (255,0,0),1)
		self.linha(None, 0, [100, 400], [100, 200], (0,0,0),2)
		self.linha(None, 0, [100, 400], [200, 200], (0,0,255),3)
		
		self.arco(None, 0, [200,300], 50, 30, 60, cor=(0,255,0), esp=1)
		self.arco(None, 0, [200,300], 70, 0, 240, cor=(0,0,255), esp=2)
		self.arco(None, 0, [200,300], 70, 0, 240, cor=(255,0,255), esp=1, sentido=-1)
		self.circulo(None, 0, [200,300], 30, cor=(0,0,0), esp=3)
		
		#self.texto_shx(None, 0, 'teste',(200,200),(200,200), 30, -30, (0,0,0))
		self.pix_count = 0
		self.pattern = [1]
		self.patt_a = 0
		
		self.texto_shx(None, 0, texto_teste,(200,200),(200,200), 30, 0, (255,0,0))
		
		self.line(None, 0, 350,80,350,120) #--------teste
		self.line(None, 0, 330,100,370,100) #--------teste
		self.texto_shx(None, 0, 'BL', (350,100), (350,100), 40, 0, (255,0,0), (0,1))
		self.texto_shx(None, 0, 'MC', [350,100], [350,100], 40, 0, (0,255,0), (1,2))
		self.texto_shx(None, 0, 'TR', [350,100], [350,100], 40, 0, (0,0,255), (2,3))
		
		self.texto_shx(None, 0, texto_teste2, [350,300], [350,300], 10, 60, (0,255,0), (0,1))
		
		self.exibe()
		
if __name__ == "__main__":
	print 'Modulo de interface ao SDL2.'
	''' o codigo abaixo eh somente para teste '''
	janela = viewer()
	with janela.pronto:
		janela.pronto.wait(1)
	janela.des_teste()
	janela.redesenha = janela.des_teste
	
	
	'''
	#cria uma camada
	camada1 = janela.lib.SDL_CreateTexture(janela.renderer, 373694468, 2, janela.largura, janela.altura)
	janela.lib.SDL_SetTextureBlendMode(camada1, 1)
	
	#janela.lib.SDL_SetRenderTarget(janela.renderer, camada1)
	#janela.lib.SDL_SetRenderDrawColor(janela.renderer, 255, 0, 0, 255)
	
	
	#exibe a imagem na tela
	janela.lib.SDL_SetRenderTarget(janela.renderer, None)
	janela.lib.SDL_RenderCopy(janela.renderer, janela.t_fundo, None, None)
	janela.lib.SDL_RenderCopy(janela.renderer, camada1, None, None)
	janela.lib.SDL_RenderPresent(janela.renderer)
	'''