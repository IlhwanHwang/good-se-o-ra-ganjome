#ifndef ENEMY
#define ENEMY

#include "cbox.h"
#include "timeline.h"
#include "sprite.h"

typedef struct {
	cbox cb;
	watch w;
	sprite *spr;
	void (*dest)(void *);
	float x, y, vx, vy, ax, ay;
	int health;
	int valid;
	int destroy;
	int ef;
	int item_prob;
} enemy;

void enemy_init();
enemy *enemy_find_empty();
enemy *enemy_create(sprite *spr, timeline *tl, void (*step)(void *),
	float x, float y, float w, float h, int health);
void enemy_update();
int enemy_damage(int i, int power);
void enemy_delete(int i);
void enemy_draw();

#define ENEMY_MAX 128

#define ENEMY_W_SMALL 30.0
#define ENEMY_H_SMALL 30.0
#define ENEMY_W_MEDIUM 45.0
#define ENEMY_H_MEDIUM 45.0
#define ENEMY_W_LARGE 60.0
#define ENEMY_H_LARGE 60.0

#define ENEMY_HEALTH_SMALL 8
#define ENEMY_HEALTH_MEDIUM 16
#define ENEMY_HEALTH_LARGE 32

#endif