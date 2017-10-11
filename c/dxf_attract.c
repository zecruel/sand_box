
#include "dxf_attract.h"

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

int transform(double *x, double *y, struct ins_space space){
	if ((x != NULL) && (y != NULL)){
		double x0, y0;
		double x_axis[3], y_axis[3], point[3];
		double wy_axis[3] = {0.0, 1.0, 0.0};
		double wz_axis[3] = {0.0, 0.0, 1.0};
		
		/* rotation constants */
		double cosine = cos(space.rot * M_PI/180);
		double sine = sin(space.rot * M_PI/180);
		
		/* scale and translation transformations */
		*x = *x * space.scale_x + space.ofs_x;
		*y = *y * space.scale_y + space.ofs_y;
		
		/* rotation transform */ 
		x0 = cosine*(*x - space.ofs_x) - sine*(*y - space.ofs_y) + space.ofs_x;
		y0 = sine*(*x - space.ofs_x) + cosine*(*y - space.ofs_y) + space.ofs_y;
		
		/* axis tranform */
		if ((fabs(space.normal[0] < 0.015625)) && (fabs(space.normal[1] < 0.015625))){
			cross_product(wy_axis, space.normal, x_axis);
		}
		else{
			cross_product(wz_axis, space.normal, x_axis);
		}
		unit_vector(x_axis);
		cross_product(space.normal, x_axis, y_axis);
		unit_vector(y_axis);
		
		point[0] = x0;
		point[1] = y0;
		point[2] = 0;
		x0 = dot_product(point, x_axis);
		y0 = dot_product(point, y_axis);

		/* update return values*/ 
		*x = x0;
		*y = y0;
		
		return 1;
	}
	return 0;
}

int dxf_line_attract(dxf_drawing *drawing, dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y,
int *init_dist, double *min_dist, struct ins_space space){
	dxf_node *current = NULL;
	int ret = 0;
	double curr_dist;
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double pt2_x = 0, pt2_y = 0, pt2_z = 0;
	
	/*flags*/
	int pt1 = 0, pt2 = 0;
	
	if (obj){ 
		if (obj->type == DXF_ENT){
			if (obj->obj.content){
				current = obj->obj.content->next;
			}
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
			}
		}
		current = current->next; /* go to the next in the list */
	}
	
	/* transform coordinates, according insert space */
	if (pt1) transform(&pt1_x, &pt1_y, space);
	if (pt2) transform(&pt2_x, &pt2_y, space);
	
	if(type & ATRC_END){ /* if type of attractor is flaged as endpoint */
		/* check if points of the line pass on distance criteria */
		if (pt1){ /* found point 1 */
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
		}
		if (pt2){ /* found point 2 */
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
	}
	if(type & ATRC_MID){ /* if type of attractor is flaged as midpoint */
		
	}
	
	return ret;
}

int dxf_ent_attract (dxf_drawing *drawing, dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y){
	dxf_node *current = NULL;
	dxf_node *prev = NULL;
	int ret = 0, found = 0;
	
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
	
	current = obj;
	
	while (current){
		prev = current;
		if (current->type == DXF_ENT){
			ent_type =  dxf_ident_ent_type (current);
			if (ent_type == DXF_LINE){
				if (found = dxf_line_attract (drawing, current, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist, ins_stack[ins_stack_pos])){
					ret = found;
				}
				//printf("line %d\n", found);
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
	}
	//printf("%d\n", ret);
	return ret;
}