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
#include "i_svg_media.h"

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

#define NANOSVG_IMPLEMENTATION
#include "nanosvg.h"
#define NANOSVGRAST_IMPLEMENTATION
#include "nanosvgrast.h"

#ifdef __linux__
#define OS_LINUX
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif

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

void nk_dxf_ent_info (struct nk_context *ctx, dxf_node *ent, int id){ /* print the entity structure */
	/* use Nuklear widgets to show a DXF entity structure */
	/* this function is non recursive */
	int i = 0;
	int level = 0;
	dxf_node *current, *prev;
	int tree_st[10]; /* max of ten levels of entities inside entities*/
	char text[401];
	
	text[0] = 0;
	id *= 1000; /* each Tree widget has a unique id, up to 1000 inside main entity*/
	
	current = ent;
	while (current){
		prev = current;
		if (current->type == DXF_ENT){
			/* DXF entities are show as Tree widget */
			if (current->obj.name){
				id++; /* increment id for child Trees */
				if (level == 0){ /* verify if is the first Tree */
					if (tree_st[level] = nk_tree_push_id(ctx, NK_TREE_TAB, current->obj.name, NK_MINIMIZED, id)) {
						/* if Tree is not minimized, start the placement of child widgets */
						nk_layout_row_dynamic(ctx, 13, 1);
						nk_label(ctx, "-----", NK_TEXT_LEFT);
					}
				}
				else if (tree_st[level - 1]){ /* verify if the up level Tree is not minimized */
					if (tree_st[level] = nk_tree_push_id(ctx, NK_TREE_TAB, current->obj.name, NK_MINIMIZED, id)) {
						/* if Tree is not minimized, start the placement of child widgets */
						nk_layout_row_dynamic(ctx, 13, 1);
						nk_label(ctx, "-----", NK_TEXT_LEFT);
					}
				}
				else{
					tree_st[level] = 0;
				}
			}
			if (current->obj.content){
				/* starts entity content sweep */
				prev = current->obj.content;
				current = current->obj.content->next;
				level++;
			}
		}
		else if (current->type == DXF_ATTR){
			/* DXF attributes are show as Label widget */
			/* combine Group and Value, acording its type, in a string */
			i = snprintf (text, 400, "%d = ", current->value.group);
			switch (current->value.t_data) {
				case DXF_STR:
					if(current->value.s_data){
						i += snprintf (text + i, 400 - i, current->value.s_data);
					}
					break;
				case DXF_FLOAT:
					i += snprintf (text + i, 400 - i, "%f", current->value.d_data);
					break;
				case DXF_INT:
					i += snprintf (text + i, 400 - i, "%d", current->value.i_data);
			}
			if (tree_st[level - 1]){ /* verify if the up level Tree is not minimized */
				nk_label(ctx, text, NK_TEXT_LEFT);
			}
			/* clear string */
			i = 0; text[0] = 0;
			
			current = current->next; /* go to the next in the list */
		}
		/* if the contents sweep reachs to end, try to up in structure */
		while (current == NULL){ 
			if (prev == ent){ /* back to original entity */
				/* ends the top level Tree, if not minimized */
				if (tree_st[level - 1]){
					nk_tree_pop(ctx);
				}
				current = NULL; /* ends loop */
				break;
			}
			
			prev = prev->master;
			if (prev){
				current = prev->next; /* continue loop */
				level --; /* up in structure*/
				/* ends the current Tree, if not minimized */
				if (tree_st[level]){
					nk_tree_pop(ctx);
				}
				/* back to original entity */
				if (prev == ent){
					current = NULL; /* ends loop */
					break;
				}
			}
			else{ /* if structure ends */
				current = NULL; /* ends loop */
				break;
			}
		}
		
		if ((level < 0) || (level > 9)){/* verify if level is out of admissible range */
			current = NULL; /* ends loop */
			break;
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

void attrc_get_imgs(bmp_img *vec[], int num, int w, int h){
	
	bmp_color yellow = {.r = 255, .g = 255, .b =0, .a = 255};
	bmp_color transp = {.r = 255, .g = 255, .b = 255, .a = 0};
	
	int i, attr = 1, x = w/2, y = h/2;
	for (i = 0; i < num; i++){
		vec[i] = bmp_new(w, h, transp, yellow);
		draw_attractor(vec[i], attr, x, y, yellow);
		attr <<= 1;
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

void zoom_ext2(dxf_drawing *drawing, int x, int y, int width, int height, double *zoom, double *ofs_x, double *ofs_y){
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y;
	dxf_ents_ext(drawing, &min_x, &min_y, &max_x, &max_y);
	zoom_x = fabs(max_x - min_x)/width;
	zoom_y = fabs(max_y - min_y)/height;
	*zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	*zoom = 1/(1.1 * (*zoom));
	
	*ofs_x = min_x - ((fabs((max_x - min_x)*(*zoom) - width)/2)+x)/(*zoom);
	*ofs_y = min_y - ((fabs((max_y - min_y)*(*zoom) - height)/2)+y)/(*zoom);
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

double font_scale(shape *font, float height){
	double fnt_scale = 0;
	/* find the dimentions of SHX font */
	double fnt_size = 0, fnt_above, fnt_below;
	if(font){ /* if the font exists */
		if(font->next){ /* the font descriptor is stored in first iten of list */
			if(font->next->cmd_size > 1){ /* check if the font is valid */
				fnt_above = font->next->cmds[0]; /* size above the base line of text */
				fnt_below = font->next->cmds[1]; /* size below the base line of text */
				if((fnt_above + fnt_below) > 0){
					fnt_size = fnt_above + fnt_below;
				}
			}
		}
	}
	if (fnt_size != 0) fnt_scale = height/fnt_size;
	return fnt_scale;
}
#ifdef __MINGW32__
int WinMain(int argc, char** argv){
#else
int main(int argc, char** argv){
#endif
	//setlocale(LC_ALL,""); //seta a localidade como a current do computador para aceitar acentuacao
	int i, ok;
	
	
	/* background image dimension */
	unsigned int main_w = 0;
	unsigned int main_h = 0;
	
	/* Window dimension */
	unsigned int win_w = 1200;
	unsigned int win_h = 715;
	SDL_Rect win_r, display_r;
	
	/* init the SDL2 */
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window * window = SDL_CreateWindow(
		"CadZinho", /* title */
		SDL_WINDOWPOS_UNDEFINED, /* x position */
		SDL_WINDOWPOS_UNDEFINED, /* y position */
		win_w, /* width */
		win_h, /* height */
		SDL_WINDOW_RESIZABLE); /* flags */
		
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	
	SDL_RendererInfo rend_info;
	
	SDL_GetRendererInfo(renderer, &rend_info);
	
	main_w = rend_info.max_texture_width;
	main_h = rend_info.max_texture_height;
	
	if ((main_w <= 0) || (main_h <= 0)){
		main_w = 2048;
		main_h = 2048;
	}
	
	SDL_Texture * canvas = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, 
		main_w, /* width */
		main_h); /* height */
		
	//SDL_SetTextureBlendMode(canvas, SDL_BLENDMODE_BLEND);
	
	char fonts_path[DXF_MAX_CHARS];
	fonts_path[0] = 0;
	
	char *base_path = SDL_GetBasePath();
	
	strncpy(fonts_path, base_path, DXF_MAX_CHARS);
	strncat(fonts_path, "fonts/", DXF_MAX_CHARS - strlen(fonts_path));
	
	#ifdef OS_WIN
	char *sys_fonts_path = "C:/Windows/Fonts/";
	#elif defined(OS_LINUX)
	char *sys_fonts_path = "/usr/share/fonts/";
	#else
	char *sys_fonts_path = NULL;
	#endif
	
	printf(fonts_path);
	printf(sys_fonts_path);
	
	double zoom = 20.0 , ofs_x = -11.0, ofs_y = -71.0;
	double prev_zoom;
	int color_idx = 256;
	int layer_idx = 0, ltypes_idx = 0;
	dxf_node *element = NULL, *prev_el = NULL, *new_el = NULL, *near_el = NULL;
	double pos_x, pos_y, x0, y0, x1, y1, x2, y2, bulge = 0.0, txt_h = 1.0, scale = 1.0;
	double thick = 0.0, thick_prev = 0.0;
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
	
	
	
	
	
	/*gui pos variables */
	int next_win_x = 0, next_win_y = 0, next_win_w = 0, next_win_h = 0;
	
	
	int open_prg = 0;
	int progress = 0;
	int progr_win = 0;
	int progr_end = 0;
	unsigned int quit = 0;
	unsigned int wait_open = 0;
	int show_app_about = 0;
	int show_info = 0;
	
	int show_lay_mng = 0, sel_tmp = 0, show_color_pick = 0;
	
	int ev_type, draw = 0, draw_tmp = 0, draw_phanton = 0;
	list_node *phanton = NULL;
	struct nk_color background;
	
	char info_buffer[512];
	info_buffer[0] = 0;
	
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
	int entry_relative = 1;
	
	enum attract_type curr_attr_t = ATRC_END|ATRC_MID|ATRC_QUAD;
	double near_x, near_y;
	int near_attr; /* flag */
	int en_attr = 1;
	
	char log_msg[64];
	log_msg[0] = 0;
	
	
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
		VIEW_ZOOM_P,
		VIEW_ZOOM_M,
		VIEW_ZOOM_W,
		VIEW_PAN_U,
		VIEW_PAN_D,
		VIEW_PAN_L,
		VIEW_PAN_R,
		DELETE,
		UNDO,
		REDO,
		LAYER_CHANGE,
		COLOR_CHANGE,
		LTYPE_CHANGE,
		THICK_CHANGE,
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
		SCALE,
		NEW_BLK,
		INSERT
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
	
	/* initialize the undo/redo list */
	struct do_list list_do;
	init_do_list(&list_do);
	
	
	/* init the main image */
	bmp_img * img = bmp_new(main_w, main_h, grey, red);
	
	/* init Nuklear GUI */
	struct font_obj font;
	font.shx_font = shx_font_open("txt.shx");
	font.scale = 1.4;
	
	struct nk_user_font default_font;
	default_font.userdata = nk_handle_ptr(&font);
	default_font.height = 12.0;
	font.scale = font_scale(font.shx_font, default_font.height);
	default_font.width = nk_user_font_get_text_width;
	
	gui_obj *gui = nk_sdl_init(&default_font);
	
	set_style(gui->ctx, THEME_ZE);
	
	gui->ctx->style.edit.padding = nk_vec2(4, -6);
	
	/* init the toolbox image */
	#define ICON_SIZE 24
	
	NSVGimage **svg_curves = i_svg_all_curves();
	bmp_img **svg_bmp = i_svg_all_bmp(svg_curves, ICON_SIZE, ICON_SIZE);
	
	bmp_free(svg_bmp[SVG_LOCK]);
	svg_bmp[SVG_LOCK] = i_svg_bmp(svg_curves[SVG_LOCK], 16, 16);
	bmp_free(svg_bmp[SVG_UNLOCK]);
	svg_bmp[SVG_UNLOCK] = i_svg_bmp(svg_curves[SVG_UNLOCK], 16, 16);
	bmp_free(svg_bmp[SVG_EYE]);
	svg_bmp[SVG_EYE] = i_svg_bmp(svg_curves[SVG_EYE], 16, 16);
	bmp_free(svg_bmp[SVG_NO_EYE]);
	svg_bmp[SVG_NO_EYE] = i_svg_bmp(svg_curves[SVG_NO_EYE], 16, 16);
	bmp_free(svg_bmp[SVG_SUN]);
	svg_bmp[SVG_SUN] = i_svg_bmp(svg_curves[SVG_SUN], 16, 16);
	bmp_free(svg_bmp[SVG_FREEZE]);
	svg_bmp[SVG_FREEZE] = i_svg_bmp(svg_curves[SVG_FREEZE], 16, 16);
	bmp_free(svg_bmp[SVG_CZ]);
	svg_bmp[SVG_CZ] = i_svg_bmp(svg_curves[SVG_CZ], 16, 16);
	
	bmp_img *i_cz48 = i_svg_bmp(svg_curves[SVG_CZ], 48, 48);
	
	bmp_img * attr_vec[15];
	attrc_get_imgs(attr_vec, 15, 16, 16);
	
	struct nk_style_button b_icon_style;
	if (gui){
		b_icon_style = gui->ctx->style.button;
	}
	b_icon_style.image_padding.x = -4;
	b_icon_style.image_padding.y = -4;
	
	/* style for toggle buttons (or select buttons) with image */
	struct nk_style_button b_icon_sel_style, b_icon_unsel_style;
	if (gui){
		b_icon_sel_style = gui->ctx->style.button;
		b_icon_unsel_style = gui->ctx->style.button;
	}
	b_icon_unsel_style.normal = nk_style_item_color(nk_rgba(58, 67, 57, 255));
	b_icon_unsel_style.hover = nk_style_item_color(nk_rgba(73, 84, 72, 255));
	//b_icon_unsel_style.active = nk_style_item_color(nk_rgba(81, 92, 80, 255));
	b_icon_sel_style.image_padding.x = -4;
	b_icon_sel_style.image_padding.y = -4;
	b_icon_unsel_style.image_padding.x = -4;
	b_icon_unsel_style.image_padding.y = -4;
	
	bmp_img * color_img = bmp_new(15, 13, black, red);
	struct nk_image i_color = nk_image_ptr(color_img);
	
	/* other font */
	struct font_obj font_tiny;
	font_tiny.shx_font = font.shx_font;
	font_tiny.scale = 1.4;
	
	struct nk_user_font font_tiny_nk;
	font_tiny_nk.userdata = nk_handle_ptr(&font_tiny);
	font_tiny_nk.height = 8.0;
	font_tiny.scale = font_scale(font_tiny.shx_font, font_tiny_nk.height);
	font_tiny_nk.width = nk_user_font_get_text_width;
	
	int show_blk_pp = 0;
	bmp_img * blk_prvw_big;
	blk_prvw_big = bmp_new(160, 160, grey, red);
	char blk_name[DXF_MAX_CHARS], tag_mark[DXF_MAX_CHARS];
	blk_name[0] = 0;
	int show_hidden_blks = 0, text2tag = 0;
	
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
	
	//dxf_new_block(drawing, "teste", "0");
	//dxf_ent_print2(drawing->blks);
	//dxf_ent_print2(drawing->blks_rec);
	
	
	dxf_ents_parse(drawing);
	

	
	
	
	
	
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(
		(void *)svg_bmp[SVG_CZ]->buf,
		svg_bmp[SVG_CZ]->width,
		svg_bmp[SVG_CZ]->height,
		32, svg_bmp[SVG_CZ]->width * 4,
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);

	// The icon is attached to the window pointer
	SDL_SetWindowIcon(window, surface);

	// ...and the surface containing the icon pixel data is no longer required.
	SDL_FreeSurface(surface);

	
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

		
	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
	
	
	/*===================== teste ===============*/
	list_node * tt_test = list_new(NULL, PRG_LIFE);
	tt_parse4(tt_test, PRG_LIFE, "Ezequiel Rabelo de Aguiar  AV çβμπ");
	/*===================== teste ===============*/
	
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
						mouse_y = main_h - mouse_y;
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
						mouse_y = main_h - mouse_y;
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
						mouse_y = main_h - mouse_y;
						pos_x = (double) mouse_x/zoom + ofs_x;
						pos_y = (double) mouse_y/zoom + ofs_y;
						draw = 1;
						break;
					case SDL_MOUSEWHEEL:
						prev_zoom = zoom;
						zoom = zoom + event.wheel.y * 0.2 * zoom;
						
						SDL_GetMouseState(&mouse_x, &mouse_y);
						mouse_y = main_h - mouse_y;
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
						if ((event.key.keysym.sym == SDLK_RETURN) || (event.key.keysym.sym == SDLK_RETURN2)){
							keyEnter = 1;
						}
						else if (event.key.keysym.sym == SDLK_UP){
							action = VIEW_PAN_U;
						}
						else if (event.key.keysym.sym == SDLK_DOWN){
							action = VIEW_PAN_D;
						}
						else if (event.key.keysym.sym == SDLK_LEFT){
							action = VIEW_PAN_L;
						}
						else if (event.key.keysym.sym == SDLK_RIGHT){
							action = VIEW_PAN_R;
						}
						else if (event.key.keysym.sym == SDLK_KP_MINUS){
							action = VIEW_ZOOM_M;
						}
						else if (event.key.keysym.sym == SDLK_KP_PLUS){
							action = VIEW_ZOOM_P;
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
		
		next_win_h = 6 + 4 + ICON_SIZE + 4 + 6 + 4 + ICON_SIZE + 4 + 6 + 8;
		next_win_x = 2;
		next_win_y = 2;
		
		if (nk_begin(gui->ctx, "Main", nk_rect(next_win_x, next_win_y, win_w - 4, next_win_h),
		NK_WINDOW_BORDER)){
			/* first line */
			nk_layout_row_begin(gui->ctx, NK_STATIC, ICON_SIZE + 12, 8);
			
			/* file tools*/
			nk_layout_row_push(gui->ctx, 6*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "file", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_NEW]))){
					printf("NEW\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_OPEN]))){
					action = FILE_OPEN;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_SAVE]))){
					action = FILE_SAVE;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_EXPORT]))){
					action = EXPORT;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_CLOSE]))){
					printf("CLOSE\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_PRINT]))){
					printf("PRINT\n");
				}
				
				nk_group_end(gui->ctx);
			}
			
			/* clipboard tools*/
			nk_layout_row_push(gui->ctx, 3*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "clipboard", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				
				
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_COPY]))){
					printf("Copy\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_CUT]))){
					printf("Cut\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_PASTE]))){
					printf("Paste\n");
				}
				
				nk_group_end(gui->ctx);
			}
			
			/* undo/redo tools*/
			nk_layout_row_push(gui->ctx, 2*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "undo-redo", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_UNDO]))){
					action = UNDO;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_REDO]))){
					action = REDO;
				}
				nk_group_end(gui->ctx);
			}
			
			/* managers*/
			nk_layout_row_push(gui->ctx, 5*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "managers", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LAYERS]))){
					//printf("Layers\n");
					show_lay_mng = 1;
					//sel_tmp = -1;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_FONT]))){
					printf("FONT\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LTYPE]))){
					printf("Line types\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_PUZZLE]))){
					printf("Blocks\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TAGS]))){
					printf("APPID\n");
				}
				nk_group_end(gui->ctx);
			}
			
			/* zoom tools*/
			nk_layout_row_push(gui->ctx, 4*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "zoom", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ZOOM_P]))){
					action = VIEW_ZOOM_P;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ZOOM_M]))){
					action = VIEW_ZOOM_M;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ZOOM_W]))){
					action = VIEW_ZOOM_W;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ZOOM_A]))){
					action = VIEW_ZOOM_EXT;
				}
				nk_group_end(gui->ctx);
			}
			
			/* pan tools*/
			nk_layout_row_push(gui->ctx, 4*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "pan", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_UP]))){
					action = VIEW_PAN_U;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_DOWN]))){
					action = VIEW_PAN_D;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LEFT]))){
					action = VIEW_PAN_L;
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_RIGTH]))){
					action = VIEW_PAN_R;
				}
				nk_group_end(gui->ctx);
			}
			
			/* config tools*/
			nk_layout_row_push(gui->ctx, 3*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "config", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_INFO]))){
					show_info = 1;
					//printf("Info\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TOOL]))){
					printf("Tools\n");
				}
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_GEAR]))){
					printf("Config\n");
				}
				nk_group_end(gui->ctx);
			}
			
			/* config tools*/
			nk_layout_row_push(gui->ctx, 1*(ICON_SIZE + 4 + 4) + 13);
			if (nk_group_begin(gui->ctx, "help", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_static(gui->ctx, ICON_SIZE + 4, ICON_SIZE + 4, 10);
				if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_HELP]))){
					show_app_about = 1;
					//printf("HELP\n");
				}
				nk_group_end(gui->ctx);
			}
			
			nk_layout_row_end(gui->ctx);
			/*------------ end first line --------------*/
			
			/* second line */
			nk_layout_row_begin(gui->ctx, NK_STATIC, ICON_SIZE + 4, ICON_SIZE + 4);
			
			static char text[64];
			int text_len;
			nk_layout_row_push(gui->ctx, 900);
			if (nk_group_begin(gui->ctx, "Prop", NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_begin(gui->ctx, NK_STATIC, 20, 20);
				
				/*layer*/
				nk_layout_row_push(gui->ctx, 60);
				nk_label(gui->ctx, "Layer: ", NK_TEXT_RIGHT);
				nk_layout_row_push(gui->ctx, 200);
				if (nk_combo_begin_label(gui->ctx, drawing->layers[layer_idx].name, nk_vec2(300,300))){
					float wid[] = {175, 20, 20, 20, 20};
					nk_layout_row(gui->ctx, NK_STATIC, 20, 5, wid);
					int num_layers = drawing->num_layers;
					for (i = 0; i < num_layers; i++){
						//strcpy(layer_nam[i], drawing->layers[i].name);
						if (nk_button_label(gui->ctx, drawing->layers[i].name)){
							layer_idx = i;
							action = LAYER_CHANGE;
							nk_combo_close(gui->ctx);
						}
						
						struct nk_color b_color = {
							.r = dxf_colors[drawing->layers[i].color].r,
							.g = dxf_colors[drawing->layers[i].color].g,
							.b = dxf_colors[drawing->layers[i].color].b,
							.a = dxf_colors[drawing->layers[i].color].a
						};
						if(nk_button_color(gui->ctx, b_color)){
							
						}
						
						if (drawing->layers[i].off){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_NO_EYE]))){
								drawing->layers[i].off = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_EYE]))){
								drawing->layers[i].off = 1;
							}
						}
						if (drawing->layers[i].frozen){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_FREEZE]))){
								drawing->layers[i].frozen = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_SUN]))){
								drawing->layers[i].frozen= 1;
							}
						}
						if (drawing->layers[i].lock){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LOCK]))){
								drawing->layers[i].lock = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_UNLOCK]))){
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
				/* fill the tiny bitmap with selected color*/
				bmp_fill(color_img, dxf_colors[c_idx]);
				
				/* print the name (number) of color */
				if (color_idx <256){
					text_len = snprintf(text, 63, "%d", color_idx);
				}
				else{
					text_len = snprintf(text, 63, "%s", "ByL");
				}
				nk_layout_row_push(gui->ctx, 60);
				nk_label(gui->ctx, "Color: ", NK_TEXT_RIGHT);
				nk_layout_row_push(gui->ctx, 70);
				if (nk_combo_begin_image_label(gui->ctx, text, i_color, nk_vec2(215,320))){
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
							action = COLOR_CHANGE;
							nk_combo_close(gui->ctx);
						}
					}
					nk_layout_row_dynamic(gui->ctx, 20, 1);
						if (nk_button_label(gui->ctx, "By Layer")){
							color_idx = 256;
							action = COLOR_CHANGE;
							nk_combo_close(gui->ctx);
						}
					nk_combo_end(gui->ctx);
				}
				
				/*line type*/
				nk_layout_row_push(gui->ctx, 90);
				nk_label(gui->ctx, "Line type: ", NK_TEXT_RIGHT);
				nk_layout_row_push(gui->ctx, 200);
				if (nk_combo_begin_label(gui->ctx, drawing->ltypes[ltypes_idx].name, nk_vec2(300,200))){
					nk_layout_row_dynamic(gui->ctx, 20, 2);
					int num_ltypes = drawing->num_ltypes;
					
					for (i = 0; i < num_ltypes; i++){
						
						if (nk_button_label(gui->ctx, drawing->ltypes[i].name)){
							ltypes_idx = i;
							action = LTYPE_CHANGE;
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
				thick = nk_propertyd(gui->ctx, "Thickness", 0.0d, thick_prev, 20.0d, 0.1d, 0.1d);
				if (thick_prev != thick){
					action = THICK_CHANGE;
					//printf ("thick change\n");
				}
				thick_prev = thick;
				
				nk_layout_row_end(gui->ctx);
				
				nk_group_end(gui->ctx);
			}
			
			nk_layout_row_end(gui->ctx);
			/*------------ end second line --------------*/
			
			if (show_app_about){
				/* about popup */
				static struct nk_rect s = {20, 100, 400, 150};
				if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "About", NK_WINDOW_CLOSABLE, s)){
					nk_layout_row_dynamic(gui->ctx, 50, 2);
					nk_label(gui->ctx, "CadZinho", NK_TEXT_RIGHT);
					nk_image(gui->ctx, nk_image_ptr(i_cz48));
					//nk_layout_row_dynamic(gui->ctx, 165, 1);
					//nk_image(gui->ctx, i_cz);
					nk_layout_row_begin(gui->ctx, NK_DYNAMIC, 20, 2);
					nk_layout_row_push(gui->ctx, 0.7f);
					nk_label(gui->ctx, "By Ezequiel Rabelo de Aguiar", NK_TEXT_RIGHT);
					nk_layout_row_push(gui->ctx, 0.3f);
					nk_image(gui->ctx, nk_image_ptr(svg_bmp[SVG_BRAZIL]));
					nk_layout_row_end(gui->ctx);
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "CadZinho is licensed under the MIT License.",  NK_TEXT_LEFT);
					nk_popup_end(gui->ctx);
				} else show_app_about = nk_false;
			}
			
		}
		nk_end(gui->ctx);
		
		
		
		
		
		
		
		/*
		if (nk_begin(gui->ctx, "Icons", nk_rect(500, 50, 200, 500),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE)){
			nk_layout_row_static(gui->ctx, 28, 28, 6);
			for (i = 0; i <  SVG_MEDIA_SIZE; i++){
				nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[i]));
			}
			
		}
		nk_end(gui->ctx);*/
		
		next_win_y += next_win_h + 3;
		next_win_w = 210;
		next_win_h = 500;
		
		if (nk_begin(gui->ctx, "Toolbox", nk_rect(next_win_x, next_win_y, next_win_w, next_win_h),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
		NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE|NK_WINDOW_NO_SCROLLBAR)){
			
			//if (nk_tree_push(gui->ctx, NK_TREE_TAB, "Place", NK_MAXIMIZED)) {
			nk_layout_row_dynamic(gui->ctx, 20, 1);
			nk_label(gui->ctx, "Modify:", NK_TEXT_LEFT);
			
			nk_layout_row_static(gui->ctx, 28, 28, 6);
			
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_CURSOR]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","SELECT");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TEXT_E]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_MOVE]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","MOVE");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_DUPLI]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","DUPLI");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_SCALE]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","SCALE");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ROT]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_MIRROR]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_BLOCK]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","NEW_BLK");
				/*dxf_new_block(drawing, "teste", "0", sel_list, &list_do);
				dxf_ent_print2(drawing->blks);
				dxf_ent_print2(drawing->blks_rec);*/
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_EXPLODE]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_EDIT]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TAG]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TAG_E]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_FIND]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_RULER]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_STYLE]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TRASH]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","DELETE");
			}
				
			
			nk_layout_row_dynamic(gui->ctx, 20, 1);
			nk_label(gui->ctx, "Place:", NK_TEXT_LEFT);
				
			nk_layout_row_static(gui->ctx, 28, 28, 6);
			
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LINE]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","LINE");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_PLINE]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","POLYLINE");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_RECT]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","RECT");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_TEXT]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","TEXT");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_CIRCLE]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","CIRCLE");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ELIPSE]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_ARC]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_SPLINE]))){
				
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_BOOK]))){
				recv_comm_flag = 1;
				snprintf(recv_comm, 64, "%s","INSERT");
			}
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_IMAGE]))){
				
			}
			
			nk_layout_row_dynamic(gui->ctx, 20, 1); /*space*/
			
			struct nk_vec2 wid_pos = nk_widget_position(gui->ctx);
			struct nk_vec2 win_pos = nk_window_get_position(gui->ctx);
			struct nk_rect win_cont = nk_window_get_content_region(gui->ctx);
		
			nk_layout_row_dynamic(gui->ctx, win_cont.h - (wid_pos.y - win_pos.y), 1);
			if (nk_group_begin(gui->ctx, "especific", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
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
				case NEW_BLK:
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Create a new block from selection", NK_TEXT_LEFT);
					//nk_label(gui->ctx, "Enter base point", NK_TEXT_LEFT);
					nk_label(gui->ctx, "Block Name:", NK_TEXT_LEFT);
					nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, txt, DXF_MAX_CHARS, nk_filter_default);
					nk_checkbox_label(gui->ctx, "Text to Tags", &text2tag);
					nk_label(gui->ctx, "Tag mark:", NK_TEXT_LEFT);
					nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, tag_mark, DXF_MAX_CHARS, nk_filter_default);
					break;
				case INSERT:
					
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Place a block", NK_TEXT_LEFT);
					
					nk_layout_row_dynamic(gui->ctx, 20, 1);
					nk_label(gui->ctx, "Block Name:", NK_TEXT_LEFT);
					nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, blk_name, DXF_MAX_CHARS, nk_filter_default);
					
					if (nk_button_label(gui->ctx, "Explore")) show_blk_pp = 1;
					if (show_blk_pp){
						/* select block popup */
						static struct nk_rect s = {20, 10, 420, 330};
						if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Select Block", NK_WINDOW_CLOSABLE, s)){
							
							list_node *blk_g; /*graphic object of current block */
							dxf_node *blk, *blk_nm; /* current block and its name attribute */
							int blk_ei; /*extents flag of current block */
							/* extents and zoom parameters */
							double blk_x0, blk_y0, blk_x1, blk_y1, z, z_x, z_y, o_x, o_y;
							
							nk_layout_row_dynamic(gui->ctx, 225, 2);
							i = 0;
							int blk_idx = -1;
							if (nk_group_begin(gui->ctx, "Block_names", NK_WINDOW_BORDER)) {
								nk_layout_row_dynamic(gui->ctx, 20, 1);
								while (blk = dxf_find_obj_i(drawing->blks, "BLOCK", i)){
									
									/* get name of current block */
									blk_nm = dxf_find_attr2(blk, 2);
									if (blk_nm){
										if((blk_nm->value.s_data[0] != '*') || (show_hidden_blks)){
											if (nk_button_label(gui->ctx, blk_nm->value.s_data)){
												blk_idx = i;
												strncpy(blk_name, blk_nm->value.s_data, DXF_MAX_CHARS-1);
											}
										}
									}
									
									i++;
								}
								nk_group_end(gui->ctx);
							}
							if (blk_idx >= 0){
								blk = dxf_find_obj_i(drawing->blks, "BLOCK", blk_idx);
								blk_idx = -1;
								
								blk_ei = 0;
								/* get graphics of current block*/
								blk_g = dxf_graph_parse(drawing, blk, 0, FRAME_LIFE);
								/* get extents parameters of current block*/
								graph_list_ext(blk_g, &blk_ei, &blk_x0, &blk_y0, &blk_x1, &blk_y1);
								
								/* calcule the zoom and offset for preview */
								z_x = fabs(blk_x1 - blk_x0)/blk_prvw_big->width;
								z_y = fabs(blk_y1 - blk_y0)/blk_prvw_big->height;
								z = (z_x > z_y) ? z_x : z_y;
								if (z <= 0) z =1;
								else z = 1/(1.1 * z);
								o_x = blk_x0 - (fabs((blk_x1 - blk_x0)*z - blk_prvw_big->width)/2)/z;
								o_y = blk_y0 - (fabs((blk_y1 - blk_y0)*z - blk_prvw_big->height)/2)/z;
								
								snprintf(txt, DXF_MAX_CHARS, "(%0.2f,%0.2f)-(%0.2f,%0.2f)", blk_x0, blk_y0, blk_x1, blk_y1);
								
								/* draw graphics in current preview bitmap */
								bmp_fill(blk_prvw_big, blk_prvw_big->bkg); /* clear bitmap */
								graph_list_draw(blk_g, blk_prvw_big, o_x, o_y, z);
								
							}
							if (nk_group_begin(gui->ctx, "Block_prev", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
								/* preview img */
								nk_layout_row_dynamic(gui->ctx, 175, 1);
								nk_button_image(gui->ctx,  nk_image_ptr(blk_prvw_big));
								
								/* current block name */
								nk_layout_row_dynamic(gui->ctx, 20, 1);
								nk_label(gui->ctx, blk_name, NK_TEXT_CENTERED);
								
								/* current block corners */
								nk_layout_row_dynamic(gui->ctx, 15, 1);
								nk_style_push_font(gui->ctx, &font_tiny_nk); /* change font to tiny*/
								nk_label(gui->ctx, txt, NK_TEXT_CENTERED);
								nk_style_pop_font(gui->ctx); /* back to default font*/
								
								nk_group_end(gui->ctx);
							}
							
							nk_layout_row_dynamic(gui->ctx, 20, 1);
							
							nk_checkbox_label(gui->ctx, "Hidden", &show_hidden_blks);
							
							if (nk_button_label(gui->ctx, "Select")){
								show_blk_pp = 0;
								nk_popup_close(gui->ctx);
							}
							/*
							if (nk_button_label(gui->ctx, "test")){
								//snprintf(txt, DXF_MAX_CHARS, "text");
								blk = dxf_find_obj_descr2(drawing->blks, "BLOCK", blk_name);
								if(blk){
									/* create a new attdef text 
									//dxf_node * dxf_new_attdef (double x0, double y0, double z0, double h,
									//char *txt, char *tag, double thick, int color, char *layer, char *ltype, int paper){

									dxf_node * new_el = dxf_new_attdef (
										0.0, 0.0, 0.0, 1.0, /* pt1, height 
										"test", "tag1",(double) thick, /* text, thickness 
										color_idx, drawing->layers[layer_idx].name, /* color, layer 
										drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space 
									ent_handle(drawing, new_el);
									dxf_block_append(blk, new_el);
									snprintf(txt, DXF_MAX_CHARS, "attdef");
								}
							}*/
							
							nk_popup_end(gui->ctx);
						}
						else show_blk_pp = 0;
					}
					break;
			}
			nk_group_end(gui->ctx);
			}
		}
		nk_end(gui->ctx);
		
		if (show_lay_mng){
			next_win_x += next_win_w + 3;
			//next_win_y += next_win_h + 3;
			next_win_w = 520;
			next_win_h = 400;
			
			//if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Info", NK_WINDOW_CLOSABLE, nk_rect(310, 50, 200, 300))){
			if (nk_begin(gui->ctx, "Layer Manager", nk_rect(next_win_x, next_win_y, next_win_w, next_win_h),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
			NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE)){
				
				static int sel_lay = -1;
				
				nk_layout_row_dynamic(gui->ctx, 200, 1);
				if (nk_group_begin(gui->ctx, "Lay_view", NK_WINDOW_BORDER)) {
				
					nk_layout_row(gui->ctx, NK_STATIC, 20, 6, (float[]){175, 20, 20, 20, 20, 175});
					int num_layers = drawing->num_layers;
					
					for (i = 0; i < num_layers; i++){
						//strcpy(layer_nam[i], drawing->layers[i].name);
						//nk_selectable_label(gui->ctx, "Relative", NK_TEXT_CENTERED, &entry_relative);
						
						if (sel_lay == i){
							if (nk_button_label_styled(gui->ctx, &b_icon_sel_style, drawing->layers[i].name)){
								sel_lay = -1;
							}
						}
						else {
							if (nk_button_label_styled(gui->ctx,&b_icon_unsel_style, drawing->layers[i].name)){
								sel_lay = i;
							}
						}
						
						struct nk_color b_color = {
							.r = dxf_colors[drawing->layers[i].color].r,
							.g = dxf_colors[drawing->layers[i].color].g,
							.b = dxf_colors[drawing->layers[i].color].b,
							.a = dxf_colors[drawing->layers[i].color].a
						};
						if(nk_button_color(gui->ctx, b_color)){
							show_color_pick = 1;
							sel_lay = i;
						}
						
						if (drawing->layers[i].off){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_NO_EYE]))){
								drawing->layers[i].off = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_EYE]))){
								drawing->layers[i].off = 1;
							}
						}
						if (drawing->layers[i].frozen){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_FREEZE]))){
								drawing->layers[i].frozen = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_SUN]))){
								drawing->layers[i].frozen= 1;
							}
						}
						if (drawing->layers[i].lock){
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_LOCK]))){
								drawing->layers[i].lock = 0;
							}
						}
						else{
							if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_UNLOCK]))){
								drawing->layers[i].lock = 1;
							}
						}
						if (nk_button_label(gui->ctx, drawing->layers[i].ltype)){
							
						}
					}
					nk_group_end(gui->ctx);
				}

				
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				nk_label(gui->ctx, "Layer Name:",  NK_TEXT_LEFT);
				nk_edit_string_zero_terminated(gui->ctx, NK_EDIT_SIMPLE, txt, DXF_MAX_CHARS, nk_filter_default);
				if (nk_button_label(gui->ctx, "Create")){
					if (!dxf_new_layer (drawing, txt, color_idx, drawing->ltypes[ltypes_idx].name)){
						snprintf(log_msg, 63, "Error: Layer already exists");
					}
				}
				if ((show_color_pick) && (sel_lay >= 0)){
					if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Layer Color", NK_WINDOW_CLOSABLE, nk_rect(220, 10, 220, 300))){
						
						int j;
						nk_layout_row_static(gui->ctx, 15, 15, 10);
						struct nk_color b_color;
						for (j = 0; j < 256; j++){
							
							b_color.r = dxf_colors[j].r;
							b_color.g = dxf_colors[j].g;
							b_color.b = dxf_colors[j].b;
							b_color.a = dxf_colors[j].a;
							
							if(nk_button_color(gui->ctx, b_color)){
								drawing->layers[sel_lay].color = j;
								dxf_attr_change(drawing->layers[sel_lay].obj, 62, &j);
								nk_popup_close(gui->ctx);
								show_color_pick = 0;
							}
						}
						nk_popup_end(gui->ctx);
					} else show_color_pick = 0;
				}
					
			} else show_lay_mng = nk_false;
			nk_end(gui->ctx);
		}
		
		if (show_info){
			next_win_x += next_win_w + 3;
			//next_win_y += next_win_h + 3;
			next_win_w = 200;
			next_win_h = 300;
			
			//if (nk_popup_begin(gui->ctx, NK_POPUP_STATIC, "Info", NK_WINDOW_CLOSABLE, nk_rect(310, 50, 200, 300))){
			if (nk_begin(gui->ctx, "Info", nk_rect(next_win_x, next_win_y, next_win_w, next_win_h),
			NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
			NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE)){
				nk_layout_row_dynamic(gui->ctx, 20, 1);
				nk_label(gui->ctx, "BLK:", NK_TEXT_LEFT);
				i = 1;
				nk_dxf_ent_info (gui->ctx, drawing->blks, i);
				
				nk_label(gui->ctx, "ENTS:", NK_TEXT_LEFT);
				if (sel_list != NULL){				
					list_node *current = sel_list->next;
					// starts the content sweep 
					i = 2;
					while (current != NULL){
						if (current->data){
							if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									
								// -------------------------------------------
								nk_dxf_ent_info (gui->ctx, (dxf_node *)current->data, i);
								i++;
								
								//---------------------------------------
							}
						}
						current = current->next;
					}
				}
				//nk_popup_end(gui->ctx);
			} else show_info = nk_false;
			nk_end(gui->ctx);
		}
		
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
				nk_progress(gui->ctx, (nk_size *)&progress, 100, NK_FIXED);
				//nk_popup_end(gui->ctx);
				nk_end(gui->ctx);
			}
			
			if (progr_end){
				progr_win = 0;
				progr_end = 0;
				nk_window_close(gui->ctx, "Progress");
			}
		}
		
		/* interface to the user visualize and enter coordinates distances*/
		if (nk_begin(gui->ctx, "POS", nk_rect(2, win_h - 80, win_w - 4, 78),
		NK_WINDOW_BORDER))
		{
			nk_flags res;
			/* flags to verify which coordinate is predominant */
			int flag_x = fabs(step_x[step] - step_x[step - 1]) >= fabs(step_y[step] - step_y[step - 1]);
			int flag_y = fabs(step_x[step] - step_x[step - 1]) < fabs(step_y[step] - step_y[step - 1]);
			
			nk_layout_row_begin(gui->ctx, NK_STATIC, 32, 5);
			nk_layout_row_push(gui->ctx, 380);
			if (nk_group_begin(gui->ctx, "coord", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
				nk_layout_row_begin(gui->ctx, NK_STATIC, 20, 10);
				nk_layout_row_push(gui->ctx, 20);
				/* X distance */
				/* hilite coordinate, if coord is predominant during a drawing operation*/
				if ((en_distance) && (step > 0) && (step < 10) && (flag_x)){
					nk_label_colored(gui->ctx, "X=", NK_TEXT_RIGHT, nk_rgb(255,255,0));
				}
				else {
					nk_label(gui->ctx, "X=", NK_TEXT_RIGHT);
				}
				/* verify if the user initiate a number entry during a drawing operation */
				if (((en_distance)||(!entry_relative)) && (user_number) && (step >= 0) && (step < 10) &&
				(!user_flag_x) && (flag_x)){
					user_number = 0; /* clear user flag */
					user_str_x[0] = 0; /* clear edit string */
					/* set focus to edit */
					nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
				}
				
				nk_layout_row_push(gui->ctx, 120);
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
					if ((!entry_relative) && (step >= 0) && (step < 10)){
						snprintf(user_str_x, 63, "%f", step_x[step]);
					}
					else if ((en_distance) && (step > 0) && (step < 10)){
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
				
				nk_layout_row_push(gui->ctx, 20);
				/* Y distance */
				/* hilite coordinate, if coord is predominant during a drawing operation*/
				if ((en_distance) && (step > 0) && (step < 10) && (flag_y)){
					nk_label_colored(gui->ctx, "Y=", NK_TEXT_RIGHT, nk_rgb(255,255,0));
				}
				else {
					nk_label(gui->ctx, "Y=", NK_TEXT_RIGHT);
				}
				/* verify if the user initiate a number entry during a drawing operation */
				if (((en_distance)||(!entry_relative)) && (user_number) && (step >= 0) && (step < 10) &&
				(!user_flag_y) && (flag_y)){
					user_number = 0; /* clear user flag */
					user_str_y[0] = 0; /* clear edit string */
					/* set focus to edit */
					nk_edit_focus(gui->ctx, NK_EDIT_SIMPLE|NK_EDIT_SIG_ENTER|NK_EDIT_SELECTABLE|NK_EDIT_AUTO_SELECT);
				}
				
				nk_layout_row_push(gui->ctx, 120);
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
					if ((!entry_relative) && (step >= 0) && (step < 10)){
						snprintf(user_str_y, 63, "%f", step_y[step]);
					}
					else if ((en_distance) && (step > 0) && (step < 10)){
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
				
				/* select if entry mode is in absolute coordinates or relative distance*/
				nk_layout_row_push(gui->ctx, 70);
				if (entry_relative){
					nk_selectable_label(gui->ctx, "Relative", NK_TEXT_CENTERED, &entry_relative);
					flag_x = 1;
				}
				else nk_selectable_label(gui->ctx, "Absolute", NK_TEXT_CENTERED, &entry_relative);
				nk_group_end(gui->ctx);
			}
			
			
			
			/*----------- attractors --------------*/
			//nk_layout_row_push(gui->ctx, 160);
			//nk_label(gui->ctx, "Attractors ->", NK_TEXT_RIGHT);
			
			/* Toggle on/off attractors*/
			nk_layout_row_push(gui->ctx, 30);
			/*if (en_attr){
				nk_selectable_label(gui->ctx, "On", NK_TEXT_CENTERED, &en_attr);
			}
			else nk_selectable_label(gui->ctx, "Off", NK_TEXT_CENTERED, &en_attr);*/
			if (en_attr){
				if (nk_button_image_styled(gui->ctx, &b_icon_sel_style, nk_image_ptr(svg_bmp[SVG_MAGNET]))){
					en_attr = 0;
				}
			}else {
				if (nk_button_image_styled(gui->ctx, &b_icon_unsel_style, nk_image_ptr(svg_bmp[SVG_MAGNET]))){
					en_attr = 1;
				}
			}
			nk_layout_row_push(gui->ctx, 16*(22 + 3) + 13);
			if (nk_group_begin(gui->ctx, "attractors", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)) {
				/* Buttons to select attractor mode*/
				int selected, i, attr = 1;
				nk_layout_row_static(gui->ctx, 22, 22, 15);
				for (i = 0; i < 15; i++){
					selected = (curr_attr_t & attr);
					/* uses styles "sel" or "unsel", deppending each status*/
					//nk_layout_row_push(gui->ctx, 22);
					if (selected){
						if (nk_button_image_styled(gui->ctx, &b_icon_sel_style, nk_image_ptr(attr_vec[i]))){
							curr_attr_t &= ~attr; /* clear bit of current type*/
						}
					}else {
						if (nk_button_image_styled(gui->ctx, &b_icon_unsel_style, nk_image_ptr(attr_vec[i]))){
							curr_attr_t |= attr; /* set bit of current type*/
						}
					}
					attr <<= 1; /* next attractor type (bit coded)*/
				}
				/*-------------------------------*/
				nk_group_end(gui->ctx);
			}
			
			nk_layout_row_push(gui->ctx, ICON_SIZE + 4);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_PREV]))){
				
			}
			nk_layout_row_push(gui->ctx, ICON_SIZE + 4);
			if (nk_button_image_styled(gui->ctx, &b_icon_style, nk_image_ptr(svg_bmp[SVG_NEXT]))){
				
			}
			nk_layout_row_end(gui->ctx);
			
			nk_layout_row_begin(gui->ctx, NK_STATIC, 20, 20);
			
			int text_len;
			char text[64];
			
			nk_style_push_font(gui->ctx, &font_tiny_nk); /* change font to tiny*/
			
			/* view coordinates of mouse in drawing units */
			nk_layout_row_push(gui->ctx, 280);
			text_len = snprintf(text, 63, "(%f,  %f)", pos_x, pos_y);
			nk_label(gui->ctx, text, NK_TEXT_CENTERED);
			
			nk_layout_row_push(gui->ctx, 280);
			nk_label(gui->ctx, log_msg, NK_TEXT_LEFT);
			
			nk_style_pop_font(gui->ctx); /* return to the default font*/
			
			/*-------------------------------*/
			nk_layout_row_end(gui->ctx);
		}
		nk_end(gui->ctx);
		
		/* */
		
		
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
				
				//dxf_ent_print2(drawing->blks);
				
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
			zoom_ext2(drawing, 0, main_h - win_h, win_w, win_h, &zoom, &ofs_x, &ofs_y);
			draw = 1;
		}
		else if(action == VIEW_ZOOM_P){
			action = NONE;
			
			prev_zoom = zoom;
			zoom = zoom + 0.2 * zoom;
			ofs_x += (win_w/2)*(1/prev_zoom - 1/zoom);
			ofs_y += (win_h/2)*(1/prev_zoom - 1/zoom);
			draw = 1;
		}
		else if(action == VIEW_ZOOM_M){
			action = NONE;
			prev_zoom = zoom;
			zoom = zoom - 0.2 * zoom;
			ofs_x += (win_w/2)*(1/prev_zoom - 1/zoom);
			ofs_y += (win_h/2)*(1/prev_zoom - 1/zoom);
			draw = 1;
		}
		else if(action == VIEW_PAN_U){
			action = NONE;
			ofs_y += (win_h*0.1)/zoom;
			draw = 1;
		}
		else if(action == VIEW_PAN_D){
			action = NONE;
			ofs_y -= (win_h*0.1)/zoom;
			draw = 1;
		}
		else if(action == VIEW_PAN_L){
			action = NONE;
			ofs_x -= (win_w*0.1)/zoom;
			draw = 1;
		}
		else if(action == VIEW_PAN_R){
			action = NONE;
			ofs_x += (win_w*0.1)/zoom;
			draw = 1;
		}
		else if(action == DELETE){
			action = NONE;
		
			if (sel_list != NULL){
				
				do_add_entry(&list_do, "DELETE");
				
				list_node *current = sel_list->next;
				
				// starts the content sweep 
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							
							if (do_add_item(list_do.current, (dxf_node *)current->data, NULL)) {
								//printf("add item = %d\n", current->data);
							}
							
							// -------------------------------------------
							//dxf_obj_detach((dxf_node *)current->data);
							dxf_obj_subst((dxf_node *)current->data, NULL);
							
							//---------------------------------------
						}
					}
					current = current->next;
				}
				list_clear(sel_list);
			}
			draw = 1;
		}
		else if(action == UNDO){
			action = NONE;
			list_clear(sel_list);
			char *text = list_do.current->text;
			
			if (do_undo(&list_do)){
				snprintf(log_msg, 63, "UNDO: %s", text);
				goto first_step;
			}
			else{
				snprintf(log_msg, 63, "No actions to undo");
			}
			draw = 1;
		}
		
		else if(action == REDO){
			action = NONE;
			list_clear(sel_list);
			if (do_redo(&list_do)){
				snprintf(log_msg, 63, "REDO: %s", list_do.current->text);
				goto first_step;
			}
			else{
				snprintf(log_msg, 63, "No actions to redo");
			}
			draw = 1;
		}
		
		else if(action == LAYER_CHANGE){
			action = NONE;
			if (sel_list != NULL){
				/* sweep the selection list */
				list_node *current = sel_list->next;
				dxf_node *new_ent = NULL;
				if (current != NULL){
					do_add_entry(&list_do, "CHANGE LAYER");
				}
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
							
							dxf_attr_change(new_ent, 8, drawing->layers[layer_idx].name);
							
							new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
							
							dxf_obj_subst((dxf_node *)current->data, new_ent);
							
							do_add_item(list_do.current, (dxf_node *)current->data, new_ent);

							current->data = new_ent;
						}
					}
					current = current->next;
				}
			}
			draw = 1;
		}
		
		else if(action == COLOR_CHANGE){
			action = NONE;
			if (sel_list != NULL){
				/* sweep the selection list */
				list_node *current = sel_list->next;
				dxf_node *new_ent = NULL;
				if (current != NULL){
					do_add_entry(&list_do, "CHANGE COLOR");
				}
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
							
							dxf_attr_change(new_ent, 62, &color_idx);
							new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
							
							dxf_obj_subst((dxf_node *)current->data, new_ent);
							
							do_add_item(list_do.current, (dxf_node *)current->data, new_ent);

							current->data = new_ent;
						}
					}
					current = current->next;
				}
			}
			draw = 1;
		}
		else if(action == LTYPE_CHANGE){
			action = NONE;
			if (sel_list != NULL){
				/* sweep the selection list */
				list_node *current = sel_list->next;
				dxf_node *new_ent = NULL;
				if (current != NULL){
					do_add_entry(&list_do, "CHANGE LINE TYPE");
				}
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
							
							dxf_attr_change(new_ent, 6, drawing->ltypes[ltypes_idx].name);
							new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
							
							dxf_obj_subst((dxf_node *)current->data, new_ent);
							
							do_add_item(list_do.current, (dxf_node *)current->data, new_ent);

							current->data = new_ent;
						}
					}
					current = current->next;
				}
			}
			draw = 1;
		}
		else if(action == THICK_CHANGE){
			action = NONE;
			if (sel_list != NULL){
				/* sweep the selection list */
				list_node *current = sel_list->next;
				dxf_node *new_ent = NULL;
				if (current != NULL){
					do_add_entry(&list_do, "CHANGE THICKNESS");
				}
				while (current != NULL){
					if (current->data){
						if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
							new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
							
							dxf_attr_change(new_ent, 39, &thick);
							new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
							
							dxf_obj_subst((dxf_node *)current->data, new_ent);
							
							do_add_item(list_do.current, (dxf_node *)current->data, new_ent);

							current->data = new_ent;
						}
					}
					current = current->next;
				}
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
			else if (strcmp(recv_comm, "NEW_BLK") == 0){
				modal = NEW_BLK;
				step = 0;
			}
			else if (strcmp(recv_comm, "INSERT") == 0){
				modal = INSERT;
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
				near_attr = ATRC_NONE;
				
				if ((modal != SELECT)&& (step >= 1)){
					/* update current position by the attractor of near element */
					if (near_attr = dxf_ent_attract(drawing, near_el, curr_attr_t,
					step_x[step], step_y[step], step_x[step-1], step_y[step-1],
					(double) 20/zoom, &near_x , &near_y)){
						step_x[step] = near_x;
						step_y[step] = near_y;
					}
				}
				else if (modal != SELECT){
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
			if ((!entry_relative) && (step >= 0) && (step < 10)){
				/* check the user entry */
				if (user_flag_x){
					step_x[step] = user_x;
				}
				if (user_flag_y){
					step_y[step] = user_y;
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
				//free(phanton->data);
				//free(phanton);
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
						//dxf_ent_print2(element);
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
				//double ret_x, ret_y;
				
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
					goto next_step;
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
					
					do_add_entry(&list_do, "LINE");
					do_add_item(list_do.current, NULL, new_el);
					
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
					goto next_step;
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
						
						do_add_entry(&list_do, "POLYLINE");
						do_add_item(list_do.current, NULL, new_el);
						
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
					goto next_step;
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
					
					do_add_entry(&list_do, "CIRCLE");
					do_add_item(list_do.current, NULL, new_el);
					
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
					goto next_step;
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
					
					do_add_entry(&list_do, "RECT");
					do_add_item(list_do.current, NULL, new_el);
					
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
				goto next_step;
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
					
					do_add_entry(&list_do, "TEXT");
					do_add_item(list_do.current, NULL, new_el);
					
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
		if (modal == INSERT){
			if (step == 0){
				if (leftMouseButtonClick){
					/* verify if block exist */					
					if (dxf_find_obj_descr2(drawing->blks, "BLOCK", blk_name)){
						draw_tmp = 1;
						//dxf_new_insert (char *name, double x0, double y0, double z0,int color, char *layer, char *ltype, int paper);
						new_el = dxf_new_insert (blk_name,
							step_x[step], step_y[step], 0.0, /* pt1 */
							color_idx, drawing->layers[layer_idx].name, /* color, layer */
							drawing->ltypes[ltypes_idx].name, 0); /* line type, paper space */
						element = new_el;
						step = 1;
						en_distance = 1;
						goto next_step;
					}
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					dxf_attr_change_i(new_el, 10, &step_x[step], -1);
					dxf_attr_change_i(new_el, 20, &step_y[step], -1);
					
					drawing_ent_append(drawing, new_el);
					
					
					/*=========================*/
					dxf_node *blk = dxf_find_obj_descr2(drawing->blks, "BLOCK", blk_name);
					dxf_node *attdef, *attrib;
					i = 0;
					while (attdef = dxf_find_obj_i(blk, "ATTDEF", i)){
						attrib = dxf_attrib_cpy(attdef, step_x[step], step_y[step], 0.0);
						ent_handle(drawing, attrib);
						dxf_insert_append(drawing, new_el, attrib);
						
						i++;
					}
					
					/*===================*/
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 0);
					do_add_entry(&list_do, "INSERT");
					do_add_item(list_do.current, NULL, new_el);
					
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
					dxf_attr_change(new_el, 6, drawing->ltypes[ltypes_idx].name);
					dxf_attr_change(new_el, 8, drawing->layers[layer_idx].name);
					dxf_attr_change(new_el, 62, &color_idx);					
					new_el->obj.graphics = dxf_graph_parse(drawing, new_el, 0 , 1);
				}
			}
		}
		if (modal == NEW_BLK){
			if (step == 0){
				if (leftMouseButtonClick){
					/* verify if text is valid */
					if (strlen(txt) > 0){
						if(!text2tag) dxf_new_block(drawing, txt, "0", sel_list, &list_do);
						
						else dxf_new_block2(drawing, txt, tag_mark, "0", sel_list, &list_do);
					}
				}
				else if (rightMouseButtonClick){
					goto default_modal;
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
					goto next_step;
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
						if (current != NULL){
							do_add_entry(&list_do, "MOVE");
						}
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
									dxf_edit_move(new_ent, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
									//drawing_ent_append(drawing, new_ent);
									
									dxf_obj_subst((dxf_node *)current->data, new_ent);
									
									do_add_item(list_do.current, (dxf_node *)current->data, new_ent);
									
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
					graph_list_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
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
					goto next_step;
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
						if (current != NULL){
							do_add_entry(&list_do, "DUPLI");
						}
						
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
									dxf_edit_move(new_ent, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
									drawing_ent_append(drawing, new_ent);
									
									do_add_item(list_do.current, NULL, new_ent);
									
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
					graph_list_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
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
					graph_list_modify(phanton, step_x[step]*(1 - scale), step_y[step]*(1 - scale), scale, scale, 0.0);
					element = NULL;
					draw_phanton = 1;
					en_distance = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
					step = 1;
					step_x[step + 1] = step_x[step];
					step_y[step + 1] = step_y[step];
					goto next_step;
				}
				else if (rightMouseButtonClick){
					goto default_modal;
				}
			}
			else{
				if (leftMouseButtonClick){
					if (sel_list != NULL){
						list_node *current = sel_list->next;
						dxf_node *new_ent = NULL;
						if (current != NULL){
							do_add_entry(&list_do, "SCALE");
						}
						while (current != NULL){
							if (current->data){
								if (((dxf_node *)current->data)->type == DXF_ENT){ // DXF entity 
									new_ent = dxf_ent_copy((dxf_node *)current->data, 0);
									
									dxf_edit_scale(new_ent, scale, scale, scale);
									dxf_edit_move(new_ent, step_x[step - 1]*(1 - scale), step_y[step - 1]*(1 - scale), 0.0);
									dxf_edit_move(new_ent, step_x[step] - step_x[step - 1], step_y[step] - step_y[step - 1], 0.0);
									
									new_ent->obj.graphics = dxf_graph_parse(drawing, new_ent, 0 , 0);
									//drawing_ent_append(drawing, new_ent);
									
									dxf_obj_subst((dxf_node *)current->data, new_ent);
									
									do_add_item(list_do.current, (dxf_node *)current->data, new_ent);
									
									current->data = new_ent;
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
					graph_list_modify(phanton, step_x[step] - step_x[step + 1], step_y[step] - step_y[step + 1], 1.0, 1.0, 0.0);
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
				//free(phanton->data);
				//free(phanton);
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
			/*get current window size */
			SDL_GetWindowSize(window, &win_w, &win_h);
			if (win_w > main_w){ /* if window exceedes main image */
				/* fit windo to main image size*/
				win_w = main_w;
				SDL_SetWindowSize(window, win_w, win_h);
			}
			if (win_h > main_h){/* if window exceedes main image */
				/* fit windo to main image size*/
				win_h = main_h;
				SDL_SetWindowSize(window, win_w, win_h);
			}
			
			/* set image visible window*/
			img->clip_x = 0; img->clip_y = main_h - win_h;
			img->clip_w = win_w;
			img->clip_h = win_h;
		
			bmp_fill_clip(img, img->bkg); /* clear bitmap */
			dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom); /* redraw */
			
			/*===================== teste ===============*/
			graph_list_draw(tt_test, img, ofs_x, ofs_y, zoom);
			/*===================== teste ===============*/
			
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
				graph_list_draw_fix(element->obj.graphics, img, ofs_x, ofs_y, zoom, hilite);
			}
			if((draw_phanton)&&(phanton)){
				//dxf_list_draw(sel_list, img, ofs_x - (x1 - x0), ofs_y - (y1 - y0), zoom, hilite);
				graph_list_draw_fix(phanton, img, ofs_x, ofs_y, zoom, hilite);
			}
			dxf_list_draw(sel_list, img, ofs_x, ofs_y, zoom, hilite);
			
			
			/* set image visible window*/
			img->clip_x = 0; img->clip_y = 0;
			
			/*draw gui*/
			nk_sdl_render(gui, img);
			
			
			
			
			win_r.x = 0; win_r.y = 0;
			win_r.w = win_w; win_r.h = win_h;
			
			SDL_UpdateTexture(canvas, &win_r, img->buf, main_w * 4);
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, canvas, &win_r, NULL);
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
	SDL_free(base_path);
	SDL_DestroyTexture(canvas);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	list_mem_pool(FREE_LIST, 0);
	list_mem_pool(FREE_LIST, 1);
	list_mem_pool(FREE_LIST, ONE_TIME);
	dxf_mem_pool(FREE_DXF, 0);
	graph_mem_pool(FREE_ALL, 0);
	graph_mem_pool(FREE_ALL, 1);
	graph_mem_pool(FREE_ALL, 2);
	graph_mem_pool(FREE_ALL, PRG_LIFE);
	
	do_mem_pool(FREE_DO_ALL);
	
	bmp_free(img);
	bmp_free(color_img);
	bmp_free(blk_prvw_big);
	bmp_free(i_cz48);
	
	i_svg_free_bmp(svg_bmp);
	i_svg_free_curves(svg_curves);
	
	
	for (i = 0; i<drawing->num_fonts; i++){
		shx_font_free(drawing->text_fonts[i].shx_font);
	}
	nk_sdl_shutdown(gui);
	shx_font_free(font.shx_font);
	free(drawing);
	
		
	return 0;
}