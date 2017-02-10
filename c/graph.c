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
			int x0, y0, x1, y1;			
			line_node *current = master->list->next;
			
			/* set the pattern */
			patt_change(img, master->pattern, master->patt_size);
			/* set the color */
			img->frg = master->color;
			/* set the tickness */
			img->tick = master->tick;
			
			/* draw the lines */
			while(current){ /*sweep the list content */
				/* apply the scale and offset */
				x0 = (int) round((current->x0 - ofs_x) * scale);
				y0 = (int) round((current->y0 - ofs_y) * scale);
				x1 = (int) round((current->x1 - ofs_x) * scale);
				y1 = (int) round((current->y1 - ofs_y) * scale);
				
				bmp_line(img, x0, y0, x1, y1);
				//printf("%f %d %d %d %d\n", scale, x0, y0, x1, y1);
				
				current = current->next; /* go to next */
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

void graph_arc(graph_obj * master, double c_x, double c_y, double radius, double ang_start, double ang_end, int sig){
	if (master){
		int n = 32; //numero de vertices do polígono regular que aproxima o circulo ->bom numero 
		double ang;
		int steps, i;
		double x0, y0, x1, y1;
		
		ang_start *= M_PI/180;
		ang_end *= M_PI/180;
		
		ang = (ang_start - ang_end) * sig; //angulo do arco
		if (ang <= 0){ ang = ang + 2*M_PI;}
		
		//descobre quantos passos para o laço a seguir
		steps = (int) floor(fabs(ang*n/(2*M_PI))); //numero de vertices do arco
		
		x0 = c_x + radius * cos(ang_start);
		y0 = c_y + radius * sin(ang_start);
		
		//printf("Arco, stp = %d, r = %0.2f, ang = %0.2f\n pts = )", steps, radius, ang);
		
		//já começa do segundo vértice
		for (i = 1; i < steps; i++){
			x1 = c_x + radius * cos(2 * M_PI * i * sig/ n + ang_start);
			y1 = c_y + radius * sin(2 * M_PI * i * sig/ n + ang_start);
			
			
			line_add(master, x0, y0, x1, y1);
			//printf("(%0.2f,%0.2f),", x1, y1);
			x0=x1;
			y0=y1;
		}
		// o ultimo vertice do arco eh o ponto final, nao calculado no laço
		x1 = c_x + radius * cos(ang_end);
		y1 = c_y + radius * sin(ang_end);
		line_add(master, x0, y0, x1, y1);
		//printf("(%0.2f,%0.2f)\n", x1, y1);
	}
}

void graph_arc_bulge(graph_obj * master, 
			double pt1_x, double pt1_y,
			double pt2_x, double pt2_y, 
			double bulge){
	
	double theta, alfa, d, radius, ang_c, ang_start, ang_end, center_x, center_y;
	int sig;
	
	theta = 2 * atan(bulge);
	alfa = atan2(pt2_y-pt1_y, pt2_x-pt1_x);
	d = sqrt((pt2_y-pt1_y)*(pt2_y-pt1_y) + (pt2_x-pt1_x)*(pt2_x-pt1_x)) / 2;
	radius = d*(bulge*bulge + 1)/(2*bulge);
	
	ang_c = M_PI+(alfa - M_PI/2 - theta);
	center_x = radius*cos(ang_c) + pt1_x;
	center_y = radius*sin(ang_c) + pt1_y;
	
	//angulo inicial e final obtidos das coordenadas iniciais
	ang_start = atan2(pt1_y-center_y,pt1_x-center_x);
	ang_end = atan2(pt2_y-center_y,pt2_x-center_x);
	
	sig = 1;
	if (bulge < 0){
		ang_start += M_PI;
		ang_end += M_PI;
		sig = -1;
	}
	//converte para garus
	ang_start *= 180/M_PI;
	ang_end *= 180/M_PI;
	
	//arco(entidade, camada, center, radius, ang_start, ang_end, cor, esp, sig);
	graph_arc(master, center_x, center_y, radius, ang_start, ang_end, sig);
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