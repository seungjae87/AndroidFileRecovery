/*
 * FAT File system Directory explore module
 * 2013. 1. 22, v 0.0.1
 * FAT File System Directory Entry functions
 * Author	: MinWoo, Kim
 * 2013 Spring semester, Capstone Project
 * Handong University
 */
#include <dirent.h>
#include <string.h>
#include "defs.h"
#include "fat_dir.h"
#include "fat_bootsec.h"

typedef struct _LFN_LINK{
	char name[LFN_PART_MAX+1];
	struct _LFN_LINK *next;
}LFN_LINK;

static int sync_dirent_buf(FATDIR *dirp);
static int combine_lfn(fat_dirent_lfn* lfn_dirent, char* result);
static int do_entry(traverse_info *info);
static int do_dir(traverse_info *info);
static int get_lfn(FATDIR *dirp,fat_dirent_t *result);
static int get_lfn_e(FATDIR *dirp, fat_dirent_t *result);
static int get_sfn(FATDIR *dirp, fat_dirent_t *result);
static int UniToUTF8(unsigned char *buf, unsigned char *result);
/*
 * function description
 */
FATDIR * fat_opendir(const char* device, const char* path, const char* mount_point, const struct fat_boot_sector *bsinfo)
{
	FATDIR *dirp=NULL;
	fat_dirent_t result;
	FATCLUSTER* f = NULL;
	CHAIN_MODE mode = ACTIVE;
	cluster_t first_cluster;
	char *path_tmp=NULL, *ptr=NULL;
	char *dname_list[LFN_MAX];
	int i=1,cnt=1;

	ec_null(dirp = (FATDIR*)malloc(sizeof(FATDIR)+strlen(device)+1));
	ec_null(memset(dirp,0,sizeof(FATDIR)+strlen(device)+1));
	ec_neg1(dirp->fd = open(device,O_RDONLY));

	dirp->sectors_per_cluster = bsinfo->sectors_per_cluster;
	dirp->fats = bsinfo->fats;
	dirp->sector_size = bsinfo->sector_size;
	dirp->reserved_sectors = bsinfo->reserved_sectors;
	dirp->fat_sectors32 = bsinfo->fat_sectors32;
	dirp->rootdir_start = bsinfo->rootdir_start;
	strncpy(dirp->deviceName,device,strlen(device)+1);
	
	f = open_fat_cluster(bsinfo,dirp->deviceName);
	get_fat_cluster_chain(f,dirp->rootdir_start,0,&dirp->head,mode);
	dirp->cur_point = dirp->head;
	
	ec_null(path_tmp = strdup(path));
	ptr = strtok(path_tmp,"/");
	
	if(ptr == NULL){
		close_fat_cluster(f);
		free(path_tmp);
		return dirp;
	}
	ec_null(dname_list[0] = strdup(ptr));
	while((ptr = strtok(NULL,"/"))!=NULL){
		ec_null(dname_list[cnt] = strdup(ptr));
		cnt++;
	}

	for(i=0; i<cnt; i++){
		while(fat_readdir(dirp,&result,A_DIR)!=0){
			if(strcasecmp(dname_list[i],result.name)==0){
				first_cluster = CAL_FIRST_CLUSTER((cluster_t)result.dirent.first_cluster_hi,(cluster_t)result.dirent.first_cluster_lo);
				get_fat_cluster_chain(f,first_cluster,0,&dirp->head,mode);
				dirp->cur_point = dirp->head;
				dirp->offset = 0;
				memset(dirp->buf,0,FAT_SECTOR_SIZE);
				break;
			}
		}
	}
	close_fat_cluster(f);
	free(path_tmp);

	for(i=0; i<cnt; i++)
		free(dname_list[i]);
	return dirp;

	EC_CLEANUP_BGN
		if(f!=NULL)
			close_fat_cluster(f);
		if(dirp!=NULL)
			free(dirp);
		if(path_tmp!=NULL)
			free(path_tmp);
		if(dname_list[0] != NULL){
			for(i=0; i<cnt; i++)
				free(dname_list[i]);
		}
		return NULL;
	EC_CLEANUP_END
}

/*
 * function description
 */
int fat_readdir(FATDIR* dirp,fat_dirent_t *result,ENTRY_MODE mode)
{	
	uint8_t buf_offset=0;
	int sync_stat=0, lfn_stat=0, i;
	if(dirp == NULL){
		errno = 1203;
		EC_FAIL
	}
	if(result == NULL){
		errno = 1202;
		EC_FAIL
	}
	memset(result,0,sizeof(result));

	switch(mode){
	case A_DIR: // Search Active Directory 
		while((sync_stat=sync_dirent_buf(dirp))!=0){
			if(sync_stat==-1)
				EC_FAIL
			buf_offset = dirp->offset%16;
			if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE] == FAT_FLAG_ERASE){ //Erased Directory Entry
				dirp->offset++;
				continue;
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_SUBDIR || dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_SUBDIR2){ //Subdir Directory Entry
				if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]==FAT_FLAG_INIT){
					dirp->offset++;
					continue;
				}
				memcpy(&result->dirent,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);
				ec_neg1(get_sfn(dirp,result));
				dirp->offset++;
				break;
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_LFN){ //LFN Directory Entry
				ec_neg1(get_lfn(dirp,result));
				if(result->dirent.attr == FAT_ATTR_SUBDIR || result->dirent.attr == FAT_ATTR_SUBDIR2){
					break;
				}else{
					memset(result,0,sizeof(fat_dirent_t));
					continue;
				}
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]==0x00){ //No more data
				return 0;
			}else{
				dirp->offset++;
				continue;
			}
		}
		break;
	case A_REG: // Search Active Regular File(Archive File)
		while((sync_stat=sync_dirent_buf(dirp))!=0){
			if(sync_stat==-1)
				EC_FAIL
			buf_offset = dirp->offset%16;
			if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE] == FAT_FLAG_ERASE){
				dirp->offset++;
				continue;
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_ARCHIVE){
				memcpy(&result->dirent,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);
				ec_neg1(get_sfn(dirp,result));
				dirp->offset++;
				break;
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_LFN){
				ec_neg1(get_lfn(dirp,result));
				if(result->dirent.attr == FAT_ATTR_ARCHIVE){
					break;
				}else{
					memset(result,0,sizeof(fat_dirent_t));
					continue;
				}
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]==0x00){
				return 0;
			}else{
				dirp->offset++;
				continue;
			}
		}
		break;
	case D_DIR: // Search Deleted Directory
		return 0;
		break;
	case D_REG: // Search Deleted Regular File(Archive File)
		while((sync_stat=sync_dirent_buf(dirp))!=0){
			if(sync_stat==-1)
				EC_FAIL
			buf_offset = dirp->offset%16;
			if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE] == FAT_FLAG_ERASE && dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_ARCHIVE){
				memcpy(&result->dirent,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);
				lfn_stat = get_lfn_e(dirp,result);
				if(lfn_stat!=0){
					ec_neg1(get_sfn(dirp,result));
					result->name[0]=0x5F;
				}
				dirp->offset++;
				break;
			}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]==0x00){
				return 0;
			}else{
				dirp->offset++;
				continue;
			}
		}
		break;

	default:
		errno = 1203;
		return -1;
	}
	if(sync_stat==0)
		return 0;

	return 1;
	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}

/*
 * function description
 */
int fat_rewinddir(FATDIR* dirp){
	
	if(dirp == NULL){
		errno = 1203;
		EC_FAIL
	}
	
	dirp->offset = 0;
	dirp->cur_point = dirp->head;

	return 0;

	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}

/*
 * function description
 */
int fat_closedir(FATDIR* dirp)
{
	if(dirp==NULL){
		errno = 1203;
		EC_FAIL
	}

	ec_neg1(close(dirp->fd));
	free_fat_cluster_chain(&dirp->head);
	free(dirp);
	
	return 0;

	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}
/*
 * function description
 */
long get_pathcount(const char* top_path,const char* mount_point)
{
	int count;
	char *path;
	traverse_info info;
	int strlength = 0;
	strlength = strlen(mount_point)+strlen(top_path);
	
	ec_null(path = (char*)malloc(strlength+1));
	memset(path,0,strlength+1);
	ec_null(strncpy(path, mount_point, strlen(mount_point)));
	ec_null(strcat(path,top_path));
	
	memset(&info,0,sizeof(info));
	info.name = path;
	ec_neg1(do_entry(&info));
	
	count = info.count;
	
	if(path!=NULL)
		free(path);
	return count;

	EC_CLEANUP_BGN
		if(path!=NULL)
			free(path);
		return -1;
	EC_CLEANUP_END
}


/*
 * function description
 */
static int sync_dirent_buf(FATDIR *dirp)
{
	unsigned int nread;
	sector_t data_start;
	cluster_t f_cluster;
	sector_t f_sector;
	sector_t offset_sector;
	cluster_t offset_cluster;

	data_start = GET_DATA_START(dirp->reserved_sectors,dirp->fats,dirp->fat_sectors32);
	f_cluster = dirp->cur_point->cluster_num;
	f_sector = GET_SECTOR_ADDR(f_cluster,
								dirp->rootdir_start,
								dirp->sectors_per_cluster,
								data_start);
	offset_sector = f_sector+
					(dirp->offset*FAT_DIRENTRY_SIZE)/dirp->sector_size;
	

	offset_cluster = GET_CLUSTER_ADDR(offset_sector,
										dirp->rootdir_start,
										dirp->sectors_per_cluster,
										data_start);


	if(offset_cluster != dirp->cur_point->cluster_num){
		 if(dirp->cur_point->next != NULL){
			 dirp->cur_point = dirp->cur_point->next;
			 f_cluster = dirp->cur_point->cluster_num;
			 f_sector = GET_SECTOR_ADDR(f_cluster,
			 								dirp->rootdir_start,
			 								dirp->sectors_per_cluster,
			 								data_start);
			 offset_sector = f_sector;
			 dirp->offset = 0;
		 }
		 else if(dirp->cur_point->next == NULL){
			 //EOF
			 return 0;
		 }
	}

	if(dirp->offset % 16 ==0){
		memset(dirp->buf,0,sizeof(dirp->buf));
		if(lseek64(dirp->fd, offset_sector*FAT_SECTOR_SIZE,SEEK_SET)==-1)
			return -1;
		nread = read(dirp->fd, dirp->buf, FAT_SECTOR_SIZE);

		if(nread == -1){
			return -1;
		}
		else if(nread != FAT_SECTOR_SIZE){
			errno = 1006;
			return -1;
		}
	}

	return 1;
}

/*
 * function description
 * arguments
 * return value
 *  -  0: Success
 *  - -1: error
 * errno
 *  - system errors
 *  - Addtional errors
 */
static int get_sfn(FATDIR *dirp, fat_dirent_t *result)
{
	char name[9];
	char extender[4];
	char *n_ptr = NULL, *e_ptr=NULL;

	if(dirp == NULL){
		errno = 1203;
		return -1;
	}

	if(result==NULL){
		errno = 1202;
		return -1;
	}
	memset(result->name,0,LFN_MAX);
	memset(name,0,sizeof(name));
	memset(extender,0,sizeof(extender));

	memcpy(name,result->dirent.name,8);
	memcpy(extender,result->dirent.extender,3);

	n_ptr = strtok(name," ");
	e_ptr = strtok(extender," ");

	if(n_ptr != NULL){
		strncpy(result->name,n_ptr,strlen(n_ptr));
	}else{
		strncpy(result->name,name,strlen(name));
	}

	if(e_ptr !=NULL){
		strncat(result->name,".",1);
		strncat(result->name,e_ptr,strlen(e_ptr));
	}
	return 0;
}
/*
 * function description
 */
static int get_lfn(FATDIR *dirp,fat_dirent_t *result)
{
	uint8_t buf_offset=0, order_num=0;
	fat_dirent_lfn lfn_entry;
	char tmp[LFN_PART_MAX+1];
	int str_len=0, name_offset=0,sync_stat=0,i=0;
	LFN_LINK *lfn_head= NULL, *lfn_tmp=NULL;

	if(dirp==NULL){
		errno = 1203;
		return -1;
	}
	if(result==NULL){
		errno = 1202;
		return -1;
	}
	memset(result->name,0,LFN_MAX);

	while((sync_stat=sync_dirent_buf(dirp))!=0){
		buf_offset = dirp->offset%16;
		if(sync_stat==-1)
			return -1;

		if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]!=FAT_FLAG_ERASE &&
			dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] == FAT_ATTR_LFN){
			memset(&lfn_entry,0,sizeof(lfn_entry));
			memcpy(&lfn_entry,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);
			
			memset(tmp,0,sizeof(tmp));
			if(combine_lfn(&lfn_entry,tmp)!=0){
				errno = 1200;
				return -1;
			}
			str_len += strlen(tmp);
			if(lfn_head==NULL){
				lfn_head = (LFN_LINK*)malloc(sizeof(LFN_LINK));
				lfn_head->next = NULL;
				memset(lfn_head->name,0,LFN_PART_MAX+1);
				strncpy(lfn_head->name,tmp,strlen(tmp));
			}else{
				lfn_tmp = (LFN_LINK*)malloc(sizeof(LFN_LINK));
				memset(lfn_tmp->name,0,LFN_PART_MAX+1);
				strncpy(lfn_tmp->name,tmp,strlen(tmp));
				lfn_tmp->next = lfn_head;
				lfn_head = lfn_tmp;
				lfn_tmp = NULL;
			}

			dirp->offset++;
		}else{
			break;
		}
	}
	if(str_len > 255){
		errno = 1201; 
		return -1;
	}

	if(sync_stat == 0){
		return 0;
	}

	while(lfn_head != NULL){
		lfn_tmp = lfn_head;
		strncat(result->name,lfn_head->name,strlen(lfn_head->name));
		lfn_head = lfn_head->next;
		free(lfn_tmp);
		lfn_tmp = NULL;
	}

	memcpy(&result->dirent,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);
	dirp->offset++;
	return 1;
}

/*
 * function description
 */
static int get_lfn_e(FATDIR *dirp, fat_dirent_t *result)
{
	uint8_t buf_offset=0;
	offset_t prev_offset=0;
	fat_dirent_lfn lfn_entry;
	char tmp[LFN_PART_MAX+1];
	int prev_len=0;

	memset(result->name,0,LFN_MAX);
	prev_offset = dirp->offset-1;
	buf_offset = prev_offset %16;

	if(prev_offset < 0){
		return -1;
	}else if(prev_offset%16==15){
		return -1;
	}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE] != FAT_FLAG_ERASE){
		return -1;
	}else if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11] != FAT_ATTR_LFN){
		return -1;
	}

	while(prev_offset % 16 != 15){
		buf_offset = prev_offset%16;
		if(dirp->buf[buf_offset*FAT_DIRENTRY_SIZE]==FAT_FLAG_ERASE &&
			dirp->buf[buf_offset*FAT_DIRENTRY_SIZE+11]==FAT_ATTR_LFN){
			memset(&lfn_entry,0,sizeof(lfn_entry));
			memcpy(&lfn_entry,&dirp->buf[buf_offset*FAT_DIRENTRY_SIZE],FAT_DIRENTRY_SIZE);

			memset(tmp,0,sizeof(tmp));
			if(combine_lfn(&lfn_entry,tmp)!=0){
				errno = 1200;
				return -1;
			}
			strncpy(result->name+prev_len,tmp,strlen(tmp));
			prev_len += strlen(tmp);
			prev_offset--;
		}else{
			break;
		}
	}

	return 0;
}

/*
 * function description
 */
static int combine_lfn(fat_dirent_lfn* lfn_dirent, char* result)
{
	int i, index=0;
	
	unsigned char name1[FAT_LFN_N1_SIZE+1];
	unsigned char name2[FAT_LFN_N2_SIZE+1];
	unsigned char name3[FAT_LFN_N3_SIZE+1];

	unsigned char letter[2];
	unsigned char convert[4];

	memset(name1,0,sizeof(name1));
	memset(name2,0,sizeof(name2));
	memset(name3,0,sizeof(name3));
	memset(convert,0,sizeof(convert));
	memset(letter,0,sizeof(letter));

	if(result == NULL){
		//set errno
		return -1;
	}
	for(i=0; i<FAT_LFN_B1_SIZE; i+=2){

		letter[0] = lfn_dirent->name1[i+1];
		letter[1] = lfn_dirent->name1[i];

		if(letter[1]==0xFF)
			break;
		UniToUTF8(letter,convert);

		if(strlen(name1)==0){
			strncpy(name1,convert,strlen(convert));
		}else{
			strncat(name1,convert,strlen(convert));
		}
	}

	memset(convert,0,sizeof(convert));
	memset(letter,0,sizeof(letter));

	for(i=0; i<FAT_LFN_B2_SIZE; i+=2){
		letter[0] = lfn_dirent->name2[i+1];
		letter[1] = lfn_dirent->name2[i];

		if(letter[1]==0xFF)
			break;

		UniToUTF8(letter, convert);

		if(strlen(name2)==0){
			strncpy(name2,convert,strlen(convert));
		}else{
			strncat(name2,convert,strlen(convert));
		}
	}

	memset(convert,0,sizeof(convert));
	memset(letter,0,sizeof(letter));

	for(i=0; i<FAT_LFN_B3_SIZE; i+=2){

		letter[0] = lfn_dirent->name3[i+1];
		letter[1] = lfn_dirent->name3[i];

		if(letter[1]==0xFF)
			break;

		UniToUTF8(letter,convert);
		if(strlen(name3)==0){
			strncpy(name3,convert,strlen(convert));
		}else{
			strncat(name3,convert,strlen(convert));
		}
	}

	strncpy(result,name1,strlen(name1));
	strncat(result,name2,strlen(name2));	
	strncat(result,name3,strlen(name3));
	return 0;
}

static int UniToUTF8(unsigned char *buf, unsigned char *result){
	unsigned char c1, c2, c3;
	uint16_t num = 0;


	if(buf == NULL || result == NULL)
		return -1;

	num = ((uint16_t)buf[0])<<8 | (uint16_t)buf[1];

	if(num < (uint16_t)0x80){
		result[0] = buf[1];
		result[1] = '\0';
		return 0;
	}else if(num < 0xAC00 || num > 0xD7AF){
		result[0]='\0';
	}
	c1 = buf[0] >> 4;
	c1 = c1 | 0xE0;

	c2 = (buf[0] & 0x0F) << 2;
	c2 = c2 | (buf[1] & 0xC0) >> 6;
	c2 = c2 | 0x80;

	c3 = buf[1] & 0x30;
	c3 = c3 | (buf[1] & 0x0F);
	c3 = c3 | 0x80;

	result[0] = c1;
	result[1] = c2;
	result[2] = c3;
	return 0;
}
/*
 * function description
 */
static int do_entry(traverse_info *info)
{
	int is_dir;

	if(lstat(info->name,&info->st)==-1)
		return -1;
	
	is_dir = S_ISDIR(info->st.st_mode);
	
	if(is_dir)
		do_dir(info);

	return 0;
}

/*
 * function description
 */
static int do_dir(traverse_info *info)
{
	DIR *sp = NULL;
	struct dirent *dp;
	int dirfd = -1;

	dirfd = open(".",O_RDONLY);

	if(dirfd==-1)
		return -1;

	if((sp=opendir(info->name))==NULL || chdir(info->name)==-1)
		return -1;

	info->count++;
	while((dp=readdir(sp))!=NULL){
		if(strcmp(dp->d_name,".")==0 || strcmp(dp->d_name,"..")==0)
			continue;
		info->name = dp->d_name;
		do_entry(info);
	}

	if(dirfd != -1){
		fchdir(dirfd);
		close(dirfd);
	}

	if(sp!=NULL)
		closedir(sp);

	return 0;
}
