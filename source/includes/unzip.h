#ifndef _UNZIP_H_
#define _UNZIP_H_

#define UP_ALL              0
#define UP_TINFOIL_FOLDER   1
#define UP_TINFOIL_NRO      2
#define UP_APP              3

int unzip(const char *output, int mode);

#endif