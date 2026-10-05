#pragma once

typedef int color_t;

#define DWIDTH 396
#define DHEIGHT 224
#define C_RGB(r,g,b) ((color_t)0)
#define C_WHITE ((color_t)0)

void dclear(color_t color);
void drect(int x1, int y1, int x2, int y2, color_t color);
void dtext(int x, int y, color_t color, char const *text);
void dupdate(void);
