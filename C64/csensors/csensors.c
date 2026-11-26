#include <cbm.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MODEM_LFN 2
char resp[256];
int quit = 0;

void show_screen();
void delay_ticks(clock_t ticks);
int check_quit();
int send_command(char *cmd, int timeout);
int get_status();
int get_temperature();
int get_pressure();
int get_air_quaility();

int main(void)
{
    // Dispositivo 2 = modem en userport, 300bauds 8,n,1
    const char config[2] = {6, 0};

    show_screen();

    gotoxy(4, 23);
    cbm_k_close(MODEM_LFN);
    cbm_k_clrch();
    cbm_k_setlfs(MODEM_LFN, 2, 0);
    cbm_k_setnam(config);
    cbm_k_open();

    while (!quit)
    {
        if (get_status())
            continue;
        get_temperature();
        get_pressure();
        get_air_quaility();

        gotoxy(4, 23);
        cprintf("%-33.33s", "durmiendo ...");
        delay_ticks(5 * 60);
    }

    clrscr();
    cbm_k_close(MODEM_LFN);
    return 0;
}

void show_screen()
{
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

int check_quit()
{
    if (kbhit())
    {
        unsigned char k = cgetc();
        if (k == CH_STOP || k == 95 || k == 'q')
            quit = 1;
    }
    return quit;
}

int send_command(char *cmd, int timeout)
{
    unsigned char ch, status;
    int i;
    clock_t now;
    resp[0] = 0;

    // extraemos los residuos del buffer
    gotoxy(4, 23);
    cprintf("%-4.4s: %-27.27s", cmd, "limpiando residuos");
    if (cbm_k_chkin(MODEM_LFN) != 0)
    {
        cbm_k_clrch();
        gotoxy(4, 23);
        cprintf("%-4.4s: %-27.27s", cmd, "error en acceso al modem");
        return 1;
    }
    now = clock();
    while (1)
    {
        ch = cbm_k_getin();
        status = cbm_k_readst() & 0x0F;
        if (status & 0x08)
        {
            if ((clock() - now) > 1 * 60)
                break;
        }
        else
            now = clock();
    }
    cbm_k_clrch();

    // enviamos el comando
    gotoxy(4, 23);
    cprintf("%-4.4s: %-27.27s", cmd, "enviando comando");
    if (cbm_k_ckout(MODEM_LFN))
    {
        cbm_k_clrch();
        gotoxy(4, 23);
        cprintf("%-4.4s: %-27.27s", cmd, "error al activar envio");
        return 1;
    }
    i = 0;
    while (cmd[i] != 0)
    {
        cbm_k_bsout(cmd[i++]);
    }
    cbm_k_bsout(0x0D);
    cbm_k_clrch();

    // leemos la respuesta
    gotoxy(4, 23);
    cprintf("%-4.4s: %-27.27s", cmd, "leyendo respuesta");
    if (cbm_k_chkin(MODEM_LFN) != 0)
    {
        cbm_k_clrch();
        gotoxy(4, 23);
        cprintf("%-4.4s: %-27.27s", cmd, "error en acceso al modem");
        return 1;
    }
    i = 0;
    now = clock();
    while (1)
    {
        ch = cbm_k_getin();
        status = cbm_k_readst() & 0x0F;
        if (status == 0x00)
        {
            if (ch == 0x0D)
            {
                cbm_k_clrch();
                resp[i] = 0;
                gotoxy(10, 23);
                cprintf("%-27.27s", resp);
                return 0;
            }
            if (i < sizeof(resp) - 2)
            {
                resp[i++] = ch;
                now = clock();
            }
            else
            {
                cbm_k_clrch();
                resp[i] = 0;
                gotoxy(10, 23);
                cprintf("%-27.27s", resp);
                return 0;
            }
        }
        else if (status & 0x08)
        {
            if ((clock() - now) > timeout)
            {
                cbm_k_clrch();
                gotoxy(4, 23);
                cprintf("%-4.4s: %-27.27s", cmd, "timeout");
                break;
            }
        }
    }
    cbm_k_clrch();

    resp[i] = 0;
    return 1;
}

int get_status()
{
    send_command("stat", 3 * 60);
    if (strcmp(resp, "+ready") == 0)
        return 0;
    delay_ticks(2 * 60);
    return 1;
}

int get_temperature()
{
    send_command("temp", 3 * 60);
    if (strncmp(resp, "+temp", 5) != 0)
        return 0;

    gotoxy(11, 4);
    cprintf("%.*s", 5, resp + 6);
    gotoxy(11, 5);
    cprintf("%.*s", 5, resp + 12);
    gotoxy(11, 6);
    cprintf("%.*s", 5, resp + 18);
    gotoxy(6, 9);
    cprintf("%.*s", 10, resp + 24);
    gotoxy(7, 10);
    cprintf("%.*s", 8, resp + 35);

    return 1;
}

int get_pressure()
{
    send_command("pres", 3 * 60);
    if (strncmp(resp, "+pres", 5) != 0)
        return 0;

    gotoxy(28, 4);
    cprintf("%.*s", 6, resp + 6);
    gotoxy(28, 5);
    cprintf("%.*s", 6, resp + 13);
    gotoxy(28, 6);
    cprintf("%.*s", 6, resp + 20);
    gotoxy(28, 7);
    cprintf("%.*s", 6, resp + 27);
    gotoxy(24, 9);
    cprintf("%.*s", 10, resp + 34);
    gotoxy(25, 10);
    cprintf("%.*s", 8, resp + 45);

    return 1;
}

int get_air_quaility()
{
    send_command("nova", 3 * 60);
    if (strncmp(resp, "+nova", 5) != 0)
        return 0;

    gotoxy(11, 14);
    cprintf("%.*s", 6, resp + 6);
    gotoxy(11, 15);
    cprintf("%.*s", 6, resp + 13);
    gotoxy(11, 16);
    cprintf("%.*s", 6, resp + 20);
    gotoxy(6, 19);
    cprintf("%.*s", 10, resp + 27);
    gotoxy(7, 20);
    cprintf("%.*s", 8, resp + 38);

    return 1;
}
