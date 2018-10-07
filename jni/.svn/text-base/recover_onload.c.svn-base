
#include "jni.h"
#include <android/log.h>
#include <stdio.h>
#define LOG_TAG "CtrlZ"

int register_android_recover_fat(JNIEnv* env);

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK){
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "GetEnv failed");
		return result;
	}
	if( env == NULL)
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Could not retrieve the env!");;

	register_android_recover_fat(env);

	return JNI_VERSION_1_4;
}
