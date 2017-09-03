#include "action.h"
#include "timeline.h"
#include "enemy.h"
#include "player.h"
#include "util.h"

void action_set(action *act, void (*func)(void*, void*), void *aux, unsigned frame) {
	act->func = func;
	act->aux = aux;
	act->frame = frame;
}

void action_void (void *self, void *aux) {}

void action_enemy_stop(void *self, void *aux) {
	enemy *e = (enemy *)self;
	e->vx = 0;
	e->vy = 0;
}

void action_enemy_towards_player(void *self, void *aux) {
	enemy *e = (enemy *)self;
	float vx, vy, d;

	vx = player_get_x() - e->x;
	vy = player_get_y() - e->y;
	d = (*(float *)(&aux)) * Q_rsqrt(vx*vx + vy*vy);

	vx *= d;
	vy *= d;

	e->vx = vx;
	e->vy = vy;
}

void action_enemy_horizontal(void *self, void *aux) {
	enemy *e = (enemy *)self;
	e->vx = (*(float *)(&aux));
	e->vy = 0.0;
}

void action_enemy_vertical(void *self, void *aux) {
	enemy *e = (enemy *)self;
	e->vy = (*(float *)(&aux));
	e->vx = 0.0;
}

void action_enemy_change_direction(void *self, void *aux) {
	enemy *e = (enemy *)self;
	float dir = (*(float *)(&aux));
	float spd = sqrt((e->vx)*(e->vx) + (e->vy)*(e->vy));
	e->vx = spd * cos(dir);
	e->vy = spd * sin(dir);
}

void action_enemy_rewind(void *self, void *aux) {
	enemy *e = (enemy *)self;
	watch_set_frame(&e->w, (int)aux);
}

void action_enemy_destroy(void *self, void *aux) {
	enemy *e = (enemy *)self;
	e->destroy = 1;
}

void action_watch_rewind(void *self, void *aux) {
	watch *w = (watch *)self;
	watch_set_frame(w, 0);
}