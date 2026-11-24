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

int main(void)
{
    // 300bauds 8,n,1
    const char config[2] = {6, 0};
    cbm_close(MODEM_DEVICE);
    cbm_open(MODEM_DEVICE, 2, 0, config);

    show_screen();

    while (!quit)
    {
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

void show_screen()
{
    int i;

    clrscr();
    bordercolor(COLOR_BLACK);
    bgcolor(COLOR_BLACK);
    textcolor(COLOR_GREEN);
    putchar(142);

    gotoxy(0, 0);
    printf("\x12  commodore 64 B 300bps B 8n1 B noctrl  \n");
    printf("  %c\x12  sensor dht22  \x92%c%c\x12sensor mpl3115a2\x92%c\n", 176, 174, 176, 174);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  temp:      c  %c%c  temp:      c  %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  hume:      %%  %c%c  pres:      h  %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  volt:      v  %c%c  alti:      m  %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c  volt:      v  %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    printf("  %c\x12 sensor nova pm \x92%c%cCCCCCCCCCCCCCCCC%c\n", 171, 179, 171, 179);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  pm2.5:      u %c%c xxx      xxxxx %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  pm10 :      u %c%c  x   xx    x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c  volt :      v %c%c  x  x  x   x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c  x  x  x   x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c  x  x  x   x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c  x  x  x   x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c xxx  xx    x   %c\r\n", 98, 98, 98, 98);
    cprintf("  %c                %c%c                %c\r\n", 98, 98, 98, 98);
    cprintf("  %cCCCCCCCCCCCCCCCC%c%cCCCCCCCCCCCCCCCC%c\r\n", 171, 177, 177, 179);
    cprintf("  %c                                  %c\r\n", 98, 98);
    cprintf("  %cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC%c", 173, 189);
}

void delay_ticks(clock_t ticks)
{
    clock_t now = clock();
    while ((clock() - now) < ticks)
        ;
}

void check_quit()
{
    if (kbhit() && cgetc() == CH_STOP)
        quit = 1;
}

int send_command(char *cmd, int timeout)
{
    unsigned char ch, status;
    int n, i;
    clock_t now;
    resp[0] = 0;

    gotoxy(4, 23);
    cprintf("%-4.4s: %-27.27s", cmd, "limpiando buffer");

    // extraemos lo que esta en el buffer
    now = clock();
    while (1)
    {
        if (cbm_k_chkin(MODEM_DEVICE) != 0)
        {
            cbm_k_clrch();
            gotoxy(4, 23);
            cprintf("%-4.4s: %-27.27s", cmd, "error en acceso al modem");
            return 0;
        }
        ch = cbm_k_getin();
        status = cbm_k_readst();
        cbm_k_clrch();

        if (status == 0x00)
        {
            now = clock();
            continue;
        }
        else if (status & 0x08)
        {
            if ((clock() - now) > 1 * 60)
                break;
        }
        else
        {
            return 0;
        }
    }

    gotoxy(4, 23);
    cprintf("%-4.4s: %-27.27s", cmd, "enviando comando");

    i = 0;
    now = clock();
    while (1)
    {
        if (cbm_k_chkin(MODEM_DEVICE) != 0)
        {
            cbm_k_clrch();
            gotoxy(4, 23);
            cprintf("%-4.4s: %-27.27s", cmd, "error en acceso al modem");
            return 0;
        }

        ch = cbm_k_getin();
        status = cbm_k_readst();
        cbm_k_clrch();

        if (status == 0x00)
        {
            if (ch == '\r')
            {
                resp[i] = 0;
                gotoxy(10, 23);
                cprintf("%-27.27s", resp);
                return i;
            }
            if (i < sizeof(resp) - 1)
            {
                resp[i++] = ch;
                now = clock();
            }
        }
        else if (status & 0x08)
        {
            if ((clock() - now) > timeout)
            {
                gotoxy(4, 23);
                cprintf("%-4.4s: %-27.27s", cmd, "timeout");
                break;
            }
        }
        else
        {
            gotoxy(4, 23);
            cprintf("%-4.4s: %-27.27s", cmd, "error en acceso al modem");
            break;
        }
    }

    resp[0] = 0;
    return 0;
}

int get_status()
{
    send_command("stat", 3 * 60);
    if (strcmp(resp, "+ready") == 0)
        return 1;

    gotoxy(10, 23);
    cprintf("%-27.27s", "respuesta invalida");
    delay_ticks(2 * 60);
}

int get_temperature() { return 0; }

int get_pressure() { return 0; }

int get_air_quaility() { return 0; }
