#include <stdio.h>
#include "cbox.h"
#include "item.h"
#include "sprite.h"

item item_pool[ITEM_MAX];
static unsigned pool_seek = 0;

extern cbox cb_scr_unpad;
extern sprite spr_soytong;
extern sprite spr_ihi;
extern sprite spr_iflower;
extern sprite spr_life;
extern sprite spr_mountain;

void item_init() {
	int i;
	for (i = 0; i < ITEM_MAX; i++) {
		item_pool[i].type = -1;
		item_pool[i].cb.w = 16;
		item_pool[i].cb.h = 16;
	}
}

item *item_get_empty() {
	int i;
	for (i = 0; i < ITEM_MAX; i++) {
		int idx = pool_seek;
		pool_seek = (pool_seek + 1) % ITEM_MAX;
		if (item_pool[idx].type == -1) {
			return &item_pool[idx];
		}
	}
	return NULL;
}

void item_create(int type, float x, float y) {
	item *it = item_get_empty();

	if (it == NULL) {
		printf("Nothing left in item pool!!\n");
		return;
	}

	printf("Item pooled %d, %d\n", (int)x, (int)y);

	it->type = type;
	it->span = 600;
	it->x = x;
	it->y = y;
	it->vx = rand() % 2 ? 2.0 : -2.0;
	it->vy = 2.0;
}

void item_update() { 
	int i;
	for (i = 0; i < ITEM_MAX; i++) {
		item *it = &item_pool[i];
		if (it->type != -1) {
			it->x += it->vx;
			it->y += it->vy;

			if (cb_scr_unpad.x1 > it->x) {
				it->x = cb_scr_unpad.x1;
				it->vx = -(it->vx);
			}

			if (cb_scr_unpad.x2 < it->x) {
				it->x = cb_scr_unpad.x2;
				it->vx = -(it->vx);
			}

			if (cb_scr_unpad.y1 > it->y) {
				it->y = cb_scr_unpad.y1;
				it->vy = -(it->vy);
			} 

			if (cb_scr_unpad.y2 < it->y) {
				it->y = cb_scr_unpad.y2;
				it->vy = -(it->vy);
			}

			cbox_refresh_centered(&it->cb, it->x, it->y);

			if (it->span-- <= 0)
				it->type = -1;
		}
	}
}

void item_delete(int i) {
	item *it = &item_pool[i];
	it->type = -1;
}

static sprite *item_get_sprite(int type) {
	switch (type) {
	case ITEM_SOY: return &spr_soytong;
	case ITEM_HA: return &spr_ihi;
	case ITEM_FLOWER: return &spr_iflower;
	case ITEM_LIFE: return &spr_life;
	}
	return &spr_mountain;
}

void item_draw() {
	int i;
	for (i = 0; i < ITEM_MAX; i++) {
		item *it = &item_pool[i];
		if (it->type != -1) {
			if ((it->span > 60) || (it->span % 2))
				sprite_draw(item_get_sprite(it->type), it->x, it->y);
		}
	}
}