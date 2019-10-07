/* I wrote this header to be as portable as possible for other homebrew switch projects */
/* The only thing that needs to be changed will be the name / number of textures you want to load */
/* If you decide to use this header and add functions, please consider opening a pr for said functions */
/* I would greatly appreaciate it :) */

#ifndef _SDL_MEMES_H_
#define _SDL_MEMES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_W    1280                                                            //width of the window
#define SCREEN_H    720                                                             //hight of the window

#define white       0
#define grey        1
#define dark_grey   2
#define black       3
#define pink        4
#define neon_pink   5
#define hot_pink    6
#define orange      7
#define yellow      8
#define gold        9
#define brown       10
#define red         11
#define dark_red    12
#define green       13
#define lime_green  14
#define aqua        15
#define teal        16
#define light_blue  17
#define blue        18
#define jordy_blue  19
#define faint_blue  20
#define dark_blue   21
#define purple      22
#define indigo      23
#define beige       24

// buttons from NintendoExt
#define BUTTON_A            0xE0E0
#define BUTTON_B            0xE0E1
#define BUTTON_X            0xE0E2
#define BUTTON_Y            0xE0E3
#define BUTTON_L            0xE0E4
#define BUTTON_R            0xE0E5
#define BUTTON_ZL           0xE0E6
#define BUTTON_ZR           0xE0E7
#define BUTTON_SL           0xE0E8
#define BUTTON_SR           0xE0E9
#define BUTTON_UP           0xE0EB
#define BUTTON_DOWN         0xE0EC
#define BUTTON_LEFT         0xE0ED
#define BUTTON_RIGHT        0xE0EE
#define BUTTON_PLUS         0xE0EF
#define BUTTON_MINUS        0xE0F0
#define BUTTON_HOME         0xE0F4
#define BUTTON_SCREENSSHOT  0xE0F5
#define BUTTON_LS           0xE101
#define BUTTON_RS           0xE102
#define BUTTON_L3           0xE104
#define BUTTON_R3           0xE105

SDL_Texture *black_background, *screen_shot;
TTF_Font *fntSmall, *fntMedium, *fntLarge, *fntButton, *fntButtonBig;

SDL_Colour SDL_GetColour(int colour_option);                                            //pass the name of colour, returns the colour
SDL_Window* SDL_GetWindow(void);                                                        //get sdl window

void SDL_ImageLoad(SDL_Texture **texture, char *path);                                  //load image from texture
void SDL_ImageLoadMem(SDL_Texture **texture, void *data, int size);                     //load image from memory

void SDL_DrawText(TTF_Font *font, int x, int y, int colour, const char *text, ...);     //draw text to screen
void SDL_DrawButton(TTF_Font *font, u_int16_t btn, int x, int y, int colour);           //draw button to screen
void SDL_DrawImage(SDL_Texture *texture, int x, int y);                                 //draw image to screen from texture
void SDL_DrawImageScale(SDL_Texture *texture, int x, int y, int w, int h);              //scale the image drawn to screen
void SDL_DrawShape(int colour, int x, int y, int w, int h);                             //draw shap (rect)
void SDL_DrawCircle(int colour, int x, int y, int r);
void SDL_ScreenShot(SDL_Texture **texture);
void SDL_ScreenShotSave(SDL_Texture **texture, const char *save_path);

void SDL_LoadFonts(void);                                                               //load all fonts
void SDL_CloseFonts(void);                                                              //close all loaded fonts
void SDL_LoadTextures(void);                                                            //load all textures
void SDL_DestroyTextures(void);                                                         //destroy all textures

void SDL_ClearRenderer(void);                                                           //clear the screen
void SDL_UpdateRenderer(void);                                                          //update the screen

void sdlInit();                                                                         //init all sdl stuff
void sdlExit();                                                                         //clean and exit

#endif