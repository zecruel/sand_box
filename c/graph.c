#include "graph.h"
#define GRAPH_PAGE 1000
#define LINE_PAGE 1000

void * graph_mem_pool(enum graph_pool_action action){
	static graph_obj *graph_pool[1000];
	static line_node *line_pool[1000];
	static int graph_size = 0;
	static int line_size = 0;
	static int graph_pos = 0;
	static int line_pos = 0;
	static int graph_i_page = 0;
	static int line_i_page = 0;
	
	int i;
	
	void *tmp_ptr = NULL, *ret_ptr = NULL;
	
	if (graph_size < 1){
		graph_pool[0] = malloc(GRAPH_PAGE * sizeof(graph_obj));
		if (graph_pool){
			graph_size = 1;
			//printf("Init graph\n");
		}
	}
	
	if (line_size < 1){
		line_pool[0] = malloc(LINE_PAGE * sizeof(line_node));
		if (line_pool){
			line_size = 1;
			//printf("Init line\n");
		}
	}
	
	
	if ((graph_pos >= GRAPH_PAGE) && (graph_size > 0)){
		if (graph_i_page < graph_size - 1){
			graph_i_page++;
			graph_pos = 0;
			//printf("change graph page\n");
		}
		else if(graph_i_page < 1000-1){
			graph_pool[graph_i_page + 1] = malloc(GRAPH_PAGE * sizeof(graph_obj));
			if (graph_pool[graph_i_page + 1]){
				graph_i_page++;
				graph_size ++;
				graph_pos = 0;
				//printf("Realloc graph\n");
			}
		}
	}
	
	if ((line_pos >= LINE_PAGE) && (line_size > 0)){
		if (line_i_page < line_size - 1){
			line_i_page++;
			line_pos = 0;
			//printf("change line page\n");
		}
		else if(line_i_page < 1000-1){
			line_pool[line_i_page + 1] = malloc(LINE_PAGE * sizeof(line_node));
			if (line_pool[line_i_page + 1]){
				line_i_page++;
				line_size ++;
				line_pos = 0;
				//printf("Realloc line\n");
			}
		}
	}
	
	ret_ptr = NULL;
	
	if ((graph_pool[graph_i_page] != NULL) &&  (line_pool[line_i_page] != NULL)){
		switch (action){
			case ADD_GRAPH:
				if (graph_pos < GRAPH_PAGE){
					ret_ptr = &(graph_pool[graph_i_page][graph_pos]);
					graph_pos++;
				}
				break;
			case ADD_LINE:
				if (line_pos < LINE_PAGE){
					ret_ptr = &(line_pool[line_i_page][line_pos]);
					line_pos++;
				}
				break;
			case ZERO_GRAPH:
				graph_pos = 0;
				graph_i_page = 0;
				break;
			case ZERO_LINE:
				line_pos = 0;
				line_i_page = 0;
				break;
			case FREE_ALL:
				for (i = 0; i < graph_size; i++){
					free(graph_pool[i]);
					graph_pool[i] = NULL;
				}
				graph_pos = 0;
				graph_i_page = 0;
				graph_size = 0;
				for (i = 0; i < line_size; i++){
					free(line_pool[i]);
					line_pool[i] = NULL;
				}
				line_pos = 0;
				line_i_page = 0;
				line_size = 0;
				break;
		}
	}
	return ret_ptr;
}

graph_obj * graph_new(void){
	/* create new graphics object */
	
	/* allocate the main struct */
	//graph_obj * new_obj = malloc(sizeof(graph_obj));
	graph_obj * new_obj = graph_mem_pool(ADD_GRAPH);
	
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
		new_obj->pattern[0] = 1.0;
		
		/* extent init */
		new_obj->ext_ini = 0;
		
		/* allocate the line list */
		//new_obj->list = malloc(sizeof(line_node));
		new_obj->list = graph_mem_pool(ADD_LINE);
		
		if(new_obj->list){
			/* the list is empty */
			new_obj->list->next = NULL;
		}
		else{ /* if allocation fails */
			//free(new_obj); /* clear the main struct */
			new_obj = NULL;
		}
	}
	return new_obj;
}

void line_add(graph_obj * master, double x0, double y0, double x1, double y1){
	/* create and add a line object to the graph master´s list */
	
	if (master){
		//line_node *new_line = malloc(sizeof(line_node));
		line_node *new_line = graph_mem_pool(ADD_LINE);
		
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
			//printf("Liberando %d = %d\n", i, (((graph_obj **)vec->data)[i]));
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
		
		ang = (ang_end - ang_start) * sig; //angulo do arco
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

void graph_ellipse(graph_obj * master, double p1_x, double p1_y, double p2_x, double p2_y, double minor_ax, double ang_start, double ang_end){
	if (master){
		int n = 32; //numero de vertices do polígono regular que aproxima o circulo ->bom numero 
		double ang, major_ax, cosine, sine;
		int steps, i;
		double x0, y0, x1, y1;
		double xx0, yy0, xx1, yy1;
		
		//ang_start *= M_PI/180;
		//ang_end *= M_PI/180;
		
		major_ax = sqrt(pow(p2_x, 2) + pow(p2_y, 2)) ;
		minor_ax *= major_ax;
		
		/* rotation constants */
		cosine = cos(atan2(p2_y, p2_x));
		sine = sin(atan2(p2_y, p2_x));
		
		//printf("major = %0.2f, minor = %0.2f\n", major_ax, minor_ax);
		//printf("ang_start = %0.2f, ang_end = %0.2f\n", ang_start, ang_end);
		
		ang = (ang_end - ang_start); //angulo do arco
		if (ang <= 0){ ang = ang + 2*M_PI;}
		
		//descobre quantos passos para o laço a seguir
		steps = (int) floor(fabs(ang*n/(2*M_PI))); //numero de vertices do arco
		
		x0 = p1_x + major_ax * cos(ang_start);
		y0 = p1_y + minor_ax * sin(ang_start);
		
		//printf("Arco, stp = %d, r = %0.2f, ang = %0.2f\n pts = )", steps, radius, ang);
		
		//já começa do segundo vértice
		for (i = 1; i < steps; i++){
			x1 = p1_x + major_ax * cos(2 * M_PI * i / n + ang_start);
			y1 = p1_y + minor_ax * sin(2 * M_PI * i / n + ang_start);
			
			xx0 = cosine*(x0-p1_x) - sine*(y0-p1_y) + p1_x;
			yy0 = sine*(x0-p1_x) + cosine*(y0-p1_y) + p1_y;
			xx1 = cosine*(x1-p1_x) - sine*(y1-p1_y) + p1_x;
			yy1 = sine*(x1-p1_x) + cosine*(y1-p1_y) + p1_y;
			line_add(master, xx0, yy0, xx1, yy1);
			//printf("(%0.2f,%0.2f),", x1, y1);
			x0=x1;
			y0=y1;
		}
		// o ultimo vertice do arco eh o ponto final, nao calculado no laço
		x1 = p1_x + major_ax * cos(ang_end);
		y1 = p1_y + minor_ax * sin(ang_end);
		
		xx0 = cosine*(x0-p1_x) - sine*(y0-p1_y) + p1_x;
		yy0 = sine*(x0-p1_x) + cosine*(y0-p1_y) + p1_y;
		xx1 = cosine*(x1-p1_x) - sine*(y1-p1_y) + p1_x;
		yy1 = sine*(x1-p1_x) + cosine*(y1-p1_y) + p1_y;
		line_add(master, xx0, yy0, xx1, yy1);
		
		//printf("(%0.2f,%0.2f)\n", x1, y1);
	}
}

void graph_modify(graph_obj * master, double ofs_x, double ofs_y, double scale_x, double scale_y, double rot){
	if ((master != NULL)){
		if(master->list->next){ /* check if list is not empty */
			double x0, y0, x1, y1;
			double sine = 0, cosine = 1;
			double min_x, min_y, max_x, max_y;
			line_node *current = master->list->next;
			master->ext_ini = 0;
			
			/* rotation constants */
			cosine = cos(rot*M_PI/180);
			sine = sin(rot*M_PI/180);
			
			/* apply changes to each point */
			while(current){ /*sweep the list content */
				/* apply the scale and offset */
				current->x0 = current->x0 * scale_x + ofs_x;
				current->y0 = current->y0 * scale_y + ofs_y;
				current->x1 = current->x1 * scale_x + ofs_x;
				current->y1 = current->y1 * scale_y + ofs_y;
				
				x0 = cosine*(current->x0-ofs_x) - sine*(current->y0-ofs_y) + ofs_x;
				y0 = sine*(current->x0-ofs_x) + cosine*(current->y0-ofs_y) + ofs_y;
				x1 = cosine*(current->x1-ofs_x) - sine*(current->y1-ofs_y) + ofs_x;
				y1 = sine*(current->x1-ofs_x) + cosine*(current->y1-ofs_y) + ofs_y;
				
				/* update the graph */
				current->x0 = x0;
				current->y0 = y0;
				current->x1 = x1;
				current->y1 = y1;
				
				/*update the extent of graph */
				/* sort the coordinates of entire line*/
				min_x = (x0 < x1) ? x0 : x1;
				min_y = (y0 < y1) ? y0 : y1;
				max_x = (x0 > x1) ? x0 : x1;
				max_y = (y0 > y1) ? y0 : y1;
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
				
				current = current->next; /* go to next */
			}
		}
	}
}

void vec_graph_modify(vector_p * vec, double ofs_x, double ofs_y , double scale_x, double scale_y, double rot){
	int i;
	if (vec){
		for(i = 0; i < vec->size; i++){
			graph_modify(((graph_obj **)vec->data)[i], ofs_x, ofs_y, scale_x, scale_y, rot);
		}
	}
}

void vec_graph_modify_idx(vector_p * vec, double ofs_x, double ofs_y , double scale_x, double scale_y, double rot, int start_idx, int end_idx){
	int i;
	if (vec){
		for(i = start_idx; i <= end_idx; i++){
			graph_modify(((graph_obj **)vec->data)[i], ofs_x, ofs_y, scale_x, scale_y, rot);
		}
	}
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



knot(n,c,x)

int n,c;
int x[];

	/*
	Subroutine to generate a B-spline open knot vector with multiplicity
	equal to the order at the ends.

	c            = order of the basis function
	n            = the number of defining polygon vertices
	nplus2       = index of x() for the first occurence of the maximum knot vector value
	nplusc       = maximum value of the knot vector -- $n + c$
	x()          = array containing the knot vector
	*/
{
	int nplusc,nplus2,i;

	nplusc = n + c;
	nplus2 = n + 2;
	
	if (nplusc < MAX_SPLINE_PTS){
		x[1] = 0;
		for (i = 2; i <= nplusc; i++){
			if ( (i > c) && (i < nplus2) )
				x[i] = x[i-1] + 1;
			else
				x[i] = x[i-1];
		}
	}
}	

rbasis(c,t,npts,x,h,r)

int c,npts;
double t;
int x[];
double h[];
double r[];

	/*  Subroutine to generate rational B-spline basis functions--open knot vector

	C code for An Introduction to NURBS
	by David F. Rogers. Copyright (C) 2000 David F. Rogers,
	All rights reserved.
	
	Name: rbasis
	Language: C
	Subroutines called: none
	Book reference: Chapter 4, Sec. 4. , p 296

	c        = order of the B-spline basis function
	d        = first term of the basis function recursion relation
	e        = second term of the basis function recursion relation
	h[]	     = array containing the homogeneous weights
	npts     = number of defining polygon vertices
	nplusc   = constant -- npts + c -- maximum number of knot values
	r[]      = array containing the rationalbasis functions
	       r[1] contains the basis function associated with B1 etc.
	t        = parameter value
	temp[]   = temporary array
	x[]      = knot vector
	*/

{
	int nplusc;
	int i,j,k;
	double d,e;
	double sum;
	double temp[MAX_SPLINE_PTS];

	nplusc = npts + c;
	
	if (nplusc < MAX_SPLINE_PTS){

		/*		
		printf("knot vector is \n");
		for (i = 1; i <= nplusc; i++){
			printf(" %d %d \n", i,x[i]);
		}
		printf("t is %f \n", t);
		*/

		/* calculate the first order nonrational basis functions n[i]	*/

		for (i = 1; i<= nplusc-1; i++){
			if (( t >= x[i]) && (t < x[i+1]))
				temp[i] = 1;
			else
				temp[i] = 0;
		}

		/* calculate the higher order nonrational basis functions */

		for (k = 2; k <= c; k++){
			for (i = 1; i <= nplusc-k; i++){
				if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
					d = ((t-x[i])*temp[i])/(x[i+k-1]-x[i]);
				else
					d = 0;

				if (temp[i+1] != 0)     /* if the lower order basis function is zero skip the calculation */
					e = ((x[i+k]-t)*temp[i+1])/(x[i+k]-x[i+1]);
				else
					e = 0;

				temp[i] = d + e;
			}
		}

		if (t == (double)x[nplusc]){		/*    pick up last point	*/
			temp[npts] = 1;
		}
		/*
		printf("Nonrational basis functions are \n");
		for (i=1; i<= npts; i++){
			printf("%f ", temp[i]);
		}
		printf("\n");
		*/
		/* calculate sum for denominator of rational basis functions */

		sum = 0.;
		for (i = 1; i <= npts; i++){
			sum = sum + temp[i]*h[i];
		}

		/* form rational basis functions and put in r vector */

		for (i = 1; i <= npts; i++){
			if (sum != 0){
				r[i] = (temp[i]*h[i])/(sum);}
			else
				r[i] = 0;
		}
	}
}

rbspline(npts,k,p1,b,h,p)

int npts,k,p1;

double b[];
double h[];
double p[];

	/*  Subroutine to generate a rational B-spline curve using an uniform open knot vector

	C code for An Introduction to NURBS
	by David F. Rogers. Copyright (C) 2000 David F. Rogers,
	All rights reserved.
	
	Name: rbspline.c
	Language: C
	Subroutines called: knot.c, rbasis.c, fmtmul.c
	Book reference: Chapter 4, Alg. p. 297

	b[]         = array containing the defining polygon vertices
		  b[1] contains the x-component of the vertex
		  b[2] contains the y-component of the vertex
		  b[3] contains the z-component of the vertex
	h[]			= array containing the homogeneous weighting factors 
	k           = order of the B-spline basis function
	nbasis      = array containing the basis functions for a single value of t
	nplusc      = number of knot values
	npts        = number of defining polygon vertices
	p[,]        = array containing the curve points
		  p[1] contains the x-component of the point
		  p[2] contains the y-component of the point
		  p[3] contains the z-component of the point
	p1          = number of points to be calculated on the curve
	t           = parameter value 0 <= t <= npts - k + 1
	x[]         = array containing the knot vector
	*/

{
	int i,j,icount,jcount;
	int i1;
	int x[MAX_SPLINE_PTS];		/* allows for 20 data points with basis function of order 5 */
	int nplusc;

	double step;
	double t;
	double nbasis[MAX_SPLINE_PTS];
	double temp;


	nplusc = npts + k;
	
	if (nplusc < MAX_SPLINE_PTS){

		/*  zero and redimension the knot vector and the basis array */

		for(i = 0; i <= npts; i++){
			nbasis[i] = 0.;
		}

		for(i = 0; i <= nplusc; i++){
			x[i] = 0.;
		}

		/* generate the uniform open knot vector */

		knot(npts,k,x);

		/*
		printf("The knot vector is ");
		for (i = 1; i <= nplusc; i++){
			printf(" %d ", x[i]);
		}
		printf("\n");
		*/

		
		icount = 0;

		/*    calculate the points on the rational B-spline curve */

		t = 0;
		step = ((double)x[nplusc])/((double)(p1-1));

		for (i1 = 1; i1<= p1; i1++){
			if ((double)x[nplusc] - t < 5e-6){
				t = (double)x[nplusc];
			}
			
			rbasis(k,t,npts,x,h,nbasis);      /* generate the basis function for this value of t */
			//printf("cpts=%d, order=%d, t=%0.2f \n",npts,k,t);
			/*
			printf("t = %f \n",t);
			printf("nbasis = ");
			for (i = 1; i <= npts; i++){
				printf("%f  ",nbasis[i]);
			}
			printf("\n");
			*/
			for (j = 1; j <= 3; j++){      /* generate a point on the curve */
				jcount = j;
				p[icount+j] = 0.;

				for (i = 1; i <= npts; i++){ /* Do local matrix multiplication */
					temp = nbasis[i]*b[jcount];
					p[icount + j] = p[icount + j] + temp;
					/*
					printf("jcount,nbasis,b,nbasis*b,p = %d %f %f %f %f\n",jcount,nbasis[i],b[jcount],temp,p[icount+j]);
					*/
					jcount = jcount + 3;
				}
			}
			/*
			printf("icount, p %d %f %f %f \n",icount,p[icount+1],p[icount+2],p[icount+3]);
			*/
			icount = icount + 3;
			t = t + step;
		}
	}
}