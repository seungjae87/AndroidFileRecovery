
/*
 * Recover and Scan Module
 * 2013. 1. 21, v 0.0.1
 * 복구 가능한 데이터 스캐닝
 * 삭제된 데이터 복구 기능
 * Author	: Seungjae, Lee
			: Minseok, Kwon (recover_file_mv, init_temp)
 * 2013 Spring semester, Capstone Project
 * Handong University
 */
#ifndef _RECOVERY_H_
#define _RECOVERY_H_
#include "defs.h"
#include "fat.h"
#include "fat_cluster.h"
#include "get_device.h"
#include "fat_dir.h"
#include <dirent.h>

#define MOUNTPOINT_TMP		"/data/"
#define DIRECTORY_TMP		"TEMP_FILE/"
#define DIRECTORY_RECOVER	"/RECOVERED/"
#define DIRECTORY_APPDATA	MOUNTPOINT_TMP"data/"PACKAGE_NAME
#define PATH_TMPRECOVER		DIRECTORY_APPDATA"/"DIRECTORY_TMP
#define CMD_RM				"rm "PATH_TMPRECOVER"*"
#define THRESHOLD			0.8
#define BUFSIZE				512


typedef struct RECOVER_LIST {
	char		filename[LFN_MAX];
	char		extender[3];
	char*		dir_path;
	cluster_t	first_cluster;
	unsigned long size;
	int			expected_possibility; // only supported on continious mode
	struct RECOVER_LIST* next;
} RECOVER_LIST;

extern int recover_file_tmp( const struct fat_boot_sector* bsinfo,
							 const char* device,
							 const char* filename, 
							 unsigned long filesize,
							 const CLUSTER_CHAIN* head);

extern int recover_file_mv(const char* src, const char* dst);
extern int fat_init_temp();

extern int fatscan_dir(const struct DEV_INFO* device, 
				 const struct fat_boot_sector* bsinfo,
				 const char *path, 
				 RECOVER_LIST** header);
extern int add_recover_list(RECOVER_LIST** head, RECOVER_LIST* item);
extern void free_recover_list(RECOVER_LIST** head);
extern void print_recover_list(RECOVER_LIST* header);// for debugging

#endif