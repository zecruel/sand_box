#ifndef _DXF_ATTRACT_LIB
#define _DXF_ATTRACT_LIB

#include "dxf.h"
#include <math.h>

enum attract_type {
	/* bit coded */
	ATRC_NONE = 0,
	ATRC_END = 1,
	ATRC_MID = 2,
	ATRC_CENTER = 4,
	ATRC_QUAD = 8,
	ATRC_INTER = 16,
	ATRC_PERP = 32,
	ATRC_INS = 64,
	ATRC_CTRL = 128,
	ATRC_KEY = 256,
	ATRC_ANY = 512
};

struct ins_space{
	dxf_node *ins_ent, *prev;
	double ofs_x, ofs_y, ofs_z;
	double rot, scale_x, scale_y, scale_z;
	double normal[3];
};

struct inter_obj{
	int type;
	union {
		struct {
			double p1x;
			double p1y;
			double p2x;
			double p2y;
			double bulge;
		} line;
		struct {
			double p1x;
			double p1y;
			double p2x;
			double p2y;
			double cx;
			double cy;
			double radius;
		} arc;
	};
};

int dxf_ent_attract (dxf_drawing *drawing, dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y);

#endif