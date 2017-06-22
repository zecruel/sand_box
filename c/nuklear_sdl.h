/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 * authored from 2015-2017 by Micha Mettke
 */
 
 

 
/*
 * ==============================================================
 *
 *                              API
 *
 * ===============================================================
 */
#ifndef NK_SDL_H_
#define NK_SDL_H_

#include <SDL.h>
NK_API struct nk_context*   nk_sdl_init(SDL_Window *win, shape *shx_font);
NK_API int                  nk_sdl_handle_event(SDL_Event *evt);
NK_API void                 nk_sdl_render(bmp_img *img);
NK_API void                 nk_sdl_shutdown(void);

#endif
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
//#ifdef NK_SDL_IMPLEMENTATION

#define FONT_SCALE 1.4
static struct nk_sdl {
    SDL_Window *win;
    struct nk_context ctx;
    struct nk_user_font font;
} sdl;

struct Gui_obj {
	struct nk_context ctx;
	struct nk_user_font font;
	void *buf; /*for fixed memory */
	void *last_buf; /* to verify if needs to draw */
	int draw;
	int focus;
};

static float nk_user_font_get_text_width(nk_handle handle, float height, const char *text, int len){
	shape *font = (shape*)handle.ptr;
	if ((text!= NULL) && (font!=NULL)) {
	
		/* We must copy into a new buffer with exact length null-terminated
		as nuklear uses variable size buffers and shx_fonts routines doesn't
		accept a length, it infers length from null-termination */
		char txt_cpy[len+1];
		strncpy((char*)&txt_cpy, text, len);
		txt_cpy[len] = '\0';
		
		
		nk_rune str_uni[255];
		char str[255];
		int glyph_size, char_size, pos = 0;
		
		char *curr = 0, *curr_pos =0;
		
		curr = (char *)txt_cpy;
		curr_pos = str;
		pos = 0;
		
		while ((*curr != 0) && (pos < 254)){
		
			glyph_size = nk_utf_decode(curr, str_uni, 10);
			if (glyph_size){
				char_size = wctomb(curr_pos, (wchar_t)str_uni[0]);
				curr += glyph_size;
				pos += char_size;
				curr_pos += char_size;
			}
			else {
				curr = 0;
			}
		}
		
		if(pos<255){
			str[pos] = 0;
		}
		else{
			str[254] = 0;
		}
		
		graph_obj *curr_graph = shx_font_parse(font, 0, str);
		if (curr_graph){
			double txt_w;
			txt_w = FONT_SCALE*fabs(curr_graph->ext_max_x - curr_graph->ext_min_x);
			return (float) txt_w;
		}
	}
	return 0;
}

bmp_color nk_to_bmp_color(struct nk_color color){
	bmp_color ret_color = {.r = color.r, .g = color.g, .b = color.b, .a = color.a };
	return ret_color;
}

NK_API void nk_sdl_render(bmp_img *img){
	const struct nk_command *cmd;
	bmp_color color = {.r = 255, .g = 255, .b =255, .a = 255};
	
	
	static int one_time = 0;
	if (!one_time){
		//one_time =1;
	}
	
	if (img){
		/* initialize the image with a solid line pattern */
		img->patt_i = 0;
		img->pix_count = 0;
		img->patt_size = 1;
		img->pattern[0] = 1;

		nk_foreach(cmd, &sdl.ctx){
			switch (cmd->type) {
				case NK_COMMAND_NOP: break;
				
				case NK_COMMAND_SCISSOR: {
					const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
					img->clip_x = (unsigned int)s->x;
					img->clip_y = (unsigned int)s->y;
					img->clip_w = (unsigned int)s->w;
					img->clip_h = (unsigned int)s->h;
				} break;
				
				case NK_COMMAND_LINE: {
					const struct nk_command_line *l = (const struct nk_command_line *)cmd;
					color = nk_to_bmp_color(l->color);
					/*change the color */
					img->frg = color;
					/*change tickness */
					img->tick = (unsigned int) l->line_thickness;
					
					bmp_line(img, l->begin.x, l->begin.y, l->end.x,l->end.y);
				} break;
				
				case NK_COMMAND_RECT: {
					const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
					color = nk_to_bmp_color(r->color);
					/*change the color */
					img->frg = color;
					/*change tickness */
					img->tick = (unsigned int) r->line_thickness;

					bmp_line(img, r->x,r->y, r->x + r->w, r->y);
					bmp_line(img, r->x + r->w, r->y, r->x + r->w, r->y + r->h);
					bmp_line(img, r->x + r->w, r->y + r->h, r->x, r->y + r->h);
					bmp_line(img, r->x, r->y + r->h, r->x, r->y);
				} break;
				
				case NK_COMMAND_RECT_FILLED: {
					const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
					int vert_x[4] = {r->x, r->x + r->w, r->x + r->w, r->x};
					int vert_y[4] = {r->y, r->y, r->y + r->h, r->y + r->h};
					
					color = nk_to_bmp_color(r->color);
					/*change the color */
					img->frg = color;
					
					bmp_poly_fill(img, 4, vert_x, vert_y);
				} break;
				
				case NK_COMMAND_CIRCLE: {
					const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
					color = nk_to_bmp_color(c->color);
					/*change the color */
					img->frg = color;
					/*change tickness */
					img->tick = c->line_thickness;
					int xr = c->w/2;
					
					bmp_circle(img, c->x + xr, c->y + xr, xr);
				} break;
				
				case NK_COMMAND_CIRCLE_FILLED: {
					const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
					color = nk_to_bmp_color(c->color);
					/*change the color */
					img->frg = color;
					int xr = c->w/2;
					
					bmp_circle_fill(img, c->x + xr, c->y + xr, xr);
				} break;
				
				case NK_COMMAND_TRIANGLE: {
					const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
					color = nk_to_bmp_color(t->color);
					/*change the color */
					img->frg = color;
					/*change tickness */
					img->tick = t->line_thickness;
					bmp_line(img, t->a.x, t->a.y, t->b.x, t->b.y);
					bmp_line(img, t->b.x, t->b.y, t->c.x, t->c.y);
					bmp_line(img, t->c.x, t->c.y, t->a.x, t->a.y);
				} break;
				
				case NK_COMMAND_TRIANGLE_FILLED: {
					const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
					int vert_x[3] = {t->a.x, t->b.x, t->c.x};
					int vert_y[3] = {t->a.y, t->b.y, t->c.y};
					
					color = nk_to_bmp_color(t->color);
					/*change the color */
					img->frg = color;
					
					bmp_poly_fill(img, 3, vert_x, vert_y);
				} break;
				
				case NK_COMMAND_POLYGON: {
					const struct nk_command_polygon *p = (const struct nk_command_polygon*)cmd;
					color = nk_to_bmp_color(p->color);
					//int i;
					//float vertices[p->point_count * 2];
					//for (i = 0; i < p->point_count; i++) {
					// vertices[i*2] = p->points[i].x;
					// vertices[(i*2) + 1] = p->points[i].y;
					//}
					//al_draw_polyline((const float*)&vertices, (2 * sizeof(float)),
					//    (int)p->point_count, ALLEGRO_LINE_JOIN_ROUND, ALLEGRO_LINE_CAP_CLOSED,
					//  color, (float)p->line_thickness, 0.0);
					//printf("polygon ");//------------------------------------teste
				} break;
				
				case NK_COMMAND_POLYGON_FILLED: {
					const struct nk_command_polygon_filled *p = (const struct nk_command_polygon_filled *)cmd;
					color = nk_to_bmp_color(p->color);
					//int i;
					//float vertices[p->point_count * 2];
					// for (i = 0; i < p->point_count; i++) {
					//    vertices[i*2] = p->points[i].x;
					//     vertices[(i*2) + 1] = p->points[i].y;
					// }
					//  al_draw_filled_polygon((const float*)&vertices, (int)p->point_count, color);
					//printf("fill_polygon ");//------------------------------------teste
				} break;
				
				case NK_COMMAND_POLYLINE: {
					const struct nk_command_polyline *p = (const struct nk_command_polyline *)cmd;
					color = nk_to_bmp_color(p->color);
					//int i;
					//float vertices[p->point_count * 2];
					//  for (i = 0; i < p->point_count; i++) {
					//      vertices[i*2] = p->points[i].x;
					//      vertices[(i*2) + 1] = p->points[i].y;
					//  }
					//  al_draw_polyline((const float*)&vertices, (2 * sizeof(float)),
					//      (int)p->point_count, ALLEGRO_LINE_JOIN_ROUND, ALLEGRO_LINE_CAP_ROUND,
					//      color, (float)p->line_thickness, 0.0);
					//printf("polyline ");//------------------------------------teste
				} break;
				
				case NK_COMMAND_TEXT: {
					const struct nk_command_text *t = (const struct nk_command_text*)cmd;
					color = nk_to_bmp_color(t->foreground);
					
					nk_rune str_uni[255];
					char str[255];
					int glyph_size, char_size, pos = 0;
					
					char *curr = 0, *curr_pos =0;
					
					curr = (char *)t->string;
					curr_pos = str;
					pos = 0;
					
					while ((*curr != 0) && (pos < 254)){
					
						glyph_size = nk_utf_decode(curr, str_uni, 10);
						if (glyph_size){
							char_size = wctomb(curr_pos, (wchar_t)str_uni[0]);
							curr += glyph_size;
							pos += char_size;
							curr_pos += char_size;
						}
						else {
							curr = 0;
						}
					}
					
					if(pos<255){
						str[pos] = 0;
					}
					else{
						str[254] = 0;
					}
					
					shape *font = (shape*)t->font->userdata.ptr;
					graph_obj *curr_graph = shx_font_parse(font, 0, (const char*)str);
					/*change the color */
					curr_graph->color = color;

					/* apply the scales, offsets and rotation to graphs */
					graph_modify(curr_graph, t->x, t->y + t->font->height, FONT_SCALE, -FONT_SCALE, 0);
					graph_draw(curr_graph, img, 0, 0, 1);
				} break;
				
				case NK_COMMAND_CURVE: {
					const struct nk_command_curve *q = (const struct nk_command_curve *)cmd;
					color = nk_to_bmp_color(q->color);
					// float points[8];
					// points[0] = (float)q->begin.x;
					// points[1] = (float)q->begin.y;
					// points[2] = (float)q->ctrl[0].x;
					// points[3] = (float)q->ctrl[0].y;
					//points[4] = (float)q->ctrl[1].x;
					// points[5] = (float)q->ctrl[1].y;
					// points[6] = (float)q->end.x;
					// points[7] = (float)q->end.y;
					// al_draw_spline(points, color, (float)q->line_thickness);
					//printf("curve ");//------------------------------------teste
				} break;
				
				case NK_COMMAND_ARC: {
					const struct nk_command_arc *a = (const struct nk_command_arc *)cmd;
					color = nk_to_bmp_color(a->color);
					//    al_draw_arc((float)a->cx, (float)a->cy, (float)a->r, a->a[0],
					//       a->a[1], color, (float)a->line_thickness);
					//printf("arc ");//------------------------------------teste
				} break;
				
				case NK_COMMAND_RECT_MULTI_COLOR: {
					if (!one_time){
						one_time =1;
						printf("multi_c ");//------------------------------------teste
					}
				} break;
				
				case NK_COMMAND_IMAGE: {
					const struct nk_command_image *i = (struct nk_command_image *)cmd;
					bmp_img *img = (bmp_img *)i->img.handle.ptr;
					if (!one_time){
						one_time =1;
						printf("image ");//------------------------------------teste
						printf("%d,%d", img->width, img->height);
					}
				} break;
				
				case NK_COMMAND_ARC_FILLED: {
					
				} break;
				
				default: break;
			}
		}
	}
	nk_clear(&sdl.ctx); /*IMPORTANT */
}

static void nk_sdl_clipbard_paste(nk_handle usr, struct nk_text_edit *edit){
	const char *text = SDL_GetClipboardText();
	if (text) nk_textedit_paste(edit, text, nk_strlen(text));
	(void)usr;
}

static void nk_sdl_clipbard_copy(nk_handle usr, const char *text, int len){
	char *str = 0;
	(void)usr;
	if (!len) return;
	str = (char*)malloc((size_t)len+1);
	if (!str) return;
	memcpy(str, text, (size_t)len);
	str[len] = '\0';
	SDL_SetClipboardText(str);
	free(str);
}

NK_API struct nk_context* nk_sdl_init(SDL_Window *win, shape *shx_font){
	
	/* find the dimentions of SHX font */
	double fnt_size = 0, fnt_above, fnt_below;
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
	
	struct nk_user_font *font = &sdl.font;
	font->userdata = nk_handle_ptr(shx_font);
	font->height = (float)FONT_SCALE*fnt_size;
	font->width = nk_user_font_get_text_width;
	nk_style_set_font(&sdl.ctx, font);	

	sdl.win = win;
	nk_init_default(&sdl.ctx, font);
	sdl.ctx.clip.copy = nk_sdl_clipbard_copy;
	sdl.ctx.clip.paste = nk_sdl_clipbard_paste;
	sdl.ctx.clip.userdata = nk_handle_ptr(0);
	return &sdl.ctx;
}

NK_API int
nk_sdl_handle_event(SDL_Event *evt)
{
	struct nk_context *ctx = &sdl.ctx;

	/* optional grabbing behavior */
	if (ctx->input.mouse.grab) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		ctx->input.mouse.grab = 0;
	}
	else if (ctx->input.mouse.ungrab) {
		int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_WarpMouseInWindow(sdl.win, x, y);
		ctx->input.mouse.ungrab = 0;
	}
	if (evt->type == SDL_KEYUP || evt->type == SDL_KEYDOWN) {
	/* key events */
		int down = evt->type == SDL_KEYDOWN;
		const Uint8* state = SDL_GetKeyboardState(0);
		SDL_Keycode sym = evt->key.keysym.sym;
		
		if (sym == SDLK_RSHIFT || sym == SDLK_LSHIFT)
			nk_input_key(ctx, NK_KEY_SHIFT, down);
		else if (sym == SDLK_DELETE)
			nk_input_key(ctx, NK_KEY_DEL, down);
		else if (sym == SDLK_RETURN)
			nk_input_key(ctx, NK_KEY_ENTER, down);
		else if (sym == SDLK_TAB)
			nk_input_key(ctx, NK_KEY_TAB, down);
		else if (sym == SDLK_BACKSPACE)
			nk_input_key(ctx, NK_KEY_BACKSPACE, down);
		else if (sym == SDLK_HOME) {
			nk_input_key(ctx, NK_KEY_TEXT_START, down);
			nk_input_key(ctx, NK_KEY_SCROLL_START, down);
		}
		else if (sym == SDLK_END) {
			nk_input_key(ctx, NK_KEY_TEXT_END, down);
			nk_input_key(ctx, NK_KEY_SCROLL_END, down);
		}
		else if (sym == SDLK_PAGEDOWN) {
			nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
		}
		else if (sym == SDLK_PAGEUP) {
			nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
		}
		else if (sym == SDLK_z)
			nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_r)
			nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_c)
			nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_v)
			nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_x)
			nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_b)
			nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_e)
			nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);
		else if (sym == SDLK_UP)
			nk_input_key(ctx, NK_KEY_UP, down);
		else if (sym == SDLK_DOWN)
			nk_input_key(ctx, NK_KEY_DOWN, down);
		else if (sym == SDLK_LEFT) {
			if (state[SDL_SCANCODE_LCTRL])
				nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
			else nk_input_key(ctx, NK_KEY_LEFT, down);
		} 
		else if (sym == SDLK_RIGHT) {
			if (state[SDL_SCANCODE_LCTRL])
				nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
			else nk_input_key(ctx, NK_KEY_RIGHT, down);
		} else return 0;
		return 1;
	} 
	else if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) {
		/* mouse button */
		int down = evt->type == SDL_MOUSEBUTTONDOWN;
		const int x = evt->button.x, y = evt->button.y;
		
		if (evt->button.button == SDL_BUTTON_LEFT) {
			if (evt->button.clicks > 1)
				nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
			nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
		}
		else if (evt->button.button == SDL_BUTTON_MIDDLE)
			nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
		else if (evt->button.button == SDL_BUTTON_RIGHT)
			nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
		else return 0;
		return 1;
	}
	else if (evt->type == SDL_MOUSEMOTION) {
		/* mouse motion */
		if (ctx->input.mouse.grabbed) {
			int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
			nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
		}
		else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
		return 1;
	}
	else if (evt->type == SDL_TEXTINPUT) {
		/* text input */
		nk_glyph glyph;
		memcpy(glyph, evt->text.text, NK_UTF_SIZE);
		nk_input_glyph(ctx, glyph);
		return 1;
	}
	else if (evt->type == SDL_MOUSEWHEEL) {
		/* mouse wheel */
		nk_input_scroll(ctx,nk_vec2((float)evt->wheel.x,(float)evt->wheel.y));
		return 1;
	}
	return 0;
}

NK_API
void nk_sdl_shutdown(void)
{
    nk_free(&sdl.ctx);
    memset(&sdl, 0, sizeof(sdl));
}

//#endif
