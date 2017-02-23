#include <SDL.h>

#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "dxf_graph.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

int main(int argc, char** argv){
	unsigned int width = 640;
	unsigned int height = 480;
	unsigned int quit = 0;
	
	/*============================*/
	char url[]="teste.dxf";
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
	
	int center [] = {12, -6, 2 , -6};
	int dash [] = {8, -8};
	
	bmp_img * img = bmp_new(width, height, white, red);
	
	zoom_x = (max_x - min_x)/img->width;
	zoom_y = (max_y - min_y)/img->height;
	zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	if (zoom <= 0){ zoom =1;}
	else{ zoom = 1/(1.1 * zoom);}
	
	ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
	ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
	
	dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
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
	
	SDL_Texture * texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		width, /* width */
		height); /* height */
	
	while (quit == 0){
		/*=======================*/
		SDL_UpdateTexture(texture, NULL, img->buf, width * 4);

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
					break;
			}

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
	}
	
	/* safe quit */
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	/*=======================*/
	dxf_ent_clear(drawing.main_struct);
	bmp_free(img);
	
	return 0;
}