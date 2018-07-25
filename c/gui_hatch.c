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
					gui->hatch_solid, gui->hatch_assoc,
					0, 0, 0.0, 1.0,
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
		static int show_pat_pp = 0;
		
		nk_layout_row_dynamic(gui->ctx, 20, 3);
		nk_checkbox_label(gui->ctx, "Associative", &gui->hatch_assoc);
		
		nk_layout_row_dynamic(gui->ctx, 20, 3);
		if (nk_selectable_label(gui->ctx, "User", NK_TEXT_CENTERED, &gui->hatch_user)){
			if(gui->hatch_user) {
				gui->hatch_predef = 0;
				gui->hatch_solid = 0;
				
				gui->hatch_idx = 0;
			}
		}
		if (nk_selectable_label(gui->ctx, "Library", NK_TEXT_CENTERED, &gui->hatch_predef)){
			if(gui->hatch_predef) {
				gui->hatch_user = 0;
				gui->hatch_solid = 0;
			}
		}
		if (nk_selectable_label(gui->ctx, "Solid", NK_TEXT_CENTERED, &gui->hatch_solid)){
			if(gui->hatch_solid) {
				gui->hatch_predef = 0;
				gui->hatch_user = 0;
			}
		}
		
		if (gui->hatch_user){
			nk_layout_row_dynamic(gui->ctx, 20, 1);
			
			gui->user_patt.ang = nk_propertyd(gui->ctx, "Angle", 0.0d, gui->user_patt.ang, 360.0d, 0.5d, 0.5d);
			gui->user_patt.dy = nk_propertyd(gui->ctx, "Spacing", 0.0d, gui->user_patt.dy, DBL_MAX, 0.1d, 0.1d);
		}
		else if (gui->hatch_predef){
			struct h_pattern *curr_h = &(gui->list_pattern);
			int i = 0;
			
			while (curr_h){
				if (gui->hatch_idx == i){
					
					strncpy(gui->patt_name, curr_h->name, DXF_MAX_CHARS);
					strncpy(gui->patt_descr, curr_h->descr, DXF_MAX_CHARS);
				}
				
				i++;
				curr_h = curr_h->next;
			}
			
			nk_layout_row_dynamic(gui->ctx, 20, 2);
			nk_label(gui->ctx, "Pattern:", NK_TEXT_CENTERED);
			nk_label(gui->ctx, gui->patt_name, NK_TEXT_CENTERED);
			nk_layout_row_dynamic(gui->ctx, 20, 1);
			nk_label(gui->ctx, gui->patt_descr, NK_TEXT_CENTERED);
			
			gui->patt_scale = nk_propertyd(gui->ctx, "Scale", 0.0d, gui->patt_scale, DBL_MAX, 0.1d, 0.1d);
			gui->patt_ang = nk_propertyd(gui->ctx, "Angle", 0.0d, gui->patt_ang, 360.0d, 0.5d, 0.5d);
			
			
			if (nk_button_label(gui->ctx, "Explore")) show_pat_pp = 1;
			if (show_pat_pp){
				/* select block popup */
				static struct nk_rect s = {100, 10, 420, 330};
				if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Select Pattern", NK_WINDOW_CLOSABLE, s)){
					graph_obj *ref_graph = NULL, *curr_graph = NULL;
					list_node * pat_g = NULL;
					
					int pat_ei = 0; /*extents flag */
					/* extents and zoom parameters */
					double pat_x0, pat_y0, pat_x1, pat_y1, z, z_x, z_y, o_x, o_y;
					double cosine, sine, dx, dy, max;
					
					nk_layout_row_dynamic(gui->ctx, 225, 2);
					if (nk_group_begin(gui->ctx, "Patt_names", NK_WINDOW_BORDER)) {
						nk_layout_row_dynamic(gui->ctx, 20, 1);
						curr_h = &(gui->list_pattern);
						i = 0;
						while (curr_h){
							if (nk_button_label(gui->ctx, curr_h->name)){
								gui->hatch_idx = i;
								strncpy(gui->patt_name, curr_h->name, DXF_MAX_CHARS);
								strncpy(gui->patt_descr, curr_h->descr, DXF_MAX_CHARS);
							}
							
							i++;
							curr_h = curr_h->next;
						}
						nk_group_end(gui->ctx);
					}
					
					/*get current hatch */
					curr_h = &(gui->list_pattern);
					i = 0;
					while (curr_h){
						if (gui->hatch_idx == i) break;
						
						i++;
						curr_h = curr_h->next;
					}
					
					struct hatch_line *curr_l = NULL;
					if (curr_h){
						max = 0.0;
						double patt_len = 0.0;
						
						curr_l = curr_h->lines;
						while (curr_l){
							/*
							for (i = 0; i < curr_l->num_dash; i++ ){
								patt_len += fabs(curr_l->dash[i]);
							}*/
							
							if (curr_l->num_dash < 2)
								max = (max > sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy))? max : sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy);
							else
								max = (max > sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy)/curr_l->num_dash)? max : sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy)/curr_l->num_dash;
							
							curr_l = curr_l->next;
						}
							
						max *= 10;
						
						pat_g = list_new(NULL, FRAME_LIFE);
						
						/*create reference graph*/
						ref_graph = graph_new(FRAME_LIFE);
						line_add(ref_graph, 0.0, 0.0, 0.0, max, 00.0, 0.0);
						line_add(ref_graph, max, 0.0, 0.0, max, max, 0.0);
						line_add(ref_graph, max, max, 0.0, 0.0, max, 0.0);
						line_add(ref_graph, 0.0, max, 0.0, 0.0, 0.0, 0.0);
						
						curr_l = curr_h->lines;
					}
			
					while (curr_l){
						
						cosine = cos(curr_l->ang * M_PI/180);
						sine = sin(curr_l->ang * M_PI/180);
						dx = cosine*curr_l->dx - sine*curr_l->dy;
						dy = sine*curr_l->dx + cosine*curr_l->dy;
						
						if (curr_l->num_dash > 0){
							curr_graph = graph_hatch(ref_graph, curr_l->ang * M_PI/180,
								curr_l->ox, curr_l->oy,
								dx, dy,
								curr_l->dash, curr_l->num_dash,
								FRAME_LIFE);
						}
						else{
							curr_graph = graph_hatch(ref_graph, curr_l->ang * M_PI/180,
								curr_l->ox, curr_l->oy,
								dx, dy,
								(double[]){1.0}, 1,
								FRAME_LIFE);
						}
						
						if ((curr_graph != NULL) && (pat_g != NULL)) list_push(pat_g, list_new((void *)curr_graph, FRAME_LIFE));
						
						
						
						curr_l = curr_l->next;
					}
					
					graph_list_ext(pat_g, &pat_ei, &pat_x0, &pat_y0, &pat_x1, &pat_y1);
						
					/* calcule the zoom and offset for preview */
					z_x = fabs(pat_x1 - pat_x0)/gui->preview_img->width;
					z_y = fabs(pat_y1 - pat_y0)/gui->preview_img->height;
					z = (z_x > z_y) ? z_x : z_y;
					if (z <= 0) z =1;
					else z = 1/(1.1 * z);
					o_x = pat_x0 - (fabs((pat_x1 - pat_x0)*z - gui->preview_img->width)/2)/z;
					o_y = pat_y0 - (fabs((pat_y1 - pat_y0)*z - gui->preview_img->height)/2)/z;
					
					/* draw graphics in preview bitmap */
					bmp_fill(gui->preview_img, gui->preview_img->bkg); /* clear bitmap */
					graph_list_draw(pat_g, gui->preview_img, o_x, o_y, z);
					
					
					if (nk_group_begin(gui->ctx, "Patt_prev", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
						/* preview img */
						nk_layout_row_dynamic(gui->ctx, 175, 1);
						nk_button_image(gui->ctx,  nk_image_ptr(gui->preview_img));
						
						/* current pattern name */
						nk_layout_row_dynamic(gui->ctx, 20, 1);
						nk_label(gui->ctx, gui->patt_name, NK_TEXT_CENTERED);
						
						/* current pattern description */
						nk_layout_row_dynamic(gui->ctx, 15, 1);
						//nk_style_push_font(gui->ctx, &font_tiny_nk); /* change font to tiny*/
						nk_label(gui->ctx, gui->patt_descr, NK_TEXT_CENTERED);
						//nk_style_pop_font(gui->ctx); /* back to default font*/
						
						nk_group_end(gui->ctx);
					}
					
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					
					//nk_checkbox_label(gui->ctx, "Hidden", &show_hidden_blks);
					
					if (nk_button_label(gui->ctx, "Select")){
						show_pat_pp = 0;
						nk_popup_close(gui->ctx);
					}
					
					nk_popup_end(gui->ctx);
				}
				else show_pat_pp = 0;
			}
		}
		
		
		nk_layout_row_dynamic(gui->ctx, 20, 1);
		if (gui->step == 0){
			nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
		} else {
			nk_label(gui->ctx, "Enter next point", NK_TEXT_LEFT);
		}
	}
	return 1;
}