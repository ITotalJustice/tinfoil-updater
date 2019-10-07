/* I wrote this header to be as portable as possible for other homebrew switch projects */
/* The only thing that needs to be changed will be the name / number of textures you want to load */
/* If you decide to use this header and add functions, please consider opening a pr for said functions */
/* I would greatly appreaciate it :) */

#include <SDL2/SDL_image.h>
#include <stdarg.h>
#include <switch.h>

#include "sdl.h"

static SDL_Window *main_window;
static SDL_Renderer *main_renderer;

SDL_Colour colours[] = {
    { 255, 255, 255 },          //white
    { 140, 140, 140, 255 },     //grey
    { 81, 81, 81 },             //dark grey
    { 0, 0, 0 },                //black
    { 255, 192, 203 },          //pink
    { 253, 52, 131, 1 },        //neon pink
    { 255, 105, 180 },          //hotpink
    { 255, 165, 0 },            //orange
    { 255, 255, 0 },            //yellow
    { 255, 215, 0 },            //gold
    { 139, 69, 19 },            //brown
    { 255, 0, 0 },              //red
    { 139, 0, 0 },              //dark red
    { 0, 128, 0 },              //green
    { 50, 205, 50 },            //lime green
    { 0, 255, 255 },            //aqua
    { 0, 128, 128 },            //teal
    { 0, 191, 255 },            //light blue
    { 0, 0, 255 },              //blue
    { 131, 177, 218 },          //jordy blue
    { 97, 115, 255 },           //faint blue
    { 28, 33, 73 },             //dark blue
    { 160, 32, 240 },           //purple
    { 75, 0, 130 },             //indigo
    { 245, 245, 220 },          //beige
    };

SDL_Colour SDL_GetColour(int colour_option)
{
    return colours[colour_option];
}

SDL_Window *SDL_GetWindow(void)
{
    return main_window;
}

void SDL_ImageLoad(SDL_Texture **texture, char *path)
{
	SDL_Surface *Surface = IMG_Load(path);
    SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA8888, 0);
    *texture = SDL_CreateTextureFromSurface(main_renderer, Surface);
	SDL_FreeSurface(Surface);
}

void SDL_ImageLoadMem(SDL_Texture **texture, void *data, int size)
{
	SDL_Surface *surface = IMG_Load_RW(SDL_RWFromMem(data, size), 1);
	if (surface) *texture = SDL_CreateTextureFromSurface(main_renderer, surface);
	SDL_FreeSurface(surface);
}

void SDL_DrawText(TTF_Font *font, int x, int y, int colour, const char *text, ...)
{
    char textBuffer[256];
    va_list argv;
    va_start(argv, text);
    vsnprintf(textBuffer, sizeof(textBuffer), text, argv);
    va_end(argv);

    SDL_Colour col = SDL_GetColour(colour);
    SDL_Surface *Surface = TTF_RenderText_Blended_Wrapped(font, textBuffer, col, SCREEN_W);
    SDL_Texture *Tex = SDL_CreateTextureFromSurface(main_renderer, Surface);
    SDL_Rect pos = { pos.x = x, pos.y = y, pos.w = Surface ->w, pos.h = Surface->h };

    SDL_RenderCopy(main_renderer, Tex, NULL, &pos);
    SDL_DestroyTexture(Tex);
    SDL_FreeSurface(Surface);
}

void SDL_DrawButton(TTF_Font *font, u_int16_t btn, int x, int y, int colour)
{
    SDL_Colour col = SDL_GetColour(colour);
    SDL_Surface *Surface = TTF_RenderGlyph_Blended(font, btn, col);
    SDL_Texture *Tex = SDL_CreateTextureFromSurface(main_renderer, Surface);
    SDL_Rect pos = { pos.x = x, pos.y = y, pos.w = Surface ->w, pos.h = Surface->h };

    SDL_RenderCopy(main_renderer, Tex, NULL, &pos);
    SDL_DestroyTexture(Tex);
    SDL_FreeSurface(Surface);
}

void SDL_DrawImage(SDL_Texture *texture, int x, int y)
{
    SDL_Rect pos = { pos.x = x, pos.y = y };
	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(main_renderer, texture, NULL, &pos);
}

void SDL_DrawImageScale(SDL_Texture *texture, int x, int y, int w, int h)
{
    SDL_Rect pos = { pos.x = x, pos.y = y, pos.w = w, pos.h = h };
	SDL_RenderCopy(main_renderer, texture, NULL, &pos);
}

void SDL_DrawShape(int colour, int x, int y, int w, int h)
{
    SDL_Colour col = SDL_GetColour(colour);
    SDL_Rect pos = { pos.x = x, pos.y = y, pos.w = w, pos.h = h };
    SDL_SetRenderDrawColor(main_renderer, col.r, col.g, col.b, col.a);
    SDL_RenderFillRect(main_renderer, &pos);
}

void SDL_DrawCircle(int colour, int x, int y, int r)
{
    SDL_Colour col = SDL_GetColour(colour);
    SDL_SetRenderDrawColor(main_renderer, col.r, col.g, col.b, col.a);

    //TODO//
}

void SDL_ScreenShot(SDL_Texture **texture)
{
    SDL_Surface *Surface = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(main_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, Surface->pixels, Surface->pitch);
    SDL_ConvertSurfaceFormat(Surface, SDL_PIXELFORMAT_RGBA8888, 0);
    *texture = SDL_CreateTextureFromSurface(main_renderer, Surface);
	SDL_FreeSurface(Surface);
}

void SDL_ScreenShotSave(SDL_Texture **texture, const char *save_path)
{
    SDL_Surface *Surface = SDL_CreateRGBSurface(0, SCREEN_W, SCREEN_H, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(main_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, Surface->pixels, Surface->pitch);
    SDL_SaveBMP(Surface, save_path);
	SDL_FreeSurface(Surface);
}

void SDL_LoadFonts()
{
    // Get the fonts from system
    PlFontData font;
    PlFontData button_data;
    plGetSharedFontByType(&font, PlSharedFontType_Standard);
    plGetSharedFontByType(&button_data, PlSharedFontType_NintendoExt);

    fntSmall        = TTF_OpenFontRW(SDL_RWFromMem(font.address, font.size), 1, 36);
    fntMedium       = TTF_OpenFontRW(SDL_RWFromMem(font.address, font.size), 1, 48);
    fntLarge        = TTF_OpenFontRW(SDL_RWFromMem(font.address, font.size), 1, 72);
    fntButton       = TTF_OpenFontRW(SDL_RWFromMem(button_data.address, button_data.size), 1, 30);
    fntButtonBig    = TTF_OpenFontRW(SDL_RWFromMem(button_data.address, button_data.size), 1, 36);
}

void SDL_CloseFonts()
{
    TTF_CloseFont(fntSmall);
    TTF_CloseFont(fntMedium);
    TTF_CloseFont(fntLarge);
    TTF_CloseFont(fntButton);
    TTF_CloseFont(fntButtonBig);
}

void SDL_LoadTextures()
{
    SDL_ImageLoad(&black_background, "romfs:/black_background.png");
}

void SDL_DestroyTextures()
{
    SDL_DestroyTexture(black_background);
    if (screen_shot) SDL_DestroyTexture(screen_shot);
}

void SDL_ClearRenderer()
{
    SDL_RenderClear(main_renderer);
    SDL_RenderCopy(main_renderer, black_background, NULL, NULL);
}

void SDL_UpdateRenderer()
{
    SDL_RenderPresent(main_renderer);
}

void sdlInit()
{
    TTF_Init();
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    main_window = SDL_CreateWindow("totaljustice", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    // highest quality
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    SDL_LoadFonts();
    SDL_LoadTextures();
}

void sdlExit()
{
    SDL_CloseFonts();
    SDL_DestroyTextures();
    SDL_DestroyRenderer(main_renderer);
    SDL_DestroyWindow(main_window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}