/*
 * FAT File system Interface (Android & C Library)
 * 2013. 1. 21, v 0.0.1
 * �ȵ���̵�?���� C library ����
 * Author	: Seungjae, Lee
 * 2013 Spring semester, Capstone Project
 * Handong University
 */

#include "defs.h"
#include "fat_recover.h"
#include "get_device.h"
#include "fat_dir.h"
#include "fat_cluster.h"
#include "fat_bootsec.h"
#include "fat_delete.h"
#include "fat_dscan.h"
#include "jni.h"
#include "jnihelp.h"
#include <string.h>

int register_android_recover_fat(JNIEnv* env);

typedef struct RECOVER_LIST_SCAN_HEADER{			
	unsigned long current_location;	
	char* path;						
	RECOVER_LIST* head;
} RECOVER_LIST_SCAN_HEADER;

static int do_dir(JNIEnv* env, jobject asyncobj, jmethodID publishProgressID,
				  char* path, const struct DEV_INFO* dev,
				  RECOVER_LIST_SCAN_HEADER* const header, 
				  struct fat_boot_sector* bsinfo);
static RECOVER_LIST_SCAN_HEADER* create_scan_header(const char* toppath);
static int free_scan_header(RECOVER_LIST_SCAN_HEADER* header);
static int fatscan_path(JNIEnv* env, jobject asyncobj, struct DEV_INFO* dev, RECOVER_LIST_SCAN_HEADER* header);
int register_android_recover_fat(JNIEnv* env);

static jint native_recover_file_tmp(JNIEnv* env, jobject obj,
								jstring device_path, jstring file_name, jstring mount_dir,
								jint f_cluster, jlong file_size);

static jint native_recover_file_mv(JNIEnv* env, jobject obj,
								jstring file_name, jstring mount_dir);

static jint native_recover_file(JNIEnv* env, jobject obj, 
								jstring device_path, jstring file_name, jstring mount_dir,
								jint f_cluster, jlong file_size);

static jint native_get_pathcount(JNIEnv* env, jobject obj,
								jstring device_path, jstring top_path, jstring mount_dir);

static jobject native_scan_path(JNIEnv* env, jobject obj, jobject asyncobj,
								jstring device_path, jstring top_path, jstring mount_dir);

static jobject native_scan_detail(JNIEnv* env, jobject obj, jobject asyncobj, jstring device_path);

static jint native_delete_file_with_cluster(JNIEnv* env, jobject obj,
								jstring device_path, jstring mount_point,
								jbyteArray dir_path, jbyteArray file_name, jint f_cluster, jlong size);

static jint native_delete_file_with_filename(JNIEnv* env, jobject obj,
								jstring device_path, jstring mount_point,
								jstring dir_path, jstring file_name);

static jint native_fat_init_temp(JNIEnv* env, jobject obj);
static jobject convertRecoverList2ArrayList(JNIEnv* env, RECOVER_LIST* head);

// static char* replaceUnUTF8Chars(char* str, int length);

static RECOVER_LIST_SCAN_HEADER* create_scan_header(const char* toppath)
{
	RECOVER_LIST_SCAN_HEADER* header = NULL;

	if( toppath == NULL){
		errno = EINVAL;
		return NULL;
	}
	
	if( (header = (RECOVER_LIST_SCAN_HEADER*)malloc(sizeof(RECOVER_LIST_SCAN_HEADER))) == NULL)
			return NULL;
	
	header->head = NULL;
	header->current_location = 0;

	if( (header->path = (char*)malloc(strlen(toppath)+1)) == NULL){
		free(header);
		return NULL;
	}
	memset(header->path,0, strlen(toppath)+1);
	strncpy( header->path, toppath, strlen(toppath));

	return header;
}

static int free_scan_header(RECOVER_LIST_SCAN_HEADER* header)
{
	if( header->head != NULL ){
		errno = ESEQFREE;
		return -1;
	}
	free(header->path);
	free(header);
	return 0;
}


static int do_dir(JNIEnv* env, jobject asyncobj, jmethodID publishProgressID,
				  char* path, const struct DEV_INFO* dev,
				  RECOVER_LIST_SCAN_HEADER* const header, 
				  struct fat_boot_sector* bsinfo)
{
	FATDIR* fdir = NULL;
	fat_dirent_t result;
	int read_result = 0;
	char* tmppath = NULL;

	jstring jpath;
	jobject jnum;
	jclass objectArrayClass;
	jobjectArray progressData;

	/* Scan */
	ec_neg1( fatscan_dir(dev, bsinfo, header->path, &(header->head)) );
//	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "scanning: %s", header->path);

	/* Call Back to User Interface (Progress) */
	(header->current_location)++;
	ec_null(jpath = (*env)->NewStringUTF(env,header->path));
	if(asyncobj == NULL || publishProgressID == NULL ){
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "asyncobj == NULL || publishProgressID == NULL");
		EC_FAIL
	}
	ec_null( objectArrayClass = (*env)->FindClass(env, "[Ljava/lang/Object;") );
	ec_null( progressData = (*env)->NewObjectArray(env, 2, objectArrayClass, 0));
	ec_null( jnum = toInteger(env, header->current_location));
	if(progressData==NULL || jnum==NULL){
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "null");
		EC_FAIL
	}
	(*env)->SetObjectArrayElement(env, progressData, 0, jnum);
	(*env)->SetObjectArrayElement(env, progressData, 1, jpath);
	// publish progress
	(*env)->CallVoidMethod(env, asyncobj, publishProgressID, progressData);
	// release references
	(*env)->DeleteLocalRef(env, jpath);
	(*env)->DeleteLocalRef(env, jnum);
	(*env)->DeleteLocalRef(env, objectArrayClass);
	(*env)->DeleteLocalRef(env, progressData);

	// �ڽ��� Sub directory Ž��
	ec_null( fdir = fat_opendir(dev->fs_dev, header->path, dev->mount_dir, bsinfo) );
	while( (read_result = fat_readdir(fdir, &result, A_DIR)) != 0 ){
		ec_neg1(read_result);
	
		// save path
		tmppath = strdup( header->path );
		if( (header->path = realloc(header->path, strlen(header->path) + strlen(result.name) + 2))
			== NULL){
			EC_FAIL
		}
		strcat(header->path, result.name);
		strcat(header->path, "/");
		ec_neg1(do_dir(env, asyncobj, publishProgressID, header->path, dev, header, bsinfo ));
		// path roll back
		if( (header->path = (char*)realloc(header->path, strlen(tmppath)+1)) == NULL){
			EC_FAIL
		}
		memset(header->path, 0, strlen(tmppath)+1);
		memcpy(header->path, tmppath, strlen(tmppath));
		(void)free(tmppath);
		tmppath = NULL;
	}
	(void)fat_closedir(fdir);
	return 0;

	EC_CLEANUP_BGN
		if(fdir != NULL)
			(void)fat_closedir(fdir);
		if(tmppath != NULL)
			(void)free(tmppath);
		return -1;
	EC_CLEANUP_END
}

static int fatscan_path(JNIEnv* env, jobject asyncobj, struct DEV_INFO* dev, RECOVER_LIST_SCAN_HEADER* header)
{
	struct fat_boot_sector bsinfo;
	jclass asyncClass = NULL;
	jmethodID publishProgressID = NULL;

	if(	dev == NULL || 	header == NULL){
		errno = EINVAL;
		EC_FAIL
	}

	asyncClass = (*env)->GetObjectClass(env, asyncobj);
	if(asyncClass == NULL){
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "classID is null");
		EC_FAIL
	}

	publishProgressID = (*env)->GetMethodID(env, asyncClass, "publishProgress", "([Ljava/lang/Object;)V");

	if(publishProgressID == NULL){
		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "methodID is null");
		EC_FAIL
	}
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "device: %s", dev->fs_dev);
	ec_neg1( get_fat_bootsec(dev->fs_dev, &bsinfo) )

	ec_neg1( do_dir(env, asyncobj, publishProgressID, header->path, dev, header, &bsinfo) );
	
	return 0;

	EC_CLEANUP_BGN
		return -1;
	EC_CLEANUP_END
}

static JNINativeMethod sMethods[] = {
	{"native_recover_file", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IJ)I", (void*)native_recover_file },
	{"native_recover_file_tmp", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IJ)I", (void*)native_recover_file_tmp},
	{"native_recover_file_mv", "(Ljava/lang/String;Ljava/lang/String;)I",(void*)native_recover_file_mv},
	{"native_get_pathcount", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I", (void*)native_get_pathcount},
	{"native_scan_path", "(Landroid/os/AsyncTask;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/util/ArrayList;",(void*)native_scan_path },
	{"native_scan_detail", "(Landroid/os/AsyncTask;Ljava/lang/String;)Ljava/util/ArrayList;", (void*)native_scan_detail},
	{"native_delete_file_with_cluster", "(Ljava/lang/String;Ljava/lang/String;[B[BIJ)I",(void*)native_delete_file_with_cluster },
	{"native_delete_file_with_filename", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I",(void*)native_delete_file_with_filename },
	{"native_init_temp","()I", (void*)native_fat_init_temp}
};

int register_android_recover_fat(JNIEnv* env)
{
	return jniRegisterNativeMethods(env, "edu/handong/ghost/ctrlz/nativeclass/recover/NativeFatRecover",
									sMethods, NELEM(sMethods));
}

static jint native_get_pathcount(JNIEnv* env, jobject obj, 
								jstring device_path, jstring top_path, jstring mount_dir)
{
	char* _device_path = NULL;
	char* _top_path = NULL;
	char* _mount_dir = NULL;
	const char* sz;
	int count = -1;

	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, top_path, 0);
	_top_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, top_path, sz);

	sz = (*env)->GetStringUTFChars(env, mount_dir, 0);
	_mount_dir = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_dir, sz);

	ec_neg1(count = get_pathcount(_top_path, _mount_dir));
//	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "scan path count: %ld", count);
	
	(void)free(_device_path);
	(void)free(_top_path);
	(void)free(_mount_dir);

	return count;

	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_top_path);
		(void)free(_mount_dir);
		EC_FLUSH("native_get_pathcount()");
		throwException(env, "java/io/IOException",errno);
		return -1;
	EC_CLEANUP_END
}

static jint native_recover_file_tmp(JNIEnv* env, jobject obj,
									jstring device_path, jstring file_name, jstring mount_dir,
									jint f_cluster, jlong file_size)
{
	char* _device_path = NULL;
	char* _file_name = NULL;
	char* _mount_dir = NULL;
	const char* sz;
	struct fat_boot_sector bsinfo;
	FATCLUSTER* c = NULL;
	CLUSTER_CHAIN* head = NULL;

	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, file_name,0);
	_file_name = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, file_name, sz);

	sz = (*env)->GetStringUTFChars(env, mount_dir, 0);
	_mount_dir = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_dir, sz);

	ec_neg1( get_fat_bootsec(_device_path, &bsinfo) );
	ec_null( c = open_fat_cluster(&bsinfo, _device_path) );
	ec_neg1( get_fat_cluster_chain(c, (cluster_t)f_cluster, (unsigned long)file_size, &head, JUMP) );
	ec_neg1( recover_file_tmp(&bsinfo, _device_path, _file_name, (unsigned long)file_size, head) );

	(void)free(_device_path);
	(void)free(_file_name);
	(void)free(_mount_dir);
	(void)free_fat_cluster_chain(&head);
	(void)close_fat_cluster(c);
	return 0;

	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_file_name);
		(void)free(_mount_dir);
		if( c != NULL )
			(void)close_fat_cluster(c);
		if( head != NULL )
			(void)free_fat_cluster_chain(&head);
		EC_FLUSH("native_recover_file_tmp()");
		throwException(env, "java/io/IOException",errno);
		return -1;
	EC_CLEANUP_END
}

static jint native_recover_file_mv(JNIEnv* env, jobject obj,
								   jstring file_name, jstring mount_point)
{
	char *src = NULL, *dst = NULL;
	char* _file_name = NULL;
	char* _mount_point = NULL;
	const char* sz;

	sz = (*env)->GetStringUTFChars(env, file_name, 0);
	_file_name = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, file_name, sz);

	sz = (*env)->GetStringUTFChars(env, mount_point,0);
	_mount_point = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_point, sz);

	if( (src=(char*)malloc(strlen(PATH_TMPRECOVER)+strlen(_file_name)+1)) == NULL)
		EC_FAIL
	if( (dst=(char*)malloc(strlen(_mount_point)+ strlen(DIRECTORY_RECOVER)+strlen(_file_name)+1)) == NULL)
		EC_FAIL

	memset(src,0,strlen(PATH_TMPRECOVER)+strlen(_file_name)+1);
	strncpy(src, PATH_TMPRECOVER, strlen(PATH_TMPRECOVER));
	strcat(src, _file_name);

	memset(dst,0,strlen(_mount_point)+strlen(DIRECTORY_RECOVER)+strlen(_file_name)+1);
	strncpy(dst, _mount_point, strlen(_mount_point));
	strcat(dst, DIRECTORY_RECOVER);
	strcat(dst, _file_name);

	ec_neg1( recover_file_mv(src, dst) );

	(void)free(src);
	(void)free(dst);
	(void)free(_file_name);
	(void)free(_mount_point);
	return 0;

	EC_CLEANUP_BGN
		(void)free(src);
		(void)free(dst);
		(void)free(_file_name);
		(void)free(_mount_point);
		EC_FLUSH("native_recover_file_mv()");
		throwException(env, "java/io/IOException", errno);
		return -1;
	EC_CLEANUP_END
}

static jint native_recover_file(JNIEnv* env, jobject obj, 
								jstring device_path, jstring file_name, jstring mount_dir,
								jint f_cluster, jlong file_size)
{
	char* _device_path = NULL;
	char* _file_name = NULL;
	char* _mount_dir = NULL;
	char* src = NULL, *dst = NULL;
	const char* sz;
	struct fat_boot_sector bsinfo;
	FATCLUSTER* c = NULL;
	CLUSTER_CHAIN* head = NULL;

	/*Convert string to C style*/
	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, file_name, 0);
	_file_name = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, file_name, sz);

	sz = (*env)->GetStringUTFChars(env, mount_dir, 0);
	_mount_dir = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_dir, sz);

	if( (src=(char*)malloc(strlen(PATH_TMPRECOVER)+strlen(_file_name)+1)) == NULL)
		EC_FAIL
	if( (dst=(char*)malloc(strlen(_mount_dir)+strlen(DIRECTORY_RECOVER)+strlen(_file_name)+1)) == NULL)
		EC_FAIL
	
	memset(src,0,strlen(PATH_TMPRECOVER)+strlen(_file_name)+1);
	strncpy(src, PATH_TMPRECOVER, strlen(PATH_TMPRECOVER));
	strcat(src, _file_name);

	memset(dst,0,strlen(_mount_dir)+strlen(DIRECTORY_RECOVER)+strlen(_file_name)+1);
	strncpy(dst, _mount_dir, strlen(_mount_dir));
	strcat(dst, DIRECTORY_RECOVER);
	strcat(dst, _file_name);

	ec_neg1( get_fat_bootsec(_device_path, &bsinfo) );
	ec_null( c = open_fat_cluster(&bsinfo, _device_path) );
	ec_neg1( get_fat_cluster_chain(c, (cluster_t)f_cluster, (unsigned long)file_size, &head, JUMP) );
	ec_neg1( recover_file_tmp(&bsinfo, _device_path, _file_name, (unsigned long)file_size, head) );
	ec_neg1( recover_file_mv(src, dst) );

	(void)free(_device_path);
	(void)free(_file_name);
	(void)free(_mount_dir);
	(void)free(src);
	(void)free(dst);
	(void)free_fat_cluster_chain(&head);
	(void)close_fat_cluster(c);
	return 0;

	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_file_name);
		(void)free(_mount_dir);
		(void)free(src);
		(void)free(dst);
		if( c != NULL )
			(void)close_fat_cluster(c);
		if( head != NULL )
			(void)free_fat_cluster_chain(&head);
		EC_FLUSH("native_recover_file()");
		throwException(env, "java/io/IOException", errno);
		return -1;
	EC_CLEANUP_END
}

static jobject native_scan_path(JNIEnv* env, jobject obj, jobject asyncobj,
								jstring device_path, jstring top_path, jstring mount_dir)
{
	long count = 0;
	RECOVER_LIST_SCAN_HEADER* header = NULL;
	struct DEV_INFO dev;
	jobject listobj = NULL;
	char* _device_path = NULL;
	char* _top_path = NULL;
	char* _mount_dir = NULL;
	const char* sz;

	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, top_path, 0);
	_top_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, top_path, sz);

	sz = (*env)->GetStringUTFChars(env, mount_dir, 0);
	_mount_dir = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_dir, sz);
	
	ec_neg1(get_device_info(_device_path, &dev));
	ec_null(header = create_scan_header(_top_path));
	ec_neg1(fatscan_path(env, asyncobj, &dev, header));
//	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "scan Done!");
//	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "real countpath: %d!!", header->current_location);
	ec_null(listobj = convertRecoverList2ArrayList(env, header->head));
	
	// debugging
	// print_recover_list(header->head);

	(void)free(_device_path);
	(void)free(_top_path);
	(void)free(_mount_dir);
	(void)free_recover_list(&(header->head));
	(void)free_scan_header(header);

	return listobj;

	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_top_path);
		(void)free(_mount_dir);
		(void)free_recover_list(&(header->head));
		(void)free_scan_header(header);
		EC_FLUSH("native_scan_path()");
		throwException(env, "java/io/IOException", errno);
		return NULL;
	EC_CLEANUP_END
}

static jobject native_scan_detail(JNIEnv* env, jobject obj, jobject asyncobj, jstring device_path)
{
	const char* sz;
	char* _device_path = NULL;
	FAT_DATA* fdata = NULL;
	RECOVER_LIST* head = NULL;
	jobject listobj = NULL;
	int nread = 0;
	int count = 0;

	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	ec_null(fdata = fat_opendata(_device_path));
	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "\n\n[[Start Scanning]] cluster range: %d ~ %d\n", 2, fdata->e_cluster);
	
	do{
		count++;
		ec_neg1(nread);

		if(count == 10000){
		/*	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "\rScanning Cluster ... %ld/%ld sector: %lld (%3d%%)", 
							(unsigned long)fdata->c_cluster, 
							(unsigned long)fdata->e_cluster, fdata->c_sector,
							((fdata->c_cluster)*100)/fdata->e_cluster);
		*/
			count = 0;
		}
		
	}while((nread = fat_data_cluster_scan(fdata, &head)) != 0);

	ec_null(listobj = convertRecoverList2ArrayList(env, head));

	fat_closedata(fdata);
	free_recover_list(&head);

	return listobj;

	EC_CLEANUP_BGN
		if(fdata != NULL)
			fat_closedata(fdata);
		if(head != NULL)
			free_recover_list(&head);
		EC_FLUSH("detail scan");
		throwException(env, "java/io/IOException", errno);
		return NULL;
	EC_CLEANUP_END

}

static jint native_delete_file_with_cluster(JNIEnv* env, jobject obj,
											jstring device_path, jstring mount_point,
											jbyteArray dir_path, jbyteArray file_name, jint f_cluster, jlong size)
{
	char* _device_path = NULL;
	char* _mount_point = NULL;
	char* _dir_path = NULL;
	char* _file_name = NULL;
	char* _file_path = NULL;
	int s_length = 0;
	const char* sz;
	char* _b_sz;
	jboolean isCopy;
	struct fat_boot_sector bsinfo;
	FATCLUSTER* c = NULL;
	CLUSTER_CHAIN* chain_head = NULL;

	/*Convert string to C style*/
	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, mount_point, 0);
	_mount_point = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_point, sz);

	_b_sz = (*env)->GetByteArrayElements(env, dir_path, &isCopy);
	_dir_path = strdup(_b_sz);
	if(isCopy)
		(*env)->ReleaseByteArrayElements(env, dir_path, _b_sz, JNI_ABORT);

	_b_sz = (*env)->GetByteArrayElements(env, file_name, &isCopy);
	_file_name = strdup(_b_sz);
	if(isCopy)
		(*env)->ReleaseByteArrayElements(env, file_name, _b_sz, JNI_ABORT);

	s_length = strlen(_dir_path) + strlen(_file_name) + 1;
	ec_null( _file_path = (char*)malloc(s_length) );
	memset(_file_path,0,s_length);
	strncpy(_file_path, _dir_path, strlen(_dir_path));
	strncat(_file_path, _file_name, strlen(_file_name));

	ec_neg1( get_fat_bootsec(_device_path, &bsinfo));
	ec_null( c = open_fat_cluster(&bsinfo, _device_path));
	ec_neg1( get_fat_cluster_chain(c, (cluster_t)f_cluster, (unsigned long)size, &chain_head, JUMP));
	ec_neg1( delete_file(&bsinfo, _device_path, _file_path, _mount_point, chain_head, JUMP));

	(void)free(_device_path);
	(void)free(_mount_point);
	(void)free(_dir_path);
	(void)free(_file_name);
	(void)free(_file_path);
	(void)close_fat_cluster(c);
	(void)free_fat_cluster_chain(&chain_head);
	return 0;
	
	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_mount_point);
		(void)free(_dir_path);
		(void)free(_file_name);
		(void)free(_file_path);
		if(c != NULL)
			(void)close_fat_cluster(c);
		if(chain_head != NULL)
			(void)free_fat_cluster_chain(&chain_head);
		EC_FLUSH("native_delete_file_with_cluster()");
		throwException(env, "java/io/IOException", errno);
		return -1;
	EC_CLEANUP_END
}

static jint native_delete_file_with_filename(JNIEnv* env, jobject obj,
											 jstring device_path, jstring mount_point,
											 jstring dir_path, jstring file_name)
{
	char* _device_path = NULL;
	char* _mount_point = NULL;
	char* _dir_path = NULL;
	char* _file_name = NULL;
	const char* sz;
	char* _file_path = NULL;
	struct fat_boot_sector bsinfo;
	FATCLUSTER* c = NULL;
	CLUSTER_CHAIN* chain_head = NULL;
	FATDIR *fdir = NULL;
	fat_dirent_t result;
	int read_result;
	bool isfind = false;

	/*Convert string to C style*/
	sz = (*env)->GetStringUTFChars(env, device_path, 0);
	_device_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, device_path, sz);

	sz = (*env)->GetStringUTFChars(env, mount_point, 0);
	_mount_point = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, mount_point, sz);

	sz = (*env)->GetStringUTFChars(env, dir_path, 0);
	_dir_path = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, dir_path, sz);

	sz = (*env)->GetStringUTFChars(env, file_name, 0);
	_file_name = strdup(sz);
	(*env)->ReleaseStringUTFChars(env, file_name, sz);

	ec_null( _file_path = (char*)malloc(strlen(_dir_path)+strlen(_file_name)+1) );
	memset(_file_path,0,strlen(_dir_path)+strlen(_file_name)+1);
	strncpy(_file_path, _dir_path, strlen(_dir_path));
	strcat(_file_path, _file_name);

	ec_neg1( get_fat_bootsec(_device_path, &bsinfo) );
	ec_null( fdir = fat_opendir(_device_path, _dir_path, _mount_point, &bsinfo) );
	
	while( (read_result = fat_readdir(fdir, &result, A_REG)) != 0 ){
		ec_neg1(read_result);
		if( strcasecmp(result.name, _file_name) == 0){
			isfind = true;
			break;
		}
	}

	if(!isfind)
		EC_FAIL
	
	ec_null( c = open_fat_cluster(&bsinfo, _device_path));
	ec_neg1( get_fat_cluster_chain(c, CAL_FIRST_CLUSTER(result.dirent.first_cluster_hi,
														result.dirent.first_cluster_lo),
									(unsigned long)result.dirent.size,
									&chain_head, ACTIVE));
	ec_neg1(delete_file(&bsinfo, _device_path, _file_path, _mount_point, chain_head, ACTIVE));														

	(void)free(_device_path);
	(void)free(_mount_point);
	(void)free(_dir_path);
	(void)free(_file_name);
	(void)free(_file_path);
	(void)fat_closedir(fdir);
	(void)close_fat_cluster(c);
	(void)free_fat_cluster_chain(&chain_head);
	return 0;

	EC_CLEANUP_BGN
		(void)free(_device_path);
		(void)free(_mount_point);
		(void)free(_dir_path);
		(void)free(file_name);
		(void)free(_file_path);
		if(fdir != NULL)
			(void)fat_closedir(fdir);
		if(c != NULL)
			(void)close_fat_cluster(c);
		if(chain_head != NULL)
			(void)free_fat_cluster_chain(&chain_head);
		EC_FLUSH("native_delete_file_with_filename()");
		throwException(env, "java/io/IOException", errno);
		return -1;
	EC_CLEANUP_END
}

static jint native_fat_init_temp(JNIEnv* env, jobject obj)
{	
	if(fat_init_temp() == -1){
		throwException(env, "java/io/IOException", errno);
		return -1;
	}
	return 0;
}

static jobject convertRecoverList2ArrayList(JNIEnv* env, RECOVER_LIST* head)
{
	RECOVER_LIST* p = NULL;

	jclass clazz = (*env)->FindClass(env, "java/util/ArrayList");
	jclass targetClass = (*env)->FindClass(env, "edu/handong/ghost/ctrlz/recovery/obj/FatRecoverFile");
	
	jobject listobj = (*env)->NewObject(env, clazz, (*env)->GetMethodID(env, clazz, "<init>", "()V"));
	jmethodID mid;
	
	for(p=head; p; p=p->next){
		mid = (*env)->GetMethodID(env, targetClass, "<init>", "()V");
		jobject newObject = (*env)->NewObject(env, targetClass, mid);
		jstring filename = (*env)->NewStringUTF(env,p->filename);
		jstring extender = (*env)->NewStringUTF(env, p->extender);		
		jstring dirpath = (*env)->NewStringUTF(env,p->dir_path);
		jbyteArray filename_b =  convertChars2ByteArray(env, p->filename);
		jbyteArray dirpath_b = convertChars2ByteArray(env, p->dir_path);
		jint first_cluster = p->first_cluster;
		jlong size = p->size;
		jint expected_possibility = p->expected_possibility;
		
		mid = (*env)->GetMethodID(env, targetClass, "setFileName", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, filename);
		mid = (*env)->GetMethodID(env, targetClass, "setFileName_byte", "([B)V");
		(*env)->CallVoidMethod(env, newObject, mid, filename_b);
		mid = (*env)->GetMethodID(env, targetClass, "setExtender", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, extender);
		mid = (*env)->GetMethodID(env, targetClass, "setDirPath", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, dirpath);
		mid = (*env)->GetMethodID(env, targetClass, "setDirPath_byte", "([B)V");
		(*env)->CallVoidMethod(env, newObject, mid, dirpath_b);
		mid = (*env)->GetMethodID(env, targetClass, "setSize", "(J)V");
		(*env)->CallVoidMethod(env, newObject, mid, size);
		mid = (*env)->GetMethodID(env, targetClass, "setExpectedPossibility","(I)V");
		(*env)->CallVoidMethod(env, newObject, mid, expected_possibility);
		mid = (*env)->GetMethodID(env, targetClass, "setFirstCluster", "(I)V");
		(*env)->CallVoidMethod(env, newObject, mid, first_cluster);

		mid = (*env)->GetMethodID(env,clazz, "add", "(Ljava/lang/Object;)Z");
		(*env)->CallBooleanMethod(env,listobj, mid, newObject);

		(*env)->DeleteLocalRef(env, filename);
		(*env)->DeleteLocalRef(env, extender);
		(*env)->DeleteLocalRef(env, dirpath);
		(*env)->DeleteLocalRef(env, filename_b);
		(*env)->DeleteLocalRef(env, dirpath_b);
		(*env)->DeleteLocalRef(env, newObject);
	}
	
	return listobj;
}
/*
static char* replaceUnUTF8Chars(char* str, int length)
{
	int i;
	for(i=0; i<length; i++){
		if(str[i] >= 0x80)
			str[i] = 'a';
	}
	return str;
}
*/