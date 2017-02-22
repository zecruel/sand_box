#include "dxf.h"
#include "bmp.h"
#include "graph.h"
#include "dxf_graph.h"

int main(void)
{	
	char url[]="teste.dxf";
	dxf_drawing drawing;
	int i;
	double min_x, min_y, max_x, max_y;
	double zoom_x, zoom_y, zoom, ofs_x, ofs_y;
	
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
	
	bmp_img * img = bmp_new(800, 600, white, red);
	
	zoom_x = (max_x - min_x)/img->width;
	zoom_y = (max_y - min_y)/img->height;
	zoom = (zoom_x > zoom_y) ? zoom_x : zoom_y;
	if (zoom <= 0){ zoom =1;}
	else{ zoom = 1/(1.1 * zoom);}
	
	ofs_x = min_x - (fabs((max_x - min_x)*zoom - img->width)/2)/zoom;
	ofs_y = min_y - (fabs((max_y - min_y)*zoom - img->height)/2)/zoom;
	
	dxf_ents_draw(drawing, img, ofs_x, ofs_y, zoom);
	//bmp_save("teste.ppm", img);

	dxf_ent_clear(drawing.main_struct);
	bmp_free(img);

	return 0;
}
