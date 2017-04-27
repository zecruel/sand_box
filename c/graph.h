#ifndef _GRAPH_LIB
#define _GRAPH_LIB

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#include "dxf.h"
#include "bmp.h"

#define MAX_SPLINE_PTS 1000

enum graph_pool_action{
	ADD_GRAPH,
	ADD_LINE,
	ZERO_GRAPH,
	ZERO_LINE,
	FREE_ALL
};

struct Graph_pool_slot{
	void *pool[1000];
	/* the pool is a vector of pages. The size of each page is out of this definition */
	int pos; /* current position in current page vector */
	int page; /* current page index */
	int size; /* number of pages available in slot */
};
typedef struct Graph_pool_slot graph_pool_slot;

struct Line_node{
	double x0, y0, z0, x1, y1, z1;
	struct Line_node * next;
};
typedef struct Line_node line_node;

struct Graph_obj{
	dxf_node * owner;
	bmp_color color;
	double rot, scale, ofs_x, ofs_y, ofs_z;
	double tick;
	/* pattern information */
	double pattern[20];
	int patt_size;
	/* extent information */
	double ext_min_x, ext_min_y;
	double ext_max_x, ext_max_y;
	int ext_ini;
	
	line_node * list;
};
typedef struct Graph_obj graph_obj;

void * graph_mem_pool(enum graph_pool_action action);

graph_obj * graph_new(void);

void line_add(graph_obj * master, double x0, double y0, double z0, double x1, double y1, double z1);

void graph_draw(graph_obj * master, bmp_img * img, double ofs_x, double ofs_y, double scale);

void vec_graph_draw(vector_p * vec, bmp_img * img, double ofs_x, double ofs_y, double scale);

int vec_graph_ext(vector_p * vec, int *init, double * min_x, double * min_y, double * max_x, double * max_y);

void graph_arc(graph_obj * master, double c_x, double c_y, double c_z, double radius, double ang_start, double ang_end, int sig);

void graph_arc_bulge(graph_obj * master, 
		double pt1_x, double pt1_y , double pt1_z,
		double pt2_x, double pt2_y, double pt2_z, 
		double bulge);
			
void graph_ellipse(graph_obj * master,
		double p1_x, double p1_y, double p1_z,
		double p2_x, double p2_y, double p2_z,
		double minor_ax, double ang_start, double ang_end);

void graph_modify(graph_obj * master, double ofs_x, double ofs_y, double scale_x, double scale_y, double rot);

void vec_graph_modify(vector_p * vec, double ofs_x, double ofs_y , double scale_x, double scale_y, double rot);

#endif