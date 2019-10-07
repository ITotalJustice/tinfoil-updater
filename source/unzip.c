#include <stdio.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#include "unzip.h"
#include "menu.h"

#define WRITEBUFFERSIZE     15000000 // 15MB
#define MAXFILENAME         256


int unzip(const char *output, int mode)
{
    unzFile zfile = unzOpen(output);
    unz_global_info gi;
    unzGetGlobalInfo(zfile, &gi);

    for (int i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[MAXFILENAME];
        unz_file_info file_info;

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);
        
        // don't overwrite tinfoil config.
        if (!strcmp(filename_inzip, "switch/tinfoil/options.json"))
        {
            // check if the file exists.
            FILE *fp = fopen("/tinfoil/options.json", "r");
            if (fp) fclose(fp);
            else goto jump_to_end;
        }

        if (mode == UP_TINFOIL_FOLDER && !strstr(filename_inzip, "/tinfoil/")) goto jump_to_end;
        else if (mode == UP_TINFOIL_NRO && strcmp(filename_inzip, "switch/tinfoil/tinfoil.nro")) goto jump_to_end;

        SDL_RenderClear(SDL_GetRenderer(SDL_GetWindow()));
        SDL_RenderCopy(SDL_GetRenderer(SDL_GetWindow()), screen_shot, NULL, NULL);

        // check if the string ends with a /, if so, then its a directory.
        if ((filename_inzip[strlen(filename_inzip) - 1]) == '/')
        {
            mkdir(filename_inzip, 0777);
            empty_pop_up_box();
            SDL_DrawText(fntSmall, 350, 250, white, "creating directory: \n\n%s", filename_inzip);
            SDL_UpdateRenderer();
        }

        else
        {
            const char *write_filename = filename_inzip;
            FILE *outfile = fopen(write_filename, "wb");
            if (outfile)
            {
                void *buf = malloc(WRITEBUFFERSIZE);

                empty_pop_up_box();
                SDL_DrawText(fntSmall, 350, 250, white, "writing file: \n\n%s", write_filename);
                SDL_UpdateRenderer();

                for (int j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
                {
                    fwrite(buf, 1, j, outfile);
                }

                fclose(outfile);
                free(buf);
            }
        }

        jump_to_end: // goto
        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
    }

    unzClose(zfile);
    remove(output);

    return 0;
}
