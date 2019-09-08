#include <stdio.h>
#include <sys/stat.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"

#define URL "https://tinfoil.io/repo/tinfoil.latest.zip" // don't change this blawar pls
#define APP_URL "https://github.com/ITotalJustice/tinfoil-updater/releases/latest/download/tinfoil-updater.nro"
#define AMS_URL "https://github.com/Atmosphere-NX/Atmosphere/releases/download/0.9.3/atmosphere-0.9.3-master-25218795+hbl-2.2+hbmenu-3.1.0.zip"
#define ROOT "/"
#define PATH "/switch/tinfoil-updater/"
#define OUTPUT "/switch/tinfoil-updater/tinfoil.zip"
#define APP_OUTPUT "/switch/tinfoil-updater/tinfoil-updater.nro"
#define AMS_OUTPUT "/switch/tinfoil-updater/ams.zip"

#define UP_ALL 0
#define UP_TINFOIL_FOLDER 1
#define UP_TINFOIL_NRO 2
#define UP_APP 3
#define UP_AMS 4

void refreshScreen(int cursor)
{
    consoleClear();
    printf("Tinfoil-Updater: v%.1f ^v^\n\n\n", 0.1);

    char *list[] = {"= FULL tinfoil update", "= tinfoil folder update", "= tinfoil.nro update only", "= update this app", "= update ams (EXPERIMENTAL! REBOOT AFTER INSTALL!)"};

    for (int i = 0; i < 5; i++)
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

    int cursor = 0, cursor_max = 4;
    refreshScreen(cursor);

    // muh loooooop
    while(appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // move cursor down...
        if (kDown & KEY_DOWN)
        {
            if (cursor == cursor_max) cursor = 0;
            else cursor++;

            refreshScreen(cursor);
        }

        // move cursor up...
        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = cursor_max;
            else cursor--;

            refreshScreen(cursor);
        }

        if (kDown & KEY_A)
        {
            switch (cursor)
            {
            case UP_ALL:
                downloadFile(URL, OUTPUT);
                unzip(OUTPUT, ROOT, UP_ALL);
                break;

            case UP_TINFOIL_FOLDER:
                downloadFile(URL, OUTPUT);
                unzip(OUTPUT, ROOT, UP_TINFOIL_FOLDER);
                break;

            case UP_TINFOIL_NRO:
                downloadFile(URL, OUTPUT);
                unzip(OUTPUT, ROOT, UP_TINFOIL_NRO);
                break;

            case UP_APP:
                downloadFile(APP_URL, APP_OUTPUT);
                break;

            case UP_AMS:
                downloadFile(AMS_URL, AMS_OUTPUT);
                unzip(AMS_OUTPUT, ROOT, UP_AMS);
                break;
            }
        }
        
        if (kDown & KEY_PLUS) break;
    }

    // cleanup then exit
    socketExit();
    consoleExit(NULL);
    return 0;
}