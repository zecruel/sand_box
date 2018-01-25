#include <stdio.h>
#include "dxf.h"
#include "graph.h"
#include <math.h>

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

/* number of segments for the curve */
#define N_SEG 5

graph_obj * tt_parse_v(stbtt_vertex *vertices, int num_verts, double scale, int pool_idx, double *curr_pos){
	int i, j;
	graph_obj *line_list = NULL;
	
	if (vertices){
		double pre_x = 0;
		double pre_y = 0;
		double px = 0;
		double py = 0;
		double cx = 0;
		double cy = 0;
		double t, a, b, c;
		
		line_list = graph_new(pool_idx);
		if(line_list) line_list->fill = 1;
		for (i=0; i < num_verts; i++){
			px = vertices[i].x * scale;
			py = vertices[i].y * scale;
			if (vertices[i].type == 3){ /*quadratic bezier curve */
				for (j = 1; j <= N_SEG; j++){
					t = (double)j / (double)N_SEG;
					a = pow((1.0 - t), 2.0);
					b = 2.0 * t * (1.0 - t);
					c = pow(t, 2.0);
					
					px = (a * vertices[i-1].x + b * vertices[i].cx + c * vertices[i].x) * scale;
					py = (a * vertices[i-1].y + b * vertices[i].cy + c * vertices[i].y) * scale;
					
					line_add(line_list, pre_x, pre_y, 0.0, px, py, 0.0);
					
					pre_x = px;
					pre_y = py;
				}
			}
			else if (vertices[i].type>1){
				line_add(line_list, pre_x, pre_y, 0.0, px, py, 0.0);
			}
			
			pre_x = px;
			pre_y = py;
		}
		*curr_pos = px;
	}
	return line_list;
}

int tt_load_font (char *path, stbtt_fontinfo *font, double *scale){
	int ok = 0;
	char *ttf_buffer;
	long fsize;
	
	ttf_buffer = dxf_load_file(path, &fsize);
	ok = stbtt_InitFont(font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
	free(ttf_buffer);
	*scale = stbtt_ScaleForPixelHeight(font, 1);
	
	return ok;
}

int tt_parse2(vector_p *v_return, int pool_idx, const char *txt){
	int ok = 0;
	stbtt_fontinfo font;
	double scale;
	graph_obj *curr;
	
	if(tt_load_font("c:/windows/fonts/arial.ttf", &font, &scale)){
	
		int i, str_len, num_verts;
		wchar_t str_uni[255];
		double ofs_x = 0.0, ofs_y = 0.0, curr_pos, new_ofs_x = 0.0;
		stbtt_vertex *vertices = NULL;
	
		//converte o texto em uma string unicode
		str_len = mbstowcs(str_uni, txt, 255);
		
		int adv = 0, ls = 0, kadv = 0;
	
		for(i = 0; i < str_len; i++){
			if ((str_uni[i] != 9) && (str_uni[i] != 32)){
				num_verts = stbtt_GetCodepointShape(&font, str_uni[i], &vertices);
				
				curr = tt_parse_v(vertices, num_verts, scale, pool_idx, &curr_pos);
				
				//stbtt_GetCodepointHMetrics(&font, str_uni[i], &adv, &ls);
				if(i>0) kadv = stbtt_GetCodepointKernAdvance(&font, str_uni[i-1], str_uni[i]);
				
				stbtt_FreeShape(&font, vertices);
				if (curr){
					
					ofs_x += kadv * scale + 0.05;
					new_ofs_x = ofs_x + curr->ext_max_x;
					
					graph_modify(curr, ofs_x, 0.0, 1.0, 1.0, 0.0);
					/* store the graph in the return vector */
					stack_push(v_return, curr);
					
					ofs_x = new_ofs_x;
					
					
				}
			}
			else if ((str_uni[i] == 9)){/* TAB*/
				ofs_x += 1.5;
			}
			else if ((str_uni[i] == 32)){ /* SPACE */
				ofs_x += 0.3;
			}
		}
		ok = 1;
	}

	return ok;
}

graph_obj * tt_parse(int pool_idx, const char *txt, double *w)
{
	char *ttf_buffer;
	long fsize;
	
	stbtt_fontinfo font;
	stbtt_vertex *vertices;
	
	int i, j, str_len, num_vert;
	
	wchar_t str_uni[255];
	double pre_x = 0;
	double pre_y = 0;
	double px = 0;
	double py = 0;
	double max_x = 0;
	double max_y = 0;
	double min_x = 0;
	double min_y = 0;
	
	double ofs_x = 0.0, ofs_y = 0.0;
	
	//converte o texto em uma string unicode
	str_len = mbstowcs(str_uni, txt, 255);
	
	ttf_buffer = dxf_load_file("c:/windows/fonts/arialbd.ttf", &fsize);
	stbtt_InitFont(&font, ttf_buffer, stbtt_GetFontOffsetForIndex(ttf_buffer,0));
	free(ttf_buffer);
	float scale = stbtt_ScaleForPixelHeight(&font, 20);
	
	//cria a lista de retorno
	graph_obj *line_list = graph_new(pool_idx);
	if(line_list) line_list->fill = 1;
	
	for(i = 0; i < str_len; i++){
		num_vert = stbtt_GetCodepointShape(&font, str_uni[i], &vertices);
		//printf("Num verts = %d\n", num_vert);
		

		for (j=0; j < num_vert; j++){

			//printf ("t=%d (%0.2f,%0.2f)\n", vertices[j].type, vertices[j].x*scale, vertices[j].y*scale);
			px = vertices[j].x * scale + ofs_x;
			py = vertices[j].y * scale + ofs_y;
			
			if (vertices[j].type>1) line_add(line_list, pre_x, pre_y, 0.0, px, py, 0.0);
			
			pre_x = px;
			pre_y = py;
			
			if (j>0){
				max_x = (max_x > pre_x) ? max_x : pre_x;
				max_y = (max_y > pre_y) ? max_y : pre_y;
				min_x = (min_x < pre_x) ? min_x : pre_x;
				min_y = (min_y < pre_y) ? min_y : pre_y;
			}
			else{
				max_x = pre_x;
				max_y = pre_y;
				min_x = pre_x;
				min_y = pre_y;
			}
		}
		stbtt_FreeShape(&font, vertices);
		
		ofs_x = max_x;
		
	}
	
	
	
	if (w != NULL) *w = max_x - min_x;

	return line_list;
}