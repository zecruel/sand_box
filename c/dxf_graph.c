#include "dxf_graph.h"

int dxf_graph_parse(dxf_drawing drawing, dxf_node * ent){
	vector_p stack, v_search, *v_return = NULL;
	dxf_node *current = NULL, *pline_ent = NULL, *insert_ent = NULL, *blk = NULL;
	enum dxf_graph ent_type;
	int lay_idx, ltype_idx;
	graph_obj * curr_graph = NULL;
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double pt2_x = 0, pt2_y = 0, pt2_z = 0;
	double pt3_x = 0, pt3_y = 0, pt3_z = 0;
	double pt4_x = 0, pt4_y = 0, pt4_z = 0;
	double offset_x = 0, offset_y = 0, offset_z = 0;
	double radius = 0, rot = 0, tick = 0, elev = 0;
	double ang_start = 0, ang_end = 0, bulge = 0;
	double t_size = 0, t_rot = 0;
	
	char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS], t_style[DXF_MAX_CHARS], layer[DXF_MAX_CHARS], comment[DXF_MAX_CHARS];
	char t_text[DXF_MAX_CHARS], name1[DXF_MAX_CHARS], name2[DXF_MAX_CHARS];
	
	int color = 256, paper = 0;
	int t_alin_v = 0, t_alin_h = 0;
	
	/*flags*/
	int pt1 = 0, pt2 = 0, pt3 = 0, pt4 = 0;
	
	/*for polylines*/
	int pline_flag;
	int first = 0, poly = 0, closed =0;
	double prev_x, prev_y, last_x, last_y;
	
	
	
	
	
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
					
					switch (ent_type){ /* simple entities */
						case DXF_LINE:
							//printf("linha (%.2f,%.2f)-(%.2f,%.2f)  cor=%d ltype = %d \n", pt1_x, pt1_y, pt2_x, pt2_y, color, ltype_idx);
							if (curr_graph){
								line_add(curr_graph, pt1_x, pt1_y, pt2_x, pt2_y);
								stack_push(v_return, curr_graph);
								//printf("ADD %d, %d\n", ent_type, curr_graph);
							}
							goto reinit_vars;
						
						case DXF_POINT:
							goto reinit_vars;
							
						case DXF_CIRCLE:
							if (curr_graph){
								graph_arc(curr_graph, pt1_x, pt1_y, radius, 0.0, 0.0, 1);
								stack_push(v_return, curr_graph);
							}
							goto reinit_vars;
							
						case DXF_ARC:
							if (curr_graph){
								graph_arc(curr_graph, pt1_x, pt1_y, radius, ang_start, ang_end, -1);
								stack_push(v_return, curr_graph);
							}
							goto reinit_vars;
							
						case DXF_TRACE:
							goto reinit_vars;
							
						case DXF_SOLID:
							goto reinit_vars;
							
						case DXF_TEXT:
							goto reinit_vars;
							
						case DXF_SHAPE:
							goto reinit_vars;
							
						case DXF_ATTRIB:
							goto reinit_vars;
							
						case DXF_VERTEX:
							//printf("vertice (%0.2f, %0.2f)\n", pt1_x, pt1_y);
							if(poly){
								
								if(first){
									line_add(curr_graph, prev_x, prev_y, pt1_x, pt1_y);
									
								}
								else{
									first = 1;
									last_x = pt1_x;
									last_y = pt1_y;
								}
								prev_x = pt1_x;
								prev_y = pt1_y;
							}
							goto reinit_vars;
							
						case DXF_LWPOLYLINE:
							poly = 1;
							first = 0;
							goto reinit_vars;
							
						case DXF_3DFACE:
							goto reinit_vars;
							
						case DXF_VIEWPORT:
							goto reinit_vars;
							
						case DXF_DIMENSION:
							goto reinit_vars;
						
						
							
						reinit_vars:
						
						ent_type = DXF_NONE;
							
						pt1_x = 0; pt1_y = 0; pt1_z = 0;
						pt2_x = 0; pt2_y = 0; pt2_z = 0;
						pt3_x = 0; pt3_y = 0; pt3_z = 0;
						pt4_x = 0; pt4_y = 0; pt4_z = 0;
						radius = 0; rot = 0;
						tick = 0; elev = 0;
						ang_start = 0; ang_end = 0; bulge =0;
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
					
					if ((current != NULL)&&(current == pline_ent)){ /* back on polyline ent */
						/*then end the polyline entity */
						pline_ent = NULL;
						
						/*reset polylines*/
						printf("reset polylines\n");
						first = 0; poly = 0; closed =0;
						prev_x = 0; prev_y =0; last_x = 0; last_y = 0;
					}
					
					if ((current != NULL)&&(current == insert_ent)){ /* back on insert ent */
						/*then end the insert entity */
						insert_ent = NULL;
						
						/*reset inserts*/
						printf("reset inserts\n");
						first = 0; poly = 0; closed =0;
						prev_x = 0; prev_y =0; last_x = 0; last_y = 0;
					}
					
					if (current == ent){ /* back on initial ent */
						current = NULL;
					}
						
					if (current){
						current = current->next; /* go to the next in the list */
					}
				}
				else if (current->type == DXF_ENT){
					
					switch (ent_type){ /* complex entities */
						case DXF_INSERT:
							/* look for block */
							v_search = dxf_find_obj_descr(drawing.blks, "BLOCK", name1);
							if (v_search.data){ /* block found */
								blk = ((dxf_node **) v_search.data)[0];
								printf ("bloco\n");
								free(v_search.data);
								/* save current entity for future process */
								stack_push (&stack, current->prev);
								/* now, current is the block */
								current = blk->obj.content->next;
							}
							goto reinit_pline_ent;
							
						case DXF_POLYLINE:
							//printf("polyline");
							poly = 1;
							first = 0;
							if (pline_flag & 1){
								closed = 1;
							}
						
						reinit_pline_ent:	
						pline_flag = 0;							
						pt1_x = 0; pt1_y = 0; pt1_z = 0;
						pt2_x = 0; pt2_y = 0; pt2_z = 0;
						pt3_x = 0; pt3_y = 0; pt3_z = 0;
						pt4_x = 0; pt4_y = 0; pt4_z = 0;
						radius = 0; rot = 0;
						tick = 0; elev = 0;
						ang_start = 0; ang_end = 0; bulge =0;
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
						ent_type = DXF_NONE;
						continue;
					}
					
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
						
						if (ent_type == DXF_POLYLINE){
							pline_ent = current;
						}
						if (ent_type == DXF_INSERT){
							insert_ent = current;
						}
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
							ang_start = current->value.d_data;
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
						case 70:
							pline_flag = current->value.i_data;
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

vector_p * dxf_graph_parse2(dxf_drawing drawing, dxf_node * ent){
	/* this function is non recursive */
	
	vector_p *v_return = NULL;
	dxf_node *current = NULL, *pline_ent = NULL, *insert_ent = NULL, *blk = NULL , *prev;
	enum dxf_graph ent_type;
	int lay_idx, ltype_idx;
	graph_obj * curr_graph = NULL;
	
	double pt1_x = 0, pt1_y = 0, pt1_z = 0;
	double pt2_x = 0, pt2_y = 0, pt2_z = 0;
	double pt3_x = 0, pt3_y = 0, pt3_z = 0;
	double pt4_x = 0, pt4_y = 0, pt4_z = 0;
	double offset_x = 0, offset_y = 0, offset_z = 0;
	double radius = 0, rot = 0, tick = 0, elev = 0;
	double ang_start = 0, ang_end = 0, bulge = 0;
	double t_size = 0, t_rot = 0;
	
	char handle[DXF_MAX_CHARS], l_type[DXF_MAX_CHARS], t_style[DXF_MAX_CHARS], layer[DXF_MAX_CHARS], comment[DXF_MAX_CHARS];
	char t_text[DXF_MAX_CHARS], name1[DXF_MAX_CHARS], name2[DXF_MAX_CHARS];
	
	int color = 256, paper = 0;
	int t_alin_v = 0, t_alin_h = 0;
	
	/*flags*/
	int pt1 = 0, pt2 = 0, pt3 = 0, pt4 = 0;
	
	/*for polylines*/
	int pline_flag;
	int first = 0, poly = 0, closed =0;
	double prev_x, prev_y, last_x, last_y;
	
	int i;
	int indent = 0;
	
	/* Initialize */
	/*create the vector of returned values */
	v_return = vect_new ();
	if (v_return){
		current = ent;
	}
	else{
		current = NULL;
	}
	
	while (current){
		prev = current;
		/* ============================================================= */
		if (current->type == DXF_ENT){
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
			
			
			if (current->obj.content){
				/* starts the content sweep */
				current = current->obj.content->next;
			}
		}
		/* ============================================================= */
		else if (current->type == DXF_ATTR){ /* DXF attibute */
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
					ang_start = current->value.d_data;
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
				case 70:
					pline_flag = current->value.i_data;
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
			
			current = current->next; /* go to the next in the list */
		}
		/* ============================================================= */
		if (current == NULL){
			/* end of list sweeping */
			
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
			
			switch (ent_type){ /* simple entities */
				case DXF_LINE:
					//printf("linha (%.2f,%.2f)-(%.2f,%.2f)  cor=%d ltype = %d \n", pt1_x, pt1_y, pt2_x, pt2_y, color, ltype_idx);
					curr_graph = graph_new();
					if (curr_graph){
						line_add(curr_graph, pt1_x, pt1_y, pt2_x, pt2_y);
						stack_push(v_return, curr_graph);
						//printf("ADD %d, %d\n", ent_type, curr_graph);
					}
					goto reinit_vars;
				
				case DXF_POINT:
					goto reinit_vars;
					
				case DXF_CIRCLE:
					curr_graph = graph_new();
					if (curr_graph){
						graph_arc(curr_graph, pt1_x, pt1_y, radius, 0.0, 0.0, 1);
						stack_push(v_return, curr_graph);
					}
					goto reinit_vars;
					
				case DXF_ARC:
					curr_graph = graph_new();
					if (curr_graph){
						graph_arc(curr_graph, pt1_x, pt1_y, radius, ang_start, ang_end, -1);
						stack_push(v_return, curr_graph);
					}
					goto reinit_vars;
					
				case DXF_TRACE:
					goto reinit_vars;
					
				case DXF_SOLID:
					goto reinit_vars;
					
				case DXF_TEXT:
					goto reinit_vars;
					
				case DXF_SHAPE:
					goto reinit_vars;
					
				case DXF_ATTRIB:
					goto reinit_vars;
					
				case DXF_VERTEX:
					//printf("vertice (%0.2f, %0.2f)\n", pt1_x, pt1_y);
					if(poly){
						
						if((first != 0) && (curr_graph != NULL)){
							line_add(curr_graph, prev_x, prev_y, pt1_x, pt1_y);
							
						}
						else{
							first = 1;
							last_x = pt1_x;
							last_y = pt1_y;
						}
						prev_x = pt1_x;
						prev_y = pt1_y;
					}
					goto reinit_vars;
					
				case DXF_LWPOLYLINE:
					poly = 1;
					first = 0;
					goto reinit_vars;
					
				case DXF_3DFACE:
					goto reinit_vars;
					
				case DXF_VIEWPORT:
					goto reinit_vars;
					
				case DXF_DIMENSION:
					goto reinit_vars;
				
				case DXF_INSERT:
					goto reinit_vars;
					
				case DXF_POLYLINE:
					curr_graph = graph_new();
					//printf("polyline");
					poly = 1;
					first = 0;
					if (pline_flag & 1){
						closed = 1;
					}
					pline_ent = current;
					goto reinit_vars;
						
				reinit_vars:
				
				ent_type = DXF_NONE;
					
				pt1_x = 0; pt1_y = 0; pt1_z = 0;
				pt2_x = 0; pt2_y = 0; pt2_z = 0;
				pt3_x = 0; pt3_y = 0; pt3_z = 0;
				pt4_x = 0; pt4_y = 0; pt4_z = 0;
				radius = 0; rot = 0;
				tick = 0; elev = 0;
				ang_start = 0; ang_end = 0; bulge =0;
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
			
		}
		/* ============================================================= */
		while (current == NULL){
			
			/* try to back in structure hierarchy */
			if (prev == ent){ /* stop the search if back on initial entity */
				current = NULL;
				break;
			}
			prev = prev->master;
			if (prev){ /* up in structure */
				
				/* ====== close complex entities ============== */
				if (prev == pline_ent){ /* back on polyline ent */
					/*then end the polyline entity */
					pline_ent = NULL;
					/* store the graph */
					if (curr_graph){
						stack_push(v_return, curr_graph);
					}
					
					/*reset polylines*/
					printf("reset polylines\n");
					first = 0; poly = 0; closed =0;
					prev_x = 0; prev_y =0; last_x = 0; last_y = 0;
				}
				
				
				/* ============================== */
				
				/* try to continue on previous point in structure */
				current = prev->next;
				//indent --;
				if (prev == ent){
					/* stop the search if back on initial entity */
					current = NULL;
					//printf("fim loop ");
					break;
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
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				dxf_graph_parse(drawing, current);
				/*if (current->obj.name){
					printf("%s\n", current->obj.name);
				}*/
				//---------------------------------------
			}
			current = current->next;
		}
	}
}

int dxf_ents_parse2(dxf_drawing drawing){
	dxf_node *current = NULL;
	vector_p *vec_graph;
		
	if ((drawing.ents != NULL) && (drawing.main_struct != NULL)){
		current = drawing.ents->obj.content->next;
		
		// starts the content sweep 
		while (current != NULL){
			if (current->type == DXF_ENT){ // DXF entity 
				
				// -------------------------------------------
				vec_graph = dxf_graph_parse2(drawing, current);
				if (vec_graph){
					current->obj.graphics = vec_graph;
				}
				/*if (current->obj.name){
					printf("%s\n", current->obj.name);
				}*/
				//---------------------------------------
			}
			current = current->next;
		}
	}
}

/*
int dxf_graph_draw(dxf_drawing drawing, dxf_node * ent, bmp_img * img){
	if (ent != NULL){
		vec_graph_draw(ent->obj.graphics, img);
	}
	
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
	
	return 0;
}*/

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
