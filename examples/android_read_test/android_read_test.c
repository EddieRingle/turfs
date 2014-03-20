#include <stdlib.h>
#include <stdio.h>

#include <android/log.h>

#include <turfs.h>

#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "turfs::android_read_test", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "turfs::android_read_test", __VA_ARGS__))

void android_main(struct android_app *app)
{
    char buf[2048] = {0};
    int ret;
    turfs_fp fp;
    turfs_init("turfs", "read_test", app->activity);
    ret = turfs_file_open("/assets/read_this.txt", "rb", &fp);
    if (ret) {
        LOGE("read_test: File open failed.\n");
    } else {
        ret = turfs_file_read(fp, buf, 2048, NULL);
        if (ret) {
            LOGE("read_test: File read failed.\n");
        } else {
            LOGI("Read success, file contents:\n%s\n", buf);
        }
        LOGI("File contents: %s\n", buf);
        turfs_file_close(&fp);
    }
    turfs_quit();
}