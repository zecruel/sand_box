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
					
					struct h_pattern *curr_h;// = &(gui->list_pattern);
					struct h_family *curr_fam = gui->hatch_fam.next;
					int i = 0;
					
					if(gui->hatch_user) {
						curr_h = &(gui->list_pattern);
					}
					else{
						curr_h = NULL;
						
						while (curr_fam){
							if (gui->hatch_fam_idx == i){
								curr_h = curr_fam->list->next;
								break;
							}
							
							i++;
							curr_fam = curr_fam->next;
						}
						i = 0;
						
						while ((curr_h) && (i < gui->hatch_idx)){
							i++;
							curr_h = curr_h->next;
						}
					}
					dxf_node *new_hatch_el = dxf_new_hatch (curr_h, bound,
					gui->hatch_solid, gui->hatch_assoc,
					0, 0, /* style, type */
					gui->patt_ang, gui->patt_scale,
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
		static int show_pat_pp = 0, show_pat_file = 0;
		struct h_pattern *curr_h = NULL;
		struct h_family *curr_fam = gui->hatch_fam.next;
		static double patt_scale = 1, patt_rot = 0.0;
		
		int i = 0;
		//nk_layout_row_dynamic(gui->ctx, 20, 3);
		nk_layout_row(gui->ctx, NK_STATIC, 20, 3, (float[]){40, 60, 40});
		if (nk_selectable_label(gui->ctx, "User", NK_TEXT_CENTERED, &gui->hatch_user)){
			if(gui->hatch_user) {
				gui->hatch_predef = 0;
				gui->hatch_solid = 0;
				gui->patt_scale = 1;
				gui->patt_ang = 0.0;
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
		
		nk_layout_row_dynamic(gui->ctx, 125, 1);
		if (nk_group_begin(gui->ctx, "Patt_controls", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
		
			if (gui->hatch_user){
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				
				gui->user_patt.ang = nk_propertyd(gui->ctx, "Angle", 0.0d, gui->user_patt.ang, 360.0d, 0.5d, 0.5d);
				gui->user_patt.dy = nk_propertyd(gui->ctx, "Spacing", 0.0d, gui->user_patt.dy, DBL_MAX, 0.1d, 0.1d);
			}
			else if (gui->hatch_predef){
				curr_fam = gui->hatch_fam.next;
				i = 0;
				
				gui->patt_name[0] = 0;
				gui->patt_descr[0] = 0;
				curr_h = NULL;
				
				while (curr_fam){
					if (gui->hatch_fam_idx == i){
						
						strncpy(gui->h_fam_name, curr_fam->name, DXF_MAX_CHARS);
						strncpy(gui->h_fam_descr, curr_fam->descr, DXF_MAX_CHARS);
						curr_h = curr_fam->list->next;
					}
					
					i++;
					curr_fam = curr_fam->next;
				}
				
				
				
				//curr_h = &(gui->list_pattern);
				i = 0;
				
				while (curr_h){
					if (gui->hatch_idx == i){
						
						strncpy(gui->patt_name, curr_h->name, DXF_MAX_CHARS);
						strncpy(gui->patt_descr, curr_h->descr, DXF_MAX_CHARS);
					}
					
					i++;
					curr_h = curr_h->next;
				}
				
				
				nk_layout_row(gui->ctx, NK_DYNAMIC, 20, 2, (float[]){0.85f, 0.15f});
				//nk_label_colored(gui->ctx, gui->patt_descr, NK_TEXT_CENTERED, nk_rgb(100,115,255));
				if (nk_combo_begin_label(gui->ctx, gui->h_fam_name, nk_vec2(150,300))){
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					curr_fam = gui->hatch_fam.next;
					i = 0;
					while (curr_fam){
						if (nk_button_label(gui->ctx, curr_fam->name)){
							gui->hatch_fam_idx = i;
							gui->hatch_idx = 0;
							nk_combo_close(gui->ctx);
						}
						i++;
						curr_fam = curr_fam->next;
					}
					
					nk_combo_end(gui->ctx);
				}
				if (nk_button_symbol(gui->ctx, NK_SYMBOL_PLUS)) show_pat_file = 1;
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				if (nk_button_label(gui->ctx, "Explore")) show_pat_pp = 1;
				
				nk_layout_row(gui->ctx, NK_DYNAMIC, 20, 2, (float[]){0.2f, 0.8f});
				nk_label(gui->ctx, "Name:", NK_TEXT_RIGHT);
				nk_label_colored(gui->ctx, gui->patt_name, NK_TEXT_CENTERED, nk_rgb(255,255,0));
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				gui->patt_scale = nk_propertyd(gui->ctx, "#Scale", 0.0d, gui->patt_scale, DBL_MAX, 0.1d, 0.1d);
				gui->patt_ang = nk_propertyd(gui->ctx, "Angle", 0.0d, gui->patt_ang, 360.0d, 0.5d, 0.5d);
			}
			nk_group_end(gui->ctx);
		}
		
		nk_layout_row_dynamic(gui->ctx, 20, 1);
		nk_checkbox_label(gui->ctx, "Associative", &gui->hatch_assoc);
		
		if (gui->step == 0){
			nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
		} else {
			nk_label(gui->ctx, "Enter next point", NK_TEXT_LEFT);
		}
		
		if (show_pat_pp){
			/* select block popup */
			static int patt_idx = 0, last_idx = 0;
			static char patt_name[DXF_MAX_CHARS], patt_descr[DXF_MAX_CHARS];
			static struct nk_rect s = {120, -210, 420, 490};
			if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Select Pattern", NK_WINDOW_CLOSABLE, s)){
				graph_obj *ref_graph = NULL, *curr_graph = NULL;
				list_node * pat_g = NULL;
				
				int pat_ei = 0; /*extents flag */
				/* extents and zoom parameters */
				double pat_x0, pat_y0, pat_x1, pat_y1, z, z_x, z_y, o_x, o_y;
				double cosine, sine, dx, dy, max;
				double ang, ox, oy, dash[20];
				int num_dash;
				
				curr_fam = gui->hatch_fam.next;
				i = 0;
				curr_h = NULL;
				
				while (curr_fam){
					if (gui->hatch_fam_idx == i){
						curr_h = curr_fam->list->next;
					}
					
					i++;
					curr_fam = curr_fam->next;
				}
				
				nk_layout_row(gui->ctx, NK_DYNAMIC, 20, 2, (float[]){0.15f, 0.85f});
				nk_label(gui->ctx, "Family:", NK_TEXT_RIGHT);
				nk_label_colored(gui->ctx, gui->h_fam_name, NK_TEXT_LEFT, nk_rgb(255,255,0));
				nk_layout_row_dynamic(gui->ctx, 50, 1);
				nk_label_colored_wrap(gui->ctx, gui->h_fam_descr, nk_rgb(100,115,255));
				
				nk_layout_row_dynamic(gui->ctx, 360, 2);
				if (nk_group_begin(gui->ctx, "Patt_names", NK_WINDOW_BORDER)) {
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					//curr_h = &(gui->list_pattern);
					i = 0;
					while (curr_h){
						if (nk_button_label(gui->ctx, curr_h->name)){
							patt_idx = i;
						}
						
						i++;
						curr_h = curr_h->next;
					}
					nk_group_end(gui->ctx);
				}
				
				/*get current hatch */
				//curr_h = &(gui->list_pattern);
				curr_fam = gui->hatch_fam.next;
				i = 0;
				curr_h = NULL;
				
				while (curr_fam){
					if (gui->hatch_fam_idx == i){
						curr_h = curr_fam->list->next;
					}
					
					i++;
					curr_fam = curr_fam->next;
				}
				i = 0;
				while (curr_h){
					strncpy(patt_name, curr_h->name, DXF_MAX_CHARS);
					strncpy(patt_descr, curr_h->descr, DXF_MAX_CHARS);
					if (patt_idx == i) break;
					
					i++;
					curr_h = curr_h->next;
				}
				
				struct hatch_line *curr_l = NULL;
				if (curr_h){
					
					max = 0.0;
					double patt_len = 0.0;
					
					if (patt_idx != last_idx){
						curr_l = curr_h->lines;
						while (curr_l){
							if (curr_l->num_dash < 2)
								max = (max > sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy))? max : sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy);
							else
								max = (max > sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy)/curr_l->num_dash)? max : sqrt(curr_l->dx*curr_l->dx + curr_l->dy*curr_l->dy)/curr_l->num_dash;
							curr_l = curr_l->next;
						}
						
						if (max > 0.0) patt_scale = 1/max;
						else patt_scale = 1.0;
						
						if (curr_h->num_lines > 1) patt_scale *= sqrt(curr_h->num_lines);
						
						patt_rot = 0.0;
						
						last_idx = patt_idx;
					}
					
					pat_g = list_new(NULL, FRAME_LIFE);
					
					/*create reference graph*/
					ref_graph = graph_new(FRAME_LIFE);
					line_add(ref_graph, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0);
					line_add(ref_graph, 10.0, 0.0, 0.0, 10.0, 10.0, 0.0);
					line_add(ref_graph, 10.0, 10.0, 0.0, 0.0, 10.0, 0.0);
					line_add(ref_graph, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0);
					
					curr_l = curr_h->lines;
				}
		
				while (curr_l){
					ang = fmod(curr_l->ang + patt_rot, 360.0);
					
					cosine = cos(ang * M_PI/180);
					sine = sin(ang * M_PI/180);
					dx = patt_scale * (cosine*curr_l->dx - sine*curr_l->dy);
					dy = patt_scale * (sine*curr_l->dx + cosine*curr_l->dy);
					cosine = cos(patt_rot * M_PI/180);
					sine = sin(patt_rot * M_PI/180);
					ox = patt_scale * (cosine*curr_l->ox - sine*curr_l->oy);
					oy = patt_scale * (sine*curr_l->ox + cosine*curr_l->oy);
					num_dash = curr_l->num_dash;
					
					for (i = 0; i < num_dash; i++){
						dash[i] = patt_scale * curr_l->dash[i];
					}
					if (num_dash == 0) {
						dash[0] = 1.0;
						num_dash = 1;
					}
					curr_graph = graph_hatch(ref_graph, ang * M_PI/180,
						ox, oy,
						dx, dy,
						dash, num_dash,
						FRAME_LIFE);
					
					if ((curr_graph != NULL) && (pat_g != NULL)){
						/*change color*/
						curr_graph->color.r = 255;// - gui->preview_img->bkg.r;
						curr_graph->color.g = 255;// - gui->preview_img->bkg.g;
						curr_graph->color.b = 255;// - gui->preview_img->bkg.b;
						
						list_push(pat_g, list_new((void *)curr_graph, FRAME_LIFE));
					}
					
					
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
					/* current pattern name */
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label_colored(gui->ctx, patt_name, NK_TEXT_CENTERED, nk_rgb(255,255,0));
					
					/* preview img */
					nk_layout_row_dynamic(gui->ctx, 175, 1);
					nk_button_image(gui->ctx,  nk_image_ptr(gui->preview_img));
					nk_layout_row_dynamic(gui->ctx, 50, 1);
					nk_label_colored_wrap(gui->ctx, patt_descr, nk_rgb(100,115,255));
					
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Ref: 10 x 10 units", NK_TEXT_CENTERED);
					patt_scale = nk_propertyd(gui->ctx, "#Scale", 0.001, patt_scale, DBL_MAX, 0.001, 0.001);
					patt_rot = nk_propertyd(gui->ctx, "#Rotation", 0.00, patt_rot, 360.0, 0.1, 0.1);
					
					if (nk_button_label(gui->ctx, "Select")){
						gui->hatch_idx = patt_idx;
						gui->patt_scale = patt_scale;
						gui->patt_ang = patt_rot;
						show_pat_pp = 0;
					}
					
					
					nk_group_end(gui->ctx);
				}
				
				nk_popup_end(gui->ctx);
			}
			else show_pat_pp = 0;
		}
		if (show_pat_file){
			/* file open popup */
			static char pat_path[DXF_MAX_CHARS];
			static int pat_path_len = 0;
			
			static struct nk_rect s = {20, 100, 400, 150};
			if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Add pattern family", NK_WINDOW_CLOSABLE, s)){
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				nk_label(gui->ctx, "File to Open:", NK_TEXT_CENTERED);
				//nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, pat_path, DXF_MAX_CHARS, nk_filter_default);
				nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
				nk_edit_string(gui->ctx, NK_EDIT_SIMPLE | NK_EDIT_CLIPBOARD, pat_path, &pat_path_len, DXF_MAX_CHARS, nk_filter_default);
				
				nk_layout_row_dynamic(gui->ctx, 20, 2);
				if (nk_button_label(gui->ctx, "OK")) {
					pat_path[pat_path_len] = 0;
					if (strlen(pat_path) > 4){
						
						gui->end_fam->next = dxf_hatch_family_file(NULL, pat_path);
						if(gui->end_fam->next) gui->end_fam = gui->end_fam->next;
					}
					show_pat_file = nk_false;
					pat_path_len = 0;
				}
				nk_popup_end(gui->ctx);
			} else {
				show_pat_file = nk_false;
				pat_path_len = 0;
			}
		}
	}
	return 1;
}