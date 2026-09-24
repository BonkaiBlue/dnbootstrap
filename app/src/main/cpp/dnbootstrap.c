#include <jni.h>
#include <android/log.h>
#include <linux/limits.h>
#include <dlfcn.h>
#include <hostfxr.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "libnethost.h"
#include <android/looper.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __FILE_NAME__, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, __FILE_NAME__, __VA_ARGS__)

//
// Created by maks on 14.12.2025.
//

static void init_logger() {

    const char* home = getenv("HOME");
    if(home == NULL) {
        LOGE("Failed to set up logger: HOME is not set");
        return;
    }

    char logpath[PATH_MAX];
    snprintf(logpath, PATH_MAX, "%s", home);
    char* lastSlash = strrchr(logpath, '/');
    if(lastSlash != NULL) *lastSlash = '\0';
    strncat(logpath, "/log.txt", PATH_MAX - strlen(logpath) - 1);

    int logfd = open(logpath, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(logfd == -1) {
        LOGI("Failed to set up logger: %s", strerror(errno));
        return;
    }
    //pipe(pipefd);
    dup2(logfd, 1);
    dup2(logfd, 2);

    setvbuf(stdout, NULL, _IOLBF, 4096);
    setvbuf(stderr, NULL, _IONBF, 0);
}


static int dnb_find_hostfxr(const char* dotnet_root, char_t path_buf[PATH_MAX]) {
    struct get_hostfxr_parameters parameters = {
            .assembly_path = NULL,
            .dotnet_root = dotnet_root
    };
    parameters.size = sizeof(parameters);

    size_t path_size = sizeof(char_t[PATH_MAX]) / sizeof(char_t);

    int result = get_hostfxr_path(path_buf, &path_size, &parameters);

    return result;
}

static void dnb_run_v2(void* hostfxr_ptr, const char* dotnetRoot, const char* nativeLibDir) {
    hostfxr_main_startupinfo_fn main_startupinfo_fn = dlsym(hostfxr_ptr, "hostfxr_main_startupinfo");
    //setenv("MONO_ENV_OPTIONS", "--trace=T:Vintagestory.Client.NoObf.ShaderProgramFinal", true);
    //setenv("MONO_ENV_OPTIONS", "--interpreter", true);
    char eglPath[PATH_MAX];
    snprintf(eglPath, PATH_MAX, "%s/%s", nativeLibDir, "libEGL_angle.so");
    setenv("ANGLE_DEFAULT_PLATFORM", "gl", true);
    setenv("LTW_EMULATE_DEPTHCLEAR", "1", true);
    setenv("LIBGL_EGL", eglPath, true);
    char hostPath[PATH_MAX];
    snprintf(hostPath, PATH_MAX, "%s/%s", dotnetRoot, "dotnet");
    char appPath[PATH_MAX];
    snprintf(appPath, PATH_MAX, "%s/%s", dotnetRoot, "dotnet.dll");

    const char* argv[] = { hostPath, "Vintagestory.dll" };
    const int argc = 2;

    LOGI("dotnetRoot: %s appPath: %s", dotnetRoot, appPath);
    pthread_setname_np(pthread_self(), "dnb main thread");
    int rc = main_startupinfo_fn(argc, argv, hostPath, dotnetRoot, appPath);
    LOGI("hostfxr done: %x", rc);
}

__attribute((used)) void dnb_main(JavaVM *vm, jobject instance) {
    JNIEnv *env;
    (*vm)->AttachCurrentThread(vm, &env, NULL);
    jclass activityClass = (*env)->GetObjectClass(env, instance);
    jmethodID kickstartMthod = (*env)->GetMethodID(env, activityClass, "kickstart", "()V");
    (*env)->CallVoidMethod(env, instance, kickstartMthod);

}

JNIEXPORT void JNICALL
Java_git_artdeell_dnbootstrap_MainActivity_runDotnet(JNIEnv *env, jobject thiz,
                                                     jstring dotnetRoot, jstring vsDir, jstring nativeLibDir) {

    init_logger();

    char_t path_buf[PATH_MAX];
    const char* dotnet_root = (*env)->GetStringUTFChars(env, dotnetRoot, NULL);
    uint32_t result = dnb_find_hostfxr(dotnet_root, path_buf);

    if(result != 0) {
        LOGI("Cannot find hostfxr: %x", result);
    }

    void* hostfxr_ptr = dlopen(path_buf, RTLD_NOW);

    const char* vs_dir = (*env)->GetStringUTFChars(env, vsDir, NULL);
    const char* native_lib_dir = (*env)->GetStringUTFChars(env, nativeLibDir, NULL);
    chdir(vs_dir);
    (*env)->ReleaseStringUTFChars(env, vsDir, vs_dir);

    dnb_run_v2(hostfxr_ptr, dotnet_root, native_lib_dir);
    (*env)->ReleaseStringUTFChars(env, nativeLibDir, native_lib_dir);
}