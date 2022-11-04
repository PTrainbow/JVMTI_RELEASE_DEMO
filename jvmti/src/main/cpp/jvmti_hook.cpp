//
// learn from:  https://juejin.cn/post/6942782366993612813
// use XDL not inlineHook
//
// Created by zhaozheyu on 2022/11/03.
//

#include <jni.h>
#include <string>
#include "xdl.h"
#include "jvmti.h"
#include "logger.h"

static jvmtiEnv *sJVMTIEnv = nullptr;
const char *libart = nullptr;

extern "C"
JNIEXPORT jint JNICALL
Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
    jint result = vm->GetEnv((void **) &sJVMTIEnv, JVMTI_VERSION_1_2);
    if (result != JNI_OK) {
        LOGI("attach jvmti agent failed!");
        return result;
    }
    return JNI_OK;
}

int dl_iterate_callback(dl_phdr_info *info, size_t size, void *data) {
    // find libart
    if (strstr(info->dlpi_name, "libart.so")) {
        libart = info->dlpi_name;
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rain_sad_jvmti_JvmtiManager_nInitInReleaseMode(JNIEnv *env, jobject thiz) {
    xdl_iterate_phdr(dl_iterate_callback, nullptr, XDL_FULL_PATHNAME);
    // open libart.so
    void *dl = xdl_open(libart, XDL_ALWAYS_FORCE_LOAD);
    if (dl != nullptr) {
        LOGE("open libart.so successful!");
        // find SetJdwpAllowed function pointer
        auto (*SetJdwpAllowed)(bool) = reinterpret_cast<void (*)(bool)>(
                xdl_sym(dl, "_ZN3art3Dbg14SetJdwpAllowedEb", nullptr));
        if (SetJdwpAllowed != nullptr) {
            SetJdwpAllowed(true);
            LOGE("SetJdwpAllowed successful!");
        } else {
            LOGE("SetJdwpAllowed failed!");
        }
        // find setJavaDebuggable function pointer
        auto (*setJavaDebuggable)(void *, bool) = reinterpret_cast<void (*)(void *, bool)>(
                xdl_sym(dl, "_ZN3art7Runtime17SetJavaDebuggableEb", nullptr));
        // get runtime parameter(java Runtime??)
        void **instance_ = static_cast<void **>(
                xdl_sym(dl, "_ZN3art7Runtime9instance_E", nullptr));
        if (setJavaDebuggable != nullptr) {
            setJavaDebuggable(*instance_, true);
            LOGE("setJavaDebuggable successful!");
        } else {
            LOGE("setJavaDebuggable failed!");
        }
        xdl_close(dl);
    } else {
        LOGE("open libart.so failed!");
    }
    // that's ok!
}