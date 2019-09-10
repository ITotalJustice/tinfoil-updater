#include <stdio.h>
#include <zlib.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#define WRITEBUFFERSIZE     10000000 // 10MB
#define MAXFILENAME         256

#define UP_ALL              0
#define UP_TINFOIL_FOLDER   1
#define UP_TINFOIL_NRO      2
#define UP_APP              3

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
        
        // don't overwrite tinfoil config...
        if (strstr(filename_inzip, "/tinfoil/options.json")) goto jump_to_end;

        if (mode == UP_TINFOIL_FOLDER && !strstr(filename_inzip, "/tinfoil/")) goto jump_to_end;
        else if (mode == UP_TINFOIL_NRO && strcmp(filename_inzip, "switch/tinfoil/tinfoil.nro")) goto jump_to_end;

        // check if the string ends with a /, if so, then its a directory.
        if ((filename_inzip[strlen(filename_inzip) - 1]) == '/')
        {
            // check if directory exists
            DIR *dir = opendir(filename_inzip);
            if (dir) closedir(dir);
            else
            {
                printf("creating directory: %s\n", filename_inzip);
                mkdir(filename_inzip, 0777);
            }
        }

        else
        {
            const char *write_filename = filename_inzip;
            FILE *outfile = fopen(write_filename, "wb");
            if (outfile)
            {
                void *buf = malloc(WRITEBUFFERSIZE);

                printf("writing file: %s\n", write_filename);
                consoleUpdate(NULL);

                for (int j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE); j > 0; j = unzReadCurrentFile(zfile, buf, WRITEBUFFERSIZE))
                    fwrite(buf, 1, j, outfile);

                fclose(outfile);
                free(buf);
            }
        }

        jump_to_end: // goto
        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
        consoleUpdate(NULL);
    }

    unzClose(zfile);
    remove(output);
    printf("\nfinished!\n");
    consoleUpdate(NULL);

    return 0;
}
