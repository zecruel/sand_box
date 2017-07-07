#include "dxf.h"
int ident_attr_type (int group){
	/* Identifies the data type of the attribute,  */
	/* according to the value of the group. (DXF ranges) */
	
	if ((group >= 0) && (group < 10)){
		return DXF_STR; /* string */
	}
	else if( (group >= 10) && (group < 60)){
		return DXF_FLOAT; /* float */
	}
	else if( (group >= 60) && (group < 80)){
		return DXF_INT; /* integer */
	}
	else if( (group >= 140) && (group < 148)){
		return DXF_FLOAT; /* float */
	}
	else if( (group >= 170) && (group < 176)){
		return DXF_INT; /* integer */
	}
	else if( (group >= 210) && (group < 240)){
		return DXF_FLOAT; /*  float */
	}
	else if( (group >= 999) && (group < 1010)){
		return DXF_STR; /* string */
	}
	else if( (group >= 1010) && (group < 1060)){
		return DXF_FLOAT; /* float */
	}
	else if( (group >= 1060) && (group < 1080)){
		return DXF_INT; /* integer */
	}
	else{ /* not defined type */
		return DXF_STR; /* default type is string */
	}
}

int dxf_attr_append(dxf_node *master, int group, void *value){
	if (master){
		int type = ident_attr_type(group);
		dxf_node *new_attr = dxf_attr_new(group, type, value);
		if (new_attr){
			/*  append new to last obj's list */
			new_attr->master = master;
			new_attr->prev = master->end;
			if (master->end){
				master->end->next = new_attr;
			}
			new_attr->next = NULL; /* append at end of list */
			master->end = new_attr;
			return 1;
		}
	}
	return 0;
}

dxf_node * dxf_new_line (double x0, double y0, double z0,
double x1, double y1, double z1,
double thick, double elev, int color, char *layer, char *ltype, int paper){
	int ok = 1;
	dxf_node * new_line = dxf_obj_new ("LINE");
	ok &= dxf_attr_append(new_line, 6, (void *) ltype);
	ok &= dxf_attr_append(new_line, 8, (void *) layer);
	ok &= dxf_attr_append(new_line, 10, (void *) &x0);
	ok &= dxf_attr_append(new_line, 11, (void *) &x1);
	ok &= dxf_attr_append(new_line, 20, (void *) &y0);
	ok &= dxf_attr_append(new_line, 21, (void *) &y1);
	ok &= dxf_attr_append(new_line, 30, (void *) &z0);
	ok &= dxf_attr_append(new_line, 31, (void *) &z1);
	ok &= dxf_attr_append(new_line, 38, (void *) &elev);
	ok &= dxf_attr_append(new_line, 39, (void *) &thick);
	ok &= dxf_attr_append(new_line, 62, (void *) &color);
	ok &= dxf_attr_append(new_line, 67, (void *) &paper);
	if(ok){
		return new_line;
	}

	return NULL;
}

void drawing_ent_append(dxf_drawing *drawing, dxf_node *element){
	if (drawing && element){
		if ((drawing->ents != NULL) && (drawing->main_struct != NULL)){
			/*  append drawing entities's list */
			element->master = drawing->ents;
			element->prev = drawing->ents->end;
			if (drawing->ents->end){
				drawing->ents->end->next = element;
			}
			element->next = NULL; /* append at end of list */
			drawing->ents->end = element;
		}
	}
}