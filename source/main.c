#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <switch.h>

#include "includes/download.h"
#include "includes/unzip.h"

#define ROOT                "/"
#define APP_PATH            "/switch/tinfoil-updater/"
#define APP_OUTPUT          "/switch/tinfoil-updater/tinfoil-updater.nro"
#define OLD_APP_PATH        "/switch/tinfoil-updater.nro"
#define TINFOIL_OUTPUT      "/switch/tinfoil-updater/tinfoil.zip"
#define TEMP_FILE           "/switch/tinfoil-updater/temp"

#define APP_VERSION         "0.3.1"
#define ON                  1
#define OFF                 0
#define CURSOR_MAX_LIST     3


void refreshScreen(int cursor, int url_location)
{
    consoleClear();

    char *url_list[] = {"tinfoil.io", "thehbg.shop"};
    char *option_list[] = {"= FULL tinfoil update", "= Tinfoil folder update", "= Tinfoil.nro update only", "= Update this app"};

    printf("\x1B[36mTinfoil-Updater: v%s.\x1B[37m\tDownloading from: \x1B[33m%s\x1B[37m\n\n\n", APP_VERSION, url_list[url_location]);
    printf("Press (X) to change download URL\n\n");
    printf("Press (Y) to for help\n\n");
    printf("Press (+) to exit\n\n\n");

    for (int i = 0; i < (CURSOR_MAX_LIST+1); i++)
    {
        if (cursor != i) printf("[ ] %s\n\n", option_list[i]);
        else printf("[X] %s\n\n", option_list[i]);
    }
    consoleUpdate(NULL);
}

void helpScreen(int cursor, int url_location)
{
    consoleClear();

    printf("\n\n\x1B[33mFULL tinfoil update:\x1B[37m\n\nUnzips the entire zip file, everthing inside will be copied over.\n\n\n\n\n\n");
    printf("\x1B[33mTinfoil folder update:\n\n\x1B[37mUnzips the contents of the tinfoil folder from the zip file.\n\n");
    printf("If downloading from hbgshop.ga, this will include the locations.conf.\n\n\n\n\n\n");
    printf("\x1B[33mTinfoil.nro update only:\x1B[37m\n\nThis only updates the tinfoil.nro\n\n\n\n\n\n");
    printf("\x1B[33mUpdate this app:\x1B[37m\n\nThis option downloads the latest version of this app from github!\n\n\n\n\n\n\n");

    printf("If you encounter a bug, or have a suggestion, open an issue of my github:\n\n");
    printf("https://github.com/ITotalJustice/tinfoil-updater/issues.\n\n\n\n");

    printf("\x1B[33m*** Press any key to exit this help screen... ***");

    consoleUpdate(NULL);

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown)
        {
            refreshScreen(cursor, url_location);
            break;
        }
    }
    
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
    short url_location = 0;
    short cursor = 0;
    short help_mode = OFF;
    refreshScreen(cursor, url_location);

    // muh loooooop
    while(appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // move cursor down...
        if (kDown & KEY_DOWN)
        {
            if (cursor == CURSOR_MAX_LIST) cursor = 0;
            else cursor++;
            refreshScreen(cursor, url_location);
        }

        // move cursor up...
        if (kDown & KEY_UP)
        {
            if (cursor == 0) cursor = CURSOR_MAX_LIST;
            else cursor--;
            refreshScreen(cursor, url_location);
        }

        if (kDown & KEY_A && help_mode == OFF)
        {
            switch (cursor)
            {
            case UP_ALL:
                if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                    unzip(TINFOIL_OUTPUT, UP_ALL);
                break;

            case UP_TINFOIL_FOLDER:
                if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                    unzip(TINFOIL_OUTPUT, UP_TINFOIL_FOLDER);
                break;

            case UP_TINFOIL_NRO:
                if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                    unzip(TINFOIL_OUTPUT, UP_TINFOIL_NRO);
                break;

            case UP_APP:
                if (!downloadFile(APP_URL, APP_OUTPUT))
                {
                    FILE *f = fopen(OLD_APP_PATH, "r");
                    if (f) 
                    {
                        fclose(f);
                        remove(OLD_APP_PATH);
                    }
                    else fclose(f);
                }
                break;
            }
        }

        if (kDown & KEY_X)
        {
            if (url_location == 0) url_location = 1;
            else url_location = 0;
            refreshScreen(cursor, url_location);
        }

        if (kDown & KEY_Y) helpScreen(cursor, url_location);
        
        if (kDown & KEY_PLUS) break;
    }

    // cleanup then exit
    socketExit();
    consoleExit(NULL);
    return 0;
}