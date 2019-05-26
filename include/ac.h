#include <3ds.h>

#ifndef AC_H
#define AC_H

char ssid[0x20], passphrase[0x40];

Result aciInit(void);
Result aciExit(void);
Result ACI_GetSSID(char * ssid);
Result ACI_GetPassphrase(char * passphrase);

#endif