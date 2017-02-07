#include "dxf.h"
#include "bmp.h"
#include "graph.h"

int main(void)
{
	char url[]="teste.dxf";
	dxf_drawing drawing;
	vector_p stack;
	int i;
	
	stack.size = 0;
	stack.data = NULL;
	
	drawing = dxf_open(url);
	
	//dxf_ent_print(drawing.main_struct->obj.content->next, 0);
	
	/*
	if (dxf_save ("save_test.dxf", drawing)){
		printf("\nSUCESSO!\n");
	}
	else{
		printf("\nFalhou\n");
	}
	*/
	
	dxf_graph_parse(drawing, drawing.ents);
	
	bmp_color white = {.r = 255, .g = 200, .b =200, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	
	int center [] = {12, -6, 2 , -6};
	int dash [] = {8, -8};
	
	bmp_img * img = bmp_new(200, 200, white, black);
	
	dxf_graph_draw(drawing, drawing.ents, img);
	
	
	dxf_ent_clear(drawing.main_struct);
	
	bmp_save("teste.ppm", img);
	bmp_free(img);

	printf("suceso\n");
	return 0;
}
	
	/*
	printf("%d\n", drawing.num_layers);
	for (i = 0; i < drawing.num_layers; i++){
		printf("%s %d %s off = %d lock = %d froz = %d\n",
		drawing.layers[i].name, 
		drawing.layers[i].color, 
		drawing.layers[i].ltype,
		drawing.layers[i].off,
		drawing.layers[i].lock,
		drawing.layers[i].frozen);
	}
	
	
	printf("%d\n", dxf_lay_idx(drawing, "PEDAL"));
	
	printf("%d\n", drawing.num_ltypes);
	for (i = 0; i < drawing.num_ltypes; i++){
		printf("%s %d %f [%f %f %f %f]\n",
		drawing.ltypes[i].name, 
		drawing.ltypes[i].size,
		drawing.ltypes[i].length,
		drawing.ltypes[i].pat[0],
		drawing.ltypes[i].pat[1],
		drawing.ltypes[i].pat[2],
		drawing.ltypes[i].pat[3]
		);
	}
	
	printf("%d\n", dxf_ltype_idx(drawing, "CENTER"));
	
	//dxf_ent_print(drawing.t_layer->obj.content->next, 0);
	
	/*
	stack_push (&stack, drawing.head->obj.content->next);
	stack_push (&stack, drawing.t_layer->obj.content->next);

	dxf_ent_print(stack_pop(&stack), 0);
	dxf_ent_print(stack_pop(&stack), 0);
	
	printf("\n%d",stack.size);
	*/
	
	/*sec = dxf_find_obj(drawing, "SECTION");
	
	if ((sec.size > 0) && (sec.data != NULL)){
		//ent_print(((dxf_node **) sec.data)[0]->obj.content->next, 0);
		
		int i;
		for (i=0; i < sec.size; i++){
			printf ("\n%s ", ((dxf_node **) sec.data)[i]->obj.name);
			name = dxf_find_attr(((dxf_node **) sec.data)[i], 2);
			printf ("%d ", name.size);
			if (name.data){
				printf("%s", ((dxf_node **) name.data)[0]->value.s_data);
			}
		}
	}
	
	head = dxf_find_obj_descr(drawing, "SECTION", "HEADER");
	if (head.data){
		ent_print(((dxf_node **) head.data)[0]->obj.content->next, 0);
	}
	
	free(sec.data);
	free(name.data);
	free(head.data); 
	
	dxf_ent_clear(drawing.main_struct);
	
	bmp_color white = {.r = 255, .g = 200, .b =200, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	
	int center [] = {12, -6, 2 , -6};
	int dash [] = {8, -8};
	
	bmp_img * img = bmp_new(200, 200, white, black);
	
	//bmp_fill(img, blue);
	bmp_point(img, 10, 20);
	img->frg = blue;
	bmp_line(img, 20, 20, 500, 700);
	
	patt_change(img, center, 4);
	img->tick = 2;
	bmp_line(img, 0, 100, 50, 70);
	
	patt_change(img, dash, 2);
	img->frg = red;
	bmp_line(img, -50, 50, 250, 50);
	
	img->frg = green;
	img->tick = 3;
	bmp_line(img, 100, 0, 0, 70);
	
	graph_obj * test = graph_new();
	line_add(test, 0,0,100,100);
	line_add(test, 210,210,100,2);
	test->tick = 5;
	test->color = red;
	graph_draw(test, img);
	
	bmp_save("teste.ppm", img);
	bmp_free(img);

	
	return 0;
}

/*
//Bitmap/Bresenham's line algorithm
//http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

void line(int x0, int y0, int x1, int y1) {
 
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    setPixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}


Depth-first search (DFS): Finding connected components.

*/