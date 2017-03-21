#include "dxf.h"
#include "bmp.h"
#include "graph.h"

vector_p * dxf_graph_parse(dxf_drawing drawing, dxf_node * ent, int p_space);

int dxf_ents_parse(dxf_drawing drawing);

int dxf_ents_draw(dxf_drawing drawing, bmp_img * img, double ofs_x, double ofs_y, double scale);

int dxf_ents_ext(dxf_drawing drawing, double * min_x, double * min_y, double * max_x, double * max_y);