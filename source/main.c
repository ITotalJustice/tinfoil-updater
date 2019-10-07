#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"
#include "menu.h"
#include "sdl.h"


void appInit()
{
    socketInitializeDefault();
    #ifdef DEBUG
    nxlinkStdio();
    #endif
    plInitialize();
    romfsInit();
    sdlInit();
    romfsExit();
}

void appExit()
{
    socketExit();
    plExit();
    sdlExit();
}

int main(int argc, char **argv)
{
    // init stuff
    appInit();
    chdir(ROOT);

    // make paths
    mkdir(APP_PATH, 0777);

    main_menu();

    // cleanup then exit
    appExit();
    return 0;
}