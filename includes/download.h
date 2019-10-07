#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define TINFOIL_URL         "https://tinfoil.io/repo/tinfoil.latest.zip"
#define TINFOIL_NRO_URL     "https://tinfoil.media/repo/tinfoil.json"
#define HBG_URL             "https://thehbg.shop/files/hbgshop.latest.zip"
#define APP_URL             "https://github.com/ITotalJustice/tinfoil-updater/releases/latest/download/tinfoil-updater.nro"

// legacy versions of tinfoil.
// This is the lazy way of loading them.
// I will eventually self host and download a .json and parse it on app start up.
#define legacy500           "https://tinfoil.io/repo/tinfoil.500.nro"
#define legacy410           "https://tinfoil.io/repo/tinfoil.410.nro"
#define legacy400           "https://tinfoil.io/repo/tinfoil.400.nro"
#define legacy302           "https://tinfoil.io/repo/tinfoil.302.nro"
#define legacy301           "https://tinfoil.io/repo/tinfoil.301.nro"
#define legacy300           "https://tinfoil.io/repo/tinfoil.300.nro"
#define legacy210           "https://tinfoil.io/repo/tinfoil.210.nro"
#define legacy201           "https://tinfoil.io/repo/tinfoil.201.nro"
#define legacy200           "https://tinfoil.io/repo/tinfoil.200.nro"


int downloadFile(const char *url, const char *output);

#endif