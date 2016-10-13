// Example program:
// Using SDL2 to create an application window

#include "SDL.h"
#include <stdio.h>
#include <math.h>
#define MAX_CAMADAS 10
#define MAX_PATTERN 10

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *camadas[MAX_CAMADAS];
int num_camadas = 0;
SDL_Texture *t_fundo = NULL;
SDL_Texture *t_hilite = NULL;
SDL_Event evento;

int pos_x = SDL_WINDOWPOS_UNDEFINED;
int pos_y = SDL_WINDOWPOS_UNDEFINED;
int largura = 600;
int altura = 400;
char titulo[255] = "teste SDL 2.0";
int cor_fundo[4] = {255, 255, 255, 255};
int offset[2] = {0,0};
double zoom = 1.0;
int pix_count = 0;
int pattern[MAX_PATTERN];
int patt_a = 0;
int tam_patt = 0;

int init(void){
	
	pattern[0] = 1;
	
	// inicia o video e timer
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER); 
	
	//cria as entidades SDL basicas
	window = SDL_CreateWindow(
		titulo,             // titulo da janela
		pos_x,           // posicao inicial x
		pos_y,           // posicao inicial y
		largura,         // largura, em pixels
		altura,          // altura, em pixels
		0                  // flags - nehuma
	);
	
	renderer = SDL_CreateRenderer(window, -1, 0); //tela principal
	
	t_fundo = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		largura,
		altura
	); //imagem de fundo
	
	//camadas.append(t_fundo)
	if (num_camadas < MAX_CAMADAS){
		camadas[num_camadas] = t_fundo;
		num_camadas++;}
	
	t_hilite = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		largura,
		altura
	); //imagem de hilite
	
	SDL_SetTextureBlendMode(t_hilite, SDL_BLENDMODE_BLEND);
	//camadas.append(t_hilite)
	if (num_camadas < MAX_CAMADAS){
		camadas[num_camadas] = t_hilite;
		num_camadas++;}
	
	//limpa a imagem de fundo com a cor determinada
	SDL_SetRenderTarget(renderer, t_fundo);
	SDL_SetRenderDrawColor(renderer,
		cor_fundo[0], cor_fundo[1], cor_fundo[2], cor_fundo[3]);
	SDL_RenderClear(renderer);
	
	//exibe a imagem de fundo na tela
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, t_fundo, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	//adiciona um timer com 500ms de resposta
	//SDL_AddTimer(500, sdl_timer, None)
	
	return 0;
}

void sai(void){
	//saida segura do SDL
	int i;
	for (i=0; i < num_camadas; i++){
		SDL_DestroyTexture(camadas[i]);}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void exibe(void){
	//exibe as texturas na tela
	SDL_SetRenderTarget(renderer, NULL);
	int i;
	for (i=0; i < num_camadas; i++){
		SDL_RenderCopy(renderer, camadas[i], NULL, NULL);}
	SDL_RenderPresent(renderer);
}

void limpa_camada(int camada, int cor[4]){
	//limpa a camada com a cor determinada
	if (camada < num_camadas){
		SDL_SetRenderTarget(renderer, camadas[camada]);
		SDL_SetRenderDrawColor(renderer,
			cor[0], cor[1], cor[2], cor[3]);
		SDL_RenderClear(renderer);
	}
}

int eventos(void){
	SDL_PollEvent(&evento);
	return evento.type;
}

void set_camada(int camada, int cor[4]){
	if (camada < num_camadas){
		//verifica se eh a mesma cor do fundo de tela
		if ((cor[0] == cor_fundo[0]) && (
		cor[1] == cor_fundo[1]) && (
		cor[2] == cor_fundo[2])){
			cor[0] ^= 255; //faz um XOR na cor
			cor[1] ^= 255; //faz um XOR na cor
			cor[2] ^= 255; //faz um XOR na cor
		}
		
		SDL_SetRenderTarget(renderer, camadas[camada]);
		SDL_SetRenderDrawColor(renderer,
			cor[0], cor[1], cor[2], cor[3]);
	}
}

void set_pt(int x, int y, int esp){
	///desenha um ponto na camada SDL
	if ((x < largura) && (y < altura) && (x >= 0) && (y >= 0)){
		if (esp<2){
			SDL_RenderDrawPoint(renderer, x, y);}
		else{
			SDL_Rect rect;
			rect.h = esp;
			rect.w = esp;
			rect.x = (int)round(x - (double)esp / 2);
			rect.y = (int)round(y - (double)esp / 2);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void linha(void *entidade, int camada, double ponto1[2], double ponto2[2], int color[4], int esp){

	int pt1[2];
	int pt2[2];
	int x, y, dx, dy, sx, sy, err;
	
	//determina a camada e a cor
	set_camada(camada, color);
	
	//if camada == 1: #para a camada de hilite, aumenta a espessura
	//	esp += 2
	
	
	//aplica zoom e offset aos pontos
	ponto1[0] *= zoom;
	ponto1[1] *= zoom;
	ponto2[0] *= zoom;
	ponto2[1] *= zoom;
	ponto1[0] -= offset[0];
	ponto1[1] -= offset[1];
	ponto2[0] -= offset[0];
	ponto2[1] -= offset[1];
	
	pt1[0] = (int) round(ponto1[0]);
	pt1[1] = (int) round(ponto1[1]);
	pt2[0] = (int) round(ponto2[0]);
	pt2[1] = (int) round(ponto2[1]);
	
	//#desenha somente se a linha esta dentro da area de desenho
	//if util.intersect(pt1, pt2, [0,0], [self.largura, self.altura]):
		
		//self.selec.add_line(entidade, [[pt1[0], pt1[1]], [pt2[0], pt2[1]]])
		
		//corrige a origem em y para emular um plano cartesiano
		pt1[1] = altura - pt1[1];
		pt2[1] = altura - pt2[1];
		
		dx = abs(pt2[0] - pt1[0]);
		dy = abs(pt2[1] - pt1[1]);
		x = pt1[0];
		y = pt1[1];
		sx = (pt1[0] > pt2[0]) ? -1 : 1;
		sy = (pt1[1] > pt2[1]) ? -1 : 1;
		
		if ((abs(dx+dy)) > 0){
			/*otimizacao para linhas verticais e horizontais.
			traduzido da linguagem C da pagina
			http://www.willperone.net/Code/codeline.php*/
			if (dx == 0){  // verticais
				// 'linha vertical'
				if ((x >= 0) && (x < largura)){
					if ((dy*sy) > 0){
						while (y <= pt2[1]){
							pix_count += 1;
							if (pattern[patt_a] >=0){
								set_pt(x, y, esp);}
							if (pix_count >= abs(pattern[patt_a])){
								pix_count = 0;
								patt_a += 1;
								if (patt_a >= tam_patt){
									patt_a = 0;}
							}
							y += 1;
						}
					}
					else{
						y = pt2[1];
						while (y <= pt1[1]){
							pix_count += 1;
							if (pattern[patt_a] >=0){
								set_pt(x, y, esp);}
							if (pix_count >= abs(pattern[patt_a])){
								pix_count = 0;
								patt_a += 1;
								if (patt_a >= tam_patt){
									patt_a = 0;}
							}
							y += 1;
						}
					}
				}
			}
			else if (dy == 0){// horizontais
				// 'linha horizontal'
				if ((y >= 0) && (y < altura)){
					if ((dx*sx) > 0){
						while (x <= pt2[0]){
							pix_count += 1;
							if (pattern[patt_a] >=0){
								set_pt(x, y, esp);}
							if (pix_count >= abs(pattern[patt_a])){
								pix_count = 0;
								patt_a += 1;
								if (patt_a >= tam_patt){
									patt_a = 0;}
							}
							x += 1;
						}
					}
					else {
						x = pt2[0];
						while (x <= pt1[0]){
							pix_count += 1;
							if (pattern[patt_a] >=0){
								set_pt(x, y, esp);}
							if (pix_count >= abs(pattern[patt_a])){
								pix_count = 0;
								patt_a += 1;
								if (patt_a >= tam_patt){
									patt_a = 0;}
							}
							x += 1;
						}
					}
				}
			}
			else{
				/*Bresenham's line algorithm
				http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#pt2[1]thon*/
				// 'linha em angulo'
				if (dx > dy){
					err = dx / 2;
					while (x != pt2[0]){
						pix_count += 1;
						if (pattern[patt_a] >=0){
							set_pt(x, y, esp);}
						if (pix_count >= abs(pattern[patt_a])){
							pix_count = 0;
							patt_a += 1;
							if (patt_a >= tam_patt){
								patt_a = 0;}
						}
						err -= dy;
						if (err < 0){
							y += sy;
							err += dx;
						}
						x += sx;
					}
				}
				else {
					err = dy / 2;
					while (y != pt2[1]){
						pix_count += 1;
						if (pattern[patt_a] >=0){
							set_pt(x, y, esp);}
						if (pix_count >= abs(pattern[patt_a])){
							pix_count = 0;
							patt_a += 1;
							if (patt_a >= tam_patt){
								patt_a = 0;}
						}
						err -= dx;
						if (err < 0){
							x += sx;
							err += dy;
						}
						y += sy;
					}
				}						
				pix_count += 1;
				if (pattern[patt_a] >=0){
					set_pt(x, y, esp);}
				if (pix_count >= abs(pattern[patt_a])){
					pix_count = 0;
					patt_a += 1;
					if (patt_a >= tam_patt){
						patt_a = 0;}
				}
			}
		}
}

void muda_pattern(int pat[], int tam){
	pix_count = 0;
	patt_a = 0;
	if (tam > MAX_PATTERN){
		tam = MAX_PATTERN;}
	tam_patt = tam;
	for (patt_a = 0; patt_a < tam; patt_a++){
		pattern[patt_a] = pat[patt_a];}
	patt_a = 0;
}

void arco(void *entidade, int camada, double centro[2], double raio, double ang_ini, double ang_fim, int cor[4], int esp, int sentido){
	int n = 32; //numero de vertices do polígono regular que aproxima o circulo ->bom numero 
	double angulo;
	int passos, i;
	double pt1[2], pt2[2];
	
	ang_ini *= M_PI/180;
	ang_fim *= M_PI/180;
	
	angulo = (ang_fim-ang_ini) * sentido; //angulo do arco
	if (angulo <= 0){ angulo = angulo + 2*M_PI;}
	
	//descobre quantos passos para o laço a seguir
	passos = (int) floor(fabs(angulo*n/(2*M_PI))); //numero de vertices do arco
	
	pt1[0] = centro[0] + raio * cos(ang_ini);
	pt1[1] = centro[1] + raio * sin(ang_ini);
	
	//já começa do segundo vértice
	for (i=1; i<passos; i++){
		pt2[0] = centro[0] + raio * cos(2 * M_PI * i * sentido/ n + ang_ini);
		pt2[1] = centro[1] + raio * sin(2 * M_PI * i * sentido/ n + ang_ini);
		
		linha(entidade, camada, pt1, pt2, cor, esp);
		pt1[0]=pt2[0];
		pt1[1]=pt2[1];
	}
	// o ultimo vertice do arco eh o ponto final, nao calculado no laço
	pt2[0] = centro[0] + raio * cos(ang_fim);
	pt2[1] = centro[1] + raio * sin(ang_fim);
	linha(entidade, camada, pt1, pt2, cor, esp);
}

void arco_bulge(void *entidade, int camada, double pt1[2], double pt2[2], double bulge, int cor[4], int esp){
	
	double theta, alfa, d, raio, ang_c, ang_ini, ang_final, centro[2];
	int sentido;
	
	theta = 2 * atan(bulge);
	alfa = atan2(pt2[1]-pt1[1], pt2[0]-pt1[0]);
	d = sqrt((pt2[1]-pt1[1])*(pt2[1]-pt1[1]) + (pt2[0]-pt1[0])*(pt2[0]-pt1[0])) / 2;
	raio = d*(bulge*bulge + 1)/(2*bulge);
	
	ang_c = M_PI+(alfa - M_PI/2 - theta);
	centro[0] = raio*cos(ang_c) + pt1[0];
	centro[1] = raio*sin(ang_c) + pt1[1];
	
	//angulo inicial e final obtidos das coordenadas iniciais
	ang_ini = atan2(pt1[1]-centro[1],pt1[0]-centro[0]);
	ang_final = atan2(pt2[1]-centro[1],pt2[0]-centro[0]);
	
	sentido = 1;
	if (bulge < 0){
		ang_ini += M_PI;
		ang_final += M_PI;
		sentido = -1;
	}
	//converte para garus
	ang_ini *= 180/M_PI;
	ang_final *= 180/M_PI;
	
	arco(entidade, camada, centro, raio, ang_ini, ang_final, cor, esp, sentido);
}
/*
int main(int argc, char* argv[]) {

    SDL_Event event;

    init();
	while (1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) {
		    break;
		}
	    }

    sai();
    return 0;
}*/