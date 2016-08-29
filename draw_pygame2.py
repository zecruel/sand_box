# -*- coding: cp1252 -*-
# Import a library of functions called 'pygame'
import pygame
import pygame.freetype
import threading
from math import pi

class janela(threading.Thread):
	def __init__(self, group=None, target=None, name=None,
				args=(), kwargs=None, verbose=None):
		''' Deriva a classe threading, permitindo a passagem
		de argumentos na inicializacao da classe '''
		threading.Thread.__init__(self, group=group, target=target,
							name=name, verbose=verbose)
		
		#os argumentos passados sao armaz. em variaveis internas
		#args eh uma tupla e kwargs eh um dicionario
		self.args = args
		self.kwargs = kwargs
		#------------- variaveis internas ---------------
		self.zoom = 1
		self.offset = [0,0]
		self.redesenha = None #funcao externa para redesenho
		#------------------------------------------------------------
		self.start()

	def run(self):
		''' Modifica o metodo run da classe threading'''
		
		# Initialize the game engine
		pygame.init()
		pygame.freetype.init() #inicializa o modulo de renderização de texto
		
		#fonte =  'E:/documentos/prog/python/Lib/pygame/freesansbold.ttf'
		#fonte = None
		fonte = 'E:/documentos/prog/python/fontes/Source_Code_Pro/SourceCodePro-ExtraLight.ttf'
		self.fonte = pygame.freetype.Font(fonte)
		 
		# Define the colors we will use in RGB format
		BLACK = (  0,   0,   0)
		WHITE = (255, 255, 255)
		BLUE =  (  0,   0, 255)
		GREEN = (  0, 255,   0)
		RED =   (255,   0,   0)
		 
		# Set the height and width of the screen
		self.screen = pygame.display.set_mode(self.kwargs['size'])
		 
		pygame.display.set_caption(self.kwargs['caption'])
		 
		#Loop until the user clicks the close button.
		done = False
		clock = pygame.time.Clock()
		
		# Clear the screen and set the screen background
		#self.screen.fill(BLACK)
		self.screen.fill(WHITE)
		 
		while not done:
			
			# This limits the while loop to a max of 10 times per second.
			# Leave this out and we will use all CPU we can.
			clock.tick(10)

			for event in pygame.event.get(): # User did something
				if event.type == pygame.QUIT: # If user clicked close
					done=True # Flag that we are done so we exit this loop
				# handle MOUSEBUTTONUP
				
				elif event.type == pygame.MOUSEBUTTONUP:
					pos = pygame.mouse.get_pos()
					zoom_ant = self.zoom
					if event.button == 4:
						self.zoom += 0.2*self.zoom
						
					elif event.button == 5:
						self.zoom -= 0.2*self.zoom
					
					self.offset[0] *= self.zoom/zoom_ant
					self.offset[1] *= self.zoom/zoom_ant
					self.offset[0] += float(pos[0])*(self.zoom/zoom_ant-1)
					self.offset[1] += (self.kwargs['size'][1]-float(pos[1]))*(self.zoom/zoom_ant-1)
						
					if self.redesenha != None:
						self.redesenha()
					
					#print pos, event.button
				

			# All drawing code happens after the for loop and but
			# inside the main while done==False loop.

			# Go ahead and update the screen with what we've drawn.
			# This MUST happen after all the other drawing commands.
			pygame.display.flip()
		# Be IDLE friendly
		pygame.quit()
		pygame.freetype.quit()
	
	def desenha(self):
		
		# Define the colors we will use in RGB format
		BLACK = (  0,   0,   0)
		WHITE = (255, 255, 255)
		BLUE =  (  0,   0, 255)
		GREEN = (  0, 255,   0)
		RED =   (255,   0,   0)
				
		# Clear the screen and set the screen background
		self.screen.fill(WHITE)

		# Draw on the screen a GREEN line from (0,0) to (50.75) 
		# 5 pixels wide.
		pygame.draw.line(self.screen, GREEN, [0, 0], [50,30], 5)

		# Draw on the screen a GREEN line from (0,0) to (50.75) 
		# 5 pixels wide.
		pygame.draw.lines(self.screen, BLACK, False, [[0, 80], [50, 90], [200, 80], [220, 30]], 5)

		# Draw on the screen a GREEN line from (0,0) to (50.75) 
		# 5 pixels wide.
		pygame.draw.aaline(self.screen, GREEN, [0, 50],[50, 80], True)

		# Draw a rectangle outline
		pygame.draw.rect(self.screen, BLACK, [75, 10, 50, 20], 2)

		# Draw a solid rectangle
		pygame.draw.rect(self.screen, BLACK, [150, 10, 50, 20])

		# Draw an ellipse outline, using a rectangle as the outside boundaries
		pygame.draw.ellipse(self.screen, RED, [225, 10, 50, 20], 2) 

		# Draw an solid ellipse, using a rectangle as the outside boundaries
		pygame.draw.ellipse(self.screen, RED, [300, 10, 50, 20]) 

		# This draws a triangle using the polygon command
		pygame.draw.polygon(self.screen, BLACK, [[100, 100], [0, 200], [200, 200]], 5)

		# Draw an arc as part of an ellipse. 
		# Use radians to determine what angle to draw.
		pygame.draw.arc(self.screen, BLACK,[210, 75, 150, 125], 0, pi/2, 2)
		pygame.draw.arc(self.screen, GREEN,[210, 75, 150, 125], pi/2, pi, 2)
		pygame.draw.arc(self.screen, BLUE, [210, 75, 150, 125], pi,3*pi/2, 2)
		pygame.draw.arc(self.screen, RED,  [210, 75, 150, 125], 3*pi/2, 2*pi, 2)

		# Draw a circle
		pygame.draw.circle(self.screen, BLUE, [60, 250], 40)
	
	def desenha2(self):
		self.limpa_tela()
		self.linha([0,0], [100,100])
		self.texto('Textozão', [100,100], 20, 0, (255,0,0))
		self.texto('Textozão', [100,100], 20, 30, (255,0,0))
		self.texto('Textozão', [100,100], 20, 60, (255,0,0))
		self.texto('Textozão', [100,100], 20, 90, (255,0,0))
		
		self.texto('BL', [250,100], 20, 0, (255,0,0), (0,1))
		self.texto('MC', [250,100], 20, 0, (0,255,0), (1,2))
		self.texto('TR', [250,100], 20, 0, (0,0,255), (2,3))
	
	def limpa_tela(self):
		# Clear the screen and set the screen background
		self.screen.fill((255, 255, 255))
		
	def linha(self, pt1, pt2, cor=(0,0,0), esp=1):
		'''Desenha uma linha na tela. Os pontos sao listas com dois valores, x e y'''
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt2[0] *= self.zoom
		pt2[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		pt2[0] -= self.offset[0]
		pt2[1] -= self.offset[1]
		
		
		#corrige a origem em y para emular um plano cartesiano
		y_orig = self.kwargs['size'][1]
		pt1[1] = y_orig - pt1[1]
		pt2[1] = y_orig - pt2[1]
		
		pygame.draw.line(self.screen, cor, pt1, pt2, esp)
		
	def circulo(self, pt1, raio, cor=(0,0,0), esp=1):
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		
		raio *= self.zoom
		
		#transforma os dados do retangulo em inteiros devido a compatibilidade com o Pygame
		pt1[0] = int(round(pt1[0]))
		pt1[1] = int(round(pt1[1]))
		raio = int(round(raio))
		
		if raio >= 1:
			#corrige a origem em y para emular um plano cartesiano
			y_orig = self.kwargs['size'][1]
			pt1[1] = y_orig - pt1[1]
		
			pygame.draw.circle(self.screen, cor, pt1, raio, esp)
			
	def arco(self, pt1, raio, a_ini, a_fim, cor=(0,0,0), esp=1):
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		
		raio *= self.zoom
		
		#descobre o retangulo onde dera desenhado o arco
		#neste caso considera-se o arco de um circulo perfeito
		#quando largura e altura sao diferentes, eh um arco de uma elipse
		retang = [pt1[0]-raio, pt1[1]+raio, 2*raio, 2*raio]
		
		#transforma os dados do retangulo em inteiros devido a compatibilidade com o Pygame
		retang = [int(round(i)) for i in retang]
		
		a_ini *= pi/180
		a_fim *= pi/180
		
		if raio >= 1:
			#corrige a origem em y para emular um plano cartesiano
			y_orig = self.kwargs['size'][1]
			pt1[1] = y_orig - pt1[1]
			retang[1] = y_orig - retang[1]
			
			pygame.draw.arc(self.screen,cor,retang, a_ini, a_fim, esp)
	
	def texto(self, texto, pt1, tam=1, rot=0, cor=(0,0,0), alin=(0,1)):
		'''Desenha um texto na tela.'''
		
		#aplica zoom e offset aos pontos
		pt1[0] *= self.zoom
		pt1[1] *= self.zoom
		pt1[0] -= self.offset[0]
		pt1[1] -= self.offset[1]
		
		tam *= self.zoom
		
		rot = int(round(rot))
		
		if tam >= 1:
		
			texto = texto.decode('cp1252') #ajusta o texto para codificação que permita acentuação
			
			#renderiza o texto
			t_surf = self.fonte.render(texto, cor, None, pygame.freetype.STYLE_DEFAULT, rot , tam)
			
			#obtem as medidas do retangulo do texto rotacionado
			ret_rot = self.fonte.get_rect(texto, pygame.freetype.STYLE_DEFAULT, rot , tam)
			centro_x = ret_rot.width / 2
			centro_y = ret_rot.height / 2
			
			#corrige a origem em y para emular um plano cartesiano
			y_orig = self.kwargs['size'][1]
			pt1[1] = y_orig - pt1[1] - ret_rot.height + (alin[1]-1)*centro_y
			
			#corrige a origem em x devido a rotação do texto
			pt1[0] = pt1[0] + ret_rot.x - alin[0] * centro_x
			
			#poe na tela
			self.screen.blit(t_surf[0], pt1)
		
if __name__ == "__main__":
	import time
	
	jan = janela(args=(1,2,3),
		kwargs = {
		'size':[400,200] ,
		'caption':'Teste Pygame'
		})
	time.sleep(1)
	jan.zoom = 0.5
	jan.offset = [-100,-100]
	#jan.desenha()
	jan.linha([0,0], [100,100])
	jan.texto('Textozão', [100,100], 20, 0, (255,0,0))
	jan.texto('Textozão', [100,100], 20, 30, (255,0,0))
	jan.texto('Textozão', [100,100], 20, 60, (255,0,0))
	jan.texto('Textozão', [100,100], 20, 90, (255,0,0))
	
	jan.texto('BL', [250,100], 20, 0, (255,0,0), (0,1))
	jan.texto('MC', [250,100], 20, 0, (0,255,0), (1,2))
	jan.texto('TR', [250,100], 20, 0, (0,0,255), (2,3))
	
	jan.arco([100,100], 50, 30, 60, cor=(0,255,0), esp=1)
	jan.arco([100,100], 70, 0, 240, cor=(0,0,255), esp=2)
	jan.redesenha = jan.desenha2
	'''
	print 'Janela Continua em execucao'
	time.sleep(2)
	jan.desenha()
	'''