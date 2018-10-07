
/*
 * FAT Cluster Module
 * 2013. 1. 21, v 0.0.1
 * FAT Filesystem FAT Table의 Cluster정보에 관한 함수들이 정의되어 있다.
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "defs.h"
#include "fat_cluster.h"

static int sync_cluster_buf(FATCLUSTER* c, cluster_t cluster_num);
static int add_cluster_chain(CLUSTER_CHAIN** head, cluster_t cluster_num);


/*
 * FAT Table의 클러스터 정보 Read하기전 FATCLUSTER구조체를 생성해야 한다.
 * FATCLUSTER구조체는 FAT Table 클러스터 정보를 읽는 데 필요한 변수들을
 * 멤버로 가지고 있다. 구조체는 fat_cluster.h에 정의되어 있다.
 * arguments
 *  - bs	 : FAT Boot sector 정보를 담고 있는 구조체의 주소
 *			   Boot sector 구조체는 get_fat_bootsec()함수를 통해 가져올 수 있다.
 *  - device : 해당 파일 시스템의 Block special file path 문자열
			   ex) "/dev/sda3"
 * return value
 *  -  FATCLUSTER Structure address
 *  -  Null: error
 * errno
 *  - system errors
 */
FATCLUSTER* open_fat_cluster(const struct fat_boot_sector* bs, const char* device)
{
	FATCLUSTER* f = NULL;
	if((f=(FATCLUSTER*)malloc(offsetof(struct FATCLUSTER, device)
								+ strlen(device)+1))==NULL)
		return NULL;
	memset(f, 0, offsetof(struct FATCLUSTER, device)+strlen(device)+1);
	f->sector_size = bs->sector_size;
	f->cluster_size = bs->sector_size * bs->sectors_per_cluster;
	f->fat_start_sector = bs->reserved_sectors;
	f->fat_end_sector = bs->reserved_sectors + bs->fat_sectors32 -1;
	f->p_sector = 0;
	f->max_cluster = ((bs->sectors32 - bs->reserved_sectors - bs->fats*bs->fat_sectors32)/bs->sectors_per_cluster)+1;
	memset(f->buf, 0, FAT_SECTOR_SIZE);
	strncpy( f->device, device,strlen(device)+1);
	return f;
}


/*
 * FAT Table의 클러스터 정보를 Read하기 위해 생성한 FATCLUSTER 구조체를 닫는다.
 * arguments
 *  - c: open_fat_cluster()로 생성한 FATCLUSTER 구조체의 주소
 * return value
 *  -  void
 * errno
 *  - no errors
 */
void close_fat_cluster(FATCLUSTER* c)
{
	(void)free(c);
}


/*
 * 특정 클러스터를 시작으로 하는 클러스터 체인을 가져온다.
 * 클러스터 체인 정보는 살아있는 데이터 뿐만 아니라 삭제된 데이터도 대상으로 한다.
 * 삭제된 데이터는 클러스터 체인 정보가 없기 때문에 세가지 기준으로 체인 정보를 추출한다.
 * 삭제된 데이터에 대한 추출 기준은 fat_cluster.h에 CHAIN_MODE enum type으로 정의되어 있다.
 * arguments
 *  - c				: open_fat_cluster()를 통해 생성한 FATCLUSTER 구조체 주소
 *  - first_cluster : 첫번째 클러스터 number
 *  - size			: 파일 크기 (ACTIVE Mode이외에는 size가 0이 올 수 없다.)
 *  - head			: CLUSTER_CHAIN Linked list header의 주소
 *  - mode			: ACTIVE -> 살아 있는 파일
 *					  CONTINIOUS -> 삭제된 데이터의 첫째 클러스터부터 연속된 클러스터 리스트
 *					  JUMP -> 삭제된 데이터의 첫째 클러스터부터 비할당된 클러스터의 리스트
 *					  HYBRID -> 삭제된 데이터의 첫째 클러스터부터 할당된 클러스터의 경우
								50%확률로 선택된 리스트
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 *   - EOBCLUSTER(1001): Cluster number is out of bound
 *   - EBADCLUSTER(1002): Bad Cluster
 *   - ENOTMATCH(1003): Cluster number is not match with File size
 *   - EFSIZEZERO(1004): File size zero is not supported
 */
int get_fat_cluster_chain(FATCLUSTER* c,
					  cluster_t first_cluster, 
					  unsigned long size,
					  CLUSTER_CHAIN** head, 
					  CHAIN_MODE mode)
{
	int i = 0;
	int count = 0;
	offset_t offset;
	cluster_t p = first_cluster, n=0;
	uint32_t cluster_num = 0;

	if(c == NULL || head == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	if( p < 2 || p > c->max_cluster){
		errno = EOBCLUSTER;
		EC_FAIL
	}

	if( size != 0 )
		cluster_num = (uint32_t)ceil(((double)size)/(c->cluster_size));
	else // directory
		cluster_num = c->max_cluster; // maximum size
	
	if(cluster_num > c->max_cluster){
		errno = EOBCLUSTER;
		EC_FAIL
	}

	free_fat_cluster_chain(head);

	switch(mode){
		case ACTIVE:
			for(i=0; i<cluster_num; i++){
				ec_neg1( sync_cluster_buf(c, p) )
				ec_neg1( add_cluster_chain(head, p))
				offset = get_cluster_offset(p);

				memcpy(&n, (c->buf)+offset, sizeof(fat_entry_t));
				count++;
				
				if(count > cluster_num){
					errno = EOBCLUSTER;
					EC_FAIL
				}

				if(n == FAT_ENTRY_BAD){ // bad cluster
					errno = EBADCLUSTER;
					EC_FAIL
				}
				else if(n >= FAT_ENTRY_EOC){
					if( size != 0 && count != cluster_num){
						errno = ENOTMATCH;
						EC_FAIL
					}
					break;
				}
				p = n;
			}
			break;
		case CONTINUOUS:
			if(size == 0){ // invalid
				errno = EFSIZEZERO;
				EC_FAIL
			}
			for(i=0; i<cluster_num; i++, p++){
				ec_neg1(sync_cluster_buf(c, p))
				ec_neg1(add_cluster_chain(head, p))
			}
			count = i;
			break;
		case JUMP:
			if(size == 0){ // invalid
				errno = EFSIZEZERO;
				EC_FAIL
			}
			for(i=0; i<cluster_num; ){
				ec_neg1(sync_cluster_buf(c, p))
				offset = get_cluster_offset(p);
				memcpy(&n, (c->buf)+offset, sizeof(cluster_t));
				if( n != 0x00000000 ){
					p++;  // Jump
					continue;
				}
				if(p > c->max_cluster){
					errno = EOBCLUSTER;
					EC_FAIL
				}
				ec_neg1(add_cluster_chain(head, p))
				p++;
				i++;
			}
			count = i;
			break;
		case HYBRID:
			srand((unsigned int)time(NULL));
			if(size == 0){
				errno = EFSIZEZERO;
				EC_FAIL
			}
			for(i=0; i<cluster_num; ){
				ec_neg1(sync_cluster_buf(c, p))
				offset = get_cluster_offset(p);
				memcpy(&n, (c->buf)+offset, sizeof(cluster_t));
				if( n != 0x00000000 ){
					if( rand()%2 ){ // 50% Chance
						ec_neg1(add_cluster_chain(head, p))
						p++;
						i++;
						continue;
					}
					else{
						// jump
						p++;
						continue;
					}
				}
				if(p > c->max_cluster){
					errno = EFSIZEZERO;
					EC_FAIL
				}
				ec_neg1(add_cluster_chain(head, p))
				p++;
				i++;
			}
			count = i;
			break;
		default:
			EC_FAIL
	}

	return count;

	EC_CLEANUP_BGN
		(void)free_fat_cluster_chain(head);
		*head = NULL;
		return -1;
	EC_CLEANUP_END
}

int guess_recovery_possibility( const struct fat_boot_sector* bs, 
							const char* device,
							cluster_t first_cluster, 
							unsigned long size)
{
	int i = 0;
	int count = 0;
	offset_t offset;
	cluster_t p = first_cluster, n=0;
	uint32_t cluster_num = 0;
	FATCLUSTER* c = NULL;
	int corrupted = 0;
	double possibility = 0.0;
	
	ec_null( c = open_fat_cluster( bs, device) );
	
	if( p < 2 || p > c->max_cluster){
		errno = EOBCLUSTER;
		EC_FAIL
	}
	
	if( size != 0 )
		cluster_num = (uint32_t)ceil(((double)size)/(c->cluster_size));
	else // directory
		cluster_num = c->max_cluster; // maximum size

	if(cluster_num > c->max_cluster){
		errno = EOBCLUSTER;
		EC_FAIL
	}

	if( size == 0 ){ // invalid
		errno = EFSIZEZERO;
		EC_FAIL
	}
	
	for(i=0; i<cluster_num; i++, p++){
		ec_neg1(sync_cluster_buf(c, p))
		offset = get_cluster_offset(p);
		memcpy(&n, (c->buf)+offset, sizeof(cluster_t));
		if( n != 0x00000000 ){
			corrupted++;
		}
		if(p > c->max_cluster){
			errno = EOBCLUSTER;
			EC_FAIL
		}
	}
	possibility = ((double)(cluster_num - corrupted)) / (double)cluster_num;
	possibility = possibility * 100;

	close_fat_cluster(c);
	return (int)possibility;

	EC_CLEANUP_BGN
		if( c != NULL)
			close_fat_cluster(c);
		return -1;
	EC_CLEANUP_END
}

/*
 * CLUSTER_CHAIN Linked list를 메모리에 반환한다.
 * arguments
 *  - head: CLUSTER_CHAIN Linked list header address
 * return value
 *  - void
 * errno
 *  - no error
 */
void free_fat_cluster_chain(CLUSTER_CHAIN** head)
{
	CLUSTER_CHAIN *p, *n;
	for(p=*head; p; p=n){
		n = p->next;
		(void)free(p);
	}
	*head = NULL;
}


/*
 * FATCLUSTER구조체 내의 buf에는 특정 섹터의 512byte 값이 들어있다.
 * File I/O를 최소로 하기위해 구조체내에 Buf를 유지한다.
 * sync_cluster_buf()함수는 특정 클러스터 정보가 현재 buf안에 들어있는지
 * 확인하고 buf를 동기화 시켜준다. 인자로 전달된 클러스터 주소가 해당 buf에
 * 없으면 해당 sector를 다시 계산하여 512byte를 읽어 들인 후 buf에 저장한다.
 * arguments
 *  - c			  : FATCLUSTER 구조체 주소
 *  - cluster_num : cluster number
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 *  - Additional errors
 *   - EREADZERO(1005): File size zero is not supported
 */
static int sync_cluster_buf(FATCLUSTER* c, cluster_t cluster_num)
{
	int nread;
	int fd = -1;
	sector_t s;

	s = get_cluster_sector(c->fat_start_sector, cluster_num);

	if(c->p_sector != s){
		if( (fd = open(c->device, O_RDONLY)) == -1)
			return -1;
		
		lseek64(fd, ((offset_t)s)*FAT_SECTOR_SIZE, SEEK_SET);
		switch(nread=read(fd,c->buf,FAT_SECTOR_SIZE)){
			case 0:
				errno = EREADZERO;
				(void)close(fd);
				return -1;
			case -1:
				(void)close(fd);
				return -1;
			default:
				if(nread != FAT_SECTOR_SIZE){
					errno = EREADSIZE;
					(void)close(fd);
					return -1;
				}
		}
		c->p_sector = s;
		(void)close(fd);
	}
	return 0;
}


/*
 * CLUSTER_CHAIN Linked list에 클러스터를 추가시킨다.
 * arguments
 *  - head		  : CLUSTER_CHAIN Linked list header 주소
 *  - cluster_num : cluster number
 * return value
 *  -  0: success
 *  - -1: error
 * errno
 *  - system errors
 */
static int add_cluster_chain(CLUSTER_CHAIN** head, cluster_t cluster_num)
{
	CLUSTER_CHAIN *p = NULL;

	if(*head == NULL){
		if((*head = (CLUSTER_CHAIN*)malloc(sizeof(CLUSTER_CHAIN))) == NULL){
			return -1;
		}
		(*head)->cluster_num = cluster_num;
		(*head)->next = NULL;
		return 0;
	}

	for( p=*head; p->next; p=p->next);
	
	if( (p->next = (CLUSTER_CHAIN*)malloc(sizeof(CLUSTER_CHAIN))) == NULL){
		return -1;
	}
	p->next->cluster_num = cluster_num;
	p->next->next = NULL;

	return 0;
}

void print_fat_cluster_chain(CLUSTER_CHAIN* head, int count)
{
	int i;
	CLUSTER_CHAIN *p;

	printf("\n\n[Cluster Chain]\n");
	for(i=0, p=head; p; p=p->next, i++)
	{
		printf("[cluster #%d] %ld\n", i+1, (unsigned long)(p->cluster_num));
	}
}
