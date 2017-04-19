#include "dxf_graph.h"
#include "shape2.h"

#include "dxf_colors.h"
#include <string.h>


graph_obj * dxf_line_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		double tick = 0, elev = 0;
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0, pt2 = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
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
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
						break;
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			graph_obj *curr_graph = graph_new();
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* add the line */
				line_add(curr_graph, pt1_x, pt1_y, pt2_x, pt2_y);
			}
			return curr_graph;
		}
	}
	return NULL;
}

graph_obj * dxf_circle_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double radius, tick = 0, elev = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
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
					case 40:
						radius = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			graph_obj *curr_graph = graph_new();
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* add the graph */
				graph_arc(curr_graph, pt1_x, pt1_y, radius, 0.0, 0.0, 1);
				
				/* convert OCS to WCS */
				normal[0] = extru_x;
				normal[1] = extru_y;
				normal[2] = extru_z;
				graph_mod_axis(curr_graph, normal);
			}
			return curr_graph;
		}
	}
	return NULL;
}

graph_obj * dxf_arc_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double radius, tick = 0, elev = 0;
		double start_ang = 0.0, end_ang = 0.0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
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
					case 40:
						radius = current->value.d_data;
						break;
					case 50:
						start_ang = current->value.d_data;
						break;
					case 51:
						end_ang = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			graph_obj *curr_graph = graph_new();
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* add the graph */
				graph_arc(curr_graph, pt1_x, pt1_y, radius, start_ang, end_ang, 1);
				
				/* convert OCS to WCS */
				normal[0] = extru_x;
				normal[1] = extru_y;
				normal[2] = extru_z;
				graph_mod_axis(curr_graph, normal);
			}
			return curr_graph;
		}
	}
	return NULL;
}

graph_obj * dxf_ellipse_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		double minor_ax, tick = 0, elev = 0;
		double start_ang = 0.0, end_ang = 0.0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0, pt2 = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
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
						minor_ax = current->value.d_data;
						break;
					case 41:
						start_ang = current->value.d_data;
						break;
					case 42:
						end_ang = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			graph_obj *curr_graph = graph_new();
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* add the graph */
				graph_ellipse(curr_graph, pt1_x, pt1_y, pt2_x, pt2_y, minor_ax, start_ang, end_ang);
				
				/* convert OCS to WCS */
				normal[0] = extru_x;
				normal[1] = extru_y;
				normal[2] = extru_z;
				graph_mod_axis(curr_graph, normal);
			}
			return curr_graph;
		}
	}
	return NULL;
}

graph_obj * dxf_pline_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL, *prev;
		graph_obj *curr_graph = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double start_w = 0, end_w = 0;
		double tick = 0, elev = 0, bulge = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		int pline_flag = 0;
		int first = 0, closed =0;
		double prev_x, prev_y, last_x, last_y;
		double prev_bulge = 0;
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0, init = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			prev = current;
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
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
					case 40:
						start_w = current->value.d_data;
						break;
					case 41:
						end_w = current->value.d_data;
						break;
					case 42:
						bulge = current->value.d_data;
						break;
					case 70:
						pline_flag = current->value.i_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			else if (current->type == DXF_ENT){
				if((init == 0) &&
				(((p_space == 0) && (paper == 0)) || 
				((p_space != 0) && (paper != 0)))){
					init = 1;
					curr_graph = graph_new();
					if (curr_graph){
						//printf("primeiro\n");
						/* find the layer index */
						lay_idx = dxf_lay_idx(drawing, layer);
						
						/* check if  object's color  is definied by layer,
						then look for layer's color */
						if (color >= 256){
							color = drawing.layers[lay_idx].color;
						}
						
						/* check if  object's ltype  is definied by layer,
						then look for layer's ltype */
						if ((strcmp(l_type, "BYLAYER") == 0) ||
							((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
							strcpy(l_type, drawing.layers[lay_idx].ltype);
						}
						
						/* find the ltype index */
						ltype_idx = dxf_ltype_idx(drawing, l_type);
						
						/* change the graph line pattern */
						curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
						for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
							curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
						}
						
						/*change the color */
						curr_graph->color = dxf_colors[color];
						
						pt1_x = 0; pt1_y = 0; pt1_z = 0;
						bulge =0;
						
						if (pline_flag & 1){
							closed = 1;
						}
						else {
							closed = 0;
						}
						
						/* convert OCS to WCS */
						normal[0] = extru_x;
						normal[1] = extru_y;
						normal[2] = extru_z;
						//graph_mod_axis(curr_graph, normal);
					}
				}
				
				if ((strcmp(current->obj.name, "VERTEX") == 0) && (current->obj.content)){
					current = current->obj.content->next;
					//printf("vertice\n");
					continue;
				}
			}
			current = current->next; /* go to the next in the list */
		
		
			if (current == NULL){
				
				if((first != 0) && (curr_graph != NULL)){
					//printf("(%0.2f, %0.2f)-(%0.2f, %0.2f)\n", prev_x, prev_y, pt1_x, pt1_y);
					if (prev_bulge == 0){
						line_add(curr_graph, prev_x, prev_y, pt1_x, pt1_y);
					}
					else{
						graph_arc_bulge(curr_graph, prev_x, prev_y, pt1_x, pt1_y, prev_bulge);
					}
				}
				else if(first == 0){
					first = 1;
					
					//printf("primeiro vertice\n");
					last_x = pt1_x;
					last_y = pt1_y;
				}
				prev_x = pt1_x;
				prev_y = pt1_y;
				prev_bulge = bulge;
				
				pt1_x = 0; pt1_y = 0; pt1_z = 0;
				bulge =0;
			}
		
			while (current == NULL){
				
				prev = prev->master;
				if (prev){ /* up in structure */
					
					/* ====== close complex entities ============== */
					if (prev == ent){ /* back on polyline ent */
						if((closed != 0) && (curr_graph != NULL)){
							if (prev_bulge == 0){
								line_add(curr_graph, prev_x, prev_y, last_x, last_y);
							}
							else{
								graph_arc_bulge(curr_graph, prev_x, prev_y, last_x, last_y, prev_bulge);
							}
						}
						//printf("fim\n");
						
						/* convert OCS to WCS */
						graph_mod_axis(curr_graph, normal);
						
						break;
					}
					/* try to continue on previous point in structure */
					current = prev->next;
				}
			}
		}
		
		return curr_graph;
	}
	return NULL;
}

graph_obj * dxf_lwpline_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL, *prev;
		graph_obj *curr_graph = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double start_w = 0, end_w = 0;
		double tick = 0, elev = 0, bulge = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		int pline_flag = 0;
		int first = 0, closed =0;
		double prev_x, prev_y, last_x, last_y, curr_x;
		double prev_bulge = 0;
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i;
		
		/*flags*/
		int pt1 = 0, init = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			prev = current;
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 40:
						start_w = current->value.d_data;
						break;
					case 41:
						end_w = current->value.d_data;
						break;
					case 42:
						bulge = current->value.d_data;
						break;
					case 70:
						pline_flag = current->value.i_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			if (pt1){
				pt1 = 0;
				
				if((init != 0) &&(first == 0)){
					first = 1;
					
					//printf("primeiro vertice\n");
					last_x = curr_x;
					last_y = pt1_y;
					prev_x = curr_x;
					prev_y = pt1_y;
				}
				else if((init == 0) &&
				(((p_space == 0) && (paper == 0)) || 
				((p_space != 0) && (paper != 0)))){
					init = 1;
					curr_graph = graph_new();
					if (curr_graph){
						//printf("primeiro\n");
						/* find the layer index */
						lay_idx = dxf_lay_idx(drawing, layer);
						
						/* check if  object's color  is definied by layer,
						then look for layer's color */
						if (color >= 256){
							color = drawing.layers[lay_idx].color;
						}
						
						/* check if  object's ltype  is definied by layer,
						then look for layer's ltype */
						if ((strcmp(l_type, "BYLAYER") == 0) ||
							((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
							strcpy(l_type, drawing.layers[lay_idx].ltype);
						}
						
						/* find the ltype index */
						ltype_idx = dxf_ltype_idx(drawing, l_type);
						
						/* change the graph line pattern */
						curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
						for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
							curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
						}
						
						/*change the color */
						curr_graph->color = dxf_colors[color];
						
						//pt1_x = 0; pt1_y = 0; pt1_z = 0;
						//bulge =0;
						
						if (pline_flag & 1){
							closed = 1;
						}
						else {
							closed = 0;
						}
					}
				}
				else if((first != 0) && (curr_graph != NULL)){
					//printf("(%0.2f, %0.2f)-(%0.2f, %0.2f)\n", prev_x, prev_y, curr_x, pt1_y);
					if (prev_bulge == 0){
						line_add(curr_graph, prev_x, prev_y, curr_x, pt1_y);
					}
					else{
						graph_arc_bulge(curr_graph, prev_x, prev_y, curr_x, pt1_y, prev_bulge);
						//bulge =0;
					}
					prev_x = curr_x;
					prev_y = pt1_y;
				}
				
				prev_bulge = bulge;
				bulge = 0;
				
				curr_x = pt1_x;
			}
			current = current->next; /* go to the next in the list */
		}
		
		/* last vertex */
		if((first != 0) && (curr_graph != NULL)){
			//printf("(%0.2f, %0.2f)-(%0.2f, %0.2f)\n", prev_x, prev_y, curr_x, pt1_y);
			if (prev_bulge == 0){
				line_add(curr_graph, prev_x, prev_y, curr_x, pt1_y);
			}
			else{
				graph_arc_bulge(curr_graph, prev_x, prev_y, curr_x, pt1_y, prev_bulge);
				//bulge =0;
			}
			prev_x = curr_x;
			prev_y = pt1_y;
		}
		
		if((closed != 0) && (curr_graph != NULL)){
			if (bulge == 0){
				line_add(curr_graph, prev_x, prev_y, last_x, last_y);
			}
			else{
				graph_arc_bulge(curr_graph, prev_x, prev_y, last_x, last_y, bulge);
			}
		}
		
		/* convert OCS to WCS */
		normal[0] = extru_x;
		normal[1] = extru_y;
		normal[2] = extru_z;
		graph_mod_axis(curr_graph, normal);
		
		return curr_graph;
	}
	return NULL;
}


graph_obj * dxf_spline_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL, *prev;
		graph_obj *curr_graph = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double tick = 0, elev = 0;
		
		int pline_flag = 0, closed = 0;
		double prev_x, prev_y, curr_x;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, i, count;
		
		/*flags*/
		int pt1 = 0, init = 0;
		
		int num_cpts, order, num_ret, num_knots;
		double weight = 1.0;
		double ctrl_pts[3 * MAX_SPLINE_PTS], ret[3 * MAX_SPLINE_PTS];
		double weights[MAX_SPLINE_PTS], knots[MAX_SPLINE_PTS];
		int knot_count = 1;

		count =0;
		
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			prev = current;
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
					case 10:
						pt1_x = current->value.d_data;
						pt1 = 1; /* set flag */
						break;
					case 20:
						pt1_y = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 30:
						pt1_z = current->value.d_data;
						//pt1 = 1; /* set flag */
						break;
					case 40:
						knots[knot_count] = current->value.d_data;
						knot_count++;
						break;
					case 41:
						weight = current->value.d_data;
						break;
					case 70:
						pline_flag = current->value.i_data;
						break;
					case 71:
						order = current->value.i_data;
						break;
					case 72:
						num_knots = current->value.i_data;
						break;
					case 73:
						num_cpts = current->value.i_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			if (pt1){
				pt1 = 0;
				
				if ((init != 0) && (count < MAX_SPLINE_PTS)){
					ctrl_pts[count*3+1] = curr_x;
					ctrl_pts[count*3+2] = pt1_y;
					ctrl_pts[count*3+3] = pt1_z;
					weights[count+1] = weight;
					count++;
				}
				else if((init == 0) &&
				(((p_space == 0) && (paper == 0)) || 
				((p_space != 0) && (paper != 0)))){
					init = 1;
				}
				
				curr_x = pt1_x;
			}
			current = current->next; /* go to the next in the list */
		}
		
		/* last vertex */
		if ((init != 0) && (count < MAX_SPLINE_PTS)){
			ctrl_pts[count*3+1] = curr_x;
			ctrl_pts[count*3+2] = pt1_y;
			ctrl_pts[count*3+3] = 0; //pt1_z
			weights[count+1] = weight;
			count++;
		}
		
		curr_graph = graph_new();
		if ((curr_graph)&&((count + order)*5 < MAX_SPLINE_PTS)){
			//printf("primeiro\n");
			/* find the layer index */
			lay_idx = dxf_lay_idx(drawing, layer);
			
			/* check if  object's color  is definied by layer,
			then look for layer's color */
			if (color >= 256){
				color = drawing.layers[lay_idx].color;
			}
			
			/* check if  object's ltype  is definied by layer,
			then look for layer's ltype */
			if ((strcmp(l_type, "BYLAYER") == 0) ||
				((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
				strcpy(l_type, drawing.layers[lay_idx].ltype);
			}
			
			/* find the ltype index */
			ltype_idx = dxf_ltype_idx(drawing, l_type);
			
			/* change the graph line pattern */
			curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
			for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
				curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
			}
			
			/*change the color */
			curr_graph->color = dxf_colors[color];
			
			//pt1_x = 0; pt1_y = 0; pt1_z = 0;
			//bulge =0;
			
			if (pline_flag & 1){
				closed = 1;
			}
			else {
				closed = 0;
			}
		
			num_ret = (num_cpts + order)*5; /* num pts on curve */
			
			for(i = 1; i <= 3*num_ret; i++){
				ret[i] = 0.0;
			}
			
			rbspline(num_cpts, order+1, num_ret, ctrl_pts, weights, ret);
			
			prev_x = ret[1];
			prev_y = ret[2];
			
			for(i =4 ; i <= 3*num_ret; i = i+3){
				line_add(curr_graph, prev_x, prev_y, ret[i], ret[i+1]);
				prev_x = ret[i];
				prev_y = ret[i+1];
				/*printf(" %f %f %f \n",ret[i],ret[i+1],ret[i+2]);*/
			}
			/* convert OCS to WCS */
			normal[0] = extru_x;
			normal[1] = extru_y;
			normal[2] = extru_z;
			graph_mod_axis(curr_graph, normal);
		}
		return curr_graph;
	}
	return NULL;
}



graph_obj * dxf_text_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		graph_obj *curr_graph = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0, normal[3];
		
		shape *shx_font = NULL;
		
		double t_size = 0, t_rot = 0;
		
		int t_alin_v = 0, t_alin_h = 0;
		
		double fnt_size, fnt_above, fnt_below, txt_size;
		double t_pos_x, t_pos_y, t_center_x = 0, t_center_y = 0, t_base_x = 0, t_base_y = 0;
		double t_scale_x = 1, t_scale_y = 1, txt_w, txt_h;
		
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		char text[DXF_MAX_CHARS], t_style[DXF_MAX_CHARS];
		char tmp_str[DXF_MAX_CHARS];
		char *pos_st, *pos_curr, *pos_tmp, special;
		
		double tick = 0, elev = 0;
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, fnt_idx, i;
		
		/*flags*/
		int pt1 = 0, pt2 = 0;
		int under_l, over_l;
		
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		text[0] = 0;
		t_style[0] = 0;
		tmp_str[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 1:
						strcpy(text, current->value.s_data);
						break;
					case 7:
						strcpy(t_style, current->value.s_data);
						break;
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
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
						t_size = current->value.d_data;
						break;
					case 41:
						t_scale_x = current->value.d_data;
						break;
					case 50:
						t_rot = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
						break;
					case 72:
						t_alin_h = current->value.i_data;
						break;
					case 73:
						t_alin_v = current->value.i_data;
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			
			/* find the font index and font*/
			fnt_idx = dxf_font_idx(drawing, t_style);
			shx_font = drawing.text_fonts[fnt_idx].shx_font;
			
			if(shx_font == NULL){ /* if font not loaded*/
				/* use the deafault font*/
				shx_font = drawing.text_fonts[0].shx_font;
			}
			
			/* find the dimentions of SHX font */
			if(shx_font){ /* if the font exists */
				if(shx_font->next){ /* the font descriptor is stored in first iten of list */
					if(shx_font->next->cmd_size > 1){ /* check if the font is valid */
						fnt_above = shx_font->next->cmds[0]; /* size above the base line of text */
						fnt_below = shx_font->next->cmds[1]; /* size below the base line of text */
						if((fnt_above + fnt_below) > 0){
							fnt_size = fnt_above + fnt_below;
						}
					}
				}
			}
			
			/* find and replace special symbols in the text*/
			under_l = 0; /* under line flag*/
			over_l = 0; /* over line flag*/
			pos_curr = strstr(text, "%%");
			pos_st = text;
			pos_tmp = tmp_str;
			while (pos_curr){
				/* copy the part of text until the control string */
				strncpy(pos_tmp, pos_st, pos_curr - pos_st);
				/*control string is stripped in new string */
				pos_tmp += pos_curr - pos_st;
				/*get the control character */
				special = *(pos_curr + 2);
				/* verify the action to do */
				switch (special){
					/* put the  diameter simbol (unicode D8 Hex) in text*/
					case 'c':
						pos_tmp += wctomb(pos_tmp, L'\xd8');
						break;
					case 'C':
						pos_tmp += wctomb(pos_tmp, L'\xd8');
						break;
					/* put the degrees simbol in text*/
					case 'd':
						pos_tmp += wctomb(pos_tmp, L'\xb0');
						break;
					case 'D':
						pos_tmp += wctomb(pos_tmp, L'\xb0');
						break;
					/* put the plus/minus tolerance simbol in text*/
					case 'p':
						pos_tmp += wctomb(pos_tmp, L'\xb1');
						break;
					case 'P':
						pos_tmp += wctomb(pos_tmp, L'\xb1');
						break;
					/* under line */
					case 'u':
						under_l = 1;
						break;
					case 'U':
						under_l = 1;
						break;
					/* over line */
					case 'o':
						over_l = 1;
						break;
					case 'O':
						over_l = 1;
						break;
				}
				/*try to find new  control sequences in the rest of text*/
				pos_curr += 3;
				pos_st = pos_curr;
				pos_curr = strstr(pos_curr, "%%");
			}
			/* copy the rest of text after the last control string */
			strcpy(pos_tmp, pos_st);
			//printf("%s\n", tmp_str);
			
			curr_graph = shx_font_parse(shx_font, tmp_str);
			
			
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* find the dimentions of text */
				txt_size = t_size/fnt_size;
				txt_w = fabs(curr_graph->ext_max_x - curr_graph->ext_min_x);
				txt_h = fabs(curr_graph->ext_max_y - curr_graph->ext_min_y);
				
				if (under_l){
					/* add the under line */
					line_add(curr_graph, 
						curr_graph->ext_min_x,
						(double)fnt_size * -0.1,
						curr_graph->ext_max_x, 
						(double)fnt_size * -0.1);
				}
				if (over_l){
					/* add the over line */
					line_add(curr_graph, 
						curr_graph->ext_min_x,
						(double)fnt_size * 1.1,
						curr_graph->ext_max_x, 
						(double)fnt_size * 1.1);
				}
				
				/* find the insert point of text, in function of its aling */
				if(t_alin_h < 3){
					t_center_x = (double)t_alin_h * (t_scale_x*txt_w * txt_size/2);
					t_base_x =  (double)t_alin_h * (pt2_x - pt1_x)/2;
					t_base_y =  (double)t_alin_h * (pt2_y - pt1_y)/2;
				}
				else{ 
					if(t_alin_h == 4){
						t_base_x = (pt2_x - pt1_x)/2;
						t_base_y = (pt2_y - pt1_y)/2;
					}
					else{
						t_scale_x = sqrt(pow((pt2_x - pt1_x), 2) + pow((pt2_y - pt1_y), 2))/(txt_w * txt_size);
					}
					//rot = atan2((pt2_y - pt1_y),(pt2_x - pt1_x)) * 180/M_PI;
					
					//printf("alinhamento=%d\n", t_alin_h);
				}
				if(t_alin_v >0){
					t_center_y = (double)(t_alin_v - 1) * (txt_size/2);
				}
				
				t_pos_x = pt1_x + t_base_x - t_center_x;
				t_pos_y = pt1_y + t_base_y - t_center_y;
				
				/* apply the scales, offsets and rotation to graphs */
				graph_modify(curr_graph, t_pos_x, t_pos_y, t_scale_x*txt_size, txt_size, t_rot);
				
				/* convert OCS to WCS */
				normal[0] = extru_x;
				normal[1] = extru_y;
				normal[2] = extru_z;
				graph_mod_axis(curr_graph, normal);
				
			}
			return curr_graph;
		}
	}
	return NULL;
}

graph_obj * dxf_attrib_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	if(ent){
		dxf_node *current = NULL;
		graph_obj *curr_graph = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		
		shape *shx_font = NULL;
		
		double t_size = 0, t_rot = 0;
		
		int t_alin_v = 0, t_alin_h = 0;
		
		double fnt_size, fnt_above, fnt_below, txt_size;
		double t_pos_x, t_pos_y, t_center_x = 0, t_center_y = 0, t_base_x = 0, t_base_y = 0;
		double t_scale_x = 1, t_scale_y = 1, txt_w, txt_h;
		
		
		char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS];
		char comment[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
		char text[DXF_MAX_CHARS], t_style[DXF_MAX_CHARS];
		
		double tick = 0, elev = 0;
		int color = 256, paper = 0;
		int lay_idx, ltype_idx, fnt_idx, i;
		
		/*flags*/
		int pt1 = 0, pt2 = 0;
		
		/* clear the strings */
		handle[0] = 0;
		l_type[0] = 0;
		layer[0] = 0;
		comment[0] = 0;
		text[0] = 0;
		t_style[0] = 0;
		
		if (ent->type == DXF_ENT){
			if (ent->obj.content){
				current = ent->obj.content->next;
				//printf("%s\n", ent->obj.name);
			}
		}
		while (current){
			if (current->type == DXF_ATTR){ /* DXF attibute */
				switch (current->value.group){
					case 1:
						strcpy(text, current->value.s_data);
						break;
					case 7:
						strcpy(t_style, current->value.s_data);
						break;
					case 5:
						strcpy(handle, current->value.s_data);
						break;
					case 6:
						strcpy(l_type, current->value.s_data);
						break;
					case 8:
						strcpy(layer, current->value.s_data);
						break;
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
						t_size = current->value.d_data;
						break;
					case 41:
						t_scale_x = current->value.d_data;
						break;
					case 50:
						t_rot = current->value.d_data;
						break;
					case 38:
						elev = current->value.d_data;
						break;
					case 39:
						tick = current->value.d_data;
						break;
					case 62:
						color = current->value.i_data;
						break;
					case 67:
						paper = current->value.i_data;
						break;
					case 72:
						t_alin_h = current->value.i_data;
						break;
					case 74:
						t_alin_v = current->value.i_data;
						break;
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
		
		if (((p_space == 0) && (paper == 0)) || ((p_space != 0) && (paper != 0))){
			
			/* find the font index and font*/
			fnt_idx = dxf_font_idx(drawing, t_style);
			shx_font = drawing.text_fonts[fnt_idx].shx_font;
			
			if(shx_font == NULL){ /* if font not loaded*/
				/* use the deafault font*/
				shx_font = drawing.text_fonts[0].shx_font;
			}
			
			/* find the dimentions of SHX font */
			if(shx_font){ /* if the font exists */
				if(shx_font->next){ /* the font descriptor is stored in first iten of list */
					if(shx_font->next->cmd_size > 1){ /* check if the font is valid */
						fnt_above = shx_font->next->cmds[0]; /* size above the base line of text */
						fnt_below = shx_font->next->cmds[1]; /* size below the base line of text */
						if((fnt_above + fnt_below) > 0){
							fnt_size = fnt_above + fnt_below;
						}
					}
				}
			}
			
			
			curr_graph = shx_font_parse(shx_font, text);
			
			
			if (curr_graph){
				/* find the layer index */
				lay_idx = dxf_lay_idx(drawing, layer);
				
				/* check if  object's color  is definied by layer,
				then look for layer's color */
				if (color >= 256){
					color = drawing.layers[lay_idx].color;
				}
				
				/* check if  object's ltype  is definied by layer,
				then look for layer's ltype */
				if ((strcmp(l_type, "BYLAYER") == 0) ||
					((l_type[0] == 0))){ /* if the value is omitted, the ltype is BYLAYER too */
					strcpy(l_type, drawing.layers[lay_idx].ltype);
				}
				
				/* find the ltype index */
				ltype_idx = dxf_ltype_idx(drawing, l_type);
				
				/* change the graph line pattern */
				curr_graph->patt_size = drawing.ltypes[ltype_idx].size;
				for (i = 0; i < drawing.ltypes[ltype_idx].size; i++){
					curr_graph->pattern[i] = drawing.ltypes[ltype_idx].pat[i];
				}
				
				/*change the color */
				curr_graph->color = dxf_colors[color];
				
				/* find the dimentions of text */
				txt_size = t_size/fnt_size;
				txt_w = fabs(curr_graph->ext_max_x - curr_graph->ext_min_x);
				txt_h = fabs(curr_graph->ext_max_y - curr_graph->ext_min_y);
				
				/* find the insert point of text, in function of its aling */
				if(t_alin_h < 3){
					t_center_x = (double)t_alin_h * (t_scale_x*txt_w * txt_size/2);
					t_base_x =  (double)t_alin_h * (pt2_x - pt1_x)/2;
					t_base_y =  (double)t_alin_h * (pt2_y - pt1_y)/2;
				}
				else{ 
					if(t_alin_h == 4){
						t_base_x = (pt2_x - pt1_x)/2;
						t_base_y = (pt2_y - pt1_y)/2;
					}
					else{
						t_scale_x = sqrt(pow((pt2_x - pt1_x), 2) + pow((pt2_y - pt1_y), 2))/(txt_w * txt_size);
					}
					//rot = atan2((pt2_y - pt1_y),(pt2_x - pt1_x)) * 180/M_PI;
					
					//printf("alinhamento=%d\n", t_alin_h);
				}
				if(t_alin_v >0){
					t_center_y = (double)(t_alin_v - 1) * (txt_size/2);
				}
				
				t_pos_x = pt1_x + t_base_x - t_center_x;
				t_pos_y = pt1_y + t_base_y - t_center_y;
				
				/* apply the scales, offsets and rotation to graphs */
				graph_modify(curr_graph, t_pos_x, t_pos_y, t_scale_x*txt_size, txt_size, t_rot);
				
			}
			return curr_graph;
		}
	}
	return NULL;
}

vector_p * dxf_graph_parse(dxf_drawing drawing, dxf_node * ent, int p_space){
	/* this function is non recursive */
	
	vector_p *v_return = NULL, v_search;
	dxf_node *current = NULL, *insert_ent = NULL, *blk = NULL , *prev;
	enum dxf_graph ent_type;
	int lay_idx, ltype_idx;
	graph_obj * curr_graph = NULL;
	
	/* for insert objects */
	struct ins_save{
		dxf_node * ins_ent, *prev;
		double ofs_x, ofs_y, ofs_z;
		double rot, scale_x, scale_y, scale_z;
		int start_idx, end_idx;
		double normal[3];
	};
	
	struct ins_save ins_stack[10];
	int ins_stack_pos = 0;
	
	struct ins_save ins_zero = {
		.ins_ent = ent, .prev = NULL,
		.ofs_x = 0.0, .ofs_y =0.0, .ofs_z =0.0,
		.rot = 0.0, .scale_x = 1.0 , .scale_y = 1.0, .scale_z = 1.0,
		.normal = {0.0, 0.0, 1.0}
	};
	
	ins_stack[0] = ins_zero;
	int ins_flag = 0;
	/* ---- */
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double t_rot = 0, rot = 0, tick = 0, elev = 0;
	double scale_x = 1.0, scale_y = 1.0, scale_z = 1.0;
	double extru_x = 0.0, extru_y = 0.0, extru_z = 1.0;
	
	char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS], layer[DXF_MAX_CHARS];
	char name1[DXF_MAX_CHARS], name2[DXF_MAX_CHARS], comment[DXF_MAX_CHARS];
	
	int color = 256, paper = 0;
	
	/*flags*/
	int pt1 = 0;
	int i;
	int blk_flag = 0;
	
	/* Initialize */
	/*create the vector of returned values */
	v_return = vect_new ();
	if (v_return){
		current = ent;
	}
	else{
		current = NULL;
	}
	ent_type = DXF_NONE;
	while (current){
		prev = current;
		/* ============================================================= */
		if (current->type == DXF_ENT){
			ent_type = DXF_NONE;
			if (strcmp(current->obj.name, "LINE") == 0){
				ent_type = DXF_LINE;
				curr_graph = dxf_line_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "TEXT") == 0){
				curr_graph = dxf_text_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				ent_type = DXF_TEXT;
				
				
			}
			else if (strcmp(current->obj.name, "CIRCLE") == 0){
				ent_type = DXF_CIRCLE;
				curr_graph = dxf_circle_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "ARC") == 0){
				ent_type = DXF_ARC;
				curr_graph = dxf_arc_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "POLYLINE") == 0){
				ent_type = DXF_POLYLINE;
				curr_graph = dxf_pline_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "VERTEX") == 0){
				ent_type = DXF_VERTEX;
				
				
			}
			else if (strcmp(current->obj.name, "TRACE") == 0){
				ent_type = DXF_TRACE;
				
				
			}
			else if (strcmp(current->obj.name, "SOLID") == 0){
				ent_type = DXF_SOLID;
				
				
			}
			else if (strcmp(current->obj.name, "LWPOLYLINE") == 0){
				ent_type = DXF_LWPOLYLINE;
				curr_graph = dxf_lwpline_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
			}
			else if (strcmp(current->obj.name, "SPLINE") == 0){
				//ent_type = DXF_LWPOLYLINE;
				curr_graph = dxf_spline_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
			}
			else if (strcmp(current->obj.name, "ATTRIB") == 0){
				ent_type = DXF_ATTRIB;
				curr_graph = dxf_attrib_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
				
			}
			else if (strcmp(current->obj.name, "POINT") == 0){
				ent_type = DXF_POINT;
				
				
			}
			
			else if (strcmp(current->obj.name, "SHAPE") == 0){
				ent_type = DXF_SHAPE;
				
				
			}
			else if (strcmp(current->obj.name, "VIEWPORT") == 0){
				ent_type = DXF_VIEWPORT;
				
				
			}
			else if (strcmp(current->obj.name, "3DFACE") == 0){
				ent_type = DXF_3DFACE;
				
				
			}
			else if (strcmp(current->obj.name, "ELLIPSE") == 0){
				ent_type = DXF_ELLIPSE;
				curr_graph = dxf_ellipse_parse(drawing, current, p_space);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "MTEXT") == 0){
				ent_type = DXF_MTEXT;
				
				
			}
			
			/* ============================================================= */
			/* complex entities */
			else if (strcmp(current->obj.name, "INSERT") == 0){
				ent_type = DXF_INSERT;
				insert_ent = current;
				ins_flag = 1;
				
				if (current->obj.content){
					/* starts the content sweep */
					current = current->obj.content->next;
					prev = current;
					continue;
				}
			}
			else if (strcmp(current->obj.name, "BLOCK") == 0){
				ent_type = DXF_BLK;
				blk_flag = 1;
				
				if (current->obj.content){
					/* starts the content sweep */
					current = current->obj.content->next;
					continue;
				}
			}
			else if (strcmp(current->obj.name, "DIMENSION") == 0){
				ent_type = DXF_DIMENSION;
				
				if (current->obj.content){
					/* starts the content sweep */
					//current = current->obj.content->next;
				}
			}
		}
		
		/* ============================================================= */
		else if (current->type == DXF_ATTR){ /* DXF attibute */
			//printf("%d\n", current->value.group);
			switch (current->value.group){
				case 2:
					strcpy(name1, current->value.s_data);
					break;
				case 3:
					strcpy(name2, current->value.s_data);
					break;
				case 5:
					strcpy(handle, current->value.s_data);
					break;
				case 6:
					strcpy(l_type, current->value.s_data);
					break;
				case 8:
					strcpy(layer, current->value.s_data);
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
				case 39:
					tick = current->value.d_data;
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
				case 62:
					color = current->value.i_data;
					break;
				case 67:
					paper = current->value.i_data;
					//printf("Paper %d\n", paper);
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
				case 999:
					strcpy(comment, current->value.s_data);
					break;
			}
			
			
		}
		current = current->next; /* go to the next in the list */
		/* ============================================================= */
		/* complex entities */
		
		if (((ins_flag != 0) && (current == NULL))||
			((ins_flag != 0) && (current != NULL) && (current != insert_ent) && (current->type == DXF_ENT))){
			ins_flag = 0;
			/* look for block */
			v_search = dxf_find_obj_descr(drawing.blks, "BLOCK", name1);
			if ((v_search.data) && /* block found */
			/* Also check the paper space parameter */
			(((p_space == 0) && (paper == 0)) || 
			((p_space != 0) && (paper != 0)))){ 
				blk = ((dxf_node **) v_search.data)[0];
				//printf ("bloco %s\n", name1);
				free(v_search.data);
				/* save current entity for future process */
				ins_stack_pos++;
				ins_stack[ins_stack_pos].ins_ent = blk;
				ins_stack[ins_stack_pos].prev = prev;
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
				if (v_return->size > 0){
					ins_stack[ins_stack_pos].start_idx = v_return->size;
				}
				else{
					ins_stack[ins_stack_pos].start_idx = 0;
				}
				
				/* now, current is the block */
				prev = blk;
				current = blk;
				//p_space = paper;
				
				/*reinit_vars: */
				
				ent_type = DXF_NONE;
					
				pt1_x = 0; pt1_y = 0; pt1_z = 0; rot = 0;
				tick = 0; elev = 0; t_rot = 0;
				scale_x = 1.0; scale_y = 1.0; scale_z = 1.0;
				extru_x = 0.0; extru_y = 0.0; extru_z = 1.0;
				
				/* clear the strings */
				handle[0] = 0;
				l_type[0] = 0;
				layer[0] = 0;
				comment[0] = 0;
				name1[0] = 0;
				name2[0] = 0;
				
				color = 256; paper= 0;
				
				/*clear flags*/
				pt1 = 0;
				continue;
			}
		}
		
		else if (((blk_flag != 0) && (current == NULL))||
			((blk_flag != 0) && (current != NULL) && (current != blk) && (current->type == DXF_ENT))){
			blk_flag = 0;
			//p_space = paper;
			
			//printf("Bloco %0.2f, %0.2f, %0.2f\n", pt1_x, pt1_y, pt1_z);
		}
		
		if (prev == ent){ /* stop the search if back on initial entity */
			//printf("para\n");
			current = NULL;
			break;
		}
		
		/* ============================================================= */
		while (current == NULL){
			/* end of list sweeping */
			/* try to back in structure hierarchy */
			if (prev == ent){ /* stop the search if back on initial entity */
				//printf("para\n");
				current = NULL;
				break;
			}
			prev = prev->master;
			if (prev){ /* up in structure */
				/* try to continue on previous point in structure */
				current = prev->next;
				//indent --;
				if (prev == ins_stack[ins_stack_pos].ins_ent){/* back on initial entity */
					if (v_return->size > 0){
						vec_graph_modify_idx(v_return,
							ins_stack[ins_stack_pos].ofs_x,
							ins_stack[ins_stack_pos].ofs_y,
							ins_stack[ins_stack_pos].scale_x,
							ins_stack[ins_stack_pos].scale_y,
							ins_stack[ins_stack_pos].rot,
							ins_stack[ins_stack_pos].start_idx,
							v_return->size - 1
							);
						vec_graph_mod_ax(v_return,
							ins_stack[ins_stack_pos].normal,
							ins_stack[ins_stack_pos].start_idx,
							v_return->size - 1
							);
					}
					/*printf("%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%d,%d\n",
						ins_stack[ins_stack_pos].ofs_x,
						ins_stack[ins_stack_pos].ofs_y,
						ins_stack[ins_stack_pos].scale_x,
						ins_stack[ins_stack_pos].scale_y,
						ins_stack[ins_stack_pos].rot,
						ins_stack[ins_stack_pos].start_idx,
						v_return->size - 1
					);*/
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
	return v_return;
}

int dxf_ents_parse(dxf_drawing drawing){
	dxf_node *current = NULL;
	vector_p *vec_graph;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				vec_graph = dxf_graph_parse(drawing, current, 0);
				if (vec_graph){
					current->obj.graphics = vec_graph;
				}
				/*if (current->obj.name){
					printf("%s\n", current->obj.name);
				}*/
				//---------------------------------------
			}
			current = current->next;
			//printf("%d\n", current);
		}
	}
}

int dxf_ents_draw(dxf_drawing drawing, bmp_img * img, double ofs_x, double ofs_y, double scale){
	dxf_node *current = NULL;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				vec_graph_draw(current->obj.graphics, img, ofs_x, ofs_y, scale);
				
				//---------------------------------------
			}
			current = current->next;
		}
	}
}

int dxf_ents_ext(dxf_drawing drawing, double * min_x, double * min_y, double * max_x, double * max_y){
	dxf_node *current = NULL;
	int ext_ini = 0;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){
				
				vec_graph_ext(current->obj.graphics, &ext_ini, min_x, min_y, max_x, max_y);
				
				/* -------------------------------------------
				if (ext_ini == 0){
					ext_ini = 1;
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
				
				//---------------------------------------*/
			}
			current = current->next;
		}
	}
}
