
/*
 * FAT Cluster Module
 * 2013. 1. 21, v 0.0.1
 * FAT Filesystem FAT Table의 Cluster정보에 관한 함수들이 정의되어 있다.
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#ifndef _CLUSTER_CHAIN_H_
#define _CLUSTER_CHAIN_H_

#include "fat.h"

#define	FAT_CLUSTER_SIZE	sizeof(fat_entry_t)

#define	get_cluster_sector(start_sector, cluster) (((cluster)/FAT_ENTRIES_PER_SECTOR)+(start_sector))
#define get_cluster_offset(cluster) (((cluster)%FAT_ENTRIES_PER_SECTOR)*FAT_CLUSTER_SIZE)
#define ceil(x) ( ((x) > (int)(x))? ((x)+1):(x) )

/* FAT Table의 cluster정보를 읽는데 사용 */
typedef struct FATCLUSTER {
	uint16_t  sector_size;
	uint16_t  cluster_size;
	sector_t  fat_start_sector;
	sector_t  fat_end_sector;
	sector_t  p_sector;
	cluster_t max_cluster;
	char	  buf[FAT_SECTOR_SIZE];
	char	  device[1];
} FATCLUSTER;

/* Cluster linked list */
typedef struct CLUSTER_CHAIN {
	cluster_t cluster_num;
	struct CLUSTER_CHAIN* next;
} CLUSTER_CHAIN;

/*
 * type description
 *  - ACTIVE	  : 살아있는 데이터
 *  - CONTINIUOUS : 삭제된 데이터의 첫째 클러스터부터 연속된 클러스터 리스트
 *  - JUMP		  : 삭제된 데이터의 첫째 클러스터부터 비할당된 클러스터의 리스트
 *  - HYBRID	  : 삭제된 데이터의 첫째 클러스터부터 할당된 클러스터의 경우
 *					50%확률로 선택된 리스트
 */
typedef enum { ACTIVE, CONTINUOUS, JUMP, HYBRID } CHAIN_MODE;

extern FATCLUSTER* open_fat_cluster(const struct fat_boot_sector* bs, const char* device);

extern void close_fat_cluster(FATCLUSTER* c);

extern int get_fat_cluster_chain(FATCLUSTER* c,
							cluster_t first_cluster, 
							unsigned long size,
							CLUSTER_CHAIN** head, 
							CHAIN_MODE mode);
extern int guess_recovery_possibility( const struct fat_boot_sector* bs, 
							const char* device,
							cluster_t first_cluster, 
							unsigned long size);
extern void free_fat_cluster_chain(CLUSTER_CHAIN** head);

//for debugging
extern void print_fat_cluster_chain(CLUSTER_CHAIN* head, int count);

#endif