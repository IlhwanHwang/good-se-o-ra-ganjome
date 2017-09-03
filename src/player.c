#include <stdio.h>
#include "player.h"
#include "pad.h"
#include "util.h"
#include "frame.h"
#include "sprite.h"
#include "bullet.h"
#include "cbox.h"
#include "enemy.h"
#include "item.h"
#include "effect.h"

static float x;
static float y;
static int cooldown = 0;
static int life = 5;
static int type = ITEM_SOY;
static int invinc = 60;
static int over = 0;
static cbox cb = {10.0, 10.0};

extern bulletpool ebullet_pool;
extern enemy enemy_pool[ENEMY_MAX];
extern item item_pool[ITEM_MAX];

extern sprite spr_ganjome;

extern sprite spr_bbrown;
extern sprite spr_hi_red;
extern sprite spr_hi_orange;
extern sprite spr_hi_yellow;
extern sprite spr_hi_green;
extern sprite spr_hi_blue;
extern sprite spr_hi_navy;
extern sprite spr_hi_purple;
extern sprite spr_bflower;

extern sprite spr_life;

void player_init() {
	x = (float)(SCREENW / 6);
	y = (float)(SCREENH / 2);
	life = 5;
	invinc = 60;
	over = 0;
	type = ITEM_SOY;
	cooldown = 0;
}

static void shoot_bbrown() {
	bullet_create_player(&spr_bbrown, x, y, 25, 0, 30, 20, 3);
	bullet_create_player(&spr_bbrown, x, y, 23, 2, 30, 20, 2);
	bullet_create_player(&spr_bbrown, x, y, 23, -2, 30, 20, 2);
	cooldown = 5;
}

static void shoot_hi() {
	static float phase = 0.0;
	static int col = 0;
	sprite *spr;
	float angle = 0.2 * sin(phase);

	phase += 0.8;
	col = (col + 1) % 7;

	switch (col) {
		case 0: spr = &spr_hi_red; break;
		case 1: spr = &spr_hi_orange; break;
		case 2: spr = &spr_hi_yellow; break;
		case 3: spr = &spr_hi_green; break;
		case 4: spr = &spr_hi_blue; break;
		case 5: spr = &spr_hi_navy; break;
		case 6: spr = &spr_hi_purple; break;
	}

	bullet_create_player(spr, x, y, 20.0 * cos(angle), 20.0 * sin(angle), 20, 20, 3);
	cooldown = 2;
}

static void shoot_bflower() {
	static float phase = 0.0;
	bullet *b;

	phase += 0.5;
	b = bullet_create_player(&spr_bflower, x, y, 0.0, 0.0, 20, 20, 2);
	b->ax = 0.7*cos(phase);
	b->ay = 0.7*sin(phase);
	cooldown = 1;
}

void player_update() {
	int i;

	if (over)
		return;

	x += pad_get_vx();
	y += pad_get_vy();

	x = fclamp(x, (float)(PLAYERPADDING), (float)(SCREENW - PLAYERPADDING));
	y = fclamp(y, (float)(PLAYERPADDING), (float)(SCREENH - PLAYERPADDING));

	cbox_refresh_centered(&cb, x, y);

	if (cooldown > 0)
		cooldown--;

	if (touch_is_valid() && cooldown <= 0) {
		switch (type) {
			case ITEM_SOY: shoot_bbrown(); break;
			case ITEM_HA: shoot_hi(); break;
			case ITEM_FLOWER: shoot_bflower(); break;
		}
	}

	for (i = 0; i < ITEM_MAX; i++) {
		item *it = &item_pool[i];
		if (it->type != -1) {
			if (cbox_collide(&cb, &it->cb)) {
				switch (it->type) {
				case ITEM_SOY:
				case ITEM_HA:
				case ITEM_FLOWER:
					type = it->type;
					break;
				case ITEM_LIFE:
					life++;
					break;
				}
				effect_large(x, y);
				item_delete(i);
			}
		}
	}

	if (invinc <= 0) {
		for (i = 0; i < ebullet_pool.size; i++) {
			bullet *b = &ebullet_pool.b_array[i];
			if (b->valid) {
				if (cbox_collide(&cb, &b->cb)) {
					life--;
					invinc = 60;
					printf("Collide with bullet\n");
					break;
				}
			}
		}

		if (invinc <= 0) {
			for (i = 0; i < ENEMY_MAX; i++) {
				enemy *e = &enemy_pool[i];
				if (e->valid) {
					if (cbox_collide(&cb, &e->cb)) {
						life--;
						invinc = 60;
						printf("Collide with enemy\n");
						break;
					}
				}
			}
		}

		if (invinc > 0) {
			for (i = 0; i < ebullet_pool.size; i++) {
				bullet *b = &ebullet_pool.b_array[i];
				if (b->valid) {
					b->ef = 1;
					bullet_delete(&ebullet_pool, i);
				}
			}

			for (i = 0; i < ENEMY_MAX; i++) {
				enemy *e = &enemy_pool[i];
				if (e->valid) {
					e->ef = 1;
					enemy_damage(i, 10);
				}
			}

			if (life <= 0) {
				effect_large(x, y);
				over = 1;
			}
		}
	}
	else {
		invinc--;
	}
}

void player_draw() {
	if (!(invinc % 2) && !over)
		sprite_draw(&spr_ganjome, x, y);
}

void player_life_draw() {
	int i;
	for (i = 0; i < life; i++) {
		sprite_draw(&spr_life, 13 + 20 * i, 12);
	}
}

inline float player_get_x() {
	return x;
}

inline float player_get_y() {
	return y;
}

inline int player_get_life() {
	return life;
}