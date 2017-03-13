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

struct Widget{
	//struct Widget * next;
	unsigned int x, y, w, h;
	graph_obj * graph;
	bmp_img * img;
	char action[255];
};
typedef struct Widget widget;

struct Toolbox{
	unsigned int x, y, w, h, size;
	widget ** list;
	bmp_img * img;
};
typedef struct Toolbox toolbox;

widget * wdg_new(graph_obj * graph, int x, int y, int w, int h, char *action){
	/* create new widget */
	
	/*colors */
	bmp_color bkg = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color frg = {.r = 255, .g = 255, .b =255, .a = 255};
	//double ofs_x, ofs_y, zoom, zoom_x, zoom_y;
	
	
	/* alloc the structure */
	widget * wdg = (widget *) malloc (sizeof (widget));
	if (wdg){
		/* initialize */
		wdg->graph = graph;
		if (wdg->graph){
			wdg->graph->color = frg;
		}
		wdg->x = x;
		wdg->y = y;
		wdg->w = w;
		wdg->h = h;
		//wdg->next = NULL;
		strcpy(wdg->action, action);
		
		wdg->img = bmp_new(w, h, bkg, frg);
		if (wdg->img){
			bmp_fill(wdg->img, wdg->img->bkg); /* clear bitmap */
			if (wdg->graph){
				/* fit the graph on bmp img */
				/*
				zoom_x = (wdg->graph->ext_max_x - wdg->graph->ext_min_x)/(wdg->img->width*0.7);
				zoom_y = (wdg->graph->ext_max_y - wdg->graph->ext_min_y)/(wdg->img->height*0.7);
				zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
				if (zoom <= 0){ zoom = 1;}
				else{ zoom = 1/(1.1 * zoom);}
				
				ofs_x = wdg->graph->ext_min_x - (fabs((wdg->graph->ext_max_x - wdg->graph->ext_min_x)*zoom - wdg->img->width)/2)/zoom;
				ofs_y = wdg->graph->ext_min_y - (fabs((wdg->graph->ext_max_y - wdg->graph->ext_min_y)*zoom - wdg->img->height)/2)/zoom;
				graph_draw(wdg->graph, wdg->img, ofs_x, ofs_y, zoom);*/
				graph_draw(wdg->graph, wdg->img, 0, 0, 1);
			}
		}
		
	}
	return wdg;
}

void wdg_free(widget * wdg){
	if (wdg){
		graph_free(wdg->graph);
		bmp_free(wdg->img);
		free(wdg);
	}
};

widget * wdg_new_txt2(char * txt, shape *shx_font, int x, int y, int w, int h, char *action){
	graph_obj *graph = shx_font_parse(shx_font, txt);
	widget * wdg = wdg_new(graph, x, y, w, h, action);
	return wdg;
}

widget * wdg_new_txt(char * txt, shape *shx_font, int x, int y, int w, int h, char *action){
	double fnt_size, fnt_above, fnt_below, txt_size;
	double txt_w, txt_h, pos_x, pos_y;
	
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
	graph_obj *graph = shx_font_parse(shx_font, txt);
	
	if (graph){
		/* find the dimentions of text */
		txt_size = (double)h * 0.8/fnt_size;
		txt_w = fabs(graph->ext_max_x - graph->ext_min_x);
		txt_h = fabs(graph->ext_max_y - graph->ext_min_y);
		
		pos_x = (double)w/2 - (txt_w * txt_size/2);
		pos_y = (double)h * 0.1;

		graph_modify(graph, pos_x, pos_y, txt_size, txt_size, 0);
	}
	
	widget * wdg = wdg_new(graph, x, y, w, h, action);
	return wdg;
}

toolbox * tbx_new(int x, int y, int w, int h){
	/* create new toolbox */
	
	/*colors */
	bmp_color bkg = {.r = 250, .g = 250, .b =250, .a = 255};
	bmp_color frg = {.r = 0, .g = 0, .b =0, .a = 255};
	
	/* alloc the structure */
	toolbox * tbx = (toolbox *) malloc (sizeof (toolbox));
	if (tbx){
		/* initialize */
		tbx->x = x;
		tbx->y = y;
		tbx->w = w;
		tbx->h = h;
		tbx->size = 0;
		tbx->list = NULL;
		tbx->img = bmp_new(w, h, bkg, frg);
		if (tbx->img){
			bmp_fill(tbx->img, tbx->img->bkg); /* clear bitmap */
		}
	}
	return tbx;
}

void tbx_free(toolbox * tbx){
	if(tbx){
		int i;
		
		bmp_free(tbx->img);
		for (i = 0; i < tbx->size; i++){
			wdg_free(tbx->list[i]);
		}
		free (tbx->list);
		free(tbx);
	}
}

void tbx_add(toolbox * tbx, widget * wdg){
	int size;
	widget **data;
	
	if ((tbx != NULL) && (wdg != NULL)){ /* check if toolbox and widget exists */
		size = tbx->size + 1;
		/* try to allocate more memory */
		data = realloc(tbx->list, size * sizeof(widget *));
		if (data){ /*success on memory allocation */
			/* change the tbx */
			tbx->list = data; 
			tbx->size = size;
			/* store new widget at end of list */
			data[size-1] = wdg;
			
			/* place the widget image in the toolbox image */
			bmp_copy(wdg->img, tbx->img, wdg->x, wdg->y);
		}
		/* if memory allocation fails, the tbx is unchanged */
	}
}

widget * tbx_event(toolbox * tbx, int x, int y){
	if (tbx){ /*check if toolbox exists */
		/* first, check if the event is inside of toolbox */
		if (rect_find_pos(x, y, tbx->x, tbx->y,
		tbx->x + tbx->w, tbx->y + tbx->h) == INSIDE){
			/* then find the widget that was clicked */
			int i;
			widget * wdg;
			/* convert the global coordninates to local in toolbox */
			x -= tbx->x;
			y -= tbx->y;
			/* sweep the toolbox list of widgets */
			for (i = 0; i < tbx->size; i++){
				wdg = tbx->list[i];
				if (wdg){ /* check if wdg exists */
					if (rect_find_pos(x, y, wdg->x, wdg->y,
					wdg->x + wdg->w, wdg->y + wdg->h) == INSIDE){
						/* success in search */
						return wdg;
					}
				}
			}
		}
	}
	return NULL;
}

int main(int argc, char** argv){
	unsigned int width = 1024;
	unsigned int height = 720;
	unsigned int quit = 0;
	SDL_Rect canvas_rect;
	canvas_rect.x = 20;
	canvas_rect.y = 0;
	canvas_rect.w = width - 20;
	canvas_rect.h = height - 20;
	
	SDL_Rect status_rect;
	status_rect.x = 0;
	status_rect.y = height-20;
	status_rect.w = width;
	status_rect.h = 20;
	
	
	
	
	/*============================*/
	
	shape *shx_font = shx_font_open("txt.shx");
	//graph_obj *t_open = shx_font_parse(shx_font, "Open");
	//widget *w_open = wdg_new_txt("Abrir", shx_font, 0, 0, 50, 10);
	//graph_modify(t_open, 5, 5, 2, 2, 0);
	
	widget *wdg;
	widget *w_open = wdg_new_txt("Open", shx_font, 2, 2, 50, 10, "open");
	widget *w_quit = wdg_new_txt("Quit", shx_font, 200, 2, 50, 10, "quit");
	widget *w_save = wdg_new_txt("Save", shx_font, 100, 2, 50, 10, "save");
	toolbox *main_tbx = tbx_new(0, 0, status_rect.w, status_rect.h);
	//bmp_save("teste.ppm", main_tbx->img);
	tbx_add(main_tbx, w_open);
	tbx_add(main_tbx, w_quit);
	tbx_add(main_tbx, w_save);
	
	/*
	SDL_Rect r_w_open;
	r_w_open.x = 100; //w_open->x;
	r_w_open.y = w_open->y;
	r_w_open.w = w_open->w;
	r_w_open.h = w_open->h;*/
	
	char const * lFilterPatterns[2] = { "*.dxf", "*.txt" };
	//tinyfd_messageBox("DXF Viewer", "click OK to continue", "ok", "info", 0);
	char *url = NULL;
	
	
	//char url[]="teste.dxf";
	dxf_drawing drawing;
	int i;
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y, zoom, ofs_x, ofs_y;
	double prev_zoom;
	
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
	
	bmp_img * img = bmp_new(canvas_rect.w, canvas_rect.h, grey, red);
	//bmp_img * status_img = bmp_new(status_rect.w, status_rect.h, white, black);
	
	zoom_x = (max_x - min_x)/img->width;
	zoom_y = (max_y - min_y)/img->height;
	zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	if (zoom <= 0){ zoom =1;}
	else{ zoom = 1/(1.1 * zoom);}
	
	ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
	ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
	
	dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
	
	//bmp_fill(status_img, status_img->bkg); /* clear bitmap */
	//graph_draw(t_open, status_img, 0, 0, 1);
	//bmp_copy(w_open->img, status_img, 300, 5);
	//bmp_save("teste.ppm", img);
	
	/*============================*/
	
	SDL_Event event;
	int mouse_x, mouse_y;

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
		canvas_rect.w, /* width */
		canvas_rect.h); /* height */
		
	SDL_Texture * status = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		status_rect.w, /* width */
		status_rect.h); /* height */
	
	while (quit == 0){
		/*=======================*/
		wdg = NULL;
		if(SDL_PollEvent(&event)){
			switch (event.type){
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT){
						//leftMouseButtonDown = false;
						
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT){
						//leftMouseButtonDown = true;
						SDL_GetMouseState(&mouse_x, &mouse_y);
						mouse_y = height - mouse_y;
						
						wdg = tbx_event(main_tbx, mouse_x, mouse_y);
						
					}
					break;
				case SDL_MOUSEMOTION:
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
					
					bmp_fill(img, img->bkg); /* clear bitmap */
					dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
					//graph_draw(t_open, img, 0, 0, 1);
					break;
			}
			SDL_UpdateTexture(canvas, NULL, img->buf, canvas_rect.w * 4);
			SDL_UpdateTexture(status, NULL, main_tbx->img->buf, main_tbx->img->width * 4);
			//SDL_UpdateTexture(status, &r_w_open, w_open->img->buf, r_w_open.w * 4);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, canvas, NULL, &canvas_rect);
			SDL_RenderCopy(renderer, status, NULL, &status_rect);
			SDL_RenderPresent(renderer);
		}
		
		if (wdg) {
			
			if(strcmp(wdg->action, "open") == 0) {	
				url = (char *) tinyfd_openFileDialog(
				"Open a Drawing",
				"",
				2,
				lFilterPatterns,
				NULL,
				0);
				if (url){
					dxf_ent_clear(drawing.main_struct);
					drawing = dxf_open(url);
					dxf_ents_parse(drawing);
					dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
					zoom_x = (max_x - min_x)/img->width;
					zoom_y = (max_y - min_y)/img->height;
					zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
					if (zoom <= 0){ zoom =1;}
					else{ zoom = 1/(1.1 * zoom);}
					
					ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
					ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
					
					bmp_fill(img, img->bkg); /* clear bitmap */
					dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
					
					SDL_UpdateTexture(canvas, NULL, img->buf, canvas_rect.w * 4);
					SDL_UpdateTexture(status, NULL, main_tbx->img->buf, main_tbx->img->width * 4);
					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, canvas, NULL, &canvas_rect);
					SDL_RenderCopy(renderer, status, NULL, &status_rect);
					SDL_RenderPresent(renderer);
				}
			}
			else if (strcmp(wdg->action, "quit") == 0){
				quit = 1;
				break;
			}
			else if(strcmp(wdg->action, "save") == 0) {	
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
	}
	
	/* safe quit */
	SDL_DestroyTexture(canvas);
	SDL_DestroyTexture(status);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	/*=======================*/
	dxf_ent_clear(drawing.main_struct);
	bmp_free(img);
	shx_font_free(shx_font);
	tbx_free(main_tbx);
	//graph_free(t_open);
	//wdg_free(w_open);
	
	return 0;
}