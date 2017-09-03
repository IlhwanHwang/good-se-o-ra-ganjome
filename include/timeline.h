#ifndef TIMELINE
#define TIMELINE

typedef struct {
	void (*func)(void*, void*);
	void *aux;
	unsigned frame;
} action;

typedef struct {
	action* act;
	unsigned size;
} timeline;

typedef struct {
	timeline *tl;
	unsigned seek;
	unsigned fcnt;
} watch;

#define ACTION_MAX 256

action *action_alloc(int size);
void watch_update(watch *w, void *aux);
void watch_set_frame(watch *w, unsigned fcnt);

#endif