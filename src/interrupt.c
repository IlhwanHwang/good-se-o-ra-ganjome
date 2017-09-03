#include <stdio.h>

#include "lcd.h"
#include "util.h"
#include "graphics.h"
#include "s3c_uart.h"
#include "s3c6410.h"
#include "vic.h"
#include "interrupt.h"

static int touch_valid = 0;
static float touch_x = 0.0;
static float touch_y = 0.0;

/* enable interrupt in CPU level */
void enable_interrupts(void){
	__asm__ __volatile__ ("mrs r0, cpsr");
	__asm__ __volatile__ ("bic r0, r0, #0x80");
	__asm__ __volatile__ ("msr cpsr_c, r0");
}

/* disable interrupt in CPU level */
void disable_interrupts(void){
	__asm__ __volatile__ ("mrs r0, cpsr");
	__asm__ __volatile__ ("orr r0, r0, #0x80");
	__asm__ __volatile__ ("msr cpsr_c, r0");
}

unsigned int timer1_isr_call_count = 0;
unsigned int touch_isr_call_count = 0;

void touchInterruptServiceRoutine(void){
	unsigned int temp;
	unsigned int temp2;

	if( !(VIC1RAWINTR_REG & 1<<30) )
		return;

	/* Disable any other interrupt */
	temp = VIC1INTENABLE_REG;
	VIC1INTENCLEAR_REG = 0xffffffff;

	temp2 = readl(ADCCON);
	temp2 |= 1;
	writel(temp2, ADCCON);
	writel(0xd4, ADCTSC);

	//printf ("Touch Detected\t");
	touch_isr_call_count ++;

	writel(0x1, ADCCLRINTPNDNUP);

	/* Enable other interrupts */
	VIC1INTENABLE_REG = temp;
}

void touchInterruptServiceRoutine2(void){
	static int first = 1;
	unsigned int temp;
	unsigned int x, y;

	if( !(VIC1RAWINTR_REG & 1<<31) )
		return;

	/* Disable any other interrupt */
	temp = VIC1INTENABLE_REG;
	VIC1INTENCLEAR_REG = 0xffffffff;

	while( !(readl(ADCCON) & 1<<15) );
	
	x = readl(ADCDAT0) & 0x3ff;
	y = readl(ADCDAT1) & 0x3ff;

	touch_x = ((float)x - 200.0) * 800.0 / 640.0 / 2.0;
	touch_y = ((float)y - 340.0) * 480.0 / 360.0 / 2.0;
	if (first)
		first = 0;
	else
		touch_valid = 1;

	//printf("(%d, %d)\n", rx, ry);
 
	writel(0xd3, ADCTSC);
	writel(0x1, ADCCLRINT);
		
	/* Enable other interrupts */
	VIC1INTENABLE_REG = temp;
}

void mango_interrupt_init(void){
	VIC1INTENABLE_REG |= BIT30;
	VIC1INTENABLE_REG |= BIT31;

	writel(0xffff, ADCDLY);
	writel(0xd3, ADCTSC);
	writel(0x7fc1, ADCCON); 

	VIC1VECTADDR30 = (unsigned)touchInterruptServiceRoutine;
	VIC1VECTADDR31 = (unsigned)touchInterruptServiceRoutine2;

	TCFG0_REG = (TCFG0_REG & ~(0xff00)) | (0x21 << 8); //Prescaler 0: 0x21:33
	TCFG1_REG = (TCFG1_REG & ~(0xf00)) | (0x1 << 8); //divider MUX2: 1/2

	TCNTB2_REG = 0xffffffff;
	TCMPB2_REG = 0xffffffff - 1;

	TCON_REG |= BIT13; //Timer2 Manual update
	TCON_REG = (TCON_REG & ~(0xf << 12)) | BIT15 | BIT12;
	//Timer1 Auto-reload on & Timer1 on
}

unsigned utime(void) {
	return ~TCNTO2_REG;
}

inline void touch_clear() {
	touch_valid = 0;
}

inline int touch_is_valid() {
	return touch_valid;
}

inline float touch_get_x() {
	return touch_x;
}

inline float touch_get_y() {
	return touch_y;
}