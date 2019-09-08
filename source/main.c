#include <stdio.h>
#include <sys/stat.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"

#define URL "http://tinfoil.io/repo/tinfoil.latest.zip" // don't change this blawar pls
#define ROOT "/"
#define PATH "/switch/tinfoil-updater/"
#define OUTPUT "/switch/tinfoil-updater/tinfoil.zip"
#define APPOUTPUT "/switch/tinfoil-updater/tinfoil-updater.nro"

#define UPALL 0
#define UPTINFOILFOLDER 1
#define UPTINFOILNRO 2
#define UPAPP 3

void refreshScreen(int cursor)
{
    consoleClear();
    printf("Tinfoil-Updater: v%.1f ^v^\n\n\n", 0.1);

    char *list[] = {"= FULL tinfoil update", "= tinfoil folder update", "= tinfoil.nro update only", "= update this app"};

    for (int i = 0; i < 4; i++)
    {
        if (cursor != i) printf("[ ] %s\n\n", list[i]);
        else printf("[x] %s\n\n", list[i]);
    }
    consoleUpdate(NULL);
}

int main(int argc, char **argv)
{
    // init stuff
    socketInitializeDefault();
    consoleInit(NULL);

    // make paths
    mkdir(PATH, 0777);

    int cursor = 0, cursor_max = 3;
    refreshScreen(cursor);

    // muh loooooop
    while(appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_DOWN)
        {
            if (cursor == cursor_max) cursor = 0;
            else cursor++;

            refreshScreen(cursor);
        }

        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = cursor_max;
            else cursor--;

            refreshScreen(cursor);
        }

        if (kDown & KEY_A)
        {
            if (cursor == UPTINFOILNRO)
            {
                if (downloadFile(URL, OUTPUT) != 0) return 1;
                if (unzip(OUTPUT, ROOT, UPTINFOILNRO) != 0) return 1;
            }
        }
        
        if (kDown & KEY_PLUS) break;
    }

    // cleanup then exit
    socketExit();
    consoleExit(NULL);
    return 0;
}