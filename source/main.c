#include <stdio.h>
#include <sys/stat.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"

#define TINFOIL_URL "https://tinfoil.io/repo/tinfoil.latest.zip" // don't change this blawar pls
#define HBG_URL "https://thehbg.shop/files/hbgshop.latest.zip"
#define APP_URL "https://github.com/ITotalJustice/tinfoil-updater/releases/latest/download/tinfoil-updater.nro"

#define ROOT "/"
#define APP_PATH "/switch/tinfoil-updater/"
#define TINFOIL_OUTPUT "/switch/tinfoil-updater/tinfoil.zip"
#define APP_OUTPUT "/switch/tinfoil-updater/tinfoil-updater.nro"

#define UP_ALL 0
#define UP_TINFOIL_FOLDER 1
#define UP_TINFOIL_NRO 2
#define UP_APP 3

void refreshScreen(int cursor, int url_location)
{
    consoleClear();

    char *url[] = {"tinfoil.io", "thehbg.shop"};
    char *list[] = {"= FULL tinfoil update", "= tinfoil folder update", "= tinfoil.nro update only", "= update this app"};

    printf("Tinfoil-Updater: v%.1f.\tDownloading from: %s\n\n\n", 0.2, url[url_location]);

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
    mkdir(APP_PATH, 0777);

    char *url[] = {TINFOIL_URL, HBG_URL};
    int url_location = 0;
    int cursor = 0, cursor_max = 3;
    refreshScreen(cursor, url_location);

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

            refreshScreen(cursor, url_location);
        }

        // move cursor up...
        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = cursor_max;
            else cursor--;

            refreshScreen(cursor, url_location);
        }

        if (kDown & KEY_A)
        {
            switch (cursor)
            {
            case UP_ALL:
                downloadFile(url[url_location], TINFOIL_OUTPUT);
                unzip(TINFOIL_OUTPUT, ROOT, UP_ALL);
                break;

            case UP_TINFOIL_FOLDER:
                downloadFile(url[url_location], TINFOIL_OUTPUT);
                unzip(TINFOIL_OUTPUT, ROOT, UP_TINFOIL_FOLDER);
                break;

            case UP_TINFOIL_NRO:
                downloadFile(url[url_location], TINFOIL_OUTPUT);
                unzip(TINFOIL_OUTPUT, ROOT, UP_TINFOIL_NRO);
                break;

            case UP_APP:
                downloadFile(APP_URL, APP_OUTPUT);
                break;
            }
        }

        if (kDown & KEY_X)
        {
            if (url_location == 0) url_location = 1;
            else url_location = 0;
            refreshScreen(cursor, url_location);
        }
        
        if (kDown & KEY_PLUS) break;
    }

    // cleanup then exit
    socketExit();
    consoleExit(NULL);
    return 0;
}