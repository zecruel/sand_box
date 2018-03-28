
#ifndef _CZ_GUI_LIB
#define _CZ_GUI_LIB

#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "shape2.h"
#include "i_svg_media.h"

#include <SDL.h>

#define NK_INCLUDE_FIXED_TYPES
#include "nuklear.h"

#include "nanosvg.h"
#include "nanosvgrast.h"

#define FONT_SCALE 1.4
#define FIXED_MEM 128*1024

struct Gui_obj {
	struct nk_context *ctx;
	struct nk_user_font *font;
	void *buf; /*for fixed memory */
	void *last; /* to verify if needs to draw */
	
	dxf_drawing *drawing;
	
	/* background image dimension */
	unsigned int main_w;
	unsigned int main_h;
	
	/* Window dimension */
	unsigned int win_w;
	unsigned int win_h;
	
	/*gui pos variables */
	int next_win_x, next_win_y, next_win_w, next_win_h;
	
	double zoom, ofs_x, ofs_y;
	double prev_zoom;
	
	
	int color_idx, lw_idx;
	int layer_idx, ltypes_idx;
	
	bmp_color background;
	
	NSVGimage **svg_curves;
	bmp_img **svg_bmp;
	
	struct nk_style_button b_icon_style;
	
	/* style for toggle buttons (or select buttons) with image */
	struct nk_style_button b_icon_sel_style, b_icon_unsel_style;
	
	char log_msg[64];
	
};
typedef struct Gui_obj gui_obj;

struct font_obj{
	shape *shx_font;
	double scale;
};

enum theme {THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK, THEME_ZE};

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
	LW_CHANGE,
	EXIT
};

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
};

void set_style(struct nk_context *ctx, enum theme theme);

float nk_user_font_get_text_width(nk_handle handle, float height, const char *text, int len);

bmp_color nk_to_bmp_color(struct nk_color color);

int gui_check_draw(gui_obj *gui);

NK_API void nk_sdl_render(gui_obj *gui, bmp_img *img);

static void nk_sdl_clipbard_paste(nk_handle usr, struct nk_text_edit *edit);

static void nk_sdl_clipbard_copy(nk_handle usr, const char *text, int len);

NK_API gui_obj* nk_sdl_init(struct nk_user_font *font);

NK_API int nk_sdl_handle_event(gui_obj *gui, SDL_Window *win, SDL_Event *evt);

NK_API void nk_sdl_shutdown(gui_obj *gui);

#endif
