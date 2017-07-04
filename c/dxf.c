#include "dxf.h"
#include <math.h>
#include "shape2.h"
#define DXF_NUM_POOL 5
#define DXF_PAGE 500000

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

char *str_replace(char *orig, char *rep, char *with) {
	/*
	Author: jmucchiello
	http://stackoverflow.com/questions/779875/what-is-the-function-to-replace-string-in-c
	*/
	/* You must free the result if result is non-NULL. */
	char *result; /* the return string */
	char *ins;    /* the next insert point */
	char *tmp;    /* varies */
	int len_rep;  /* length of rep (the string to remove) */
	int len_with; /* length of with (the string to replace rep with) */
	int len_front; /* distance between rep and end of last rep */
	int count;    /* number of replacements */

	/* sanity checks and initialization */
	if (!orig || !rep)
		return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0)
		return NULL; /* empty rep causes infinite loop during count */
	if (!with)
		with = "";
	len_with = strlen(with);

	/* count the number of replacements needed */
	ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count) {
		ins = tmp + len_rep;
	}

	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if (!result)
		return NULL;

	/*
	first time through the loop, all the variable are set correctly
	from here on, tmp points to the end of the result string
	ins points to the next occurrence of rep in orig
	orig points to the remainder of orig after "end of rep"
	*/
	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; /* move to next "end of rep" */
	}
	strcpy(tmp, orig);
	return result;
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

void * dxf_mem_pool(enum dxf_pool_action action, int idx){
	
	static dxf_pool_slot mem_pool[DXF_NUM_POOL], line[DXF_NUM_POOL];
	int i;
	
	void *ret_ptr = NULL;
	
	if ((idx >= 0) && (idx < DXF_NUM_POOL)){ /* check if index is valid */
		
		/* initialize the pool, the first allocation */
		if (mem_pool[idx].size < 1){
			mem_pool[idx].pool[0] = malloc(DXF_PAGE * sizeof(dxf_node));
			if (mem_pool[idx].pool){
				mem_pool[idx].size = 1;
				//printf("Init mem_pool\n");
			}
		}
		
		/* if current page is full */
		if ((mem_pool[idx].pos >= DXF_PAGE) && (mem_pool[idx].size > 0)){
			/* try to change to page previuosly allocated */
			if (mem_pool[idx].page < mem_pool[idx].size - 1){
				mem_pool[idx].page++;
				mem_pool[idx].pos = 0;
				//printf("change mem_pool page\n");
			}
			/* or then allocatte a new page */
			else if(mem_pool[idx].page < DXF_POOL_PAGES-1){
				mem_pool[idx].pool[mem_pool[idx].page + 1] = malloc(DXF_PAGE * sizeof(dxf_node));
				if (mem_pool[idx].pool[mem_pool[idx].page + 1]){
					mem_pool[idx].page++;
					mem_pool[idx].size ++;
					mem_pool[idx].pos = 0;
					//printf("Realloc mem_pool\n");
				}
			}
		}
		
		ret_ptr = NULL;
		
		if ((mem_pool[idx].pool[mem_pool[idx].page] != NULL)){
			switch (action){
				case ADD_DXF:
					if (mem_pool[idx].pos < DXF_PAGE){
						ret_ptr = &(((dxf_node *)mem_pool[idx].pool[mem_pool[idx].page])[mem_pool[idx].pos]);
						mem_pool[idx].pos++;
					}
					break;
				case ZERO_DXF:
					mem_pool[idx].pos = 0;
					mem_pool[idx].page = 0;
					break;
				case FREE_DXF:
					for (i = 0; i < mem_pool[idx].size; i++){
						//free(mem_pool[idx].pool[i]);
						dxf_ent_clear (mem_pool[idx].pool[i]);
						mem_pool[idx].pool[i] = NULL;
					}
					mem_pool[idx].pos = 0;
					mem_pool[idx].page = 0;
					mem_pool[idx].size = 0;
					break;
			}
		}
	}
	return ret_ptr;
}


void dxf_ent_clear (dxf_node *ent){ /* free the memory of list or entity */
	if (ent){
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				//dxf_ent_clear(ent->obj.content); /* recursive call */
			}
			if (ent->obj.name){
				//free(ent->obj.name);  /* free the string of entity's name */
				//---------------------------------------
				//vec_graph_free(ent->obj.graphics);
				//graph_mem_pool(ZERO_GRAPH);
				//graph_mem_pool(ZERO_LINE);
			}
		}
		else if (ent->type == DXF_ATTR){
			if (ent->value.t_data == DXF_STR){
				if(ent->value.s_data){
					//free(ent->value.s_data); /* free the string of data */
				}
			}
		}
		
		if (ent->next){
			//dxf_ent_clear(ent->next); /* recursive call */
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

void dxf_ent_print2 (dxf_node *ent){ /* print the entity structure */
	/* this function is non recursive */
	int i;
	int indent = 0;
	dxf_node *current, *prev;
	
	current = ent;
	while (current){
		prev = current;
		if (current->type == DXF_ENT){ /* DXF entity */
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
		while (current == NULL){
			if (prev == ent){
				current = NULL;
				break;
			}
			prev = prev->master;
			if (prev){
				current = prev->next;
				indent --;
				if (prev == ent){
					current = NULL;
					printf("fim loop ");
					break;
				}
			}
			else{
				current = NULL;
				break;
			}
		}
	}
}

void dxf_ent_print_f (dxf_node *ent, char *path){ /* print the entity structure on file*/
	/* this function is non recursive */
	int i;
	int indent = 0;
	dxf_node *current, *prev;
	
	FILE *file;
	file = fopen(path, "w"); /* open the file */
	
	current = ent;
	while ((current != NULL) && (file != NULL)){
		prev = current;
		if (current->type == DXF_ENT){ /* DXF entity */
			if (current->obj.name){
				for (i=0; i<indent; i++){ /* print the indentation spaces */
					fprintf(file, "    ");
				}
				fprintf(file, current->obj.name);  /* print the string of entity's name */
				fprintf(file, "\n");
			}
			if (current->obj.content){
				/* starts the content sweep */
				current = current->obj.content->next;
				indent++;
			}
		}
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			for (i=0; i<indent; i++){ /* print the indentation spaces */
				fprintf(file, "    ");
			}
			fprintf(file, "%d = ", current->value.group); /* print the DFX group */
			/* print the value of atrribute, acording its type */
			switch (current->value.t_data) {
				case DXF_STR:
					if(current->value.s_data){
						fprintf(file, current->value.s_data);
					}
					break;
				case DXF_FLOAT:
					fprintf(file, "%f", current->value.d_data);
					break;
				case DXF_INT:
					fprintf(file, "%d", current->value.i_data);
			}
			fprintf(file, "\n");
			current = current->next; /* go to the next in the list */
		}
		while (current == NULL){
			if (prev == ent){
				current = NULL;
				break;
			}
			prev = prev->master;
			if (prev){
				current = prev->next;
				indent --;
				if (prev == ent){
					current = NULL;
					fprintf(file, "fim loop ");
					break;
				}
			}
			else{
				current = NULL;
				break;
			}
		}
	}
	fclose(file);
}

dxf_node * dxf_obj_new (char *name){
	//char *new_name = NULL;
	
	/* create a new DXF Object */
	//dxf_node *new_obj = (dxf_node *) malloc(sizeof(dxf_node));
	//dxf_mem_pool(enum dxf_pool_action action, int idx)
	dxf_node *new_obj = dxf_mem_pool(ADD_DXF, 0);
	if (new_obj){
		new_obj->obj.name[0] = 0;
		if(name){
			//new_name = malloc(strlen(name)+1);  /* create new string */
			strncpy(new_obj->obj.name, name, DXF_MAX_CHARS); /* and copy the name */
		}
		new_obj->master = NULL;
		new_obj->prev = NULL;
		new_obj->next = NULL;
		new_obj->type = DXF_ENT;
		//new_obj->obj.name = new_name;
		new_obj->obj.graphics = NULL;
		
		/* create head of content's list */
		//new_obj->obj.content = (dxf_node *) malloc(sizeof(dxf_node));
		new_obj->obj.content = (dxf_node *) dxf_mem_pool(ADD_DXF, 0);
		if(new_obj->obj.content){
			new_obj->end = new_obj->obj.content;
			new_obj->obj.content->master = new_obj;
			new_obj->obj.content->prev = NULL;
			new_obj->obj.content->next = NULL;
			new_obj->obj.content->type = DXF_ATTR;
			new_obj->obj.content->value.t_data = DXF_INT;
		}
		else { /* if error, free the memory */
			//free(new_name);
			//free(new_obj);
			new_obj = NULL;
		}
	}
	return new_obj;
}

dxf_node * dxf_attr_new (int group, int type, void *value){
	/* create a new DXF attribute */
	//dxf_node *new_attr = (dxf_node *) malloc(sizeof(dxf_node));
	dxf_node *new_attr = (dxf_node *) dxf_mem_pool(ADD_DXF, 0);
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
				//new_attr->value.s_data = malloc(strlen((char *) value)+1);  /* create new string */
				new_attr->value.s_data[0] = 0;
				strncpy(new_attr->value.s_data,(char *) value, DXF_MAX_CHARS); /* and copy the string */
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
		//printf("Num Layers = %d\n", drawing->num_layers);
		free(v_search.data);
	}
}

void dxf_ltype_assemb (dxf_drawing *drawing){
	vector_p v_search;
	int i, j, pat_idx;
	dxf_node *current;
	
	char name[DXF_MAX_CHARS], descr[DXF_MAX_CHARS];
	int size;
	double pat[DXF_MAX_PAT];
	double length, max;
	
	/* always set the index 0 as the default ltype*/
	drawing->num_ltypes = 1;
	drawing->ltypes[0].name[0] = 0;
	drawing->ltypes[0].descr[0] = 0;
	drawing->ltypes[0].size = 1;
	drawing->ltypes[0].pat[0] = 0;
	drawing->ltypes[0].length = 0;
	
	v_search = dxf_find_obj(drawing->t_ltype, "LTYPE"); /* get the list of ltypes */
	if (v_search.data){
		drawing->num_ltypes += v_search.size;
		for (i = 0; ((i < v_search.size) && (i < DXF_MAX_LTYPES-1)); i++){
			name[0] = 0;
			descr[0] = 0;
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
						case 3: /* ltype descriptive text */
							strcpy(descr, current->value.s_data);
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
			
			/* adjust pattern to pixel units */
			/* first, find the max patt length*/
			max = 0.0;
			for(j = 0; j < size; j++){
				if (max < fabs(pat[j])){
					max = fabs(pat[j]);
				}
			}
			if (max == 0.0) max = 1.0;
			/* then normalize each value in pattern */
			for(j = 0; j < size; j++){
				pat[j] = pat[j]/max;
			}
			
			/* set the variables on the current ltype in drawing structure */
			strcpy(drawing->ltypes[i+1].name, name);
			strcpy(drawing->ltypes[i+1].descr, descr);
			memcpy(drawing->ltypes[i+1].pat, pat, size * sizeof(double));
			drawing->ltypes[i+1].size = size;
			drawing->ltypes[i+1].length = length;
		}
		free(v_search.data);
	}
}

void dxf_fonts_assemb (dxf_drawing *drawing){
	vector_p v_search;
	int i, flags;
	dxf_node *current;
	
	char name[DXF_MAX_CHARS];
	char file_name[DXF_MAX_CHARS];
	
	drawing->num_fonts = 0;
	
	/* open default font */
	shape *shx_font = shx_font_open("txt.shx");
	
	if (shx_font){
		/* always set the index 0 as the default font*/
		drawing->num_fonts = 1;
		drawing->text_fonts[0].name[0] = 0;
		drawing->text_fonts[0].shx_font = shx_font;
	}
	
	v_search = dxf_find_obj(drawing->t_style, "STYLE"); /* get the list of fonts */
	if (v_search.data){
		drawing->num_fonts += v_search.size;
		for (i = 0; ((i < v_search.size) && (i < DXF_MAX_FONTS-1)); i++){
			name[0] = 0;
			file_name[0] = 0;
			
			current = ((dxf_node **) v_search.data)[i]; /* get the font */
			/* and sweep its content */
			current = current->obj.content->next;
			while (current){
				if (current->type == DXF_ATTR){
					switch (current->value.group){
						case 2: /* font name */
							strcpy(name, current->value.s_data);
							break;
						case 3: /* file name */
							strcpy(file_name, current->value.s_data);
							break;
						case 70: /* flags */
							flags = current->value.i_data;
					}
				}
				current = current->next;
			}
			/* set the variables on the current layer in drawing structure */
			strcpy(drawing->text_fonts[i+1].name, name);
			shx_font = shx_font_open(file_name);
			drawing->text_fonts[i+1].shx_font = shx_font;
		}
		free(v_search.data);
	}
}

int dxf_lay_idx (dxf_drawing *drawing, char *name){
	int i;
	if (drawing){
		for (i=1; i < drawing->num_layers; i++){
			if (strcmp(drawing->layers[i].name, name) == 0){
				return i;
			}
		}
	}
	
	return 0; /*if search fails, return the standard layer */
}

int dxf_ltype_idx (dxf_drawing *drawing, char *name){
	int i;
	if (drawing){
		for (i=1; i < drawing->num_ltypes; i++){
			if (strcmp(drawing->ltypes[i].name, name) == 0){
				return i;
			}
		}
	}
	
	return 0; /*if search fails, return the standard layer */
}

int dxf_font_idx (dxf_drawing *drawing, char *name){
	int i;
	if (drawing){
		for (i=1; i < drawing->num_fonts; i++){
			if (strcmp(drawing->text_fonts[i].name, name) == 0){
				return i;
			}
		}
	}
	
	return 0; /*if search fails, return the standard font */
}

void dxf_open (dxf_drawing *drawing, char *path){
	/*deprecated*/
	if (drawing){
		char *buf, *line, *cur_line, *next_line;
		FILE *file;
		long f_index = 0;  /*  indexes the file´s lines */
		dxf_node *new_node = NULL;
		dxf_node *master, *prev, *next, *tmp, *last_obj;
		dxf_node *main_struct = NULL;
		int group = 0; /* current group */
		int t_data = DXF_STR; /* current type of data */
		double d_data;
		int i_data;
		vector_p part;

		/* init the drawing */
		drawing->head = NULL;
		drawing->tabs = NULL;
		drawing->blks = NULL;
		drawing->ents = NULL; 
		drawing->t_ltype = NULL;
		drawing->t_layer = NULL;
		drawing->t_style = NULL;
		drawing->t_view = NULL;
		drawing->t_ucs = NULL;
		drawing->t_vport = NULL;
		drawing->t_dimst = NULL;
		drawing->t_appid = NULL;
		drawing->main_struct = NULL;
		
		/* create a new main_struct */
		main_struct = dxf_obj_new(NULL);
		
		file = fopen(path, "rb");
		if(file == NULL){
			//free(main_struct);
			main_struct = NULL;
		}
		else if (main_struct){
			
			fseek(file, 0, SEEK_END);
			long fsize = ftell(file); /* size of file*/
			fseek(file, 0, SEEK_SET);  //same as rewind(f);
			printf("file size = %d\n", fsize);
			
			char *buf = malloc(fsize + 1);
			fread(buf, fsize, 1, file);
			fclose(file);
			file = NULL;
			buf[fsize] = 0;
			
			cur_line = buf;
			
			master = main_struct; /* current master is the main struct */
			last_obj = main_struct;
			prev = main_struct->obj.content; /* the list starts on main_struct's content */
			next = NULL; /* end of list (empty list) */
			
			while(cur_line){
				next_line = strchr(cur_line, '\n');
				if (next_line) *next_line = '\0';  /*terminate the current line*/
				
				/* trim leading/trailing whitespace of line */
				line = trimwhitespace(cur_line);
				
				
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
				cur_line = next_line ? (next_line+1) : NULL;
			}
			free(buf);
			buf = NULL;
		}
		if (file){
			fclose(file);
			file = NULL;
		}
		
		drawing->main_struct = main_struct;
		
		/* disassembly the drawing structure */
		/* the main sections */
		part = dxf_find_obj_descr(main_struct, "SECTION", "HEADER");
		if (part.data){
			drawing->head = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "TABLES");
		if (part.data){
			drawing->tabs = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "BLOCKS");
		if (part.data){
			drawing->blks = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "ENTITIES");
		if (part.data){
			drawing->ents = ((dxf_node **) part.data)[0]; 
			free(part.data);
		}
		
		/* the tables */
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "LTYPE");
		if (part.data){
			drawing->t_ltype = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "LAYER");
		if (part.data){
			drawing->t_layer = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "STYLE");
		if (part.data){
			drawing->t_style = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "VIEW");
		if (part.data){
			drawing->t_view = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "UCS");
		if (part.data){
			drawing->t_ucs = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "VPORT");
		if (part.data){
			drawing->t_vport = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "DIMSTYLE");
		if (part.data){
			drawing->t_dimst = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "APPID");
		if (part.data){
			drawing->t_appid = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		
		/* assemble the layers list */
		dxf_layer_assemb (drawing);
		
		/* assemble the ltypes list */
		dxf_ltype_assemb (drawing);
		
		/* assemble the fonts list */
		dxf_fonts_assemb(drawing);
		
		//return drawing;
	}
}

int dxf_save (char *path, dxf_drawing *drawing){
	
	FILE *file;
	vector_p stack, attr;
	dxf_node *current;
	int ret_success;
	
	/* initialize the stack */
	stack.size = 0;
	stack.data = NULL;
	
	ret_success = 0;
	if (drawing){
		file = fopen(path, "w"); /* open the file */
		
		
		if ((file != NULL) && (drawing->main_struct != NULL)){
			current = drawing->main_struct->obj.content->next;
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
	}
	return ret_success;
}

int dxf_open2 (dxf_drawing *drawing, char *path, int *prog){
	static enum {NONE, INIT, READ, FINISH} state = INIT;
	static char *buf, *line, *cur_line, *next_line;
	static dxf_node *main_struct = NULL;
	static dxf_node *master, *prev, *next, *tmp, *last_obj;
	
	static FILE *file;
	static long fsize, f_index = 0;  /*  indexes the file´s lines */

	static dxf_node *new_node = NULL;
	
	
	static int group = 0; /* current group */
	static int t_data = DXF_STR; /* current type of data */
	static double d_data;
	static int i_data;
	static vector_p part;
	
	if (state == INIT){
		if (!drawing){
			goto quit_error;
		}

		/* init the drawing */
		drawing->head = NULL;
		drawing->tabs = NULL;
		drawing->blks = NULL;
		drawing->ents = NULL; 
		drawing->t_ltype = NULL;
		drawing->t_layer = NULL;
		drawing->t_style = NULL;
		drawing->t_view = NULL;
		drawing->t_ucs = NULL;
		drawing->t_vport = NULL;
		drawing->t_dimst = NULL;
		drawing->t_appid = NULL;
		drawing->main_struct = NULL;
		
		/* create a new main_struct */
		main_struct = dxf_obj_new(NULL);
		if (!main_struct){
			goto quit_error;
		}
			
		file = fopen(path, "rb");
		if(file == NULL){
			main_struct = NULL;
			goto quit_error;
		}
		
		fseek(file, 0, SEEK_END);
		fsize = ftell(file); /* size of file*/
		fseek(file, 0, SEEK_SET);  //same as rewind(f);
		printf("file size = %d\n", fsize);
		
		buf = malloc(fsize + 1);
		if (!buf){
			goto quit_error;
		}
		fread(buf, fsize, 1, file);
		fclose(file);
		file = NULL;
		buf[fsize] = 0;
		
		cur_line = buf;
		
		master = main_struct; /* current master is the main struct */
		last_obj = main_struct;
		prev = main_struct->obj.content; /* the list starts on main_struct's content */
		next = NULL; /* end of list (empty list) */
		f_index = 0;
		state = READ;
	}
	
	if (state == READ){
		int i = 0;
		while((i++ < 10001) && (state == READ)){
			next_line = strchr(cur_line, '\n');
			if (next_line) *next_line = '\0';  /*terminate the current line*/
			
			/* trim leading/trailing whitespace of line */
			line = trimwhitespace(cur_line);
			
			
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
						//tmp = master->obj.content;
						//while(tmp->next != NULL)
						//	tmp = tmp->next;
						//prev = tmp; /* new objs will append here */
						prev = master->end;
					
					
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
								master->end = new_node;
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
								//tmp = master->obj.content;
								//while(tmp->next != NULL)
								//	tmp = tmp->next;
								//prev = tmp; /* new items will append here */
								prev = master->end;
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
								master->end = new_node;
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
							last_obj->end = new_node;
						}
					
						if (i_data != 0){
							/* new level of hierarchy */
							master = last_obj; /*  the last obj becomes the master */
							
							/* Find the end of the master's content list */
							//tmp = master->obj.content;
							//while(tmp->next != NULL)
							//	tmp = tmp->next;
							//prev = tmp; /* new objs will append here */
							prev = master->end;
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
							last_obj->end = new_node;
							prev = new_node;
						}
					
				}
				new_node = NULL;
			}
			
			f_index++; /* next line index */
			cur_line = next_line ? (next_line+1) : NULL;
			if(cur_line == NULL){
				free(buf);
				buf = NULL;
				state = FINISH;
			}
		}
		if (cur_line){
			*prog = (int)((double)(cur_line - buf)*100/fsize);
		}
		return 1;
	}
	if (state == FINISH){
			
		drawing->main_struct = main_struct;
		
		/* disassembly the drawing structure */
		/* the main sections */
		part = dxf_find_obj_descr(main_struct, "SECTION", "HEADER");
		if (part.data){
			drawing->head = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "TABLES");
		if (part.data){
			drawing->tabs = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "BLOCKS");
		if (part.data){
			drawing->blks = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(main_struct, "SECTION", "ENTITIES");
		if (part.data){
			drawing->ents = ((dxf_node **) part.data)[0]; 
			free(part.data);
		}
		
		/* the tables */
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "LTYPE");
		if (part.data){
			drawing->t_ltype = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "LAYER");
		if (part.data){
			drawing->t_layer = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "STYLE");
		if (part.data){
			drawing->t_style = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "VIEW");
		if (part.data){
			drawing->t_view = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "UCS");
		if (part.data){
			drawing->t_ucs = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "VPORT");
		if (part.data){
			drawing->t_vport = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "DIMSTYLE");
		if (part.data){
			drawing->t_dimst = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		part = dxf_find_obj_descr(drawing->tabs, "TABLE", "APPID");
		if (part.data){
			drawing->t_appid = ((dxf_node **) part.data)[0];
			free(part.data);
		}
		
		/* assemble the layers list */
		dxf_layer_assemb (drawing);
		
		/* assemble the ltypes list */
		dxf_ltype_assemb (drawing);
		
		/* assemble the fonts list */
		dxf_fonts_assemb(drawing);
		
		//return drawing;
		state = INIT;
		*prog = 100;
	}
	return 0;
	
quit_error:
	state = INIT;
	return -1;
}

void dxf_append(dxf_node *master, dxf_node *new_node){
	if (master && new_node){
		dxf_node *prev = master->end;
		/*  append new to master's list */
		new_node->master = master;
		new_node->prev = prev;
		if (prev){
			prev->next = new_node;
		}
		new_node->next = NULL; /* append at end of list */
		master->end = new_node;
	}
}

void dxf_list_clear (dxf_node *list){
	if (list){
		list->obj.name[0] = 0;
		list->master = NULL;
		list->prev = NULL;
		list->next = NULL;
		list->type = DXF_ENT;
		list->obj.graphics = NULL;
		if(list->obj.content){
			list->end = list->obj.content;
			list->obj.content->master = list;
			list->obj.content->prev = NULL;
			list->obj.content->next = NULL;
			list->obj.content->type = DXF_ATTR;
			list->obj.content->value.t_data = DXF_INT;
		}
	}
}