/* nuklear - 1.32.0 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <SDL.h>


#include "bmp.h"
#include "graph.h"
#include "shape2.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_SDL_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl.h"


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the include
 * and the corresponding function. */
/*#include "../style.c"*/
#include "calculator.c"
#include "overview.c"
#include "node_editor.c"

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
int
main(int argc, char* argv[])
{
	
	shape *shx_font = shx_font_open("txt.shx");
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	bmp_color grey = {.r = 100, .g = 100, .b = 100, .a = 255};
	
	int i;
	
	int center [] = {12, -6, 2 , -6};
	int dash [] = {8, -8};
	
	bmp_img * img = bmp_new(WINDOW_WIDTH, WINDOW_HEIGHT, grey, red);
	img->zero_tl =1;	
	bmp_fill(img, img->bkg); /* clear bitmap */
	
    /* Platform */
    SDL_Window *win;
    struct nk_color background;
    int win_width, win_height;
    int running = 1;

    /* GUI */
    struct nk_context *ctx;

    /* SDL setup */
    SDL_Init(SDL_INIT_VIDEO);
    win = SDL_CreateWindow("Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_GetWindowSize(win, &win_width, &win_height);
	
	SDL_Renderer * renderer = SDL_CreateRenderer(win, -1, 0);
	
	SDL_Texture * canvas = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		WINDOW_WIDTH, /* width */
		WINDOW_HEIGHT); /* height */

    /* GUI */
    ctx = nk_sdl_init(win, shx_font);
    

    background = nk_rgb(28,48,62);
    while (running)
    {
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        /* GUI */
        if (nk_begin(ctx, "Demo", nk_rect(50, 50, 210, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                printf("button pressed\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, background, nk_vec2(nk_widget_width(ctx),400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                background = nk_color_picker(ctx, background, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                background.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, background.r, 255, 1,1);
                background.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, background.g, 255, 1,1);
                background.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, background.b, 255, 1,1);
                background.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, background.a, 255, 1,1);
                nk_combo_end(ctx);
            }
        }
        nk_end(ctx);

        /* -------------- EXAMPLES ---------------- */
        //calculator(ctx);
        overview(ctx);
        //node_editor(ctx);
        /* ----------------------------------------- */

        /* Draw */
        SDL_GetWindowSize(win, &win_width, &win_height);
	
	nk_sdl_render(img);
	/*
	nk_sdl_render(&v_ret);
	for(i = 0; i < v_ret.size; i++){
		graph_draw((v_ret.data)[i], img, 0, 0, 1);
	}*/
        
	SDL_UpdateTexture(canvas, NULL, img->buf, WINDOW_WIDTH * 4);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, canvas, NULL, NULL);
	SDL_RenderPresent(renderer);
	
	graph_mem_pool(ZERO_GRAPH, 0);
	graph_mem_pool(ZERO_LINE, 0);
	bmp_fill(img, img->bkg); /* clear bitmap */
        
    }

cleanup:
    nk_sdl_shutdown();
    SDL_DestroyTexture(canvas);
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    graph_mem_pool(FREE_ALL, 0);
	bmp_free(img);
	shx_font_free(shx_font);
    return 0;
}

