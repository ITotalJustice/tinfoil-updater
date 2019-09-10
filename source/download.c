#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <switch.h>

#define Megabytes_in_Bytes	1048576
#define API_AGENT           "ITotalJustice"

int download_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
{	
    printf("* DOWNLOADING: %.2fMB of %.2fMB *\r", dlnow / Megabytes_in_Bytes, dltotal / Megabytes_in_Bytes);
	
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int counter = round(tv.tv_usec / 100000);

    if (counter == 0 || counter == 2 || counter == 4 || counter == 6 || counter == 8)
    {
        consoleUpdate(NULL);
    }
    fflush(stdout);
	return 0;
}

int downloadFile(const char *url, const char *output)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        FILE *fp = fopen(output, "wb");
        if (fp)
        {
            printf("\n");
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, API_AGENT);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

            curl_easy_setopt(curl, CURLOPT_BUFFERSIZE, 524288L);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, download_progress);

            // execute curl
            CURLcode res = curl_easy_perform(curl);

            // clean
            fclose(fp);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK)
            {
                printf("\n\ndownload complete!\n\n");
                consoleUpdate(NULL);
                return 0;
            }
        }
    }

    printf("\n\ndownload failed...\n\n");
    consoleUpdate(NULL);
    return 1;
}
