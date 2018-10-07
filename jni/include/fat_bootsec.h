#ifndef _GET_FAT_BOOTSEC_H_
#define _GET_FAT_BOOTSEC_H_

#include "defs.h"
#include "fat.h"

extern int get_fat_bootsec(const char* device, struct fat_boot_sector* bsinfo);

#endif