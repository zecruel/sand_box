#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_TRIANG 500
#define TOLERANCE 1e-6
#define MAX_SCAN_LINES 1000
#define MAX_P_NODES 1000

#define GLSL(src) "#version 150 core\n" #src

struct Vertex {	/*use a struct to represent a vertex in openGL */
    GLfloat pos[3];  /*vertex position - x, y, z*/
    GLfloat uv[2];  /*texture coordinates */
    GLubyte col[4]; /* vertex color - RGBA each 0-255d */
};

struct ogl { /* openGL context to pass main parameters */
	struct Vertex *verts; /* vertex buffer */
	GLuint *elems; /*elements buffer */
	int vert_count; /* vertex count */
	int elem_count; /* elements count */
	
	int win_w, win_h; /* canvas size - to perform pixel to openGL conversion */
	int flip_y; /* orientation flag - use cartesian's like oritentation, or canvas like (window up corner) */
	GLubyte fg[4], bg[4]; /*foreground and background colors */
	
	GLuint tex;
	int tex_w, tex_h;
};

struct edge { /* polygon edge */
	int x0, y0, x1, y1;
};

struct p_node { /* node to perform scanline fill algorithm */
	int up, low;
};

struct Image {
	int w, h;
	GLubyte *data;
};

/* mantain one unique element in a sorted array - array of integer values */
int unique (int n, int * a) {
	int dst = 0, i;
	for (i = 1; i < n; ++i) {
	if (a[dst] != a[i])
		a[++dst] = a[i];
	}
	return dst + 1;
}

/* comparation function for qsort - array of integer values */
int cmp_int (const void * a, const void * b) {
	return (*(int*)a - *(int*)b);
}

/* comparation function for qsort  - array of nodes */
int cmp_node (const void * a, const void * b) {
	struct p_node *aa, *bb;
	aa = (struct p_node *) a; bb = (struct p_node *) b;
	
	int r = aa->up - bb->up;
	if (r > 0) return 1;
	else if (r < 0) return -1;
	/* if node.up values are same, compare node.low values */
	else return (aa->low - bb->low);
}

int line_gl (struct ogl *gl_ctx, int p0[2], int p1[2], int thick){
	/* emulate drawing a single line with thickness, using triangles in openGL */
	
	/* verify struct and buffers */
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
	
	/* get polar parameters of line */
	float dx = p1[0] - p0[0];
	float dy = p1[1] - p0[1];
	float modulus = sqrt(pow(dx, 2) + pow(dy, 2));
	float cosine = 1.0;
	float sine = 0.0;
	if (modulus > TOLERANCE){
		cosine = dx/modulus;
		sine = dy/modulus;
	}
	
	if (thick <= 0) thick = 1; /* one pixel as minimal thickness */
	
	/* convert input coordinates, in pixles (int), to openGL units */
	float tx = (float) thick / gl_ctx->win_w;
	float ty = (float) thick / gl_ctx->win_h;
	float x0 = ((float) p0[0] / gl_ctx->win_w) * 2.0 - 1.0;
	float y0 = ((float) p0[1] / gl_ctx->win_h) * 2.0 - 1.0;
	float x1 = ((float) p1[0] / gl_ctx->win_w) * 2.0 - 1.0;
	float y1 = ((float) p1[1] / gl_ctx->win_h) * 2.0 - 1.0;
	
	/* orientation in drawing area */
	float flip_y = (gl_ctx->flip_y) ? -1.0 : 1.0;
	
	/* store vertices - 4 vertices */
	/* 0 */
	int j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = x0 - sine * tx;
	gl_ctx->verts[j].pos[1] = flip_y * (y0 + cosine * ty);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 1 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = x0 + sine * tx;
	gl_ctx->verts[j].pos[1] = flip_y * (y0 - cosine * ty);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 2 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = x1 - sine * tx;
	gl_ctx->verts[j].pos[1] = flip_y * (y1 + cosine * ty);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 1.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 3 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = x1 + sine * tx;
	gl_ctx->verts[j].pos[1] = flip_y * (y1 - cosine * ty);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 1.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	
	/* store vertex indexes in elements buffer - 2 triangles that share vertices  */
	/* 0 */
	j = gl_ctx->elem_count * 3;
	gl_ctx->elems[j] = gl_ctx->vert_count - 4;
	gl_ctx->elems[j+1] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+2] = gl_ctx->vert_count - 2;
	/* 1 */
	gl_ctx->elems[j+3] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+4] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+5] = gl_ctx->vert_count - 1;
	gl_ctx->elem_count+= 2;
	
	return 1;
}

int quad_gl (struct ogl *gl_ctx, int tl[2], int bl[2], int tr[2], int br[2]){
	/* emulate drawing a filled and convex quadrilateral, using triangles in openGL */
	
	/* verify struct and buffers */
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
	
	/* orientation in drawing area */
	float flip_y = (gl_ctx->flip_y) ? -1.0 : 1.0;
	
	/* convert input coordinates, in pixles (int), to openGL units and store vertices - 4 vertices */
	/* 0 */
	int j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) bl[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) bl[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 1 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) tl[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) tl[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 2 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) br[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) br[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 1.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 3 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) tr[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) tr[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 1.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* store vertex indexes in elements buffer - 2 triangles that share vertices  */
	/* 0 */
	j = gl_ctx->elem_count * 3;
	gl_ctx->elems[j] = gl_ctx->vert_count - 4;
	gl_ctx->elems[j+1] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+2] = gl_ctx->vert_count - 2;
	/* 1 */
	gl_ctx->elems[j+3] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+4] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+5] = gl_ctx->vert_count - 1;
	gl_ctx->elem_count+= 2;
	
	return 1;
}

int triang_gl (struct ogl *gl_ctx, int p0[2], int p1[2], int p2[2]){
	/* add a triangle to openGL */
	
	/* verify struct and buffers */
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
		
	/* orientation in drawing area */
	float flip_y = (gl_ctx->flip_y) ? -1.0 : 1.0;
	
	/* convert input coordinates, in pixles (int), to openGL units and store vertices - 3 vertices */
	/* 0 */
	int j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) p0[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) p0[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 1 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) p1[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) p1[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	/* 2 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) p2[0] / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) p2[1] / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 1.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y);
	gl_ctx->vert_count ++;
	
	/* store vertex indexes in elements buffer - single element  */
	j = gl_ctx->elem_count * 3;
	gl_ctx->elems[j] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+1] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+2] = gl_ctx->vert_count - 1;
	gl_ctx->elem_count++;
	
	return 1;
}

int image_gl (struct ogl *gl_ctx, int x, int y, int w, int h, struct Image *img){
	/* emulate drawing a filled and convex quadrilateral, using triangles in openGL */
	
	/* verify struct and buffers */
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
	if (!img) return 0;
	
	/* orientation in drawing area */
	float flip_y = (gl_ctx->flip_y) ? -1.0 : 1.0;
	float scale_u = 1.0;
	float scale_v = 1.0;
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gl_ctx->tex);
	
	if (img->w > gl_ctx->tex_w || img->h > gl_ctx->tex_h){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
		scale_u = 1.0;
		scale_v = 1.0;
		gl_ctx->tex_w = img->w;
		gl_ctx->tex_h = img->h;
	}
	else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
		scale_u = (float) img->w / (float) gl_ctx->tex_w;
		scale_v = (float) img->h / (float) gl_ctx->tex_h;
	}
	
	/* convert input coordinates, in pixles (int), to openGL units and store vertices - 4 vertices */
	/* 0 */
	int j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) x / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) y / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y) * scale_v;
	gl_ctx->vert_count ++;
	/* 1 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) x / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) (y + h) / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = 0.0;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y) * scale_v;
	gl_ctx->vert_count ++;
	/* 2 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) (x + w) / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) y / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] = scale_u;
	gl_ctx->verts[j].uv[1] = (float)(gl_ctx->flip_y) * scale_v;
	gl_ctx->vert_count ++;
	/* 3 */
	j = gl_ctx->vert_count;
	gl_ctx->verts[j].pos[0] = ((float) (x + w) / gl_ctx->win_w) * 2.0 - 1.0;
	gl_ctx->verts[j].pos[1] = flip_y * (((float) (y + h) / gl_ctx->win_h) * 2.0 - 1.0);
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = gl_ctx->fg[0];
	gl_ctx->verts[j].col[1] = gl_ctx->fg[1];
	gl_ctx->verts[j].col[2] = gl_ctx->fg[2];
	gl_ctx->verts[j].col[3] = gl_ctx->fg[3];
	gl_ctx->verts[j].uv[0] =  scale_u;
	gl_ctx->verts[j].uv[1] = (float)(1 - gl_ctx->flip_y) * scale_v;
	gl_ctx->vert_count ++;
	/* store vertex indexes in elements buffer - 2 triangles that share vertices  */
	/* 0 */
	j = gl_ctx->elem_count * 3;
	gl_ctx->elems[j] = gl_ctx->vert_count - 4;
	gl_ctx->elems[j+1] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+2] = gl_ctx->vert_count - 2;
	/* 1 */
	gl_ctx->elems[j+3] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+4] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+5] = gl_ctx->vert_count - 1;
	gl_ctx->elem_count+= 2;
	
	return 1;
}

int polygon_gl (struct ogl *gl_ctx, int n, struct edge edges[]){
	/* draw a arbitrary and filled polygon, in openGL - use a scanline like algorithm */
	
	/* verify struct and buffers */
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
	
	if (2 * n > MAX_SCAN_LINES) return 0;
	
	int scan_lines[MAX_SCAN_LINES]; /* scan line defined by y coordinate */
	struct p_node nodes[MAX_P_NODES];
	
	int i = 0, j = 0, scan_count = 0, nodes_count = 0;
	
	/* define a scanline in each edge vertex */
	for (i = 0; i < n; i++){
		scan_lines[scan_count] = edges[i].y0;
		scan_count++;
		scan_lines[scan_count] = edges[i].y1;
		scan_count++;
	}
	
	/* sort and delete duplicates in scanline array */
	qsort(scan_lines, scan_count, sizeof(int), cmp_int);
	scan_count = unique (scan_count, scan_lines);
	
	/* perform polygon fill, by draw quads in scanline pairs */
	for (i = 1; i < scan_count; i++){ /* sweep scanlines by pairs */
		nodes_count = 0;
		
		for(j = 0; j < n; j++){ /* sweep polygon edges */
			if (((edges[j].y0 <= scan_lines[i - 1] && edges[j].y1 >= scan_lines[i - 1]) || 
			(edges[j].y1 <= scan_lines[i - 1] && edges[j].y0 >= scan_lines[i - 1])) &&
			((edges[j].y0 <= scan_lines[i] && edges[j].y1 >= scan_lines[i]) || 
			(edges[j].y1 <= scan_lines[i ] && edges[j].y0 >= scan_lines[i]))){
				/* find interceptions between edges and  current scanlines pair */
				
				/* up scanline */
				if (edges[j].y0 == scan_lines[i - 1]) { /*interception in one vertex */
					nodes[nodes_count].up = edges[j].x0;
				}
				else if (edges[j].y1 == scan_lines[i-1]) {  /*interception in other vertex */
					nodes[nodes_count].up = edges[j].x1;
				}
				else {  /* claculate interception in ege*/
					nodes[nodes_count].up = (int) round(edges[j].x1 + (double) (scan_lines[i - 1] - edges[j].y1)/(edges[j].y0 - edges[j].y1)*(edges[j].x0 - edges[j].x1));
				}
				
				/* lower scanline */
				if (edges[j].y0 == scan_lines[i]) { /*interception in one vertex */
					nodes[nodes_count].low = edges[j].x0;
				}
				else if (edges[j].y1 == scan_lines[i]) { /*interception in other vertex */
					nodes[nodes_count].low = edges[j].x1;
				}
				else { /* claculate interception in ege*/
					nodes[nodes_count].low = (int) round(edges[j].x1 + (double) (scan_lines[i] - edges[j].y1)/(edges[j].y0 - edges[j].y1)*(edges[j].x0 - edges[j].x1));
				}
				
				nodes_count++;
			}
		}
		
		/* sort interception nodes, by x coordinates */
		qsort(nodes, nodes_count, sizeof(struct p_node), cmp_node);
		
		/* draw quads (or triangles) between nodes pairs*/
		for (j = 1; j < nodes_count; j+= 2){ /* triangle, if up corners are same */
			if (nodes[j - 1].up == nodes[j].up){
				triang_gl (gl_ctx, (int[]){nodes[j - 1].up, scan_lines[i-1]},
					(int[]){nodes[j - 1].low, scan_lines[i]},
					(int[]){nodes[j].low, scan_lines[i]});
			}
			
			else if (nodes[j - 1].low == nodes[j].low){ /* triangle, if lower corners are same */
				triang_gl (gl_ctx, (int[]){nodes[j - 1].up, scan_lines[i-1]},
					(int[]){nodes[j - 1].low, scan_lines[i]},
					(int[]){nodes[j].up, scan_lines[i-1]});
			}
			else { /* general case - quadrilateral polygon */
				quad_gl (gl_ctx, (int[]){nodes[j - 1].up, scan_lines[i-1]},
					(int[]){nodes[j - 1].low, scan_lines[i]},
					(int[]){nodes[j].up, scan_lines[i-1]},
					(int[]){nodes[j].low, scan_lines[i]});
			}
		}
	}
	
	return 1;
}

int main(int argc, char *argv[])
{
	struct ogl gl_ctx;
	gl_ctx.vert_count = 0;
	gl_ctx.elem_count = 0;
	gl_ctx.verts = NULL;
	gl_ctx.elems = NULL;
	gl_ctx.win_w = 800;
	gl_ctx.win_h = 600;
	gl_ctx.flip_y = 0;
	gl_ctx.fg[0] = 255; gl_ctx.fg[1] = 255; gl_ctx.fg[2] = 255; gl_ctx.fg[3] = 255;
	gl_ctx.bg[0] = 100; gl_ctx.bg[1] = 100; gl_ctx.bg[2] = 100; gl_ctx.bg[3] = 255;
	
	gl_ctx.tex = 0;
	gl_ctx.tex_w = 20;
	gl_ctx.tex_h = 20;
	
	struct edge test_edges[] = {
		{500, 100, 300, 300},
		{300, 300, 300, 600},
		{300, 600, 200, 600},
		{200, 600, 100, 700},
		{100, 700, 500, 900},
		{500, 900, 800, 700},
		{800, 700, 900, 700},
		{900, 700, 900, 600},
		{900, 600, 700, 200},
		{700, 200, 500, 100},
		{600, 300, 500, 400},
		{500, 400, 650, 500},
		{650, 500, 600, 300}
	};
	
	struct edge test_edges2[] = {
		{20, 10, 50, 20},
		{50, 20, 70, 10},
		{70, 10, 80, 30},
		{80, 30, 70, 60},
		{70, 60, 30, 60},
		{30, 60, 10, 30},
		{10, 30, 20, 10},
		{30, 40, 40, 50},
		{40, 50, 40, 30},
		{40, 30, 30, 40},
		{60, 30, 65, 50},
		{65, 50, 70, 40},
		{70, 40, 60, 30}
	};
	
	/* buffer setup */
        GLsizei vs = sizeof(struct Vertex);
        size_t vp = offsetof(struct Vertex, pos);
        size_t vt = offsetof(struct Vertex, uv);
        size_t vc = offsetof(struct Vertex, col);
	
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	/* enable ati-aliasing */
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
	
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, gl_ctx.win_w, gl_ctx.win_h, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	/* Init GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	/* Create Vertex Array Object */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Create a Vertex Buffer Object and copy the vertex data to it */
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_TRIANG*3*vs, NULL, GL_STREAM_DRAW); //GL_STATIC_DRAW);
	
	/* Create a Element Buffer Object */
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_TRIANG*3*sizeof(GLuint), NULL,  GL_STREAM_DRAW);//GL_STATIC_DRAW); //GL_STREAM_DRAW
	
	/* Create and compile the vertex shader */
	const char* vertexSource = GLSL(
		in vec3 position;
		in vec2 uv;
		in vec4 color;
		out vec4 vertexColor;
		out vec2 texcoord;
	
		void main() {
			gl_Position = vec4(position, 1.0);
			vertexColor = color;
			texcoord = uv;
		}
	); /* =========== vertex shader */

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	/* Create and compile the fragment shader */
	const char* fragmentSource = GLSL(
		in vec4 vertexColor;
		in vec2 texcoord;
		
		out vec4 outColor;
		
		uniform sampler2D tex;

		void main() {
			outColor = texture(tex, texcoord) * vertexColor;
		}
	); /* ========== fragment shader */

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	/* Link the vertex and fragment shader into a shader program */
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
		
	/*texture */
	GLuint textures[2];
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	/* blank texture (default) */
	GLubyte blank[] = {255,255,255,255};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_ctx.tex_w, gl_ctx.tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	gl_ctx.tex = textures[1];
	
	struct Image bubble, checker;
	
	int img_w, img_h, num_ch;
	bubble.data = stbi_load("bubble.png", &bubble.w, &bubble.h, &num_ch, 4);
	
	/* Black/white checkerboard */
	GLubyte pixels[] = {
		0,0,0,255,    255,255,255,255,
		255,255,255,255,    0,0,0,255
	};
	checker.data = pixels;
	checker.w = 2; checker.h = 2;
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 1);

	/* Specify the layout of the vertex data */
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	GLint uvAttrib = glGetAttribLocation(shaderProgram, "uv");
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(uvAttrib);
	glEnableVertexAttribArray(colorAttrib);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/* load vertices/elements directly into vertex/element buffer */
        gl_ctx.verts = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        gl_ctx.elems = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	//glScissor(200,200,100,100);
	//glEnable(GL_SCISSOR_TEST);
	//glDisable(GL_SCISSOR_TEST);
	
	//polygon_gl (&gl_ctx, 13, test_edges);
	
	int i = 0, j;
	SDL_Event windowEvent;
	while (1) {
		
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
				if (windowEvent.button.button == SDL_BUTTON_LEFT){
					i = !i;
				}
				else if(windowEvent.button.button == SDL_BUTTON_RIGHT){
					gl_ctx.flip_y = !gl_ctx.flip_y;
				}
				
			}
		}
		
		//glBindTexture(GL_TEXTURE_2D, textures[1]);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 2, 2, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		
		/* load vertices/elements directly into vertex/element buffer */
		gl_ctx.verts = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		gl_ctx.elems = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		
		//i = !i;
		//glUniform1i(glGetUniformLocation(shaderProgram, "tex"), i);
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
		
		SDL_GetWindowSize(window, &gl_ctx.win_w, &gl_ctx.win_h);
		glViewport(0, 0, gl_ctx.win_w, gl_ctx.win_h);
		/* Clear the screen to black */
		glClearColor((GLfloat) gl_ctx.bg[0] /255, (GLfloat) gl_ctx.bg[1] /255, (GLfloat) gl_ctx.bg[2] /255, (GLfloat) gl_ctx.bg[3] /255);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//gl_ctx.fg[0] = gl_ctx.bg[0]; gl_ctx.fg[1] = gl_ctx.bg[1]; gl_ctx.fg[2] = gl_ctx.bg[2]; gl_ctx.fg[3] = gl_ctx.bg[3];
		//quad_gl (&gl_ctx, (int[]){0, 0},  (int[]){0, gl_ctx.win_h}, (int[]){gl_ctx.win_w, 0}, (int[]){gl_ctx.win_w, gl_ctx.win_h});
		
		gl_ctx.fg[0] = 255; gl_ctx.fg[1] = 0; gl_ctx.fg[2] = 0; gl_ctx.fg[3] = 150;
		line_gl(&gl_ctx, (int[]){5, 5}, (int[]){120, 100}, 2);
		gl_ctx.fg[0] = 255; gl_ctx.fg[1] = 0; gl_ctx.fg[2] = 255; gl_ctx.fg[3] = 150;
		line_gl(&gl_ctx, (int[]){50, 50}, (int[]){800, 600}, 0);
		gl_ctx.fg[0] = 255; gl_ctx.fg[1] = 255; gl_ctx.fg[2] = 0; gl_ctx.fg[3] = 150;
		line_gl(&gl_ctx, (int[]){5, 5}, (int[]){5, 100}, 3);
		gl_ctx.fg[0] = 0; gl_ctx.fg[1] = 0; gl_ctx.fg[2] = 255; gl_ctx.fg[3] = 150;
		line_gl(&gl_ctx, (int[]){800, 0}, (int[]){120, 200}, 20);
		
		gl_ctx.fg[0] = 0; gl_ctx.fg[1] = 255; gl_ctx.fg[2] = 0; gl_ctx.fg[3] = 255;
		for (j = 0; j < 13; j++){
			int p0[2], p1[2];
			p0[0] = test_edges[j].x0;
			p0[1] = test_edges[j].y0;
			p1[0] = test_edges[j].x1;
			p1[1] = test_edges[j].y1;
			
			line_gl(&gl_ctx, p0, p1, 2);
		}
		
		gl_ctx.fg[0] = 0; gl_ctx.fg[1] = 255; gl_ctx.fg[2] = 0; gl_ctx.fg[3] = 100;
		polygon_gl (&gl_ctx, 13, test_edges);
		
		gl_ctx.fg[0] = 0; gl_ctx.fg[1] = 0; gl_ctx.fg[2] = 255; gl_ctx.fg[3] = 100;
		polygon_gl (&gl_ctx, 13, test_edges2);
		
		

		/* Draw a triangle from the 3 vertices */
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, gl_ctx.elem_count*3, GL_UNSIGNED_INT, 0);
		
		gl_ctx.vert_count = 0;
		gl_ctx.elem_count = 0;
		
		/* load vertices/elements directly into vertex/element buffer */
		gl_ctx.verts = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		gl_ctx.elems = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		
		//glBindTexture(GL_TEXTURE_2D, textures[1]);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_w, img_h, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 1);
		gl_ctx.fg[0] = 255; gl_ctx.fg[1] = 255; gl_ctx.fg[2] = 255; gl_ctx.fg[3] = 255;
		if (i) image_gl (&gl_ctx, 50, 50, 200, 200, &bubble);
		else image_gl (&gl_ctx, 50, 50, 200, 200, &checker);
		
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(GL_TRIANGLES, gl_ctx.elem_count*3, GL_UNSIGNED_INT, 0);
		
		gl_ctx.vert_count = 0;
		gl_ctx.elem_count = 0;
		

		/* Swap buffers */
		SDL_GL_SwapWindow(window);
	}
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	
	/* Delete allocated resources */
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
	free(bubble.data);
	
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}