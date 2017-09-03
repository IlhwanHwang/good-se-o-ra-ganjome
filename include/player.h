#ifndef PLAYER
#define PLAYER

#define PLAYERPADDING 10

void player_init();
void player_update();
void player_draw();
void player_life_draw();
inline float player_get_x();
inline float player_get_y();
inline int player_get_life();

#endif