
/*
 * FAT FileSystem Detail Scan module
 * 2013. 1. 21, v 0.0.1
 * FAT Filesystem FAT Data Cluster영역 정밀 스캔
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "fat_dscan.h"

static bool determin_directory(const char* buf);
static int fetch_recoverfile_from_buf(FAT_DATA* fdata, RECOVER_LIST** head);

FAT_DATA*
fat_opendata(const char* dev_path)
{
	FAT_DATA* f = NULL;
	int fd = -1;
	struct fat_boot_sector bsinfo;

	if(dev_path == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	ec_neg1(get_fat_bootsec(dev_path, &bsinfo));
	ec_null(f = (FAT_DATA*)malloc(offsetof(FAT_DATA, dev_path) + strlen(dev_path) + 1));
	f->s_sector = bsinfo.reserved_sectors + bsinfo.fats*bsinfo.fat_sectors32;
	f->e_sector = bsinfo.sectors32-1;
	f->c_sector = f->s_sector;
	f->e_cluster = ((bsinfo.sectors32 - bsinfo.reserved_sectors - bsinfo.fats*bsinfo.fat_sectors32)/bsinfo.sectors_per_cluster)+1;
	f->c_cluster = 2;
	f->sectors_per_cluster = bsinfo.sectors_per_cluster;
	strncpy(f->dev_path, dev_path, strlen(dev_path)+1);

	ec_neg1( fd = open(dev_path, O_RDONLY) );
	f->fd = fd;
	
	ec_neg1( lseek64(fd, ((offset_t)(f->c_sector))*FAT_SECTOR_SIZE, SEEK_SET) );
	ec_neg1( read(fd, f->buf, FAT_SECTOR_SIZE) );
	
	return f;

	EC_CLEANUP_BGN
		free(f);
		if(fd != -1)
			close(fd);
		return NULL;
	EC_CLEANUP_END
}

void
fat_closedata(FAT_DATA* fdata)
{
	(void)free(fdata);
}

int
fat_data_cluster_scan(FAT_DATA* fdata, RECOVER_LIST** head)
{
	int walker;

	if(fdata == NULL || head == NULL){
		errno = EINVAL;
		EC_FAIL
	}
	
	if(determin_directory(fdata->buf) == false){
		fdata->c_cluster++;
		fdata->c_sector += (fdata->sectors_per_cluster);

		// end of data area
		if( fdata->c_cluster >= fdata->e_cluster ||
			fdata->c_sector >= fdata->e_sector ){
			return 0;
		}
		
		ec_neg1(lseek64(fdata->fd, ((offset_t)(fdata->c_sector))*FAT_SECTOR_SIZE,SEEK_SET));
		ec_neg1(read(fdata->fd, fdata->buf, FAT_SECTOR_SIZE));
		return 1;
	}

	for( walker=0; walker < fdata->sectors_per_cluster; walker++){
		fetch_recoverfile_from_buf(fdata, head);

		fdata->c_sector++;
		ec_neg1(lseek64(fdata->fd, ((offset_t)(fdata->c_sector))*FAT_SECTOR_SIZE,SEEK_SET));
		ec_neg1(read(fdata->fd, fdata->buf, FAT_SECTOR_SIZE));
	}
	fdata->c_cluster++;

	// end of data area
	if( fdata->c_cluster >= fdata->e_cluster ||
		fdata->c_sector >= fdata->e_sector ){
		return 0;
	}
	return 1;

	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}


static bool 
determin_directory(const char* buf)
{
	int dentry_offset;
	int count = 0;
	struct fat_dirent dirent;
	char flags[2] = { 0x00, 0xe5 };

	if(buf == NULL)
		return false;
	
	for(dentry_offset=0; dentry_offset < FAT_DENTRYS_PER_SECTOR; dentry_offset++){
		memcpy(&dirent, &buf[dentry_offset*FAT_DENTRY_SIZE], FAT_DENTRY_SIZE);
		if( dirent.name[0] == flags[0] ) // assume end of directory
			break;
		switch( dirent.attr ){
			case FAT_ATTR_READONLY:
			case FAT_ATTR_HIDDEN:
			case FAT_ATTR_SYSTEM:
			case FAT_ATTR_VOLLABEL:
			case FAT_ATTR_SUBDIR:
			case FAT_ATTR_ARCHIVE:
			case FAT_ATTR_DEVICE:
			case FAT_ATTR_LFN:
				count++;
				break;
			default:
				return false;
		}
	}
	if(count == 0)
		return false;

	return true;
}

static int
fetch_recoverfile_from_buf(FAT_DATA* fdata, RECOVER_LIST** head)
{
	RECOVER_LIST tmp;
	struct fat_dirent dirent;
	int dentry_offset;
	char flags[2] = { 0x00, 0xe5 };
	int tmp_firstcluster=0;

	if(fdata == NULL || head == NULL){
		errno = EINVAL;
		EC_FAIL
	}
	
	for( dentry_offset=0; dentry_offset < FAT_DENTRYS_PER_SECTOR; dentry_offset++){
		memcpy(&dirent, &(fdata->buf[dentry_offset*FAT_DENTRY_SIZE]), FAT_DENTRY_SIZE);
		

		if( dirent.name[0] == flags[0] )
			return 0;

		if( dirent.name[0] == flags[1]
		   && dirent.attr == FAT_ATTR_ARCHIVE){
			tmp_firstcluster = CAL_FIRST_CLUSTER(dirent.first_cluster_hi,
												 dirent.first_cluster_lo);
			if(tmp_firstcluster < 2 || tmp_firstcluster > fdata->e_cluster )
				break;

			dirent.name[0] = '_';
			strncpy(tmp.filename, dirent.name, 8);
			strncpy(tmp.extender, dirent.extender, 3);
			tmp.filename[8] = '\0';
			tmp.dir_path = strdup("nodir");
			tmp.first_cluster = tmp_firstcluster;
			tmp.size = dirent.size;
			tmp.expected_possibility = 0;
			tmp.next = NULL;
			ec_neg1(add_recover_list(head, &tmp));
		}
	}
	return 0;

	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}
