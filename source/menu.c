#include <unistd.h>
#include <switch.h>

#include "download.h"
#include "unzip.h"
#include "menu.h"

#define APP_VERSION "0.3.1"


void empty_pop_up_box()
{
    // outline. box
    SDL_DrawShape(black, (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    SDL_DrawShape(dark_blue, SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
}

void pop_up_box(TTF_Font *font, int x, int y, int colour, const char *text)
{
    // outline. box
    SDL_DrawShape(black, (SCREEN_W/4)-5, (SCREEN_H/4)-5, (SCREEN_W/2)+10, (SCREEN_H/2)+10);
    // popup box.
    SDL_DrawShape(dark_blue, SCREEN_W/4, SCREEN_H/4, SCREEN_W/2, SCREEN_H/2);
    // text to draw.
    SDL_DrawText(font, x, y, colour, text);
}

int yes_no_box(int x, int y, const char *question)
{
    pop_up_box(fntMedium, x, y, white, question);

    // highlight box.
    SDL_DrawShape(faint_blue, 380, 410, 175, 65);
    SDL_DrawShape(faint_blue, 700, 410, 190, 65);
    // option text.
    SDL_DrawButton(fntButtonBig, BUTTON_B, 410, 425, white);
    SDL_DrawText(fntMedium, 455, 425, white, "No");
    SDL_DrawButton(fntButtonBig, BUTTON_A, 725, 425, white);
    SDL_DrawText(fntMedium, 770, 425, white, "Yes");

    SDL_UpdateRenderer();

    while (1)
    {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_A) return YES;
        if (kDown & KEY_B) return NO;
    }
}

void error_box(int x, int y, const char *error_text)
{
    pop_up_box(fntMedium, x, y, white, error_text);
    SDL_UpdateRenderer();

    sleep(3);
}

void print_options_list(const char **options, int cursor, int list_move, int list_max, int y_start, int y_newline)
{
    SDL_ClearRenderer();

    for (int i = 0, j = list_move, nl = y_start; i < list_max && i < 7; i++, j++, nl+=y_newline)
    {
        if (j == cursor)
        {
            SDL_DrawText(fntSmall, 450, nl, grey, options[j]);
            SDL_DrawShape(white, 450, nl+50, 450, 5);
        }
        else
            SDL_DrawText(fntSmall, 450, nl, white, options[j]);
    }
}

void print_menu_tabs(const char **tabs, int in_tab, int cursor, int list_max)
{
    for (int i = 0, nl = 125; i < list_max; i++, nl+=90)
    {
        if (i == cursor)
        {
            SDL_DrawShape(white, 50, nl+50, 250, 5);
            if (in_tab)
                SDL_DrawText(fntSmall, 50, nl, grey, tabs[i]);
            else
                SDL_DrawText(fntSmall, 50, nl, white, tabs[i]);
        }
        else
            SDL_DrawText(fntSmall, 50, nl, white, tabs[i]);
    }
}

void print_title(const char *url)
{
    SDL_DrawText(fntSmall, 50, 35, white, "Tinfoil Updater: v%s", APP_VERSION);
    SDL_DrawText(fntSmall, 650, 35, white, "Downloading From: %s", url);
}

int move_cursor_up(int cursor, int cursor_max)
{
    if (cursor == 0) cursor = cursor_max;
    else cursor--;
    return cursor;
}

int move_cursor_down(int cursor, int cursor_max)
{
    if (cursor == cursor_max) cursor = 0;
    else cursor++;
    return cursor;
}

void main_menu()
{
    const char *main_options[]   = { "FULL Tinfoil Update", \
                                     "Tinfoil Folder Update", \
                                     "Tinfoil.nro Update Only", \
                                     "Update app" };

    const char *legacy_options[] = { "Tinfoil v5.0.0", \
                                     "Tinfoil v4.1.0", \
                                     "Tinfoil v4.0.0", \
                                     "Tinfoil v3.0.2", \
                                     "Tinfoil v3.0.1", \
                                     "Tinfoil v3.0.0", \
                                     "Tinfoil v2.1.0", \
                                     "Tinfoil v2.0.1", \
                                     "Tinfoil v2.0.0" };

    const char *legacy_url[]    =  { legacy500, \
                                     legacy410, \
                                     legacy400, \
                                     legacy302, \
                                     legacy301, \
                                     legacy300, \
                                     legacy210, \
                                     legacy201, \
                                     legacy200 };

    const char *tinmod_options[] = { "TinfoilMod + Tinfoil Folder Update", "TinfoilMod.nro Update Only" };

    const char *menu_tabs_list[] = { "Tinfoil", "Legacy Tinfoil", "TinfoilMod" };

    const char *url_list[] = { "tinfoil.io", "hbgshop.ga" };

    const char *url[] = { TINFOIL_URL, HBG_URL };

    int url_location = 0;
    int list_move = 0;

    int main_cursor = 0;
    int legacy_cursor = 0;
    int tinmod_cursor = 0;
    int tab_cursor = 0;

    int in_tab = 0;

    while (appletMainLoop())
    {
        // scan for input.
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        // render screen.
        if (tab_cursor == 0)
            print_options_list(main_options, main_cursor, 0, CURSOR_LIST_MAX, 150, NEWLINE);
        else if (tab_cursor == 1)
            print_options_list(legacy_options, legacy_cursor, list_move, 8, 120, 75);
        else if (tab_cursor == 2)
        {
            print_options_list(tinmod_options, tinmod_cursor, 0, 2, 150, 100);
            SDL_DrawText(fntSmall, 450, 450, white, "ADD WARNING TEXT HERE\n\nADD LINK TO TINMOD GITHUB\n");
        }

        print_menu_tabs(menu_tabs_list, in_tab, tab_cursor, 3);
        print_title(url_list[url_location]);

        // move cursor down...
        if (kDown & KEY_DOWN)
        {
            if (in_tab)
                tab_cursor = move_cursor_down(tab_cursor, 2);

            else if (tab_cursor == 0)
                main_cursor = move_cursor_down(main_cursor, CURSOR_LIST_MAX - 1);

            else if (tab_cursor == 1)
            {
                legacy_cursor = move_cursor_down(legacy_cursor, 8);

                if (legacy_cursor == 6 + list_move + 1) list_move++;
                else if (legacy_cursor == 0) list_move = 0;
            }

            else if (tab_cursor == 2)
                tinmod_cursor = move_cursor_down(tinmod_cursor, 2 - 1);
        }

        // move cursor up...
        if (kDown & KEY_UP)
        {
            if (in_tab)
                tab_cursor = move_cursor_up(tab_cursor, 2);

            else if (tab_cursor == 0)
                main_cursor = move_cursor_up(main_cursor, CURSOR_LIST_MAX - 1);

            else if (tab_cursor == 1)
            {
                legacy_cursor = move_cursor_up(legacy_cursor, 8);

                if (legacy_cursor == list_move-1) list_move--;
                else if (legacy_cursor == 8) list_move = legacy_cursor - 6;
            }

            else if (tab_cursor == 2)
                tinmod_cursor = move_cursor_up(tinmod_cursor, 2 - 1);
        }

        // enter menu tab.
        if (kDown & KEY_LEFT) in_tab = 1;

        // exit menu tab.
        if (kDown & KEY_RIGHT) in_tab = 0;

        // select option.
        if (kDown & KEY_A)
        {
            SDL_ScreenShot(&screen_shot);

            if (in_tab) in_tab = 0;

            else if (tab_cursor == 1)
            {
                if (YES == yes_no_box(400, 250, legacy_options[legacy_cursor]))
                    downloadFile(legacy_url[legacy_cursor], "/switch/tinfoil/tinfoil.nro");
            }
            
            else if (tab_cursor == 0)
            {
                switch (main_cursor)
                {
                case UP_ALL:
                    if (YES == yes_no_box(400, 250, "FULL Tinfoil Update?"))
                        if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                            unzip(TINFOIL_OUTPUT, UP_ALL);
                    break;

                case UP_TINFOIL_FOLDER:
                    if (YES == yes_no_box(400, 250, "Update Tinfoil Folder?"))
                        if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                            unzip(TINFOIL_OUTPUT, UP_TINFOIL_FOLDER);
                    break;

                case UP_TINFOIL_NRO:
                    if (YES == yes_no_box(400, 250, "Update Tinfoil NRO?"))
                        if (!downloadFile(url[url_location], TINFOIL_OUTPUT))
                            unzip(TINFOIL_OUTPUT, UP_TINFOIL_NRO);
                    break;

                case UP_APP:
                    if (YES == yes_no_box(400, 250, "Update App?"))
                        if (!downloadFile(APP_URL, APP_OUTPUT))
                            remove(OLD_APP_PATH);
                    break;
                }
            }
        }

        // change download url.
        if (kDown & KEY_X)
        {
            if (url_location == 0) url_location = 1;
            else url_location = 0;
        }

        // exit.
        if (kDown & KEY_PLUS) break;

        // update the renderer buffer.
        SDL_UpdateRenderer();
    }
}