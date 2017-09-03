#ifndef CBOX
#define CBOX

typedef struct {
	float w, h;
	float x1, y1, x2, y2;
} cbox;

inline int cbox_inclusion(cbox *cb, float x, float y);
void cbox_refresh(cbox *cb, float x, float y);
void cbox_refresh_centered(cbox *cb, float x, float y);
int cbox_collide(cbox *c1, cbox *c2);

#endif