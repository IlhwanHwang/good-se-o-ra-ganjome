#include "timeline.h"
#include "enemy.h"
#include "player.h"
#include "util.h"

action action_pool[ACTION_MAX];
static int pool_seek = 0;

action *action_alloc(int size) {
	int idx = pool_seek;
	pool_seek += size;
	if (pool_seek >= ACTION_MAX)
		printf("********\nError::Action pool size exceeded!!\n********\n");
	return &action_pool[idx];
}

void watch_update(watch *w, void *self) {
	timeline *tl = w->tl;
	action *act = tl->act;

	while (1) {
		if (tl->size <= w->seek)
			break;
		else if (act[w->seek].frame != w->fcnt)
			break;
		else {
			unsigned idx = w->seek++;
			act[idx].func(self, act[idx].aux);
		}
	}

	w->fcnt++;
}

void watch_set_frame(watch *w, unsigned fcnt) {
	timeline *tl = w->tl;
	action *act = tl->act;

	w->fcnt = fcnt;
	w->seek = 0;

	while (act[w->seek].frame < w->fcnt) {
		w->seek++;
	}
}