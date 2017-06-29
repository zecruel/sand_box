#include <SDL.h>

#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "shape2.h"
#include "dxf_graph.h"

#include "dxf_colors.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
//#include <locale.h>

#include "tinyfiledialogs.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_ZERO_COMMAND_MEMORY
#include "nuklear.h"
#include "gui.h"

void draw_cursor(bmp_img *img, int x, int y, bmp_color color){
	/* draw cursor */
	/* set the pattern */
	double pat = 1;
	patt_change(img, &pat, 1);
	/* set the color */
	img->frg = color;
	/* set the tickness */
	img->tick = 3;
	bmp_line(img, 0, y, img->width, y);
	bmp_line(img, x, 0, x, img->height);
	img->tick = 1;
	bmp_line(img, x-5, y+5, x+5, y+5);
	bmp_line(img, x-5, y-5, x+5, y-5);
	bmp_line(img, x+5, y-5, x+5, y+5);
	bmp_line(img, x-5, y-5, x-5, y+5);
}

int main(int argc, char** argv){
	//setlocale(LC_ALL,""); //seta a localidade como a current do computador para aceitar acentuacao
	
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y, zoom, ofs_x, ofs_y;
	double prev_zoom;
	int color_idx = 256;
	int layer_idx = 0;
	dxf_node *element = NULL, *prev_el = NULL;
	double pos_x, pos_y;
	
	unsigned int width = 1024;
	unsigned int height = 720;
	int open_prg = 0;
	int progress = 0;
	unsigned int quit = 0;
	unsigned int wait_open = 0;
	int i;
	int ev_type, draw = 0;
	int present = 1, draw_gui = 1, draw_canvas = 1, draw_motion = 1;
	struct nk_color background;
	
	int leftMouseButtonDown = 0;
	
	SDL_Event event;
	int mouse_x, mouse_y;
	
	double rect_pt1[2], rect_pt2[2];
	int low_proc = 1;
	
	enum Action {
		NONE,
		FILE_OPEN,
		FILE_SAVE,
		EXIT
	} action = NONE;
	
	char *url = NULL;
	char const * lFilterPatterns[2] = { "*.dxf", "*.txt" };
	
	/* Colors in use */
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	bmp_color grey = {.r = 100, .g = 100, .b = 100, .a = 255};
	bmp_color hilite = {.r = 255, .g = 0, .b = 255, .a = 150};
	bmp_color transp = {.r = 255, .g = 255, .b = 255, .a = 0};
	bmp_color cursor = {.r = 255, .g = 255, .b = 255, .a = 100};
	background = nk_rgb(28,48,62);
	
	/* line types in use */
	double center [] = {12, -6, 2 , -6};
	double dash [] = {8, -8};
	double continuous[] = {1};
	
	/* init the main images */
	bmp_img * img = bmp_new(width, height, grey, red);
	bmp_img * tmp_img = bmp_new(width, height, transp, hilite);
	
	/* init Nuklear GUI */
	shape *shx_font = shx_font_open("txt.shx");
	gui_obj *gui = nk_sdl_init(shx_font);
	
	/* init the drawing */
	dxf_drawing *drawing = malloc(sizeof(dxf_drawing));
	url = NULL; /* pass a null file only for initialize the drawing structure */
	dxf_open(drawing, url);
	dxf_ents_parse(drawing);
	dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);

	/* init the SDL2 */
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow(
		"DXF Viewer with SDL2", /* title */
		SDL_WINDOWPOS_UNDEFINED, /* x position */
		SDL_WINDOWPOS_UNDEFINED, /* y position */
		width, /* width */
		height, /* height */
		0); /* flags */
	

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	/*
	SDL_RendererInfo info;
	if (SDL_GetRenderDriverInfo(0, &info) == 0){
		//printf("Driver num pix format = %d\n", info.num_texture_formats);
		for (i = 0; i < info.num_texture_formats; i++){
			switch (info.texture_formats[i]){
				case SDL_PIXELFORMAT_ARGB8888:
					//printf("pix format ARGB8888\n");
					//img->r_i = 2;
					//img->g_i = 1;
					//img->b_i = 0;
					//img->a_i = 3;
					break;
				case SDL_PIXELFORMAT_RGBA8888:
					//printf("pix format RGBA8888\n");
					break;
				case SDL_PIXELFORMAT_ABGR8888:
					//printf("pix format ABGR8888\n");
					break;
				case SDL_PIXELFORMAT_BGRA8888:
					//printf("pix format BGRA8888\n");
					break;
			}
		}
	}
	*/
	
	SDL_Texture * canvas = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		width, /* width */
		height); /* height */
		
	SDL_Texture * tmp_tex = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		width, /* width */
		height); /* height */
		
	SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(tmp_tex, SDL_BLENDMODE_BLEND);
	
	/* main loop */
	while (quit == 0){
		ev_type = 0;
		low_proc = 1;
		
		//SDL_ShowCursor(SDL_DISABLE);
		
		/* get events for Nuklear GUI input */
		nk_input_begin(gui->ctx);
		if(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) quit = 1;
			nk_sdl_handle_event(gui, window, &event);
			ev_type = event.type;
		}
		nk_input_end(gui->ctx);
		/* ===============================*/
		
		/* GUI */
		/* main toolbox, for open files, save or exit */
		if (nk_begin(gui->ctx, "Main", nk_rect(5, 5, 200, 40),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE)){
			/*
			nk_layout_row_dynamic(gui->ctx, 30, 1);
			if (nk_button_label(gui->ctx, "Open")){
				action = FILE_OPEN;
			}
			if (nk_button_label(gui->ctx, "Save")){
				action = FILE_SAVE;
			}
			if (nk_button_label(gui->ctx, "Exit")){
				action = EXIT;
				quit = 1;
			}*/
			/*main menu */
			nk_menubar_begin(gui->ctx);
			nk_layout_row_dynamic(gui->ctx, 15, 3);
			/* new menu*/
			if (nk_menu_begin_label(gui->ctx, "FILE", NK_TEXT_LEFT, nk_vec2(120, 200))){
				nk_layout_row_dynamic(gui->ctx, 15, 1);
				if (nk_menu_item_label(gui->ctx, "Open", NK_TEXT_LEFT)){
					action = FILE_OPEN;
				}
				if (nk_menu_item_label(gui->ctx, "Save", NK_TEXT_LEFT)){
					action = FILE_SAVE;
				}
				if (nk_menu_item_label(gui->ctx, "Exit", NK_TEXT_LEFT)){
					action = EXIT;
					quit = 1;
				}
				
				nk_menu_end(gui->ctx);
			}
			/* new menu*/
			if (nk_menu_begin_label(gui->ctx, "VIEW", NK_TEXT_LEFT, nk_vec2(120, 200))){
				nk_layout_row_dynamic(gui->ctx, 15, 1);
				if (nk_menu_item_label(gui->ctx, "Extend", NK_TEXT_LEFT)){
					
					dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
					zoom_x = (max_x - min_x)/img->width;
					zoom_y = (max_y - min_y)/img->height;
					zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
					if (zoom <= 0){ zoom =1;}
					else{ zoom = 1/(1.1 * zoom);}
					
					ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
					ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
					draw = 1;
				}
				if (nk_menu_item_label(gui->ctx, "Zoom in", NK_TEXT_LEFT)){
					//action = FILE_OPEN;
				}
				nk_menu_end(gui->ctx);
			}
			/* new menu*/
			if (nk_menu_begin_label(gui->ctx, "HELP", NK_TEXT_LEFT, nk_vec2(120, 200))){
				nk_layout_row_dynamic(gui->ctx, 15, 1);
				if (nk_menu_item_label(gui->ctx, "About", NK_TEXT_LEFT)){
					//action = FILE_OPEN;
				}
				
				nk_menu_end(gui->ctx);
			}
			
			nk_menubar_end(gui->ctx);
		}
		nk_end(gui->ctx);
		
		
		/*if (wait_open != 0){
			/* opening 
			//if (nk_begin(gui->ctx, "opening", nk_rect(200, 200, 400, 40),
			//NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
			//NK_WINDOW_SCALABLE|NK_WINDOW_NO_SCROLLBAR))
			if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "opening", 0, nk_rect(200, 200, 400, 40)))
			{
				static char text[64];
				static int text_len;
				nk_layout_row_dynamic(gui->ctx, 20, 2);
				text_len = snprintf(text, 63, "Opening...");
				nk_label(gui->ctx, text, NK_TEXT_LEFT);
				nk_progress(gui->ctx, &progress, 100, NK_FIXED);
				nk_popup_end(gui->ctx);
			}
			//nk_end(gui->ctx);
		}*/
		
		/* status */
		if (nk_begin(gui->ctx, "status", nk_rect(415, height - 45, 600, 40),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
		NK_WINDOW_SCALABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			static char comm[64];
			static int comm_len;
			
			static char text[64];
			static int text_len;
			
			nk_layout_row_dynamic(gui->ctx, 25, 4);
			
			nk_flags res = nk_edit_string(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER, comm, &comm_len, 64, nk_filter_default);
			//NK_EDIT_ACTIVE
			if (res & NK_EDIT_COMMITED){
				comm[comm_len] = 0;
				printf("%s\n", comm);
				comm_len = 0;
			}
			
			text_len = snprintf(text, 63, "Layers=%d", drawing->num_layers);
			nk_label(gui->ctx, text, NK_TEXT_LEFT);
			
			if (wait_open != 0){
				text_len = snprintf(text, 63, "Opening...");
				nk_label(gui->ctx, text, NK_TEXT_LEFT);
				nk_progress(gui->ctx, &progress, 100, NK_FIXED);
			}
		}
		nk_end(gui->ctx);
		
		/* view coordinates of mouse in drawing units */
		if (nk_begin(gui->ctx, "POS", nk_rect(5, height - 45, 400, 40),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
		NK_WINDOW_SCALABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			static char text[64];
			static int text_len;
			float ratio[] = {0.1f, 0.4f, 0.1f, 0.4f};
			nk_layout_row(gui->ctx, NK_DYNAMIC, 30, 4, ratio);
			nk_label(gui->ctx, "X=", NK_TEXT_RIGHT);
			text_len = snprintf(text, 63, "%f", pos_x);
			nk_edit_string(gui->ctx, NK_EDIT_SIMPLE, text, &text_len, 64, nk_filter_float);
			nk_label(gui->ctx, "Y=", NK_TEXT_RIGHT);
			text_len = snprintf(text, 63, "%f", pos_y);
			nk_edit_string(gui->ctx, NK_EDIT_SIMPLE, text, &text_len, 64, nk_filter_float);
		}
		nk_end(gui->ctx);
		
		if (nk_begin(gui->ctx, "Prop", nk_rect(205, 5, 400, 40),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|
		NK_WINDOW_SCALABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			static char text[64];
			int text_len;
			
			/*layer*/
			nk_layout_row_begin(gui->ctx, NK_STATIC, 20, 3);
			
			
			nk_layout_row_push(gui->ctx, 200);
			if (nk_combo_begin_label(gui->ctx, drawing->layers[layer_idx].name, nk_vec2(200,200))){
				nk_layout_row_dynamic(gui->ctx, 25, 1);
				int num_layers = drawing->num_layers;
				for (i = 0; i < num_layers; i++){
					//strcpy(layer_nam[i], drawing->layers[i].name);
					if (nk_button_label(gui->ctx, drawing->layers[i].name)){
						layer_idx = i;
						nk_combo_close(gui->ctx);
					}
				}
				
				nk_combo_end(gui->ctx);
			}
			/*color picker */
			int c_idx = color_idx;
			if (c_idx >255){
				c_idx = drawing->layers[layer_idx].color;
				if (c_idx >255){
					c_idx = 0;
				}
			}
			struct nk_color combo_color = {
				.r = dxf_colors[c_idx].r,
				.g = dxf_colors[c_idx].g,
				.b = dxf_colors[c_idx].b,
				.a = dxf_colors[c_idx].a
			};
			nk_layout_row_push(gui->ctx, 40);
			struct nk_style_button b_style = gui->ctx->style.button;
			b_style.normal.data.color = combo_color;
			combo_color.a = 120;
			b_style.hover.data.color = combo_color;
			
			if (color_idx <256){
				text_len = snprintf(text, 63, "%d", color_idx);
			}
			else{
				text_len = snprintf(text, 63, "%s", "ByL");
			}
			
			nk_button_text_styled(gui->ctx, &b_style, text, text_len);
			//if(nk_button_color(gui->ctx, combo_color)){
				//
			//}
			nk_layout_row_push(gui->ctx, 25);
			if (nk_combo_begin_color(gui->ctx, combo_color, nk_vec2(180,320))) {
				nk_layout_row_static(gui->ctx, 15, 15, 8);
				for (i = 0; i < 256; i++){
					struct nk_color b_color = {
						.r = dxf_colors[i].r,
						.g = dxf_colors[i].g,
						.b = dxf_colors[i].b,
						.a = dxf_colors[i].a
					};
					if(nk_button_color(gui->ctx, b_color)){
						color_idx = i;
						nk_combo_close(gui->ctx);
					}
				}
				nk_layout_row_dynamic(gui->ctx, 20, 1);
					if (nk_button_label(gui->ctx, "By Layer")){
						color_idx = 256;
						nk_combo_close(gui->ctx);
					}
				nk_combo_end(gui->ctx);
			}
			nk_layout_row_end(gui->ctx);
		}
		nk_end(gui->ctx);
		
		
		if (nk_window_is_any_hovered(gui->ctx)) {
			SDL_ShowCursor(SDL_ENABLE);
		}
		else{
			SDL_ShowCursor(SDL_DISABLE);
			if (ev_type != 0){
				
				switch (event.type){
					case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_LEFT){
							leftMouseButtonDown = 0;
							
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (event.button.button == SDL_BUTTON_LEFT){
							leftMouseButtonDown = 1;
							SDL_GetMouseState(&mouse_x, &mouse_y);
							mouse_y = height - mouse_y;
							
						}
						break;
					case SDL_MOUSEMOTION:{
						
						mouse_x = event.motion.x;
						mouse_y = event.motion.y;
						mouse_y = height - mouse_y;
						pos_x = (double) mouse_x/zoom + ofs_x;
						pos_y = (double) mouse_y/zoom + ofs_y;
						
						rect_pt1[0] = (double) (mouse_x - 5)/zoom + ofs_x;
						rect_pt1[1] = (double) (mouse_y - 5)/zoom + ofs_y;
						rect_pt2[0] = (double) (mouse_x + 5)/zoom + ofs_x;
						rect_pt2[1] = (double) (mouse_y + 5)/zoom + ofs_y;
						
						/* for hilite test */
						element = (dxf_node *)dxf_ents_isect(drawing, rect_pt1, rect_pt2);
						draw = 1;
					
						//if (leftMouseButtonDown){
							//int mouseX = event.motion.x;
							//int mouseY = event.motion.y;
							//pixels[mouseY * 640 + mouseX] = 0;
						//}
						}
						break;
					case SDL_MOUSEWHEEL:
						prev_zoom = zoom;
						zoom = zoom + event.wheel.y * 0.2 * zoom;
						
						SDL_GetMouseState(&mouse_x, &mouse_y);
						mouse_y = height - mouse_y;
						//ofs_x -= ((double) mouse_x)*(1/zoom - 1/prev_zoom);
						//ofs_y -= ((double) mouse_y)*(1/zoom - 1/prev_zoom);
						ofs_x += ((double) mouse_x)*(1/prev_zoom - 1/zoom);
						ofs_y += ((double) mouse_y)*(1/prev_zoom - 1/zoom);
						
						//bmp_fill(img, img->bkg); /* clear bitmap */
						//dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
						//graph_draw(t_open, img, 0, 0, 1);
						draw = 1;
						break;
				}
			}
		}
		
		/*if (((gui_check_draw(gui) != 0) || (draw != 0))&&(wait_open == 0)){
		
			bmp_fill(img, img->bkg); /* clear bitmap 
			dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw 
			
			
			nk_sdl_render(gui, tmp_img);
			
			present = 1;
			
			draw = 0;
			
		}*/
		
		if (wait_open != 0){
			low_proc = 0;
			//bmp_fill(img, img->bkg); /* clear bitmap */
			//dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
			
			//nk_sdl_render(gui, tmp_img);
			
			draw = 1;
			
			open_prg = dxf_open2(drawing, url, &progress);
			//printf(" == %d\n", open_prg);
			
			if(open_prg <= 0){
				//time_end = SDL_GetTicks();
				//printf("Open = %d\n", time_end - time_start);
				
				//printf("Num Layers = %d\n", drawing->num_layers);
				
				//time_start = SDL_GetTicks();
				dxf_ents_parse(drawing);
				//time_end = SDL_GetTicks();
				//printf("Parse = %d\n", time_end - time_start);
				
				dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
				zoom_x = (max_x - min_x)/img->width;
				zoom_y = (max_y - min_y)/img->height;
				zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
				if (zoom <= 0){ zoom =1;}
				else{ zoom = 1/(1.1 * zoom);}
				
				ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
				ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
				
				//bmp_fill(img, img->bkg); /* clear bitmap */
				
				//time_start = SDL_GetTicks();
				//dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
				
				//SDL_UpdateTexture(canvas, NULL, img->buf, width * 4);
				//SDL_RenderClear(renderer);
				//SDL_RenderCopy(renderer, canvas, NULL, NULL);
				//SDL_RenderPresent(renderer);
				//time_end = SDL_GetTicks();
				//printf("Draw = %d\n\n", time_end - time_start);
				
				draw = 1;
				layer_idx = 0;
				color_idx = 256;
				wait_open = 0;
				printf("Num Layers = %d\n", drawing->num_layers);
			}
			
		}
		
		
	
		
		
		/*===============================*/
		
		if(action == FILE_OPEN) {
			action = NONE;
			url = (char *) tinyfd_openFileDialog(
			"Open a Drawing",
			"",
			2,
			lFilterPatterns,
			NULL,
			0);
			if (url){
				dxf_mem_pool(ZERO_DXF, 0);
				graph_mem_pool(ZERO_GRAPH, 0);
				graph_mem_pool(ZERO_LINE, 0);
				
				wait_open = 1;
				progress = 0;
				/* clear drawing structure */
				open_prg = dxf_open2(drawing, NULL, &progress);
				progress = 0;
				low_proc = 0;
			}
			SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
		}
		else if(action == FILE_SAVE) {
		action = NONE;	
			url = (char *) tinyfd_saveFileDialog(
			"Save Drawing",
			"save.txt",
			2,
			lFilterPatterns,
			NULL);
			if ((url != NULL) && (drawing->main_struct != NULL)){
				dxf_ent_print_f (drawing->main_struct, url);
			}
		}
		
		
		
		if ((gui_check_draw(gui) != 0) || (draw != 0)){
		
			bmp_fill(img, img->bkg); /* clear bitmap */
			dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
			
			draw_cursor(img, mouse_x, mouse_y, cursor);
			/*hilite test */
			if(element != NULL){
				vec_graph_draw_fix(element->obj.graphics, img, ofs_x, ofs_y, zoom, hilite);
			}
			nk_sdl_render(gui, img);
			
			present = 1;
			
			draw = 0;
			
		}
		
		if (present){
			//bmp_copy(img, screen_img, 0, 0);
			//bmp_copy(tmp_img, screen_img, 0, 0);
			
			SDL_UpdateTexture(canvas, NULL, img->buf, width * 4);
			//SDL_UpdateTexture(tmp_tex, NULL, tmp_img->buf, width * 4);
			SDL_RenderClear(renderer);
			//SDL_SetRenderTarget(renderer, NULL);
			SDL_RenderCopy(renderer, canvas, NULL, NULL);
			//SDL_RenderCopy(renderer, tmp_tex, NULL, NULL);
			SDL_RenderPresent(renderer);
			present = 0;
		}
		
		graph_mem_pool(ZERO_GRAPH, 1);
		graph_mem_pool(ZERO_LINE, 1);
		nk_clear(gui->ctx); /*IMPORTANT */
		if (low_proc){
			SDL_Delay(60);
		}
	}
	
	/* safe quit */
	SDL_DestroyTexture(canvas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	/*=======================*/
	//dxf_ent_clear(drawing->main_struct);
	dxf_mem_pool(FREE_DXF, 0);
	graph_mem_pool(FREE_ALL, 0);
	graph_mem_pool(FREE_ALL, 1);
	
	bmp_free(img);
	for (i = 0; i<drawing->num_fonts; i++){
		shx_font_free(drawing->text_fonts[i].shx_font);
	}
	//----------------------------
	nk_sdl_shutdown(gui);
	shx_font_free(shx_font);
	free(drawing);
		
	return 0;
}