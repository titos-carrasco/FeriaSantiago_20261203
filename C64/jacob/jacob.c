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

int main(void)
{
    // Dispositivo 2 = modem en userport, 300bauds 8,n,1
    const char config[2] = {6, 0};

    show_screen();

    cbm_k_close(MODEM_LFN);
    cbm_k_clrch();
    cbm_k_setlfs(MODEM_LFN, 2, 0);
    cbm_k_setnam(config);
    cbm_k_open();

    while (!quit)
    {
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
    printf("\x12  jacob robot  B 300bps B 8n1 B noctrl  \n");
}

void delay_ticks(clock_t ticks)
{
    clock_t now = clock();
    while ((clock() - now) < ticks)
        ;
}

int check_quit()
{
    if (kbhit() && cgetc() == 27)
        quit = 1;
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
