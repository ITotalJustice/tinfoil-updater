#include <stdio.h>
#include <curl/curl.h>
#include <switch.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int downloadFile(const char *url, const char *output)
{
    CURL *curl = curl_easy_init();
    CURLcode res;

    if (curl)
    {
        printf("starting download\n");
        consoleUpdate(NULL);

        FILE *fp = fopen(output, "wb");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // enable for debug
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(fp);

        printf("success!\n");
        return 0;
    }
    printf("fail\n");
    return 1;
}