#include "dxf.h"
#include "list.h"

int dxf_attr_append(dxf_node *master, int group, void *value){
	if (master){
		if (master->type == DXF_ENT){
			int type = dxf_ident_attr_type(group);
			dxf_node *new_attr = dxf_attr_new(group, type, value);
			if (new_attr){
				new_attr->master = master;
				/* start search at end of master's list */
				dxf_node *next = NULL, *prev = master->end;
				/*  find the last attribute*/
				if (prev){
					while (prev->type == DXF_ENT){
						next = prev;
						prev = prev->prev;
						if (!prev) break;
					}
				}	
				
				/* append new attr between prev and next nodes */
				new_attr->prev = prev;
				if (prev){
					prev->next = new_attr;
				}
				new_attr->next = next;
				
				if (prev == master->end){
					master->end = new_attr;
				}
				
				return 1;
			}
		}
	}
	return 0;
}

int dxf_attr_change(dxf_node *master, int group, void *value){
	if (master){
		dxf_node *found_attr = dxf_find_attr2(master, group);
		if (found_attr){
			int type = dxf_ident_attr_type(group);
			switch(type) {
				case DXF_FLOAT :
					found_attr->value.d_data = *((double *)value);
					break;
				case DXF_INT :
					found_attr->value.i_data = *((int *)value);
					break;
				case DXF_STR :
					found_attr->value.s_data[0] = 0;
					strncpy(found_attr->value.s_data,(char *) value, DXF_MAX_CHARS); /* and copy the string */
			}
			return 1;
		}
	}
	return 0;
}

int dxf_find_ext_appid(dxf_node *obj, char *appid, dxf_node **start, dxf_node **end){
	dxf_node *current;
	int found = 0;
	
	*start = NULL;
	*end = NULL;
	
	if(obj != NULL){ /* check if exist */
		if (obj->type == DXF_ENT){
			current = obj->obj.content->next;
			while (current){
				if (!found){
					if (current->type == DXF_ATTR){
						if(current->value.group == 1001){
							if(strcmp((char*) current->value.s_data, appid) == 0){
								found = 1; /* appid found */
								*start = current;
							}
						}
					}
				}
				else{
					if (current->type == DXF_ATTR){
						if(current->value.group == 1001){
							break;
						}
					}
					else break;
				}
				current = current->next;
			}
			*end = current;
		}
	}
	return found;
}



dxf_node * dxf_new_line (double x0, double y0, double z0,
double x1, double y1, double z1,
double thick, double elev, int color, char *layer, char *ltype, int paper){
	const char *handle = "0";
	const char *dxf_class = "AcDbEntity";
	const char *dxf_subclass = "AcDbLine";
	int ok = 1;
	dxf_node * new_line = dxf_obj_new ("LINE");
	
	ok &= dxf_attr_append(new_line, 5, (void *) handle);
	//ok &= dxf_attr_append(new_line, 330, (void *) handle);
	ok &= dxf_attr_append(new_line, 100, (void *) dxf_class);
	ok &= dxf_attr_append(new_line, 67, (void *) &paper);
	ok &= dxf_attr_append(new_line, 8, (void *) layer);
	ok &= dxf_attr_append(new_line, 6, (void *) ltype);
	ok &= dxf_attr_append(new_line, 62, (void *) &color);
	
	ok &= dxf_attr_append(new_line, 100, (void *) dxf_subclass);
	ok &= dxf_attr_append(new_line, 39, (void *) &thick);
	ok &= dxf_attr_append(new_line, 10, (void *) &x0);
	ok &= dxf_attr_append(new_line, 20, (void *) &y0);
	ok &= dxf_attr_append(new_line, 30, (void *) &z0);
	ok &= dxf_attr_append(new_line, 11, (void *) &x1);
	ok &= dxf_attr_append(new_line, 21, (void *) &y1);
	ok &= dxf_attr_append(new_line, 31, (void *) &z1);
	
	/* test */
	
	//ok &= dxf_attr_append(new_line, 1001, (void *) "ZECRUEL");
	//ok &= dxf_attr_append(new_line, 1002, (void *) "{");
	//ok &= dxf_attr_append(new_line, 1000, (void *) "test");
	
	
	
	if(ok){
		return new_line;
	}

	return NULL;
}

void drawing_ent_append(dxf_drawing *drawing, dxf_node *element){
	if (drawing && element){
		if ((drawing->ents != NULL) && (drawing->main_struct != NULL)){
			/* get current handle and increment the handle seed*/
			long int handle = 0;
			char hdl_str[DXF_MAX_CHARS];
			
			if (drawing->hand_seed){
				handle = strtol(drawing->hand_seed->value.s_data, NULL, 16); /* get the last handle value and convert to integer */
				snprintf(hdl_str, DXF_MAX_CHARS, "%x", handle); /* convert back to hexadecimal string, to write in element */
				snprintf(drawing->hand_seed->value.s_data, DXF_MAX_CHARS, "%x", handle + 1); /* increment value of seed and write back */
			}
			
			/* change element handle */
			if (handle){
				dxf_attr_change(element, 5, hdl_str);
			}
			
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