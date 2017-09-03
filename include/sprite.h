#ifndef SPRITE
#define SPRITE

typedef struct {
	int ofx, ofy, w, h;
	unsigned *data;
} sprite;

void sprite_resource_init();
void sprite_set_target(unsigned *t);
void sprite_draw(sprite *spr, float __x, float __y);
void sprite_draw_clear(unsigned wrapper);

#endif