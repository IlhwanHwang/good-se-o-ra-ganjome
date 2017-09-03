#ifndef ITEM
#define ITEM

#define ITEM_MAX 8
#define ITEM_SOY 1
#define ITEM_HA 2
#define ITEM_FLOWER 3
#define ITEM_LIFE 4

#include "cbox.h"

typedef struct {
	int type;
	int span;
	cbox cb;
	float x, y, vx, vy;
} item;

void item_init();
item *item_get_empty();
void item_create(int type, float x, float y);
void item_update();
void item_delete(int i);
void item_draw();

#endif