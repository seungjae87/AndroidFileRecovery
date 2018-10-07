#ifndef _NATIVEHELPER_JNIHELP_H
#define _NATIVEHELPER_JNIHELP_H
#include "jni.h"
#include "defs.h"
#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

extern int jniRegisterNativeMethods(JNIEnv* env, const char* classPath, 
									const JNINativeMethod* sMethods, int numMethods);
jint throwException(JNIEnv* env, const char* exceptionClassName, int errno_arg );
jbyteArray convertChars2ByteArray(JNIEnv* env, char* c);
jobject toInteger(JNIEnv* env, jint num);
#endif