#ifndef BULLET
#define BULLET

#include "sprite.h"
#include "cbox.h"

typedef struct {
	sprite* spr;
	cbox cb;
	float x, y, vx, vy, ax, ay;
	int valid;
	int power;
	int ef;
} bullet;

typedef struct {
	bullet* b_array;
	unsigned count;
	unsigned size;
	unsigned seek;
} bulletpool;

#define EBULLET_MAX 256
#define PBULLET_MAX 32

void bullet_init();
bullet* bullet_find_empty(bulletpool *bp);
bullet* bullet_create_enemy(sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power);
bullet* bullet_create_player(sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power);
bullet* bullet_create(bulletpool *bp, sprite *spr, 
	float x, float y, float vx, float vy, float w, float h, int power);
void bullet_update(bulletpool *bp);
void bullet_update_master();
void bullet_delete(bulletpool *bp, int i);
void bullet_draw(bulletpool *bp);
void bullet_draw_master();
void bullet_towards_player(bullet *b, float v, float dir);

#endif