#include "bmp.h"

rect_pos rect_find_pos(double x, double y, double xmin, double ymin, double xmax, double ymax){
/* Compute the bit code for a point (x, y) using the clip rectangle
 bounded diagonally by (xmin, ymin), and (xmax, ymax) */
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

int line_clip(bmp_img *img, double *x0, double *y0, double *x1, double *y1) {

	/* 
	clip the line on the window area
	
	from: https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
	Cohen–Sutherland clipping algorithm clips a line from
	P0 = (*x0, *y0) to P1 = (*x1, *y1) against a rectangle of bitmap image */
	
	/* compute outcodes for P0, P1, and whatever point lies outside the clip rectangle */
	rect_pos pos_p0 = rect_find_pos(*x0, *y0, 0, 0, img->width, img->height);
	rect_pos pos_p1 = rect_find_pos(*x1, *y1, 0, 0, img->width, img->height);
	
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
			use formulas y = *y0 + slope * (x - *x0), x = *x0 + (1 / slope) * (y - *y0) */
			if (pos_out & TOP) {           /* point is above the clip rectangle */
				x = *x0 + (*x1 - *x0) * (img->height - *y0) / (*y1 - *y0);
				y = img->height;
			} else if (pos_out & BOTTOM) { /* point is below the clip rectangle*/
				x = *x0 + (*x1 - *x0) * (0 - *y0) / (*y1 - *y0);
				y = 0;
			} else if (pos_out & RIGHT) {  /* point is to the right of clip rectangle */
				y = *y0 + (*y1 - *y0) * (img->width - *x0) / (*x1 - *x0);
				x = img->width;
			} else if (pos_out & LEFT) {   /* point is to the left of clip rectangle */
				y = *y0 + (*y1 - *y0) * (0 - *x0) / (*x1 - *x0);
				x = 0;
			}

			/* Now we move outside point to intersection point to clip
			and get ready for next pass.*/
			if (pos_out == pos_p0) {
				*x0 = x;
				*y0 = y;
				pos_p0 = rect_find_pos(*x0, *y0, 0, 0, img->width, img->height);
			} else {
				*x1 = x;
				*y1 = y;
				pos_p1 = rect_find_pos(*x1, *y1, 0, 0, img->width, img->height);
			}
		}
	}
	return accept;
}

int bmp_fill (bmp_img *img, bmp_color color){
	/* fill the bmp image with passed color */
	
	unsigned int i, n, r_i, g_i, b_i, a_i;
	
	if (img){
		/* get the order of color components */
		r_i = img->r_i;
		g_i = img->g_i;
		b_i = img->b_i;
		a_i = img->a_i;
		
		n = 4 * img->width * img->height; /* four components: RGBA */
		for (i=0; i < n; i += 4) /* increment by 4 in each step */
		{ /* store each component in memory buffer */
			img->buf[i+r_i] = color.r;
			img->buf[i+g_i] = color.g;
			img->buf[i+b_i] = color.b;
			img->buf[i+a_i] = color.a;
		}
		return 1; /*return success*/
	}
	return 0; /* return fail */
}

bmp_img * bmp_new (unsigned int width, unsigned int height, bmp_color bkg, bmp_color frg){
	/* create new bmp image */
	
	/* alloc the structure */
	bmp_img * img = (bmp_img *) malloc (sizeof (bmp_img));
	if (img){
		/* initialize */
		/* dimensions */
		img->width = width;
		img->height = height;
		/* colors */
		img->bkg = bkg;
		img->frg = frg;
		/* line pattern generation vars  */
		img->tick = 0;
		img->patt_i = 0;
		img->pix_count = 0;
		/* line pattern */
		/* initialize the image with a solid line pattern */
		img->patt_size = 1;
		img->pattern[0] = 1;
		img->pat_scale = 15;
		
		/*order of color components in buffer. Init with ARGB */
		img->r_i = 2;
		img->g_i = 1;
		img->b_i = 0;
		img->a_i = 3;
		
		/* alloc the pix map buffer
		the image will have 4 chanels: R, G, B and alfa */
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
	/* save the pixmap in a PPM file */
	
	FILE *file;
	int ret_success;
	unsigned int i, n, r_i, g_i, b_i, a_i;
	
	ret_success = 0;
	file = fopen(path, "w"); /* open the file */
	if ((file != NULL) && (img != NULL)){
		/* get the order of color components */
		r_i = img->r_i;
		g_i = img->g_i;
		b_i = img->b_i;
		a_i = img->a_i;
		
		/* the header of PPM: binary file with 8 bit deph */
		fprintf(file, "P6 %d %d 255\r",  img->width, img->height);
		
		/* sweep the buffer of image*/
		n = 4 * img->width * img->height;
		for (i=0; i < n; i += 4){
			/* only the RGB components are writen */
			fputc(img->buf[i+r_i], file);
			fputc(img->buf[i+g_i], file);
			fputc(img->buf[i+b_i], file);
		}
		ret_success = 1; /* return success */
	}
	
	fclose(file);
	
	return ret_success;
}

void bmp_point_raw (bmp_img *img, int x, int y){
	/* draw one pixel on x,y coordinates in bmp image */
	
	unsigned int ofs, r_i, g_i, b_i, a_i;
	
	if(img != NULL){
		/* get the order of color components */
		r_i = img->r_i;
		g_i = img->g_i;
		b_i = img->b_i;
		a_i = img->a_i;
		
		/* check if point is in image bounds */
		if((x >= 0) && (x < img->width) && 
			(y >= 0) && (y < img->height)){
			/* find the initial position on image buffer */
			/* (y = img->height - y) emulate the cartesian coordinates */
			ofs = 4 * (((img->height - 1 - y) * img->width) + x);
			/* store each component in memory buffer 
			with the image´s foreground color*/
			img->buf[ofs+r_i] = img->frg.r;
			img->buf[ofs+g_i] = img->frg.g;
			img->buf[ofs+b_i] = img->frg.b;
			img->buf[ofs+a_i] = img->frg.a;
		}
	}
}

void bmp_define_brush(bmp_img *img, int x0, int y0, int x1, int y1) {
	double x, y, modulus, p_error_x, p_error_y;
	int dx, dy, sx, sy, err, e2, i;
	int xb, yb, x_end, y_end; 
	int half_x, half_y; /* to determine line's center*/

	/* get the vector of line */
	x = x1 - x0;
	y = y1 - y0;
	modulus = sqrt(pow(x, 2) + pow(y, 2));
	if (modulus > 0){ /* change to unit */
		x /= modulus;
		y /= modulus;
	}
	/* normal vector is xn = -y, yn = x */
	x_end = (int)(round((double)img->tick * -y));
	y_end = (int)(round((double)img->tick * x));
	
	p_error_x = fabs(fabs(x) - fabs(y_end));
	p_error_y = fabs(fabs(y) - fabs(x_end));
	
	
	half_x = x_end/2;
	half_y = y_end/2;
	
	
	/* generate the image brush by Bresenham's line algorithm*/
	dx = abs(x_end);
	dy = abs(y_end);
	
	sx = x_end>=0 ? 1 : -1;
	sy = y_end>=0 ? 1 : -1;
	err = (dx>dy ? dx : -dy)/2;
	
	xb = 0;
	yb = 0;

	for(i = 0; i < BMP_MAX_BRUSH; i++){
		/* index of brush pixels */
		img->brush_x[i] = xb - half_x;
		img->brush_y[i] = yb - half_y;
		
		if (xb==x_end && yb==y_end) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; xb += sx; }
		if (e2 < dy) { err += dx; yb += sy; }
	}
	img->brush_size = i +1;
	
	/*repeat */
	if ((p_error_x > 0) || (p_error_x > 0)){
		if (fabs(x)>fabs(y)){
			xb = 1;
			yb = 0;
			x_end++;
		}
		else{
			yb = 1;
			xb = 0;
			y_end++;
		}

		for(; i < BMP_MAX_BRUSH; i++){
			/* index of brush pixels */
			img->brush_x[i] = xb - half_x;
			img->brush_y[i] = yb - half_y;
			
			if (xb==x_end && yb==y_end) break;
			e2 = err;
			if (e2 >-dx) { err -= dy; xb += sx; }
			if (e2 < dy) { err += dx; yb += sy; }
		}

		img->brush_size = i +1;
	}
}

void bmp_point (bmp_img *img, int xc, int yc){
	/* Draw a point on image. The coordinates xc,yc indicates the center of point.
	the tickness is specified in image */
	
	if(img != NULL){
		if (img->tick > 1){ /* if the point have a tickness */
			unsigned int i;
			
			for (i = 0; i < img->brush_size; i++){
				bmp_point_raw (img, xc+img->brush_x[i], yc+img->brush_y[i]);
			}
		}
		else{ /* a skinny point */
			bmp_point_raw (img, xc, yc);
		}
	}
}

int patt_change(bmp_img *img, double patt[], int size){
	/* change the current pattern in image to draw lines */
	if(img != NULL){
		int i;
		/*set size */
		img->patt_size = size;
		/*TODO: verifiy the maximum size */
		
		/* restart the pattern generation*/
		img->patt_i = 0;
		img->pix_count = 0;
		
		/*set the pattern data*/
		for (i = 0; i < size; i++){
			img->pattern[i] = (int) (img->pat_scale * patt[i]);
		}
		
		return 1; /* return success */
	}
	return 0; /* fail */
}

int patt_check(bmp_img *img){
	/* auxiliary function to line pattern generation
	check if current position on line will be draw ("pen down") or will be blank ("pen up") */
	
	int pen = 0; /* pen up by default*/
	
	if(img != NULL){
		img->pix_count += 1; /* increment the position */
		/* check if current pattern data is positive */
		if (img->pattern[img->patt_i] >=0){ 
			/* its means the draw command */
			pen = 1; /* pen down */
		}
		/*check the end of current pattern data */
		if (img->pix_count >= abs(img->pattern[img->patt_i])){
			img->pix_count = 0; /* restart the position */
			img->patt_i += 1; /* go to the next pattern data */
			/*check the end of pattern */
			if (img->patt_i >= img->patt_size){
				img->patt_i = 0; /* restart the pattern */
			}
		}
	}
	return pen;
}

void bmp_circle_fill(bmp_img *img, int x0, int y0, int radius){
	int x = radius;
	int y = 0;
	int err = 0;
	int x_start, x_end, i, signal;

	while (x >= y){
		//putpixel(x0 + x, y0 + y);
		//putpixel(x0 - x, y0 + y);
		x_start = x0 + x;
		x_end = x0 - x;
		signal = x_start < x_end ? 1 : -1;
		for(i = x_start; i != x_end; i += signal){
			bmp_point_raw (img, i, y0 + y);
		}
		
		//putpixel(x0 + y, y0 + x);
		//putpixel(x0 - y, y0 + x);
		x_start = x0 + y;
		x_end = x0 - y;
		signal = x_start < x_end ? 1 : -1;
		for(i = x_start; i != x_end; i += signal){
			bmp_point_raw (img, i, y0 + x);
		}
		
		//putpixel(x0 + x, y0 - y);
		//putpixel(x0 - x, y0 - y);
		x_start = x0 + x;
		x_end = x0 - x;
		signal = x_start < x_end ? 1 : -1;
		for(i = x_start; i != x_end; i += signal){
			bmp_point_raw (img, i, y0 - y);
		}
		
		//putpixel(x0 - y, y0 - x);
		//putpixel(x0 + y, y0 - x);
		x_start = x0 + y;
		x_end = x0 - y;
		signal = x_start < x_end ? 1 : -1;
		for(i = x_start; i != x_end; i += signal){
			bmp_point_raw (img, i, y0 - x);
		}
		

		if (err <= 0){
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0){
			x -= 1;
			err -= 2*x + 1;
		}
	}
}

void bmp_line_raw(bmp_img *img, int x0, int y0, int x1, int y1) {
/* Draw a line on bmp image
Bitmap/Bresenham's line algorithm
from: http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C */
	
	int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
	int err = (dx>dy ? dx : -dy)/2, e2;

	for(;;){
		if (patt_check(img)){ /* image's line pattern generation*/
			bmp_point(img, x0, y0);
		}
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	
	
	/* at end of line, draw a circle */
	if (img->tick > 1){ /* if the point have a tickness*/
		bmp_circle_fill(img, x1, y1, (int)round(img->tick /2));
	}
	/*
		unsigned int i, j, half;
		int x, y;
		/* TODO : better aproximation insted of a rectangle
		
		/* find the initial coordinates
		half = img->tick/2;
		x = x1 - half;
		y = y1 - half;
		
		/* fill the rectangle 
		for (i = 0; i < img->tick; i++){
			for (j = 0; j < img->tick; j++){
				bmp_point_raw (img, x+i, y+j);
			}
		}
	}*/
}

void bmp_line(bmp_img *img, double x0, double y0, double x1, double y1) {

	/* 
	Draw a line on bmp image - clip the line on the window area
	*/
	
	if (line_clip(img, &x0, &y0, &x1, &y1)) {
		bmp_define_brush(img, (int) x0, (int) y0, (int) x1, (int) y1);
		bmp_line_raw(img, (int) x0, (int) y0, (int) x1, (int) y1);
	}
}



void bmp_copy(bmp_img *src, bmp_img *dst, int x, int y){
	
	unsigned int i, j, ofs_src, ofs_dst, src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a;
	int ofs_x, ofs_y;
	
	if((src != NULL) && (dst != NULL)){
		/* get the order of color components */
		src_r= src->r_i;
		src_g = src->g_i;
		src_b = src->b_i;
		src_a = src->a_i;
		dst_r= dst->r_i;
		dst_g = dst->g_i;
		dst_b = dst->b_i;
		dst_a = dst->a_i;
		
		/* sweep the source image */
		for (i=0; i < src->width; i++){
			for (j=0; j < src->height; j++){
				/* check if point is in destination bounds */
				ofs_x = i + x;
				ofs_y = j + y;
				if((ofs_x >= 0) && (ofs_x < dst->width) && 
					(ofs_y >= 0) && (ofs_y < dst->height)){
					/* find the position on destination buffer */
					/* (y = dst->height - y) emulate the cartesian coordinates */
					ofs_dst = 4 * (((dst->height - 1 - ofs_y) * dst->width) + ofs_x);
					ofs_src = 4 * (((src->height - 1 - j) * src->width) + i);
					/* store each component in memory buffer */
					dst->buf[ofs_dst + dst_r] = src->buf[ofs_src + src_r];
					dst->buf[ofs_dst + dst_g] = src->buf[ofs_src + src_g];
					dst->buf[ofs_dst + dst_b] = src->buf[ofs_src + src_b];
					dst->buf[ofs_dst + dst_a] = src->buf[ofs_src + src_a];
				}
			}
		}
	}
}