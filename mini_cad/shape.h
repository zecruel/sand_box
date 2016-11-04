#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <locale.h>

struct Shape{
	long numero;
	char *nome;
	unsigned char *comandos;
	unsigned int tam_com;
	struct Shape *prox;
}; 
typedef struct Shape shape;

struct Lin{
	double pt1_x, pt1_y, pt2_x, pt2_y;
	struct Lin *prox;
}; 
typedef struct Lin lin;

void fonte_add(shape *fonte, long numero, char *nome, unsigned char *comandos, unsigned int tam_com);

void fonte_limpa(shape *fonte);

void fonte_exibe(shape *fonte);

shape *fonte_busca(shape *fonte, long numero);

void lin_add(lin *lista_lin, double pt1_x, double pt1_y, double pt2_x, double pt2_y);

void lin_limpa(lin *lista_lin);

void lin_exibe(lin *lista_lin);

shape *fonte_abre(char *arquivo);

lin *interpreta(shape *fonte, char *texto);