#include <stdio.h>
#include <stdlib.h>

#include <android/asset_manager.h>
#include <android/native_activity.h>
#include <android/log.h>

#include <turfs.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "turfs::android_read_test", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "turfs::android_read_test", __VA_ARGS__))

extern ANativeActivity *g_nativeActivity;

struct turfs_fp {
    AAsset *asset;
    FILE *fp;
    int eof;
};

TURFSAPI turfs_file_open(const char *file, const char *mode, turfs_fp *fp)
{
    turfs_ret err;
    turfs_path_type type;
    char *rpath = NULL;
    size_t rpath_len = 0;
    if (file == NULL || mode == NULL || fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if ((err = turfs_silph_scope(file, &rpath, &rpath_len, &type)) != TURFS_RET_OK) {
        return err;
    }
    *fp = malloc(sizeof(struct turfs_fp));
    if (*fp == NULL) {
        free(rpath);
        return TURFS_RET_MEMORY_ERROR;
    }
    switch (type) {
        case TURFS_PATH_ASSETS:
            (*fp)->fp = NULL;
            if (rpath[0] == '/') {
                (*fp)->asset = AAssetManager_open(g_nativeActivity->assetManager, &rpath[1], AASSET_MODE_STREAMING);
            } else {
                (*fp)->asset = AAssetManager_open(g_nativeActivity->assetManager, rpath, AASSET_MODE_STREAMING);
            }
            if ((*fp)->asset == NULL) {
                free(*fp);
                *fp = NULL;
                free(rpath);
                return TURFS_RET_NOT_FOUND;
            }
            break;
        case TURFS_PATH_USERDATA:
            (*fp)->fp = fopen(rpath, mode);
            (*fp)->asset = NULL;
            if ((*fp)->fp == NULL) {
                free(*fp);
                *fp = NULL;
                free(rpath);
                return TURFS_RET_NOT_FOUND;
            }
            break;
    }
    free(rpath);
    (*fp)->eof = 0;
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_read(turfs_fp fp, void *buffer, size_t count, size_t *bytes_read)
{
    size_t read_bytes = 0;
    if (fp == NULL || buffer == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fp->asset == NULL && fp->fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fp->asset != NULL) {
        read_bytes = AAsset_read(fp->asset, buffer, count);
        fp->eof = read_bytes == 0;
    } else if (fp->fp != NULL) {
        read_bytes = fread(buffer, sizeof(char), count, fp->fp);
        fp->eof = feof(fp->fp);
    }
    if (bytes_read != NULL) {
        *bytes_read = read_bytes;
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_seek(turfs_fp fp, off_t offset, int whence)
{
    if (fp == NULL || (fp->asset == NULL && fp->fp == NULL)) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fp->asset != NULL) {
        if (AAsset_seek(fp->asset, offset, whence) < 0) {
            return TURFS_RET_UNKNOWN;
        }
        fp->eof = 0;
    } else if (fp->fp != NULL) {
        if (fseek(fp->fp, offset, whence)) {
            return TURFS_RET_UNKNOWN;
        }
        fp->eof = feof(fp->fp);
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_length(turfs_fp fp, size_t *len)
{
    long cur;
    if (fp == NULL || (fp->asset == NULL && fp->fp == NULL)) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fp->asset != NULL) {
        if (len != NULL) {
            *len = AAsset_getLength(fp->asset);
        }
    } else if (fp->fp != NULL) {
        cur = ftell(fp->fp);
        fseek(fp->fp, 0, SEEK_END);
        if (len != NULL) {
            *len = (size_t) ftell(fp->fp);
        }
        fseek(fp->fp, cur, SEEK_SET);
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_tell(turfs_fp fp, long int *pos)
{
    if (fp == NULL || (fp->asset == NULL && fp->fp == NULL)) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fp->asset != NULL) {
        if (pos != NULL) {
            *pos = AAsset_getLength(fp->asset) - AAsset_getRemainingLength(fp->asset);
            return (*pos > -1L) ? TURFS_RET_OK : TURFS_RET_UNKNOWN;
        }
        return TURFS_RET_OK;
    } else if (fp->fp != NULL) {
        if (pos != NULL) {
            *pos = ftell(fp->fp);
            return (*pos > -1L) ? TURFS_RET_OK : TURFS_RET_UNKNOWN;
        }
        return TURFS_RET_OK;
    }
    return TURFS_RET_UNKNOWN;
}

TURFSAPI turfs_file_eof(turfs_fp fp)
{
    if (fp->eof) {
        return TURFS_RET_END_OF_FILE;
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_close(turfs_fp *fp)
{
    if (fp == NULL || *fp == NULL)  {
        return TURFS_RET_INVALID_ARGS;
    }
    if ((*fp)->asset == NULL && (*fp)->fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if ((*fp)->asset != NULL) {
        AAsset_close((*fp)->asset);
    } else if ((*fp)->fp != NULL) {
        if (fclose((*fp)->fp)) {
            return TURFS_RET_UNKNOWN;
        }
    }
    free(*fp);
    *fp = NULL;
    return TURFS_RET_OK;
}
