#include "cbox.h"


inline int cbox_inclusion(cbox *cb, float x, float y) {
	return (cb->x1 <= x && cb->x2 > x) && (cb->y1 <= y && cb->y2 > y);
}

void cbox_refresh(cbox *cb, float x, float y) {
	cb->x1 = x;
	cb->y1 = y;
	cb->x2 = x + cb->w;
	cb->y2 = y + cb->h;
}

void cbox_refresh_centered(cbox *cb, float x, float y) {
	cb->x1 = x - cb->w*0.5;
	cb->y1 = y - cb->h*0.5;
	cb->x2 = x + cb->w*0.5;
	cb->y2 = y + cb->h*0.5;
}

int cbox_collide(cbox *c1, cbox *c2) {
	return c1->x1 <= c2->x2 && c1->x2 >= c2->x1 && 
		c1->y1 <= c2->y2 && c1->y2 >= c2->y1;
}