#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* position of a point in rectangle */
enum Rect_pos{  /* is bit coded */
	INSIDE = 0, /* 0000 */
	LEFT = 1,   /* 0001 */
	RIGHT = 2,  /* 0010 */
	BOTTOM = 4, /* 0100 */
	TOP = 8    /* 1000 */
};
typedef enum Rect_pos rect_pos;

struct Bmp_color {
	/* the color depth is 8 bit per component */
	unsigned char r, g, b, a;
};
typedef struct Bmp_color bmp_color;

struct Bmp_img {
	unsigned int width;
	unsigned int height;
	bmp_color bkg; /* background color */
	bmp_color frg; /* current foreground color */
	unsigned int tick; /* current brush tickness */
	
	/* the pixmap */
	unsigned char * buf; /* the color depth is 8 bit per component */
};
typedef struct Bmp_img bmp_img;


/* Compute the bit code for a point (x, y) using the clip rectangle
 bounded diagonally by (xmin, ymin), and (xmax, ymax) */
rect_pos rect_find_pos(double x, double y, double xmin, double ymin, double xmax, double ymax){
	rect_pos code;
	code = INSIDE;          /* initialised as being inside of [[clip window]]*/

	if (x < xmin)           /* to the left of clip window*/
		code |= LEFT;
	else if (x > xmax)      /* to the right of clip window*/
		code |= RIGHT;
	if (y < ymin)           /* below the clip window*/
		code |= BOTTOM;
	else if (y > ymax)      /* above the clip window*/
		code |= TOP;
	
	return code;
}

int bmp_fill (bmp_img *img, bmp_color color){
	unsigned int i, n;
	
	if (img){
		n = 4* img->width * img->height;
		for (i=0; i < n; i += 4)
		{
			img->buf[i] = color.r;
			img->buf[i+1] = color.g;
			img->buf[i+2] = color.b;
			img->buf[i+3] = color.a;
		}
		return 1; /*return success*/
	}
	return 0; /* return fail */
}

bmp_img * bmp_new (unsigned int width, unsigned int height, bmp_color bkg, bmp_color frg){
	bmp_img * img = (bmp_img *) malloc (sizeof (bmp_img));
	if (img){
		img->width = width;
		img->height = height;
		img->bkg = bkg;
		img->frg =frg;
		img->tick = 0;
		/* the image will have 4 chanels: R, G, B and alfa */
		img->buf = malloc(4 * width * height * sizeof(unsigned char));
		if (img->buf == NULL){ /* fail in memory allocation */
			free (img);
			img = NULL;
		}
		else{
			bmp_fill (img, bkg); /* fill the image with the background color */
		}
	}
	return img;
}

void bmp_free(bmp_img *img){
	if(img){
		free(img->buf);
		free(img);
	}
}

int bmp_save (char *path, bmp_img *img){
	FILE *file;
	int ret_success;
	unsigned int i, n;
	
	ret_success = 0;
	file = fopen(path, "w"); /* open the file */
	if ((file != NULL) && (img != NULL)){
		fprintf(file, "P6 %d %d 255\r",  img->width, img->height);
		
		
		n = 4 * img->width * img->height;
		for (i=0; i < n; i += 4)
		{
			fputc(img->buf[i], file);
			fputc(img->buf[i+1], file);
			fputc(img->buf[i+2], file);
			//fputc(img->buf[i+3], file);
		}
		
		
		ret_success = 1; /* return success */
	}
	
	fclose(file);
	
	return ret_success;
}

int bmp_point_raw (bmp_img *img, int x, int y){
	unsigned int ofs;
	
	if(img != NULL){
		if((abs(x) < img->width) && (abs(y) < img->height)){
			
			ofs = 4 * ((y * img->width) + x);
			img->buf[ofs] = img->frg.r;
			img->buf[ofs+1] = img->frg.g;
			img->buf[ofs+2] = img->frg.b;
			img->buf[ofs+3] = img->frg.a;
			
			return 1;
		}
	}
	return 0;
}

int bmp_point (bmp_img *img, int xc, int yc){
	if(img != NULL){
		if (img->tick > 1){
			unsigned int i, j, half;
			int x, y;
			
			half = img->tick/2;
			x = xc - half;
			y = yc - half;
			
			for (i = 0; i < img->tick; i++){
				for (j = 0; j < img->tick; j++){
					if((abs(x) < img->width) && (abs(y) < img->height)){
						bmp_point_raw (img, x+i, y+j);
					}
				}
			}
			return 1;
		}
		else{
			bmp_point_raw (img, xc, yc);
			return 1;
		}
	}
	return 0;
}

void bmp_line_raw(bmp_img *img, int x0, int y0, int x1, int y1) {
/*Bitmap/Bresenham's line algorithm
from: http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C */
	
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		bmp_point(img, x0, y0);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void bmp_line(bmp_img *img, double x0, double y0, double x1, double y1) {

	/* 
	from: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
	Cohen–Sutherland clipping algorithm clips a line from
	P0 = (x0, y0) to P1 = (x1, y1) against a rectangle of bitmap image */
	
	/* compute outcodes for P0, P1, and whatever point lies outside the clip rectangle */
	rect_pos pos_p0 = rect_find_pos(x0, y0, 0, 0, img->width, img->height);
	rect_pos pos_p1 = rect_find_pos(x1, y1, 0, 0, img->width, img->height);
	
	int accept = 0;

	while (1) {
		if (!(pos_p0 | pos_p1)) { /* Bitwise OR is 0. Trivially accept and get out of loop */
			accept = 1;
			break;
		}
		else if (pos_p0 & pos_p1) { /* Bitwise AND is not 0. 
			(implies both end points are in the same region outside the 
			window). Reject and get out of loop */
			break;
		}
		else {
			/* failed both tests, so calculate the line segment to clip
			from an outside point to an intersection with clip edge*/
			double x, y;

			/* At least one endpoint is outside the clip rectangle; pick it. */
			rect_pos pos_out = pos_p0 ? pos_p0 : pos_p1;

			/* Now find the intersection point;
			use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0) */
			if (pos_out & TOP) {           /* point is above the clip rectangle */
				x = x0 + (x1 - x0) * (img->height - y0) / (y1 - y0);
				y = img->height;
			} else if (pos_out & BOTTOM) { /* point is below the clip rectangle*/
				x = x0 + (x1 - x0) * (0 - y0) / (y1 - y0);
				y = 0;
			} else if (pos_out & RIGHT) {  /* point is to the right of clip rectangle */
				y = y0 + (y1 - y0) * (img->width - x0) / (x1 - x0);
				x = img->width;
			} else if (pos_out & LEFT) {   /* point is to the left of clip rectangle */
				y = y0 + (y1 - y0) * (0 - x0) / (x1 - x0);
				x = 0;
			}

			/* Now we move outside point to intersection point to clip
			and get ready for next pass.*/
			if (pos_out == pos_p0) {
				x0 = x;
				y0 = y;
				pos_p0 = rect_find_pos(x0, y0, 0, 0, img->width, img->height);
			} else {
				x1 = x;
				y1 = y;
				pos_p1 = rect_find_pos(x1, y1, 0, 0, img->width, img->height);
			}
		}
	}
	if (accept) {
		bmp_line_raw(img, (int) x0, (int) y0, (int) x1, (int) y1);
		//printf("(%d,%d)----(%d,%d)\n", (int) x0, (int) y0, (int) x1, (int) y1);
	}
}

int main (void){
	
	bmp_color white = {.r = 255, .g = 255, .b =255, .a = 255};
	bmp_color black = {.r = 0, .g = 0, .b =0, .a = 255};
	bmp_color blue = {.r = 0, .g = 0, .b =255, .a = 255};
	bmp_color red = {.r = 255, .g = 0, .b =0, .a = 255};
	bmp_color green = {.r = 0, .g = 255, .b =0, .a = 255};
	
	bmp_img * img = bmp_new(100,100, white, black);
	
	//bmp_fill(img, blue);
	bmp_point(img, 10, 20);
	img->frg = blue;
	bmp_line(img, 20, 20, 500, 700);
	
	img->tick = 3;
	bmp_line(img, 0, 100, 50, 70);
	img->frg = red;
	bmp_line(img, -50, 50, 250, 50);
	
	bmp_save("teste.ppm", img);
	bmp_free(img);
	
	
	return 0;
}