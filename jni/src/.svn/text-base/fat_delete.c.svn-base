/*
 * FAT File system Secure Delete modul
 * 2013. 1. 21, v 0.0.1
 * 클러스터 체인 영역의 정보를 '0'으로 오버라이팅 시킨다.
 * CHAIN_MODE가 ACTIVE인 경우 오버라이팅 이후 디렉토리 엔트리 정보도 오버라이팅 한다.
 * ACTIVE 모드 이외의 CHAIN_MODE는 해당 클러스터의 FAT 테이블 값이 0인 경우만 오버라이팅한다.
 * path 경로에 올 수 있는 것은, 오직 FAT Partition내의 파일만이 가능하다. 이외 값들은 오류처리한다.
 * Author	: Minseok, Kwon
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "defs.h"
#include "fat_delete.h"
#include <dirent.h>
#include <string.h>

static offset_t get_fat_offset(const struct fat_boot_sector* bsinfo, cluster_t cluster_num);
static offset_t get_data_offset(const struct fat_boot_sector* bsinfo, cluster_t cluster_num);

 /*
 * function description
 *  지정한 파일에 관한 정보를 가지고 있는 FAT영역과 data영역에 접근하여 전부 0으로 오버라이팅 시킨다.
 * arguments
 *  - bsinfo: FAT의 boot sector정보
 *  - device: device path
 *  - path: delete할 file의 path(root는 ount point)
 *  - mount_point: 장치가 mount되어 있는 디렉토리
 *  - head: delete할 file의 CHAIN_CLUSTER의 pointer
 *  - mode: delete할 file의 삭제 유무와 할당방식을 나타냄
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 */
int delete_file(const struct fat_boot_sector* bsinfo, 
				const char* device, 
				const char* file_path,
				const char* mount_point,
				CLUSTER_CHAIN* head, 
				CHAIN_MODE mode)
{
	int fd = -1, index_slash, i;
	CLUSTER_CHAIN* temp = NULL;
	offset_t fat_offset=0, data_offset=0, dirent_offset=0;
	sector_t data_start, f_sector;
	ssize_t nread = 0;
	FATDIR *dir=NULL;
	fat_entry_t fat_entry_buf;
	char *unlink_path=NULL, *dir_path=NULL, *file_name=NULL;
	char zero_cluster[FAT_SECTOR_SIZE] = {0};
	const char *str = " DELETE";
	struct fat_dirent d_entry;
	fat_dirent_t result;

	
	if(bsinfo == NULL || device == NULL || file_path == NULL ||
	   mount_point == NULL || head == NULL){
		errno = EINVAL;
		return -1;
	}
	
	ec_neg1( fd = open(device, O_RDWR) )
	
	ec_null(unlink_path = (char*)malloc(strlen(file_path) + strlen(mount_point) + 1));
	memset(unlink_path, 0, strlen(file_path)+strlen(mount_point)+1);
	strncpy(unlink_path, mount_point, strlen(mount_point));
	strncpy(unlink_path+strlen(mount_point), file_path, strlen(file_path));
	
	/*디렉토리 path와 파일 path 분리*/
	index_slash = strrchr(file_path, '/') - file_path;
	ec_null(dir_path = (char*)malloc(index_slash + 2));
	memset(dir_path, 0, index_slash+2);
	ec_null(file_name = (char*)malloc(strlen(file_path) - (index_slash + 1) + 1));
	strncpy(dir_path, file_path, index_slash + 1);
	strncpy(file_name, &file_path[index_slash + 1], strlen(file_path) - (index_slash + 1) + 1);

	ec_null(dir = fat_opendir(device, dir_path, mount_point, bsinfo));

	data_start = GET_DATA_START(bsinfo->reserved_sectors, bsinfo->fats, bsinfo->fat_sectors32);
	memset(&d_entry, 0, sizeof(d_entry));

	switch(mode){
		case ACTIVE:
			// cluster chain을 따라 data를 0으로 overwriting
			while(head != NULL){
				temp = head;
				head = head->next;
				fat_offset = get_fat_offset(bsinfo, temp->cluster_num);
				data_offset = get_data_offset(bsinfo, temp->cluster_num);
				for(i=0; i<bsinfo->sectors_per_cluster; i++)
					ec_neg1( lseek64(fd,  data_offset+(i*FAT_SECTOR_SIZE), SEEK_SET) );
					ec_neg1( write(fd, zero_cluster, sizeof(zero_cluster)) );
			}

			// directory entry name변경
			memset(&result, 0, sizeof(result));
			while((nread = fat_readdir(dir, &result, A_REG))!=0){
				if(nread == -1)
					EC_FAIL
				if( strcasecmp(result.name, file_name) == 0 ){
					// file unlink
					ec_neg1(unlink(unlink_path));

					f_sector = GET_SECTOR_ADDR(dir->head->cluster_num, bsinfo->rootdir_start, bsinfo->sectors_per_cluster, data_start);
					dirent_offset = (f_sector*bsinfo->sector_size) + (dir->offset-1)*FAT_DIRENTRY_SIZE;
					ec_neg1( lseek64(dir->fd, dirent_offset, SEEK_SET));
					ec_neg1( read(dir->fd, &d_entry, FAT_DIRENTRY_SIZE) )
					d_entry.name[0] = 0xe5;
					strncpy( d_entry.name+1, str, strlen(str));
					ec_neg1( lseek64(dir->fd, dirent_offset, SEEK_SET));
					ec_neg1( write(fd, &d_entry, FAT_DIRENTRY_SIZE));
					break;
				}
			}		
			break;
		case CONTINUOUS:
		case JUMP:
		case HYBRID:
			// cluster chain을 따라 data를 0으로 overwriting
			while(head != NULL){
				temp = head;
				head = head->next;
				fat_offset = get_fat_offset(bsinfo, temp->cluster_num);
				data_offset = get_data_offset(bsinfo, temp->cluster_num);
				ec_neg1( lseek64(fd, fat_offset, SEEK_SET));
				ec_neg1( read(fd, &fat_entry_buf, sizeof(fat_entry_t)) );
				if( fat_entry_buf == 0 ){
					for(i=0; i<bsinfo->sectors_per_cluster; i++)
						ec_neg1( lseek64(fd, data_offset+(i*FAT_SECTOR_SIZE), SEEK_SET) );
						ec_neg1( write(fd, zero_cluster, sizeof(zero_cluster)) );
				}
			}
			// directory entry name변경
			memset(&result, 0, sizeof(result));
			while((nread = fat_readdir(dir, &result, D_REG))!=0){
				if(nread==-1)
					EC_FAIL
				if( strcmp(result.name, file_name) == 0 ){
					f_sector = GET_SECTOR_ADDR(dir->head->cluster_num, bsinfo->rootdir_start, bsinfo->sectors_per_cluster, data_start);
					dirent_offset = (f_sector*bsinfo->sector_size) + (dir->offset-1)*FAT_DIRENTRY_SIZE;
					ec_neg1( lseek64(dir->fd, dirent_offset, SEEK_SET) );
					ec_neg1( read(dir->fd, &d_entry, FAT_DIRENTRY_SIZE) );
					d_entry.name[0] = 0xe5;
					strncpy(d_entry.name+1, str, strlen(str));
					ec_neg1( lseek64(fd, dirent_offset, SEEK_SET));
					ec_neg1( write(fd, &d_entry, FAT_DIRENTRY_SIZE) );
				}
			}
			break;
		default:
			break;
	}
	ec_neg1(close(fd))
	fat_closedir(dir);
	free(unlink_path);
	free(dir_path);
	free(file_name);
	return 0;

	EC_CLEANUP_BGN
		if(fd != -1)
			(void)close(fd);
		fat_closedir(dir);
		free(unlink_path);
		free(dir_path);
		free(file_name);
		return -1;
	EC_CLEANUP_END
}


 /*
 * function description
 *  cluster number에 해당하는 FAT entry offset을 구해주는 함수
 * arguments
 *  - bsinfo: FAT의 boot sector정보
 *  - cluster_num: FAT entry offset값을 알고자 하는 cluster number
 * return value
 *  -  offset_t: 해당 cluster에 대한 FAT entry offset
 */
static offset_t get_fat_offset(const struct fat_boot_sector* bsinfo, cluster_t cluster_num){
	return (bsinfo->reserved_sectors * FAT_SECTOR_SIZE) + (cluster_num * sizeof(fat_entry_t));
}

/*
 * function description
 *  cluster number에 해당하는 offset을 구해주는 함수
 * arguments
 *  - bsinfo: FAT의 boot sector정보
 *  - cluster_num: offset값을 알고자 하는 cluster number
 * return value
 *  -  offset_t: 해당 cluster에 대한 offset
 */
static offset_t get_data_offset(const struct fat_boot_sector* bsinfo, cluster_t cluster_num){
	sector_t data_start_sector;
	offset_t data_start_offset;

	data_start_sector = (bsinfo->reserved_sectors + (bsinfo->fats * bsinfo->fat_sectors32));
	data_start_offset = data_start_sector * FAT_SECTOR_SIZE;
	return data_start_offset + ((cluster_num - 2) * bsinfo->sectors_per_cluster * FAT_SECTOR_SIZE);
}
