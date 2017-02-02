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
	dxf_ltype ltype;
	double rot, scale, ofs_x, ofs_y;
	line_node * list;
};
typedef struct Graph_obj graph_obj;