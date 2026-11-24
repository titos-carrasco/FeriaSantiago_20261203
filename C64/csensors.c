#include <cbm.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MODEM_DEVICE 2
char resp[256];
int quit = 0;

void show_screen();
void delay_ticks();
void check_quit();
int send_command(char *cmd, int timeout);
int get_status();
int get_temperature();
int get_pressure();
int get_air_quaility();

int main(void) {
    // 300;8,n,1
    const char config[2] = {6, 0};
    cbm_close(MODEM_DEVICE);
    cbm_open(MODEM_DEVICE, 2, 0, config);

    show_screen();

    while (!quit) {
        if (!get_status())
            continue;
        get_temperature();
        get_pressure();
        get_air_quaility();
    }

    clrscr();
    cbm_close(MODEM_DEVICE);
    return 0;
}

void show_screen() {
    int i;

    clrscr();
    bordercolor(COLOR_BLACK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_GREEN);
    putchar(142);

    gotoxy(0, 0);
    revers(1);
    cputs("  commodore 64 B 300bps B 8n1 B noctrl  ");
    revers(0);

    gotoxy(2, 2);
    putchar(176);
    revers(1);
    cputs("  sensor dht22  ");
    revers(0);
    putchar(174);
    gotoxy(20, 2);
    putchar(176);
    revers(1);
    cputs("sensor mpl3115a2");
    revers(0);
    putchar(174);
    for (i = 0; i < 9; i++) {
        gotoxy(2, i + 3);
        putchar(98);
        gotoxy(19, i + 3);
        putchar(98);
        gotoxy(20, i + 3);
        putchar(98);
        gotoxy(37, i + 3);
        putchar(98);
    }

    gotoxy(2, 12);
    putchar(171);
    revers(1);
    cputs(" sensor nova pm ");
    revers(0);
    putchar(179);
    gotoxy(20, 12);
    putchar(171);
    for (i = 1; i < 17; i++)
        putchar(99);
    putchar(179);
    for (i = 0; i < 7; i++) {
        gotoxy(2, i + 13);
        putchar(98);
        gotoxy(19, i + 13);
        putchar(98);
        gotoxy(20, i + 13);
        putchar(98);
        gotoxy(37, i + 13);
        putchar(98);
    }
    gotoxy(2, 20);
    putchar(171);
    for (i = 1; i < 17; i++)
        putchar(99);
    putchar(177);
    putchar(177);
    for (i = 1; i < 17; i++)
        putchar(99);
    putchar(179);

    gotoxy(2, 21);
    putchar(98);
    gotoxy(37, 21);
    putchar(98);

    gotoxy(2, 22);
    putchar(173);
    for (i = 1; i < 35; i++)
        putchar(99);
    putchar(189);

    gotoxy(5, 4);
    cputs("temp:      c");
    gotoxy(5, 5);
    cputs("hume:      %");
    gotoxy(5, 6);
    cputs("volt:      v");

    gotoxy(22, 4);
    cputs("temp:      c");
    gotoxy(22, 5);
    cputs("pres:      h");
    gotoxy(22, 6);
    cputs("alti:      m");
    gotoxy(22, 7);
    cputs("volt:      v");

    gotoxy(4, 14);
    cputs("pm2.5:      u");
    gotoxy(4, 15);
    cputs("pm10 :      u");
    gotoxy(4, 16);
    cputs("volt :      v");

    gotoxy(22, 14);
    cputs("xxx      xxxxx");
    gotoxy(22, 15);
    cputs(" x   xx    x  ");
    gotoxy(22, 16);
    cputs(" x  x  x   x ");
    gotoxy(22, 17);
    cputs(" x  x  x   x ");
    gotoxy(22, 18);
    cputs("xxx  xx    x  ");
}

void delay_ticks(clock_t ticks) {
    clock_t now = clock();
    while ((clock() - now) < ticks)
        ;
}

void check_quit() {
    if (kbhit() && cgetc() == CH_STOP)
        quit = 1;
}

int send_command(char *cmd, int timeout) {
    clock_t now;

    gotoxy(4, 21);
    cprintf("%-4.4s: %-27.27s", cmd, " ");

    resp[0] = 0;
    if (cbm_k_chkin(MODEM_DEVICE) != 0)
        return 0;


    return 0;
}

int get_status() {
    send_command("stat", 3 * 60);
    if (strcmp(resp, "+ready") == 0)
        return 1;

    gotoxy(10, 21);
    cprintf("%-27.27s", "respuesta invalida");
    delay_ticks(2 * 60);
}

int get_temperature() { return 0; }

int get_pressure() { return 0; }

int get_air_quaility() { return 0; }
