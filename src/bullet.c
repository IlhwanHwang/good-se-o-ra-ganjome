#include <stdio.h>
#include "bullet.h"
#include "cbox.h"
#include "sprite.h"
#include "player.h"
#include "util.h"
#include "effect.h"

bullet enemy_bullet_array[EBULLET_MAX];
bullet player_bullet_array[PBULLET_MAX];

bulletpool ebullet_pool = {enemy_bullet_array, 0, EBULLET_MAX};
bulletpool pbullet_pool = {player_bullet_array, 0, PBULLET_MAX};

extern cbox cb_screen;

void bullet_init() {
	int i;

	for (i = 0; i < ebullet_pool.size; i++)
		ebullet_pool.b_array[i].valid = 0;

	for (i = 0; i < pbullet_pool.size; i++)
		pbullet_pool.b_array[i].valid = 0;
}

bullet* bullet_find_empty(bulletpool *bp) {
	int i;
	for (i = 0; i < bp->size; i++) {
		if (!bp->b_array[bp->seek].valid)
			return &bp->b_array[bp->seek];
		bp->seek++;
		bp->seek %= bp->size;
	}
	return NULL;
}

bullet* bullet_create_enemy(sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power) {
	return bullet_create(&ebullet_pool, spr, x, y, vx, vy, w, h, power);
}

bullet* bullet_create_player(sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power) {
	return bullet_create(&pbullet_pool, spr, x, y, vx, vy, w, h, power);
}

bullet* bullet_create(bulletpool *bp, sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power) {
	bullet *b = bullet_find_empty(bp);

	if (b == NULL) {
		printf("No bullet pool left!!\n");
		return NULL;
	}

	b->spr = spr;

	b->ax = 0.0;
	b->ay = 0.0;
	b->x = x;
	b->y = y;
	b->vx = vx;
	b->vy = vy;
	b->ef = 0;

	b->cb.w = w;
	b->cb.h = h;
	cbox_refresh_centered(&b->cb, x, y);

	b->power = power;

	b->valid = 1;
	bp->count++;

	return b;
}

void bullet_update(bulletpool *bp) {
	int i;
	for (i = 0; i < bp->size; i++) {
		bullet *b = &bp->b_array[i];
		if (b->valid) {
			b->x += b->vx;
			b->y += b->vy;
			b->vx += b->ax;
			b->vy += b->ay;
			cbox_refresh_centered(&b->cb, b->x, b->y);

			if (!cbox_inclusion(&cb_screen, b->x, b->y)) {
				bullet_delete(bp, i);
			}
		}
	}
}

void bullet_update_master() {
	bullet_update(&pbullet_pool);
	bullet_update(&ebullet_pool);
}

void bullet_delete(bulletpool *bp, int i) {
	bullet *b = &bp->b_array[i];
	if (b->ef) {
		effect_small(b->x, b->y);
	}
	b->ef = 0;
	b->valid = 0;
	b->ax = 0.0;
	b->ay = 0.0;
	bp->count--;
}

void bullet_draw(bulletpool *bp) {
	int i;
	for (i = 0; i < bp->size; i++) {
		bullet *b = &bp->b_array[i];
		if (b->valid) {
			sprite_draw(b->spr, b->x, b->y);
		}
	}
}

void bullet_draw_master() {
	bullet_draw(&pbullet_pool);
	bullet_draw(&ebullet_pool);
}

void bullet_towards_player(bullet *b, float v, float dir) {
	 float dx, dy, d, dx2, dy2;
	 dx = player_get_x() - b->x;
	 dy = player_get_y() - b->y;
	 d = v * Q_rsqrt(dx*dx + dy*dy);
	 dx2 = dx * cos(dir) - dy * sin(dir);
	 dy2 = dy * cos(dir) + dx * sin(dir);
	 b->vx = dx2 * d;
	 b->vy = dy2 * d;
}