#include <stdio.h>
#include "frame.h"
#include "s3c6410.h"
#include "lcd.h"
#include "util.h"
#include "graphics.h"
#include "postproc.h"
#include "interrupt.h"
#include "timeline.h"
#include "cbox.h"
#include "bullet.h"
#include "interrupt.h"
#include "pad.h"
#include "player.h"
#include "enemy.h"
#include "smanage.h"
#include "effect.h"
#include "item.h"
#include "stage1.h"

int frame_count = 0;
cbox cb_screen = {(float)SCREENW, (float)SCREENH, 0.0, 0.0, (float)SCREENW, (float)SCREENH};
cbox cb_scr_pad = {(float)(SCREENW + SCREENPAD), (float)(SCREENH + SCREENPAD), 
	-(float)SCREENPAD, -(float)SCREENPAD, 
	(float)(SCREENW + SCREENPAD), (float)(SCREENH + SCREENPAD)};
cbox cb_scr_unpad = {(float)(SCREENW - SCREENUNPAD), (float)(SCREENH - SCREENUNPAD), 
	(float)SCREENUNPAD, (float)SCREENUNPAD, 
	(float)(SCREENW - SCREENUNPAD), (float)(SCREENH - SCREENUNPAD)};

unsigned *scr_target;

static int frame_asserted = 0;

static unsigned screen[SCREENSIZE];

static unsigned fb_odd[S3CFB_SIZE];
static unsigned fb_even[S3CFB_SIZE];
static unsigned *fb_shown;
static unsigned *fb_working;

extern timeline tl_stg1_main;

static unsigned state = 0;
static unsigned deadcnt = 0;
unsigned endcount = 0;

extern sprite spr_title;
extern sprite spr_gameover;
extern sprite spr_ending;

void (*bg_draw)();

int frame_is_asserted(void) {
	return frame_asserted;
}

void frame_set_fb(unsigned *fb) {
	unsigned ufb = (unsigned)fb;
	S3C_VIDW00ADD0B0 = ufb; 
	S3C_VIDW00ADD1B0 = S3C_VIDWxxADD1_VBASEL_F(ufb + 
		(PAGE_WIDTH + S3CFB_OFFSET) * S3CFB_VRES);
	S3C_VIDW00ADD2  = S3C_VIDWxxADD2_OFFSIZE_F(S3CFB_OFFSET) |
		S3C_VIDWxxADD2_PAGEWIDTH_F(PAGE_WIDTH);
}

void frame_init(void) {
	frame_set_fb(fb_even);

	printf("Frame initialized.\n");
	printf("Screen collision box is (%d, %d) (%d, %d).\n", (int)cb_screen.x1, (int)cb_screen.y1, (int)cb_screen.x2, (int)cb_screen.y2);
}

void frame_assert(void) {
	frame_asserted = 1;
}

static void screen_refresh();

void frame_service(void) {
	static int parity = 0;

	if (frame_asserted) {
		fb_shown = parity ? fb_odd : fb_even;
		fb_working = !parity ? fb_odd : fb_even;
		parity = !parity;

		frame_set_fb(fb_shown);
		scr_target = screen;

		if (state == 0) {
			if (touch_is_valid()) {
				bullet_init();
				player_init();
				enemy_init();
				effect_init();
				item_init();
				smanage_set(&tl_stg1_main);
				bg_draw = stg1_draw;

				deadcnt = 0;
				endcount = 0;
				state = 1;
			}
		}
		else if (state == 1) {
			smanage_update();

			pad_update();
			player_update();
			enemy_update();
			item_update();

			bullet_update_master();
			effect_update();

			if (player_get_life() == 0)
				deadcnt++;

			if (endcount > 0)
				endcount++;

			if (deadcnt >= TRANSITIONTIME)
				state = 2;

			if (endcount >= TRANSITIONTIME)
				state = 3;
		}
		else if (state == 2) {
			if (touch_is_valid()) {
				state = 0;
			}
		}
		else if (state == 3) {
			if (touch_is_valid()) {
				state = 0;
			}
		}

		screen_refresh();
		gfx_poll();
		screen_fetch();

		touch_clear();
		frame_count++;
		frame_asserted = 0;
	}
}

void screen_fetch(void) {
	ppc_set(screen, fb_working);
}

static void screen_refresh() {
	if (state == 0) {
		sprite_draw_clear(0xFFFFFFFF);
		sprite_draw(&spr_title, 200.0, 120.0);
	}
	if (state == 1) {
		bg_draw();
		smanage_draw();

		player_draw();
		enemy_draw();
		bullet_draw_master();
		item_draw();
		effect_draw();

		player_life_draw();
		pad_draw();

		if (player_get_life() == 0 || endcount > 0) {
			unsigned alpha = (deadcnt + endcount) * 255 / TRANSITIONTIME;
			sprite_draw_clear(0xFFFFFF | (alpha << 24));
		}
	}
	if (state == 2) {
		sprite_draw_clear(0xFFFFFFFF);
		sprite_draw(&spr_gameover, 200.0, 120.0);
	}
	if (state == 3) {
		sprite_draw_clear(0xFFFFFFFF);
		sprite_draw(&spr_ending, 200.0, 120.0);
	}
}
