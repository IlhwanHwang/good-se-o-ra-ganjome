#ifndef FRAME
#define FRAME

#define SCREENPAD 80
#define SCREENUNPAD 30
#define SCREENW 400
#define SCREENH 240
#define SCREENSIZE (SCREENW * SCREENH)

#define TRANSITIONTIME 60

void frame_init(void);
void frame_assert(void);
void frame_service(void);
int frame_is_asserted(void);

void screen_fetch(void);

#endif
