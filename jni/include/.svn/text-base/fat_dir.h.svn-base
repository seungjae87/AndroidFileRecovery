/*
 * Comment Example Module
 * 2013. 1. 22, v 0.0.1
 * Author	: MinWoo, Kim
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#ifndef __FAT_DIR_FUNC__
#define __FAT_DIR_FUNC__

#include "fat.h"
#include "fat_cluster.h"
#include <string.h>


/*
 *  - FAT_FLAG_ERASE: indicate deleted directory entry
 *  - FAT_FLAG_INIT : indicate initial directory entry which is "." and ".."
 */
#define FAT_DIRENTRY_SIZE 32
#define FAT_SECTOR_SIZE 512
#define FAT_ATTR_LFN 0x0f
#define FAT_FLAG_ERASE 0xe5
#define FAT_FLAG_INIT 0x2e
#define FAT_ATTR_SUBDIR2 0x30
/*
 *  - FAT_LFN_N1_SIZE: LFN name1 size
 *  - FAT_LFN_N2_SIZE: LFN name2 size
 *  - FAT_LFN_N3_SIZE: LFN name3 size
 *  - FAT_LFN_B1_SIZE: LFN name1 field size
 *  - FAT_LFN_B2_SIZE: LFN name2 field size
 *  - FAT_LFN_B3_SIZE: LFN name3 field size
 *  - LFN_MAX        : FAT LFN MAX length
 *  - LFN_PART_MAX   : FAT LFN PART MAX length
 */
#define FAT_LFN_N1_SIZE 15
#define FAT_LFN_N2_SIZE 18
#define FAT_LFN_N3_SIZE 6
#define FAT_LFN_B1_SIZE 10
#define FAT_LFN_B2_SIZE 12
#define FAT_LFN_B3_SIZE 4
#define LFN_MAX 255
#define LFN_PART_MAX 39

/*
 *  - GET_SECTOR_ADDR   : Macro function getting sector address
 *  - GET_DATA_START    : Macro function getting the place where data area start
 *  - GET_CLUSTER_ADDR  : Macro function getting cluster address
 *  - CAL_FIRST_CLUSTER : Macro function that calculate first cluster address

 */
#define GET_SECTOR_ADDR(cluster,rootdir_start,sectors_per_cluster,data_start) (((cluster-rootdir_start)*(sectors_per_cluster))+(data_start))
#define GET_DATA_START(reserved_sectors,fats,fat_sectors) ((reserved_sectors)+((fats)*(fat_sectors)))
#define GET_CLUSTER_ADDR(sector,rootdir_start,sectors_per_cluster,data_start) ((((sector)-(data_start))/(sectors_per_cluster))+(rootdir_start))
#define CAL_FIRST_CLUSTER(first_cluster_high, first_cluster_low) ((((cluster_t)(first_cluster_high))<<16) | ((cluster_t)(first_cluster_low)))

/*
 *  - fd       : file descriptor of device block file
 *  - head     : point first node of cluster chain
 *  - cur_point: point the node which is currently examined
 *  - buf      : store directory entries(512 bytes)
 */
struct _FATDIR{
	int fd;
	uint64_t offset;
	uint8_t sectors_per_cluster;
	uint8_t fats;
	uint16_t sector_size;
	uint16_t reserved_sectors;
	uint32_t fat_sectors32;
	uint32_t rootdir_start;
	CLUSTER_CHAIN *cur_point;
	CLUSTER_CHAIN *head;
	unsigned char buf[FAT_SECTOR_SIZE];
	char deviceName[1];
} __attribute__((__packed__));

/*
 *  - FAT LFN structure
 */
struct _fat_dirent_lfn
{
	uint8_t order_num;
	unsigned char name1[10];
	uint8_t attr;
	uint8_t reserved1;
	uint8_t check_sum;
	unsigned char name2[12];
	uint16_t reserved2;
	unsigned char name3[4];

}__attribute__((__packed__));

/*
 *  - dirent: fat_dirent structure
 *  - name  : name of file or directory
 */
struct _fat_dirent_t
{
	struct fat_dirent dirent; 
	char name[LFN_MAX];
}__attribute__((__packed__));

/*
 *  - count: number of directory
 *  - name : name of file or directory
 *  - st   : stat structure
 */
struct _traverse_info{
  int count;
  char *name;
  struct stat st;
};

typedef struct _traverse_info traverse_info;
typedef struct _fat_dirent_lfn fat_dirent_lfn;
typedef struct _fat_dirent_t fat_dirent_t;
typedef struct _FATDIR FATDIR;

/*
 *  - A_DIR: Active Directory
 *  - A_REG: Active Regular
 *  - D_DIR: Deleted Directory
 *  - D_REG: Deleted Regular
 */
typedef enum {A_DIR, A_REG, D_DIR, D_REG}ENTRY_MODE;

extern FATDIR * fat_opendir(const char* device,const char* path, const char* mount_point, const struct fat_boot_sector *bsinfo);
extern int fat_closedir(FATDIR* dirp);
extern int fat_readdir(FATDIR* dirp,fat_dirent_t *result, ENTRY_MODE mode);
extern int fat_rewinddir(FATDIR* dirp);
extern void fat_seekdir(FATDIR* dirp, long loc);
extern void fat_telldir(FATDIR* dirp);
extern long get_pathcount(const char* top_path,const char* mount_point);

#endif
