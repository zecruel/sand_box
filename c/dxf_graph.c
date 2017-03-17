#include "dxf_graph.h"
#include "shape2.h"

#include "dxf_colors.h"


graph_obj * dxf_line_parse(dxf_drawing drawing, dxf_node * ent){
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
	return NULL;
}

graph_obj * dxf_circle_parse(dxf_drawing drawing, dxf_node * ent){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double radius, tick = 0, elev = 0;
		
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
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
		}
		return curr_graph;
	}
	return NULL;
}

graph_obj * dxf_arc_parse(dxf_drawing drawing, dxf_node * ent){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double radius, tick = 0, elev = 0;
		double start_ang = 0.0, end_ang = 0.0;
		
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
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
		}
		return curr_graph;
	}
	return NULL;
}

graph_obj * dxf_ellipse_parse(dxf_drawing drawing, dxf_node * ent){
	if(ent){
		dxf_node *current = NULL;
		double pt1_x = 0, pt1_y = 0, pt1_z = 0;
		double pt2_x = 0, pt2_y = 0, pt2_z = 0;
		double minor_ax, tick = 0, elev = 0;
		double start_ang = 0.0, end_ang = 0.0;
		
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
					case 999:
						strcpy(comment, current->value.s_data);
				}
			}
			current = current->next; /* go to the next in the list */
		}
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
		}
		return curr_graph;
	}
	return NULL;
}

vector_p * dxf_graph_parse(dxf_drawing drawing, dxf_node * ent){
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
	};
	
	struct ins_save ins_stack[10];
	int ins_stack_pos = 0;
	
	struct ins_save ins_zero = {
		.ins_ent = ent, .prev = NULL,
		.ofs_x = 0.0, .ofs_y =0.0, .ofs_z =0.0,
		.rot = 0.0, .scale_x = 1.0 , .scale_y = 1.0, .scale_z = 1.0
	};
	
	ins_stack[0] = ins_zero;
	int ins_flag = 0;
	/* ---- */
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double t_rot = 0, rot = 0, tick = 0, elev = 0;
	double scale_x = 1.0, scale_y = 1.0, scale_z = 1.0;
	
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
				curr_graph = dxf_line_parse(drawing, current);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "TEXT") == 0){
				ent_type = DXF_TEXT;
				
				
			}
			else if (strcmp(current->obj.name, "CIRCLE") == 0){
				ent_type = DXF_CIRCLE;
				curr_graph = dxf_circle_parse(drawing, current);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "ARC") == 0){
				ent_type = DXF_ARC;
				curr_graph = dxf_arc_parse(drawing, current);
				if (curr_graph){
					/* store the graph in the return vector */
					stack_push(v_return, curr_graph);
				}
				
			}
			else if (strcmp(current->obj.name, "POLYLINE") == 0){
				ent_type = DXF_POLYLINE;
				
				
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
				
				
			}
			else if (strcmp(current->obj.name, "ATTRIB") == 0){
				ent_type = DXF_ATTRIB;
				
				
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
				curr_graph = dxf_ellipse_parse(drawing, current);
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
				}
			}
			else if (strcmp(current->obj.name, "BLOCK") == 0){
				ent_type = DXF_BLK;
				blk_flag = 1;
				
				if (current->obj.content){
					/* starts the content sweep */
					current = current->obj.content->next;
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
			if (v_search.data){ /* block found */
				blk = ((dxf_node **) v_search.data)[0];
				//printf ("bloco %s\n", name1);
				free(v_search.data);
				/* save current entity for future process */
				ins_stack_pos++;
				ins_stack[ins_stack_pos].ins_ent = blk;
				ins_stack[ins_stack_pos].prev = prev;
				ins_stack[ins_stack_pos].ofs_x = pt1_x + ins_stack[ins_stack_pos - 1].ofs_x;
				ins_stack[ins_stack_pos].ofs_y = pt1_y + ins_stack[ins_stack_pos - 1].ofs_y;
				ins_stack[ins_stack_pos].ofs_z = pt1_z + ins_stack[ins_stack_pos - 1].ofs_z;
				ins_stack[ins_stack_pos].scale_x = scale_x * ins_stack[ins_stack_pos - 1].scale_x;
				ins_stack[ins_stack_pos].scale_y = scale_y * ins_stack[ins_stack_pos - 1].scale_y;
				ins_stack[ins_stack_pos].scale_z = scale_z * ins_stack[ins_stack_pos - 1].scale_z;
				ins_stack[ins_stack_pos].rot = t_rot + ins_stack[ins_stack_pos - 1].rot;
				
				/* now, current is the block */
				prev = blk;
				current = blk;
				
				/*reinit_vars: */
				
				ent_type = DXF_NONE;
					
				pt1_x = 0; pt1_y = 0; pt1_z = 0; rot = 0;
				tick = 0; elev = 0; t_rot = 0;
				scale_x = 1.0; scale_y = 1.0; scale_z = 1.0;
				
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
			
			//printf("Bloco x=%0.2f y=%0.2f\n", pt1_x, pt1_y);
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
					if (ins_stack_pos < 1){
						/* stop the search if back on initial entity */
						current = NULL;
						break;
					}
					else{
						prev = ins_stack[ins_stack_pos].prev;
						ins_stack_pos--;
						//prev = ins_stack[ins_stack_pos].ins_ent;
						//printf("retorna %d\n", prev);
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
				vec_graph = dxf_graph_parse(drawing, current);
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
