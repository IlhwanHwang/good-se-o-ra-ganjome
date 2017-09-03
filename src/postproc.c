#include "postproc.h"
#include "lcd.h"
#include "s3c6410.h"
#include "vic.h"
#include "frame.h"


void ppc_interrupt_service_routine(void) {
	PPCMODE &= ~BIT6;
}

static int logi(unsigned x) {
	int ret = 0;
	x >>= 1;
	while (x) {
		x >>= 1;
		ret++;
	}
	return ret;
}

static int maxi(unsigned x, unsigned y) {
	return x > y ? x : y;
}

void ppc_init() {
	unsigned hclk = get_hclk();
	unsigned pres_h_ratio = maxi(SCREENW / S3CFB_HRES, 1);
	unsigned pres_v_ratio = maxi(SCREENH / S3CFB_VRES, 1);
	unsigned pres_dest_h = SCREENW / pres_h_ratio;
	unsigned pres_dest_v = SCREENH / pres_v_ratio;
	unsigned h_shift = logi(pres_h_ratio);
	unsigned v_shift = logi(pres_v_ratio);

	VIC0INTENABLE_REG |= (1 << 9);
	VIC0VECTADDR9 = (unsigned)ppc_interrupt_service_routine;

	PPCSRCIMGSIZE = (SCREENH << 12) | SCREENW;
	PPCDESTIMGSIZE = (S3CFB_VRES << 12) | S3CFB_HRES;

	PPCPRESCLRATIO = (pres_v_ratio << 7) | pres_h_ratio;
	PPCPRESCLSIZE = (pres_dest_v << 12) | pres_dest_h;

	PPCMSCLHRATIOH = (SCREENW << 8) / (S3CFB_HRES << h_shift);
	PPCMSCLHRATIOV = (SCREENH << 8) / (S3CFB_VRES << v_shift);

	PPCPRESCLSH = (10 - h_shift - v_shift);

	PPCMODE = BIT18 | BIT7 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1;
	PPCMODE2 = BIT4;

	printf("Postprocessor initialized.\n");
}

void ppc_set(unsigned *src, unsigned *dest) {
	PPCADDRSY = (unsigned)src;
	PPCADDRSRGB = (unsigned)dest;

	PPCADDREY = (unsigned)src + (SCREENSIZE << 2);
	PPCADDRERGB = (unsigned)dest + (S3CFB_SIZE << 2);

	PPCPOSTENVID = BIT31;
}