#include "shape.h"

const double direcoes_x[16] = {1, 1, 1, 0.5, 0, -0.5, -1, -1, -1, -1, -1, -0.5, 0, 0.5, 1, 1};
const double direcoes_y[16] = {0, 0.5, 1, 1, 1, 1, 1, 0.5, 0, -0.5, -1, -1, -1, -1, -1, -0.5};
const double octantes_x[8] = {1, 0.707106781, 0, -0.707106781, -1, -0.707106781, 0, 0.707106781};	
const double octantes_y[8] = {0, 0.707106781, 1, 0.707106781, 0, -0.707106781, -1, -0.707106781};

void fonte_add(shape *fonte, long numero, char *nome, unsigned char *comandos, unsigned int tam_com){
	if (fonte){
		shape *novo = (shape *) malloc(sizeof(shape));
		if (novo){
			novo->numero = numero;
			
			novo->nome = malloc(strlen(nome)+1);
			strcpy(novo->nome, nome);
			
			novo->comandos = malloc(tam_com);
			memcpy(novo->comandos, comandos, tam_com);
			
			novo->tam_com = tam_com;
			
			novo->prox = NULL;
			
			
			if(fonte->prox == NULL){ //verifica se a lista esta vazia
				fonte->prox=novo;} // o novo elemento e o ultimo
			else{ // lista nao vazia
				//busca o final da lista
				shape *tmp = fonte->prox;
				while(tmp->prox != NULL){
					tmp = tmp->prox;}
				tmp->prox = novo; //acrescenta o novo elemento no final
			}
		}
	}
}

void fonte_limpa(shape *fonte){
	if (fonte){
		if(fonte->prox){ //verifica se a lista esta vazia
			shape *proxshape, *atual;
			
			atual = fonte->prox;
			fonte->prox = NULL;
			while(atual){
				proxshape = atual->prox;
				free(atual->nome);
				free(atual->comandos);
				free(atual);
				atual = proxshape;
			}
		}
	}
}

void fonte_exibe(shape *fonte){
	int i;
	if (fonte){
		if(fonte->prox){ //verifica se a lista esta vazia
			shape *atual;
			
			atual = fonte->prox;
			while(atual){
				printf("%lc %d [", atual->numero, atual->numero);
				for(i=0; i < atual->tam_com; i++){
					printf("%u,", atual->comandos[i]);
				}
				printf("]\n");
				atual = atual->prox;
			}
		}
	}
}

shape *fonte_busca(shape *fonte, long numero){
	if (fonte){ //verifica se existe a fonte
		if(fonte->prox){ //verifica se a lista esta vazia
			//pula o primeiro shape da fonte que eh a descricao da propria fonte
			if(fonte->prox->prox){
				shape *atual;
				atual = fonte->prox->prox;
				while(atual){
					if(atual->numero == numero){
						return(atual);}
					atual = atual->prox;
				}
			}
		}
	}
	return(NULL);
}

void lin_add(lin *lista_lin, double pt1_x, double pt1_y, double pt2_x, double pt2_y){
	if (lista_lin){
		lin *novo = (lin *) malloc(sizeof(lin));
		if (novo){
			novo->pt1_x = pt1_x;
			novo->pt1_y = pt1_y;
			novo->pt2_x = pt2_x;
			novo->pt2_y = pt2_y;
			novo->prox = NULL;
			
			if(lista_lin->prox == NULL){ //verifica se a lista esta vazia
				lista_lin->prox=novo;} // o novo elemento e o ultimo
			else{ // lista nao vazia
				//busca o final da lista
				lin *tmp = lista_lin->prox;
				while(tmp->prox != NULL){
					tmp = tmp->prox;}
				tmp->prox = novo; //acrescenta o novo elemento no final
			}
		}
	}
}

void lin_limpa(lin *lista_lin){
	if (lista_lin){
		if(lista_lin->prox){ //verifica se a lista esta vazia
			lin *proxlin, *atual;
			
			atual = lista_lin->prox;
			lista_lin->prox = NULL;
			while(atual){
				proxlin = atual->prox;
				free(atual);
				atual = proxlin;
			}
		}
	}
}

void lin_exibe(lin *lista_lin){
	if (lista_lin){
		if(lista_lin->prox){ //verifica se a lista esta vazia
			lin *atual;
			
			atual = lista_lin->prox;
			while(atual){
				printf("(%3.1f;%3.1f)-", atual->pt1_x, atual->pt1_y);
				printf("(%3.1f;%3.1f),", atual->pt2_x, atual->pt2_y);
				atual = atual->prox;
			}
		}
	}
}

shape *fonte_abre(char *arquivo){
	FILE *arq;
	shape *fonte;
	
	int valor; //valor lido na posicao atual do arquivo
	long index = 0; //posicao atual no arquivo
	long prox_index = 0; //indica o proxima marca a ser processada
	
	int comentario = 0; //flag que indica que esta lendo os comentarios iniciais do arquivo shx
	int cabecalho = 0; //flag que indica que esta lendo o cabecalho do shape
	int nome = 0; //flag que indica que esta lendo os dados do shape
	
	long numero; //numero do shape
	char str_tmp[255]; //armazena a string de nome de cada shape
	unsigned char buffer[255]; //armazena a sequencia de comandos de cada shape
	unsigned int tam_buff = 0; //tamanho da sequencia de comandos
	
	arq = fopen(arquivo,"rb"); //abre o arquivo -> somente leitura, modo binario
	if(arq == NULL) {
		perror("Erro, nao foi possivel abrir o arquivo\n");
		return(NULL);
	}
	
	// cria a lista de shapes da fonte
	fonte = (shape *) malloc(sizeof(shape));
	if(fonte == NULL) {
		perror("Erro, nao foi possivel alocar memoria\n");
		return(NULL);
	}
	fonte->prox = NULL; //indica que a lista esta vazia
	
	while ((valor = (unsigned) fgetc(arq)) != EOF){ //considera o byte lido como um inteiro nao negativo
		index++;
		if (!comentario){
			//busca os comentarios no inicio do arquivo
			if (valor != 26){
				//printf("%c", valor); //imprime os comentarios
				continue;
			}
			else{ //termina quando encontra o valor 26 (1A hex)
				comentario = 1; //sinaliza que esta pronto
				
				//espera que o proximo campo tenha 6 bytes de comprimento (cabecalho do arquivo)
				prox_index = index + 6;
				continue;
			}
		}
		//a partir deste ponto comeca a ler as definicoes de cada letra
		// as rotinas abaixo serao repetidas para cada letra
		else if (!cabecalho){
			//busca o cabecalho da letra
			if (index != prox_index){
				buffer[tam_buff] = valor; //armazena no buffer
				tam_buff++;
			}
			else{ //termina quando atinge o comprimento de 4 bytes (ou 6, se for a definicao da fonte)
				//verifica se o ultimo byte eh igual a zero, senao para por erro
				if (valor != 0){
					break; //<=erro
				}
				
				//o ultimo byte eh o comprimento do campo de dados da letra
				prox_index = index + buffer[tam_buff - 1];
				
				// combina os dois primeiros bytes como um inteiro, que eh a especificacao unicode
				numero = ((buffer[1]&255) << 8)|(buffer[0]&255);
				
				//prepara para a leitura dos dados
				cabecalho = 1;
				nome = 0;
				tam_buff = 0;
			}
		}	
		else if (cabecalho){
			//busca os dados da letra
			if (index != prox_index){
				if (!nome){ //busca o nome do shape
					if (valor != 0){ //o valor 00 indica o fim do nome
						str_tmp[tam_buff] = valor; //armazena na string temporaria
						tam_buff++;
					}
					else{
						str_tmp[tam_buff] = valor; // completa a string
						
						//prepara para leitura dos comandos de desenho
						tam_buff = 0;
						nome = 1;
					}
				}
				else{ //comandos de desenho
					buffer[tam_buff] = valor; //armazena no buffer
					tam_buff++;
				}
			}
			else{ //termina quando atinge o comprimento especificado
				//verifica se o ultimo byte eh igual a zero, senao para por erro
				if (valor != 0){
					break; //<=erro
				}
				//adiciona o shape a lista da fonte
				fonte_add(fonte, numero, str_tmp, buffer, tam_buff);
				
				// prepara para a proxima letra
				prox_index = index + 4; //cabecalho com 4 bytes de compr
				cabecalho = 0;
				tam_buff = 0;
			}
		}
	}
	//printf ("\nbytes: %d\n", index);
	
	fclose(arq); // fecha o arquivo	
	return(fonte); // retorna a fonte
}

lin *interpreta(shape *fonte, const char *texto, double *altura, double *largura){
	double pre_x = 0;
	double pre_y = 0;
	double px = 0;
	double py = 0;
	int pena = 1;
	int executa = 0;
	lin *lista = NULL;
	double max_x = 0;
	double max_y = 0;
	double min_x = 0;
	double min_y = 0;
	double pilha_x[50];
	double pilha_y[50];
	int tam_pilha = 0;
	
	long index = 0;
	long prox_index = 0;
	int comando = 0; //sem comando
	int coord_y = 0; //indica que eh a coordenada y
	int pula = 0;
	int bulge_f = 0;
	double bulge = 0;
	int arco_f = 0;
	double esc_tmp = 1.0;
	double escala = 1.0;
	
	double compr, centro_x, centro_y, ang_ini;
	int vetor, octante, sentido, num_oct, raio;
	
	wchar_t str_uni[255];
	int tam_str;
	
	int i, j, ii;
	
	//double altura, largura;
	
	//cria a lista de retorno
	lista = (lin *) malloc(sizeof(lin));
	if (lista){
		lista->prox = NULL;
	}
	
	//converte o texto em uma string unicode
	tam_str = mbstowcs(str_uni, texto, 255); //tamanho maximo de 255
	//printf("tamanho str = %d\n", tam_str);
	//printf("str = %s\n", str_uni);

	
	for(i = 0; i < tam_str; i++){
		//procura a letra na fonte
		shape *letra = fonte_busca(fonte, (long) str_uni[i]);
		if(letra){
			//printf("\nCod. %d\n", letra->numero);
		//}
		//else: i = [42, 'asterisco', [0, 2, 14, 8, 254, 251, 33, 1, 68, 2, 46, 1, 72, 2, 65, 1, 74, 2, 68, 1, 78, 2, 47, 14, 8, 252, 253]]
		
		prox_index = 0;
		for(index = 0; index < letra->tam_com; index++){
			j = letra->comandos[index];
			if (index == prox_index){
				executa = 0;
				comando = j;
				//printf("%d,", comando);
				
				if(comando > 14){
					//comando imediato de movimento
					prox_index = index + 1;
					//obtem o comprimento do nibble maior
					compr = (comando & 240)/16;
					//obtem o indice do vetor do nibble menor
					vetor = comando & 15;
					// o vetor eh obtido da consultando o indice na tabela
					px = compr * direcoes_x[vetor];
					py = compr * direcoes_y[vetor];
					//print compr, vetor
					executa = 1;
				}
				else if(comando == 1){
					//desenho ativado
					prox_index = index + 1;
					if (pula){
						pula = 0;
						continue;
					}
					pena = 1;
					continue;
				}
				else if(comando == 2){
					//desenho desligado
					prox_index = index + 1;
					if (pula){ 
						pula = 0;
						continue;
					}
					pena = 0;
					continue;
				}
				else if(comando == 3){
					//escala divide
					prox_index = index + 2;
					continue;
				}
				else if(comando == 4){
					//escala multiplica
					prox_index = index + 2;
					continue;
				}
				else if(comando == 5){
					//salva posicao atual
					prox_index = index + 1;
					if(pula){ 
						pula = 0;
						continue;
					}
					pilha_x[tam_pilha] = pre_x;
					pilha_y[tam_pilha] = pre_y;
					tam_pilha++;
					
					continue;
				}
				else if(comando == 6){
					//restaura posicao atual
					prox_index = index + 1;
					if(pula){ 
						pula = 0;
						continue;
					}
					if(tam_pilha>0){
						tam_pilha--;
						pre_x = pilha_x[tam_pilha];
						pre_y = pilha_y[tam_pilha];
					}
					continue;
				}
				else if(comando == 7){
					//subshape
					prox_index = index + 3; //unicode pula 2 bytes
					if(pula){ 
						pula = 0;
						continue;
					}
					//
					continue;
				}
				else if(comando == 8){
					//uma coordenada (x,y)
					prox_index = index + 3;
					coord_y = 0;
					continue;
				}
				else if(comando == 9){
					// sequencia de coordenadas (x,y), terminada em (0,0)
					prox_index = index + 3;
					coord_y = 0;
					continue;
				}
				else if(comando == 10){
					//arco por octante
					prox_index = index + 3;
					continue;
				}
				else if(comando == 11){
					//arco fracionario
					prox_index = index + 6;
					if(pula){ 
						pula = 0;
						continue;
					}
					//
					continue;
				}
				else if(comando == 12){
					//arco por bulge
					prox_index = index + 4;
					if(pula){ 
						pula = 0;
						continue;
					}
					//
					continue;
				}
				else if(comando == 13){
					//sequencia de arcos por bulge, terminada em (0,0)
					prox_index = index + 3;
					if(pula){ 
						pula = 0;
						continue;
					}
					//
					continue;
				}
				else if(comando == 14){
					// salta o proximo comando se eh um texto horizontal
					prox_index = index + 1;
					pula = 1;
					continue;
				}
			}
			else{
				//print comando
				if(comando == 3){
					if(abs(j) > 0){ esc_tmp = escala/j; }
					executa = 1;
				}
				if(comando == 4){
					esc_tmp = escala*j;
					executa = 1;
				}
				if(comando == 8){
					if(!coord_y){
						px = (double)((signed char) j);
						coord_y = 1;
						continue;
					}
					else{
						py = (double)((signed char) j);
						coord_y = 0;
						executa = 1;
					}
				}
				else if(comando == 9){
					if(!coord_y){
						px = (double)((signed char) j);
						coord_y = 1;
						continue;
					}
					else{
						py = (double)((signed char) j);
						if(!((px==0) && (py==0))){
							coord_y = 0;
							prox_index = index + 3;
							executa = 1;
							//print px, py
						}
					}
				}
				if(comando == 10){
					if(!coord_y){
						raio = j;
						coord_y = 1;
					}
					else{
						//obtem o primeiro octante e o sentido do nibble maior
						octante = (j & 112)/16;
						sentido = (j & 128)/16;
						if(sentido){ sentido = -1;}
						else{ sentido =1;}
						
						//obtem a quantidade de octantes do nibble menor
						num_oct = j & 15;
						if(num_oct == 0){ num_oct = 8;} //circulo completo
						
						coord_y = 0;
						arco_f = 1;
						executa = 1;
					}
				}
				if(comando == 12){
					if(!coord_y){
						px = (double)((signed char) j);
						coord_y = 1;
						continue;
					}
					else if(!bulge_f){
						py = (double)((signed char) j);
						bulge_f = 1;
					}
					else{
						bulge = (double)((signed char) j);
						coord_y = 0;
						bulge_f = 0;
						executa = 1;
					}
				}
				else if(comando == 13){
					if(!coord_y){
						px = (double)((signed char) j);
						coord_y = 1;
						continue;
					}
					else if(!bulge_f){
						py = (double)((signed char) j);
						if(!((px==0) && (py==0))){
							prox_index = index + 2;
							bulge_f = 1;
						}
						continue;
					}
					else{
						bulge = (double)((signed char) j);
						coord_y = 0;
						prox_index = index + 3;
						executa = 1;
						bulge_f = 0;
					}
				}
			}
			if(executa){
				executa = 0;
				//print comando
				if(pula){ 
					pula = 0;
					esc_tmp = escala;
					arco_f = 0;
				}
				else{
					if(escala != esc_tmp){ escala = esc_tmp; }
					else if(arco_f){
						arco_f = 0;
						centro_x = pre_x - raio * octantes_x[octante];
						centro_y = pre_y - raio * octantes_y[octante];
						ang_ini = octante * M_PI/4;
						for(ii=1; ii <= num_oct+1; ii++){
							px = centro_x + raio * cos(2 * M_PI * i * sentido/ 8 + ang_ini);
							py = centro_y + raio * sin(2 * M_PI * i * sentido/ 8 + ang_ini);
							//lista.append(((pre_x,pre_y),(px,py)))
							lin_add(lista, pre_x, pre_y, px, py);
							pre_x=px;
							pre_y=py;
						}
						//print centro_x, centro_y, raio
						//print octante, num_oct
					}
					else{
						px *= escala;
						py *= escala;
						if(pena){
							//adiciona a linha na lista de retorno
							lin_add(lista, pre_x, pre_y, pre_x+px, pre_y+py);
							
							//calcula os valores maximo e minimo de cada coordenada
							if((pre_x + px) > max_x){ max_x = (pre_x + px); }
							if((pre_y + py) > max_y){ max_y = (pre_y + py); }
							if((pre_x + px) < min_x){ min_x = (pre_x + px); }
							if((pre_y + py) < min_y){ min_y = (pre_y + py); }
							//max_x = max([max_x, pre_x, pre_x+px])
							//max_y = max([max_y, pre_y, pre_y+py])
							
							//printf("%3.2f,%3.2f,%3.2f,%3.2f\n", pre_x, pre_y, px, py);
						}
						pre_x += px;
						pre_y += py;
						//print [pre_x,pre_y]
					}
				}
			}
		}
		}//temporario ate implementar o else
	}
	//calcula a altura e a largura do texto interpretado
	*altura = fabs(max_y - min_y);
	*largura = fabs(max_x - min_x);
	
	//printf("Altura = %3.2f, largura = %3.2f\n", altura, largura);
	
	//return lista, max_x, max_y
	return(lista);
}
/*
int main (){
	setlocale(LC_ALL,""); //seta a localidade como a atual do computador para aceitar acentuacao
		
	shape *txt = fonte_abre("txt.shx"); //abre a fonte txt
	//fonte_exibe(txt); //exibe os comandos
	
	lin *teste = interpreta(txt, "testeÇ");
	lin_exibe(teste);
	
	fonte_limpa(txt); //desaloca a memoria
	free(txt);
	lin_limpa(teste);
	free(teste);
	
	return(0);
}*/