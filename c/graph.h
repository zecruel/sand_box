#ifndef _GRAPH_LIB
#define _GRAPH_LIB

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#include "dxf.h"
#include "bmp.h"

struct Line_node{
	double x0, y0, x1, y1;
	struct Line_node * next;
};
typedef struct Line_node line_node;

struct Graph_obj{
	dxf_node * owner;
	bmp_color color;
	double rot, scale, ofs_x, ofs_y;
	int tick;
	/* pattern information */
	int pattern[20];
	int patt_size;
	/* extent information */
	double ext_min_x, ext_min_y;
	double ext_max_x, ext_max_y;
	int ext_ini;
	
	line_node * list;
};
typedef struct Graph_obj graph_obj;

graph_obj * graph_new(void);

void line_add(graph_obj * master, double x0, double y0, double x1, double y1);

void lines_free(graph_obj * master);

void graph_free(graph_obj * master);

void graph_draw(graph_obj * master, bmp_img * img, double ofs_x, double ofs_y, double scale);

void vec_graph_draw(vector_p * vec, bmp_img * img, double ofs_x, double ofs_y, double scale);

void vec_graph_free(vector_p * vec);

#endif