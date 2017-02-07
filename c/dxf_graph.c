#include "dxf_graph.h"

int dxf_graph_parse(dxf_drawing drawing, dxf_node * ent){
	vector_p stack, v_search, *v_return = NULL;
	dxf_node *current = NULL, *e_layer = NULL;
	enum dxf_graph ent_type;
	int lay_idx, ltype_idx;
	graph_obj * curr_graph = NULL;
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double pt2_x = 0, pt2_y = 0, pt2_z = 0;
	double pt3_x = 0, pt3_y = 0, pt3_z = 0;
	double pt4_x = 0, pt4_y = 0, pt4_z = 0;
	double offset_x = 0, offset_y = 0, offset_z = 0;
	double radius = 0, rot = 0, tick = 0, elev = 0;
	double ang_ini = 0, ang_end = 0, bulge = 0;
	double t_size = 0, t_rot = 0;
	
	char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS], t_style[DXF_MAX_CHARS], layer[DXF_MAX_CHARS], comment[DXF_MAX_CHARS];
	char t_text[DXF_MAX_CHARS], name1[DXF_MAX_CHARS], name2[DXF_MAX_CHARS];
	
	int color = 256, paper = 0;
	int t_alin_v = 0, t_alin_h = 0;
	
	/*flags*/
	int pt1 = 0, pt2 = 0, pt3 = 0, pt4 = 0;
	
	
	/* initialize the stack */
	stack.size = 0;
	stack.data = NULL;
	
	if ((ent != NULL) && (drawing.ents != NULL) && (drawing.main_struct != NULL)){
		if (ent->type == DXF_ENT){ /* check if is a DXF entity */
			current = ent;
			//curr_graph = ent->obj.graphics;
			
			/* starts the content sweep */
			while ((current != NULL) || (stack.size > 0)){
				if (current == NULL){ /* end of list sweeping */
					
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
					
					switch (ent_type){
						case DXF_LINE:
							printf("linha (%.2f,%.2f)-(%.2f,%.2f)  cor=%d ltype = %d \n", pt1_x, pt1_y, pt2_x, pt2_y, color, ltype_idx);
							if (curr_graph){
								line_add(curr_graph, pt1_x, pt1_y, pt2_x, pt2_y);
								stack_push(v_return, curr_graph);
								printf("ADD %d, %d\n", ent_type, curr_graph);
							}
							goto reinit_vars;
						
						case DXF_POINT:
							goto reinit_vars;
							
						case DXF_CIRCLE:
							goto reinit_vars;
							
						case DXF_ARC:
							goto reinit_vars;
							
						case DXF_TRACE:
							goto reinit_vars;
							
						case DXF_SOLID:
							goto reinit_vars;
							
						case DXF_TEXT:
							goto reinit_vars;
							
						case DXF_SHAPE:
							goto reinit_vars;
							
						case DXF_INSERT:
							goto reinit_vars;
							
						case DXF_ATTRIB:
							goto reinit_vars;
							
						case DXF_POLYLINE:
							goto reinit_vars;
							
						case DXF_VERTEX:
							goto reinit_vars;
							
						case DXF_LWPOLYLINE:
							goto reinit_vars;
							
						case DXF_3DFACE:
							goto reinit_vars;
							
						case DXF_VIEWPORT:
							goto reinit_vars;
							
						case DXF_DIMENSION:
							
						reinit_vars:
						
						ent_type = DXF_NONE;
							
						pt1_x = 0; pt1_y = 0; pt1_z = 0;
						pt2_x = 0; pt2_y = 0; pt2_z = 0;
						pt3_x = 0; pt3_y = 0; pt3_z = 0;
						pt4_x = 0; pt4_y = 0; pt4_z = 0;
						radius = 0; rot = 0;
						tick = 0; elev = 0;
						ang_ini = 0; ang_end = 0; bulge =0;
						t_size = 0; t_rot = 0;
						
						/* clear the strings */
						handle[0] = 0;
						l_type[0] = 0;
						t_style[0] = 0;
						layer[0] = 0;
						comment[0] = 0;
						t_text[0] =0;
						name1[0] = 0;
						name2[0] = 0;
						
						color = 256; paper= 0;
						t_alin_v = 0; t_alin_h = 0;
						
						/*clear flags*/
						pt1 = 0; pt2 = 0; pt3 = 0; pt4 = 0;
					}
					
					
					
					
					/* try to up in the structure hierarchy */
					current = stack_pop (&stack);
					if (current == ent){ /* back on initial ent */
						current = NULL;
					}
						
					if (current){
						current = current->next; /* go to the next in the list */
					}
				}
				else if (current->type == DXF_ENT){ /* DXF entity */
					ent_type = DXF_NONE;
					if (strcmp(current->obj.name, "LINE") == 0){
						ent_type = DXF_LINE;
					}
					else if (strcmp(current->obj.name, "TEXT") == 0){
						ent_type = DXF_TEXT;
					}
					else if (strcmp(current->obj.name, "CIRCLE") == 0){
						ent_type = DXF_CIRCLE;
					}
					else if (strcmp(current->obj.name, "ARC") == 0){
						ent_type = DXF_ARC;
					}
					else if (strcmp(current->obj.name, "POLYLINE") == 0){
						ent_type = DXF_POLYLINE;
					}
					else if (strcmp(current->obj.name, "VERTEX") == 0){
						ent_type = DXF_VERTEX;
					}
					else if (strcmp(current->obj.name, "INSERT") == 0){
						ent_type = DXF_INSERT;
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
					else if (strcmp(current->obj.name, "DIMENSION") == 0){
						ent_type = DXF_DIMENSION;
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
					
					/* -------------------------------------------*/
					if((ent_type != DXF_NONE) &&
						(ent_type != DXF_VERTEX)){
						/*
						if((ent_type == DXF_POINT) ||
						(ent_type == DXF_CIRCLE) ||
						(ent_type == DXF_ARC) ||
						(ent_type == DXF_TRACE) ||
						(ent_type == DXF_SOLID) ||
						(ent_type == DXF_TEXT) ||
						(ent_type == DXF_SHAPE) ||
						(ent_type == DXF_INSERT) ||
						(ent_type == DXF_ATTRIB) ||
						(ent_type == DXF_POLYLINE) ||
						(ent_type == DXF_VERTEX) ||
						(ent_type == DXF_LWPOLYLINE) ||
						(ent_type == DXF_3DFACE) ||
						(ent_type == DXF_VIEWPORT) ||
						(ent_type == DXF_DIMENSION))*/
						if (v_return == NULL){
							/*create the vector of returned values */
							v_return = vect_new ();
						}
						curr_graph = graph_new();
					}
					/*---------------------------------------*/
					
					/* down in the structure hierarchy */
					stack_push (&stack, current);
					
					if (current->obj.content){
						/* starts the content sweep */
						current = current->obj.content->next;
					}
				}
				else if (current->type == DXF_ATTR){ /* DXF attribute */
					switch (current->value.group){
						case 1:
							strcpy(t_text, current->value.s_data);
							break;
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
						case 7:
							strcpy(t_style, current->value.s_data);
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
						case 12:
							pt3_x = current->value.d_data;
							pt3 = 1; /* set flag */
							break;
						case 13:
							pt4_x = current->value.d_data;
							pt4 = 1; /* set flag */
							break;
						case 20:
							pt1_y = current->value.d_data;
							pt1 = 1; /* set flag */
							break;
						case 21:
							pt2_y = current->value.d_data;
							pt2 = 1; /* set flag */
							break;
						case 22:
							pt3_y = current->value.d_data;
							pt3 = 1; /* set flag */
							break;
						case 23:
							pt4_y = current->value.d_data;
							pt4 = 1; /* set flag */
							break;
						case 30:
							pt1_z = current->value.d_data;
							pt1 = 1; /* set flag */
							break;
						case 31:
							pt2_z = current->value.d_data;
							pt2 = 1; /* set flag */
							break;
						case 32:
							pt3_z = current->value.d_data;
							pt3 = 1; /* set flag */
							break;
						case 33:
							pt4_z = current->value.d_data;
							pt4 = 1; /* set flag */
							break;
						case 38:
							elev = current->value.d_data;
							break;
						case 39:
							tick = current->value.d_data;
							break;
						case 40:
							radius = current->value.d_data;
							t_size = current->value.d_data;
							break;
						case 42:
							bulge = current->value.d_data;
							break;
						case 50:
							ang_ini = current->value.d_data;
							t_rot = current->value.d_data;
							break;
						case 51:
							ang_end = current->value.d_data;
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
						case 74:
							t_alin_v = current->value.i_data;
							break;
						case 999:
							strcpy(comment, current->value.s_data);
							break;
					}
					current = current->next;
				}
			}
		}
	}
	ent->obj.graphics = v_return;
	return 0;
}

int dxf_ents_parse(dxf_drawing drawing){
	dxf_node *current = NULL;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				dxf_graph_parse(drawing, current);
				if (current->obj.name){
					printf("%s\n", current->obj.name);
				}
				//---------------------------------------
			}
			current = current->next;
		}
	}
}
	

int dxf_graph_draw(dxf_drawing drawing, dxf_node * ent, bmp_img * img){
	if (ent != NULL){
		vec_graph_draw(ent->obj.graphics, img);
	}
	
	
	/*
	vector_p stack, v_search;
	dxf_node *current = NULL;
	graph_obj * curr_graph = NULL;
	
	// initialize the stack 
	stack.size = 0;
	stack.data = NULL;
	
	if ((ent != NULL) && (drawing.ents != NULL) && (drawing.main_struct != NULL)){
		if (ent->type == DXF_ENT){ // check if is a DXF entity 
			current = ent;
			
			// starts the content sweep 
			while ((current != NULL) || (stack.size > 0)){
				if (current == NULL){ // end of list sweeping 
					
					// try to up in the structure hierarchy 
					current = stack_pop (&stack);
					if (current){
						current = current->next; // go to the next in the list 
					}
				}
				else if (current->type == DXF_ENT){ // DXF entity 
					
					// -------------------------------------------
					if (current->obj.graphics){
						//curr_graph = current->obj.graphics;
						graph_draw(current->obj.graphics, img);
					}
					//---------------------------------------
					
					// down in the structure hierarchy 
					stack_push (&stack, current);
					if (current->obj.name){
						//fprintf(file, "0\n%s\n", current->obj.name); // write the start of entity 
					}
					if (current->obj.content){
						// starts the content sweep 
						current = current->obj.content->next;
					}
				}
				else if (current->type == DXF_ATTR){ // DXF attribute 
					
					current = current->next;
				}
			}
		}
	}
	*/
	return 0;
}

int dxf_ents_draw(dxf_drawing drawing, bmp_img * img){
	dxf_node *current = NULL;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				vec_graph_draw(current->obj.graphics, img);
				//---------------------------------------
			}
			current = current->next;
		}
	}
}
