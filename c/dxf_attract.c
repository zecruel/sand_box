
#include "dxf_attract.h"

int dxf_line_attract (dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y,
int *init_dist, double *min_dist){
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
	
	return ret;
}

int dxf_ent_attract (dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y){
	dxf_node *current = NULL;
	dxf_node *prev = NULL;
	int ret = 0, first = 0, pt = 0, init_dist = 0;
	enum dxf_graph ent_type = DXF_NONE;
	double curr_x, curr_y, curr_z, next_x, curr_dist, min_dist;
	
	current = obj;
	
	while (current){
		prev = current;
		if (obj->type == DXF_ENT){
			ent_type =  dxf_ident_ent_type (obj);
			switch (ent_type){
				case DXF_LINE:
					ret = dxf_line_attract (obj, type, pos_x, pos_y, sensi, ret_x, ret_y, &init_dist, &min_dist);
					break;
				default:
					break;
			}
		}
		
			
		current = current->next; /* go to the next in the list*/
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
				
			}
			else{ /* stop the search if structure ends */
				current = NULL;
				break;
			}
		}
	}
	return ret;
}