#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"
#include "stage3.h"
#include "timeline.h"
#include "action.h"
#include "enemy.h"
#include "util.h"
#include "frame.h"
#include "player.h"
#include "bullet.h"
#include "smanage.h"

timeline tl_stg3_main;

static timeline bamsoo1;
static timeline smilesoo1;
static timeline angrysoo1;

extern sprite spr_bamsoo;
extern sprite spr_smilesoo;
extern sprite spr_angrysoo;
extern sprite spr_mountain;
extern sprite spr_app;

extern sprite spr_bpink;
extern sprite spr_byellow;

extern void (*bg_draw)();
extern unsigned *scr_target;

extern int endcount;

static void finish() {
	endcount = 1;
}

static void create_bamsoo(void *self, void *aux) {
	enemy_create(&spr_bamsoo, &bamsoo1, finish,
		440.0, 120.0, 73.0, 96.0, 1000);
}

static void shoot_app_much(void *self, void *aux) {
	static float phase = 0.0;
	float f;
	enemy *e = (enemy *)self;

	phase += CONST_PI / 48.0;

	for (f = 0.0; f < CONST_PIDOUBLE; f += CONST_PI / 12.0) {
		bullet *b = bullet_create_enemy(&spr_app, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		b->ax = 0.3*cos(f+phase);
		b->ay = 0.3*sin(f+phase);
	}
}

static void shoot_app_very_much(void *self, void *aux) {
	enemy *e = (enemy *)self;
	int i;

	for (i = 0; i < 40; i++) {
		bullet *b = bullet_create_enemy(&spr_app, e->x, e->y, 0.0, 0.0, 10, 10, 0);
		b->vx = -((float)rand() / RAND_MAX) * 8.0 - 3.0;
		b->vy = -((float)rand() / RAND_MAX) * 6.0;
		b->ay = 0.2;
		b->ax = -0.05;
	}
}

static void spawn_smilesoo(void *self, void *aux) {
	enemy *e = enemy_create(&spr_smilesoo, &smilesoo1, NULL,
		440.0, 0.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_LARGE);
	e->vx = -3.0;
	e->vy = 2.0;
}

static void spawn_angrysoo(void *self, void *aux) {
	enemy *e = enemy_create(&spr_angrysoo, &angrysoo1, NULL,
		440.0, 240.0, ENEMY_W_SMALL, ENEMY_H_SMALL, ENEMY_HEALTH_LARGE);
	e->vx = -3.0;
	e->vy = -2.0;
}

static void shoot_yellow_spiral(void *self, void *aux) {
	static float phase = 0.0;
	enemy *e = (enemy *)self;

	phase += CONST_PI / 17.0;

	bullet *b = bullet_create_enemy(&spr_byellow, e->x, e->y, 4.0*cos(phase), 4.0*sin(phase), 10, 10, 0);
}

static void shoot_pink_spiral(void *self, void *aux) {
	static float phase = 0.0;
	enemy *e = (enemy *)self;

	phase -= CONST_PI / 13.0 + 0.001;
	if (phase < 0)
		phase += CONST_PIDOUBLE;

	bullet *b = bullet_create_enemy(&spr_bpink, e->x, e->y, 4.0*cos(phase), 4.0*sin(phase), 10, 10, 0);
}

void stg3_init() {
	action *act;
	int i = 0;
	int frame = 0;

	//bg_draw = stg3_draw;

	tl_stg3_main.size = 1;
	tl_stg3_main.act = action_alloc(tl_stg3_main.size);
	act = tl_stg3_main.act;

	frame += 10; action_set(&act[i++], create_bamsoo, NULL, frame);

	//bamsoo1
	bamsoo1.size = 15;
	bamsoo1.act = action_alloc(bamsoo1.size);
	act = bamsoo1.act;
	i = 0;

	frame = 0; action_set(&act[i++], action_enemy_horizontal, FLOATNEG2, frame);
	frame += 60; action_set(&act[i++], action_enemy_stop, NULL, frame);

	frame += 60; action_set(&act[i++], shoot_app_much, NULL, frame);
	frame += 3; action_set(&act[i++], shoot_app_much, NULL, frame);
	frame += 3; action_set(&act[i++], shoot_app_much, NULL, frame);
	frame += 3; action_set(&act[i++], shoot_app_much, NULL, frame);
	frame += 3; action_set(&act[i++], shoot_app_much, NULL, frame);

	frame += 60; action_set(&act[i++], spawn_smilesoo, NULL, frame);
	frame += 60; action_set(&act[i++], spawn_angrysoo, NULL, frame);

	frame += 60; action_set(&act[i++], shoot_app_very_much, NULL, frame);

	frame += 60; action_set(&act[i++], action_enemy_rewind, 60, frame);

	//smilesoo
	smilesoo1.size = 2;
	smilesoo1.act = action_alloc(smilesoo1.size);
	act = smilesoo1.act;
	i = 0;

	frame = 11; action_set(&act[i++], shoot_yellow_spiral, NULL, frame);
	action_set(&act[i++], action_enemy_rewind, 10, frame);

	//angrysoo
	angrysoo1.size = 2;
	angrysoo1.act = action_alloc(angrysoo1.size);
	act = angrysoo1.act;
	i = 0;

	frame = 11; action_set(&act[i++], shoot_pink_spiral, NULL, frame);
	action_set(&act[i++], action_enemy_rewind, 10, frame);
}

void stg3_draw() {
	sprite_draw_clear(0xFF000000);
}
