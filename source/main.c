#include <stdio.h>
#include <sys/stat.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"

#define URL "http://tinfoil.io/repo/tinfoil.latest.zip" // don't change this blawar pls
#define PATH "/switch/tinfoil-updater/"
#define PATHTEMP "/switch/tinfoil-updater/temp/"
#define OUTPUT "/switch/tinfoil-updater/temp/tinfoil.zip"

int main(int argc, char **argv)
{
    // init stuff
    socketInitializeDefault();
    consoleInit(NULL);

    // make paths
    mkdir(PATH, 0777);
    mkdir(PATHTEMP, 0777);

    // muh loooooop
    while(appletMainLoop())
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A)
        {
            if (downloadZIP(URL, OUTPUT) != 0) return 1;
            if (unzip(OUTPUT, PATHTEMP) != 0) return 1;
            printf("nice\n");
        }
        
        if (kDown & KEY_PLUS) break;

        consoleUpdate(NULL);
    }

    // cleanup then exit
    socketExit();
    consoleExit(NULL);
    return 0;
}