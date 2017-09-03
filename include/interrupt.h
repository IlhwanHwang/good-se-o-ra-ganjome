#ifndef INTERRUPT
#define INTERRUPT

void enable_interrupts(void);
void disable_interrupts(void);
void mango_interrupt_init(void);
unsigned utime(void);

inline void touch_clear();
inline int touch_is_valid();
inline float touch_get_x();
inline float touch_get_y();

#endif