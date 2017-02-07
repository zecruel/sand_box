#include "dxf.h"
#include "bmp.h"
#include "graph.h"

int dxf_graph_parse(dxf_drawing drawing, dxf_node * ent);

int dxf_ents_parse(dxf_drawing drawing);

int dxf_graph_draw(dxf_drawing drawing, dxf_node * ent, bmp_img * img);

int dxf_ents_draw(dxf_drawing drawing, bmp_img * img);