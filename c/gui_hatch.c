#include "gui_use.h"
#include <float.h>

int gui_hatch_interactive(gui_obj *gui){
	if (gui->modal == HATCH){
		static dxf_node *new_el;
		
		if (gui->step == 0){
			if (gui->ev & EV_ENTER){
				/* create a new DXF lwpolyline */
				new_el = (dxf_node *) dxf_new_lwpolyline (
					gui->step_x[gui->step], gui->step_y[gui->step], 0.0, /* pt1, */
					0.0, /* bulge */
					gui->color_idx, gui->drawing->layers[gui->layer_idx].name, /* color, layer */
					gui->drawing->ltypes[gui->ltypes_idx].name, dxf_lw[gui->lw_idx], /* line type, line weight */
					0); /* paper space */
				dxf_lwpoly_append (new_el, gui->step_x[gui->step], gui->step_y[gui->step], 0.0, gui->bulge);
				dxf_attr_change_i(new_el, 70, (void *) (int[]){1}, 0);
				gui->element = new_el;
				gui->step = 1;
				gui->en_distance = 1;
				gui->draw_tmp = 1;
				goto next_step;
			}
			else if (gui->ev & EV_CANCEL){
				goto default_modal;
			}
		}
		else{
			if (gui->ev & EV_ENTER){
				gui->step_x[gui->step - 1] = gui->step_x[gui->step];
				gui->step_y[gui->step - 1] = gui->step_y[gui->step];
				
				dxf_attr_change_i(new_el, 10, &gui->step_x[gui->step], -1);
				dxf_attr_change_i(new_el, 20, &gui->step_y[gui->step], -1);
				//dxf_attr_change_i(new_el, 42, &gui->bulge, -1);
				
				new_el->obj.graphics = dxf_graph_parse(gui->drawing, new_el, 0 , 1);
				
				dxf_lwpoly_append (new_el, gui->step_x[gui->step], gui->step_y[gui->step], 0.0, gui->bulge);
				gui->step = 2;
				goto next_step;
			}
			else if (gui->ev & EV_CANCEL){
				gui->draw_tmp = 0;
				if (gui->step == 2){
					dxf_lwpoly_remove (new_el, -1);
					//new_el->obj.graphics = dxf_graph_parse(gui->drawing, new_el, 0 , 0);
					//drawing_ent_append(gui->drawing, new_el);
					
					graph_obj *bound = dxf_lwpline_parse(gui->drawing, new_el, 0 , 0);
					
					struct h_pattern *curr_h = &(gui->list_pattern);
					int i = 0;
					
					while ((curr_h) && (i < gui->hatch_idx)){
						i++;
						curr_h = curr_h->next;
					}
					
					dxf_node *new_hatch_el = dxf_new_hatch (curr_h, bound,
					0, 1, 0, 0, 0.0, 1.0,
					gui->color_idx, gui->drawing->layers[gui->layer_idx].name, /* color, layer */
					gui->drawing->ltypes[gui->ltypes_idx].name, dxf_lw[gui->lw_idx], /* line type, line weight */
					0); /* paper space */
					
					if (new_hatch_el){
						new_hatch_el->obj.graphics = dxf_graph_parse(gui->drawing, new_hatch_el, 0 , 0);
						
						drawing_ent_append(gui->drawing, new_hatch_el);
						
						do_add_entry(&gui->list_do, "HATCH");
						do_add_item(gui->list_do.current, NULL, new_hatch_el);
					}
					
					gui->step = 0;
				}
				gui->element = NULL;
				goto next_step;
			}
			if (gui->ev & EV_MOTION){
				dxf_attr_change(new_el, 6, gui->drawing->ltypes[gui->ltypes_idx].name);
				dxf_attr_change(new_el, 8, gui->drawing->layers[gui->layer_idx].name);
				dxf_attr_change_i(new_el, 10, &gui->step_x[gui->step], -1);
				dxf_attr_change_i(new_el, 20, &gui->step_y[gui->step], -1);
				//dxf_attr_change_i(new_el, 42, &gui->bulge, -1);
				dxf_attr_change(new_el, 370, &dxf_lw[gui->lw_idx]);
				dxf_attr_change(new_el, 62, &gui->color_idx);
				
				new_el->obj.graphics = dxf_graph_parse(gui->drawing, new_el, 0 , 1);
			}
		}
		
	}
	goto end_step;
	default_modal:
		gui->modal = SELECT;
	first_step:
		gui->en_distance = 0;
		gui->draw_tmp = 0;
		gui->element = NULL;
		gui->draw = 1;
		gui->step = 0;
		gui->draw_phanton = 0;
		//if (gui->phanton){
		//	gui->phanton = NULL;
		//}
	next_step:
		
		gui->lock_ax_x = 0;
		gui->lock_ax_y = 0;
		gui->user_flag_x = 0;
		gui->user_flag_y = 0;

		gui->draw = 1;
	end_step:
		return 1;
}

int gui_hatch_info (gui_obj *gui){
	if (gui->modal == HATCH) {
		
		nk_layout_row_dynamic(gui->ctx, 20, 1);
		
		gui->user_patt.ang = nk_propertyd(gui->ctx, "Angle", -180.0d, gui->user_patt.ang, 180.0d, 0.5d, 0.5d);
		gui->user_patt.dy = nk_propertyd(gui->ctx, "Spacing", 0.0d, gui->user_patt.dy, DBL_MAX, 0.1d, 0.1d);
		if (nk_button_label(gui->ctx, "By Selection")) {
			
				
				#if(0)
				/*--------------------------------------
				test **** test **** test *** test ****/
				
				if (gui->sel_list != NULL){
					
					
				list_node *current = gui->sel_list->next, *curr_gr_n, *grph_list;
				dxf_node *curr_ent;
				graph_obj *curr_graph = NULL, *hatch;
				
				
				// starts the content sweep 
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							
							curr_ent = (dxf_node *)current->data;
							
							grph_list = curr_ent->obj.graphics;
							
							if (grph_list != NULL){
								curr_gr_n = grph_list->next;
								
								/* sweep the main curr_ent->obj.graphics */
								while (curr_gr_n != NULL){
									if (curr_gr_n->data){
										curr_graph = (graph_obj *)curr_gr_n->data;
										hatch = graph_hatch(curr_graph, hatch_angle*M_PI/180,
										0.0, 0.0, hatch_spacing, 0.0, (double[]){1}, 1, 0);
										if (hatch){
											list_push(grph_list, list_new((void *)hatch, 0));
										}
									}
									curr_gr_n = curr_gr_n->next;
								}
							}
						}
					}
					current = current->next;
				}
				//list_clear(gui->sel_list);
			}
			/*------------------------------*/
			#endif
		}
		if (gui->step == 0){
			nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
		} else {
			nk_label(gui->ctx, "Enter next point", NK_TEXT_LEFT);
		}
	}
	return 1;
}