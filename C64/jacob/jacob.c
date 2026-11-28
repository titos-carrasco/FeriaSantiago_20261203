
#include <c64.h>
#include <cbm.h>
#include <conio.h>
#include <joystick.h>
#include <stdint.h>
#include <string.h>

#define BITMAP_ADDR ((uint8_t *)0x4000)
#define SCREEN_ADDR ((uint8_t *)0x6000)
#define CHAR_ROM ((uint8_t *)0xD000)
#define CHAR_RAM ((uint8_t *)0xC000)
#define RODBS ((volatile uint8_t *)0x029D)
#define RODBE ((volatile uint8_t *)0x029E)

#define JACOB_SCR "jacob.hir,p,r"
#define MODEM_LFN 2

void init_charset(void);
void set_hires_mode();
void set_normal_mode();
int load_screen();
uint8_t petscii_to_screencode(uint8_t c);
void hires_puts(uint8_t col, uint8_t row, const char *str, uint8_t color,
                uint8_t inverse);
int send_command(uint8_t);

int main(void) {
    uint8_t joy;
    uint8_t cmd = 's';
    int x = 11, y = 2;
    const char config[2] = {6, 0};

    init_charset();
    joy_install(joy_static_stddrv);
    set_hires_mode();
    if (load_screen() == 0) {
        cbm_k_close(MODEM_LFN);
        cbm_k_clrch();
        cbm_k_setlfs(MODEM_LFN, 2, 0);
        cbm_k_setnam(config);
        cbm_k_open();
        while (*RODBS != *RODBE)
            ;

        while (!kbhit()) {
            joy = joy_read(JOY_2);
            if (JOY_UP(joy)) {
                if (cmd != 'f') {
                    cmd = 'f';
                    hires_puts(x, y, "AVANZA    ", 0xD0, 1);
                    send_command(cmd);
                }
            } else if (JOY_DOWN(joy)) {
                if (cmd != 'b') {
                    cmd = 'b';
                    hires_puts(x, y, "RETROCEDE ", 0xD0, 1);
                    send_command(cmd);
                }
            } else if (JOY_LEFT(joy)) {
                if (cmd != 'l') {
                    cmd = 'l';
                    hires_puts(x, y, "IZQUIERDA ", 0xD0, 1);
                    send_command(cmd);
                }
            } else if (JOY_RIGHT(joy)) {
                if (cmd != 'r') {
                    cmd = 'r';
                    hires_puts(x, y, "DERECHA   ", 0xD0, 1);
                    send_command(cmd);
                }
            } else {
                if (cmd != 's') {
                    cmd = 's';
                    hires_puts(x, y, "DETENIDO  ", 0xD0, 1);
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

void init_charset(void) {
    asm("sei");
    *(uint8_t *)0x01 &= 0xFB;
    memcpy(CHAR_RAM, (void *)0xD800, 2048);
    *(uint8_t *)0x01 |= 0x04;
    asm("cli");
}

void set_hires_mode() {
    memset(BITMAP_ADDR, 0x00, 8000);
    memset(SCREEN_ADDR, 0xD0, 1000);

    CIA2.pra = (CIA2.pra & 0xFC) | 0x02;
    VIC.addr = 0x80;
    VIC.bordercolor = 0;
    VIC.ctrl1 = 0x3B;
}

void set_normal_mode() {
    CIA2.pra = (CIA2.pra & 0xFC) | 0x03;
    VIC.addr = 0x15;
    VIC.bordercolor = 14;
    VIC.ctrl1 = 0x1B;
}

int load_screen() {
    uint8_t ch;

    hires_puts(11, 24, "CARGANDO IMAGEN...", 0xD0, 0);

    if (cbm_open(1, 8, 2, JACOB_SCR) != 0)
        return -1;
    if (cbm_read(1, &ch, 1) == 1)
        if (cbm_read(1, &ch, 1) == 1)
            if (cbm_read(1, BITMAP_ADDR, 8000) == 8000) {
                // if (cbm_read(1, SCREEN_ADDR, 1000) == 1000) {
                hires_puts(11, 2, "DETENIDO  ", 0xD0, 1);
                cbm_close(1);
                return 0;
            }
    cbm_close(1);
    return -1;
}

uint8_t petscii_to_screencode(uint8_t c) {
    if (c == ' ' || c == 0xA0)
        return 32; // Espacio
    if (c == '|')
        return 93;
    if (c == '@')
        return 0;
    if (c >= 0x41 && c <= 0x5A)
        return c - 64;
    if (c >= 0xC1 && c <= 0xDA)
        return c - 128;
    if (c >= 0x5B && c <= 0x5F)
        return c - 64;
    return c & 0x7F;
}

void hires_puts(uint8_t col, uint8_t row, const char *str, uint8_t color,
                uint8_t inverse) {
    uint8_t sc, r;
    const uint8_t *src;
    uint8_t *bmp_dst;
    uint8_t *col_dst;
    uint16_t offset;
    uint8_t inv_mask = inverse ? 0xFF : 0x00;

    if (row >= 25)
        return;

    offset = ((uint16_t)row * 40) + col;
    col_dst = SCREEN_ADDR + offset;
    bmp_dst = BITMAP_ADDR + (offset * 8);

    while (*str && col < 40) {
        sc = petscii_to_screencode((uint8_t)*str++);

        // Apuntar directamente a nuestra copia segura en RAM ($C000)
        src = CHAR_RAM + ((uint16_t)sc * 8);

        // Volcar las 8 filas directamente al Bitmap (sin tocar 0x01 ni SEI/CLI)
        for (r = 0; r < 8; ++r) {
            bmp_dst[r] = src[r] ^ inv_mask;
        }

        // Actualizar color en Screen RAM
        *col_dst = color;

        col++;
        col_dst++;
        bmp_dst += 8;
    }
}

int send_command(uint8_t cmd) {
    if (cbm_k_ckout(MODEM_LFN)) {
        cbm_k_clrch();
        return 1;
    }
    cbm_k_bsout(cmd);
    cbm_k_bsout(0x0D);
    cbm_k_clrch();

    while (*RODBS != *RODBE)
        ;

    return 0;
}
