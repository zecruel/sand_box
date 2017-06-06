#include <SDL.h>

#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "shape2.h"
#include "dxf_graph.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

#include "tinyfiledialogs.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_ZERO_COMMAND_MEMORY
#include "nuklear.h"
#include "gui.h"

int main(int argc, char** argv){
	unsigned int width = 1024;
	unsigned int height = 720;
	unsigned int quit = 0;
	
	int time_start, time_end;
	
	shape *shx_font = shx_font_open("txt.shx");
	gui_obj *gui = nk_sdl_init(shx_font);
	struct nk_color background;
	background = nk_rgba(28,48,62,100);
	struct nk_rect total_space = nk_rect(0, 0, 200, 100);
	
	
	char const * lFilterPatterns[2] = { "*.dxf", "*.txt" };
	//tinyfd_messageBox("DXF Viewer", "click OK to continue", "ok", "info", 0);
	char *url = NULL;
	
	
	//char url[]="teste.dxf";
	dxf_drawing drawing;
	int i;
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y, zoom, ofs_x, ofs_y;
	double prev_zoom;
	
	/* init the drawing */
	drawing = dxf_open(url);
	dxf_ents_parse(drawing);
	dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
	
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	bmp_color grey = {.r = 100, .g = 100, .b = 100, .a = 255};
	
	int center [] = {12, -6, 2 , -6};
	int dash [] = {8, -8};
	
	bmp_img * img = bmp_new(width, height, grey, red);
	
	
	//zoom_x = (max_x - min_x)/img->width;
	//zoom_y = (max_y - min_y)/img->height;
	//zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	//if (zoom <= 0){ zoom =1;}
	//else{ zoom = 1/(1.1 * zoom);}
	
	//ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
	//ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
	
	//dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
	
	//bmp_fill(status_img, status_img->bkg); /* clear bitmap */
	//graph_draw(t_open, status_img, 0, 0, 1);
	//bmp_copy(w_open->img, status_img, 300, 5);
	//bmp_save("teste.ppm", img);
	
	/*============================*/
	
	SDL_Event event;
	int mouse_x, mouse_y;
	double pos_x, pos_y;
	
	enum Action {
		NONE,
		FILE_OPEN,
		FILE_SAVE,
		EXIT
	} action;

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
		printf("Driver num pix format = %d\n", info.num_texture_formats);
		for (i = 0; i < info.num_texture_formats; i++){
			switch (info.texture_formats[i]){
				case SDL_PIXELFORMAT_ARGB8888:
					printf("pix format ARGB8888\n");
					break;
				case SDL_PIXELFORMAT_RGBA8888:
					printf("pix format RGBA8888\n");
					break;
				case SDL_PIXELFORMAT_ABGR8888:
					printf("pix format ABGR8888\n");
					break;
				case SDL_PIXELFORMAT_BGRA8888:
					printf("pix format BGRA8888\n");
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
	
	int ev_type, draw = 0;
	
	while (quit == 0){
		int leftMouseButtonDown = 0;
		
		
		ev_type = 0;
		nk_input_begin(gui->ctx);
		if(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) quit = 1;
			nk_sdl_handle_event(gui, window, &event);
			ev_type = event.type;
		}
			
		nk_input_end(gui->ctx);
		
		if ((nk_window_is_any_hovered(gui->ctx) == 0) && (ev_type != 0)){
			switch (event.type){
				case SDL_QUIT:
					quit = 1;
					break;
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
				case SDL_MOUSEMOTION:
					pos_x = (double) event.motion.x/zoom + ofs_x;
					pos_y = (double) (height - event.motion.y)/zoom + ofs_y;
					//if (leftMouseButtonDown){
						//int mouseX = event.motion.x;
						//int mouseY = event.motion.y;
						//pixels[mouseY * 640 + mouseX] = 0;
					//}
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
		
		/* ===============================*/
		/* GUI */
		if (nk_begin(gui->ctx, "Main", nk_rect(50, 50, 120, 150),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)){	
			nk_layout_row_static(gui->ctx, 30, 80, 1);
			if (nk_button_label(gui->ctx, "Open")){
				action = FILE_OPEN;
			}
			if (nk_button_label(gui->ctx, "Save")){
				action = FILE_SAVE;
			}
			if (nk_button_label(gui->ctx, "Exit")){
				action = EXIT;
				quit = 1;
			}
		}
		nk_end(gui->ctx);
		
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
		
		
		if ((gui_check_draw(gui) != 0) || (draw != 0)){
		
			bmp_fill(img, img->bkg); /* clear bitmap */
			dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
			
			nk_sdl_render(gui, img);
			
			graph_mem_pool(ZERO_GRAPH, 1);
			graph_mem_pool(ZERO_LINE, 1);
			draw = 0;
			
		}
		
		nk_clear(gui->ctx); /*IMPORTANT */
		/*===============================*/
		
		SDL_UpdateTexture(canvas, NULL, img->buf, width * 4);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, canvas, NULL, NULL);
		SDL_RenderPresent(renderer);
	
		
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
				dxf_ent_clear(drawing.main_struct);
				graph_mem_pool(ZERO_GRAPH, 0);
				graph_mem_pool(ZERO_LINE, 0);
				
				time_start = SDL_GetTicks();
				drawing = dxf_open(url);
				time_end = SDL_GetTicks();
				printf("Open = %d\n", time_end - time_start);
				
				time_start = SDL_GetTicks();
				dxf_ents_parse(drawing);
				time_end = SDL_GetTicks();
				printf("Parse = %d\n", time_end - time_start);
				
				dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
				zoom_x = (max_x - min_x)/img->width;
				zoom_y = (max_y - min_y)/img->height;
				zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
				if (zoom <= 0){ zoom =1;}
				else{ zoom = 1/(1.1 * zoom);}
				
				ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
				ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
				
				bmp_fill(img, img->bkg); /* clear bitmap */
				
				time_start = SDL_GetTicks();
				dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
				
				//SDL_UpdateTexture(canvas, NULL, img->buf, width * 4);
				//SDL_RenderClear(renderer);
				//SDL_RenderCopy(renderer, canvas, NULL, NULL);
				//SDL_RenderPresent(renderer);
				time_end = SDL_GetTicks();
				printf("Draw = %d\n\n", time_end - time_start);
				
				draw = 1;
			}
		}
		else if(action == FILE_SAVE) {
		action = NONE;	
			url = (char *) tinyfd_saveFileDialog(
			"Save Drawing",
			"save.txt",
			2,
			lFilterPatterns,
			NULL);
			if ((url != NULL) && (drawing.main_struct != NULL)){
				dxf_ent_print_f (drawing.main_struct, url);
			}
		}
	}
	
	/* safe quit */
	SDL_DestroyTexture(canvas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	/*=======================*/
	dxf_ent_clear(drawing.main_struct);
	graph_mem_pool(FREE_ALL, 0);
	graph_mem_pool(FREE_ALL, 1);
	
	bmp_free(img);
	for (i = 0; i<drawing.num_fonts; i++){
		shx_font_free(drawing.text_fonts[i].shx_font);
	}
	//----------------------------
	nk_sdl_shutdown(gui);
	shx_font_free(shx_font);
		
	return 0;
}