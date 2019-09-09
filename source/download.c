#include <stdio.h>
#include <curl/curl.h>
#include <switch.h>

#define Megabytes_in_Bytes	1048576

int download_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow)
{	
    printf("* DOWNLOADING: %.2fMB of %.2fMB *\r", dlnow / Megabytes_in_Bytes, dltotal / Megabytes_in_Bytes);
	
	consoleUpdate(NULL);
	return 0;
}

int downloadFile(const char *url, const char *output)
{
    CURL *curl = curl_easy_init();
    CURLcode res = -1;

    if (curl)
    {
        printf("\n");

        FILE *fp = fopen(output, "wb");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // follow redirect
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // enable for debug
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // ssl bs
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); // more ssl bs
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, download_progress);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    if (res == CURLE_OK)
    {
        printf("\n\ndownload complete!\n\n");
        consoleUpdate(NULL);
        return 0;
    }

    printf("\n\ndownload failed...\n\n");
    consoleUpdate(NULL);
    return 1;
}
