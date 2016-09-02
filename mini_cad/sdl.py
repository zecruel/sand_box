from ctypes import *
import os
import threading
import sdl_event

class SDL_Point(Structure):
	_fields_ = [('x', c_int), ('y', c_int)]

class SDL_Rect(Structure):
	_fields_ = [('x', c_int), ('y', c_int),
	('w', c_int), ('h', c_int)]
	
class SDL_Color(Structure):
	_fields_ = [('r', c_uint8), ('g', c_uint8),
	('b', c_uint8), ('a', c_uint8)]

class sdl_dll:
	
	def __init__(self):
		self.dir = os.path.dirname(os.path.abspath(__file__)).replace('\\','/') + '/'
		#------------------------------------------------------------------------------------
		# inicializacao
		
		# Carrega  os dlls
		self.lib = CDLL(self.dir + 'SDL2.dll')
		self.lib_ttf = CDLL(self.dir + 'SDL2_ttf.dll')
		
		#-------------------------------------------------------------------------------------
		# definicao da funcoes a serem chamadas		
		
		#----geral
		'''int SDL_Init(Uint32 flags)'''
		self.lib.SDL_Init.restype = c_int
		self.lib.SDL_Init.argtypes = [c_uint]
		
		'''void SDL_Quit(void)'''
		
		'''int SDL_PollEvent(SDL_Event* event)'''
		self.lib.SDL_PollEvent.restype = c_int
		self.lib.SDL_PollEvent.argtypes = [POINTER(sdl_event.SDL_Event)]
		
		#----window
		'''SDL_Window* SDL_CreateWindow(const char* title,
                             int         x,
                             int         y,
                             int         w,
                             int         h,
                             Uint32      flags)'''
		self.lib.SDL_CreateWindow.restypes = [c_void_p]
		self.lib.SDL_CreateWindow.argtypes = [c_char_p, c_int, c_int, c_int, c_int, c_uint]
		
		'''void SDL_DestroyWindow(SDL_Window* window)'''
		self.lib.SDL_DestroyWindow.argtypes = [c_void_p]
		
		'''void SDL_SetWindowPosition(SDL_Window* window,
                           int         x,
                           int         y)'''
		self.lib.SDL_SetWindowPosition.argtypes = [c_void_p, c_int, c_int]
		
		#----renderer
		'''SDL_Renderer* SDL_CreateRenderer(SDL_Window* window,
                                 int         index,
                                 Uint32      flags)'''
		self.lib.SDL_CreateRenderer.restypes = [c_void_p]
		self.lib.SDL_CreateRenderer.argtypes = [c_void_p, c_int, c_uint]
		
		'''void SDL_DestroyRenderer(SDL_Renderer* renderer)'''
		self.lib.SDL_DestroyRenderer.argtypes = [c_void_p]
		
		'''int SDL_SetRenderDrawColor(SDL_Renderer* renderer,
                           Uint8         r,
                           Uint8         g,
                           Uint8         b,
                           Uint8         a)'''
		self.lib.SDL_SetRenderDrawColor.restype = c_int
		self.lib.SDL_SetRenderDrawColor.argtypes = [c_void_p, c_uint, c_uint, c_uint, c_uint]
		
		'''int SDL_RenderClear(SDL_Renderer* renderer)'''
		self.lib.SDL_RenderClear.restype = c_int
		self.lib.SDL_RenderClear.argtypes = [c_void_p]
		
		'''void SDL_RenderPresent(SDL_Renderer* renderer)'''
		self.lib.SDL_RenderPresent.argtypes = [c_void_p]
		
		'''int SDL_RenderDrawLine(SDL_Renderer* renderer,
                       int           x1,
                       int           y1,
                       int           x2,
                       int           y2)'''
		self.lib.SDL_RenderDrawLine.restype = c_int
		self.lib.SDL_RenderDrawLine.argtypes = [c_void_p, c_int, c_int, c_int, c_int]
		
		'''int SDL_RenderDrawPoint(SDL_Renderer* renderer,
                        int           x, 
                        int           y)'''
		self.lib.SDL_RenderDrawPoint.restypes = [c_int]
		self.lib.SDL_RenderDrawPoint.argtypes = [c_void_p, c_int, c_int]
		
		#---- Misto
		'''int SDL_CreateWindowAndRenderer(int            width,
                                int            height,
                                Uint32         window_flags,
                                SDL_Window**   window,
                                SDL_Renderer** renderer)'''
		self.lib.SDL_CreateWindowAndRenderer.restype = c_int
		self.lib.SDL_CreateWindowAndRenderer.argtypes = [c_int, c_int, c_uint, c_void_p, c_void_p]
		
		'''void SDL_FreeSurface(SDL_Surface* surface)'''
		self.lib.SDL_FreeSurface.argtypes = [c_void_p]
		
		#---- Texture
		
		'''SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer,
                               Uint32        format,
                               int           access,
                               int           w,
                               int           h)'''
		#formato RGBA8888 = 373694468
		self.lib.SDL_CreateTexture.restypes = [c_void_p]
		self.lib.SDL_CreateTexture.argtypes = [c_void_p, c_uint, c_int, c_int, c_int]
		
		'''int SDL_SetRenderTarget(SDL_Renderer* renderer,
                        SDL_Texture*  texture)'''
		# criar a textura com acesso = 2
		self.lib.SDL_SetRenderTarget.restype = c_int
		self.lib.SDL_SetRenderTarget.argtypes = [c_void_p, c_void_p]
		
		'''int SDL_RenderCopy(SDL_Renderer*   renderer,
                   SDL_Texture*    texture,
                   const SDL_Rect* srcrect,
                   const SDL_Rect* dstrect)'''
		self.lib.SDL_RenderCopy.restype = c_int
		self.lib.SDL_RenderCopy.argtypes = [c_void_p, c_void_p,c_void_p, c_void_p]
		
		'''void SDL_DestroyTexture(SDL_Texture* texture)'''
		self.lib.SDL_DestroyTexture.argtypes = [c_void_p]
		
		'''int SDL_SetTextureBlendMode(SDL_Texture*  texture,
                            SDL_BlendMode blendMode)'''
		self.lib.SDL_SetTextureBlendMode.restype = c_int
		self.lib.SDL_SetTextureBlendMode.argtypes = [c_void_p, c_int]
		
		'''SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer,
                                          SDL_Surface*  surface)'''
		self.lib.SDL_CreateTextureFromSurface.restypes = [c_void_p]
		self.lib.SDL_CreateTextureFromSurface.argtypes = [c_void_p, c_void_p]
		
		'''int SDL_RenderCopyEx(SDL_Renderer*          renderer,
                     SDL_Texture*           texture,
                     const SDL_Rect*        srcrect,
                     const SDL_Rect*        dstrect,
                     const double           angle,
                     const SDL_Point*       center,
                     const SDL_RendererFlip flip)'''
		self.lib.SDL_RenderCopyEx.restype = c_int
		self.lib.SDL_RenderCopyEx.argtypes = [c_void_p, c_void_p,c_void_p, c_void_p, c_double, c_void_p, c_int]
		
		'''int SDL_QueryTexture(SDL_Texture* texture,
                     Uint32*      format,
                     int*         access,
                     int*         w,
                     int*         h)'''
		self.lib.SDL_QueryTexture.restypes = [c_void_p]
		self.lib.SDL_QueryTexture.argtypes = [c_void_p, c_void_p, c_void_p, c_void_p, c_void_p]
		
		#-----------Mouse
		'''Uint32 SDL_GetMouseState(int* x,
                         int* y)'''
		self.lib.SDL_GetMouseState.restypes = [c_uint32]
		self.lib.SDL_GetMouseState.argtypes = [c_void_p, c_void_p]
		
		#--------------------------------------
		#SDL ttf - renderizacao de texto
		
		'''int TTF_Init()'''
		self.lib_ttf.TTF_Init.restype = c_int
		
		'''void TTF_Quit()'''
		
		'''TTF_Font *TTF_OpenFont(const char *file, int ptsize)'''
		self.lib_ttf.TTF_OpenFont.restypes = [c_void_p]
		self.lib_ttf.TTF_OpenFont.argtypes = [c_char_p, c_uint]
		
		'''void TTF_CloseFont(TTF_Font *font)'''
		self.lib_ttf.TTF_CloseFont.argtypes = [c_void_p]
		
		'''int TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h)'''
		
		'''SDL_Surface *TTF_RenderText_Solid(TTF_Font *font, const char *text, SDL_Color fg)'''
		self.lib_ttf.TTF_RenderText_Solid.restypes = [c_void_p]
		self.lib_ttf.TTF_RenderText_Solid.argtypes = [c_void_p, c_char_p, SDL_Color]
		
		'''def floodfill(x, y, oldColor, newColor):
		    # assume surface is a 2D image and surface[x][y] is the color at x, y.
		    if surface[x][y] != oldColor: # the base case
			return
		    surface[x][y] = newColor
		    floodfill(x + 1, y, oldColor, newColor) # right
		    floodfill(x - 1, y, oldColor, newColor) # left
		    floodfill(x, y + 1, oldColor, newColor) # down
		    floodfill(x, y - 1, oldColor, newColor) # up
		'''
		
		#--------------------------------------------------------------------------------------
		# definicoes das flags
		
		#define SDL_INIT_TIMER          0x00000001
		#define SDL_INIT_AUDIO          0x00000010
		#define SDL_INIT_VIDEO          0x00000020  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
		#define SDL_INIT_JOYSTICK       0x00000200  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
		#define SDL_INIT_HAPTIC         0x00001000
		#define SDL_INIT_GAMECONTROLLER 0x00002000  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
		#define SDL_INIT_EVENTS         0x00004000
		#define SDL_INIT_NOPARACHUTE    0x00100000  /**< compatibility; this flag is ignored. */
		
		#define SDL_WINDOWPOS_UNDEFINED_MASK    0x1FFF0000
		#define SDL_WINDOWPOS_CENTERED_MASK    0x2FFF0000
		
		'''typedef enum
		{
		    SDL_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
		    SDL_WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
		    SDL_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
		    SDL_WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
		    SDL_WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
		    SDL_WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
		    SDL_WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
		    SDL_WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
		    SDL_WINDOW_INPUT_GRABBED = 0x00000100,      /**< window has grabbed input focus */
		    SDL_WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
		    SDL_WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
		    SDL_WINDOW_FULLSCREEN_DESKTOP = ( SDL_WINDOW_FULLSCREEN | 0x00001000 ),
		    SDL_WINDOW_FOREIGN = 0x00000800,            /**< window not created by SDL */
		    SDL_WINDOW_ALLOW_HIGHDPI = 0x00002000,      /**< window should be created in high-DPI mode if supported */
		    SDL_WINDOW_MOUSE_CAPTURE = 0x00004000       /**< window has mouse captured (unrelated to INPUT_GRABBED) */
		} SDL_WindowFlags;'''

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
		self.fundo = kwargs.pop('fundo', (255,255,255))
		self.titulo = kwargs.pop('titulo', 'SDL 2.0')
	
		
		#------------- variaveis internas ---------------
		self.zoom = 1
		self.offset = [0,0]
		self.redesenha = None #funcao externa para redesenho
		
		self.sdl = sdl_dll() #carrega o dll do SDL 2.0
		self.lib = self.sdl.lib
		self.lib_ttf = self.sdl.lib_ttf
		self.pronto = threading.Condition() #informa que esta pronto
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
		
		self.fonte = self.lib_ttf.TTF_OpenFont(self.sdl.dir + 'SourceCodePro-ExtraLight.ttf', 200)
		
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
		
		while 1:
			if self.lib.SDL_PollEvent(byref(evento)):
				if evento.type == 256: #evento de sair
					break
				elif (evento.type >= 768)&(evento.type < 1024): #eventos de teclado
					pass
					#print evento.type
				elif (evento.type >= 1024)&(evento.type < 1028): #eventos de mouse
					if evento.type == 1025: #SDL_MOUSEBUTTONDOWN
						print evento.button.button
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
	
	def exibe(self):
		#exibe a imagem de fundo na tela
		self.lib.SDL_SetRenderTarget(self.renderer, None)
		self.lib.SDL_RenderCopy(self.renderer, self.t_fundo, None, None)
		self.lib.SDL_RenderPresent(self.renderer)
		
	def line(self, x0, y0, x1, y1, cor = (0,0,0)):
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
		
	def texto(self, txt, pt1, pt2, tam=1, rot=0, cor=(0,0,0), alin=(0,1)):
		cor_sdl = SDL_Color()
		cor_sdl.r = cor[0]
		cor_sdl.g = cor[1]
		cor_sdl.b = cor[2]
		cor_sdl.a = 255
		pos = [0, 0]
		
		#renderiza o texto em uma nova camada (textura)
		s_texto =self.lib_ttf.TTF_RenderText_Solid(self.fonte, str(txt).encode('ascii'), cor_sdl)
		t_texto = self.lib.SDL_CreateTextureFromSurface(self.renderer, s_texto)
		self.lib.SDL_SetTextureBlendMode(t_texto, 1)
		self.lib.SDL_FreeSurface(s_texto)
		
		#descobre as dimensoes da textura
		largura = c_int(0)
		altura = c_int(0)
		self.lib.SDL_QueryTexture(t_texto, None, None, byref(largura), byref(altura))
		
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
		r_destino = SDL_Rect()
		r_destino.x = int(round(pos[0]))
		r_destino.y = int(round(pos[1]))
		r_destino.h = int(round(tam))
		r_destino.w = int(round(float(largura.value)/altura.value * tam))
		
		#determina o pivo de rotacao (ponto SDL)
		pivo = SDL_Point()
		pivo.x = int(round(alin[0] * r_destino.w/2))
		pivo.y = int(round((3-alin[1])* r_destino.h/2))
		
		#copia o texto na camada de fundo na tela com a rotacao
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_fundo)
		#self.lib.SDL_RenderCopy(self.renderer, t_texto, None, byref(r_destino))
		self.lib.SDL_RenderCopyEx(self.renderer, t_texto, None, byref(r_destino), rot, byref(pivo), 0)
		
		#exclui a camada temporaria
		self.lib.SDL_DestroyTexture(t_texto)
	
	def ver_mouse(self):
		x = c_int(0)
		y = c_int(0)
		botoes = self.lib.SDL_GetMouseState(byref(x), byref(y))
		return botoes, x.value, y.value
	
	def sai(self):
		#saida segura do SDL
		self.lib.SDL_DestroyTexture(self.t_fundo)
		self.lib.SDL_DestroyRenderer(self.renderer)
		self.lib.SDL_DestroyWindow(self.window)
		self.lib_ttf.TTF_CloseFont(self.fonte)
		self.lib_ttf.TTF_Quit()
		self.lib.SDL_Quit()
	
	def des_teste(self):
		self.limpa()
		
		self.line(100,90,100,110) #--------teste
		self.line(90,100,110,100) #--------teste
		self.texto('teste',(0,100),(100,100), 30, 0, (0,0,255), (2,2))
		
		self.texto('teste',(100,100),(100,100), 30, 0, (0,0,0), (1,2))
		self.texto('teste',(100,100),(100,100), 30, -30, (0,0,100), (1,2))
		self.texto('teste',(100,100),(100,100), 30, -60, (0,0,255), (1,2))
		self.texto('teste',(100,100),(100,100), 30, -90, (0,0,0), (1,2))
		self.texto('teste',(100,100),(100,100), 30, -120, (0,0,0), (1,2))
		
		self.line(200,180,200,220) #--------teste
		self.line(180,200,220,200) #--------teste
		self.texto('teste',(200,200),(200,200), 30, 0, (0,0,0))
		self.texto('teste',(200,200),(200,200), 30, -30, (0,0,0))
		self.texto('teste',(200,200),(200,200), 30, -60, (0,0,0))
		self.texto('teste',(200,200),(200,200), 30, -90, (0,0,0))
		
		self.line(250,80,250,120) #--------teste
		self.line(230,100,270,100) #--------teste
		self.texto('BL', (250,100), (250,100), 40, 0, (255,0,0), (0,1))
		self.texto('MC', [250,100], [250,100], 40, 0, (0,255,0), (1,2))
		self.texto('TR', [250,100], [250,100], 40, 0, (0,0,255), (2,3))
		self.linha([100, 400], [200, 100], (255,0,0))
		self.linha([100, 400], [100, 200], (0,0,0))
		self.linha([100, 400], [200, 200], (0,0,255))
		
		self.exibe()
		
	def set_pt(self, x, y):
		'''desenha um pixel na camada SDL'''
		if (0 < x < self.largura) & (0< y < self.altura):
			self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
		
	def linha(self, ponto1, ponto2, cor=(0,0,0), esp=1):
		#determina a camada e a cor
		self.lib.SDL_SetRenderTarget(self.renderer, self.t_fundo)
		self.lib.SDL_SetRenderDrawColor(self.renderer, cor[0], cor[1], cor[2], 0)
		
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
		
		#corrige a origem em y para emular um plano cartesiano
		pt1[1] = self.altura - pt1[1]
		pt2[1] = self.altura - pt2[1]
		
		dx = abs(pt2[0] - pt1[0])
		dy = abs(pt2[1] - pt1[1])
		x, y = pt1[0], pt1[1]
		sx = -1 if pt1[0] > pt2[0] else 1
		sy = -1 if pt1[1] > pt2[1] else 1
		
		pix_count = 0
		pattern = [10, -5, 0, -5]
		atual = 0
		
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
							pix_count += 1
							if pattern[atual] >=0:
								self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
							if pix_count >= abs(pattern[atual]):
								pix_count = 0
								atual += 1
								if atual >= len(pattern):
									atual = 0
							y += 1
					else:
						y = pt2[1]
						while y <= pt1[1]:
							pix_count += 1
							if pattern[atual] >=0:
								self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
							if pix_count >= abs(pattern[atual]):
								pix_count = 0
								atual += 1
								if atual >= len(pattern):
									atual = 0
							y += 1
			elif dy == 0:  # horizontais
				#print 'linha horizontal'
				if 0 <= y < self.altura:
					if (dx*sx) > 0:
						#x = pt1[0]
						while x <= pt2[0]:
							pix_count += 1
							if pattern[atual] >=0:
								self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
							if pix_count >= abs(pattern[atual]):
								pix_count = 0
								atual += 1
								if atual >= len(pattern):
									atual = 0
							x += 1
					else:
						x = pt2[0]
						while x <= pt1[0]:
							pix_count += 1
							if pattern[atual] >=0:
								self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
							if pix_count >= abs(pattern[atual]):
								pix_count = 0
								atual += 1
								if atual >= len(pattern):
									atual = 0
							x += 1
			else:
				'''Bresenham's line algorithm
				http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#Python'''
				#print 'linha em angulo'
				if dx > dy:
					err = dx / 2.0
					while x != pt2[0]:
						pix_count += 1
						if pattern[atual] >=0:
							self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
						if pix_count >= abs(pattern[atual]):
							pix_count = 0
							atual += 1
							if atual >= len(pattern):
								atual = 0
						err -= dy
						if err < 0:
							y += sy
							err += dx
						x += sx
				else:
					err = dy / 2.0
					while y != pt2[1]:
						pix_count += 1
						if pattern[atual] >=0:
							self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
						if pix_count >= abs(pattern[atual]):
							pix_count = 0
							atual += 1
							if atual >= len(pattern):
								atual = 0
						err -= dx
						if err < 0:
							x += sx
							err += dy
						y += sy        
				pix_count += 1
				if pattern[atual] >=0:
					self.lib.SDL_RenderDrawPoint(self.renderer, x, y)
				if pix_count >= abs(pattern[atual]):
					pix_count = 0
					atual += 1
					if atual >= len(pattern):
						atual = 0
	
	
	def circulo(self, pt1, raio, cor=(0,0,0), esp=1):
		pass
	
	def arco(self, pt1, raio, a_ini, a_fim, cor=(0,0,0), esp=1):
		pass
		
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