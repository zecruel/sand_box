#include "graph.h"

graph_obj * graph_new(void){
	/* create new graphics object */
	
	/* allocate the main struct */
	graph_obj * new_obj = malloc(sizeof(graph_obj));
	
	if (new_obj){
		
		/* initialize */
		new_obj->owner = NULL;
		/* initialize with a black color */
		new_obj->color.r = 0;
		new_obj->color.g = 0;
		new_obj->color.b =0;
		new_obj->color.a = 255;
		
		new_obj->rot = 0;
		new_obj->scale = 1;
		new_obj->ofs_x = 0;
		new_obj->ofs_y = 0;
		new_obj->tick = 1;
		
		/* initialize with a solid line pattern */
		new_obj->patt_size = 1;
		new_obj->pattern[0] = 1;
		
		/* extent init */
		new_obj->ext_ini = 0;
		
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
			/*update the extent of graph */
			/* sort the coordinates of entire line*/
			double min_x = (x0 < x1) ? x0 : x1;
			double min_y = (y0 < y1) ? y0 : y1;
			double max_x = (x0 > x1) ? x0 : x1;
			double max_y = (y0 > y1) ? y0 : y1;
			if (master->ext_ini == 0){
				master->ext_ini = 1;
				master->ext_min_x = min_x;
				master->ext_min_y = min_y;
				master->ext_max_x = max_x;
				master->ext_max_y = max_y;
			}
			else{
				master->ext_min_x = (master->ext_min_x < min_x) ? master->ext_min_x : min_x;
				master->ext_min_y = (master->ext_min_y < min_y) ? master->ext_min_y : min_y;
				master->ext_max_x = (master->ext_max_x > max_x) ? master->ext_max_x : max_x;
				master->ext_max_y = (master->ext_max_y > max_y) ? master->ext_max_y : max_y;
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

void graph_draw(graph_obj * master, bmp_img * img, double ofs_x, double ofs_y, double scale){
	if ((master != NULL) && (img != NULL)){
		if(master->list->next){ /* check if list is not empty */
			line_node *current = master->list->next;
			
			/* set the pattern */
			patt_change(img, master->pattern, master->patt_size);
			/* set the color */
			img->frg = master->color;
			/* set the tickness */
			img->tick = master->tick;
			
			/* apply the scale and offset */
			int x0, y0, x1, y1;
			x0 = (int) round((current->x0 - ofs_x) * scale);
			y0 = (int) round((current->y0 - ofs_y) * scale);
			x1 = (int) round((current->x1 - ofs_x) * scale);
			y1 = (int) round((current->y1 - ofs_y) * scale);
			
			
			/* draw the lines */
			while(current){ /*sweep the list content */
				bmp_line(img, x0, y0, x1, y1);
				//printf("%f %d %d %d %d\n", scale, x0, y0, x1, y1);
				
				current = current->next; /* go to next */
				//printf("linha\n");
			}
		}
	}
}

void vec_graph_draw(vector_p * vec, bmp_img * img, double ofs_x, double ofs_y, double scale){
	int i;
	if (vec){
		for(i = 0; i < vec->size; i++){
			graph_draw(((graph_obj **)vec->data)[i], img, ofs_x, ofs_y, scale);
			//printf("%f\n", scale);
			//printf("desenha %d = %d\n", i, ((graph_obj **)vec->data)[i]);
		}
	}
}

void vec_graph_free(vector_p * vec){
	int i;
	if (vec){
		for(i = 0; i < vec->size; i++){
			graph_free(((graph_obj **)vec->data)[i]);
			//printf("Liberando %d = %d\n", i, ((graph_obj **)vec->data)[i]);
		}
		free (vec);
	}
}

int vec_graph_ext(vector_p * vec, int *init, double * min_x, double * min_y, double * max_x, double * max_y){
	int i;
	if (vec){
		graph_obj * current;
		for(i = 0; i < vec->size; i++){
			current = ((graph_obj **)vec->data)[i];
			if (current){
				if (*init == 0){
					*init = 1;
					*min_x = current->ext_min_x;
					*min_y = current->ext_min_y;
					*max_x = current->ext_max_x;
					*max_y = current->ext_max_y;
				}
				else{
					*min_x = (*min_x < current->ext_min_x) ? *min_x : current->ext_min_x;
					*min_y = (*min_y < current->ext_min_y) ? *min_y : current->ext_min_y;
					*max_x = (*max_x > current->ext_max_x) ? *max_x : current->ext_max_x;
					*max_y = (*max_y > current->ext_max_y) ? *max_y : current->ext_max_y;
				}
			}
		}
		return 1;
	}
	return 0;
}

/*
int main (void){
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	
	bmp_img * img = bmp_new(200, 200, white, black);
	graph_obj * test = graph_new();
	line_add(test, 0,0,100,100);
	line_add(test, 210,210,100,2);
	test->tick = 5;
	test->color = red;
	
	graph_draw(test, img);
	
	bmp_save("teste2.ppm", img);
	bmp_free(img);
	graph_free(test);
	return 0;
}
*/