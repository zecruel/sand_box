#ifndef _BMP_LIB
#define _BMP_LIB

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
	
	int pattern[20]; /* pattern information */
	int patt_size;
	int patt_i; /* current pattern index */
	double pat_scale;
	int pix_count;

	unsigned int tick; /* current brush tickness */
	
	/* index of each color component in buffer order */
	unsigned int r_i, g_i, b_i, a_i;
	
	/* the pixmap */
	unsigned char * buf; /* the color depth is 8 bit per component */
};
typedef struct Bmp_img bmp_img;


rect_pos rect_find_pos(double x, double y, double xmin, double ymin, double xmax, double ymax);

int bmp_fill (bmp_img *img, bmp_color color);

bmp_img * bmp_new (unsigned int width, unsigned int height, bmp_color bkg, bmp_color frg);

void bmp_free(bmp_img *img);

int bmp_save (char *path, bmp_img *img);

void bmp_point_raw (bmp_img *img, int x, int y);

int patt_change(bmp_img *img, double patt[], int size);

int patt_check(bmp_img *img);

void bmp_line(bmp_img *img, double x0, double y0, double x1, double y1);

void bmp_copy(bmp_img *src, bmp_img *dst, int x, int y);

void bmp_thick_line(bmp_img *img, int x0, int y0, int x1, int y1);
void bmp_thin_line(bmp_img *img, int x0, int y0, int x1, int y1) ;

#endif