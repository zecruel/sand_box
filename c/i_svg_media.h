#ifndef _SVG_MEDIA
#define _SVG_MEDIA

#include "nanosvg.h"
#include "nanosvgrast.h"
#include "bmp.h"

enum {
    SVG_ARC,
    SVG_BRICK,
    SVG_CIRCLE,
    SVG_COPY,
    SVG_CURSOR,
    SVG_CUT,
    SVG_CZ,
    SVG_DOWN,
    SVG_DUPLI,
    SVG_ELIPSE,
    SVG_INFO,
    SVG_LAYERS,
    SVG_LEFT,
    SVG_LINE,
    SVG_MAGNET,
    SVG_MIRROR,
    SVG_MOVE,
    SVG_PASTE,
    SVG_PIC,
    SVG_PLINE,
    SVG_PRINT,
    SVG_RECT,
    SVG_RIGTH,
    SVG_ROT,
    SVG_SCALE,
    SVG_TEXT,
    SVG_TEXT_E,
    SVG_TRASH,
    SVG_UP,
    SVG_MEDIA_SIZE
} svg_list;

//char ** svg_media(void);

NSVGimage ** i_svg_all_curves(void);

void i_svg_free_curves(NSVGimage **curves);

bmp_img ** i_svg_all_bmp(NSVGimage **curves, int w, int h);

void i_svg_free_bmp(bmp_img **img);

#endif