
from ctypes import CDLL, CFUNCTYPE, Structure, c_int, c_char_p, c_void_p, \
        c_bool, c_double, POINTER, c_short, cast, c_uint, byref, c_byte
import os
import time
import sdl_event

class app:
	
	def __init__(self):
		self.dir = os.path.dirname(os.path.abspath(__file__)).replace('\\','/') + '/'
		#------------------------------------------------------------------------------------
		# inicializacao
		
		# Carrega  o dll
		self.lib = CDLL(self.dir + 'SDL2.dll')
		
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
		
		#---- Misto
		'''int SDL_CreateWindowAndRenderer(int            width,
                                int            height,
                                Uint32         window_flags,
                                SDL_Window**   window,
                                SDL_Renderer** renderer)'''
		self.lib.SDL_CreateWindowAndRenderer.restype = c_int
		self.lib.SDL_CreateWindowAndRenderer.argtypes = [c_int, c_int, c_uint, c_void_p, c_void_p]
		
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
		
		#------------------------------------
		#inicializacao
		
		#window = c_void_p()
		#renderer = c_void_p()
		evento =sdl_event.SDL_Event()
		
		self.lib.SDL_Init(32)
		#self.lib.SDL_CreateWindowAndRenderer(320, 240, 32, byref(window), byref(renderer))
		
		window = self.lib.SDL_CreateWindow('Teste SDL', 536805376, 536805376, 320, 240, 32)
		renderer = self.lib.SDL_CreateRenderer(window, -1, 0)
		
		self.lib.SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)
		self.lib.SDL_RenderClear(renderer)
		
		self.lib.SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0)
		self.lib.SDL_RenderDrawLine(renderer, 0, 0, 100, 200)
		
		self.lib.SDL_RenderPresent(renderer)
		
		while 1:
			self.lib.SDL_PollEvent(byref(evento))
			if evento.type == 256: #evento de sair
				break
			elif (evento.type >= 768)&(evento.type < 1024): #eventos de teclado
				pass
				#print evento.type
			elif (evento.type >= 1024)&(evento.type < 1028): #eventos de mouse
				if evento.type == 1025: #SDL_MOUSEBUTTONDOWN
					print evento.button.button
				elif evento.type == 1027: #SDL_MOUSEWHEEL
					print evento.wheel.x, evento.wheel.y
				
			evento.type = 0
		
		self.lib.SDL_DestroyWindow(window)
		self.lib.SDL_DestroyRenderer(renderer)
		self.lib.SDL_Quit()
		
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

	# ------------------------------------------------------------------------------------
	# Funcoes de retorno (callbaks)

	'''    
	@staticmethod
	@CFUNCTYPE(c_int, c_int, c_bool, c_bool, c_int, c_void_p)
	def spice_exit(status, immediate, quit, id, ret): 
	    """Callback for libngspice to report fatal error in execution"""
	    app.log.put('ERRO: NGspice saiu inesperadamente. Cod erro ' + str(status))
	    return 0 '''
	    
    
if __name__ == "__main__":
    print 'Modulo de interface ao SDL2.'
    ''' o codigo abaixo eh somente para teste '''
    sdl = app()
   