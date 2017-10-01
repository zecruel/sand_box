#ifndef _DXF_CREATE_LIB
#define _DXF_CREATE_LIB

#include "dxf.h"
#include "list.h"

int dxf_obj_append(dxf_node *master, dxf_node *obj);

int dxf_obj_detach(dxf_node *obj);

int dxf_attr_append(dxf_node *master, int group, void *value);

int dxf_attr_change(dxf_node *master, int group, void *value);

int dxf_attr_change_i(dxf_node *master, int group, void *value, int idx);

int dxf_find_ext_appid(dxf_node *obj, char *appid, dxf_node **start, dxf_node **end);

int dxf_ext_append(dxf_node *master, char *appid, int group, void *value);

void dxf_obj_transverse(dxf_node *source);

dxf_node *dxf_ent_copy(dxf_node *source, int pool_dest);

dxf_node * dxf_new_line (double x0, double y0, double z0,
double x1, double y1, double z1,
double thick, double elev, int color, char *layer, char *ltype, int paper);

dxf_node * dxf_new_lwpolyline (double x0, double y0, double z0,
double bulge, double thick, int color, char *layer, char *ltype, int paper);

int dxf_lwpoly_append (dxf_node * poly,
double x0, double y0, double z0, double bulge);

int dxf_lwpoly_remove (dxf_node * poly, int idx);

dxf_node * dxf_new_circle (double x0, double y0, double z0,
double r, double thick, int color, char *layer, char *ltype, int paper);

dxf_node * dxf_new_text (double x0, double y0, double z0, double h,
char *txt, double thick, int color, char *layer, char *ltype, int paper);

int dxf_edit_move2 (dxf_node * obj, double ofs_x, double ofs_y, double ofs_z);

int dxf_edit_move (dxf_node * obj, double ofs_x, double ofs_y, double ofs_z);

int dxf_edit_scale (dxf_node * obj, double scale_x, double scale_y, double scale_z);

void drawing_ent_append(dxf_drawing *drawing, dxf_node *element);

#endif