
/*
 * Get Device information Module
 * 2013. 1. 21, v 0.0.1
 * 시스템에 Mount된 장치들의 정보를 탐색한다.
 * 전체 장치, 특정타입의 장치, 특정 경로의 장치를 탐색
 * 연결 리스트 형태로 반환한다.
 * 해당 모듈에서 사용되는 구조체는 get_device.h에 정의되어 있다.
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "get_device.h"
#include "defs.h"
#include <sys/statfs.h>

static int add_device_list(DEVLIST** head, const DEVLIST* node);


/*
 * 현재 마운트된 모든 장치 리스트 정보를 가지고 온다.
 * arguments
 *  - head: Linked list 구조체 header 주소
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int get_device_list(DEVLIST** head)
{
	FILE* f;
	char mount_dev[256];
	char mount_dir[256];
	char mount_type[256];
	char mount_opts[256];
	int mount_freq;
	int mount_passno;
	int match;
	
	struct statfs st;
	DEVLIST tmp;

	free_device_list(head);
	ec_null(f = fopen(MT_INFO_PATH, "r"))

	do{
		match = fscanf(f, "%255s %255s %255s %255s %d %d\n",
						mount_dev, mount_dir, mount_type,
						mount_opts, &mount_freq, &mount_passno);
		mount_dev[255] = '\0';
		mount_dir[255] = '\0';
		mount_type[255] = '\0';
		mount_opts[255] = '\0';

		// if match add this device info to linked list
		if( match == 6){
			ec_neg1(statfs(mount_dir, &st))

			memset(&tmp, 0, sizeof(tmp));
			strncpy(tmp.fs_dev, mount_dev, 256);
			strncpy(tmp.mount_dir, mount_dir, 256);
			strncpy(tmp.fs_type, mount_type, 256);
			strncpy(tmp.mount_opts, mount_opts, 256);
			tmp.total_size = (long long)st.f_blocks * (long long)st.f_bsize;
			tmp.free_size = (long long)st.f_bfree * (long long)st.f_bsize;
			tmp.next = NULL;

			ec_neg1(add_device_list(head, &tmp))
		}
	} while(match != EOF);

	(void)fclose(f);
	return 0;

	EC_CLEANUP_BGN
		if( f != NULL )
			(void)fclose(f);
		(void)free_device_list(head);
		return -1;
	EC_CLEANUP_END
}

/*
 * 현재 마운트된 장치 중 특정 파일시스템 타입의 리스트 정보를 가지고 온다.
 * arguments
 *  - head: Linked list 구조체 header 주소
 *  - type: File system type
 *			ex) "vfat", "ext3"
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
int get_device_list_with_type(DEVLIST** head, const char* type)
{
	FILE* f = NULL;
	char mount_dev[256];
	char mount_dir[256];
	char mount_type[256];
	char mount_opts[256];
	int mount_freq;
	int mount_passno;
	int match;

	struct statfs st;
	DEVLIST tmp;

	if(type==NULL){
		errno = EINVAL;
		EC_FAIL
	}

	free_device_list(head);
	ec_null(f = fopen(MT_INFO_PATH, "r"))
	do{
		match = fscanf(f, "%255s %255s %255s %255s %d %d\n",
			mount_dev, mount_dir, mount_type, mount_opts,
			&mount_freq, &mount_passno);
		mount_dev[255] = '\0';
		mount_dir[255] = '\0';
		mount_type[255] = '\0';
		mount_opts[255] = '\0';

		// if match add this device info to linked list
		if( match==6 && strncmp(mount_type, type, 255)==0){
			ec_neg1(statfs(mount_dir, &st))
			
			memset(&tmp, 0, sizeof(tmp));
			strncpy(tmp.fs_dev, mount_dev, 256);
			strncpy(tmp.mount_dir, mount_dir, 256);
			strncpy(tmp.fs_type, mount_type, 256);
			strncpy(tmp.mount_opts, mount_opts, 256);
			tmp.total_size = (long long)st.f_blocks * (long long)st.f_bsize;
			tmp.free_size = (long long)st.f_bfree * (long long)st.f_bsize;
			tmp.next = NULL;

			ec_neg1(add_device_list(head, &tmp))
		}
	} while(match != EOF);

	(void)fclose(f);
	return 0;

	EC_CLEANUP_BGN
		if( f != NULL )
			(void)fclose(f);
		(void)free_device_list(head);
		return -1;
	EC_CLEANUP_END
}


/*
 * 현재 마운트된 장치 중 경로의 파일시스템 타입의 리스트 정보를 가지고 온다.
 * arguments
 *  - devpath: 장치 경로
 *			   ex) "/dev/sda3"
 *  - dev: DEV_INFO Structure 주소
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 *   - EMATCHINFO(1009): Nothing match
 */
int get_device_info(const char* devpath, struct DEV_INFO* const dev)
{
	FILE* f = NULL;
	char mount_dev[256];
	char mount_dir[256];
	char mount_type[256];
	char mount_opts[256];
	int mount_freq;
	int mount_passno;
	int match;

	if(devpath == NULL || dev == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	ec_null(f = fopen(MT_INFO_PATH, "r"))
	do{
		match = fscanf(f, "%255s %255s %255s %255s %d %d\n",
			mount_dev, mount_dir, mount_type, mount_opts,
			&mount_freq, &mount_passno);
		mount_dev[255] = '\0';
		mount_dir[255] = '\0';
		mount_type[255] = '\0';
		mount_opts[255] = '\0';

		// if match, save device information to device structure
		if( match==6 && strncmp(mount_dev, devpath, 255)==0){
			strncpy(dev->fs_dev, mount_dev, 256);
			strncpy(dev->mount_dir, mount_dir, 256);
			strncpy(dev->fs_type, mount_type, 256);
			break;
		}
	} while(match != EOF);

	if(match == EOF){
		errno = EMATCHINFO;
		EC_FAIL
	}

	(void)fclose(f);
	return 0;

	EC_CLEANUP_BGN
		if( f != NULL )
			(void)fclose(f);
		return -1;
	EC_CLEANUP_END
}


/*
 * DEVLIST Linked list 메모리 반환
 * arguments
 *  - head: DEVLIST Linked list header 주소
 * return value
 *  - void
 * errno
 *  - no error
 */
void free_device_list(DEVLIST** head)
{
	DEVLIST *p = NULL, *n = NULL;

	for( p = *head; p; p=n){
		n = p->next;
		free(p);
	}
	*head = NULL;
}

/*
 * 특정 파티션의 가용 공간을 알아온다.
 * arguments
 *  - mount_dir: 마운트 포인트
 * return value
 *  - 가용공간 (byte 단위)
 *  - -1: error
 * errno
 *  - system errors
 */
long long get_device_free_size(const char* mount_point)
{
	struct statfs st;
	long long free_size;

	if( statfs(mount_point, &st) == -1)
		return -1;

	free_size = (long long)st.f_bfree * (long long)st.f_bsize;
	return free_size;
}


/*
 * DEVLIST Linked list에 노드를 추가한다.
 * arguments
 *  - head: DEVLIST Linked list header 주소
 *  - node: 추가할 정보가 있는 DEVLIST 구조체 주소
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
static int add_device_list(DEVLIST** head, const DEVLIST* node)
{
	DEVLIST *p = NULL;
	
	if( *head == NULL ){
		if( (*head = (DEVLIST*)malloc(sizeof(DEVLIST))) == NULL )
			return -1;

		memset(*head, 0, sizeof(DEVLIST));
		strncpy((*head)->fs_dev, node->fs_dev, 256);
		strncpy((*head)->mount_dir, node->mount_dir, 256);
		strncpy((*head)->fs_type, node->fs_type, 256);
		strncpy((*head)->mount_opts, node->mount_opts, 256);
		(*head)->total_size = node->total_size;
		(*head)->free_size = node->free_size;
		(*head)->next = NULL;
		return 0;
	}

	// p node를 마지막 으로 이동
	for( p=*head; p->next; p=p->next );

	if( (p->next = (DEVLIST*)malloc(sizeof(DEVLIST))) == NULL)
		return -1;

	memset(p->next, 0, sizeof(DEVLIST));
	strncpy(p->next->fs_dev, node->fs_dev, 256);
	strncpy(p->next->mount_dir, node->mount_dir, 256);
	strncpy(p->next->fs_type, node->fs_type, 256);
	strncpy(p->next->mount_opts, node->mount_opts, 256);
	p->next->total_size = node->total_size;
	p->next->free_size = node->free_size;
	p->next->next = NULL;

	return 0;
}

/*
 * For Debugging
 * Just print the list
 */
void print_device_list(DEVLIST* head)
{
	DEVLIST *p;

	if(head == NULL){
		fprintf(stderr, "Device List is empty\n");
		return;
	}
	printf("fs_dev  mount_dir total_size  free_size\n");
	for( p=head; p; p=p->next){
		printf("%s %s %s %lld %lld\n",
			p->fs_dev,
			p->mount_dir,
			p->fs_type,
			p->total_size,
			p->free_size);
	}
}
