#ifndef _MENU_H_
#define _MENU_H_

#include "sdl.h"

#define OFF                 0
#define ON                  1

#define NO                  0
#define YES                 1

#define CURSOR_LIST_MAX     4
#define HIGHLIGHT_BOX_MIN   20
#define HIGHLIGHT_BOX_MAX   70
#define NEWLINE             130
#define FIRST_LINE          130
#define MAX_STRLEN          512

#define ROOT                "/"
#define APP_PATH            "/switch/tinfoil-updater/"
#define APP_OUTPUT          "/switch/tinfoil-updater/tinfoil-updater.nro"
#define OLD_APP_PATH        "/switch/tinfoil-updater.nro"
#define TINFOIL_OUTPUT      "/switch/tinfoil-updater/tinfoil.zip"
#define TEMP_FILE           "/switch/tinfoil-updater/temp"

void empty_pop_up_box();
void pop_up_box(TTF_Font *font, int x, int y, int colour, const char *text);    // pop-up box in center of screen
int yes_no_box(int x, int y, const char *question);                   // box with yes or no option
void error_box(int x, int y, const char *errorText);                            // used for displaying error messages
void main_menu();

#endif