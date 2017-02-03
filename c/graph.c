#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#include "dxf.h"
#include "bmp.h"

struct Line_node{
	double x0, y0, x1, y1;
	struct Line_node * next;
};
typedef struct Line_node line_node;

struct Graph_obj{
	dxf_node * owner;
	bmp_color color;
	dxf_ltype ltype;
	double rot, scale, ofs_x, ofs_y;
	line_node * list;
};
typedef struct Graph_obj graph_obj;

graph_obj * graph_new(void){
	/* create new graphics object */
	
	/* allocate the main struct */
	graph_obj * new_obj = malloc(sizeof(graph_obj));
	
	if (new_obj){
		/* allocate the line list */
		new_obj->list = malloc(sizeof(line_node));
		if(new_obj->list){
			/* the list is empty */
			new_obj->list->next = NULL;
		}
		else{ /* if allocation fails */
			free(new_obj); /* clear the main struct */
			new_obj = NULL;
		}
	}
	return new_obj;
}

void line_add(graph_obj * master, double x0, double y0, double x1, double y1){
	/* create and add a line object to the graph master´s list */
	
	if (master){
		line_node *new_line = malloc(sizeof(line_node));
		if (new_line){
			new_line->x0 = x0;
			new_line->y0 = y0;
			new_line->x1 = x1;
			new_line->y1 = y1;
			new_line->next = NULL;
			
			if(master->list->next == NULL){ /* check if list is empty */
				/* then, the new line is the first element */
				master->list->next = new_line;
			}
			else{ /* look for the end of list */
				line_node *tmp = master->list->next;
				while(tmp->next != NULL){
					tmp = tmp->next;
				}
				/* then, the new line is put in end of list */
				tmp->next = new_line;
			}
		}
	}
}

void lines_free(graph_obj * master){
	if (master){
		if(master->list->next){ /* check if list is not empty */
			line_node *next, *current;
			
			current = master->list->next;
			master->list->next = NULL;
			/*sweep the list content */
			while(current){
				next = current->next;
				/* free current node */
				free(current);
				current = next; /* go to next */
				//printf("linha\n");
			}
		}
	}
}

void graph_free(graph_obj * master){
	if (master){
		//printf("geral\n");
		lines_free(master); /* free the lines */
		free(master->list); /* free the list sctructure */
		free(master); /* finally free the graph structure */
	}
}


int main (void){
	graph_obj * test = graph_new();
	line_add(test, 0,0,1,1);
	line_add(test, 0,0,2,2);
	graph_free(test);
	return 0;
}