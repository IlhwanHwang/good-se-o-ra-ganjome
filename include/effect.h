#ifndef EFFECT
#define EFFECT

#define EFFECT_MAX 128
#define EFFECT_UNIT 2

#include "sprite.h"

typedef struct {
	sprite **spr;
	unsigned size;
} animation;

typedef struct {
	animation *ani;
	int frame;
	float x, y;
} effect;

void effect_init();
effect *effect_find_empty();
void effect_large(float x, float y);
void effect_small(float x, float y);
void effect_update();
void effect_draw();

#endif