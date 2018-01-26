#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "shape2.h"
#include "list.h"

list_node * dxf_graph_parse(dxf_drawing *drawing, dxf_node * ent, int p_space, int pool_idx);

int dxf_ents_parse(dxf_drawing *drawing);

int dxf_ents_draw(dxf_drawing *drawing, bmp_img * img, double ofs_x, double ofs_y, double scale);

int dxf_ents_ext(dxf_drawing *drawing, double * min_x, double * min_y, double * max_x, double * max_y);

list_node * dxf_list_parse(dxf_drawing *drawing, list_node *list, int p_space, int pool_idx);

dxf_node * dxf_ents_isect(dxf_drawing *drawing, double rect_pt1[2], double rect_pt2[2]);

int dxf_ents_isect2(list_node *list, dxf_drawing *drawing, double rect_pt1[2], double rect_pt2[2]);

int dxf_list_draw(list_node *list, bmp_img * img, double ofs_x, double ofs_y, double scale, bmp_color color);