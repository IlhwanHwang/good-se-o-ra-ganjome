#ifndef SMANAGE
#define SMANAGE

#include "timeline.h"

void smanage_set(timeline *tl);
void smanage_update();
void smanage_next(timeline *tl, void (*next)());
void smanage_draw();

#endif