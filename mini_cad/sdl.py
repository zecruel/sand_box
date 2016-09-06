# -*- coding: cp1252 -*-
from ctypes import *
import os
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
		
		'''int SDL_RenderFillRect(SDL_Renderer*   renderer,
                       const SDL_Rect* rect)'''
		self.lib.SDL_RenderFillRect.restypes = [c_int]
		self.lib.SDL_RenderFillRect.argtypes = [c_void_p, POINTER(SDL_Rect)]
		
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