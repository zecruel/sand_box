
#include "dxf_attract.h"
#include "list.h"

#define IN_BOUNDS(x,y,p1x,p1y,p2x,p2y) ((((x <= p1x) && (x >= p2x))||((x <= p2x) && (x >= p1x))) && (((y <= p1y) && (y >= p2y))||((y <= p2y) && (y >= p1y))))
#define TOL 1e-9
#define NEAR_LN(x,y,p1x,p1y,p2x,p2y,s) (((fabs(p1x-p2x)<TOL) && (fabs(p1x - x) < s) && (((y <= p1y) && (y >= p2y))||((y <= p2y) && (y >= p1y)))) || ((fabs(p1y-p2y)<TOL) && (fabs(p1y - y) < s) && (((x <= p1x) && (x >= p2x))||((x <= p2x) && (x >= p1x)))))
#define MAX_CAND 50

#define SIG(x) (x >= 0) ? 1 : -1

static double dot_product(double a[3], double b[3]){
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}
 
static void cross_product(double a[3], double b[3], double c[3]){
	c[0] = a[1]*b[2] - a[2]*b[1];
	c[1] = a[2]*b[0] - a[0]*b[2];
	c[2] = a[0]*b[1] - a[1]*b[0];
}

static void unit_vector(double a[3]){
	double mod;
	
	mod = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
	if (mod > 0.0) {
		a[0] /= mod;
		a[1] /= mod;
		a[2] /= mod;
	}
}

void arc_bulge(double pt1_x, double pt1_y,
double pt2_x, double pt2_y, double bulge,
double *radius, double *ang_start, double *ang_end,
double *center_x, double *center_y){
	
	double theta, alfa, d, ang_c, start, end;
	//int sig;
	
	theta = 2 * atan(bulge);
	alfa = atan2(pt2_y-pt1_y, pt2_x-pt1_x);
	d = sqrt((pt2_y-pt1_y)*(pt2_y-pt1_y) + (pt2_x-pt1_x)*(pt2_x-pt1_x)) / 2;
	*radius = d*(bulge*bulge + 1)/(2*bulge);
	
	ang_c = M_PI+(alfa - M_PI/2 - theta);
	*center_x = *radius*cos(ang_c) + pt1_x;
	*center_y = *radius*sin(ang_c) + pt1_y;
	
	//angulo inicial e final obtidos das coordenadas iniciais
	start = atan2(pt1_y - *center_y, pt1_x - *center_x);
	end = atan2(pt2_y - *center_y, pt2_x - *center_x);
	
	/* set angle range to 0-2*pi */
	if (start < 0){
		start += 2*M_PI;
	}
	if (end < 0){
		end += 2*M_PI;
	}
	if (*radius >= 0){
		*ang_start = start;
		*ang_end = end;
	}
	else{
		*ang_start = end;
		*ang_end = start;
		*radius = fabs(*radius);
	}
	//converte para garus
	//*ang_start *= 180/M_PI;
	//*ang_end *= 180/M_PI;
}

int axis_transform(double *x, double *y, double *z, double normal[3]){
	if ((x != NULL) && (y != NULL) && (z != NULL)){
		double x0, y0, z0;
		double x_axis[3], y_axis[3], z_axis[3], point[3];
		double wy_axis[3] = {0.0, 1.0, 0.0};
		double wz_axis[3] = {0.0, 0.0, 1.0};
		
		/* axis tranform */
		/* choose absolute world axis, according DXF spec, and obtain x axis */
		if ((fabs(normal[0] < 0.015625)) && (fabs(normal[1] < 0.015625))){
			cross_product(wy_axis, normal, x_axis);
		}
		else{
			cross_product(wz_axis, normal, x_axis);
		}
		unit_vector(x_axis); /* normalize axis */
		cross_product(normal, x_axis, y_axis); /*obtain y axis */
		unit_vector(y_axis); /* normalize axis */
		
		/* z_axis is the same normal, but normalized*/
		z_axis[0] = normal[0]; z_axis[1] = normal[1]; z_axis[2] = normal[2];
		unit_vector(z_axis); /* normalize axis */
		
		/*obtain result point */
		point[0] = *x;
		point[1] = *y;
		point[2] = *z;
		*x = dot_product(point, x_axis);
		*y = dot_product(point, y_axis);
		*z = dot_product(point, z_axis);
		
		return 1;
	}
	return 0;
}

int transform(double *x, double *y, struct ins_space space){
	if ((x != NULL) && (y != NULL)){
		double x0, y0;
		
		/* rotation constants */
		double cosine = cos(space.rot * M_PI/180);
		double sine = sin(space.rot * M_PI/180);
		
		/* scale and translation transformations */
		*x = *x * space.scale_x + space.ofs_x;
		*y = *y * space.scale_y + space.ofs_y;
		
		/* rotation transform */ 
		x0 = cosine*(*x - space.ofs_x) - sine*(*y - space.ofs_y) + space.ofs_x;
		y0 = sine*(*x - space.ofs_x) + cosine*(*y - space.ofs_y) + space.ofs_y;

		/* update return values*/ 
		*x = x0;
		*y = y0;
		
		return 1;
	}
	return 0;
}

int ellipse_transf(double *center_x, double *center_y, double *center_z, 
double *axis, double *ratio, double *rot, double normal[3]){
	int ok = 0;
	
	if ((center_x != NULL) && (center_y != NULL) && (center_z != NULL) && (axis != NULL) && (ratio != NULL) && (rot != NULL) && (normal != NULL)){
		double pt1_x, pt1_y, pt1_z;
		/* convert OCS to WCS */
		axis_transform(center_x, center_y, center_z, normal);
		
		/* transform the circle in ellipse, by the OCS */
		pt1_x = 1.0;
		pt1_y = 1.0;
		pt1_z = 0.0;
		axis_transform(&pt1_x, &pt1_y, &pt1_z, normal);
		*axis *= fabs(pt1_x);
		*ratio *= fabs(pt1_y / pt1_x);
		*rot += atan2(normal[0], normal[1]);
		ok = 1;
	}
	return ok;
}

double ellipse_par (double ang, double a, double b){
	/* find the polar parameter (t) for ellipse */
	double t = atan(a*tan(ang)/b);
	if ((ang > M_PI/2) && (ang < M_PI)){
		t += M_PI; 
	}
	else if ((ang >= M_PI) && (ang <= 3*M_PI/2)){
		t -= M_PI; 
	}
	if (t < 0 ) t += 2*M_PI;
	
	return t;
}

void angle_range(double *ang){
	/* set angle range to 0-2*pi */
	if (fabs(*ang) > 2*M_PI) *ang = fmod(*ang, 2*M_PI);
	if (*ang < 0) *ang += 2*M_PI;
}

int dxf_line_get (dxf_drawing *drawing, dxf_node * obj, 
double *pt1_x, double *pt1_y, double *pt1_z, 
double *pt2_x, double *pt2_y, double *pt2_z){
	
	dxf_node *current = NULL;
	
	/*flags*/
	int pt1 = 0, pt2 = 0, ok = 0;
	
	if (obj){ 
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
				*pt1_x = 0, *pt1_y = 0, *pt1_z = 0;
				*pt2_x = 0, *pt2_y = 0, *pt2_z = 0;
			}
		}
	}

	while (current){
		if (current->type == DXF_ATTR){ /* DXF attibute */
			switch (current->value.group){
				case 10:
					*pt1_x = current->value.d_data;
					pt1 = 1; /* set flag */
					break;
				case 11:
					*pt2_x = current->value.d_data;
					pt2 = 1; /* set flag */
					break;
				case 20:
					*pt1_y = current->value.d_data;
					pt1 = 1; /* set flag */
					break;
				case 21:
					*pt2_y = current->value.d_data;
					pt2 = 1; /* set flag */
					break;
				case 30:
					*pt1_z = current->value.d_data;
					pt1 = 1; /* set flag */
					break;
				case 31:
					*pt2_z = current->value.d_data;
					pt2 = 1; /* set flag */
					break;
			}
		}
		current = current->next; /* go to the next in the list */
	}
	
	if((pt1 !=0) && (pt2 !=0)) ok = 1;
	
	return ok;
}

int dxf_circle_get(dxf_drawing *drawing, dxf_node * obj,
double *center_x, double *center_y, double *center_z, 
double *axis, double *ratio, double *rot){
	int ok = 0;
	
	if(obj){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0, radius = 0;
		double elev = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		/*flags*/
		int pt1 = 0;
				
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 40:
						radius = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 210:
						extru_x = current->value.d_data;
						break;
					case 220:
						extru_y = current->value.d_data;
						break;
					case 230:
						extru_z = current->value.d_data;
						break;
				}
			}
			current = current->next; /* go to the next in the list */
		}
		
		/* convert OCS to WCS, transform the circle in ellipse */
		normal[0] = extru_x;
		normal[1] = extru_y;
		normal[2] = extru_z;
		*center_x = pt1_x;
		*center_y = pt1_y;
		*center_z = pt1_z;
		*axis = radius;
		*ratio = 1.0;
		*rot = 0.0;
		ok = ellipse_transf(center_x, center_y, center_z, axis, ratio, rot, normal);
	}
	return ok;
}

int dxf_arc_get(dxf_drawing *drawing, dxf_node * obj,
double *center_x, double *center_y, double *center_z, 
double *axis, double *ratio, double *rot, 
double *start_ang, double *end_ang){
	int ok = 0;
	
	if(obj){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0, radius;
		double elev = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		/*flags*/
		int pt1 = 0;
				
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 40:
						radius = current->value.d_data;
						break;
					case 50:
						*start_ang = current->value.d_data;
						break;
					case 51:
						*end_ang = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 210:
						extru_x = current->value.d_data;
						break;
					case 220:
						extru_y = current->value.d_data;
						break;
					case 230:
						extru_z = current->value.d_data;
						break;
				}
			}
			current = current->next; /* go to the next in the list */
		}
		
		/* convert OCS to WCS, transform the circle in ellipse */
		normal[0] = extru_x;
		normal[1] = extru_y;
		normal[2] = extru_z;
		*center_x = pt1_x;
		*center_y = pt1_y;
		*center_z = pt1_z;
		*axis = radius;
		*ratio = 1.0;
		*rot = 0.0;
		ok = ellipse_transf(center_x, center_y, center_z, axis, ratio, rot, normal);
		
		/*convert to radians */
		*start_ang *= M_PI/180;
		*end_ang *= M_PI/180;
	}
	return ok;
}

int dxf_ellipse_get(dxf_drawing *drawing, dxf_node * obj,
double *center_x, double *center_y, double *center_z, 
double *axis, double *ratio, double *rot, 
double *start_ang, double *end_ang){
	int ok = 0;
	
	if(obj){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		double elev = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		/*flags*/
		int pt1 = 0, pt2 = 0;
				
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 11:
						pt2_x = current->value.d_data;
						pt2 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 21:
						pt2_y = current->value.d_data;
						pt2 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 31:
						pt2_z = current->value.d_data;
						pt2 = 1; /* set flag */
						break;
					case 40:
						*ratio = current->value.d_data;
						break;
					case 41:
						*start_ang = current->value.d_data;
						break;
					case 42:
						*end_ang = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 210:
						extru_x = current->value.d_data;
						break;
					case 220:
						extru_y = current->value.d_data;
						break;
					case 230:
						extru_z = current->value.d_data;
						break;
				}
			}
			current = current->next; /* go to the next in the list */
		}
		
		/* convert OCS to WCS */
		normal[0] = extru_x;
		normal[1] = extru_y;
		normal[2] = extru_z;
		*center_x = pt1_x;
		*center_y = pt1_y;
		*center_z = pt1_z;
		*axis = sqrt(pow(pt2_x, 2) + pow(pt2_y, 2));
		*rot = atan2(pt2_y, pt2_x);
		ok = ellipse_transf(center_x, center_y, center_z, axis, ratio, rot, normal);
		
		/*convert to radians */
		//*start_ang *= M_PI/180;
		//*end_ang *= M_PI/180;
	}
	return ok;
}

int dxf_lwpline_get_pt(dxf_drawing *drawing,
dxf_node * obj, dxf_node ** next,
double *pt1_x, double *pt1_y, double *pt1_z, double *bulge){
	
	dxf_node *current = NULL;
	static dxf_node *last = NULL;
	static double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
	
	/*flags*/
	int first = 0, pt1 = 0, ok = 0;
	static int pline_flag = 0, closed =0, init = 0;
	
	double px = 0.0, py = 0.0, pz = 0.0, bul = 0.0;
	static double last_x, last_y, last_z, curr_x, elev;
	
	double x, y, z; /*return values */
	
	if (*next == NULL){ /* parse object first time */
		pline_flag = 0; closed =0; init = 0;
		last = NULL;
		if(obj){
			if (obj->type == DXF_ENT){
				if (obj->obj.content){
					current = obj->obj.content->next;
					//printf("%s\n", obj->obj.name);
				}
			}
		}
		/* get general parameters of polyline */
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 10:
						px = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						py = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 30:
						pz = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 42:
						bul = current->value.d_data;
						break;
					case 70:
						pline_flag = current->value.i_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 210:
						extru_x = current->value.d_data;
						break;
					case 220:
						extru_y = current->value.d_data;
						break;
					case 230:
						extru_z = current->value.d_data;
						break;
				}
			}
			if (pt1){
				pt1 = 0;
				if (init == 0){
					init = 1;
					curr_x = px;
					last = current;
				}
				else{
					*next = current;
					break;
				}
			}
			current = current->next; /* go to the next in the list */
			//*next = current->next;
		}
		if (init){
			if (pline_flag & 1){
				closed = 1;
				/* if closed, the last vertex is first */
				last_x = curr_x;
				last_y = py;
				last_z = pz;
			}
			else {
				closed = 0;
				last = NULL;
			}
			
			/* to convert OCS to WCS */
			normal[0] = extru_x;
			normal[1] = extru_y;
			normal[2] = extru_z;			
			
			x = curr_x;
			y = py;
			z = pz;
			*bulge = bul;
			ok = 1;
			
		}
	}
	else if ((init) && (*next != last)){ /* continue search in next point */
		current = *next;
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 10:
						px = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						py = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 30:
						pz = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 42:
						bul = current->value.d_data;
						break;
				}
			}
			if (pt1){
				pt1 = 0;
				if (first == 0){
					first = 1;
					curr_x = px;
				}
				else{
					*next = current;
					break;
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (first){
			
			x = curr_x;
			y = py;
			z = pz;
			*bulge = bul;
			ok = 1;
			if (current == NULL){
				if (closed){
					*next = last;
				}
				else {
					*next = NULL;
					pline_flag = 0; closed =0; init = 0;
					last = NULL;
				}
			}
		}
		else if (closed){
			*next = last;
		}
		else {
			*next = NULL;
			pline_flag = 0; closed =0; init = 0;
			last = NULL;
		}
	}
	else { /* last vertex */
		x = last_x;
		y = last_y;
		z = last_z;
		*bulge = 0.0;
		ok = 1;
		*next = NULL;
		pline_flag = 0; closed =0; init = 0;
		last = NULL;
	}
	
	if (ok == 0){
		*next = NULL;
		pline_flag = 0; closed =0; init = 0;
		last = NULL;
	}
	/* convert OCS to WCS */
	else if (axis_transform(&x, &y, &z, normal)){
		/* update return values */
		*pt1_x = x;
		*pt1_y = y;
		*pt1_z = z;
	}
	
	return ok;
}


int dxf_line_attract(double pt1_x, double pt1_y, 
double pt2_x, double pt2_y, enum attract_type type,
double pos_x, double pos_y, double sensi, 
double *ret_x, double *ret_y,
int *init_dist, double *min_dist){
	
	int ret = ATRC_NONE;
	double curr_dist;
			
	if(type & ATRC_END){ /* if type of attractor is flaged as endpoint */
		/* check if points of the line pass on distance criteria */
		curr_dist = sqrt(pow(pt1_x - pos_x, 2) + pow(pt1_y - pos_y, 2));
		if (curr_dist < sensi){
			if (*init_dist == 0){
				*init_dist = 1;
				*min_dist = curr_dist;
				*ret_x = pt1_x;
				*ret_y = pt1_y;
				ret = ATRC_END;
			}
			else if (curr_dist < *min_dist){
				*min_dist = curr_dist;
				*ret_x = pt1_x;
				*ret_y = pt1_y;
				ret = ATRC_END;
			}
		}
		curr_dist = sqrt(pow(pt2_x - pos_x, 2) + pow(pt2_y - pos_y, 2));
		if (curr_dist < sensi){
			if (*init_dist == 0){
				*init_dist = 1;
				*min_dist = curr_dist;
				*ret_x = pt2_x;
				*ret_y = pt2_y;
				ret = ATRC_END;
			}
			else if (curr_dist < *min_dist){
				*min_dist = curr_dist;
				*ret_x = pt2_x;
				*ret_y = pt2_y;
				ret = ATRC_END;
			}
		}
		
	}
	if(type & ATRC_MID){ /* if type of attractor is flaged as midpoint */
		double mid_x = (pt1_x + pt2_x)/2;
		double mid_y = (pt1_y + pt2_y)/2;
		curr_dist = sqrt(pow(mid_x - pos_x, 2) + pow(mid_y - pos_y, 2));
		if (curr_dist < sensi){
			if (*init_dist == 0){
				*init_dist = 1;
				*min_dist = curr_dist;
				*ret_x = mid_x;
				*ret_y = mid_y;
				ret = ATRC_MID;
			}
			else if (curr_dist < *min_dist){
				*min_dist = curr_dist;
				*ret_x = mid_x;
				*ret_y = mid_y;
				ret = ATRC_MID;
			}
		}
	}
	
	if(type & ATRC_ANY){ /* if type of attractor is flaged as any point */
		/*consider line equation ax + by + c = 0 */
		double a = pt2_y - pt1_y;
		double b = -(pt2_x - pt1_x);
		double c = pt2_x * pt1_y - pt2_y * pt1_x;
		
		if ((a != 0) || (b != 0)){
			/* calcule distance between point  and line */
			curr_dist = fabs(a*pos_x + b*pos_y + c)/
					sqrt(pow(a, 2) + pow(b, 2));
			if (curr_dist < sensi){
				/* look the closest point on line */
				/* equation from https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line */
				double any_x, any_y;
				if (b != 0){
					any_x = (b * (b*pos_x - a*pos_y) - a*c)/
							(pow(a, 2) + pow(b, 2));
				}
				else{
					any_x = pt1_x;
				}
				if (a != 0){
					any_y = (a * (-b*pos_x + a*pos_y) - b*c)/
							(pow(a, 2) + pow(b, 2));
				}
				else {
					any_y = pt1_y;
				}
				/* verify if point is in segment */
				if ((((any_x <= pt1_x) && (any_x >= pt2_x))||
					((any_x <= pt2_x) && (any_x >= pt1_x))) &&
					(((any_y <= pt1_y) && (any_y >= pt2_y))||
					((any_y <= pt2_y) && (any_y >= pt1_y)))){
					
					if (*init_dist == 0){
						*init_dist = 1;
						*min_dist = curr_dist;
						*ret_x = any_x;
						*ret_y = any_y;
						ret = ATRC_ANY;
					}
					else if (curr_dist < *min_dist){
						*min_dist = curr_dist;
						*ret_x = any_x;
						*ret_y = any_y;
						ret = ATRC_ANY;
					}
				}
			}
		}
		
	}
	return ret;
}

int dxf_arc_attract(double radius, double ang_start, double ang_end,
double center_x, double center_y, double ratio, double rot,
enum attract_type type,
double pos_x, double pos_y, double sensi, 
double *ret_x, double *ret_y,
int *init_dist, double *min_dist){
	/* elliptical arc */
	
	int ret = ATRC_NONE;
	double curr_dist;
	
	double test, start, end;
	
	/* rotation constants */
	double cosine = cos(rot);
	double sine = sin(rot);
	
	/*ellipse's parameters */
	double a = radius, b = radius * ratio;
	
	/* find the angle of point, referenced to arc center */
	double ang_pt = atan2(pos_y - center_y, pos_x - center_x);
	
	angle_range(&rot); /* set angle range to 0-2*pi */
	
	/* update by rotation */
	ang_pt = ang_pt - rot;
	angle_range(&ang_pt); /* set angle range to 0-2*pi */
	
	/* find the ellipse polar parameter (t) for  point*/
	double t = ellipse_par(ang_pt, a, b);
	
	/* verify if arc is a full circle */
	if ((fabs(ang_end - ang_start) < 1e-6) || (fabs(ang_end - ang_start) >= (2*M_PI - 1e-6))){
		ang_start = 0.0;
		ang_end = 2*M_PI;
		type &= ~ATRC_END; /* disable the endpoint attractor */
		
		start = ang_start;
		end = ang_end;
		test = 0.0;
	}
	else{
		angle_range(&ang_start); /* set angle range to 0-2*pi */
		angle_range(&ang_end); /* set angle range to 0-2*pi */
		
		/* verify if point angle is between start and end of arc */
		test = t - ang_start;
		if (test < 0 ) test += 2*M_PI;
		end = ang_end - ang_start;
		if (end < 0 ) end += 2*M_PI;
	}
	
	if (test <= end){		
		/* point in ellipse near current position */
		double near_x = center_x + a*cos(t)*cosine - b*sin(t)*sine;
		double near_y = center_y + a*cos(t)*sine + b*sin(t)*cosine;
		
		if(type & ATRC_END){ /* if type of attractor is flaged as endpoint */
			/* start and end points of arc, in cartesian coodinates */
			double pt1_x = center_x + a*cos(ang_start)*cosine - b*sin(ang_start)*sine;
			double pt1_y = center_y + a*cos(ang_start)*sine + b*sin(ang_start)*cosine;
			double pt2_x = center_x + a*cos(ang_end)*cosine - b*sin(ang_end)*sine;
			double pt2_y = center_y + a*cos(ang_end)*sine + b*sin(ang_end)*cosine;
			
			/* check if points of the arc pass on distance criteria */
			curr_dist = sqrt(pow(pt1_x - pos_x, 2) + pow(pt1_y - pos_y, 2));
			if (curr_dist < sensi){
				if (*init_dist == 0){
					*init_dist = 1;
					*min_dist = curr_dist;
					*ret_x = pt1_x;
					*ret_y = pt1_y;
					ret = ATRC_END;
				}
				else if (curr_dist < *min_dist){
					*min_dist = curr_dist;
					*ret_x = pt1_x;
					*ret_y = pt1_y;
					ret = ATRC_END;
				}
			}
			curr_dist = sqrt(pow(pt2_x - pos_x, 2) + pow(pt2_y - pos_y, 2));
			if (curr_dist < sensi){
				if (*init_dist == 0){
					*init_dist = 1;
					*min_dist = curr_dist;
					*ret_x = pt2_x;
					*ret_y = pt2_y;
					ret = ATRC_END;
				}
				else if (curr_dist < *min_dist){
					*min_dist = curr_dist;
					*ret_x = pt2_x;
					*ret_y = pt2_y;
					ret = ATRC_END;
				}
			}
		}
		if(type & ATRC_QUAD){ /* if type of attractor is flaged as quadrant */
			
			double quad_x[4], quad_y[4], t_q;
			quad_x[0] = center_x + a*cosine;
			quad_y[0] = center_y + a*sine;
			
			//quad_x[1] = center_x - b*sine;
			//quad_y[1] = center_y + b*cosine;
			t_q = ellipse_par(M_PI/2, a, b);
			quad_x[1] = center_x + a*cos(t_q)*cosine - b*sin(t_q)*sine;
			quad_y[1] = center_y + a*cos(t_q)*sine + b*sin(t_q)*cosine;
			
			
			quad_x[2] = center_x - a*cosine;
			quad_y[2] = center_y - a*sine;
			
			//quad_x[3] = center_x + b*sine;
			//quad_y[3] = center_y - b*cosine;
			t_q = ellipse_par(3*M_PI/2, a, b);
			quad_x[3] = center_x + a*cos(t_q)*cosine - b*sin(t_q)*sine;
			quad_y[3] = center_y + a*cos(t_q)*sine + b*sin(t_q)*cosine;
			
			/* check if point pass on distance criteria */
			int i;
			for (i = 0; i < 4; i++){
				curr_dist = sqrt(pow(quad_x[i] - pos_x, 2) + pow(quad_y[i] - pos_y, 2));
				if (curr_dist < sensi){
					if (*init_dist == 0){
						*init_dist = 1;
						*min_dist = curr_dist;
						*ret_x = quad_x[i];
						*ret_y = quad_y[i];
						ret = ATRC_QUAD;
					}
					else if (curr_dist < *min_dist){
						*min_dist = curr_dist;
						*ret_x = quad_x[i];
						*ret_y = quad_y[i];
						ret = ATRC_QUAD;
					}
				}
			}
		}
		if ((type & ATRC_ANY) && (!ret)){ /* if type of attractor is flaged as center */
			/* check if point pass on distance criteria */
			curr_dist = sqrt(pow(near_x - pos_x, 2) + pow(near_y - pos_y, 2));
			if (curr_dist < sensi){
				if (*init_dist == 0){
					*init_dist = 1;
					*min_dist = curr_dist;
					*ret_x = near_x;
					*ret_y = near_y;
					ret = ATRC_ANY;
				}
				else if (curr_dist < *min_dist){
					*min_dist = curr_dist;
					*ret_x = near_x;
					*ret_y = near_y;
					ret = ATRC_ANY;
				}
			}
		}
		if ((type & ATRC_CENTER) && (!ret)){ /* if type of attractor is flaged as center */
			/* check if point pass on distance criteria */
			curr_dist = fabs(sqrt(pow(near_x - pos_x, 2) + pow(near_y - pos_y, 2)));
			if (curr_dist < sensi){
				if (*init_dist == 0){
					*init_dist = 1;
					*min_dist = curr_dist;
					*ret_x = center_x;
					*ret_y = center_y;
					ret = ATRC_CENTER;
				}
				else if (curr_dist < *min_dist){
					*min_dist = curr_dist;
					*ret_x = center_x;
					*ret_y = center_y;
					ret = ATRC_CENTER;
				}
			}
		}
	}
	return ret;
}

int dxf_inter_attract(struct inter_obj obj1, struct inter_obj obj2,
double pos_x, double pos_y, double sensi, 
double *ret_x, double *ret_y,
int *init_dist, double *min_dist){
	int ret = ATRC_NONE;
	double curr_dist = sensi;
	double inter_x = 0;
	double inter_y = 0;
	
	if ((obj1.type == DXF_LINE) && (obj2.type == DXF_LINE)){
			
		/* calcule the intersection point*/
		/*from https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection*/
		double den = (obj1.line.p1x - obj1.line.p2x) * (obj2.line.p1y - obj2.line.p2y) -
				(obj1.line.p1y - obj1.line.p2y) * (obj2.line.p1x - obj2.line.p2x);
		
		if (fabs(den) > TOL){
			inter_x = ((obj1.line.p1x*obj1.line.p2y - obj1.line.p1y*obj1.line.p2x)*(obj2.line.p1x - obj2.line.p2x) -
					(obj1.line.p1x - obj1.line.p2x)*(obj2.line.p1x*obj2.line.p2y - obj2.line.p1y*obj2.line.p2x)) / den;
			inter_y = ((obj1.line.p1x*obj1.line.p2y - obj1.line.p1y*obj1.line.p2x)*(obj2.line.p1y - obj2.line.p2y) -
					(obj1.line.p1y - obj1.line.p2y)*(obj2.line.p1x*obj2.line.p2y - obj2.line.p1y*obj2.line.p2x)) / den;
			/* verify if point is in segments */
			if ((IN_BOUNDS(inter_x, inter_y, obj1.line.p1x, obj1.line.p1y, obj1.line.p2x, obj1.line.p2y) ||
			NEAR_LN(inter_x, inter_y, obj1.line.p1x, obj1.line.p1y, obj1.line.p2x, obj1.line.p2y, sensi))&&
			(IN_BOUNDS(inter_x, inter_y, obj2.line.p1x, obj2.line.p1y, obj2.line.p2x, obj2.line.p2y) ||
			NEAR_LN(inter_x, inter_y, obj2.line.p1x, obj2.line.p1y, obj2.line.p2x, obj2.line.p2y, sensi))) {
				curr_dist = sqrt(pow(inter_x - pos_x, 2) + pow(inter_y - pos_y, 2));
			}
		}
	}
	if (curr_dist < sensi){
		if (*init_dist == 0){
			*init_dist = 1;
			*min_dist = curr_dist;
			*ret_x = inter_x;
			*ret_y = inter_y;
			ret = ATRC_INTER;
		}
		else if (curr_dist < *min_dist){
			*min_dist = curr_dist;
			*ret_x = inter_x;
			*ret_y = inter_y;
			ret = ATRC_INTER;
		}
	}
	return ret;
}

int dxf_ent_attract (dxf_drawing *drawing, dxf_node * obj_hilite, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y){
	dxf_node *current = NULL, *obj = NULL;
	dxf_node *prev = NULL;
	int ret = ATRC_NONE, found = 0;
	
	int init_dist = 0;
	double min_dist;
	
	enum dxf_graph ent_type = DXF_NONE;
	
	char name1[DXF_MAX_CHARS], name2[DXF_MAX_CHARS];
	name1[0] = 0; name2[0] = 0;
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	int pt1 = 0; /* flag */
	double t_rot = 0, rot = 0, elev = 0;
	double scale_x = 1.0, scale_y = 1.0, scale_z = 1.0;
	double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0;
	
	/* for insert objects */
	vector_p v_search;
	dxf_node *insert_ent = NULL, *blk = NULL;
	
	struct ins_space ins_stack[10];
	int ins_stack_pos = 0;
	
	struct ins_space ins_zero = {
		.ins_ent = obj, .prev = NULL,
		.ofs_x = 0.0, .ofs_y =0.0, .ofs_z =0.0,
		.rot = 0.0, .scale_x = 1.0 , .scale_y = 1.0, .scale_z = 1.0,
		.normal = {0.0, 0.0, 1.0}
	};
	ins_stack[0] = ins_zero;
	
	int ins_flag = 0;
	/* ---- */
	
	double rect_pt1[2]; double rect_pt2[2];
	int num_el = 0;
	
	rect_pt1[0] = pos_x - sensi;
	rect_pt1[1] = pos_y - sensi;
	rect_pt2[0] = pos_x + sensi;
	rect_pt2[1] = pos_y + sensi;
	
	list_node * list = list_new(NULL, 1);
	list_node *list_el = NULL;
	
	num_el = dxf_ents_isect2(list, drawing, rect_pt1, rect_pt2);
	
	if ((list != NULL) && (num_el > 0)){
		list_el = list->next;
	}
	
	struct inter_obj inter_cand[MAX_CAND];
	int num_inter = 0;
		
	
	while (list_el){/* ###### LIST LOOP ########### */
		
		obj = (dxf_node *)list_el->data; /* current obj */
		
		/* reset variables for complex entities (DXF_INSERT) */
		insert_ent = NULL;
		blk = NULL;
		ins_stack_pos = 0;
		ins_zero.ins_ent = obj;
		ins_stack[0] = ins_zero;
		ins_flag = 0;
		ent_type = DXF_NONE;
		name1[0] = 0; name2[0] = 0;
		pt1_x = 0; pt1_y = 0; pt1_z = 0;
		pt1 = 0;
		t_rot = 0; rot = 0; elev = 0;
		scale_x = 1.0; scale_y = 1.0; scale_z = 1.0;
		extru_x = 0.0; extru_y = 0.0; extru_z = 1.0;
		
		current = obj;
		while (current){ /* ########### OBJ LOOP ########*/
			prev = current;
			if (current->type == DXF_ENT){
				ent_type =  dxf_ident_ent_type (current);
				if (ent_type == DXF_LINE){
					double pt1_x = 0, pt1_y = 0, pt1_z = 0;
					double pt2_x = 0, pt2_y = 0, pt2_z = 0;
					
					if (dxf_line_get (drawing, current, &pt1_x, &pt1_y, &pt1_z, &pt2_x, &pt2_y, &pt2_z)){
						/* transform coordinates, according insert space */
						transform(&pt1_x, &pt1_y, ins_stack[ins_stack_pos]);
						transform(&pt2_x, &pt2_y, ins_stack[ins_stack_pos]);
					
						if (found = dxf_line_attract (pt1_x, pt1_y, pt2_x, pt2_y, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
							ret = found;
						}
						if ((type & ATRC_INTER) && (num_inter < MAX_CAND) &&
						(IN_BOUNDS(pos_x, pos_y, pt1_x, pt1_y, pt2_x, pt2_y) ||
						NEAR_LN(pos_x, pos_y, pt1_x, pt1_y, pt2_x, pt2_y, sensi))){
							
							inter_cand[num_inter].type = ent_type;
							inter_cand[num_inter].line.p1x = pt1_x;
							inter_cand[num_inter].line.p1y = pt1_y;
							inter_cand[num_inter].line.p2x = pt2_x;
							inter_cand[num_inter].line.p2y = pt2_y;
							inter_cand[num_inter].line.bulge = 0;
							num_inter++;
						}
					}
					//printf("line %d\n", found);
				}
				else if (ent_type == DXF_CIRCLE){
					double center_x, center_y, center_z, radius;
					double axis = 0, rot = 0, ratio = 1;
					
					if (dxf_circle_get(drawing, current, &center_x, &center_y, &center_z, &axis, &ratio, &rot)){
						/* transform coordinates, according insert space */
						transform(&center_x, &center_y, ins_stack[ins_stack_pos]);
						axis *= fabs(ins_stack[ins_stack_pos].scale_x);
						rot += ins_stack[ins_stack_pos].rot * M_PI/180;
						ratio *= fabs(ins_stack[ins_stack_pos].scale_y / ins_stack[ins_stack_pos].scale_x);
						ellipse_transf(&center_x, &center_y, &center_z, &axis, &ratio, &rot, ins_stack[ins_stack_pos].normal);
						if (found = dxf_arc_attract(axis, 0.0, 0.0, center_x, center_y, ratio, rot, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
							ret = found;
							//printf("ratio = %0.2f\n", ratio);
							//printf("pos = %d\n", ins_stack_pos);
						}
					}
				}
				else if (ent_type == DXF_ARC){
					double center_x, center_y, center_z, radius, start_ang, end_ang;
					double axis = 0, rot = 0, ratio = 1;
					//if (dxf_arc_get(drawing, current, &center_x, &center_y, &center_z, &radius, &start_ang, &end_ang)){
					if (dxf_arc_get(drawing, current, &center_x, &center_y, &center_z, &axis, &ratio, &rot, &start_ang, &end_ang)){
						/* transform coordinates, according insert space */
						transform(&center_x, &center_y, ins_stack[ins_stack_pos]);
						axis *= fabs(ins_stack[ins_stack_pos].scale_x);
						rot += ins_stack[ins_stack_pos].rot * M_PI/180;
						ratio *= fabs(ins_stack[ins_stack_pos].scale_y / ins_stack[ins_stack_pos].scale_x);
						ellipse_transf(&center_x, &center_y, &center_z, &axis, &ratio, &rot, ins_stack[ins_stack_pos].normal);
						if (found = dxf_arc_attract(axis, start_ang, end_ang, center_x, center_y, ratio, rot, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
							ret = found;
						}
					}
				}
				else if (ent_type == DXF_ELLIPSE){
					double center_x, center_y, center_z, radius, start_ang, end_ang;
					double axis = 0, rot = 0, ratio = 1;
					//if (dxf_arc_get(drawing, current, &center_x, &center_y, &center_z, &radius, &start_ang, &end_ang)){
					if (dxf_ellipse_get(drawing, current, &center_x, &center_y, &center_z, &axis, &ratio, &rot, &start_ang, &end_ang)){
						/* transform coordinates, according insert space */
						transform(&center_x, &center_y, ins_stack[ins_stack_pos]);
						axis *= fabs(ins_stack[ins_stack_pos].scale_x);
						rot += ins_stack[ins_stack_pos].rot * M_PI/180;
						ratio *= fabs(ins_stack[ins_stack_pos].scale_y / ins_stack[ins_stack_pos].scale_x);
						ellipse_transf(&center_x, &center_y, &center_z, &axis, &ratio, &rot, ins_stack[ins_stack_pos].normal);
						if (found = dxf_arc_attract(axis, start_ang, end_ang, center_x, center_y, ratio, rot, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
							ret = found;
						}
					}
				}
				else if (ent_type ==  DXF_INSERT){
					insert_ent = current;
					ins_flag = 1;
					//printf("insert %d\n", current->obj.content);
					if (current->obj.content){
						/* starts the content sweep */
						current = current->obj.content->next;
						//prev = current;
						continue;
					}
					printf("Error: empty entity\n");
				}
				else if (ent_type == DXF_LWPOLYLINE){
					double pt1_x = 0, pt1_y = 0, pt1_z = 0, bulge = 0;
					double pt2_x = 0, pt2_y = 0, pt2_z = 0, prev_bulge = 0;
					dxf_node * next_vert = NULL;
					if(dxf_lwpline_get_pt(drawing, current, &next_vert, &pt2_x, &pt2_y, &pt2_z, &bulge)){
						//printf("%0.f,%0.2f  -  %d\n",pt2_x, pt2_y, next_vert);
						/* transform coordinates, according insert space */
						while (next_vert){
							transform(&pt2_x, &pt2_y, ins_stack[ins_stack_pos]);
							pt1_x = pt2_x; pt1_y = pt2_y; prev_bulge = bulge;
							
							if(!dxf_lwpline_get_pt(drawing, current, &next_vert, &pt2_x, &pt2_y, &pt2_z, &bulge)){
								break;
							}
							
							if (fabs(prev_bulge) < TOL){ /* segment is a straight line*/
								if (found = dxf_line_attract (pt1_x, pt1_y, pt2_x, pt2_y, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
									ret = found;
								}
								if ((type & ATRC_INTER) && (num_inter < MAX_CAND) &&
								(IN_BOUNDS(pos_x, pos_y, pt1_x, pt1_y, pt2_x, pt2_y) ||
								NEAR_LN(pos_x, pos_y, pt1_x, pt1_y, pt2_x, pt2_y, sensi))){
									
									inter_cand[num_inter].type = DXF_LINE;
									inter_cand[num_inter].line.p1x = pt1_x;
									inter_cand[num_inter].line.p1y = pt1_y;
									inter_cand[num_inter].line.p2x = pt2_x;
									inter_cand[num_inter].line.p2y = pt2_y;
									inter_cand[num_inter].line.bulge = 0;
									num_inter++;
								}
							}
							else{ /* segment is an arc*/
								double radius, ang_start, ang_end, center_x, center_y, center_z = 0.0;
								double axis = 0, rot = 0, ratio = 1;
								arc_bulge(pt1_x, pt1_y, pt2_x, pt2_y, prev_bulge, &radius, &ang_start, &ang_end, &center_x, &center_y);
								/* transform coordinates, according insert space */
								transform(&center_x, &center_y, ins_stack[ins_stack_pos]);
								axis = radius * fabs(ins_stack[ins_stack_pos].scale_x);
								rot = ins_stack[ins_stack_pos].rot * M_PI/180;
								ratio = fabs(ins_stack[ins_stack_pos].scale_y / ins_stack[ins_stack_pos].scale_x);
								ellipse_transf(&center_x, &center_y, &center_z, &axis, &ratio, &rot, ins_stack[ins_stack_pos].normal);
								if (found = dxf_arc_attract(axis, ang_start, ang_end, center_x, center_y, ratio, rot, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
									ret = found;
								}
							}
						}
					}
				}
				
			}
			/* ============================================================= */
			else if ((current->type == DXF_ATTR) && (ins_flag != 0)){ /* read DXF attibutes of insert block */
				//printf("%d\n", current->value.group);
				switch (current->value.group){
					case 2:
						strcpy(name1, current->value.s_data);
						break;
					case 3:
						strcpy(name2, current->value.s_data);
						break;
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 41:
						scale_x = current->value.d_data;
						break;
					case 42:
						scale_y = current->value.d_data;
						break;
					case 43:
						scale_z = current->value.d_data;
						break;
					case 50:
						t_rot = current->value.d_data;
						break;
					case 210:
						extru_x = current->value.d_data;
						break;
					case 220:
						extru_y = current->value.d_data;
						break;
					case 230:
						extru_z = current->value.d_data;
						break;
				}
				
				
			}
				
			current = current->next; /* go to the next in the list*/
			
			/* ============================================================= */
			/* complex entities */
			
			if (((ins_flag != 0) && (current == NULL))||
				((ins_flag != 0) && (current != NULL) && (current != insert_ent) && (current->type == DXF_ENT))){
				ins_flag = 0;
				/* look for block */
				v_search = dxf_find_obj_descr(drawing->blks, "BLOCK", name1);
				if (v_search.data) { 
					blk = ((dxf_node **) v_search.data)[0];
					//printf ("bloco %s\n", name1);
					free(v_search.data);
					/* save current entity for future process */
					ins_stack_pos++;
					ins_stack[ins_stack_pos].ins_ent = blk;
					ins_stack[ins_stack_pos].prev = prev;
					
					if (ins_stack_pos > 1){
						ins_stack[ins_stack_pos].ofs_x = pt1_x + ins_stack[ins_stack_pos - 1].ofs_x;
						ins_stack[ins_stack_pos].ofs_y = pt1_y + ins_stack[ins_stack_pos - 1].ofs_y;
						ins_stack[ins_stack_pos].ofs_z = pt1_z + ins_stack[ins_stack_pos - 1].ofs_z;
						ins_stack[ins_stack_pos].scale_x = scale_x * ins_stack[ins_stack_pos - 1].scale_x;
						ins_stack[ins_stack_pos].scale_y = scale_y * ins_stack[ins_stack_pos - 1].scale_y;
						ins_stack[ins_stack_pos].scale_z = scale_z * ins_stack[ins_stack_pos - 1].scale_z;
						ins_stack[ins_stack_pos].rot = t_rot + ins_stack[ins_stack_pos - 1].rot;
						ins_stack[ins_stack_pos].normal[0] = extru_x;
						ins_stack[ins_stack_pos].normal[1] = extru_y;
						ins_stack[ins_stack_pos].normal[2] = extru_z;
					}
					else{ 
						ins_stack[ins_stack_pos].ofs_x = pt1_x;
						ins_stack[ins_stack_pos].ofs_y = pt1_y;
						ins_stack[ins_stack_pos].ofs_z = pt1_z;
						ins_stack[ins_stack_pos].scale_x = scale_x;
						ins_stack[ins_stack_pos].scale_y = scale_y;
						ins_stack[ins_stack_pos].scale_z = scale_z;
						ins_stack[ins_stack_pos].rot = t_rot;
						ins_stack[ins_stack_pos].normal[0] = extru_x;
						ins_stack[ins_stack_pos].normal[1] = extru_y;
						ins_stack[ins_stack_pos].normal[2] = extru_z;
					}
					/*
					if (v_return->size > 0){
						ins_stack[ins_stack_pos].start_idx = v_return->size;
					}
					else{
						ins_stack[ins_stack_pos].start_idx = 0;
					}*/
					
					
					//p_space = paper;
					
					/*reinit_vars: */
					
					ent_type = DXF_NONE;
						
					pt1_x = 0; pt1_y = 0; pt1_z = 0; rot = 0;
					elev = 0; t_rot = 0;
					scale_x = 1.0; scale_y = 1.0; scale_z = 1.0;
					extru_x = 0.0; extru_y = 0.0; extru_z = 1.0;
					
					/* clear the strings */
					name1[0] = 0;
					name2[0] = 0;
					
					/*clear flags*/
					pt1 = 0;
					
					if (blk->obj.content){
						/* now, current is the block */
						/* starts the content sweep */
						current = blk->obj.content->next;
						continue;
					}
					printf("Error: empty block\n");
					continue;
				}
			}
			
			if ((prev == NULL) || (prev == obj)){ /* stop the search if back on initial entity */
				current = NULL;
				break;
			}
			
			
			/* ============================================================= */
			while (current == NULL){
				/* end of list sweeping */
				if ((prev == NULL) || (prev == obj)){ /* stop the search if back on initial entity */
					//printf("para\n");
					current = NULL;
					break;
				}
				/* try to back in structure hierarchy */
				prev = prev->master;
				if (prev){ /* up in structure */
					/* try to continue on previous point in structure */
					current = prev->next;
					if (prev == ins_stack[ins_stack_pos].ins_ent){/* back on initial entity */
						if (ins_stack_pos < 1){
							/* stop the search if back on initial entity */
							current = NULL;
							break;
						}
						else{
							prev = ins_stack[ins_stack_pos].prev;
							ins_stack_pos--;
							//prev = ins_stack[ins_stack_pos].ins_ent;
							//printf("retorna %d\n", ins_stack_pos);
							current = prev;
						}
					}
					
				}
				else{ /* stop the search if structure ends */
					current = NULL;
					break;
				}
			}
		} /* ######### END OBJ LOOP ########### */
		
		list_el = list_el->next;
		
	}/* ######### END LIST LOOP ############ */
	list_mem_pool(ZERO_LIST, 1);
	
	/* verify if exist intersections between candidates*/
	if(num_inter > 1){
		int i, j;
		/* walk in all combinations */
		for (i = 0; i < num_inter - 1; i++){
			for (j = i+1; j < num_inter; j++){
				if (found = dxf_inter_attract (inter_cand[i], inter_cand[j], pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist)){
					ret = found;
				}
			}
		}
	}
	//printf("%d\n", ret);
	return ret;
}