#include <stdio.h>
#include "effect.h"
#include "sprite.h"

effect effect_pool[EFFECT_MAX];
static int pool_seek = 0;

extern sprite spr_bomb1;
extern sprite spr_bomb2;
extern sprite spr_bomb3;
extern sprite spr_sbamb1;
extern sprite spr_sbamb2;
extern sprite spr_sbamb3;

sprite *ani_large_seqeunce[3] = {&spr_bomb1, &spr_bomb2, &spr_bomb3};
sprite *ani_small_seqeunce[3] = {&spr_sbamb1, &spr_sbamb2, &spr_sbamb3};
animation ani_large = {ani_large_seqeunce, 3};
animation ani_small = {ani_small_seqeunce, 3};

void effect_init() {
	int i;
	for (i = 0; i < EFFECT_MAX; i++) {
		effect_pool[i].frame = -1;
	}
}

effect *effect_find_empty() {
	int i;
	for (i = 0; i < EFFECT_MAX; i++) {
		int idx = pool_seek;
		effect *ef = &effect_pool[idx];
		pool_seek = (pool_seek + 1) % EFFECT_MAX;
		if (ef->frame == -1) {
			return ef;
		}
	}
	return NULL;
}

void effect_large(float x, float y) {
	effect *ef = effect_find_empty();

	if (ef == NULL) {
		printf("Nothing left in effect pool!!!\n");
		return;
	}

	ef->ani = &ani_large;
	ef->frame = 0;
	ef->x = x;
	ef->y = y;
}

void effect_small(float x, float y) {
	effect *ef = effect_find_empty();

	if (ef == NULL) {
		printf("Nothing left in effect pool!!!\n");
		return;
	}

	ef->ani = &ani_small;
	ef->frame = 0;
	ef->x = x;
	ef->y = y;
}

void effect_update() {
	int i;
	for (i = 0; i < EFFECT_MAX; i++) {
		effect *ef = &effect_pool[i];
		if (ef->frame != -1) {
			ef->frame++;
			if (ef->frame >= ef->ani->size * EFFECT_UNIT) {
				ef->frame = -1;
			}
		}
	}
}

void effect_draw() {
	int i;
	for (i = 0; i < EFFECT_MAX; i++) {
		effect *ef = &effect_pool[i];
		if (ef->frame != -1) {
			sprite_draw(ef->ani->spr[ef->frame / EFFECT_UNIT], ef->x, ef->y);
		}
	}
}