#ifndef _DXF_HACTH_LIB
#define _DXF_HACTH_LIB

#include "dxf.h"

struct hatch_line {
	double ang, ox, oy, dx, dy;
	double dash[20];
	int num_dash;
	struct hatch_line *next;
};

struct h_pattern {
	char name[DXF_MAX_CHARS];
	char descr[DXF_MAX_CHARS];
	int num_lines;
	struct hatch_line *lines;
	struct h_pattern *next;
};

int dxf_parse_patt(char *buf, struct h_pattern *ret);
int dxf_hatch_free (struct h_pattern *hatch);

#endif