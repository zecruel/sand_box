#include "gui_use.h"
#include <float.h>

int gui_hatch_interactive(gui_obj *gui){
	if (gui->modal == HATCH){
		
		if (gui->ev & EV_CANCEL){
			goto default_modal;
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
		static double hatch_angle = 45.0;
		static double hatch_spacing = 1.0;
		
		nk_layout_row_dynamic(gui->ctx, 20, 1);
		
		hatch_angle = nk_propertyd(gui->ctx, "Angle", -180.0d, hatch_angle, 180.0d, 0.5d, 0.5d);
		hatch_spacing = nk_propertyd(gui->ctx, "Spacing", 0.0d, hatch_spacing, DBL_MAX, 0.1d, 0.1d);
		if (nk_button_label(gui->ctx, "By Selection")) {
			
				
				
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
		}
	}
	return 1;
}