#include <io.h>
#include <s3c_uart.h>
#include <config.h>

static unsigned long base;

static unsigned long uart_base[] = {
    UART0_BASE,
    UART1_BASE,
    UART2_BASE,
    UART3_BASE,
};

int uart_init(int chn, int baud)
{
    unsigned long reg;

    if (chn > 3)
        return -1;

    base = uart_base[chn];

    /* line configuratinon */
    reg = ULCON_8N1;
    writel(reg, base + ULCON);

    /* UART control */
    reg = UCON_CLK_PCLK | UCON_TXMODE_IRQ | UCON_RXMODE_IRQ;
    writel(reg, base + UCON);

    /* FIFO reset */
    reg = UFCON_TXTRG_16BYTES | UFCON_RXTRG_32BYTES | \
        UFCON_TXFIFO_RESET | UFCON_RXFIFO_RESET | UFCON_FIFO_ENABLE;
    writel(reg, base + UFCON);

    /* baud rate */
    reg = ((CONFIG_SYS_PCLK / baud / 16) - 1);
    writel(reg, base + UBRDIV);

    return 0;
}

void mango_uart_putc(const char c)
{
    if (c == '\n')
        mango_uart_putc('\r');

    while (readl(base + UFSTAT) & UFSTAT_TXFIFO_FULL)
        ;

    writel(c, base + UTXH);
}

int mango_uart_write(const char *buf, int len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        mango_uart_putc(buf[i]);
    }

    return len;
}

int mango_uart_read(char *buf, int len, int nowait)
{
    int i;
    for (i = 0; i < len; ++i) {
        while (!UFSTAT_RXFIFO_COUNT(readl(base + UFSTAT))) {
            if (nowait || (i > 0)) {
                return i;
            }
        }
	buf[i] = readl(base + URXH);
    }

    return i;
}
