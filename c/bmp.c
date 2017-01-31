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

struct Bmp_img {
	unsigned int width;
	unsigned int height;
	unsigned char * buf; /* the color depth is 8 bit per component */
};
typedef struct Bmp_img bmp_img;

struct Bmp_color {
	unsigned char r, g, b, a;
};
typedef struct Bmp_color bmp_color;



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

bmp_img * bmp_new (unsigned int width, unsigned int height){
	bmp_img * img = (bmp_img *) malloc (sizeof (bmp_img));
	if (img){
		img->width = width;
		img->height = height;
		/* the image will have 4 chanels: R, G, B and alfa */
		img->buf = malloc(4 * width * height * sizeof(unsigned char));
		if (img->buf == NULL){ /* fail in memory allocation */
			free (img);
			img = NULL;
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

int bmp_point (bmp_img *img, int x, int y, bmp_color color){
	unsigned int ofs;
	
	if(img != NULL){
		if((abs(x) < img->width) && (abs(y) < img->height)){
			
			ofs = 4 * ((y * img->width) + x);
			img->buf[ofs] = color.r;
			img->buf[ofs+1] = color.g;
			img->buf[ofs+2] = color.b;
			img->buf[ofs+3] = color.a;
			
			return 1;
		}
	}
	return 0;
}


void bmp_line_raw(bmp_img *img, int x0, int y0, int x1, int y1, bmp_color color) {
//Bitmap/Bresenham's line algorithm
//http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
	
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		bmp_point(img, x0, y0, color);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

void bmp_line(bmp_img *img, double x0, double y0, double x1, double y1, bmp_color color) {

	/* Cohen–Sutherland clipping algorithm clips a line from
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
		bmp_line_raw(img, (int) x0, (int) y0, (int) x1, (int) y1, color);
		//printf("(%d,%d)----(%d,%d)\n", (int) x0, (int) y0, (int) x1, (int) y1);
	}
}

int main (void){
	bmp_img * img = bmp_new(100,100);
	bmp_color blue, black;
	blue.r = 100;
	blue.g = 0;
	blue.b = 255;
	blue.a = 255;
	
	black.r = 0;
	black.g = 0;
	black.b = 0;
	black.a = 255;
	
	bmp_fill(img, blue);
	bmp_point(img, 10, 20, black);
	bmp_line(img, 20, 20, 500, 700, black);
	bmp_line(img, 0, 100, 50, 70, black);
	bmp_line(img, -50, 50, 250, 50, black);
	
	bmp_save("teste.ppm", img);
	bmp_free(img);
	
	
	return 0;
}

/*

//https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
typedef int OutCode;

const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

// ASSUME THAT xmax, xmin, ymax and ymin are global constants.

OutCode ComputeOutCode(double x, double y)
{
	OutCode code;

	code = INSIDE;          // initialised as being inside of [[clip window]]

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
void CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(x0, y0);
	OutCode outcode1 = ComputeOutCode(x1, y1);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		} else if (outcode0 & outcode1) { // Bitwise AND is not 0. (implies both end points are in the same region outside the window). Reject and get out of loop
			break;
		} else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(x0, y0);
			} else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(x1, y1);
			}
		}
	}
	if (accept) {
               // Following functions are left for implementation by user based on
               // their platform (OpenGL/graphics.h etc.)
               DrawRectangle(xmin, ymin, xmax, ymax);
               LineSegment(x0, y0, x1, y1);
	}
}
*/