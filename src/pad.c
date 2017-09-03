#include <stdio.h>
#include "pad.h"
#include "cbox.h"
#include "interrupt.h"
#include "util.h"
#include "sprite.h"

cbox cb_pad = {PADW, PADH, PADX, PADY, PADX + PADW, PADY + PADH};

static int pvalid = 0;
static float ppadx = 0.0;
static float ppady = 0.0;
static float padvx = 0.0;
static float padvy = 0.0;

extern unsigned *cb_screen;
extern sprite spr_key;

void pad_update() {
	if (touch_is_valid()) {
		float x, y;

		x = touch_get_x();
		y = touch_get_y();

		if (!pvalid && !cbox_inclusion(&cb_pad, x, y)) {
			return;
		}

		x = fclamp(x, cb_pad.x1, cb_pad.x2);
		y = fclamp(y, cb_pad.y1, cb_pad.y2);

		if (!pvalid) {
			ppadx = x;
			ppady = y;
			pvalid = 1;
		}

		ppadx = flerp(ppadx, x, 0.5);
		ppady = flerp(ppady, y, 0.5);

		padvx = (ppadx - PADX) / PADW * (2.0 + 2.0 * PADTHRESH) - (1.0 + PADTHRESH);
		padvy = (ppady - PADY) / PADH * (2.0 + 2.0 * PADTHRESH) - (1.0 + PADTHRESH);

		if (padvx > PADTHRESH)
			padvx -= PADTHRESH;
		else if (padvx > -PADTHRESH)
			padvx = 0.0;
		else
			padvx += PADTHRESH;

		if (padvy > PADTHRESH)
			padvy -= PADTHRESH;
		else if (padvy > -PADTHRESH)
			padvy = 0.0;
		else
			padvy += PADTHRESH;

		padvx *= PADVMAX;
		padvy *= PADVMAX;
	}
	else {
		pvalid = 0;
		padvx = 0.0;
		padvy = 0.0;
	}
}

void pad_draw() {
	sprite_draw(&spr_key, PADX, PADY);
}

inline float pad_get_vx() {
	return padvx;
}

inline float pad_get_vy() {
	return padvy;
}