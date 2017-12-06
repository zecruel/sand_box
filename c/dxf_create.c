#include "dxf_create.h"

void * do_mem_pool(enum dxf_pool_action action){
	
	static struct do_pool_slot entry, item;
	int i;
	
	void *ret_ptr = NULL;
	
	/* initialize the pool, the first allocation */
	if (entry.size < 1){
		entry.pool[0] = malloc(DO_PAGE * sizeof(struct do_entry));
		if (entry.pool){
			entry.size = 1;
			//printf("Init entry\n");
		}
	}
	
	/* if current page is full */
	if ((entry.pos >= DO_PAGE) && (entry.size > 0)){
		/* try to change to page previuosly allocated */
		if (entry.page < entry.size - 1){
			entry.page++;
			entry.pos = 0;
			//printf("change entry page\n");
		}
		/* or then allocatte a new page */
		else if(entry.page < DO_PAGES-1){
			entry.pool[entry.page + 1] = malloc(DO_PAGE * sizeof(struct do_entry));
			if (entry.pool[entry.page + 1]){
				entry.page++;
				entry.size ++;
				entry.pos = 0;
				//printf("Realloc entry\n");
			}
		}
	}
	
	/* initialize the pool, the first allocation */
	if (item.size < 1){
		item.pool[0] = malloc(DO_PAGE * sizeof(struct do_item));
		if (item.pool){
			item.size = 1;
			//printf("Init item\n");
		}
	}
	
	/* if current page is full */
	if ((item.pos >= DO_PAGE) && (item.size > 0)){
		/* try to change to page previuosly allocated */
		if (item.page < item.size - 1){
			item.page++;
			item.pos = 0;
			//printf("change item page\n");
		}
		/* or then allocatte a new page */
		else if(item.page < DO_PAGES-1){
			item.pool[item.page + 1] = malloc(DO_PAGE * sizeof(struct do_item));
			if (item.pool[item.page + 1]){
				item.page++;
				item.size ++;
				item.pos = 0;
				//printf("Realloc item\n");
			}
		}
	}
	
	ret_ptr = NULL;
	
	if ((entry.pool[entry.page] != NULL)){
		switch (action){
			case ADD_DO_ENTRY:
				if (entry.pos < DO_PAGE){
					ret_ptr = &(((struct do_entry *)entry.pool[entry.page])[entry.pos]);
					entry.pos++;
				}
				break;
			case ZERO_DO_ENTRY:
				entry.pos = 0;
				entry.page = 0;
				break;
			case ADD_DO_ITEM:
				if (item.pos < DO_PAGE){
					ret_ptr = &(((struct do_item *)item.pool[item.page])[item.pos]);
					item.pos++;
				}
				break;
			case ZERO_DO_ITEM:
				item.pos = 0;
				item.page = 0;
				break;
			case FREE_DO_ALL:
				for (i = 0; i < entry.size; i++){
					free(entry.pool[i]);
					entry.pool[i] = NULL;
				}
				entry.pos = 0;
				entry.page = 0;
				entry.size = 0;
				for (i = 0; i < item.size; i++){
					free(item.pool[i]);
					item.pool[i] = NULL;
				}
				item.pos = 0;
				item.page = 0;
				item.size = 0;
				break;
		}
	}

	return ret_ptr;
}

int do_add_item(struct do_entry *entry, dxf_node *obj1, dxf_node *obj2){
	int ret = 0;
	if ((entry != NULL) && ((obj1 != NULL) || (obj2 != NULL))){
		struct do_item *item = do_mem_pool(ADD_DO_ITEM);
		if (item){
			if ((entry->current) && (entry->list)){
				entry->current->next = item;
				item->prev = entry->current;
				entry->current = item;
			}
			else{ /* initialize entry's list*/
				entry->list = item;
				entry->current = item;
				item->prev = NULL;
			}
			item->next = NULL;
			item->obj1 = obj1;
			item->obj2 = obj2;
			ret = 1;
		}
	}
	return ret;
};

int do_add_entry(struct do_list *list, char *text){
	int ret = 0;
	if (list != NULL){
		struct do_entry *entry = do_mem_pool(ADD_DO_ENTRY);
		if ((entry) && (list->current) && (list->list)){
			list->count++;
			list->current->next = entry;
			entry->prev = list->current;
			entry->next = NULL;
			list->current = entry;
			
			entry->text[0] = 0; /*initialize string*/
			if (text){
				strncpy(entry->text, text, ACT_CHARS);
				entry->text[ACT_CHARS - 1] = 0; /*terminate string*/
			}
			ret = 1;
		}
	}
	return ret;
};

int init_do_list(struct do_list *list){
	int ret = 0;
	if (list != NULL){
		struct do_entry *entry = do_mem_pool(ADD_DO_ENTRY);
		if (entry){
			list->count = 0;
			list->list = entry;
			list->current = entry;
			entry->prev = NULL;
			entry->next = NULL;
			
			entry->text[0] = 0; /*initialize string*/
			ret =1;
		}
	}
	return ret;
}

int dxf_obj_append(dxf_node *master, dxf_node *obj){
	if ((master) && (obj)){
		if (master->type == DXF_ENT){
			obj->master = master;
			/* start search at end of master's list */
			dxf_node *next = NULL, *prev = master->end;
			
			/* append object between prev and next nodes */
			obj->prev = prev;
			if (prev){
				next = prev->next;
				prev->next = obj;
			}
			obj->next = next;
			if (next){
				next->prev = obj;
			}
			
			if (prev == master->end){
				master->end = obj;
			}
			
			return 1;
		}
	}
	return 0;
}

int dxf_obj_detach(dxf_node *obj){
	/* remove the object from its list */
	if (obj){
		dxf_node *master = obj->master;
		dxf_node *prev = obj->prev;
		dxf_node *next = obj->next;
		/* rebuilt the insertion point */
		if (prev){
			prev->next = next;
			obj->prev = NULL;
		}
		if (next){
			next->prev = prev;
			obj->next = NULL;
		}
		/* verify if the object is at end of master list */
		if (master){
			if (obj == master->end){
				master->end = prev;
			}
		}
		return 1;
	}
	return 0;
}

int dxf_obj_subst(dxf_node *orig, dxf_node *repl){
	/* substitute the orig object from its list for the replace obj */
	if ((repl) && (orig)){
		dxf_node *master = orig->master;
		dxf_node *prev = orig->prev;
		dxf_node *next = orig->next;
		dxf_node *end = orig->end;
		
		/* substitute inherits orig's properties */
		repl->master = master;
		repl->prev = prev;
		repl->next = next;
		repl->end = end;
		
		/* rebuilt the insertion point */
		if (prev){
			prev->next = repl;
		}
		if (next){
			next->prev = repl;
		}
		/* verify if the orig is at end of master list */
		if (master){
			if (orig == master->end){
				master->end = repl;
			}
		}
		return 1;
	}
	else if ((repl == NULL) && (orig)){
		/* detach, but preserve orig properties*/
		dxf_node *master = orig->master;
		dxf_node *prev = orig->prev;
		dxf_node *next = orig->next;
		/* rebuilt the insertion point */
		if (prev){
			prev->next = next;
		}
		if (next){
			next->prev = prev;
		}
		/* verify if the orig is at end of master list */
		if (master){
			if (orig == master->end){
				master->end = prev;
			}
		}
		return 1;
	}
	else if ((repl) && (orig == NULL)){
		/* restore the repl at their referenced point*/
		dxf_node *master = repl->master;
		dxf_node *prev = repl->prev;
		dxf_node *next = repl->next;
		/* rebuilt the insertion point */
		if (prev){
			prev->next = next;
		}
		if (next){
			next->prev = prev;
		}
		/* verify if the orig is at end of master list */
		if (master){
			if (prev == master->end){
				master->end = repl;
			}
		}
		return 1;
	}
	return 0;
}

dxf_node * dxf_obj_cpy(dxf_node *orig, int pool){
	/* copy a DXF object */
	dxf_node *new_obj = NULL;
	if (orig){
		new_obj = (dxf_node *) dxf_mem_pool(ADD_DXF, pool);
		if (new_obj){
			new_obj->master = orig->master;
			new_obj->prev = orig->prev;
			new_obj->next = orig->next;
			new_obj->end = orig->end;
			new_obj->type = orig->type;
			if (new_obj->type == DXF_ENT){
				new_obj->obj = orig->obj;
			}
			else if (new_obj->type == DXF_ATTR){
				new_obj->value = orig->value;
			}
		}
	}
	return new_obj;
}

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
				if (prev){ /*skip if is an entity */
					while (prev->type == DXF_ENT){
						prev = prev->prev;
						if (!prev) break;
					}
				}	
				
				/* append new attr between prev and next nodes */
				new_attr->prev = prev;
				if (prev){
					next = prev->next;
					prev->next = new_attr;
				}
				new_attr->next = next;
				if (next){
					next->prev = new_attr;
				}
				
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
		/* find the first attribute*/
		dxf_node *found_attr = dxf_find_attr2(master, group);
		if (found_attr){
			/* identify the type of attrib, according DXF group specification */
			int type = dxf_ident_attr_type(group);
			switch(type) {
				/* change the data */
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

int dxf_attr_change_i(dxf_node *master, int group, void *value, int idx){
	if (master){
		/* find the attribute, indicated by index */
		dxf_node *found_attr = dxf_find_attr_i(master, group, idx);
		if (found_attr){
			/* identify the type of attrib, according DXF group specification */
			int type = dxf_ident_attr_type(group);
			switch(type) {
				/* change the data */
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
	/* find the range of attributes of extended data, indicated by APPID */
	dxf_node *current;
	int found = 0;
	
	*start = NULL;
	*end = NULL;
	
	if(obj != NULL){ /* check if exist */
		if (obj->type == DXF_ENT){
			current = obj->obj.content->next;
			while (current){
				/* try to find the first entry, by matching the APPID */
				if (!found){
					if (current->type == DXF_ATTR){
						if(current->value.group == 1001){
							if(strcmp((char*) current->value.s_data, appid) == 0){
								found = 1; /* appid found */
								*start = current;
								*end = current;
							}
						}
					}
				}
				else{
					/* after the first entry, look by end */
					if (current->type == DXF_ATTR){
						/* breaks if is found a new APPID entry */
						if(current->value.group == 1001){
							break;
						}
						/* update the end mark */
						*end = current;
					}
					/* breaks if is found a entity */
					else break;
				}
				current = current->next;
			}
		}
	}
	return found;
}

int dxf_ext_append(dxf_node *master, char *appid, int group, void *value){
	/* appdend new attrib on extended data, indicated by APPID */
	if (master){
		if (master->type == DXF_ENT){
			dxf_node *start, *end, *prev = NULL, *next = NULL;
			/* look for appid in master */
			if(dxf_find_ext_appid(master, appid, &start, &end)){
				int type = dxf_ident_attr_type(group);
				dxf_node *new_attr = dxf_attr_new(group, type, value);
				if (new_attr){
					new_attr->master = master;
					/* append at end mark */
					prev = end;
					
					/* append new attr between prev and next nodes */
					new_attr->prev = prev;
					if (prev){
						next = prev->next;
						prev->next = new_attr;
					}
					new_attr->next = next;
					if (next){
						next->prev = new_attr;
					}
					
					if (prev == master->end){
						master->end = new_attr;
					}
					
					return 1;
				}
			}
		}
	}
	return 0;
}

void dxf_obj_transverse(dxf_node *source){
	/* it is only a a prototype of function to transverse a DXF entity */
	dxf_node *current = NULL;
	dxf_node *prev = NULL;
	
	if (source){ 
		if (source->type == DXF_ENT){
			if (source->obj.content){
				current = source->obj.content->next;
				prev = current;
			}
		}
	}

	while (current){
		if (current->type == DXF_ENT){
			
			if (current->obj.content){
				/* starts the content sweep */
				current = current->obj.content->next;
				prev = current;
				continue;
			}
		}
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			
			
		}
		
		current = current->next; /* go to the next in the list */
		/* ============================================================= */
		while (current == NULL){
			/* end of list sweeping */
			/* try to back in structure hierarchy */
			prev = prev->master;
			if (prev == source){ /* stop the search if back on initial entity */
				//printf("para\n");
				current = NULL;
				break;
			}
			if (prev){ /* up in structure */
				/* try to continue on previous point in structure */
				current = prev->next;
				
			}
			else{ /* stop the search if structure ends */
				current = NULL;
				break;
			}
		}
	}
}

dxf_node *dxf_ent_copy(dxf_node *source, int pool_dest){
	dxf_node *current = NULL;
	dxf_node *prev = NULL, *dest = NULL, *curr_dest = NULL, *new_ent = NULL;
	
	if (source){ 
		if (source->type == DXF_ENT){
			if (source->obj.content){
				current = source->obj.content->next;
				prev = current;
				
				dest = dxf_obj_new (source->obj.name);
				curr_dest = dest;
			}
		}
	}

	while ((current) && (curr_dest)){
		if (current->type == DXF_ENT){
			
			if (current->obj.content){
				
				new_ent = dxf_obj_new (current->obj.name);
				dxf_obj_append(curr_dest, new_ent);
				curr_dest = new_ent;
				
				/* starts the content sweep */
				current = current->obj.content->next;
				prev = current;
				
				continue;
			}
		}
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			if (current->value.t_data == DXF_STR){
				dxf_attr_append(curr_dest, current->value.group, current->value.s_data);
			} else if (current->value.t_data == DXF_FLOAT){
				dxf_attr_append(curr_dest, current->value.group, &current->value.d_data);
			} else if (current->value.t_data == DXF_INT){
				dxf_attr_append(curr_dest, current->value.group, &current->value.i_data);
			}
			
		}
		
		current = current->next; /* go to the next in the list */
		/* ============================================================= */
		while (current == NULL){
			/* end of list sweeping */
			/* try to back in structure hierarchy */
			prev = prev->master;
			curr_dest = curr_dest->master;
			if (prev == source){ /* stop the search if back on initial entity */
				//printf("para\n");
				current = NULL;
				break;
			}
			if (prev){ /* up in structure */
				/* try to continue on previous point in structure */
				current = prev->next;
				
			}
			else{ /* stop the search if structure ends */
				current = NULL;
				break;
			}
		}
	}
	
	return dest;
}

dxf_node * dxf_new_line (double x0, double y0, double z0,
double x1, double y1, double z1,
double thick, double elev, int color, char *layer, char *ltype, int paper){
	/* create a new DXF LINE */
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
	/* -------------------------------------------  
	ok &= dxf_attr_append(new_line, 1001, (void *) "ZECRUEL");
	ok &= dxf_attr_append(new_line, 1001, (void *) "ACAD");
	
	ok &= dxf_ext_append(new_line, "ZECRUEL", 1002, (void *) "{");
	ok &= dxf_ext_append(new_line, "ACAD", 1002, (void *) "{");
	ok &= dxf_ext_append(new_line, "ACAD", 1000, (void *) "test1");
	ok &= dxf_ext_append(new_line, "ZECRUEL", 1000, (void *) "test2");
	ok &= dxf_ext_append(new_line, "ACAD", 1002, (void *) "}");
	ok &= dxf_ext_append(new_line, "ZECRUEL", 1002, (void *) "}");
	/* -------------------------------------------  */
	if(ok){
		return new_line;
	}

	return NULL;
}

dxf_node * dxf_new_lwpolyline (double x0, double y0, double z0,
double bulge, double thick, int color, char *layer, char *ltype, int paper){
	/* create a new DXF LWPOLYLINE */
	const char *handle = "0";
	const char *dxf_class = "AcDbEntity";
	const char *dxf_subclass = "AcDbPolyline";
	int ok = 1;
	int verts = 1, flags = 0;
	dxf_node * new_poly = dxf_obj_new ("LWPOLYLINE");
	
	ok &= dxf_attr_append(new_poly, 5, (void *) handle);
	//ok &= dxf_attr_append(new_poly, 330, (void *) handle);
	ok &= dxf_attr_append(new_poly, 100, (void *) dxf_class);
	ok &= dxf_attr_append(new_poly, 67, (void *) &paper);
	ok &= dxf_attr_append(new_poly, 8, (void *) layer);
	ok &= dxf_attr_append(new_poly, 6, (void *) ltype);
	ok &= dxf_attr_append(new_poly, 62, (void *) &color);
	
	ok &= dxf_attr_append(new_poly, 100, (void *) dxf_subclass);
	ok &= dxf_attr_append(new_poly, 90, (void *) &verts);
	ok &= dxf_attr_append(new_poly, 70, (void *) &flags);
	ok &= dxf_attr_append(new_poly, 39, (void *) &thick);
	/* place the first vertice */
	ok &= dxf_attr_append(new_poly, 10, (void *) &x0);
	ok &= dxf_attr_append(new_poly, 20, (void *) &y0);
	ok &= dxf_attr_append(new_poly, 30, (void *) &z0);
	//ok &= dxf_attr_append(new_poly, 42, (void *) &bulge);
	
	if(ok){
		return new_poly;
	}

	return NULL;
}

int dxf_lwpoly_append (dxf_node * poly,
double x0, double y0, double z0, double bulge){
	/* append a vertice in a LWPOLYLINE */
	int ok = 1;
	/* look for the vertices counter attribute */
	dxf_node * verts = dxf_find_attr_i(poly, 90, 0);
	if (verts){ /* place the new vertice */
		ok &= dxf_attr_append(poly, 42, (void *) &bulge);
		ok &= dxf_attr_append(poly, 10, (void *) &x0);
		ok &= dxf_attr_append(poly, 20, (void *) &y0);
		ok &= dxf_attr_append(poly, 30, (void *) &z0);
		
		
		if (ok) verts->value.i_data++; /* increment the verts counter */
		
		return ok;
	}
	return 0;
}

int dxf_lwpoly_remove (dxf_node * poly, int idx){
	/* remove a vertice in a LWPOLYLINE, indicated by index */
	int ok = 1;
	/* look for the vertices counter attribute */
	dxf_node * verts = dxf_find_attr_i(poly, 90, 0);
	if (verts){
		/* look for the vertice attributes */
		dxf_node * x = dxf_find_attr_i(poly, 10, idx);
		dxf_node * y = dxf_find_attr_i(poly, 20, idx);
		dxf_node * z = dxf_find_attr_i(poly, 30, idx);
		dxf_node * bulge = dxf_find_attr_i(poly, 42, idx);
		
		/* detach the attribs from the list */
		ok &= dxf_obj_detach(x);
		ok &= dxf_obj_detach(y);
		ok &= dxf_obj_detach(z);
		
		if (ok) {
			dxf_obj_detach(bulge); /* bulge is optional, so the ok flag is not affected*/
			verts->value.i_data--; /* decrement the verts counter */
		}
		
		return ok;
	}
	return 0;
}

dxf_node * dxf_new_circle (double x0, double y0, double z0,
double r, double thick, int color, char *layer, char *ltype, int paper){
	/* create a new DXF CIRCLE */
	const char *handle = "0";
	const char *dxf_class = "AcDbEntity";
	const char *dxf_subclass = "AcDbCircle";
	int ok = 1;
	dxf_node * new_circle = dxf_obj_new ("CIRCLE");
	
	ok &= dxf_attr_append(new_circle, 5, (void *) handle);
	ok &= dxf_attr_append(new_circle, 100, (void *) dxf_class);
	ok &= dxf_attr_append(new_circle, 67, (void *) &paper);
	ok &= dxf_attr_append(new_circle, 8, (void *) layer);
	ok &= dxf_attr_append(new_circle, 6, (void *) ltype);
	ok &= dxf_attr_append(new_circle, 62, (void *) &color);
	
	ok &= dxf_attr_append(new_circle, 100, (void *) dxf_subclass);
	ok &= dxf_attr_append(new_circle, 39, (void *) &thick);
	/* place the first vertice */
	ok &= dxf_attr_append(new_circle, 10, (void *) &x0);
	ok &= dxf_attr_append(new_circle, 20, (void *) &y0);
	ok &= dxf_attr_append(new_circle, 30, (void *) &z0);
	ok &= dxf_attr_append(new_circle, 40, (void *) &r);
	
	if(ok){
		return new_circle;
	}

	return NULL;
}

dxf_node * dxf_new_text (double x0, double y0, double z0, double h,
char *txt, double thick, int color, char *layer, char *ltype, int paper){
	/* create a new DXF TEXT */
	const char *handle = "0";
	const char *dxf_class = "AcDbEntity";
	const char *dxf_subclass = "AcDbText";
	int ok = 1, int_zero = 0;
	double d_zero = 0.0;
	dxf_node * new_text = dxf_obj_new ("TEXT");
	
	ok &= dxf_attr_append(new_text, 5, (void *) handle);
	ok &= dxf_attr_append(new_text, 100, (void *) dxf_class);
	ok &= dxf_attr_append(new_text, 67, (void *) &paper);
	ok &= dxf_attr_append(new_text, 8, (void *) layer);
	ok &= dxf_attr_append(new_text, 6, (void *) ltype);
	ok &= dxf_attr_append(new_text, 62, (void *) &color);
	
	ok &= dxf_attr_append(new_text, 100, (void *) dxf_subclass);
	ok &= dxf_attr_append(new_text, 39, (void *) &thick);
	/* place the first vertice */
	ok &= dxf_attr_append(new_text, 10, (void *) &x0);
	ok &= dxf_attr_append(new_text, 20, (void *) &y0);
	ok &= dxf_attr_append(new_text, 30, (void *) &z0);
	ok &= dxf_attr_append(new_text, 40, (void *) &h);
	ok &= dxf_attr_append(new_text, 1, (void *) txt);
	ok &= dxf_attr_append(new_text, 50, (void *) &d_zero);
	ok &= dxf_attr_append(new_text, 72, (void *) &int_zero);
	
	ok &= dxf_attr_append(new_text, 11, (void *) &x0);
	ok &= dxf_attr_append(new_text, 21, (void *) &y0);
	ok &= dxf_attr_append(new_text, 31, (void *) &z0);
	ok &= dxf_attr_append(new_text, 100, (void *) dxf_subclass);
	ok &= dxf_attr_append(new_text, 73, (void *) &int_zero);
	
	if(ok){
		return new_text;
	}

	return NULL;
}

int dxf_edit_move2 (dxf_node * obj, double ofs_x, double ofs_y, double ofs_z){
	/* move the object relactive to offset distances */
	if (obj){
		dxf_node *current;
		int i, j;
		for (i = 0; i < 8; i++){ /* sweep in range of DXF points (10-17, 20-27, 30-37) */
			for (j = 0; current = dxf_find_attr_i(obj, 10 + i, j); j++){
				current->value.d_data += ofs_x;
			}
			for (j = 0; current = dxf_find_attr_i(obj, 20 + i, j); j++){
				current->value.d_data += ofs_y;
			}
			for (j = 0; current = dxf_find_attr_i(obj, 30 + i, j); j++){
				current->value.d_data += ofs_z;
			}
		}
		return 1;
	}
	return 0;
}

int dxf_edit_move (dxf_node * obj, double ofs_x, double ofs_y, double ofs_z){
	/* move the object and its childrens,  relactive to offset distances */
	dxf_node *current = NULL;
	dxf_node *prev = NULL;
	int ret = 0;
	
	if (obj){ 
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
				prev = current;
			}
		}
	}

	while (current){
		ret = 1;
		if (current->type == DXF_ENT){
			
			if (current->obj.content){
				/* starts the content sweep */
				current = current->obj.content->next;
				prev = current;
				continue;
			}
		}
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			if ((current->value.group >= 10) && (current->value.group < 19)){ 
				current->value.d_data += ofs_x;
			}
			if ((current->value.group >= 20) && (current->value.group < 29)){ 
				current->value.d_data += ofs_y;
			}
			if ((current->value.group >= 30) && (current->value.group < 38)){ 
				current->value.d_data += ofs_z;
			}
		}
		
		current = current->next; /* go to the next in the list */
		/* ============================================================= */
		while (current == NULL){
			/* end of list sweeping */
			/* try to back in structure hierarchy */
			prev = prev->master;
			if (prev == obj){ /* stop the search if back on initial entity */
				//printf("para\n");
				current = NULL;
				break;
			}
			if (prev){ /* up in structure */
				/* try to continue on previous point in structure */
				current = prev->next;
				
			}
			else{ /* stop the search if structure ends */
				current = NULL;
				break;
			}
		}
	}
	return ret;
}

int dxf_edit_scale (dxf_node * obj, double scale_x, double scale_y, double scale_z){
	/* move the object and its childrens,  relactive to offset distances */
	dxf_node *current = NULL;
	dxf_node *prev = NULL;
	int ret = 0;
	enum dxf_graph ent_type = DXF_NONE;
	
	if (obj){ 
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				ent_type =  dxf_ident_ent_type (obj);
				current = obj->obj.content->next;
				prev = current;
			}
		}
	}

	while (current){
		ret = 1;
		if (current->type == DXF_ENT){
			
			if (current->obj.content){
				/* starts the content sweep */
				current = current->obj.content->next;
				prev = current;
				continue;
			}
		}
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			if ((current->value.group >= 10) && (current->value.group < 19)){ 
				current->value.d_data *= scale_x;
			}
			if ((current->value.group >= 20) && (current->value.group < 29)){ 
				current->value.d_data *= scale_y;
			}
			if ((current->value.group >= 30) && (current->value.group < 38)){ 
				current->value.d_data *= scale_z;
			}
			
			switch (ent_type){
				case DXF_CIRCLE:
					if (current->value.group == 40) { 
						current->value.d_data *= scale_x;
					}
					break;
				case DXF_TEXT:
					if (current->value.group == 40) { 
						current->value.d_data *= scale_x;
					}
					break;
				default:
					break;
			}
		}
		
		current = current->next; /* go to the next in the list */
		/* ============================================================= */
		while (current == NULL){
			/* end of list sweeping */
			/* try to back in structure hierarchy */
			prev = prev->master;
			if (prev == obj){ /* stop the search if back on initial entity */
				//printf("para\n");
				current = NULL;
				break;
			}
			if (prev){ /* up in structure */
				/* try to continue on previous point in structure */
				current = prev->next;
				
			}
			else{ /* stop the search if structure ends */
				current = NULL;
				break;
			}
		}
	}
	return ret;
}

void drawing_ent_append(dxf_drawing *drawing, dxf_node *element){
	if (drawing && element){
		if ((drawing->ents != NULL) && (drawing->main_struct != NULL)){
			/* get current handle and increment the handle seed*/
			long int handle = 0;
			char hdl_str[DXF_MAX_CHARS];
			
			if (drawing->hand_seed){
				handle = strtol(drawing->hand_seed->value.s_data, NULL, 16); /* get the last handle value and convert to integer */
				snprintf(hdl_str, DXF_MAX_CHARS, "%X", handle); /* convert back to hexadecimal string, to write in element */
				snprintf(drawing->hand_seed->value.s_data, DXF_MAX_CHARS, "%X", handle + 1); /* increment value of seed and write back */
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