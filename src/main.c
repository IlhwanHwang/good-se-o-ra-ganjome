#include <stdio.h>
#include <string.h>
#include <s3c_uart.h>
#include <lcd.h>
#include "graphics.h"
#include "vic.h"
#include "frame.h"
#include "enemy.h"
#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "timeline.h"
#include "effect.h"
#include "item.h"

static void init(void)
{
	disable_interrupts();
	uart_init(1, 115200);
	lcd_init();
	gfx_init();
	ppc_init();
	mango_interrupt_init();
	enable_interrupts();

	sprite_resource_init();

	stg1_init();
	stg2_init();
	stg3_init();
}

int main()
{
	init();

	while(1){
		frame_service();
	}

	return 0;
}