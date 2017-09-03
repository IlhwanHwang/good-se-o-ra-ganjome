#include <stdio.h>
#include "stage1.h"
#include "stage2.h"
#include "timeline.h"
#include "action.h"
#include "enemy.h"
#include "util.h"
#include "frame.h"
#include "player.h"
#include "bullet.h"
#include "smanage.h"
#include "item.h"

timeline tl_stg1_main;
extern timeline tl_stg2_main;

static timeline soonpink1;
static timeline soonpink2;
static timeline soonyellow1;
static timeline mountain1;
static timeline partner;
static timeline ddabong;

extern sprite spr_soonpink;
extern sprite spr_soonyellow;
extern sprite spr_mountain;
extern sprite spr_partner;
extern sprite spr_ddabong;

extern sprite spr_pixelsky;

extern sprite spr_bpink;
extern sprite spr_byellow;

extern void (*bg_draw)();
extern unsigned *scr_target;

static void to_stage2(void *self) {
	smanage_next(&tl_stg2_main, stg2_draw);
	item_create(ITEM_LIFE, 200.0, 120.0);
}

static void create_soonpink1(void *self, void *aux) {
	if ((int)aux == 0) {
		enemy_create(&spr_soonpink, &soonpink1, NULL,
			420.0, 60.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_SMALL);
	}
	else {
		enemy_create(&spr_soonpink, &soonpink2, NULL,
			420.0, 180.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_SMALL);
	}
}

static void create_soonyellow1(void *self, void *aux) {
	if ((int)aux == 0) {
		enemy_create(&spr_soonyellow, &soonyellow1, NULL,
			460.0, 90.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_MEDIUM);
	}
	else {
		enemy_create(&spr_soonyellow, &soonyellow1, NULL,
			460.0, 150.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_MEDIUM);
	}
}

static void create_mountain1(void *self, void *aux) {
	enemy_create(&spr_mountain, &mountain1, NULL,
		460.0, (float)(int)aux, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_LARGE);
}

static void create_partner(void *self, void *aux) {
	enemy_create(&spr_partner, &partner, to_stage2,
		440.0, 120.0, 73.0, 96.0, 500);
}

static void partner1(void *self, void *aux) {
	enemy * e = enemy_create(&spr_ddabong, &ddabong, NULL,
		420.0, (float)(int)aux, 22, 34, 100);

	e->ax = -0.2;
	e->vx = -1.0;
}

static void ddabong1(void *self, void *aux) {
	static float phase = 0.0;
	enemy *e = (enemy *)self;
	float f;
	int parity = 0;

	phase += 0.5;
	for (f = 0.0; f < CONST_PIDOUBLE; f += CONST_PI / 6.0) {
		sprite *spr = parity ? &spr_byellow : &spr_bpink;
		bullet *b;
		parity = !parity;
		b = bullet_create_enemy(spr, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		b->ax = 0.15 * cos(f+phase);
		b->ay = 0.2 * sin(f+phase);
	}
}

static void bpink_towards(void *self, void *aux) {
	enemy *e = (enemy *)self;
	bullet *b = bullet_create_enemy(&spr_bpink, e->x, e->y, 0.0, 0.0, 10, 10, 0);
	bullet_towards_player(b, 4.0, 0.0);
}

static void byellow_towards(void *self, void *aux) {
	enemy *e = (enemy *)self;
	bullet *b = bullet_create_enemy(&spr_byellow, e->x, e->y, 0.0, 0.0, 10, 10, 0);
	bullet_towards_player(b, 4.0, 0.0);
	b = bullet_create_enemy(&spr_byellow, e->x, e->y, 0.0, 0.0, 10, 10, 0);
	bullet_towards_player(b, 4.0, 0.25);
	b = bullet_create_enemy(&spr_byellow, e->x, e->y, 0.0, 0.0, 10, 10, 0);
	bullet_towards_player(b, 4.0, -0.25);
}

static void bmountain_shoot(void *self, void *aux) {
	enemy *e = (enemy *)self;
	float f;
	int parity = 0;
	for (f = 0.0; f < CONST_PIDOUBLE; f += CONST_PI * 0.125) {
		sprite *spr = parity ? &spr_byellow : &spr_bpink;
		parity = !parity;
		bullet_create_enemy(spr, e->x, e->y, 3.0 * cos(f), 3.0 * sin(f), 10, 10, 0);
	}
}

void stg1_init() {
	action *act;
	int i = 0;
	int frame = 0;

	bg_draw = stg1_draw;

	tl_stg1_main.size = 60;
	tl_stg1_main.act = action_alloc(tl_stg1_main.size);
	act = tl_stg1_main.act;


	frame = 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);

	frame += 90; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);

	frame += 120; action_set(&act[i++], create_mountain1, (void *)120, frame);

	frame += 90; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);

	frame += 90; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonyellow1, (void *)0, frame);
	action_set(&act[i++], create_soonyellow1, (void *)1, frame);

	frame += 90; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);
	frame += 10; action_set(&act[i++], create_soonpink1, (void *)0, frame);
	action_set(&act[i++], create_soonpink1, (void *)1, frame);

	frame += 120; action_set(&act[i++], create_mountain1, (void *)60, frame);
	action_set(&act[i++], create_mountain1, (void *)180, frame);
	frame += 90;

	frame += 30; action_set(&act[i++], create_partner, NULL, frame);

	//soonpink1
	soonpink1.size = 4;
	soonpink1.act = action_alloc(soonpink1.size);
	act = soonpink1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG4, frame);
	frame += 40; action_set(&act[i++], action_enemy_change_direction, FLOAT45RAD, frame);
	action_set(&act[i++], bpink_towards, NULL, frame);
	frame += 20; action_set(&act[i++], action_enemy_horizontal, FLOATNEG4, frame);

	//soonpink2
	soonpink2.size = 4;
	soonpink2.act = action_alloc(soonpink2.size);
	act = soonpink2.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG4, frame);
	frame += 40; action_set(&act[i++], action_enemy_change_direction, FLOAT315RAD, frame);
	action_set(&act[i++], bpink_towards, NULL, frame);
	frame += 20; action_set(&act[i++], action_enemy_horizontal, FLOATNEG4, frame);

	//soonyellow1
	soonyellow1.size = 2;
	soonyellow1.act = action_alloc(soonyellow1.size);
	act = soonyellow1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_towards_player, FLOAT2, frame);
	frame += 40; action_set(&act[i++], byellow_towards, NULL, frame);

	//mountain
	mountain1.size = 3;
	mountain1.act = action_alloc(mountain1.size);
	act = mountain1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG2, frame);
	frame += 40; action_set(&act[i++], bmountain_shoot, NULL, frame);
	frame += 40; action_set(&act[i++], action_enemy_rewind, NULL, frame);

	//partner
	partner.size = 7;
	partner.act = action_alloc(partner.size);
	act = partner.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG2, frame);
	frame += 60; action_set(&act[i++], action_enemy_stop, NULL, frame);
	//frame += 60; action_set(&act[i++], action_enemy_vertical, FLOATNEG1, frame);
	frame += 40; action_set(&act[i++], partner1, (void *)60, frame);
	frame += 40; action_set(&act[i++], partner1, (void *)180, frame);
	frame += 80; action_set(&act[i++], partner1, (void *)200, frame);
	frame += 40; action_set(&act[i++], partner1, (void *)40, frame);
	frame += 40; action_set(&act[i++], action_enemy_rewind, (void *)60, frame);

	//ddabong
	ddabong.size = 3;
	ddabong.act = action_alloc(ddabong.size);
	act = ddabong.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG2, frame);
	frame += 20; action_set(&act[i++], ddabong1, NULL, frame);
	frame += 20; action_set(&act[i++], action_enemy_rewind, (void *)15, frame);
}

void stg1_draw() {
	unsigned *src = spr_pixelsky.data;
	gfx_bitblck_ext(scr_target, src,
		0, 0, SCREENW, SCREENH,
		SCREENW, SCREENH,
		0, 0, 1, SCREENH,
		1, SCREENH);
}