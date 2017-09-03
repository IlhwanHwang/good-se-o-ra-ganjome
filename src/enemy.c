#include <stdio.h>
#include <stdlib.h>
#include "enemy.h"
#include "timeline.h"
#include "sprite.h"
#include "bullet.h"
#include "effect.h"
#include "item.h"

enemy enemy_pool[ENEMY_MAX];
static unsigned pool_seek = 0;
static unsigned pool_count = 0;

extern sprite spr_soonyellow;
extern timeline t_test;

extern bulletpool pbullet_pool;

extern cbox cb_scr_pad;

void enemy_init() {
	int i;
	for (i = 0; i < ENEMY_MAX; i++) {
		enemy_pool[i].valid = 0;
		enemy_pool[i].destroy = 0;
		enemy_pool[i].ef = 0;
	}
	printf("Enemy initialized.\n");
}

enemy *enemy_find_empty() {
	int i;

	for (i = 0; i < ENEMY_MAX; i++) {
		int idx = pool_seek++;
		pool_seek %= ENEMY_MAX;

		if(!enemy_pool[idx].valid)
			return &enemy_pool[idx];
	}

	return NULL;
}

enemy *enemy_create(sprite *spr, timeline *tl, void (*dest)(void *),
	float x, float y, float w, float h, int health) {
	enemy *e = enemy_find_empty();

	if (e == NULL) {
		printf("Nothing left in enemy pool!!\n");
		return e;
	}

	e->spr = spr;
	e->dest = dest;

	e->ax = 0.0;
	e->ay = 0.0;
	e->x = x;
	e->y = y;
	e->vx = 0.0;
	e->vy = 0.0;
	e->cb.w = w;
	e->cb.h = h;
	cbox_refresh_centered(&e->cb, e->x, e->y);

	e->w.tl = tl;
	e->w.seek = 0;
	e->w.fcnt = 0;

	e->health = health;
	e->item_prob = health / 2;

	e->valid = 1;
	pool_count++;

	return e;
}

int enemy_damage(int i, int power) {
	enemy *e = &enemy_pool[i];

	e->health -= power;
	if (e->health <= 0) {
		e->ef = 1;
		enemy_delete(i);
		return 1;
	}

	return 0;
}

void enemy_update() {
	int i;

	for (i = 0; i < ENEMY_MAX; i++) {
		if (enemy_pool[i].valid) {
			enemy *e = &enemy_pool[i];
			int j;

			watch_update(&e->w, (void *)e);
			e->vx += e->ax;
			e->vy += e->ay;
			e->x += e->vx;
			e->y += e->vy;
			cbox_refresh_centered(&e->cb, e->x, e->y);

			if (e->destroy || !cbox_inclusion(&cb_scr_pad, e->x, e->y)) {
				enemy_delete(i);
				continue;
			}

			for (j = 0; j < pbullet_pool.size; j++) {
				bullet *b = &pbullet_pool.b_array[j];
				if (b->valid) {
					if (cbox_collide(&e->cb, &b->cb)) {
						bullet_delete(&pbullet_pool, j);
						if (enemy_damage(i, b->power))
							break;
					}
				}
			}
		}
	}
}

void enemy_delete(int i) {
	enemy *e = &enemy_pool[i];
	if (e->ef) {
		effect_large(e->x, e->y);
		if (rand() % 100 < e->item_prob)
			item_create(rand() % 3 + 1, e->x, e->y);
	}
	if (e->dest != NULL)
		(e->dest)((void *)e);
	e->ef = 0;
	e->ax = 0.0;
	e->ay = 0.0;
	e->valid = 0;
	e->destroy = 0;
	pool_count--;
}

void enemy_draw() {
	int i;

	for (i = 0; i < ENEMY_MAX; i++) {
		if (enemy_pool[i].valid) {
			enemy *e = &enemy_pool[i];
			sprite_draw(e->spr, e->x, e->y);
		}
	}
}