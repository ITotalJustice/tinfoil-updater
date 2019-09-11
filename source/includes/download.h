#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define TINFOIL_URL         "https://tinfoil.io/repo/tinfoil.latest.zip"
#define TINFOIL_NRO_URL     "https://tinfoil.media/repo/tinfoil.json"
#define HBG_URL             "https://thehbg.shop/files/hbgshop.latest.zip"
#define APP_URL             "https://github.com/ITotalJustice/tinfoil-updater/releases/latest/download/tinfoil-updater.nro"

int downloadFile(const char *url, const char *output);

#endif