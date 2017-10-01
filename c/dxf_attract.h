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
	ATRC_KEY = 8,
	ATRC_ANY = 16,
	ATRC_INS = 32,
	ATRC_CTRL = 64
};

int dxf_ent_attract (dxf_node * obj, enum attract_type type,
double pos_x, double pos_y, double sensi, double *ret_x, double *ret_y);

#endif