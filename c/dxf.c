#include "dxf.h"

void str_upp(char *str) { /* upper case the string */
	while (*str= toupper(*str)) str++;
}

char * trimwhitespace(char *str){
	/* Note: This function returns a pointer to a substring of the original string.
	If the given string was allocated dynamically, the caller must not overwrite
	that pointer with the returned value, since the original pointer must be
	deallocated using the same allocator with which it was allocated.  The return
	value must NOT be deallocated using free() etc. 
	Author = Adam Rosenfield
	http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
	*/
	char *end;

	// Trim leading space
	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)  // All spaces?
	return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

vector_p * vect_new (void){
	vector_p * new_vect = malloc(sizeof(vector_p));
	if(new_vect){
		new_vect->size = 0;
		new_vect->data = NULL;
	}
	return new_vect;
}

int stack_push (vector_p *stack, void *new_ptr){
	int size;
	void **data;
	
	if (stack){ /* check if stack exists */
		size = stack->size + 1;
		/* try to allocate more memory */
		data = realloc(stack->data, size * sizeof(void *));
		if (data){ /*success on memory allocation */
			/* change the stack */
			stack->data = data; 
			stack->size = size;
			/* store new pointer at end of stack */
			data[size-1] = new_ptr;
			return 1; /* return success */
		}
		/* if memory allocation fails, the stack is unchanged */
	}
	return 0; /* return fail */
}

void * stack_pop (vector_p *stack){
	int size;
	void **data, *last = NULL;
	
	if ((stack->size > 0) && (stack->data != NULL)){ /* check if stack is not empty*/
		size = stack->size - 1;
		data = stack->data;
		
		/* get the last element of stack */
		last = data[size];
		
		if (size == 0){ /* stack will be empty */
			free(stack->data); /* free the data */
			stack->data = NULL;
			stack->size = 0;
			return last; /* return success */
		}
		else{
			/* try to shrink memory of stack*/
			data = realloc(stack->data, size * sizeof(void *));
			if (data){ /*success on memory allocation */
				/* change the stack */
				stack->data = data; 
				stack->size = size;
				return last; /* return success */
			}
			/* if memory allocation fails, the stack is unchanged */
		}
	}
	return NULL; /* return fail */
}

void dxf_ent_clear (dxf_node *ent){ /* free the memory of list or entity */
	if (ent){
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				dxf_ent_clear(ent->obj.content); /* recursive call */
			}
			if (ent->obj.name){
				free(ent->obj.name);  /* free the string of entity's name */
				//---------------------------------------
				vec_graph_free(ent->obj.graphics);
			}
		}
		else if (ent->type == DXF_ATTR){
			if (ent->value.t_data == DXF_STR){
				if(ent->value.s_data){
					free(ent->value.s_data); /* free the string of data */
				}
			}
		}
		
		if (ent->next){
			dxf_ent_clear(ent->next); /* recursive call */
		}
		//printf("limpa %d", ent);
		free(ent); /* free the memory of structure */
	}
}

void ent_print (dxf_node *ent, int indent){ /* print the entity structure */
	int i;
	
	if (ent){
		
		if (ent->type == DXF_ENT){
			if (ent->obj.name){
				for (i=0; i<indent; i++){ /* print the indentation spaces */
					printf("    ");
				}
				
				printf(ent->obj.name);  /* print the string of entity's name */
				printf("\n");
			}
			if (ent->obj.content){
				/* recursive call and increment the indentation */
				ent_print(ent->obj.content->next, indent+1);
			}
		}
		else if (ent->type == DXF_ATTR){
			for (i=0; i<indent; i++){ /* print the indentation spaces */
				printf("    ");
			}
			printf ("%d = ", ent->value.group);
			/* print the value of atrribute, acording its type */
			switch (ent->value.t_data) {
				case DXF_STR:
					if(ent->value.s_data){
						printf(ent->value.s_data); /* printf the string of data */
					}
					break;
				case DXF_FLOAT:
					printf("%f", ent->value.d_data);
					break;
				case DXF_INT:
					printf("%d", ent->value.i_data);
			}
			printf("\n");
		}
		
		if (ent->next){
			ent_print(ent->next, indent); /* recursive call */
		}
		//printf("limpa %d", ent);
		//free(ent); //free the memory of structure
	}
}

void dxf_ent_print (dxf_node *ent, int indent){ /* print the entity structure */
	/* this function is non recursive */
	int i;
	vector_p stack;
	dxf_node *current;
	
	/* initialize the stack */
	stack.size = 0; 
	stack.data = NULL;
	
	if (ent){
		current = ent;
		while ((current != NULL) || (stack.size > 0)){
			if (current == NULL){ /* end of list sweeping */
				/* try to up in the structure hierarchy */
				current = stack_pop (&stack);
				if (current){
					current = current->next; /* go to the next in the list */
					indent--;
				}
			}
			else if (current->type == DXF_ENT){ /* DXF entity */
				/* down in the structure hierarchy */
				stack_push (&stack, current);
				if (current->obj.name){
					for (i=0; i<indent; i++){ /* print the indentation spaces */
						printf("    ");
					}
					printf(current->obj.name);  /* print the string of entity's name */
					printf("\n");
				}
				if (current->obj.content){
					/* starts the content sweep */
					current = current->obj.content->next;
					indent++;
				}
			}
			else if (current->type == DXF_ATTR){ /* DXF attibute */
				for (i=0; i<indent; i++){ /* print the indentation spaces */
					printf("    ");
				}
				printf ("%d = ", current->value.group); /* print the DFX group */
				/* print the value of atrribute, acording its type */
				switch (current->value.t_data) {
					case DXF_STR:
						if(current->value.s_data){
							printf(current->value.s_data);
						}
						break;
					case DXF_FLOAT:
						printf("%f", current->value.d_data);
						break;
					case DXF_INT:
						printf("%d", current->value.i_data);
				}
				printf("\n");
				current = current->next; /* go to the next in the list */
			}
		}
	}
}

dxf_node * dxf_obj_new (char *name){
	char *new_name = NULL;
	
	if(name){
		new_name = malloc(strlen(name)+1);  /* create new string */
		if (new_name) strcpy(new_name,name); /* and copy the name */
	}
	
	/* create a new DXF Object */
	dxf_node *new_obj = (dxf_node *) malloc(sizeof(dxf_node));
	if (new_obj){
		new_obj->master = NULL;
		new_obj->prev = NULL;
		new_obj->next = NULL;
		new_obj->type = DXF_ENT;
		new_obj->obj.name = new_name;
		new_obj->obj.graphics = NULL;
		
		/* create head of content's list */
		new_obj->obj.content = (dxf_node *) malloc(sizeof(dxf_node));
		if(new_obj->obj.content){
			new_obj->obj.content->master = new_obj;
			new_obj->obj.content->prev = NULL;
			new_obj->obj.content->next = NULL;
			new_obj->obj.content->type = DXF_ATTR;
			new_obj->obj.content->value.t_data = DXF_INT;
		}
		else { /* if error, free the memory */
			free(new_name);
			free(new_obj);
			new_obj = NULL;
		}
	}
	return new_obj;
}

dxf_node * dxf_attr_new (int group, int type, void *value){
	/* create a new DXF attribute */
	dxf_node *new_attr = (dxf_node *) malloc(sizeof(dxf_node));
	if (new_attr){
		new_attr->master = NULL;
		new_attr->prev = NULL;
		new_attr->next = NULL;
		new_attr->type = DXF_ATTR;
		
		new_attr->value.group = group;
		new_attr->value.t_data = type;
		
		switch(type) {
			case DXF_FLOAT :
				new_attr->value.d_data = *((double *)value);
				break;
			case DXF_INT :
				new_attr->value.i_data = *((int *)value);
				break;
			case DXF_STR :
				new_attr->value.s_data = malloc(strlen((char *) value)+1);  /* create new string */
				if (new_attr->value.s_data) strcpy(new_attr->value.s_data,(char *) value); /* and copy the string */
		}
	}
	return new_attr;
}

vector_p dxf_find_attr(dxf_node * obj, int attr){
	int size = 0;
	dxf_node **data = NULL;
	vector_p list;
	dxf_node *current;
	
	if(obj != NULL){ /* check if exist */
		if (obj->type == DXF_ENT){
			current = obj->obj.content->next;
			while (current){
				if (current->type == DXF_ATTR){
					if(current->value.group == attr){ /* success */
						size++;
						data = realloc(data, size * sizeof(void *));
						if (data){
							data[size-1] = current;
						}
						else{
							size = 0;
							break;
						}
					}
				}
				current = current->next;
			}
		}
	}
	list.size = size;
	list.data = data;
	return list;
}

vector_p dxf_find_obj(dxf_node * obj, char *name){
	int size = 0;
	dxf_node **data = NULL;
	vector_p list;
	dxf_node *current;
	
	if(obj != NULL){ /* check if exist */
		if (obj->type == DXF_ENT){
			current = obj->obj.content->next;
			while (current){
				if (current->type == DXF_ENT){
					if(strcmp(current->obj.name, name) == 0){ /* success */
						size++;
						data = realloc(data, size * sizeof(void *));
						if (data){
							data[size-1] = current;
						}
						else{
							size = 0;
							break;
						}
					}
				}
				current = current->next;
			}
		}
	}
	list.size = size;
	list.data = data;
	return list;
}

vector_p dxf_find_obj_descr(dxf_node * obj, char *name, char *descr){
	int size = 0;
	dxf_node **data = NULL;
	vector_p list, descr_attr;
	dxf_node *current;
	
	if(obj != NULL){ /* check if obj exist */
		if (obj->type == DXF_ENT){
			current = obj->obj.content->next;
			while (current){ /* sweep master content */
				if (current->type == DXF_ENT){ /* look for dxf entities */
					if(strcmp(current->obj.name, name) == 0){ /* match obj's name */
						descr_attr = dxf_find_attr(current, 2); /* look for descriptor in group 2 attribute */
						if (descr_attr.data){ /* found attribute */
							/* match descriptor */
							if(strcmp(((dxf_node **) descr_attr.data)[0]->value.s_data, descr) == 0){
								/* success */
								size++;
								data = realloc(data, size * sizeof(void *));
								if (data){
									data[size-1] = current;
								}
								else{
									size = 0;
									break;
								}
							}
							free (descr_attr.data);
						}
					}
				}
				current = current->next;
			}
		}
	}
	list.size = size;
	list.data = data;
	return list;
}

void dxf_layer_assemb (dxf_drawing *drawing){
	vector_p v_search;
	int i, flags;
	dxf_node *current;
	
	char name[DXF_MAX_CHARS];
	int color;
	char ltype[DXF_MAX_CHARS];
	int line_w;
	int frozen;
	int lock;
	int off;
	
	/* always set the index 0 as the default layer*/
	drawing->num_layers = 1;
	drawing->layers[0].name[0] = 0;
	drawing->layers[0].ltype[0] = 0;
	drawing->layers[0].color = 0;
	drawing->layers[0].line_w = 0;
	drawing->layers[0].frozen = 0;
	drawing->layers[0].lock = 0;
	drawing->layers[0].off = 0;
	
	v_search = dxf_find_obj(drawing->t_layer, "LAYER"); /* get the list of layers */
	if (v_search.data){
		drawing->num_layers += v_search.size;
		for (i = 0; ((i < v_search.size) && (i < DXF_MAX_LAYERS-1)); i++){
			name[0] = 0;
			color = 0;
			ltype[0] = 0;
			line_w = 0;
			frozen = 0;
			lock = 0;
			off = 0;
			
			current = ((dxf_node **) v_search.data)[i]; /* get the layer */
			/* and sweep its content */
			current = current->obj.content->next;
			while (current){
				if (current->type == DXF_ATTR){
					switch (current->value.group){
						case 2: /* layer name */
							strcpy(name, current->value.s_data);
							break;
						case 6: /* layer line type name */
							strcpy(ltype, current->value.s_data);
							break;
						case 62: /* layer color */
							color = current->value.i_data;
							if (color < 0) {
								off = 1;
								color = abs(color);
							}
							break;
						case 70: /* flags */
							flags = current->value.i_data;
							if (flags & 1) frozen = 1;
							if (flags & 4) lock = 1;
					}
				}
				current = current->next;
			}
			/* set the variables on the current layer in drawing structure */
			strcpy(drawing->layers[i+1].name, name);
			strcpy(drawing->layers[i+1].ltype, ltype);
			drawing->layers[i+1].color = color;
			drawing->layers[i+1].line_w = line_w;
			drawing->layers[i+1].frozen = frozen;
			drawing->layers[i+1].lock = lock;
			drawing->layers[i+1].off = off;
		}
		free(v_search.data);
	}
}

void dxf_ltype_assemb (dxf_drawing *drawing){
	vector_p v_search;
	int i, pat_idx;
	dxf_node *current;
	
	char name[DXF_MAX_CHARS];
	int size;
	double pat[DXF_MAX_PAT];
	double length;
	
	/* always set the index 0 as the default ltype*/
	drawing->num_ltypes = 1;
	drawing->ltypes[0].name[0] = 0;
	drawing->ltypes[0].size = 1;
	drawing->ltypes[0].pat[0] = 0;
	drawing->ltypes[0].length = 0;
	
	v_search = dxf_find_obj(drawing->t_ltype, "LTYPE"); /* get the list of ltypes */
	if (v_search.data){
		drawing->num_ltypes += v_search.size;
		for (i = 0; ((i < v_search.size) && (i < DXF_MAX_LTYPES-1)); i++){
			name[0] = 0;
			size = 0;
			pat[0] = 0;
			pat_idx = 0;
			length = 0;
			
			current = ((dxf_node **) v_search.data)[i]; /* get the ltype */
			/* and sweep its content */
			current = current->obj.content->next;
			while (current){
				if (current->type == DXF_ATTR){
					switch (current->value.group){
						case 2: /* ltype name */
							strcpy(name, current->value.s_data);
							break;
						case 40: /* pattern length */
							length = current->value.d_data;
							break;
						case 49: /* pattern element */
							if (pat_idx < DXF_MAX_PAT) {
								pat[pat_idx] = current->value.d_data;
								pat_idx++;
							}
							break;
						case 73: /* num of pattern elements */
							size = current->value.i_data;
							if (size > DXF_MAX_PAT) {
								size < DXF_MAX_PAT;}
					}
				}
				current = current->next;
			}
			/* set the variables on the current ltype in drawing structure */
			strcpy(drawing->ltypes[i+1].name, name);
			memcpy(drawing->ltypes[i+1].pat, pat, size * sizeof(double));
			drawing->ltypes[i+1].size = size;
			drawing->ltypes[i+1].length = length;
		}
		free(v_search.data);
	}
}

int dxf_lay_idx (dxf_drawing drawing, char *name){
	int i;
	
	for (i=1; i < drawing.num_layers; i++){
		if (strcmp(drawing.layers[i].name, name) == 0){
			return i;
		}
	}
	
	return 0; /*if search fails, return the standard layer */
}

int dxf_ltype_idx (dxf_drawing drawing, char *name){
	int i;
	
	for (i=1; i < drawing.num_ltypes; i++){
		if (strcmp(drawing.ltypes[i].name, name) == 0){
			return i;
		}
	}
	
	return 0; /*if search fails, return the standard layer */
}

dxf_drawing dxf_open (char *path){
	dxf_drawing drawing;
	char buf[255], *line;
	FILE *file;
	long f_index = 0;  /*  indexes the file�s lines */
	dxf_node *new_node = NULL;
	dxf_node *master, *prev, *next, *tmp, *last_obj;
	dxf_node *main_struct = NULL;
	int group = 0; /* current group */
	int t_data = DXF_STR; /* current type of data */
	double d_data;
	int i_data;
	vector_p part;

	/* init the drawing */
	drawing.head = NULL;
	drawing.tabs = NULL;
	drawing.blks = NULL;
	drawing.ents = NULL; 
	drawing.t_ltype = NULL;
	drawing.t_layer = NULL;
	drawing.t_style = NULL;
	drawing.t_view = NULL;
	drawing.t_ucs = NULL;
	drawing.t_vport = NULL;
	drawing.t_dimst = NULL;
	drawing.t_appid = NULL;
	drawing.main_struct = NULL;
	
	/* create a new main_struct */
	main_struct = dxf_obj_new(NULL);
	
	file = fopen(path, "r");
	if(file == NULL){
		free(main_struct);
		main_struct = NULL;
	}
	else if (main_struct){
		
		master = main_struct; /* current master is the main struct */
		last_obj = main_struct;
		prev = main_struct->obj.content; /* the list starts on main_struct's content */
		next = NULL; /* end of list (empty list) */
		
		while((fgets(buf, sizeof(buf), file))!=NULL ){
			
			/* trim leading/trailing whitespace of line */
			line = trimwhitespace(buf);
			
			if((f_index % 2) == 0){ /* check if the line is even */
				/* in DXF files, the even lines identify the groups */
				group = atoi(line);
				
				/* Identifies the data type of the next line,  */
				/* according to the value of the group. (DXF ranges) */
				
				if ((group >= 0) && (group < 10)){
					t_data = DXF_STR; /* string */
				}
				else if( (group >= 10) && (group < 60)){
					t_data = DXF_FLOAT; /* float */
				}
				else if( (group >= 60) && (group < 80)){
					t_data = DXF_INT; /* integer */
				}
				else if( (group >= 140) && (group < 148)){
					t_data = DXF_FLOAT; /* float */
				}
				else if( (group >= 170) && (group < 176)){
					t_data = DXF_INT; /* integer */
				}
				else if( (group >= 210) && (group < 240)){
					t_data = DXF_FLOAT; /*  float */
				}
				else if( (group >= 999) && (group < 1010)){
					t_data = DXF_STR; /* string */
				}
				else if( (group >= 1010) && (group < 1060)){
					t_data = DXF_FLOAT; /* float */
				}
				else if( (group >= 1060) && (group < 1080)){
					t_data = DXF_INT; /* integer */
				}
				else{ /* not defined type */
					t_data = DXF_STR; /* default type is string */
				}
				
			}
			else {
				/* strips \n of line string */
				//line[strcspn(line, "\r\n")] = 0; /* strips \n or \r of line string */
				
				switch(group){
					case 0:
						str_upp(line); /* upper case the line */
					
						/* Find the end of the master's content list */
						tmp = master->obj.content;
						while(tmp->next != NULL)
							tmp = tmp->next;
						prev = tmp; /* new objs will append here */
						
						/* new level of hierarchy  */
						if((strcmp(line, "SECTION") == 0) ||
							(strcmp(line, "TABLE") == 0) ||
							(strcmp(line, "BLOCK") == 0)){
							new_node = dxf_obj_new (line); /* new object */
							if (new_node){
								/*  append new to master's list */
								new_node->master = master;
								new_node->prev = prev;
								if (prev){
									prev->next = new_node;
								}
								new_node->next = NULL; /* at end of list */
								last_obj = new_node;
								
								/* the new becomes the master */
								master = new_node;
								prev = new_node->obj.content; /* new objs will append here */
							}
						}
						/* back to the previous level on hierarchy */
						else if((strcmp(line, "ENDSEC") == 0) ||
							(strcmp(line, "ENDTAB") == 0) ||
							(strcmp(line, "ENDBLK") == 0)||
							(strcmp(line, "SEQEND") == 0)){
							if(master->master){
								/* back to master's master ;) */
								master = master->master;
								last_obj = master;
								/* Find the end of the master's content list */
								tmp = master->obj.content;
								while(tmp->next != NULL)
									tmp = tmp->next;
								prev = tmp; /* new items will append here */
							}
						}
						
						/*  new ordinary DXF object */
						else {
							new_node = dxf_obj_new (line); /* new object */
							if (new_node){
								/*  append new to master's list */
								new_node->master = master;
								new_node->prev = prev;
								if (prev){
									prev->next = new_node;
								}
								new_node->next = NULL; /* append at end of list */
								prev = new_node->obj.content; /* new attrs will append here */
								last_obj = new_node;
							}
						}
						//new_node = NULL;
						break;
						
					case 66: /* Entities follow flag */
						i_data = atoi(line);
						new_node = dxf_attr_new (group, t_data, (void *) &i_data);
						if (new_node){
							/*  append new to last obj's list */
							new_node->master = last_obj;
							new_node->prev = prev;
							if (prev){
								prev->next = new_node;
							}
							new_node->next = NULL; /* append at end of list */
						}
					
						if (i_data != 0){
							/* new level of hierarchy */
							master = last_obj; /*  the last obj becomes the master */
							
							/* Find the end of the master's content list */
							tmp = master->obj.content;
							while(tmp->next != NULL)
								tmp = tmp->next;
							prev = tmp; /* new objs will append here */
						}
						break;
					default:
						switch(t_data) {
							case DXF_FLOAT :
								d_data = atof(line);
								new_node = dxf_attr_new (group, t_data, (void *) &d_data);
								break;
							case DXF_INT :
								i_data = atoi(line);
								new_node = dxf_attr_new (group, t_data, (void *) &i_data);
								break;
							case DXF_STR :
								new_node = dxf_attr_new (group, t_data, (void *) line);
						}
						
						if (new_node){
							/*  append new to last obj's list */
							new_node->master = last_obj;
							new_node->prev = prev;
							if (prev){
								prev->next = new_node;
							}
							new_node->next = NULL; /* append at end of list */
							prev = new_node;
						}
					
				}
				new_node = NULL;
			}
			
			f_index++; /* next line index */
		}
		
	}
	fclose(file);
	
	drawing.main_struct = main_struct;
	
	/* disassembly the drawing structure */
	/* the main sections */
	part = dxf_find_obj_descr(main_struct, "SECTION", "HEADER");
	if (part.data){
		drawing.head = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(main_struct, "SECTION", "TABLES");
	if (part.data){
		drawing.tabs = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(main_struct, "SECTION", "BLOCKS");
	if (part.data){
		drawing.blks = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(main_struct, "SECTION", "ENTITIES");
	if (part.data){
		drawing.ents = ((dxf_node **) part.data)[0]; 
		free(part.data);
	}
	
	/* the tables */
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "LTYPE");
	if (part.data){
		drawing.t_ltype = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "LAYER");
	if (part.data){
		drawing.t_layer = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "STYLE");
	if (part.data){
		drawing.t_style = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "VIEW");
	if (part.data){
		drawing.t_view = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "UCS");
	if (part.data){
		drawing.t_ucs = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "VPORT");
	if (part.data){
		drawing.t_vport = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "DIMSTYLE");
	if (part.data){
		drawing.t_dimst = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	part = dxf_find_obj_descr(drawing.tabs, "TABLE", "APPID");
	if (part.data){
		drawing.t_appid = ((dxf_node **) part.data)[0];
		free(part.data);
	}
	
	/* assemble the layers list */
	dxf_layer_assemb (&drawing);
	
	/* assemble the ltypes list */
	dxf_ltype_assemb (&drawing);
	
	return drawing;
}

int dxf_save (char *path, dxf_drawing drawing){
	FILE *file;
	vector_p stack, attr;
	dxf_node *current;
	int ret_success;
	
	/* initialize the stack */
	stack.size = 0;
	stack.data = NULL;
	
	ret_success = 0;
	file = fopen(path, "w"); /* open the file */
	
	
	if ((file != NULL) && (drawing.main_struct != NULL)){
		current = drawing.main_struct->obj.content->next;
		while ((current != NULL) || (stack.size > 0)){
			if (current == NULL){ /* end of list sweeping */
				/* try to up in the structure hierarchy */
				current = stack_pop (&stack);
				if (current){
					/* write the end of complex entities, acording its type */
					if(strcmp(current->obj.name, "SECTION") == 0){
						fprintf(file, "0\nENDSEC\n");
					}
					else if(strcmp(current->obj.name, "TABLE") == 0){
						fprintf(file, "0\nENDTAB\n");
					}
					else if(strcmp(current->obj.name, "BLOCK") == 0){
						fprintf(file, "0\nENDBLK\n");
					}
					else{
						attr = dxf_find_attr(current, 66); /* look for entities folow flag*/
						if (attr.data){ /* if flag is found and */
							if(((dxf_node **) attr.data)[0]->value.i_data != 0){ /* its value is non zero */
								fprintf(file, "0\nSEQEND\n");
							}
							free(attr.data);
						}
					}
					current = current->next; /* go to the next in the list */
				}
			}
			else if (current->type == DXF_ENT){ /* DXF entity */
				/* down in the structure hierarchy */
				stack_push (&stack, current);
				if (current->obj.name){
					fprintf(file, "0\n%s\n", current->obj.name); /* write the start of entity */
				}
				if (current->obj.content){
					/* starts the content sweep */
					current = current->obj.content->next;
				}
			}
			else if (current->type == DXF_ATTR){ /* DXF attribute */
				fprintf(file, "%d\n",  current->value.group); /* write the DFX group */
				/* write the value of attribute, acording its type */
				switch (current->value.t_data) {
					case DXF_STR:
						fprintf(file, "%s\n", current->value.s_data);
						break;
					case DXF_FLOAT:
						fprintf(file, "%f\n", current->value.d_data);
						break;
					case DXF_INT:
						fprintf(file, "%d\n",  current->value.i_data);
				}
				current = current->next; /* go to the next in the list */
			}
		}
		ret_success = 1; /* return success */
	}
	
	fclose(file);
	
	return ret_success;
}