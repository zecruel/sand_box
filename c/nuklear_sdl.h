/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 * authored from 2015-2017 by Micha Mettke
 */
 
 
 /* structure to store vectors (arrays) */
struct Vector_g{
	int size;
	graph_obj *data[1000];
};
typedef struct Vector_g vector_g;
 
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
NK_API void                 nk_sdl_render(vector_g *v_ret);
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


static struct nk_sdl {
    SDL_Window *win;
    struct nk_context ctx;
    struct nk_user_font font;
} sdl;

static float
nk_user_font_get_text_width(nk_handle handle, float height, const char *text, int len)
{
    shape *font = (shape*)handle.ptr;
    if ((text!= NULL) && (font!=NULL)) {
	graph_obj *curr_graph = shx_font_parse(font, text);
	if (curr_graph){
		double txt_w;
		txt_w = fabs(curr_graph->ext_max_x - curr_graph->ext_min_x);
		return (float) txt_w;
	}
}
return 0;
}

bmp_color nk_to_bmp_color(struct nk_color color){
	bmp_color ret_color = {.r = color.r, .g = color.g, .b = color.b, .a = color.a };
	return ret_color;
}

NK_API void
nk_sdl_render(vector_g *v_ret)
{
	bmp_color color = {.r = 255, .g = 255, .b =255, .a = 255};
	const struct nk_command *cmd;
	graph_obj *curr_graph;
	
	
	v_ret->size = 0;
	
	
		
    nk_foreach(cmd, &sdl.ctx)
    {
        switch (cmd->type) {
        case NK_COMMAND_NOP: break;
        case NK_COMMAND_SCISSOR: {
            const struct nk_command_scissor *s =(const struct nk_command_scissor*)cmd;
            //al_set_clipping_rectangle((int)s->x, (int)s->y, (int)s->w, (int)s->h);
		//printf("scissor ");//------------------------------------teste
        } break;
        case NK_COMMAND_LINE: {
            const struct nk_command_line *l = (const struct nk_command_line *)cmd;
            color = nk_to_bmp_color(l->color);
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		
		/*change tickness */
			curr_graph->tick = l->line_thickness;
		
		line_add(curr_graph, l->begin.x, l->begin.y, 0, l->end.x,l->end.y, 0);
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
            //al_draw_line((float)l->begin.x, (float)l->begin.y, (float)l->end.x,
            //    (float)l->end.y, color, (float)l->line_thickness);
		//printf("line ");//------------------------------------teste
        } break;
        case NK_COMMAND_RECT: {
            const struct nk_command_rect *r = (const struct nk_command_rect *)cmd;
            color = nk_to_bmp_color(r->color);
		color = nk_to_bmp_color(r->color);
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		
		/*change tickness */
			curr_graph->tick = r->line_thickness;
		
		line_add(curr_graph, r->x,r->y, 0, r->x + r->w, r->y, 0);
		line_add(curr_graph, r->x + r->w, r->y, 0, r->x + r->w, (r->y + r->h), 0);
		line_add(curr_graph, r->x + r->w, (r->y + r->h), 0, r->x, (r->y + r->h), 0);
		line_add(curr_graph, r->x, (r->y + r->h), 0, r->x, r->y, 0);
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
		
		
		
		
            //al_draw_rounded_rectangle((float)r->x, (float)r->y, (float)(r->x + r->w),
            //    (float)(r->y + r->h), (float)r->rounding, (float)r->rounding, color,
             //   (float)r->line_thickness);
		//printf("rect ");//------------------------------------teste
        } break;
        case NK_COMMAND_RECT_FILLED: {
            const struct nk_command_rect_filled *r = (const struct nk_command_rect_filled *)cmd;
            color = nk_to_bmp_color(r->color);
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		/* mark as filled object */
		curr_graph->fill = 1;
		
            //al_draw_filled_rounded_rectangle((float)r->x, (float)r->y,
            //    (float)(r->x + r->w), (float)(r->y + r->h), (float)r->rounding,
             //   (float)r->rounding, color);
		line_add(curr_graph, r->x,r->y, 0, r->x + r->w, r->y, 0);
		line_add(curr_graph, r->x + r->w, r->y, 0, r->x + r->w, (r->y + r->h), 0);
		line_add(curr_graph, r->x + r->w, (r->y + r->h), 0, r->x, (r->y + r->h), 0);
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
		
		//printf("fill_rect ");//------------------------------------teste
        } break;
        case NK_COMMAND_CIRCLE: {
            const struct nk_command_circle *c = (const struct nk_command_circle *)cmd;
            color = nk_to_bmp_color(c->color);
		
	curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		
		/*change tickness */
			curr_graph->tick = c->line_thickness;
		double xr, yr;
            xr = (double)c->w/2;
            yr = (double)c->h/2;
		graph_arc(curr_graph,((double)(c->x)) + xr, ((double)c->y) + yr, 0, xr, 0.0, 0.0, 1);
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
            //float xr, yr;
            //xr = (float)c->w/2;
            //yr = (float)c->h/2;
            //al_draw_ellipse(((float)(c->x)) + xr, ((float)c->y) + yr,
             //   xr, yr, color, (float)c->line_thickness);
		//printf("circle ");//------------------------------------teste
        } break;
        case NK_COMMAND_CIRCLE_FILLED: {
            const struct nk_command_circle_filled *c = (const struct nk_command_circle_filled *)cmd;
            color = nk_to_bmp_color(c->color);
		
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		
		/* mark as filled object */
		curr_graph->fill = 1;
		
		double xr, yr;
            xr = (double)c->w/2;
            yr = (double)c->h/2;
		graph_arc(curr_graph,((double)(c->x)) + xr, ((double)c->y) + yr, 0, xr, 0.0, 0.0, 1);
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
            //float xr, yr;
            //xr = (float)c->w/2;
            //yr = (float)c->h/2;
            //al_draw_filled_ellipse(((float)(c->x)) + xr, ((float)c->y) + yr,
            //    xr, yr, color);
		//printf("fill_circ ");//------------------------------------teste
        } break;
        case NK_COMMAND_TRIANGLE: {
            const struct nk_command_triangle*t = (const struct nk_command_triangle*)cmd;
            color = nk_to_bmp_color(t->color);
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		/*change tickness */
			curr_graph->tick = t->line_thickness;
		line_add(curr_graph, t->a.x, t->a.y, 0, t->b.x, t->b.y, 0);
		line_add(curr_graph, t->b.x, t->b.y, 0, t->c.x, t->c.y, 0);
		line_add(curr_graph, t->c.x, t->c.y, 0, t->a.x, t->a.y, 0);
		
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
            //al_draw_triangle((float)t->a.x, (float)t->a.y, (float)t->b.x, (float)t->b.y,
            //    (float)t->c.x, (float)t->c.y, color, (float)t->line_thickness);
		//printf("triang ");//------------------------------------teste
        } break;
        case NK_COMMAND_TRIANGLE_FILLED: {
            const struct nk_command_triangle_filled *t = (const struct nk_command_triangle_filled *)cmd;
            color = nk_to_bmp_color(t->color);
		curr_graph = graph_new();
		/*change the color */
		curr_graph->color = color;
		/* mark as filled object */
		curr_graph->fill = 1;
		line_add(curr_graph, t->a.x, t->a.y, 0, t->b.x, t->b.y, 0);
		line_add(curr_graph, t->b.x, t->b.y, 0, t->c.x, t->c.y, 0);
		//line_add(curr_graph, t->c.x, t->c.y, 0, t->a.x, t->a.y, 0);
		
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
		
            //al_draw_filled_triangle((float)t->a.x, (float)t->a.y, (float)t->b.x,
            //    (float)t->b.y, (float)t->c.x, (float)t->c.y, color);
		//printf("fill_tri ");//------------------------------------teste
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
           shape *font = (shape*)t->font->userdata.ptr;
		curr_graph = shx_font_parse(font, (const char*)t->string);
		/*change the color */
		curr_graph->color = color;
		
		/* apply the scales, offsets and rotation to graphs */
		graph_modify(curr_graph, t->x, t->y + t->font->height, 1, -1, 0);
			
		if (v_ret->size < 1000){
			v_ret->data[v_ret->size] = curr_graph;
			v_ret->size++;
		}
           // al_draw_text(font->font,
            //    color, (float)t->x, (float)t->y, 0,
            //    (const char*)t->string);
		//printf("text ");//------------------------------------teste
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
        case NK_COMMAND_RECT_MULTI_COLOR:
		//printf("multi_c ");//------------------------------------teste
        case NK_COMMAND_IMAGE:
		//printf("image ");//------------------------------------teste
        case NK_COMMAND_ARC_FILLED:
        default: break;
        }
    }
	
	nk_clear(&sdl.ctx); /*IMPORTANT */
}

static void
nk_sdl_clipbard_paste(nk_handle usr, struct nk_text_edit *edit)
{
    const char *text = SDL_GetClipboardText();
    if (text) nk_textedit_paste(edit, text, nk_strlen(text));
    (void)usr;
}

static void
nk_sdl_clipbard_copy(nk_handle usr, const char *text, int len)
{
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

NK_API struct nk_context*
nk_sdl_init(SDL_Window *win, shape *shx_font)
{
	
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
    font->height = (float)fnt_size;
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
    } else if (ctx->input.mouse.ungrab) {
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
        } else if (sym == SDLK_END) {
            nk_input_key(ctx, NK_KEY_TEXT_END, down);
            nk_input_key(ctx, NK_KEY_SCROLL_END, down);
        } else if (sym == SDLK_PAGEDOWN) {
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        } else if (sym == SDLK_PAGEUP) {
            nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
        } else if (sym == SDLK_z)
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
        } else if (sym == SDLK_RIGHT) {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else nk_input_key(ctx, NK_KEY_RIGHT, down);
        } else return 0;
        return 1;
    } else if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) {
        /* mouse button */
        int down = evt->type == SDL_MOUSEBUTTONDOWN;
        const int x = evt->button.x, y = evt->button.y;
        if (evt->button.button == SDL_BUTTON_LEFT) {
            if (evt->button.clicks > 1)
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
            nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        } else if (evt->button.button == SDL_BUTTON_MIDDLE)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        else if (evt->button.button == SDL_BUTTON_RIGHT)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        else return 0;
        return 1;
    } else if (evt->type == SDL_MOUSEMOTION) {
        /* mouse motion */
        if (ctx->input.mouse.grabbed) {
            int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
        } else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
        return 1;
    } else if (evt->type == SDL_TEXTINPUT) {
        /* text input */
        nk_glyph glyph;
        memcpy(glyph, evt->text.text, NK_UTF_SIZE);
        nk_input_glyph(ctx, glyph);
        return 1;
    } else if (evt->type == SDL_MOUSEWHEEL) {
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
