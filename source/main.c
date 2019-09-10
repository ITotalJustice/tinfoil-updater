#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <switch.h>

#include "includes/download.h"
#include "includes/unzip.h"

#define TINFOIL_URL         "https://tinfoil.io/repo/tinfoil.latest.zip"
#define HBG_URL             "https://thehbg.shop/files/hbgshop.latest.zip"
#define APP_URL             "https://github.com/ITotalJustice/tinfoil-updater/releases/latest/download/tinfoil-updater.nro"

#define ROOT                "/"
#define APP_PATH            "/switch/tinfoil-updater/"
#define TINFOIL_OUTPUT      "/switch/tinfoil-updater/tinfoil.zip"
#define APP_OUTPUT          "/switch/tinfoil-updater/tinfoil-updater.nro"

void refreshScreen(int cursor, int url_location)
{
    consoleClear();

    char *url_list[] = {"tinfoil.io", "thehbg.shop"};
    char *option_list[] = {"= FULL tinfoil update", "= tinfoil folder update", "= tinfoil.nro update only", "= update this app"};

    printf("Tinfoil-Updater: v%.1f.\tDownloading from: %s\n\n\n", 0.3, url_list[url_location]);
    printf("Press (X) to change download URL\n\n\n");

    for (int i = 0; i < 4; i++)
    {
        if (cursor != i) printf("[ ] %s\n\n", option_list[i]);
        else printf("[X] %s\n\n", option_list[i]);
    }

    consoleUpdate(NULL);
}

int main(int argc, char **argv)
{
    // init stuff
    socketInitializeDefault();
    consoleInit(NULL);
    chdir(ROOT);

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
                unzip(TINFOIL_OUTPUT, UP_ALL);
                break;

            case UP_TINFOIL_FOLDER:
                downloadFile(url[url_location], TINFOIL_OUTPUT);
                unzip(TINFOIL_OUTPUT, UP_TINFOIL_FOLDER);
                break;

            case UP_TINFOIL_NRO:
                downloadFile(url[url_location], TINFOIL_OUTPUT);
                unzip(TINFOIL_OUTPUT, UP_TINFOIL_NRO);
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