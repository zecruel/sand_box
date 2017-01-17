#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYDXF_STRDUP(str,lit) strcpy(str=malloc(strlen(lit)+1),lit)

struct Dxf_node{
	struct Dxf_node *master; /* entity to which it is contained */
	struct Dxf_node *next;    /* next node (double linked list) */
	struct Dxf_node *prev;    /* previous node (double linked list) */
	
	/* defines whether it is an DXF entity (obj) or an attribute (value) */
	enum {DXF_ENT, DXF_ATTR} type;
	
	union{
		struct {
			/* == entity dxf especific */
			char *name; /* standardized DXF name of entity */
			struct Dxf_node *content; /* the content is a list */
		} obj;
		
		struct {
			/* ==group dxf especific */
			int group; /* standardized DXF group */
			/* the group defines the type of data, which can be: */
			enum {DXF_FLOAT, DXF_INT, DXF_STR} t_data;
			union {
				double d_data; /* a float number, */
				int i_data; /* a integer number, */
				char *s_data; /* or a string. */
			};
		} value;
	};
}; 
typedef struct Dxf_node dxf_node;

/* Linked list to store pointers. The head node store the size. */
struct List_p{
	union{
		int size;
		void *data;
	};
	struct List_p *next;
};
typedef struct List_p list_p;

struct Vector_p{
	int size;
	void *data;
};
typedef struct Vector_p vector_p;


/* functions*/
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

void list_p_clear(list_p *head){
	list_p *next, *current;
	
	current = head;
	while(current){
		next = current->next;
		free(current);
		current = next;
	}
}

void ent_clear (dxf_node *ent){ /* free the memory of list or entity */
	if (ent){
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				ent_clear(ent->obj.content); /* recursive call */
			}
			if (ent->obj.name){
				free(ent->obj.name);  /* free the string of entity's name */
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
			ent_clear(ent->next); /* recursive call */
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

dxf_node * obj_new (char *name){
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

dxf_node * attr_new (int group, int type, void *value){
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

vector_p find_attr(dxf_node * obj, int attr){
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
						data = realloc(data, size * sizeof(dxf_node *));
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

vector_p find_obj(dxf_node * obj, char *name){
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
						data = realloc(data, size * sizeof(dxf_node *));
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

vector_p find_obj_descr(dxf_node * obj, char *name, char *descr){
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
						descr_attr = find_attr(current, 2); /* look for descriptor in group 2 attribute */
						if (descr_attr.data){ /* found attribute */
							/* match descriptor */
							if(strcmp(((dxf_node **) descr_attr.data)[0]->value.s_data, descr) == 0){
								/* success */
								size++;
								data = realloc(data, size * sizeof(dxf_node *));
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

dxf_node * dxf_open (char *path){
	char buf[255], *line;
	FILE *file;
	long f_index = 0;  /*  indexes the file´s lines */
	dxf_node *new_node = NULL;
	dxf_node *master, *prev, *next, *tmp, *last_obj;
	dxf_node *drawing = NULL;
	int group = 0; /* current group */
	int t_data = DXF_STR; /* current type of data */
	double d_data;
	int i_data;
	
	/* create a new drawing */
	drawing = obj_new(NULL);
	
	file = fopen(path, "r");
	if(file == NULL){
		drawing = NULL;
	}
	else if (drawing){
		
		master = drawing; /* current master is the drawing */
		last_obj = drawing;
		prev = drawing->obj.content; /* the list starts on drawing's content */
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
							new_node = obj_new (line); /* new object */
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
							new_node = obj_new (line); /* new object */
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
						new_node = attr_new (group, t_data, (void *) &i_data);
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
								new_node = attr_new (group, t_data, (void *) &d_data);
								break;
							case DXF_INT :
								i_data = atoi(line);
								new_node = attr_new (group, t_data, (void *) &i_data);
								break;
							case DXF_STR :
								new_node = attr_new (group, t_data, (void *) line);
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
	return drawing;
}

int main(void)
{
	char url[]="teste.dxf";
	dxf_node * drawing;
	vector_p sec, name, head;
	
	drawing = dxf_open(url);
	//ent_print(drawing->obj.content->next, 0);
	sec = find_obj(drawing, "SECTION");
	
	if ((sec.size > 0) && (sec.data != NULL)){
		//ent_print(((dxf_node **) sec.data)[0]->obj.content->next, 0);
		
		int i;
		for (i=0; i < sec.size; i++){
			printf ("\n%s ", ((dxf_node **) sec.data)[i]->obj.name);
			name = find_attr(((dxf_node **) sec.data)[i], 2);
			printf ("%d ", name.size);
			if (name.data){
				printf("%s", ((dxf_node **) name.data)[0]->value.s_data);
			}
		}
	}
	
	head = find_obj_descr(drawing, "SECTION", "HEADER");
	if (head.data){
		ent_print(((dxf_node **) head.data)[0]->obj.content->next, 0);
	}
	
	free(sec.data);
	free(name.data);
	free(head.data);
	
	ent_clear(drawing);
	//printf("\nNum linhas = %d", f_index);

	
	return 0;
}

/*
//Bitmap/Bresenham's line algorithm
//http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

void line(int x0, int y0, int x1, int y1) {
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    setPixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


Depth-first search (DFS): Finding connected components.

*/