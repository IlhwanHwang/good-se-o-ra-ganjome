#include <stdio.h>
#include <stdlib.h>
#include "stage2.h"
#include "stage3.h"
#include "timeline.h"
#include "action.h"
#include "enemy.h"
#include "util.h"
#include "frame.h"
#include "player.h"
#include "bullet.h"
#include "smanage.h"
#include "item.h"
#include "sprite.h"

timeline tl_stg2_main;
extern timeline tl_stg3_main;

static timeline nil;
static timeline board1;
static timeline and1;
static timeline nor1;
static timeline gwang1;

extern sprite spr_dgt;

extern sprite spr_board;
extern sprite spr_mango;
extern sprite spr_nand;
extern sprite spr_and;
extern sprite spr_nor;
extern sprite spr_gwang;
extern sprite spr_byellow;
extern sprite spr_bpink;

extern void (*bg_draw)();

static void to_stage3(void *self, void *aux) {
	smanage_next(&tl_stg3_main, stg3_draw);
	item_create(ITEM_LIFE, 200.0, 120.0);
}

static void create_board(void *self, void *aux) {
	enemy_create(&spr_board, &board1, NULL,
		420.0, (float)(int)aux, ENEMY_W_LARGE, ENEMY_H_MEDIUM, ENEMY_HEALTH_LARGE);
}

static void create_and(void *self, void *aux) {
	enemy_create(&spr_and, &and1, NULL,
		420.0, (float)(int)aux, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_SMALL);
}

static void create_nor(void *self, void *aux) {
	enemy_create(&spr_nor, &nor1, NULL,
		420.0, (float)(int)aux, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_LARGE);
}

static void create_nand(void *self, void *aux) {
	enemy *e = enemy_create(&spr_nand, &nil, NULL,
		(float)(rand() % (SCREENW - 2 * SCREENUNPAD) + SCREENUNPAD), -10.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_MEDIUM);

	e->vx = 0.0;
	e->ay = 0.2;
}

static void create_gwang(void *self, void *aux) {
	enemy_create(&spr_gwang, &gwang1, to_stage3,
		440.0, 120.0, 73.0, 96.0, 750);
}

static void create_and_bunch(void *self, void *aux) {
	int i;
	for (i = -60; i <= 60; i += 30)
		create_and(self, (void *)((int)aux + i));
}

static void create_nor_bunch(void *self, void *aux) {
	int i;
	for (i = -60; i <= 60; i += 30)
		create_nor(self, (void *)((int)aux + i));
}

static void create_nand_bunch(void *self, void *aux) {
	int i;
	for (i = 0; i < 5; i++)
		create_nand(self, aux);
}

static void board_shoot(void *self, void *aux) {
	enemy *e = (enemy *)self;
	bullet *b = bullet_create_enemy(&spr_mango, e->x, e->y, 0.0, 0.0, 15, 15, 0);
	bullet_towards_player(b, 0.2, 0.0);
	b->ax = b->vx;
	b->ay = b->vy;
	b->vx = 0.0;
	b->vy = 0.0;
}

static void gwang_mango_shoot(void *self, void *aux) {
	enemy *e = (enemy *)self;
	float f;
	int parity = 0;

	for (f = -CONST_PIHALF; f <= CONST_PIHALF; f += CONST_PI / 12.0) {
		bullet *b = bullet_create_enemy(&spr_mango, e->x, e->y, 6.5*cos(f), 6.5*sin(f), 10, 10, 0);
	}
}

static void gwang_and_shoot(void *self, void *aux) {
	enemy *e = (enemy *)self;
	int i;
	for (i = 0; i < 15; i++) {
		enemy *en = enemy_create(&spr_and, &nil, NULL,
			e->x, e->y, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_SMALL);
		en->ax = -0.4;
		en->vx = ((float)rand() / RAND_MAX) * 4.0;
		en->vy = (((float)rand() / RAND_MAX * 2) - 1.0) * 3.0;
	}
}

static void gwang_nand_shoot(void *self, void *aux) {
	enemy *e = (enemy *)self;
	float f;
	int parity = 0;

	for (f = -CONST_PIHALF/2; f <= CONST_PIHALF/2; f += CONST_PI / 24.0) {
		bullet *b = bullet_create_enemy(&spr_byellow, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		bullet_towards_player(b, 6.0, f);
		b = bullet_create_enemy(&spr_bpink, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		bullet_towards_player(b, 4.5, f);
		b = bullet_create_enemy(&spr_byellow, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		bullet_towards_player(b, 3.0, f);
	}
}

void stg2_init() {
	action *act;
	int i = 0;
	int frame = 0;

	//bg_draw = stg2_draw;

	tl_stg2_main.size = 80;
	tl_stg2_main.act = action_alloc(tl_stg2_main.size);
	act = tl_stg2_main.act;

	frame = 10; action_set(&act[i++], create_board, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_board, (void *)80, frame);
	action_set(&act[i++], create_board, (void *)160, frame);
	frame += 100; action_set(&act[i++], create_board, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_board, (void *)80, frame);
	action_set(&act[i++], create_board, (void *)160, frame);

	frame += 100; action_set(&act[i++], create_and_bunch, (void *)60, frame);
	frame += 40; action_set(&act[i++], create_and_bunch, (void *)180, frame);
	frame += 40; action_set(&act[i++], create_and_bunch, (void *)120, frame);

	frame += 80; action_set(&act[i++], create_and_bunch, (void *)60, frame);
	action_set(&act[i++], create_and_bunch, (void *)180, frame);
	frame += 20; action_set(&act[i++], create_and_bunch, (void *)120, frame);

	frame += 40; action_set(&act[i++], create_nor_bunch, (void *)60, frame);
	frame += 80; action_set(&act[i++], create_nor_bunch, (void *)180, frame);
	frame += 80; action_set(&act[i++], create_nor_bunch, (void *)60, frame);
	frame += 80; action_set(&act[i++], create_nor_bunch, (void *)180, frame);

	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 10; action_set(&act[i++], create_nand, (void *)60, frame);

	frame += 60; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);

	frame += 40; action_set(&act[i++], create_nor_bunch, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_nor_bunch, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_board, (void *)120, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)30, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)210, frame);
	frame += 60; action_set(&act[i++], create_board, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nor_bunch, (void *)60, frame);
	action_set(&act[i++], create_nor_bunch, (void *)180, frame);
	frame += 30; action_set(&act[i++], create_board, (void *)80, frame);
	action_set(&act[i++], create_board, (void *)160, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)120, frame);

	frame += 120; action_set(&act[i++], create_nor_bunch, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_nor_bunch, (void *)120, frame);
	frame += 80; action_set(&act[i++], create_board, (void *)120, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)30, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)210, frame);
	frame += 60; action_set(&act[i++], create_board, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nand, (void *)60, frame);
	frame += 5; action_set(&act[i++], create_nor_bunch, (void *)60, frame);
	action_set(&act[i++], create_nor_bunch, (void *)180, frame);
	frame += 30; action_set(&act[i++], create_board, (void *)80, frame);
	action_set(&act[i++], create_board, (void *)160, frame);
	frame += 10; action_set(&act[i++], create_and_bunch, (void *)120, frame);
	frame += 200;


	frame += 10; action_set(&act[i++], create_gwang, NULL, frame);

	//nil
	nil.size = 1;
	nil.act = action_alloc(nil.size);
	act = nil.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_void, NULL, frame);

	//board1
	board1.size = 5;
	board1.act = action_alloc(board1.size);
	act = board1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG8, frame);
	frame += 10; action_set(&act[i++], action_enemy_stop, NULL, frame);
	frame += 40; action_set(&act[i++], board_shoot, NULL, frame);
	frame += 20; action_set(&act[i++], action_enemy_horizontal, FLOATNEG1, frame);
	frame += 40; action_set(&act[i++], action_enemy_rewind, 21, frame);

	//and1
	and1.size = 1;
	and1.act = action_alloc(and1.size);
	act = and1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_towards_player, FLOAT6, frame);

	//nor1
	nor1.size = 1;
	nor1.act = action_alloc(nor1.size);
	act = nor1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG1, frame);

	//gwang1
	gwang1.size = 15;
	gwang1.act = action_alloc(gwang1.size);
	act = gwang1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG2, frame);
	frame += 60; action_set(&act[i++], action_enemy_stop, NULL, frame);
	frame += 60; action_set(&act[i++], gwang_and_shoot, NULL, frame);
	frame += 60; action_set(&act[i++], action_enemy_horizontal, FLOATNEG8, frame);
	frame += 30; action_set(&act[i++], action_enemy_stop, NULL, frame);
	action_set(&act[i++], gwang_mango_shoot, NULL, frame);
	frame += 10; action_set(&act[i++], gwang_mango_shoot, NULL, frame);
	frame += 10; action_set(&act[i++], gwang_mango_shoot, NULL, frame);
	frame += 20; action_set(&act[i++], action_enemy_horizontal, FLOAT8, frame);
	frame += 30; action_set(&act[i++], action_enemy_stop, NULL, frame);

	frame += 60; action_set(&act[i++], gwang_nand_shoot, NULL, frame);
	frame += 60; action_set(&act[i++], gwang_nand_shoot, NULL, frame);

	frame += 60; action_set(&act[i++], action_enemy_rewind, 60, frame);
}

void stg2_draw() {
	static float x = 0.0;
	float dx, dy, w, h;

	w = (float)(spr_dgt.w);
	h = (float)(spr_dgt.h);

	x -= 1.0;
	if (x < -w) {
		x += w;
	}

	for (dx = x; dx < (float)SCREENW; dx += w)
		for (dy = 0.0; dy < (float)SCREENH; dy += h)
			sprite_draw(&spr_dgt, dx, dy);
}
