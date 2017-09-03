#include <stdio.h>
#include "smanage.h"
#include "timeline.h"
#include "sprite.h"
#include "frame.h"

watch main_watch;
static int next_phase = 0;

extern unsigned *scr_target;
extern void (*bg_draw)();
static void (*bg_draw_next)();

void smanage_set(timeline *tl) {
	main_watch.seek = 0;
	main_watch.fcnt = 0;
	main_watch.tl = tl;
}

void smanage_update() {
	if (next_phase > 0) {
		next_phase += SCREENW / 30;
		if (bg_draw_next != NULL && next_phase > SCREENW) {
			bg_draw = bg_draw_next;
			bg_draw_next = NULL;
		}
		if (next_phase > SCREENW * 2) {
			main_watch.seek = 0;
			main_watch.fcnt = 0;
			next_phase = 0;
		}
	}
	else {
		watch_update(&main_watch, &main_watch);
	}
}

void smanage_next(timeline *tl, void (*next)()) {
	main_watch.tl = tl;
	next_phase = 1;
	bg_draw_next = next;
}

void smanage_draw() {
	unsigned wrapper = 0xFF202020;

	if (next_phase == 0)
		return;

	if (next_phase < SCREENW) {
		gfx_bitblck_ext(scr_target, &wrapper,
			SCREENW - next_phase, 0, SCREENW, SCREENH,
			SCREENW, SCREENH,
			0, 0, 1, 1,
			1, 1);
	}
	else {
		gfx_bitblck_ext(scr_target, &wrapper,
			0, 0, 2 * SCREENW - next_phase, SCREENH,
			SCREENW, SCREENH,
			0, 0, 1, 1,
			1, 1);
	}
}