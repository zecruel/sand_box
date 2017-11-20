#include <SDL.h>

#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "shape2.h"
#include "dxf_graph.h"
#include "list.h"
#include "dxf_create.h"
#include "dxf_attract.h"

#include "dxf_colors.h"
#include "dxf_seed.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <ctype.h>
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
#include "images.c"

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_ZE};

void
set_style(struct nk_context *ctx, enum theme theme)
{
    struct nk_color table[NK_COLOR_COUNT];
    if (theme == THEME_WHITE) {
        table[NK_COLOR_TEXT] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_HEADER] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_BORDER] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(185, 185, 185, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(170, 170, 170, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(120, 120, 120, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SELECT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(80, 80, 80, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(70, 70, 70, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(60, 60, 60, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_EDIT] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(0, 0, 0, 255);
        table[NK_COLOR_COMBO] = nk_rgba(175, 175, 175, 255);
        table[NK_COLOR_CHART] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(45, 45, 45, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(180, 180, 180, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(140, 140, 140, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(150, 150, 150, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(160, 160, 160, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(180, 180, 180, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_RED) {
        table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(30, 33, 40, 215);
        table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
        table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
        table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_BLUE) {
        table[NK_COLOR_TEXT] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(202, 212, 214, 215);
        table[NK_COLOR_HEADER] = nk_rgba(137, 182, 224, 220);
        table[NK_COLOR_BORDER] = nk_rgba(140, 159, 173, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(142, 187, 229, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(147, 192, 234, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(182, 215, 215, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SELECT] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(177, 210, 210, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(137, 182, 224, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(142, 188, 229, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(147, 193, 234, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_EDIT] = nk_rgba(210, 210, 210, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(20, 20, 20, 255);
        table[NK_COLOR_COMBO] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(137, 182, 224, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(190, 200, 200, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(156, 193, 220, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_DARK) {
        table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(57, 67, 71, 215);
        table[NK_COLOR_HEADER] = nk_rgba(51, 51, 56, 220);
        table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(63, 98, 126, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 53, 56, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SELECT] = nk_rgba(57, 67, 61, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 83, 111, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_EDIT] = nk_rgba(50, 58, 61, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_COMBO] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 58, 61, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 83, 111, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(53, 88, 116, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(58, 93, 121, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 83, 111, 255);
        nk_style_from_table(ctx, table);
    } else if (theme == THEME_ZE) {
        table[NK_COLOR_TEXT] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_WINDOW] = nk_rgba(57, 71, 58, 215);
        table[NK_COLOR_HEADER] = nk_rgba(52, 57, 52, 220);
        table[NK_COLOR_BORDER] = nk_rgba(46, 46, 46, 255);
        table[NK_COLOR_BUTTON] = nk_rgba(48, 112, 54, 255);
        table[NK_COLOR_BUTTON_HOVER] = nk_rgba(71, 161, 80, 255);
        table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(89, 201, 100, 255);
        table[NK_COLOR_TOGGLE] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(46, 57, 46, 255);
        table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(48, 112, 54, 255);
        table[NK_COLOR_SELECT] = nk_rgba(58, 67, 57, 255);
        table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(48, 112, 54, 255);
        table[NK_COLOR_SLIDER] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(48, 112, 54, 245);
        table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(59, 115, 53, 255);
        table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(71, 161, 80, 255);
        table[NK_COLOR_PROPERTY] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_EDIT] = nk_rgba(50, 61, 50, 225);
        table[NK_COLOR_EDIT_CURSOR] = nk_rgba(210, 210, 210, 255);
        table[NK_COLOR_COMBO] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_CHART] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_CHART_COLOR] = nk_rgba(48, 112, 54, 255);
        table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
        table[NK_COLOR_SCROLLBAR] = nk_rgba(50, 61, 50, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(48, 112, 54, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(59, 115, 53, 255);
        table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(71, 161, 80, 255);
        table[NK_COLOR_TAB_HEADER] = nk_rgba(48, 112, 54, 255);
        nk_style_from_table(ctx, table);
    } else {
        nk_style_default(ctx);
    }
}

void toolbox_get_imgs(bmp_img *img, int w, int h, bmp_img *vec[], int num){
	if (vec){
		int i, col = 0, lin = 0;
		for (i = 0; i < num; i++){
			vec[i] = NULL;
			vec[i] = bmp_sub_img(img, col * w, lin * h, w, h);
			col++;
			if ((col * w) + w > (int) img->width){
				col = 0;
				lin++;
			}
		}
	}
}

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

void draw_attractor(bmp_img *img, enum attract_type type, int x, int y, bmp_color color){
	/* draw attractor mark*/
	if (type != ATRC_NONE){
		/* set the pattern */
		double pat = 1;
		patt_change(img, &pat, 1);
		/* set the color */
		img->frg = color;
		/* set the tickness */
		img->tick = 1;
	}
	switch (type){
		case ATRC_END:
			/* draw square */
			bmp_line(img, x-5, y-5, x-5, y+5);
			bmp_line(img, x-5, y+5, x+5, y+5);
			bmp_line(img, x+5, y+5, x+5, y-5);
			bmp_line(img, x+5, y-5, x-5, y-5);
			break;
		case ATRC_MID:
			/* draw triangle */
			bmp_line(img, x-5, y-5, x+5, y-5);
			bmp_line(img, x-5, y-5, x, y+5);
			bmp_line(img, x+5, y-5, x, y+5);
			break;
		case ATRC_CENTER:
			/* draw circle */
			bmp_circle(img, x, y, 7);
			break;
		case ATRC_OCENTER:
			/* draw a *star */
			bmp_line(img, x, y-5, x, y+5);
			bmp_line(img, x-5, y, x+5, y);
			bmp_line(img, x-5, y-5, x+5, y+5);
			bmp_line(img, x-5, y+5, x+5, y-5);
			break;
		case ATRC_NODE:
			/* draw circle with x*/
			bmp_circle(img, x, y, 5);
			bmp_line(img, x-5, y-5, x+5, y+5);
			bmp_line(img, x-5, y+5, x+5, y-5);
			break;
		case ATRC_QUAD:
			/* draw diamond */
			bmp_line(img, x-7, y, x, y+7);
			bmp_line(img, x, y+7, x+7, y);
			bmp_line(img, x+7, y, x, y-7);
			bmp_line(img, x, y-7, x-7, y);
			break;
		case ATRC_INTER:
			/* draw x */
			img->tick = 3;
			bmp_line(img, x-5, y-5, x+5, y+5);
			bmp_line(img, x-5, y+5, x+5, y-5);
			break;
		case ATRC_EXT:
			img->tick = 3;
			bmp_line(img, x-7, y, x-2, y);
			bmp_line(img, x, y, x+3, y);
			bmp_line(img, x+5, y, x+7, y);
			break;
		case ATRC_PERP:
			/* draw square */
			bmp_line(img, x-5, y-5, x-5, y+5);
			bmp_line(img, x-5, y-5, x+5, y-5);
			bmp_line(img, x-5, y, x, y);
			bmp_line(img, x, y-5, x, y);
			break;
		case ATRC_INS:
			bmp_line(img, x-5, y+5, x+1, y+5);
			bmp_line(img, x+1, y+5, x+1, y+1);
			bmp_line(img, x+1, y+1, x+5, y+1);
			bmp_line(img, x+5, y+1, x+5, y-5);
			bmp_line(img, x+5, y-5, x-1, y-5);
			bmp_line(img, x-1, y-5, x-1, y-1);
			bmp_line(img, x-1, y-1, x-5, y-1);
			bmp_line(img, x-5, y-1, x-5, y+5);
			break;
		case ATRC_TAN:
			bmp_line(img, x-5, y+5, x+5, y+5);
			bmp_circle(img, x, y, 5);
			break;
		case ATRC_PAR:
			/* draw two lines */
			bmp_line(img, x-5, y-1, x+1, y+5);
			bmp_line(img, x-1, y-5, x+5, y+1);
			break;
		case ATRC_CTRL:
			/* draw a cross */
			bmp_line(img, x, y-5, x, y+5);
			bmp_line(img, x-5, y, x+5, y);
			break;
		case ATRC_AINT:
			/* draw square with x */
			bmp_line(img, x-5, y-5, x-5, y+5);
			bmp_line(img, x-5, y+5, x+5, y+5);
			bmp_line(img, x+5, y+5, x+5, y-5);
			bmp_line(img, x+5, y-5, x-5, y-5);
			bmp_line(img, x-5, y-5, x+5, y+5);
			bmp_line(img, x-5, y+5, x+5, y-5);
			break;
		case ATRC_ANY:
			/* draw a Hourglass */
			bmp_line(img, x-5, y-5, x+5, y+5);
			bmp_line(img, x-5, y+5, x+5, y-5);
			img->tick = 2;
			bmp_line(img, x-5, y+5, x+5, y+5);
			bmp_line(img, x-5, y-5, x+5, y-5);
			break;
	}
}

void zoom_ext(dxf_drawing *drawing, bmp_img *img, double *zoom, double *ofs_x, double *ofs_y){
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y;
	dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
	zoom_x = (max_x - min_x)/img->width;
	zoom_y = (max_y - min_y)/img->height;
	*zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	if (*zoom <= 0){ *zoom =1;}
	else{ *zoom = 1/(1.1 * (*zoom));}
	
	*ofs_x = min_x - (fabs((max_x - min_x)*(*zoom) - img->width)/2)/(*zoom);
	*ofs_y = min_y - (fabs((max_y - min_y)*(*zoom) - img->height)/2)/(*zoom);
}

void sel_list_append(list_node *list, dxf_node *ent){
	if (list && ent){
		if (list_find_data(list, ent)){
			//printf ("ja existe!\n");
		}
		else{
			list_node * new_el = list_new(ent, 0);
			if (new_el){
				list_push(list, new_el);
			}
		}
	}
}

int main(int argc, char** argv){
	//setlocale(LC_ALL,""); //seta a localidade como a current do computador para aceitar acentuacao
	
	double zoom = 20.0 , ofs_x = 0.0, ofs_y = 0.0;
	double prev_zoom;
	int color_idx = 256;
	int layer_idx = 0, ltypes_idx = 0;
	dxf_node *element = NULL, *prev_el = NULL, *new_el = NULL, *near_el = NULL;
	double pos_x, pos_y, x0, y0, x1, y1, x2, y2, bulge = 0.0, txt_h = 1.0, scale = 1.0;
	double thick = 0.0;
	char txt[DXF_MAX_CHARS];
	dxf_node *x0_attr = NULL, *y0_attr = NULL, *x1_attr = NULL, *y1_attr = NULL;
	
	/* DXF text vertical alignment definitions */
	static const char *text_al_v[] = {"Base Line", "Bottom", "Middle", "Top"};
	#define T_AL_V_LEN 4
	int t_al_v = 0;
	/* DXF text horizontal alignment definitions */
	static const char *text_al_h[] = {"Left", "Center", "Right", "Aligned", "Middle", "Fit"};
	#define T_AL_H_LEN 6
	int t_al_h = 0;
	
	unsigned int width = 1024;
	unsigned int height = 600;
	int open_prg = 0;
	int progress = 0;
	int progr_win = 0;
	int progr_end = 0;
	unsigned int quit = 0;
	unsigned int wait_open = 0;
	int show_app_about = 0;
	int i;
	int ev_type, draw = 0, draw_tmp = 0, draw_phanton = 0;
	vector_p *phanton = NULL;
	struct nk_color background;
	
	int leftMouseButtonDown = 0;
	int rightMouseButtonDown = 0;
	int leftMouseButtonClick = 0;
	int rightMouseButtonClick = 0;
	int MouseMotion = 0;
	int keyEnter = 0;
	
	int step = 0;
	int lock_ax_x = 0;
	int lock_ax_y = 0;
	double step_x[10], step_y[10];
	char user_str_x[64], user_str_y[64];
	user_str_x[0] = 0; user_str_y[0] = 0;
	double user_x = 0.0, user_y = 0.0;
	int user_flag_x = 0, user_flag_y = 0;
	int user_number = 0;
	int en_distance = 0; /* enable distance entry */
	
	enum attract_type curr_attr_t = ATRC_END|ATRC_MID|ATRC_QUAD;
	double near_x, near_y;
	int near_attr; /* flag */
	
	
	//graph_obj *tmp_graph = NULL;
	
	SDL_Event event;
	int mouse_x, mouse_y;
	
	double rect_pt1[2], rect_pt2[2];
	int low_proc = 1;
	
	enum Action {
		NONE,
		FILE_OPEN,
		FILE_SAVE,
		EXPORT,
		VIEW_ZOOM_EXT,
		DELETE,
		EXIT
	} action = NONE;
	
	enum Modal {
		SELECT,
		LINE,
		POLYLINE,
		CIRCLE,
		RECT,
		TEXT,
		ARC,
		DUPLI,
		MOVE,
		SCALE
	}modal = SELECT, prev_modal = SELECT;
	
	char recv_comm[64];
	int recv_comm_flag = 0;
	
	char *url = NULL;
	char const * lFilterPatterns[2] = { "*.dxf", "*.txt" };
	
	char *file_buf = NULL;
	long file_size = 0;
	
	char* dropped_filedir;                  /* Pointer for directory of dropped file */
	
	/* Colors in use */
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	bmp_color yellow = {.r = 255, .g = 255, .b =0, .a = 255};
	bmp_color grey = {.r = 100, .g = 100, .b = 100, .a = 255};
	bmp_color hilite = {.r = 255, .g = 0, .b = 255, .a = 150};
	bmp_color transp = {.r = 255, .g = 255, .b = 255, .a = 0};
	bmp_color cursor = {.r = 255, .g = 255, .b = 255, .a = 100};
	background = nk_rgb(28,48,62);
	
	/* line types in use */
	double center [] = {12, -6, 2 , -6};
	double dash [] = {8, -8};
	double continuous[] = {1};
	
	/* initialize the selection list */
	list_node * sel_list = list_new(NULL, 0);
	
	/* init the main image */
	bmp_img * img = bmp_new(width, height, grey, red);
	
	/* init Nuklear GUI */
	shape *shx_font = shx_font_open("txt.shx");
	gui_obj *gui = nk_sdl_init(shx_font);
	
	set_style(gui->ctx, THEME_ZE);
	
	gui->ctx->style.edit.padding = nk_vec2(4, -6);
	
	
	
	/* init the toolbox image */
	bmp_img * tool_img = bmp_load_img("tool4.png");
	bmp_img * tool_vec[54];
	toolbox_get_imgs(tool_img, 16, 16, tool_vec, 54);
	struct nk_image i_new = nk_image_ptr(tool_vec[0]);
	struct nk_image i_open = nk_image_ptr(tool_vec[1]);
	struct nk_image i_save = nk_image_ptr(tool_vec[2]);
	struct nk_image i_close = nk_image_ptr(tool_vec[3]);
	struct nk_image i_export = nk_image_ptr(tool_vec[4]);
	struct nk_image i_import = nk_image_ptr(tool_vec[5]);
	struct nk_image i_print = nk_image_ptr(tool_vec[6]);
	struct nk_image i_help = nk_image_ptr(tool_vec[7]);
	struct nk_image i_copy = nk_image_ptr(tool_vec[8]);
	struct nk_image i_cut = nk_image_ptr(tool_vec[9]);
	struct nk_image i_paste = nk_image_ptr(tool_vec[10]);
	struct nk_image i_undo = nk_image_ptr(tool_vec[11]);
	struct nk_image i_redo = nk_image_ptr(tool_vec[12]);
	struct nk_image i_view = nk_image_ptr(tool_vec[13]);
	struct nk_image i_unview = nk_image_ptr(tool_vec[14]);
	struct nk_image i_unlock = nk_image_ptr(tool_vec[15]);
	struct nk_image i_lock = nk_image_ptr(tool_vec[16]);
	struct nk_image i_select = nk_image_ptr(tool_vec[17]);
	struct nk_image i_move = nk_image_ptr(tool_vec[18]);
	struct nk_image i_dupli = nk_image_ptr(tool_vec[19]);
	struct nk_image i_rotate = nk_image_ptr(tool_vec[20]);
	struct nk_image i_mirror = nk_image_ptr(tool_vec[21]);
	struct nk_image i_group = nk_image_ptr(tool_vec[22]);
	struct nk_image i_ungroup = nk_image_ptr(tool_vec[23]);
	struct nk_image i_delete = nk_image_ptr(tool_vec[24]);
	struct nk_image i_line = nk_image_ptr(tool_vec[25]);
	struct nk_image i_poly = nk_image_ptr(tool_vec[26]);
	struct nk_image i_block = nk_image_ptr(tool_vec[27]);
	struct nk_image i_circle = nk_image_ptr(tool_vec[28]);
	struct nk_image i_arc = nk_image_ptr(tool_vec[29]);
	struct nk_image i_text = nk_image_ptr(tool_vec[30]);
	struct nk_image i_spline = nk_image_ptr(tool_vec[31]);
	struct nk_image i_z_plus = nk_image_ptr(tool_vec[32]);
	struct nk_image i_z_minus = nk_image_ptr(tool_vec[33]);
	struct nk_image i_z_win = nk_image_ptr(tool_vec[34]);
	struct nk_image i_z_all = nk_image_ptr(tool_vec[35]);
	struct nk_image i_meas = nk_image_ptr(tool_vec[36]);
	struct nk_image i_layer = nk_image_ptr(tool_vec[37]);
	struct nk_image i_image = nk_image_ptr(tool_vec[38]);
	struct nk_image i_lib = nk_image_ptr(tool_vec[39]);
	struct nk_image i_scale = nk_image_ptr(tool_vec[40]);
	struct nk_image i_elipse = nk_image_ptr(tool_vec[41]);
	struct nk_image i_t_l = nk_image_ptr(tool_vec[42]);
	struct nk_image i_t_c = nk_image_ptr(tool_vec[43]);
	struct nk_image i_t_r = nk_image_ptr(tool_vec[44]);
	struct nk_image i_t_bl = nk_image_ptr(tool_vec[45]);
	struct nk_image i_t_bc = nk_image_ptr(tool_vec[46]);
	struct nk_image i_t_br = nk_image_ptr(tool_vec[47]);
	struct nk_image i_t_tl = nk_image_ptr(tool_vec[48]);
	struct nk_image i_t_tc = nk_image_ptr(tool_vec[49]);
	struct nk_image i_t_tr = nk_image_ptr(tool_vec[50]);
	struct nk_image i_t_ml = nk_image_ptr(tool_vec[51]);
	struct nk_image i_t_mc = nk_image_ptr(tool_vec[52]);
	struct nk_image i_t_mr = nk_image_ptr(tool_vec[53]);
	
	struct nk_style_button b_icon_style;
	if (gui){
		b_icon_style = gui->ctx->style.button;
	}
	b_icon_style.image_padding.x = -4;
	b_icon_style.image_padding.y = -4;
	
	bmp_img * brazil_img = bmp_load_img2(brazil_flag.pixel_data, brazil_flag.width, brazil_flag.height);
	struct nk_image i_brazil = nk_image_ptr(brazil_img);
	
	bmp_img * cz_img = bmp_load_img2(cz.pixel_data, cz.width, cz.height);
	struct nk_image i_cz = nk_image_ptr(cz_img);
	
	bmp_img * cz48_img = bmp_load_img2(cz48.pixel_data, cz48.width, cz48.height);
	struct nk_image i_cz48 = nk_image_ptr(cz48_img);
	
	/* init comands */
	recv_comm[0] = 0;
	txt[0] = 0;
	
	/* init the drawing */
	dxf_drawing *drawing = malloc(sizeof(dxf_drawing));
	url = NULL; /* pass a null file only for initialize the drawing structure */
	//dxf_open(drawing, url);
	while (dxf_read (drawing, (char *)dxf_seed_2007, strlen(dxf_seed_2007), &progress) > 0){
		
	}
	//printf(dxf_seed_r12);
	dxf_ents_parse(drawing);

	/* init the SDL2 */
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow(
		"CadZinho", /* title */
		SDL_WINDOWPOS_UNDEFINED, /* x position */
		SDL_WINDOWPOS_UNDEFINED, /* y position */
		width, /* width */
		height, /* height */
		0); /* flags */
	
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
		(void *)cz16.pixel_data, cz16.width, cz16.height,
		32, cz16.width * 4,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	// The icon is attached to the window pointer
	SDL_SetWindowIcon(window, surface);

	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(surface);

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
		
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	
	//SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
	
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
		if (nk_window_is_any_hovered(gui->ctx)) {
			SDL_ShowCursor(SDL_ENABLE);
			//printf("show\n");
		}
		else{
			SDL_ShowCursor(SDL_DISABLE);
			if (ev_type != 0){
				
				switch (event.type){
					case SDL_MOUSEBUTTONUP:
						mouse_x = event.button.x;
						mouse_y = event.button.y;
						mouse_y = height - mouse_y;
						if (event.button.button == SDL_BUTTON_LEFT){
							leftMouseButtonDown = 0;
						}
						else if(event.button.button == SDL_BUTTON_RIGHT){
							rightMouseButtonDown = 0;
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						mouse_x = event.button.x;
						mouse_y = event.button.y;
						mouse_y = height - mouse_y;
						if (event.button.button == SDL_BUTTON_LEFT){
							leftMouseButtonDown = 1;
							leftMouseButtonClick = 1;
						}
						else if(event.button.button == SDL_BUTTON_RIGHT){
							rightMouseButtonDown = 1;
							rightMouseButtonClick = 1;
						}
						break;
					case SDL_MOUSEMOTION:
						MouseMotion = 1;
						mouse_x = event.motion.x;
						mouse_y = event.motion.y;
						mouse_y = height - mouse_y;
						pos_x = (double) mouse_x/zoom + ofs_x;
						pos_y = (double) mouse_y/zoom + ofs_y;
						draw = 1;
						break;
					case SDL_MOUSEWHEEL:
						prev_zoom = zoom;
						zoom = zoom + event.wheel.y * 0.2 * zoom;
						
						SDL_GetMouseState(&mouse_x, &mouse_y);
						mouse_y = height - mouse_y;
						ofs_x += ((double) mouse_x)*(1/prev_zoom - 1/zoom);
						ofs_y += ((double) mouse_y)*(1/prev_zoom - 1/zoom);
						draw = 1;
						break;
					
					case (SDL_DROPFILE): {      // In case if dropped file
						dropped_filedir = event.drop.file;
						// Shows directory of dropped file
						//SDL_ShowSimpleMessageBox(
						//	SDL_MESSAGEBOX_INFORMATION,
						//	"File dropped on window",
						//	dropped_filedir,
						//	window);
						printf(dropped_filedir);
						printf("\n----------------------------\n");
						SDL_free(dropped_filedir);    // Free dropped_filedir memory
						}
						break;
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_RETURN){
							keyEnter = 1;
						}
						break;
					case SDL_TEXTINPUT:
						/* text input */
						/* if the user enters a character relative a number */
						if ((*event.text.text > 41) && (*event.text.text < 58)){
							user_number = 1; /* sinalize a user flag */
						}
						break;
				}
			}
		}
		
		/* GUI */
		/* main toolbox, for open files, save or exit */
		/*if (nk_begin(gui->ctx, "Main", nk_rect(5, 5, 200, 40),
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
			/*main menu 
			nk_menubar_begin(gui->ctx);
			nk_layout_row_dynamic(gui->ctx, 15, 3);
			/* new menu
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
			/* new menu
			if (nk_menu_begin_label(gui->ctx, "VIEW", NK_TEXT_LEFT, nk_vec2(120, 200))){
				nk_layout_row_dynamic(gui->ctx, 15, 1);
				if (nk_menu_item_label(gui->ctx, "Extend", NK_TEXT_LEFT)){
					action = VIEW_ZOOM_EXT;
				}
				if (nk_menu_item_label(gui->ctx, "Zoom in", NK_TEXT_LEFT)){
					//action = FILE_OPEN;
				}
				nk_menu_end(gui->ctx);
			}
			/* new menu
			if (nk_menu_begin_label(gui->ctx, "HELP", NK_TEXT_LEFT, nk_vec2(120, 200))){
				nk_layout_row_dynamic(gui->ctx, 15, 1);
				if (nk_menu_item_label(gui->ctx, "About", NK_TEXT_LEFT)){
					//action = FILE_OPEN;
				}
				
				nk_menu_end(gui->ctx);
			}
			
			nk_menubar_end(gui->ctx);
		}
		nk_end(gui->ctx);*/
		
		if (nk_begin(gui->ctx, "Main", nk_rect(0, 0, 200, 32),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR)){
			nk_layout_row_static(gui->ctx, 20, 20, 10);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_new)){
				printf("NEW\n");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_open)){
				action = FILE_OPEN;
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_save)){
				action = FILE_SAVE;
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_export)){
				action = EXPORT;
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_close)){
				printf("CLOSE\n");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_help)){
				show_app_about = 1;
				//printf("HELP\n");
			}
			if (show_app_about){
				/* about popup */
				static struct nk_rect s = {20, 100, 400, 150};
				if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s)){
					nk_layout_row_dynamic(gui->ctx, 50, 2);
					nk_label(gui->ctx, "CadZinho", NK_TEXT_RIGHT);
					nk_image(gui->ctx, i_cz48);
					//nk_layout_row_dynamic(gui->ctx, 165, 1);
					//nk_image(gui->ctx, i_cz);
					nk_layout_row_begin(gui->ctx, NK_DYNAMIC, 20, 2);
					nk_layout_row_push(gui->ctx, 0.7f);
					nk_label(gui->ctx, "By Ezequiel Rabelo de Aguiar", NK_TEXT_RIGHT);
					nk_layout_row_push(gui->ctx, 0.3f);
					nk_image(gui->ctx, i_brazil);
					nk_layout_row_end(gui->ctx);
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "CadZinho is licensed under the MIT License.",  NK_TEXT_LEFT);
					
					nk_popup_end(gui->ctx);
				} else show_app_about = nk_false;
			}
			
		}
		nk_end(gui->ctx);
		
		if (nk_begin(gui->ctx, "Toolbox", nk_rect(2, 50, 100, 500),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)){
			
			/*
			struct nk_image tool;
			for (i = 0; i < 40; i++){
				tool = nk_image_ptr(tool_vec[i]);
				tool.w = tool_vec[0]->width;
				tool.h = tool_vec[0]->height;
				tool.region[2] = (unsigned short)tool_vec[0]->width;
				tool.region[3] = (unsigned short)tool_vec[0]->height;
				
				if (nk_button_image_styled(gui->ctx, &b_icon_style, tool)){
					printf("i = %d\n", i);
				}
			}*/
			if (nk_tree_push(gui->ctx, NK_TREE_TAB, "Place", NK_MAXIMIZED)) {
				nk_layout_row_static(gui->ctx, 20, 20, 2);
				
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_select)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","SELECT");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_line)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","LINE");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_poly)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","POLYLINE");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_block)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","RECT");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_text)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","TEXT");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_circle)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","CIRCLE");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_arc)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_spline)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_elipse)){
					
				}
				nk_tree_pop(gui->ctx);
			}
			if (nk_tree_push(gui->ctx, NK_TREE_TAB, "Modify", NK_MAXIMIZED)) {
				nk_layout_row_static(gui->ctx, 20, 20, 2);
				
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_move)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","MOVE");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_dupli)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","DUPLI");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_scale)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","SCALE");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_rotate)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_mirror)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_group)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_ungroup)){
					
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, i_delete)){
					recv_comm_flag = 1;
					snprintf(recv_comm, 64, "%s","DELETE");
				}
				nk_tree_pop(gui->ctx);
			}
			if (nk_tree_push(gui->ctx, NK_TREE_TAB, "Attract", NK_MAXIMIZED)) {
				nk_layout_row_static(gui->ctx, 15, 40, 1);
				int selected;
				
				//ATRC_END = 1,
				selected = (curr_attr_t & ATRC_END);
				nk_selectable_label(gui->ctx, "End", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_END;
				else curr_attr_t &= ~ATRC_END;
				
				//ATRC_MID = 2,
				selected = (curr_attr_t & ATRC_MID);
				nk_selectable_label(gui->ctx, "Mid", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_MID;
				else curr_attr_t &= ~ATRC_MID;
				
				//ATRC_CENTER = 4,
				selected = (curr_attr_t & ATRC_CENTER);
				nk_selectable_label(gui->ctx, "Cen", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_CENTER;
				else curr_attr_t &= ~ATRC_CENTER;
				
				//ATRC_QUAD = 8,
				selected = (curr_attr_t & ATRC_QUAD);
				nk_selectable_label(gui->ctx, "Quad", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_QUAD;
				else curr_attr_t &= ~ATRC_QUAD;
				
				//ATRC_INTER = 16,
				selected = (curr_attr_t & ATRC_INTER);
				nk_selectable_label(gui->ctx, "Int", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_INTER;
				else curr_attr_t &= ~ATRC_INTER;
				
				selected = (curr_attr_t & ATRC_INS);
				nk_selectable_label(gui->ctx, "Ins", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_INS;
				else curr_attr_t &= ~ATRC_INS;
				
				selected = (curr_attr_t & ATRC_NODE);
				nk_selectable_label(gui->ctx, "Node", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_NODE;
				else curr_attr_t &= ~ATRC_NODE;
				
				selected = (curr_attr_t & ATRC_TAN);
				nk_selectable_label(gui->ctx, "TAN", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_TAN;
				else curr_attr_t &= ~ATRC_TAN;
				
				selected = (curr_attr_t & ATRC_PERP);
				nk_selectable_label(gui->ctx, "PERP", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_PERP;
				else curr_attr_t &= ~ATRC_PERP;
				
				//ATRC_PERP = 32,
				//ATRC_INS = 64,
				//ATRC_CTRL = 128,
				//ATRC_KEY = 256,
				
				//ATRC_ANY = 512
				selected = (curr_attr_t & ATRC_ANY);
				nk_selectable_label(gui->ctx, "Any", NK_TEXT_LEFT, &selected);
				if (selected) curr_attr_t |= ATRC_ANY;
				else curr_attr_t &= ~ATRC_ANY;
				
				nk_tree_pop(gui->ctx);
			}
		}
		nk_end(gui->ctx);
		
		if (nk_begin(gui->ctx, "Tool", nk_rect(110, 50, 200, 200),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)){
			switch (modal) {
				case SELECT:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Select an object", NK_TEXT_LEFT);
					break;
				case LINE:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place a single line", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter end point", NK_TEXT_LEFT);
					}
					break;
				case POLYLINE:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place a poly line", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter next point", NK_TEXT_LEFT);
					}
					bulge = nk_propertyd(gui->ctx, "Bulge", -1.0d, bulge, 1.0d, 0.1d, 0.1d);
					break;
				case CIRCLE:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place a circle", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter center point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter end point", NK_TEXT_LEFT);
					}
					break;
				case RECT:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place a rectangle", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter first point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter end point", NK_TEXT_LEFT);
					}
					break;
				case TEXT:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place an text", NK_TEXT_LEFT);
					//nk_edit_string(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER, comm, &comm_len, 64, nk_filter_default);
					nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, txt, DXF_MAX_CHARS, nk_filter_default);
					txt_h = nk_propertyd(gui->ctx, "Text Height", 0.0d, txt_h, 100.0d, 0.1d, 0.1d);
					t_al_v = nk_combo(gui->ctx, text_al_v, T_AL_V_LEN, t_al_v, 20, nk_vec2(100,105));
					t_al_h = nk_combo(gui->ctx, text_al_h, T_AL_H_LEN, t_al_h, 20, nk_vec2(100,105));
					break;
				case ARC:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place an arc", NK_TEXT_LEFT);
					break;
				case MOVE:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Move a selection", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter base point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter destination point", NK_TEXT_LEFT);
					}
					break;
				case DUPLI:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Duplicate a selection", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter base point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter destination point", NK_TEXT_LEFT);
					}
					break;
				case SCALE:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Scale a selection", NK_TEXT_LEFT);
					if (step == 0){
						nk_label(gui->ctx, "Enter base point", NK_TEXT_LEFT);
					} else {
						nk_label(gui->ctx, "Enter destination point", NK_TEXT_LEFT);
					}
					scale = nk_propertyd(gui->ctx, "Scale", 0.0d, scale, 100.0d, 0.1d, 0.1d);
					break;
			}
		}
		nk_end(gui->ctx);
		
		if (progr_win){
			/* opening */
			if (nk_begin(gui->ctx, "Progress", nk_rect(200, 200, 400, 40),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR))
			//if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "opening", 0, nk_rect(200, 200, 400, 40)))
			{
				static char text[64];
				static int text_len;
				nk_layout_row_dynamic(gui->ctx, 20, 2);
				text_len = snprintf(text, 63, "Opening...");
				nk_label(gui->ctx, text, NK_TEXT_LEFT);
				nk_progress(gui->ctx, &progress, 100, NK_FIXED);
				//nk_popup_end(gui->ctx);
				nk_end(gui->ctx);
			}
			
			if (progr_end){
				progr_win = 0;
				progr_end = 0;
				nk_window_close(gui->ctx, "Progress");
			}
		}
		
		/* status */
		if (nk_begin(gui->ctx, "status", nk_rect(415, height - 32, 600, 32),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			static char comm[64];
			static int comm_len;
			
			static char text[64];
			static int text_len;
			float ratio[] = {0.4f, 0.2f, 0.4f};
			
			nk_layout_row(gui->ctx, NK_DYNAMIC, 20, 3, ratio);
			
			//nk_edit_focus(gui->ctx, 0);
			nk_flags res = nk_edit_string(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER, comm, &comm_len, 64, nk_filter_default);
			//
			//NK_EDIT_ACTIVE
			if (res & NK_EDIT_COMMITED){
				comm[comm_len] = 0;
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s", comm);
				//printf("%s\n", comm);
				comm_len = 0;
			}
			
			text_len = snprintf(text, 63, "Layers=%d", drawing->num_layers);
			nk_label(gui->ctx, text, NK_TEXT_LEFT);
			/* view coordinates of mouse in drawing units */
			text_len = snprintf(text, 63, "x = %f, y = %f", pos_x, pos_y);
			nk_label(gui->ctx, text, NK_TEXT_LEFT);
			
			/*if (wait_open != 0){
				text_len = snprintf(text, 63, "Opening...");
				nk_label(gui->ctx, text, NK_TEXT_LEFT);
				nk_progress(gui->ctx, &progress, 100, NK_FIXED);
			}*/
		}
		nk_end(gui->ctx);
		
		/* interface to the user visualize and enter coordinates distances*/
		if (nk_begin(gui->ctx, "POS", nk_rect(0, height - 32, 400, 32),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			float ratio[] = {0.1f, 0.4f, 0.1f, 0.4f};
			nk_flags res;
			/* flags to verify which coordinate is predominant */
			int flag_x = fabs(step_x[step] - step_x[step - 1]) >= fabs(step_y[step] - step_y[step - 1]);
			int flag_y = fabs(step_x[step] - step_x[step - 1]) < fabs(step_y[step] - step_y[step - 1]);
			
			nk_layout_row(gui->ctx, NK_DYNAMIC, 20, 4, ratio);
			
			/* X distance */
			/* hilite coordinate, if coord is predominant during a drawing operation*/
			if ((en_distance) && (step > 0) && (step < 10) && (flag_x)){
				nk_label_colored(gui->ctx, "X=", NK_TEXT_RIGHT, nk_rgb(255,255,0));
			}
			else {
				nk_label(gui->ctx, "X=", NK_TEXT_RIGHT);
			}
			/* verify if the user initiate a number entry during a drawing operation */
			if ((en_distance) && (user_number) && (step > 0) && (step < 10) &&
			(!user_flag_x) && (flag_x)){
				user_number = 0; /* clear user flag */
				user_str_x[0] = 0; /* clear edit string */
				/* set focus to edit */
				nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
			}
			
			/* edit to visualize or enter distance */
			res = nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT, user_str_x, 63, nk_filter_float);
			if (res & NK_EDIT_ACTIVE){ /* enter mode */
				if (strlen(user_str_x)){
					/* sinalize the distance of user entry */
					user_x = atof(user_str_x);
					user_flag_x = 1;
				}
				else{ /* if the user clear the string */
					/* cancel the enter mode*/
					user_flag_x = 0;
					nk_edit_unfocus(gui->ctx);
				}
			}
			else { /* visualize mode */
				if ((en_distance) && (step > 0) && (step < 10)){
					snprintf(user_str_x, 63, "%f", step_x[step] - step_x[step - 1]);
				}
				else {
					snprintf(user_str_x, 63, "%f", 0.0);
				}
			}
			if (res & NK_EDIT_COMMITED){ /* finish the enter mode */
				nk_edit_unfocus(gui->ctx);
				keyEnter = 0;
			}
			
			/* Y distance */
			/* hilite coordinate, if coord is predominant during a drawing operation*/
			if ((en_distance) && (step > 0) && (step < 10) && (flag_y)){
				nk_label_colored(gui->ctx, "Y=", NK_TEXT_RIGHT, nk_rgb(255,255,0));
			}
			else {
				nk_label(gui->ctx, "Y=", NK_TEXT_RIGHT);
			}
			/* verify if the user initiate a number entry during a drawing operation */
			if ((en_distance) && (user_number) && (step > 0) && (step < 10) &&
			(!user_flag_y) && (flag_y)){
				user_number = 0; /* clear user flag */
				user_str_y[0] = 0; /* clear edit string */
				/* set focus to edit */
				nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
			}
			
			/* edit to visualize or enter distance */
			res = nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT, user_str_y, 63, nk_filter_float);
			if (res & NK_EDIT_ACTIVE){ /* enter mode */
				if (strlen(user_str_y)){
					/* sinalize the distance of user entry */
					user_y = atof(user_str_y);
					user_flag_y = 1;
				}
				else{ /* if the user clear the string */
					/* cancel the enter mode*/
					user_flag_y = 0;
					nk_edit_unfocus(gui->ctx);
				}
			}
			else { /* visualize mode */
				if ((en_distance) && (step > 0) && (step < 10)){
					snprintf(user_str_y, 63, "%f", step_y[step] - step_y[step - 1]);
				}
				else {
					snprintf(user_str_y, 63, "%f", 0.0);
				}
			}
			if (res & NK_EDIT_COMMITED){ /* finish the enter mode */
				nk_edit_unfocus(gui->ctx);
				keyEnter = 0;
			}
		}
		nk_end(gui->ctx);
		
		/* */
		if (nk_begin(gui->ctx, "Prop", nk_rect(200, 0, 750, 32),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_NO_SCROLLBAR))
		{
			static char text[64];
			int text_len;
			nk_layout_row_begin(gui->ctx, NK_STATIC, 20, 9);
			
			/*layer*/
			nk_layout_row_push(gui->ctx, 200);
			if (nk_combo_begin_label(gui->ctx, drawing->layers[layer_idx].name, nk_vec2(200,200))){
				float wid[] = {120, 20, 20};
				nk_layout_row(gui->ctx, NK_STATIC, 20, 3, wid);
				int num_layers = drawing->num_layers;
				for (i = 0; i < num_layers; i++){
					//strcpy(layer_nam[i], drawing->layers[i].name);
					if (nk_button_label(gui->ctx, drawing->layers[i].name)){
						layer_idx = i;
						nk_combo_close(gui->ctx);
					}
					if (drawing->layers[i].off){
						if (nk_button_image_styled(gui->ctx, &b_icon_style, i_unview)){
							drawing->layers[i].off = 0;
						}
					}
					else{
						if (nk_button_image_styled(gui->ctx, &b_icon_style, i_view)){
							drawing->layers[i].off = 1;
						}
					}
					if (drawing->layers[i].lock){
						if (nk_button_image_styled(gui->ctx, &b_icon_style, i_lock)){
							drawing->layers[i].lock = 0;
						}
					}
					else{
						if (nk_button_image_styled(gui->ctx, &b_icon_style, i_unlock)){
							drawing->layers[i].lock = 1;
						}
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
			if (nk_combo_begin_color(gui->ctx, combo_color, nk_vec2(215,320))) {
				nk_layout_row_static(gui->ctx, 15, 15, 10);
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
			
			/*line type*/
			nk_layout_row_push(gui->ctx, 200);
			if (nk_combo_begin_label(gui->ctx, drawing->ltypes[ltypes_idx].name, nk_vec2(300,200))){
				nk_layout_row_dynamic(gui->ctx, 25, 2);
				int num_ltypes = drawing->num_ltypes;
				
				for (i = 0; i < num_ltypes; i++){
					
					if (nk_button_label(gui->ctx, drawing->ltypes[i].name)){
						ltypes_idx = i;
						nk_combo_close(gui->ctx);
					}
					nk_label(gui->ctx, drawing->ltypes[i].descr, NK_TEXT_LEFT);
				}
				
				nk_combo_end(gui->ctx);
			}
			
			/* thickness */
			nk_layout_row_push(gui->ctx, 150);
			//nk_property_float(struct nk_context*, const char *name, float min, float *val, float max, float step, float inc_per_pixel);
			//nk_property_float(gui->ctx, "Thick:", 0.0, &thick, 20.0, 0.1, 0.1);
			
			//double nk_propertyd(struct nk_context*, const char *name, double min, double val, double max, double step, float inc_per_pixel);
			thick = nk_propertyd(gui->ctx, "Thickness", 0.0d, thick, 20.0d, 0.1d, 0.1d);
			
			
			/* zoom*/
			nk_layout_row_push(gui->ctx, 20);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_z_plus)){
				printf("ZOOM +\n");
			}
			nk_layout_row_push(gui->ctx, 20);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_z_minus)){
				printf("ZOOM -\n");
			}
			nk_layout_row_push(gui->ctx, 20);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_z_win)){
				printf("ZOOM win\n");
			}
			nk_layout_row_push(gui->ctx, 20);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, i_z_all)){
				printf("ZOOM all\n");
				action = VIEW_ZOOM_EXT;
			}
			
			nk_layout_row_end(gui->ctx);
			nk_end(gui->ctx);
		}
		
		if (!(nk_window_is_any_hovered(gui->ctx)) && (modal != SELECT)){
			nk_window_set_focus(gui->ctx, "POS");
		}
		
		if (wait_open != 0){
			low_proc = 0;
			draw = 1;
			
			open_prg = dxf_read(drawing, file_buf, file_size, &progress);
			
			if(open_prg <= 0){
				free(file_buf);
				file_buf = NULL;
				file_size = 0;
				
				dxf_ents_parse(drawing);				
				action = VIEW_ZOOM_EXT;
				layer_idx = 0;
				ltypes_idx = 0;
				color_idx = 256;
				wait_open = 0;
				progr_end = 1;
				list_clear(sel_list);
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
				
				file_buf = dxf_load_file(url, &file_size);
				open_prg = dxf_read(drawing, file_buf, file_size, &progress);
				
				low_proc = 0;
				progr_win = 1;
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
				//dxf_ent_print_f (drawing->main_struct, url);
				dxf_save (url, drawing);
			}
		}
		else if(action == EXPORT) {
			action = NONE;
			url = (char *) tinyfd_saveFileDialog(
			"Save Drawing",
			"save.txt",
			2,
			lFilterPatterns,
			NULL);
			if ((url != NULL) && (drawing->main_struct != NULL)){
				dxf_ent_print_f (drawing->main_struct, url);
				//dxf_save (url, drawing);
			}
		}
		else if(action == VIEW_ZOOM_EXT){
			action = NONE;
			zoom_ext(drawing, img, &zoom, &ofs_x, &ofs_y);
			draw = 1;
		}
		else if(action == DELETE){
			action = NONE;
		
			if (sel_list != NULL){
				list_node *current = sel_list->next;
				
				// starts the content sweep 
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							
							// -------------------------------------------
							dxf_obj_detach((dxf_node *)current->data);
							
							//---------------------------------------
						}
					}
					current = current->next;
				}
				list_clear(sel_list);
			}
			draw = 1;
		}
		
		if(recv_comm_flag){
			recv_comm_flag =0;
			str_upp(recv_comm);
			if (strcmp(recv_comm, "SELECT") == 0){
				modal = SELECT;
				step = 0;
			}
			else if (strcmp(recv_comm, "LINE") == 0){
				modal = LINE;
				step = 0;
			}
			else if (strcmp(recv_comm, "POLYLINE") == 0){
				modal = POLYLINE;
				step = 0;
			}
			else if (strcmp(recv_comm, "DELETE") == 0){
				action = DELETE;
				step = 0;
			}
			else if (strcmp(recv_comm, "CIRCLE") == 0){
				modal = CIRCLE;
				step = 0;
			}
			else if (strcmp(recv_comm, "RECT") == 0){
				modal = RECT;
				step = 0;
			}
			else if (strcmp(recv_comm, "TEXT") == 0){
				modal = TEXT;
				step = 0;
			}
			else if (strcmp(recv_comm, "MOVE") == 0){
				modal = MOVE;
				step = 0;
			}
			else if (strcmp(recv_comm, "DUPLI") == 0){
				modal = DUPLI;
				step = 0;
			}
			else if (strcmp(recv_comm, "SCALE") == 0){
				modal = SCALE;
				step = 0;
			}
		}
		
		/* if user hit the enter key during a drawing operation, toggle axis lock */
		if ((modal != SELECT) && (step > 0) && (keyEnter)
		//&& (!user_flag_x) && (!user_flag_y)
		){
			if ((lock_ax_x != 0) || (lock_ax_y != 0)){
				/* release the lock, if previously active */
				lock_ax_x = 0;
				lock_ax_y = 0;
			}
			else{
				/* activate the lock according coordinate is predominant */
				lock_ax_x = fabs(step_x[step] - step_x[step - 1]) >= fabs(step_y[step] - step_y[step - 1]);
				lock_ax_y = fabs(step_x[step] - step_x[step - 1]) < fabs(step_y[step] - step_y[step - 1]);
			}
		}
		
		/* events to update current coordinates according the mouse position,
		axis locks, user entry, or DXf element attractor */
		if ((leftMouseButtonClick) || (rightMouseButtonClick) || (MouseMotion)){
			/* aproximation rectangle in mouse position (10 pixels wide) */
			rect_pt1[0] = (double) (mouse_x - 5)/zoom + ofs_x;
			rect_pt1[1] = (double) (mouse_y - 5)/zoom + ofs_y;
			rect_pt2[0] = (double) (mouse_x + 5)/zoom + ofs_x;
			rect_pt2[1] = (double) (mouse_y + 5)/zoom + ofs_y;
			/* get the drawing element near the mouse */
			near_el = (dxf_node *)dxf_ents_isect(drawing, rect_pt1, rect_pt2);
			
			if ((step >= 0) && (step < 10)){
				/* update current position by the mouse */
				step_x[step] = (double) mouse_x/zoom + ofs_x;
				step_y[step] = (double) mouse_y/zoom + ofs_y;
				if (step >= 1){
					/* update current position by the attractor of near element */
					if (near_attr = dxf_ent_attract(drawing, near_el, curr_attr_t,
					step_x[step], step_y[step], step_x[step-1], step_y[step-1],
					(double) 20/zoom, &near_x , &near_y)){
						step_x[step] = near_x;
						step_y[step] = near_y;
					}
				}
				else {
					/* update current position by the attractor of near element */
					if (near_attr = dxf_ent_attract(drawing, near_el, curr_attr_t,
					step_x[step], step_y[step], step_x[step], step_y[step],
					(double) 20/zoom, &near_x , &near_y)){
						step_x[step] = near_x;
						step_y[step] = near_y;
					}
				}
			}
			
			/* compute the next point coordinates by axis distances entry */
			if ((en_distance) && (step > 0) && (step < 10)){
				/* verify if an axis is locked during a drawing operation */
				if (lock_ax_y != 0){
					step_x[step] = step_x[step - 1];
				}
				if (lock_ax_x != 0){
					step_y[step] = step_y[step - 1];
				}
				/* check the user entry */
				if (user_flag_x){
					step_x[step] = step_x[step - 1] + user_x;
				}
				if (user_flag_y){
					step_y[step] = step_y[step - 1] + user_y;
				}
			}
			
		}
		
		if (prev_modal != modal){
			prev_modal = modal;
			en_distance = 0;
			draw_tmp = 0;
			element = NULL;
			draw = 1;
			step = 0;
			draw_phanton = 0;
			if (phanton){
				free(phanton->data);
				free(phanton);
				phanton = NULL;
			}
			lock_ax_x = 0;
			lock_ax_y = 0;
			user_flag_x = 0;
			user_flag_y = 0;
			//printf("change tool\n");
		}
		
		if (modal == SELECT){
			if (leftMouseButtonClick){
				sel_list_append(sel_list, element);
				/* -------------------------------test-------------- */
				
				/*dxf_edit_move (element, 0.0 , 0.0, 0.0);
				
				/*--------------------------------------------- 
				dxf_node *current, *start, *end;
				if(dxf_find_ext_appid(element, "ZECRUEL", &start, &end)){
					printf("ext data Zecruel, start = %d, end = %d\n", start, end);
					current = start;
					while (current != NULL){
						printf ("%d = ", current->value.group); 
						
						switch (current->value.t_data) {
							case DXF_STR:
								if(current->value.s_data){
									printf(current->value.s_data);
								}
								break;
							case DXF_FLOAT:
								printf("%f", current->value.d_data);
								break;
							case DXF_INT:
								printf("%d", current->value.i_data);
						}
						printf("\n");
						//printf ("%x\n", current);
						if (current == end) break;
						current = current->next;
					}
				}*/
				/* -------------------------------test-------------- */
				
				//dxf_ent_attract (dxf_node * obj, enum attract_type type, double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y)
				if (element){
					if(element->type == DXF_ENT){
						dxf_ent_print2(element);
						/*
						printf("%s\n",element->obj.name);
						if (dxf_ident_ent_type (element)  ==  DXF_INSERT){
							dxf_node *volta = dxf_find_attr2(element, 2);
							if (volta){
								printf("%s\n",volta->value.s_data);
							}
						}*/
					}
				}
				double ret_x, ret_y;
				
				/*---------------------------------------------  */
			}
			if (rightMouseButtonClick){
				list_clear(sel_list);
				draw = 1;
			}
			if (MouseMotion){				
				/* for hilite test */
				element = near_el;
				draw = 1;
			}
		}
		{
		if (modal == LINE){
			if (step == 0){
				if (leftMouseButtonClick){
					draw_tmp = 1;
					/* create a new DXF line */
					new_el = (dxf_node *) dxf_new_line (
						step_x[step], step_y[step], 0.0, step_x[step], step_y[step], 0.0, /* pt1, pt2 */
						(double) thick, 0.0, /* thickness, elevation */
						color_idx, drawing->layers[layer_idx].name, /* color, layer */
						drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
					element = new_el;
					step = 1;
					en_distance = 1;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					dxf_attr_change(new_el, 11, &step_x[step]);
					dxf_attr_change(new_el, 21, &step_y[step]);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
					drawing_ent_append(drawing, new_el);
					
					step_x[step - 1] = step_x[step];
					step_y[step - 1] = step_y[step];
					
					new_el = (dxf_node *) dxf_new_line (
						step_x[step], step_y[step], 0.0, step_x[step], step_y[step], 0.0, /* pt1, pt2 */
						(double) thick, 0.0, /* thickness, elevation */
						color_idx, drawing->layers[layer_idx].name, /* color, layer */
						drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
					
					element = new_el;
					goto next_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change(new_el, 11, &step_x[step]);
					dxf_attr_change(new_el, 21, &step_y[step]);
					dxf_attr_change(new_el, 39, &thick);
					dxf_attr_change(new_el, 62, &color_idx);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == POLYLINE){
			if (step == 0){
				if (leftMouseButtonClick){
					/* create a new DXF lwpolyline */
					new_el = (dxf_node *) dxf_new_lwpolyline (
						step_x[step], step_y[step], 0.0, /* pt1, */
						bulge, (double) thick, /* bulge, thickness, */
						color_idx, drawing->layers[layer_idx].name, /* color, layer */
						drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
					dxf_lwpoly_append (new_el, step_x[step], step_y[step], 0.0, bulge);
					element = new_el;
					step = 1;
					en_distance = 1;
					draw_tmp = 1;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					step_x[step - 1] = step_x[step];
					step_y[step - 1] = step_y[step];
					
					dxf_attr_change_i(new_el, 10, &step_x[step], -1);
					dxf_attr_change_i(new_el, 20, &step_y[step], -1);
					dxf_attr_change_i(new_el, 42, &bulge, -1);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
					
					dxf_lwpoly_append (new_el, step_x[step], step_y[step], 0.0, bulge);
					step = 2;
					goto next_step;
				}
				else if (rightMouseButtonClick){
					draw_tmp = 0;
					if (step == 2){
						dxf_lwpoly_remove (new_el, -1);
						new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
						drawing_ent_append(drawing, new_el);
						step = 0;
					}
					element = NULL;
					goto next_step;
				}
				if (MouseMotion){
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change_i(new_el, 10, &step_x[step], -1);
					dxf_attr_change_i(new_el, 20, &step_y[step], -1);
					dxf_attr_change_i(new_el, 42, &bulge, -1);
					dxf_attr_change(new_el, 39, &thick);
					dxf_attr_change(new_el, 62, &color_idx);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == CIRCLE){
			if (step == 0){
				if (leftMouseButtonClick){
					draw_tmp = 1;
					/* create a new DXF circle */
					new_el = (dxf_node *) dxf_new_circle (
						step_x[step], step_y[step], 0.0, 0.0, /* pt1, radius */
						(double) thick, /* thickness, elevation */
						color_idx, drawing->layers[layer_idx].name, /* color, layer */
						drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
					element = new_el;
					step = 1;
					en_distance = 1;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					double radius = sqrt(pow((step_x[step] - step_x[step - 1]), 2) + pow((step_y[step] - step_y[step - 1]), 2));
					dxf_attr_change(new_el, 40, &radius);
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
					drawing_ent_append(drawing, new_el);
					
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					x1 = (double) mouse_x/zoom + ofs_x;
					y1 = (double) mouse_y/zoom + ofs_y;
					double radius = sqrt(pow((step_x[step] - step_x[step - 1]), 2) + pow((step_y[step] - step_y[step - 1]), 2));
					
					dxf_attr_change(new_el, 40, &radius);
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change(new_el, 39, &thick);
					dxf_attr_change(new_el, 62, &color_idx);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == RECT){
			if (step == 0){
				if (leftMouseButtonClick){
					
					draw_tmp = 1;
					int closed = 1;
					/* create a new DXF lwpolyline */
					new_el = (dxf_node *) dxf_new_lwpolyline (
						step_x[step], step_y[step], 0.0, /* pt1, */
						0.0, (double) thick, /* bulge, thickness, */
						color_idx, drawing->layers[layer_idx].name, /* color, layer */
						drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
					dxf_lwpoly_append (new_el, step_x[step], step_y[step], 0.0, 0.0);
					dxf_lwpoly_append (new_el, step_x[step], step_y[step], 0.0, 0.0);
					dxf_lwpoly_append (new_el, step_x[step], step_y[step], 0.0, 0.0);
					dxf_attr_change_i(new_el, 70, (void *) &closed, 0);
					element = new_el;
					step = 1;
					en_distance = 1;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					
					dxf_attr_change_i(new_el, 10, (void *) &step_x[step], 1);
					dxf_attr_change_i(new_el, 10, (void *) &step_x[step], 2);
					dxf_attr_change_i(new_el, 20, (void *) &step_y[step], 2);
					dxf_attr_change_i(new_el, 20, (void *) &step_y[step], 3);
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
					drawing_ent_append(drawing, new_el);
					
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					dxf_attr_change_i(new_el, 10, (void *) &step_x[step], 1);
					dxf_attr_change_i(new_el, 10, (void *) &step_x[step], 2);
					dxf_attr_change_i(new_el, 20, (void *) &step_y[step], 2);
					dxf_attr_change_i(new_el, 20, (void *) &step_y[step], 3);
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change(new_el, 39, &thick);
					dxf_attr_change(new_el, 62, &color_idx);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == TEXT){
			if (step == 0){
				draw_tmp = 1;
				/* create a new DXF text */
				new_el = (dxf_node *) dxf_new_text (
					step_x[step], step_y[step], 0.0, txt_h, /* pt1, height */
					txt, (double) thick, /* text, thickness */
					color_idx, drawing->layers[layer_idx].name, /* color, layer */
					drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
				element = new_el;
				dxf_attr_change_i(new_el, 72, &t_al_h, -1);
				dxf_attr_change_i(new_el, 73, &t_al_v, -1);
				step = 1;
			}
			else{
				if (leftMouseButtonClick){
					dxf_attr_change_i(new_el, 10, &step_x[step], -1);
					dxf_attr_change_i(new_el, 20, &step_y[step], -1);
					dxf_attr_change_i(new_el, 11, &step_x[step], -1);
					dxf_attr_change_i(new_el, 21, &step_y[step], -1);
					dxf_attr_change(new_el, 40, &txt_h);
					dxf_attr_change(new_el, 1, txt);
					dxf_attr_change_i(new_el, 72, &t_al_h, -1);
					dxf_attr_change_i(new_el, 73, &t_al_v, -1);
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
					drawing_ent_append(drawing, new_el);
					step_x[step - 1] = step_x[step];
					step_y[step - 1] = step_y[step];
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
				if (MouseMotion){
					dxf_attr_change_i(new_el, 10, &step_x[step], -1);
					dxf_attr_change_i(new_el, 20, &step_y[step], -1);
					dxf_attr_change_i(new_el, 11, &step_x[step], -1);
					dxf_attr_change_i(new_el, 21, &step_y[step], -1);
					dxf_attr_change(new_el, 40, &txt_h);
					dxf_attr_change(new_el, 1, txt);
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change(new_el, 39, &thick);
					dxf_attr_change(new_el, 62, &color_idx);
					dxf_attr_change_i(new_el, 72, &t_al_h, -1);
					dxf_attr_change_i(new_el, 73, &t_al_v, -1);
					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == MOVE){
			if (step == 0){
				if (leftMouseButtonClick){
					draw_tmp = 1;
					/* phantom object */
					phanton = dxf_list_parse(drawing, sel_list, 0, 0);
					element = NULL;
					draw_phanton = 1;
					en_distance = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
					step = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					if (sel_list != NULL){
						/* sweep the selection list */
						list_node *current = sel_list->next;
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									dxf_edit_move((dxf_node *)current->data, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									((dxf_node *)current->data)->obj.graphics = dxf_graph_parse(drawing, ((dxf_node *)current->data), 0 , 0);
								}
							}
							current = current->next;
						}
						//list_clear(sel_list);
					}
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					vec_graph_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
			}
		}
		if (modal == DUPLI){
			if (step == 0){
				if (leftMouseButtonClick){
					draw_tmp = 1;
					/* phantom object */
					phanton = dxf_list_parse(drawing, sel_list, 0, 0);
					element = NULL;
					draw_phanton = 1;
					en_distance = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
					step = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					if (sel_list != NULL){
						/* sweep the selection list */
						list_node *current = sel_list->next;
						dxf_node *new_ent = NULL;
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
									dxf_edit_move(new_ent, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
									drawing_ent_append(drawing, new_ent);
									
									current->data = new_ent;
								}
							}
							current = current->next;
						}
						//list_clear(sel_list);
					}
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					vec_graph_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
			}
		}
		if (modal == SCALE){
			if (step == 0){
				if (leftMouseButtonClick){
					draw_tmp = 1;
					/* phantom object */
					phanton = dxf_list_parse(drawing, sel_list, 0, 0);
					vec_graph_modify(phanton, step_x[step]*(1 - scale), step_y[step]*(1 - scale), scale, scale, 0.0);
					element = NULL;
					draw_phanton = 1;
					en_distance = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
					step = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					if (sel_list != NULL){
						list_node *current = sel_list->next;
						/* sweep the selection list */
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									dxf_edit_scale((dxf_node *)current->data, scale, scale, scale);
									dxf_edit_move((dxf_node *)current->data, step_x[step - 1]*(1 - scale), step_y[step - 1]*(1 - scale), 0.0);
									dxf_edit_move((dxf_node *)current->data, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									
									((dxf_node *)current->data)->obj.graphics = dxf_graph_parse(drawing, ((dxf_node *)current->data), 0 , 0);
								}
							}
							current = current->next;
						}
						current = sel_list->next;
					}
					goto first_step;
				}
				else if (rightMouseButtonClick){
					goto first_step;
				}
				if (MouseMotion){
					vec_graph_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
				}
			}
		}
		goto end_step;
		default_modal:
			modal = SELECT;
		first_step:
			en_distance = 0;
			draw_tmp = 0;
			element = NULL;
			draw = 1;
			step = 0;
			draw_phanton = 0;
			if (phanton){
				free(phanton->data);
				free(phanton);
				phanton = NULL;
			}
		next_step:
			
			//draw_tmp = 0;
			//element = NULL;
			lock_ax_x = 0;
			lock_ax_y = 0;
			user_flag_x = 0;
			user_flag_y = 0;

			draw = 1;
		end_step: ;
		
		}
		
		if (gui_check_draw(gui) != 0){
			draw = 1;
		}
		
		if (draw != 0){
		
			bmp_fill(img, img->bkg); /* clear bitmap */
			dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
			
			draw_cursor(img, mouse_x, mouse_y, cursor);
			
			if (near_attr){ /* check if needs to draw an attractor mark */
				/* convert entities coordinates to screen coordinates */
				int attr_x = (int) round((near_x - ofs_x) * zoom);
				int attr_y = (int) round((near_y - ofs_y) * zoom);
				draw_attractor(img, near_attr, attr_x, attr_y, yellow);
			}
			/*hilite test */
			if((draw_tmp)&&(element != NULL)){
				element->obj.graphics = dxf_graph_parse(drawing, element, 0 , 1);
			}
			if(element != NULL){
				vec_graph_draw_fix(element->obj.graphics, img, ofs_x, ofs_y, zoom, hilite);
			}
			if((draw_phanton)&&(phanton)){
				//dxf_list_draw(sel_list, img, ofs_x - (x1 - x0), ofs_y - (y1 - y0), zoom, hilite);
				vec_graph_draw_fix(phanton, img, ofs_x, ofs_y, zoom, hilite);
			}
			dxf_list_draw(sel_list, img, ofs_x, ofs_y, zoom, hilite);
			
			nk_sdl_render(gui, img);
			
			SDL_UpdateTexture(canvas, NULL, img->buf, width * 4);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, canvas, NULL, NULL);
			SDL_RenderPresent(renderer);
			
			//SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
			SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEMOTION);
			draw = 0;
		}
		
		leftMouseButtonClick = 0;
		rightMouseButtonClick = 0;
		MouseMotion = 0;
		keyEnter = 0;
		
		//graph_mem_pool(ZERO_GRAPH, 2);
		//graph_mem_pool(ZERO_LINE, 2);
		
		graph_mem_pool(ZERO_GRAPH, 1);
		graph_mem_pool(ZERO_LINE, 1);
		nk_clear(gui->ctx); /*IMPORTANT */
		if (low_proc){
			SDL_Delay(20);
		}
	}
	
	/* safe quit */
	SDL_DestroyTexture(canvas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	list_mem_pool(FREE_LIST, 0);
	list_mem_pool(FREE_LIST, 1);
	dxf_mem_pool(FREE_DXF, 0);
	graph_mem_pool(FREE_ALL, 0);
	graph_mem_pool(FREE_ALL, 1);
	graph_mem_pool(FREE_ALL, 2);
	
	bmp_free(img);
	bmp_free(tool_img);
	for(i = 0; i < 40; i++){
		bmp_free(tool_vec[i]);
	}
	for (i = 0; i<drawing->num_fonts; i++){
		shx_font_free(drawing->text_fonts[i].shx_font);
	}
	nk_sdl_shutdown(gui);
	shx_font_free(shx_font);
	free(drawing);
		
	return 0;
}