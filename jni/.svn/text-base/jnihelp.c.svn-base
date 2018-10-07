
#include "jnihelp.h"
#include <stdio.h>

int
jniRegisterNativeMethods(JNIEnv* env, const char* className, 
									const JNINativeMethod* sMethods, int numMethods)
{
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if(clazz == NULL){
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "Native registration unable to find class '%s'", className);
		return JNI_FALSE;
	}
	if((*env)->RegisterNatives(env, clazz, sMethods, numMethods) < 0){
		__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "RegisterNatives failed for '%s'", className);
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

jint
throwException(JNIEnv* env, const char* exceptionClassName, int errno_arg )
{
	char *errmsg;
	char buf[200];

	errmsg = (errno_arg < 1000)? strerror(errno_arg) : strerror_custom(errno_arg) ;
	snprintf(buf, sizeof(buf), "\t\t*** %s (%d: \"%s\") ***",
	get_macrostr("errno", errno_arg, NULL), errno_arg,
	errmsg != NULL ? errmsg : "no message string");

	return (*env)->ThrowNew(env, (*env)->FindClass(env, exceptionClassName), buf);
}

jbyteArray
convertChars2ByteArray(JNIEnv* env, char* c)
{
	jbyte* buffer = c;
	jbyteArray jb = (*env)->NewByteArray(env, strlen(c)+1);
	(*env)->SetByteArrayRegion(env, jb, 0, strlen(c)+1, buffer);
	return jb;
}

jobject
toInteger(JNIEnv* env, jint num)
{
	static jclass integerClass;
	static jmethodID valueOfMethod;

	if( integerClass == NULL){
		if( (integerClass = (*env)->FindClass(env, "java/lang/Integer"))==NULL)
			return NULL;
	}
	
	if(valueOfMethod == NULL){
		valueOfMethod = (*env)->GetStaticMethodID(env, integerClass, "valueOf",
			"(I)Ljava/lang/Integer;");
		if(valueOfMethod == NULL)
			return NULL;
	}

	return (*env)->CallStaticObjectMethod(env, integerClass, valueOfMethod, num);
}