
/*
 * Recover and Scan Module
 * 2013. 1. 21, v 0.0.1
 * ���� ������ ������ ��ĳ��
 * ������ ������ ���� ���
 * Author	: Seungjae, Lee
 *			: Minseok, Kwon (recover_file_mv, init_temp)
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "fat_recover.h"

static offset_t get_offset_byte(const struct fat_boot_sector* bsinfo, cluster_t cluster_num);

/*
 * ������ �����͸� �����ϴ� �Լ�
 * ������ �������� Ŭ������ ü�� ����Ʈ ������ �������� �����͸� �����Ѵ�.
 * �ش� �����ʹ� �ӽ������� �ý��� ������ �����ȴ�. ��δ� fat_recover.h�� ���ǵǾ� �ִ�.
 * �ӽ������� �����ϴ� ��Ƽ���� ������� Ȯ���� �� ���� ������ ũ�Ⱑ ������� ���� ������
 * ���� �ʵ��� �Ѵ�. ������ �ʰ��ϸ� ������ ��ȯ�Ѵ�. �� ������ fat_recover.h��
 * THRESHOLD������ ���ǵ� �ִ�.
 * arguments
 *  - bsinfo   : FAT Bootsector Struture �ּ�
 *  - device   : Block special file path
 *				 ex) "/dev/sda3"
 *	- filename : ������ �ӽ� ���� �̸�
 *  - filesize : ���� ���� ũ��
 *  - head	   : Cluster chain list header
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 *   - EOVERSIZE_T(1007): File size is too big to recover temporarly
 *   - EREADSIZE(1006): Not equal intent size and real read size
 */
int recover_file_tmp( const struct fat_boot_sector* bsinfo,
					  const char* device,
					  const char* filename, 
					  unsigned long filesize,
					  const CLUSTER_CHAIN* head)
{
	int rdfd = -1, wrfd = -1;
	char* buf = NULL;
	char* path = NULL;
	int filename_len = 0;
	int tmppath_len = 0;
	int nread = 0;
	cluster_t cluster_count = 0;
	uint32_t cluster_num = 0;
	unsigned long cluster_size = 0;
	const CLUSTER_CHAIN* p = head;
	offset_t offset = 0;
	int readsize = 0;

	if(bsinfo == NULL || device == NULL || filename == NULL ||
		filesize <= 0 || head == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	// over size
	if( get_device_free_size(MOUNTPOINT_TMP)*THRESHOLD < filesize ){
		errno = EOVERSIZE_T;
		EC_FAIL
	}

	filename_len = strlen(filename);
	tmppath_len = strlen(PATH_TMPRECOVER);

	ec_null(path=(char*)malloc(filename_len+tmppath_len+1))
	memset(path, 0, filename_len+tmppath_len+1);
	strcat(path,PATH_TMPRECOVER);
	strcat(path,filename);
	
	cluster_size = FAT_SECTOR_SIZE * (bsinfo->sectors_per_cluster);
	ec_null( buf = (char*)malloc(cluster_size) )
	
	cluster_num = (uint32_t)ceil(((double)filesize)/cluster_size);
	
	ec_neg1( rdfd = open(device, O_RDONLY) )
	ec_neg1( wrfd = open(path, O_WRONLY| O_CREAT| O_TRUNC, PERM_FILE) )
	
	do{
		cluster_count++;
		if(cluster_count == cluster_num){
			if( filesize%cluster_size == 0){
				readsize = cluster_size;
			}
			else { 
				readsize = filesize%cluster_size;
			}
		}
		else
			readsize = cluster_size;

		offset = get_offset_byte(bsinfo, p->cluster_num);
		//nread = pread(rdfd, buf, readsize, offset);
		if(lseek64(rdfd,offset,SEEK_SET)==-1)
			return -1;
		nread = read(rdfd,buf,readsize);

		if(nread == 0)
			break;
		else if(nread == -1)
			EC_FAIL
		else if(nread != readsize){
			errno = EREADSIZE;
			EC_FAIL
		}
		ec_neg1( write(wrfd, buf, nread) )
	}while( (p = p->next) != NULL );
	
	(void)free(buf);
	(void)free(path);
	(void)close(rdfd);
	(void)close(wrfd);
	return 0;

	EC_CLEANUP_BGN
		(void)free(buf);
		(void)free(path);
		if(wrfd != -1){
			unlink(path);
			close(wrfd);
		}
		if(rdfd != -1){
			close(rdfd);
		}
		return -1;
	EC_CLEANUP_END
}


/*
 * �ӽ���ҿ� ������ �����͸� ������ ��ҷ� �̵� ��Ų��.
 * arguments
 *  - src: �ӽ� ������ ���
 *  - dst: ������ ���� ��ų ���
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int recover_file_mv(const char* src, const char* dst){
	int fromFd = -1, toFd = -1;
	ssize_t nread, nwrite, n;
	char buf[BUFSIZE];

	if(src == NULL || dst == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	ec_neg1( fromFd = open(src, O_RDONLY) )
	ec_neg1( toFd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXO) )

	while( (nread = read(fromFd, buf, sizeof(buf))) > 0 ){
		nwrite = 0;
		do{
			ec_neg1( n = write(toFd, &buf[nwrite], nread - nwrite) )
			nwrite += n;
		}while(nwrite < nread);
	}

	if(nread == -1)
		EC_FAIL

	ec_neg1( close(fromFd) )
	ec_neg1( close(toFd) )
	return 0;

	EC_CLEANUP_BGN
		if(fromFd != -1)
			(void)close(fromFd);
		if(toFd != -1)
			(void)close(toFd);
		return -1;
	EC_CLEANUP_END
}


 /*
 * function description
 *  �ӽ� ���� ��ο� �����Ͽ� ���� �����͵��� �ʱ�ȭ ��Ų��.
 *  ���� ���� ��ΰ� �������� �ʴ´ٸ� �ӽ� ���� ��θ� ���� �����.
 *  �ӽ� ���� ��δ� ������ �����Ƿ� ��� parameter�� �Ѱ����� �ʴ´�.
 * arguments
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int fat_init_temp(){
	DIR *dir = NULL;
//	struct dirent *entry;

	dir = opendir(PATH_TMPRECOVER);

	//�������� �ʴ� path�̸� ���丮 ��
	if(dir == NULL){
		ec_neg1(mkdir(PATH_TMPRECOVER, 0777));
	}
	else{
		system(CMD_RM);
	}

	if(dir != NULL)
		(void)closedir(dir);

	return 0;

	EC_CLEANUP_BGN
		if(dir != NULL)
			(void)closedir(dir);
		return -1;
	EC_CLEANUP_END
}


/*
 * Ư�� Ŭ�������� ��ġ�� �ش��ϴ� byte������ offset�� ��ȯ�Ѵ�.
 * arguments
 *  - bsinfo	  : FAT Bootsector ����ü �ּ�
 *  - cluster_num : cluster number
 * return value
 *  -  byte offset
 * errno
 *  - no error
 */
static offset_t get_offset_byte(const struct fat_boot_sector* bsinfo, cluster_t cluster_num)
{
	offset_t offset = 0;

	offset += (bsinfo->reserved_sectors);
	offset += (bsinfo->fat_sectors32)*(bsinfo->fats);
	offset += (cluster_num - 2)*(bsinfo->sectors_per_cluster);
	offset *= FAT_SECTOR_SIZE;

	return offset;
}


/*
 * RECOVER_LIST Linked list�� item�� �߰��Ѵ�.
 * arguments
 *  - head: RECOVER_LIST Linked list header address
 *  - item: �߰��ϰ��� �ϴ� ������ ��� �ִ� ����ü �ּ�
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int add_recover_list(RECOVER_LIST** head, RECOVER_LIST* item)
{
	RECOVER_LIST* p;

	if(head == NULL || item == NULL){
		errno = EINVAL;
		return -1;
	}

	if( *head == NULL ){
		if( (*head = (RECOVER_LIST*)malloc(sizeof(RECOVER_LIST))) == NULL){
			return -1;
		}
		memcpy(*head, item, sizeof(RECOVER_LIST));
		(*head)->next = NULL;
		return 0;
	}
	
	for(p=*head; p->next; p=p->next);
	
	if( (p->next = (RECOVER_LIST*)malloc(sizeof(RECOVER_LIST))) == NULL)
		return -1;

	memcpy(p->next, item, sizeof(RECOVER_LIST));
	p->next->next = NULL;

	return 0;
}


/*
 * RECOVER_LIST Linked list �޸� ��ȯ
 * arguments
 *  - head: RECOVER_LIST Linked list header address
 * return value
 *  -  void
 * errno
 *  - no error
 */
void free_recover_list(RECOVER_LIST** head)
{
	RECOVER_LIST *p, *n;

	for(p=*head; p; p=n){
		n = p->next;
		(void)free(p->dir_path);
		(void)free(p);
	}

	*head = NULL;
}


/*
 * Ư�� ���丮�� �����ִ� ������ ������ ���̺��� ��ĵ�� ��
 * RECOVER_LIST Linked list�� �߰�
 * arguments
 *  - device : DEV_INFO Structure �ּ� (device ������ ��� �ְ� get_device.h�� ���ǵ�)
 *  - bsinfo : FAT Bootsector ����ü �ּ�
 *  - path	 : block special file path 
 *			   ex) "/dev/sda3"
 *  - header : RECOVER_LIST Linked list header address
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int fatscan_dir( const struct DEV_INFO* device,
				 const struct fat_boot_sector* bsinfo,
				 const char *path,
				 RECOVER_LIST** header)
{
	FATDIR* fdir = NULL;
	fat_dirent_t result;
	int read_result = 0;
	RECOVER_LIST tmp;

	if( device == NULL || path == NULL || 
		header == NULL || bsinfo == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	ec_null( fdir = fat_opendir( device->fs_dev, path,
						device->mount_dir, bsinfo) );

	while( (read_result = fat_readdir(fdir, &result, D_REG)) != 0 ){
		ec_neg1(read_result)

		memset(&tmp, 0, sizeof(RECOVER_LIST));

		tmp.first_cluster = CAL_FIRST_CLUSTER(result.dirent.first_cluster_hi,
											  result.dirent.first_cluster_lo);

		tmp.size = result.dirent.size;

		// validation check
		if( tmp.size <= 0 || tmp.first_cluster <= 0)
			continue;

		strncpy( tmp.filename, result.name , LFN_MAX);
		strncpy( tmp.extender, result.dirent.extender, 3);
		tmp.dir_path = strdup(path);
		tmp.expected_possibility = guess_recovery_possibility(bsinfo, device->fs_dev,
									tmp.first_cluster, tmp.size); // not implement now
		tmp.next = NULL;
		ec_neg1(add_recover_list(header, &tmp));

	}
	fat_closedir(fdir);
	return 0;

	EC_CLEANUP_BGN
		if(fdir != NULL)
			fat_closedir(fdir);
		return -1;
	EC_CLEANUP_END
}

// for debugging
void print_recover_list(RECOVER_LIST* header)
{
	int i;
	char tmpfilename[LFN_MAX] ={0,};
	char tmpextender[4] ={0,};
	RECOVER_LIST* p;

	for(i=1, p=header; p; p=p->next, i++){
		strncpy(tmpfilename, p->filename, LFN_MAX);
		strncpy(tmpextender, p->extender, 3);
#ifdef ANDROID
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "[Recover List %d]", i);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "path: %s", p->dir_path);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Filename: %s", tmpfilename);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Extender: %s", tmpextender);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "FirstCluster: %ld", (unsigned long)(p->first_cluster));
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "File Size: %ld", p->size);
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "Possibility: %d", p->expected_possibility);
#else
		printf("\n[Recover List %d]\n", i);
		printf("path: %s\n", p->dir_path);
		printf("Filename: %s\n", tmpfilename);
		printf("Extender: %s\n", tmpextender);
		printf("FirstCluster: %ld\n", (unsigned long)(p->first_cluster));
		printf("File Size: %ld\n", p->size);
		printf("Possibility: %d\n", p->expected_possibility);
#endif
	}
}
