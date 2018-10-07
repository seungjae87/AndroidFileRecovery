/*
 * Comment Example Module
 * 2013. 1. 21, v 0.0.1
 * Author	: Minseok, Kwon
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#ifndef _DELETE_FILE_H_
#define _DELETE_FILE_H_

#include "fat_cluster.h"
#include "fat_dir.h"
#include <dirent.h>

extern int delete_file(const struct fat_boot_sector* bsinfo, 
				const char* device, 
				const char* file_path,
				const char* mount_point,
				CLUSTER_CHAIN* head, 
				CHAIN_MODE mode);
#endif