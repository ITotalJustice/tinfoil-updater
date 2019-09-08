#include <stdio.h>
#include <sys/stat.h>
#include <zlib.h>
#include <minizip/unzip.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#define WRITEBUFFERSIZE 8192
#define MAXFILENAME 256

int unzip(const char *output, const char *dir, int mode)
{
    // TODO, dont chdir
    chdir(dir);

    unzFile zfile = unzOpen(output);
    unz_global_info gi;
    unzGetGlobalInfo (zfile, &gi);

    for (int i = 0; i < gi.number_entry; i++)
    {
        char filename_inzip[MAXFILENAME];
        unz_file_info file_info;

        unzOpenCurrentFile(zfile);
        unzGetCurrentFileInfo(zfile, &file_info, filename_inzip, sizeof(filename_inzip), NULL, 0, NULL, 0);

        if (strcmp(filename_inzip, "switch/tinfoil/tinfoil.nro") == 0)
        {

        char *filename_withoutpath = filename_inzip;
        char *p = filename_withoutpath;

        while ((*p) != '\0')
        {
            if (((*p) == '/') || ((*p) == '\\'))
                filename_withoutpath = p + 1;
            p++;
        }

        if ((*filename_withoutpath) == '\0')
        {
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
            uInt size_buf = WRITEBUFFERSIZE;
            void *buf = malloc(size_buf);

            printf("writing file: %s\n", write_filename);
            consoleUpdate(NULL);

            for (int j = unzReadCurrentFile(zfile, buf, size_buf); j > 0; j = unzReadCurrentFile(zfile, buf, size_buf))
            {
                fwrite(buf, 1, j, outfile);
            }

            fclose(outfile);
            free(buf);
        }
        }

        unzCloseCurrentFile(zfile);
        unzGoToNextFile(zfile);
        consoleUpdate(NULL);
    }

    unzClose(zfile);
    printf("finished!\n");
    consoleUpdate(NULL);
}
