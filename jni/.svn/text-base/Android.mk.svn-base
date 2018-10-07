# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# Generate get Device list library
# Generate recover library
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_PLATFORM := android-8

LOCAL_MODULE    := recover

LOCAL_CFLAGS += -D_FILE_OFFSET_BITS=64

LOCAL_C_INCLUDES := \
	$(JNI_H_INCLUDE) \
	$(LOCAL_PATH)/include\
	$(LOCAL_PATH)/common

LOCAL_SRC_FILES := \
		src/fat_bootsec.c\
		src/fat_cluster.c\
		src/fat_delete.c\
		src/fat_dir.c\
		src/fat_recover.c\
		src/fat_dscan.c\
		src/get_device_list.c\
		common/_errno.c\
		common/ec.c\
		common/macrostr.c\
		common/options.c\
		common/pthreadstub.c\
		common/syserr.c\
		common/time.c\
		jnihelp.c\
		recover_onload.c\
		edu_handong_ghost_ctrlz_nativeclass_NativeFatRecover.c		

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)

# Generate get Device list library


include $(CLEAR_VARS)

LOCAL_MODULE    := getdevice

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include\
	$(LOCAL_PATH)/common

LOCAL_CFLAGS += -D_FILE_OFFSET_BITS=64

LOCAL_SRC_FILES := \
		src/get_device_list.c\
		common/_errno.c\
		common/ec.c\
		common/macrostr.c\
		common/options.c\
		common/pthreadstub.c\
		common/syserr.c\
		common/time.c\
		jnihelp.c\
		edu_handong_ghost_ctrlz_nativeclass_NativeDeviceList.c

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
