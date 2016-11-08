// Example program:
// Using SDL2 to create an application window
/*
import ctypes
a = "hello world"
print ctypes.cast(id(a), ctypes.py_object).value
*/
#include "SDL.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "shape.h"
#define MAX_CAMADAS 10
#define MAX_PATTERN 10
#define MAX_FONTES 10
#define TAM_RET 5

//estrutura de lista encadeada
//armazena as linhas do desenho visivel
struct Node{
	int pt1_x, pt1_y, pt2_x, pt2_y;
	long entidade;
	struct Node *prox;
}; 
typedef struct Node node;



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

double offset[2] = {0,0};
double zoom = 1.0;

int pix_count = 0;
int pattern[MAX_PATTERN];
int patt_a = 0;
int tam_patt = 0;

node *lista_busca = NULL;
shape *fontes[MAX_FONTES];
int num_fontes = 0;
//shape *fonte = NULL;

void lista_add(long entidade, int pt1_x, int pt1_y, int pt2_x, int pt2_y){
	if (lista_busca){
		node *novo = (node *) malloc(sizeof(node));
		if (novo){
			novo->entidade = entidade;
			novo->pt1_x = pt1_x;
			novo->pt1_y = pt1_y;
			novo->pt2_x = pt2_x;
			novo->pt2_y = pt2_y;
			novo->prox = NULL;
			if(lista_busca->prox == NULL){ //verifica se a lista esta vazia
				lista_busca->prox=novo;} // o novo elemento e o ultimo
			else{ // lista nao vazia
				//busca o final da lista
				node *tmp = lista_busca->prox;
				while(tmp->prox != NULL){
					tmp = tmp->prox;}
				tmp->prox = novo; //acrescenta o novo elemento no final
			}
		}
	}
}

void lista_limpa(void){
	if (lista_busca){
		if(lista_busca->prox){ //verifica se a lista esta vazia
			node *proxNode, *atual;
			
			atual = lista_busca->prox;
			lista_busca->prox = NULL;
			while(atual){
				proxNode = atual->prox;
				free(atual);
				atual = proxNode;
			}
		}
	}
}

long busca_ret(int ret1[2], int ret2 [2]){
	if (lista_busca){
		if(lista_busca->prox){ //verifica se a lista esta vazia
			node *proxNode, *atual;
			int pt1[2], pt2[2];
			
			atual = lista_busca->prox;
			while(atual){
				proxNode = atual->prox;
				pt1[0] = atual->pt1_x;
				pt1[1] = atual->pt1_y;
				pt2[0] = atual->pt2_x;
				pt2[1] = atual->pt2_y;
				if (intersect(pt1, pt2, ret1, ret2)){
					return atual->entidade;
				}
				atual = proxNode;
			}
		}
	}
	return 0;
}

int nova_fonte(char *arquivo){
	if(num_fontes < MAX_FONTES){ //verifica se o maximo admitido de fontes foi atingido
		fontes[num_fontes] = fonte_abre(arquivo); //abre a fonte desejada
		if(fontes[num_fontes]){ //verifica se obteve sucesso
			num_fontes++;
			return num_fontes; //retorna o indice + 1 da fonte
		}
	}
	return 0; //retorna 0 se nao sucedido
}

int init(int x, int y, int larg, int alt, char tit[255], int fundo[4]){
	
	//atualiza os param globais com os valores passados
	int i;
	pos_x = x;
	pos_y = y;
	largura = larg;
	altura = alt;
	
	for(i=0; i < 255; i++){
		titulo[i] = tit[i];}
	for(i=0; i < 4; i++){
		cor_fundo[i] = fundo[i];}
	for(i=0; i < MAX_FONTES; i++){
		fontes[i] = NULL;}
	
	// inicializa o tipo de linha com linha solida
	pattern[0] = 1;
		
	//inicializa a lista de busca
	lista_busca = (node *) malloc(sizeof(node));
	if (lista_busca) { 
		lista_busca->prox = NULL;} //lista vazia
	
	//inicializa a fonte
	nova_fonte("txt.shx"); //abre a fonte txt -> o indice dela eh o 1
	
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
	if (lista_busca){
		lista_limpa();
		free(lista_busca);
	}
	for (i=0; i < num_fontes; i++){ //desaloca a memoria das fontes
		fonte_limpa(fontes[i]);
		free(fontes[i]);
	}
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

int eventos(SDL_Event *ev, long *entidade, int *botoes, int *x, int *y){
	if (SDL_PollEvent(ev)){
		if ((ev->type >= 1024)&&(ev->type < 1028)){ //eventos de mouse
			int ret1[2], ret2[2];
			
			*botoes = SDL_GetMouseState(x, y);
			*y = altura - *y;
			ret1[0] = *x - TAM_RET;
			ret1[1] = *y - TAM_RET;
			ret2[0] = *x + TAM_RET;
			ret2[1] = *y + TAM_RET;
			*entidade = busca_ret(ret1, ret2);
			//printf("Achei %d \n", entidade);
		}
		return 1;
	}
	return 0;
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

void linha(long entidade, int camada, double p1[2], double p2[2], int color[4], int esp){
	
	double ponto1[2], ponto2[2];
	int pt1[2], pt2[2], ret1[2], ret2[2];
	int x, y, dx, dy, sx, sy, err;
	
	//determina a camada e a cor
	set_camada(camada, color);
	
	//if camada == 1: #para a camada de hilite, aumenta a espessura
	//	esp += 2
	
	
	//aplica zoom e offset aos pontos
	ponto1[0] = zoom * p1[0];
	ponto1[1] = zoom * p1[1];
	ponto2[0] = zoom * p2[0];
	ponto2[1] = zoom * p2[1];
	ponto1[0] = ponto1[0] - offset[0];
	ponto1[1] = ponto1[1] - offset[1];
	ponto2[0] = ponto2[0] - offset[0];
	ponto2[1] = ponto2[1] - offset[1];
	
	pt1[0] = (int) round(ponto1[0]);
	pt1[1] = (int) round(ponto1[1]);
	pt2[0] = (int) round(ponto2[0]);
	pt2[1] = (int) round(ponto2[1]);
	
	ret1[0] = 0;
	ret1[1] = 0;
	ret2[0] = largura;
	ret2[1] = altura;
	
	//#desenha somente se a linha esta dentro da area de desenho
	if (intersect(pt1, pt2, ret1, ret2)){
		
		//self.selec.add_line(entidade, [[pt1[0], pt1[1]], [pt2[0], pt2[1]]])
		lista_add(entidade, pt1[0], pt1[1], pt2[0], pt2[1]);
		
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

void muda_zoom(double n_zoom, double n_offset[2]){
	zoom = n_zoom;
	offset[0] = n_offset[0];
	offset[1] = n_offset[1];
}

void arco(long entidade, int camada, double centro[2], double raio, double ang_ini, double ang_fim, int cor[4], int esp, int sentido){
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

void arco_bulge(long entidade, int camada, double pt1[2], double pt2[2], double bulge, int cor[4], int esp){
	
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

void texto_shx(long entidade, int camada, int i_fonte, char *txt, double pt1[2], double pt2[2], double tam, double rot, int cor[4], int alin_h, int alin_v, int esp){
	
	double tam_fonte = 4;
	double tamanho = 1;
	double centro_x = 0;
	double centro_y = 0;
	double base_x = 0;
	double base_y = 0;
	double pos_x = 0;
	double pos_y = 0;
	double largura, altura;
	double pivo_x = 0;
	double pivo_y = 0;
	double seno = 0;
	double cosseno = 1;
	double p1_x, p1_y, p2_x, p2_y;
	double p1r_x, p1r_y, p2r_x, p2r_y;
	double ponto1[2], ponto2[2];
	double acima, abaixo;
	lin *linhas = NULL;
	shape *fonte = NULL;
	
	if((i_fonte <= num_fontes) && (i_fonte > 0)){
		fonte = fontes[i_fonte - 1];
	}
	
	//pega as dimensoes da fonte
	if(fonte){ //verifica se existe a fonte
		if(fonte->prox){ //o primeiro item da lista eh a descricao da fonte
			if(fonte->prox->tam_com > 1){ //verfica se a fonte eh valida
				acima = fonte->prox->comandos[0]; //dimensao acima da linha base
				abaixo = fonte->prox->comandos[1]; //dimensao abaixo da linha base
				if((acima + abaixo) > 0){
					tam_fonte = acima + abaixo;
				}
			}
		}
	}
	
	//renderiza o texto
	linhas = interpreta(fonte, txt, &altura, &largura);
	
	if (linhas){
		if(linhas->prox){ //verifica se a lista nao esta vazia
			tamanho = tam/tam_fonte*2;
			
			//determina a posicao de insercao do texto em funcao de seu alinhamento
			centro_x = alin_h * (largura * tamanho/2);
			centro_y = (alin_v - 1) * (tamanho/2);
			base_x =  alin_h * (pt2[0] - pt1[0])/2;
			base_y =  alin_h * (pt2[1] - pt1[1])/2;
			pos_x = pt1[0] + base_x - centro_x;
			pos_y = pt1[1] + base_y - centro_y;
			
			if(rot != 0){ //calcula as constantes de rotacao
				cosseno = cos(rot*M_PI/180);
				seno = sin(rot*M_PI/180);
				pivo_x = pt1[0] + base_x;
				pivo_y = pt1[1] + base_y;
			}
			
			//printf("tam=%3.2f, altura=%3.2f, largura=%3.2f\n",tamanho, altura, largura);
			
			lin *atual;
			atual = linhas->prox;
			while(atual){ //varre a lista de linhas
				p1_x = tamanho * atual->pt1_x + pos_x;
				p1_y = tamanho * atual->pt1_y + pos_y;
				p2_x = tamanho * atual->pt2_x + pos_x;
				p2_y = tamanho * atual->pt2_y + pos_y;
				
				//aplica rotacao
				if(rot != 0){
					p1r_x = cosseno*(p1_x-pivo_x) - seno*(p1_y-pivo_y) + pivo_x;
					p1r_y = seno*(p1_x-pivo_x) + cosseno*(p1_y-pivo_y) + pivo_y;
					p2r_x = cosseno*(p2_x-pivo_x) - seno*(p2_y-pivo_y) + pivo_x;
					p2r_y = seno*(p2_x-pivo_x) + cosseno*(p2_y-pivo_y) + pivo_y;
					p1_x = p1r_x;
					p1_y = p1r_y;
					p2_x = p2r_x;
					p2_y = p2r_y;
				}
				//print entidade
				//self.linha(entidade, camada, [p1_x,p1_y], [p2_x,p2_y], cor)
				ponto1[0] = p1_x;
				ponto1[1] = p1_y;
				ponto2[0] = p2_x;
				ponto2[1] = p2_y;
				linha(entidade, camada, ponto1, ponto2, cor, esp);
				atual = atual->prox;
			}
		}
		lin_limpa(linhas);
		free(linhas);
	}
}

int intersect(int lin_pt1[2], int lin_pt2[2], int rect_pt1[2], int rect_pt2[2]){
	//testa se uma linha intercepta um retangulo
	
	int r_bl_x, r_bl_y, r_tr_x,r_tr_y;
	int l_bl_x, l_bl_y, l_tr_x, l_tr_y;
	int a, b, c, pol1, pol2, pol3, pol4;
	
	//ordena os cantos do retangulo
	r_bl_x = (rect_pt1[0] < rect_pt2[0]) ? rect_pt1[0] : rect_pt2[0];
	r_bl_y = (rect_pt1[1] < rect_pt2[1]) ? rect_pt1[1] : rect_pt2[1];
	r_tr_x = (rect_pt1[0] > rect_pt2[0]) ? rect_pt1[0] : rect_pt2[0];
	r_tr_y = (rect_pt1[1] > rect_pt2[1]) ? rect_pt1[1] : rect_pt2[1];
	
	//ordena os cantos do retangulo da linha
	l_bl_x = (lin_pt1[0] < lin_pt2[0]) ? lin_pt1[0] : lin_pt2[0];
	l_bl_y = (lin_pt1[1] < lin_pt2[1]) ? lin_pt1[1] : lin_pt2[1];
	l_tr_x = (lin_pt1[0] > lin_pt2[0]) ? lin_pt1[0] : lin_pt2[0];
	l_tr_y = (lin_pt1[1] > lin_pt2[1]) ? lin_pt1[1] : lin_pt2[1];
	
	//verifica se a linha esta fora de alcance do retangulo
	if (((lin_pt1[0] > r_tr_x) && (lin_pt2[0] > r_tr_x)) || ( //linha a direita
	(lin_pt1[1] > r_tr_y) && (lin_pt2[1] > r_tr_y)) || (  //linha acima
	(lin_pt1[0] < r_bl_x) && (lin_pt2[0] < r_bl_x)) || ( //linha a esquerda
	(lin_pt1[1] < r_bl_y) && (lin_pt2[1] < r_bl_y))){ //linha abaixo
		return 0;}
	else{ //verifica se a linha cruza o retangulo
		//calcula as polarizacoes dos quatro cantos do retangulo
		a = lin_pt2[1]-lin_pt1[1];
		b = lin_pt1[0]-lin_pt2[0];
		c = lin_pt2[0]*lin_pt1[1] - lin_pt1[0]*lin_pt2[1];
		
		pol1 = a*rect_pt1[0] + b*rect_pt1[1] + c;
		pol2 = a*rect_pt1[0] + b*rect_pt2[1] + c;
		pol3 = a*rect_pt2[0] + b*rect_pt1[1] + c;
		pol4 = a*rect_pt2[0] + b*rect_pt2[1] + c;
		if (((pol1>=0) && (pol2>=0) && (pol3>=0) && (pol4>=0)) ||(
		(pol1<0) && (pol2<0) && (pol3<0) && (pol4<0))){
			return 0;}
		return 1;
	}
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