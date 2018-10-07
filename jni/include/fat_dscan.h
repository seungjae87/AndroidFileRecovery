
/*
 * FAT FileSystem Detail Scan module
 * 2013. 1. 21, v 0.0.1
 * FAT Filesystem FAT Data Cluster영역 정밀 스캔
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "defs.h"
#include "fat_bootsec.h"
#include "fat_dir.h"
#include "fat_recover.h"

#define FAT_DENTRY_SIZE	 		(sizeof(struct fat_dirent))
#define FAT_DENTRYS_PER_SECTOR	(FAT_SECTOR_SIZE/sizeof(struct fat_dirent))

typedef struct FAT_DATA {
	sector_t	s_sector;
	sector_t	c_sector;
	sector_t	e_sector;
	cluster_t	e_cluster;
	cluster_t	c_cluster;
	int			fd;
	int			sectors_per_cluster;
	char buf[FAT_SECTOR_SIZE];
	char dev_path[1];
} FAT_DATA;

extern FAT_DATA* fat_opendata(const char* dev_path);
extern int fat_data_cluster_scan(FAT_DATA* fdata, RECOVER_LIST** head);
extern void fat_closedata(FAT_DATA* fdata);
