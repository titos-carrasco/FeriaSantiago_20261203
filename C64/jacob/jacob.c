
#include <c64.h>
#include <cbm.h>
#include <joystick.h>
#include <conio.h>
#include <string.h>

#define BITMAP_ADDR ((unsigned char *)0x4000)
#define SCREEN_ADDR ((unsigned char *)0x6000)
#define CHAR_ROM ((unsigned char *)0xD000)

#define JACOB_SCR "jacob.hir,p,r"
#define MODEM_LFN 2

void set_hires_mode();
void set_normal_mode();
int load_screen();
unsigned char petscii_to_screencode(unsigned char c);
void hires_puts(unsigned char col, unsigned char row, const char *str, unsigned char color);
int send_command(unsigned char);

int main(void)
{
    unsigned char joy;
    unsigned char cmd = 'S';
    int x = 15, y = 2;
    const char config[2] = {6, 0};

    joy_install(joy_static_stddrv);
    set_hires_mode();
    if (load_screen() == 0)
    {
        cbm_k_close(MODEM_LFN);
        cbm_k_clrch();
        cbm_k_setlfs(MODEM_LFN, 2, 0);
        cbm_k_setnam(config);
        cbm_k_open();

        while (!kbhit())
        {
            joy = joy_read(JOY_2);
            if (JOY_UP(joy))
            {
                if (cmd != 'F')
                {
                    cmd = 'F';
                    hires_puts(x, y, "  Avanza  ", 0xD0);
                    send_command(cmd);
                }
            }
            else if (JOY_DOWN(joy))
            {
                if (cmd != 'B')
                {
                    cmd = 'B';
                    hires_puts(x, y, "Retrocede ", 0xD0);
                    send_command(cmd);
                }
            }
            else if (JOY_LEFT(joy))
            {
                if (cmd != 'L')
                {
                    cmd = 'L';
                    hires_puts(x, y, "Izquierda ", 0xD0);
                    send_command(cmd);
                }
            }
            else if (JOY_RIGHT(joy))
            {
                if (cmd != 'R')
                {
                    cmd = 'R';
                    hires_puts(x, y, " Derecha  ", 0xD0);
                    send_command(cmd);
                }
            }
            else
            {
                if (cmd != 'S')
                {
                    cmd = 'S';
                    hires_puts(x, y, "          ", 0xD0);
                    send_command(cmd);
                }
            }
        }
        cgetc();
        cbm_k_close(MODEM_LFN);
        set_normal_mode();
    }

    clrscr();
    return 0;
}

void set_hires_mode()
{
    memset(BITMAP_ADDR, 0x00, 8000);
    memset(SCREEN_ADDR, 0xD0, 1000);

    CIA2.pra = (CIA2.pra & 0xFC) | 0x02;
    VIC.addr = 0x80;
    VIC.bordercolor = 0;
    VIC.ctrl1 = 0x3B;
}

void set_normal_mode()
{
    CIA2.pra = (CIA2.pra & 0xFC) | 0x03;
    VIC.addr = 0x15;
    VIC.bordercolor = 14;
    VIC.ctrl1 = 0x1B;
}

int load_screen()
{
    unsigned char ch;

    hires_puts(11, 24, "CARGANDO IMAGEN...", 0xD0);

    if (cbm_open(1, 8, 2, JACOB_SCR) != 0)
        return -1;
    if (cbm_read(1, &ch, 1) == 1)
        if (cbm_read(1, &ch, 1) == 1)
            if (cbm_read(1, BITMAP_ADDR, 8000) == 8000)
            {
                // if (cbm_read(1, SCREEN_ADDR, 1000) == 1000) {

                hires_puts(0, 0, "  commodore 64 | 300bps | 8n1 | noctrl  ", 0xD0);
                cbm_close(1);
                return 0;
            }
    cbm_close(1);
    return -1;
}

unsigned char petscii_to_screencode(unsigned char c)
{
    if (c == '|')
        return 93;
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1; /* a-z -> 1..26 (mayusculas en ROM) */
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 1; /* A-Z -> 1..26 */
    return c;               /* numeros, espacio y signos coinciden */
}

void hires_puts(unsigned char col, unsigned char row, const char *str, unsigned char color)
{
    unsigned int offset = (row * 40 + col);
    unsigned char *bmp_dst = BITMAP_ADDR + (offset * 8);
    unsigned char *col_dst = SCREEN_ADDR + offset;
    const unsigned char *src;
    unsigned char sc, i;

    while (*str)
    {
        sc = petscii_to_screencode(*str++);
        src = CHAR_ROM + (sc * 8);

        /* Deshabilita interrupciones y mapea Character ROM en $D000 */
        asm("sei");
        *(unsigned char *)0x01 &= 0xFB;

        for (i = 0; i < 8; ++i)
        {
            bmp_dst[i] = src[i];
        }

        /* Restaura I/O en $D000 y reactiva interrupciones */
        *(unsigned char *)0x01 |= 0x04;
        asm("cli");

        *col_dst++ = color;
        bmp_dst += 8;
    }
}

int send_command(unsigned char cmd)
{
    if (cbm_k_ckout(MODEM_LFN))
    {
        cbm_k_clrch();
        return 1;
    }
    cbm_k_bsout(cmd);
    cbm_k_bsout(0x0D);
    cbm_k_clrch();
    return 0;
}
