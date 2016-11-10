# -*- coding: cp1252 -*-
import threading
import math
import sdl_event
#import sdl
from ctypes import *
import util
import dxf
import shape
import time

class viewer(threading.Thread):
	lib = CDLL('render.dll') #carrega o dll do SDL 2.0
	
	#int init(int x, int y, int larg, int alt, char tit[255], int fundo[4])
	lib.init.argtypes =[c_int, c_int, c_int, c_int, c_char_p, c_int*4]
	#void limpa_camada(int camada, int cor[4])
	lib.limpa_camada.argtypes =[c_int, c_int*4]
	lib.set_camada.argtypes =[c_int, c_int*4]
	#void muda_pattern(int pat[], int tam)
	lib.muda_pattern.argtypes =[POINTER(c_int), c_int]
	#lib.eventos.restype = sdl_event.SDL_Event
	lib.eventos.restype = c_int
	lib.eventos.argtypes = [POINTER(sdl_event.SDL_Event), POINTER(c_long), POINTER(c_int), POINTER(c_int), POINTER(c_int)]
	lib.muda_zoom.argtypes = [c_double, c_double*2]
	lib.linha.argtypes = [c_long, c_int, c_double*2, c_double*2, c_int*4, c_int]
	lib.arco.argtypes = [c_long, c_int, c_double*2, c_double, c_double, c_double, c_int*4, c_int, c_int]
	lib.arco_bulge.argtypes = [c_long, c_int, c_double*2, c_double*2, c_double, c_int*4, c_int]
	lib.interpreta.restype = c_void_p
	#void texto_shx(long entidade, int camada, int i_fonte, char *txt, double pt1[2], double pt2[2], double tam, double rot, int cor[4], int alin_h, int alin_v, int esp){
	lib.texto_shx.argtypes = [c_long, c_int, c_int, c_char_p, c_double*2, c_double*2, c_double, c_double,  c_int*4, c_int, c_int, c_int]
	#int nova_fonte(char *arquivo)
	lib.nova_fonte.restype = c_int
	lib.nova_fonte.argtypes = [c_char_p]
	
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
		self.fundo = kwargs.pop('fundo', (0,0,0,255))
		self.titulo = kwargs.pop('titulo', 'SDL 2.0')
		self.selec = kwargs.pop('selecao', util.selecao()) #objeto auxiliar na selecao de elementos na tela
	
		
		#------------- variaveis internas ---------------
		self.fundo = (c_int * 4)(*self.fundo)
		self.hilite = (0,0,0,0)
		self.hilite = (c_int * 4)(*self.hilite)
		self.zoom = 1
		self.offset = [0,0]
		self.redesenha = None #funcao externa para redesenho
		self.des_selec = None #funcao externa para redesenho
		
		self.plano = 'xy'
		
		self.pronto = threading.Condition() #informa que esta pronto
		
		self.fonte_shx = shape.fonte_shx()
		
		
		
		
		self.cor_hi = (255,0,255,120)
		
		#------------------------------------------------------------
		
		self.start()

	def run(self):
		''' Modifica o metodo run da classe threading'''
		#------------------------------------
		#inicializacao e criacao da janela SDL
		#janela com posicao indeterminada
		self.lib.init(536805376, 536805376, self.largura, self.altura, self.titulo, self.fundo)
		
		#depois de tudo pronto, notifica os processos em espera
		with self.pronto:
			self.pronto.notify()
			
		self.fonte = self.lib.nova_fonte('simplex.shx')
		print self.fonte
		
		
		mouse_x = c_int(0)
		mouse_y = c_int(0)
		bot = c_int(0)
		ent = c_long(0)
		ev = sdl_event.SDL_Event()
		
		ret_ini = 0
		
		while 1:
			if self.lib.eventos(byref(ev), byref(ent), byref(bot), byref(mouse_x), byref(mouse_y)):
				if  ev.type == 256: #evento de sair
					break
				elif ev.type == 1027: #SDL_MOUSEWHEEL
					#print evento.wheel.x, evento.wheel.y
					#print self.ver_mouse()
					#_, mouse_x, mouse_y = self.ver_mouse()
					zoom_ant = self.zoom
					self.zoom = self.zoom + ev.wheel.y*0.2*self.zoom
					
					self.offset[0] -= float(mouse_x.value)*(1/self.zoom-1/zoom_ant)
					self.offset[1] -= float(mouse_y.value)*(1/self.zoom-1/zoom_ant)
					'''
					self.offset[0] *= self.zoom/zoom_ant
					self.offset[1] *= self.zoom/zoom_ant
					self.offset[0] += float(mouse_x.value)*(self.zoom/zoom_ant-1)
					self.offset[1] += float(mouse_y.value)*(self.zoom/zoom_ant-1)
					'''
					self.zoom_off(self.zoom, self.offset)
					if self.redesenha != None:
						self.redesenha()
				elif ev.type == 1025: #SDL_MOUSEBUTTONDOWN
					if ret_ini == 1: ret_ini = 0
					else:
						ret_ini = 1
						x1 = mouse_x.value
						y1 = mouse_y.value
						
						#imprime as coordenadas reais referidas aos objetos
						#print 'x', x1/self.zoom+self.offset[0], 'y', y1/self.zoom+self.offset[1]
					
					#if ent.value:
					#	print cast(ent.value, py_object).value
						
				elif ev.type == 1026: #SDL_MOUSEBUTTONUP
					if ret_ini == 1:
						ret_ini = 0
						x2 = mouse_x.value
						y2 = mouse_y.value
						#ordena os cantos do retangulo
						ret_bl_x = min(x1, x2)
						ret_bl_y = min(y1, y2)
						ret_tr_x = max(x1, x2)
						ret_tr_y = max(y1, y2)
						
						ret_larg = ret_tr_x - ret_bl_x
						ret_alt = ret_tr_x - ret_bl_y
						if (abs(ret_larg) > 20) and (abs(ret_alt)>20):
							zoom_ant = self.zoom
							self.zoom = max(zoom_ant*self.largura/float(ret_larg), zoom_ant*self.altura/float(ret_alt))
								
							self.offset[0] = ret_bl_x/zoom_ant+self.offset[0]
							self.offset[1] = ret_bl_y/zoom_ant+self.offset[1]
							
							
							self.zoom_off(self.zoom, self.offset)
							if self.redesenha != None:
								self.redesenha()
							#print "retangulo", ret_larg, ret_alt, self.zoom
				
				elif ev.type == 1024:
					if ent.value:
						hi = dxf.obj_dxf()
						hi.conteudo.append(cast(ent.value, py_object).value)
						if self.des_selec != None:
							self.des_selec(hi)
							#print hi
					else:
						self.limpa_selec()
						self.exibe()
			
		'''
			if self.lib.SDL_PollEvent(byref(evento)):
				teste_edit.evento(evento)
				if evento.type == 256: #evento de sair
					break
				elif (evento.type >= 768)&(evento.type < 1024): #eventos de teclado
					if evento.type == 771:
						pass
						#print evento.text.text
					if evento.type == 768:
						pass
						if (evento.key.keysym.scancode == 25) and ((evento.key.keysym.mod & 192) != 0):
							print self.lib.SDL_GetClipboardText()
						#print evento.key.keysym.scancode
					#print evento.type
				elif (evento.type >= 1024)&(evento.type < 1028): #eventos de mouse
					if evento.type == 1025: #SDL_MOUSEBUTTONDOWN
						#print evento.button.button
						_, mouse_x, mouse_y = self.ver_mouse()
						self.selec.graf_add_pt([mouse_x,self.altura-mouse_y])
						if self.des_selec != None:
							self.des_selec()
						
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
				elif evento.type == 32768:
					pass
					#print 'tempo'
		'''
		#self.sai()
		
		self.lib.sai()
		
	def limpa(self):
		#limpa a imagem de fundo com a cor determinada
		self.lib.limpa_camada(0, self.fundo)
		
		#limpa a imagem de hilite com a cor determinada e totalmente transparente
		self.lib.limpa_camada(1, self.hilite)
		
		#zera a lista de selecao
		self.lib.lista_limpa()
	
	def limpa_selec(self):
		#limpa a imagem de hilite com a cor determinada e totalmente transparente
		self.lib.limpa_camada(1, self.hilite)
	
	def limpa_camada(self, camada=0):
		#limpa a imagem com a cor determinada e totalmente transparente
		self.lib.limpa_camada(camada, self.hilite)
	
	def exibe(self):
		#exibe a imagem de fundo na tela
		self.lib.exibe()
		
	def pattern(self, pat):
		pat =(c_int* len(pat))(*pat)
		#void muda_pattern(int pat[], int tam)
		self.lib.muda_pattern(pat, len(pat))
		
	def zoom_off(self, zoom, off):
		self.zoom = zoom
		self.offset = off
		offset = (c_double * 2)(*off[:2])
		self.lib.muda_zoom(zoom, offset)
		

	def linha(self, entidade, camada, ponto1, ponto2, color=(0,0,0), esp=1):
		#void linha(void *entidade, int camada, double ponto1[2], double ponto2[2], int color[4], int esp)
		cor = list(color)
		if len(cor) < 4: cor.append(255)
		cor = (c_int * 4)(*cor)
		
		pt1 = (c_double * 2)(*ponto1[:2])
		pt2 = (c_double * 2)(*ponto2[:2])
		
		if camada == 1:
			cor[0] = 255
			cor[1] = 0
			cor[2] = 255
			cor[3] = 120
			esp += 2
		
		self.lib.linha(id(entidade), camada, pt1, pt2, cor, esp)
	
	def circulo(self, entidade, camada, pt1, raio, cor=(0,0,0), esp=1):
		self.arco(entidade, camada, pt1, raio, 0, 0, cor, esp)
	
	def arco(self, entidade, camada, ponto1, raio, ang_ini, ang_fim, cor=(0,0,0), esp=1, sentido=1):
		
		#void arco(void *entidade, int camada, double centro[2], double raio, double ang_ini, double ang_fim, int cor[4], int esp, int sentido)
		cor = list(cor)
		if len(cor) < 4: cor.append(255)
		cor = (c_int * 4)(*cor)
		
		pt1 = (c_double * 2)(*ponto1[:2])
		
		if camada == 1:
			cor[0] = 255
			cor[1] = 0
			cor[2] = 255
			cor[3] = 120
			esp += 2
		
		self.lib.arco(id(entidade), camada, pt1, raio, ang_ini, ang_fim, cor, esp, sentido)
		
	def arco_bulge(self, entidade, camada, ponto1, ponto2, bulge, cor=(0,0,0), esp=1):
		
		#void arco_bulge(void *entidade, int camada, double pt1[2], double pt2[2], double bulge, int cor[4], int esp)
		cor = list(cor)
		if len(cor) < 4: cor.append(255)
		cor = (c_int * 4)(*cor)
		
		pt1 = (c_double * 2)(*ponto1[:2])
		pt2 = (c_double * 2)(*ponto2[:2])
		
		if camada == 1:
			cor[0] = 255
			cor[1] = 0
			cor[2] = 255
			cor[3] = 120
			esp += 2
		
		self.lib.arco_bulge(id(entidade), camada, pt1, pt2, bulge, cor, esp)
	
	
	def texto_shx(self, entidade, camada, txt, ponto1, ponto2, tam=1, rot=0, cor=(0,0,0), alin=(0,1), esp=1):
		cor = list(cor)
		if len(cor) < 4: cor.append(255)
		cor = (c_int * 4)(*cor)
		
		pt1 = (c_double * 2)(*list(ponto1)[:2])
		pt2 = (c_double * 2)(*list(ponto2)[:2])
		
		if camada == 1:
			cor[0] = 255
			cor[1] = 0
			cor[2] = 255
			cor[3] = 120
			esp += 2
			
			
		fonte = 1
		fonte = self.fonte
		#void texto_shx(long entidade, int camada, char *txt, double pt1[2], double pt2[2], double tam, double rot, int cor[4], int alin_h, int alin_v, int esp)
		self.lib.texto_shx(id(entidade), camada, fonte, txt, pt1, pt2, tam, rot, cor, alin[0], alin[1], esp)
		'''
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
				pivo_x = float(pt1[0]) + base_x
				pivo_y = float(pt1[1]) + base_y
			
			for lin in linhas:
				p1_x = tamanho * lin[0][0] + pos[0]
				p1_y = tamanho * lin[0][1] + pos[1]
				p2_x = tamanho * lin[1][0] + pos[0]
				p2_y = tamanho * lin[1][1] + pos[1]
				
				#aplica rotacao
				if rot != 0:
					p1r_x = cosseno*(p1_x-pivo_x) - seno*(p1_y-pivo_y) + pivo_x
					p1r_y = seno*(p1_x-pivo_x) + cosseno*(p1_y-pivo_y) + pivo_y
					p2r_x = cosseno*(p2_x-pivo_x) - seno*(p2_y-pivo_y) + pivo_x
					p2r_y = seno*(p2_x-pivo_x) + cosseno*(p2_y-pivo_y) + pivo_y
					p1_x = p1r_x
					p1_y = p1r_y
					p2_x = p2r_x
					p2_y = p2r_y
				#print entidade
				self.linha(entidade, camada, [p1_x,p1_y], [p2_x,p2_y], cor)
			#self.linha(None, 0, [pre_x,pre_y], [pre_x+px,pre_y+py], (0,0,0), 3)
		'''
	
	def des_teste(self):
		texto_teste ='Lorem ipsum dolor sit amet, consectetur adipiscing elit, '+\
		'sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ' +\
		'Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris ' +\
		'nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in ' +\
		'reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla ' +\
		'pariatur. Excepteur sint occaecat cupidatat non proident, sunt in ' +\
		'culpa qui officia deserunt mollit anim id est laborum.'
		texto_teste2 = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'+\
		'ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðõôóòöúùûü.,;:!@#$%¨&*'+\
		'(){}[]?><|/\\^~-=+°º²³¹ª1234567890'
		
		texto_teste3 = ''
		for x in self.fonte_shx.lista_princ:
			if len(x) > 0: texto_teste3 += unichr(x[0])
		
		self.limpa()
		self.pattern ([10, -5, 0, -5])
		
		self.texto_shx(None, 0, 'teste',(100,100),(100,100), 30, 30, (0,0,100), (1,2))
		self.texto_shx(None, 0, 'teste',(100,100),(100,100), 30, 60, (0,0,255), (1,2))
		self.texto_shx(None, 0,'teste',(100,100),(100,100), 30, 90, (0,0,0), (1,2))
		
		#self.texto_shx(None, 0,texto_teste2,(200,200),(100,100), 30, 0, (0,0,0), (1,2))
		#self.texto_shx(None, 0,'N',(200,200),(100,100), 30, 0, (0,0,0), (1,2))
		
		self.linha(None, 0, [100, 400], [200, 100], (255,0,0),1)
		self.linha(None, 0, [100, 400], [100, 200], (0,0,0),2)
		self.linha(None, 0, [100, 400], [200, 200], (0,0,255),3)
		
		self.linha(None, 1, [100, 400], [200, 100], (255,0,0),1)
		
		self.pattern ([1])
		self.arco(None, 0, [200,300], 50, 30, 60, cor=(0,255,0), esp=1)
		self.arco(None, 0, [200,300], 70, 0, 240, cor=(0,0,255), esp=2)
		self.arco(None, 0, [200,300], 70, 0, 240, cor=(255,0,255), esp=1, sentido=-1)
		self.circulo(None, 0, [200,300], 30, cor=(0,0,0), esp=3)
		self.arco_bulge(None, 0, [100, 400], [200, 100], 0.5, (255,0,0),1)
		
		self.exibe()
	
	# ------------------------------------------------------------------------------------
	# Funcoes de retorno (callbaks)

	#callback do timer
	@staticmethod
	@CFUNCTYPE(c_int, c_uint32, c_void_p)
	def sdl_timer(intervalo, param):
		#print intervalo
		#user event = 32768
		timer_event = sdl_event.SDL_Event()
		timer_event.type = 32768
		timer_event.user.code = 2
		timer_event.user.data1 = None
		timer_event.user.data2 = None
		viewer.lib.SDL_PushEvent(byref(timer_event))
		return intervalo
		
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