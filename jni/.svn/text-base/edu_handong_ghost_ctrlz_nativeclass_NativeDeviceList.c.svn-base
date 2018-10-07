
#include "edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList.h"
#include "get_device.h"
#include "defs.h"
#include "jnihelp.h"

/*
 * Class:     edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList
 * Method:    getDeviceList
 * Signature: ()Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList_getDeviceList__
  (JNIEnv *env, jobject obj)
{
	DEVLIST *head = NULL;
	DEVLIST *p = NULL;

	jclass clazz = (*env)->FindClass(env, "java/util/ArrayList");
	jclass targetClass = (*env)->FindClass(env, "edu/handong/ghost/ctrlz/recovery/obj/Device");

	jobject listobj = (*env)->NewObject(env, clazz, (*env)->GetMethodID(env, clazz, "<init>", "()V"));
	jmethodID mid;

	if( get_device_list(&head) == -1 ){
		throwException(env, "java/io/IOException", errno);
		return NULL;
	}

	for( p=head; p; p=p->next){
		mid = (*env)->GetMethodID(env, targetClass, "<init>", "()V");
		jobject newObject = (*env)->NewObject(env, targetClass, mid);

		jstring devPath = (*env)->NewStringUTF(env, p->fs_dev);
		jstring mountDir = (*env)->NewStringUTF(env, p->mount_dir);
		jstring fsType = (*env)->NewStringUTF(env, p->fs_type);
		jstring mountOpts = (*env)->NewStringUTF(env, p->mount_opts);
		jlong totalSize = p->total_size;
		jlong freeSize = p->free_size;

		mid = (*env)->GetMethodID(env, targetClass, "setDevPath", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, devPath);
		mid = (*env)->GetMethodID(env, targetClass, "setMountDir", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, mountDir);
		mid = (*env)->GetMethodID(env, targetClass, "setFsType", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, fsType);
		mid = (*env)->GetMethodID(env, targetClass, "setMountOpts", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env, newObject, mid, mountOpts);
		mid = (*env)->GetMethodID(env, targetClass, "setTotalSize", "(J)V");
		(*env)->CallVoidMethod(env, newObject, mid, totalSize);
		mid = (*env)->GetMethodID(env, targetClass, "setFreeSize", "(J)V");
		(*env)->CallVoidMethod(env, newObject, mid, freeSize);

		mid = (*env)->GetMethodID(env, clazz, "add", "(Ljava/lang/Object;)Z");
		(*env)->CallBooleanMethod(env, listobj, mid, newObject);
	}

	free_device_list(&head);
	return listobj;
}

/*
 * Class:     edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList
 * Method:    getDeviceList
 * Signature: (Ljava/lang/String;)Ljava/util/ArrayList;
 */
JNIEXPORT jobject JNICALL Java_edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList_getDeviceList__Ljava_lang_String_2
  (JNIEnv *env, jobject obj, jstring type)
{
	char strBuff[256];
	DEVLIST *head = NULL;
	DEVLIST *p = NULL;

	jclass clazz = (*env)->FindClass(env, "java/util/ArrayList");
	jclass targetClass = (*env)->FindClass(env, "edu/handong/ghost/ctrlz/recovery/obj/Device");

	jobject listobj = (*env)->NewObject(env, clazz, (*env)->GetMethodID(env,clazz, "<init>", "()V"));
	jmethodID mid;

	const char *sz = (*env)->GetStringUTFChars(env, type, 0);
	strncpy(strBuff, sz, 255);
	(*env)->ReleaseStringUTFChars(env, type, sz);

	if( get_device_list_with_type(&head, strBuff) == -1){
		throwException(env, "java/io/IOException", errno);
		return NULL;
	}

	for( p=head; p; p=p->next){
		mid = (*env)->GetMethodID(env, targetClass, "<init>", "()V");
		jobject newObject = (*env)->NewObject(env, targetClass, mid);

		jstring devPath = (*env)->NewStringUTF(env,p->fs_dev);
		jstring mountDir = (*env)->NewStringUTF(env,p->mount_dir);
		jstring fsType = (*env)->NewStringUTF(env,p->fs_type);
		jstring mountOpts = (*env)->NewStringUTF(env,p->mount_opts);
		jlong totalSize = p->total_size;
		jlong freeSize = p->free_size;

		mid = (*env)->GetMethodID(env,targetClass, "setDevPath", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env,newObject, mid, devPath);
		mid = (*env)->GetMethodID(env,targetClass, "setMountDir", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env,newObject, mid, mountDir);
		mid = (*env)->GetMethodID(env,targetClass, "setFsType", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env,newObject, mid, fsType);
		mid = (*env)->GetMethodID(env,targetClass, "setMountOpts", "(Ljava/lang/String;)V");
		(*env)->CallVoidMethod(env,newObject, mid, mountOpts);
		mid = (*env)->GetMethodID(env,targetClass, "setTotalSize", "(J)V");
		(*env)->CallVoidMethod(env,newObject, mid, totalSize);
		mid = (*env)->GetMethodID(env,targetClass, "setFreeSize", "(J)V");
		(*env)->CallVoidMethod(env,newObject, mid, freeSize);

		mid = (*env)->GetMethodID(env,clazz, "add", "(Ljava/lang/Object;)Z");
		(*env)->CallBooleanMethod(env,listobj, mid, newObject);
	}

	free_device_list(&head);
	return listobj;
}
