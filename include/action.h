#ifndef ACTION
#define ACTION

#include "timeline.h"

void action_set(action *act, void (*func)(void*, void*), void *aux, unsigned frame);
void action_void (void *self, void *aux);
void action_enemy_stop(void *self, void *aux);
void action_enemy_towards_player(void *self, void *aux);
void action_enemy_horizontal(void *self, void *aux);
void action_enemy_vertical(void *self, void *aux);
void action_enemy_change_direction(void *self, void *aux);
void action_enemy_rewind(void *self, void *aux);
void action_enemy_destroy(void *self, void *aux);

void action_watch_rewind(void *self, void *aux);

#endif