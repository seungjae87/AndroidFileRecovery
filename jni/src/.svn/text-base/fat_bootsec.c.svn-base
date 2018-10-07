/*
 * Fetch Bootsector information
 * 2013. 1. 21, v 0.0.1
 * FAT BootSector 정보를 가져온다.
 * 해당 디바이스가 FAT가 아니면 에러처리한다.
 * 정상적으로 부트섹터정보를 가져올 경우 0을 반환하고
 * 에러시 -1을 반환하고, errno에 값이 저장된다.
 * Author	: Minseok, Kwon
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "fat_bootsec.h"

/*
 * function description
 * arguments
 *  - device: device path
 *  - bsinfo: FAT의 boot sector정보
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 */
int get_fat_bootsec(const char* device, struct fat_boot_sector* bsinfo)
{
	int fd = -1;
	int nread = -1;

	if(device == NULL || bsinfo == NULL){
		errno = EINVAL;
		return -1;
	}

	if( (fd = open(device, O_RDONLY)) == -1)
		return -1;
	
	switch(nread=read(fd, bsinfo, sizeof(struct fat_boot_sector))){
		case -1:
			(void)close(fd);
			return -1;
		case 0:
			(void)close(fd);
			errno = EREADZERO;
			return -1;
		default:
			if(nread != sizeof(struct fat_boot_sector)){
				(void)close(fd);
				errno = EREADSIZE;
				return -1;
			}
	}
	(void)close(fd);
	return 0;
}