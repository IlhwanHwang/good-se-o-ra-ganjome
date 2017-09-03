#ifndef PAD
#define PAD

#define PADX 10.0
#define PADY 150.0
#define PADW 80.0
#define PADH 80.0
#define PADVMAX 10.0
#define PADTHRESH 0.1

void pad_update();
void pad_draw();
inline float pad_get_vx();
inline float pad_get_vy();

#endif