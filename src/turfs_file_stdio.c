#include <stdio.h>
#include <stdlib.h>

#include <turfs.h>

struct turfs_fp {
    FILE *fp;
};

TURFSAPI turfs_file_open(const char *file, const char *mode, turfs_fp *fp)
{
    turfs_ret err;
    char *rpath = NULL;
    size_t rpath_len = 0;
    if (file == NULL || mode == NULL || fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if ((err = turfs_silph_scope(file, &rpath, &rpath_len)) != TURFS_RET_OK) {
        return err;
    }
    *fp = malloc(sizeof(struct turfs_fp));
    if (*fp == NULL) {
        free(rpath);
        return TURFS_RET_MEMORY_ERROR;
    }
    (*fp)->fp = fopen(rpath, mode);
    if ((*fp)->fp == NULL) {
        free(*fp);
        *fp = NULL;
        free(rpath);
        return TURFS_RET_NOT_FOUND;
    }
    free(rpath);
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_read(turfs_fp fp, char *buffer, size_t count, size_t *bytes_read)
{
    size_t read_bytes = 0;
    if (fp == NULL || fp->fp == NULL || buffer == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    read_bytes = fread(buffer, sizeof(char), count, fp->fp);
    if (bytes_read != NULL) {
        *bytes_read = read_bytes;
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_seek(turfs_fp fp, off_t offset, int whence)
{
    if (fp == NULL || fp->fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fseeko(fp->fp, offset, whence)) {
        return TURFS_RET_UNKNOWN;
    }
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_length(turfs_fp fp, size_t *len)
{
    if (fp == NULL || fp->fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    fseek(fp->fp, 0, SEEK_END);
    if (len != NULL) {
        *len = (size_t) ftell(fp->fp);
    }
    rewind(fp->fp);
    return TURFS_RET_OK;
}

TURFSAPI turfs_file_tell(turfs_fp fp, long int *pos)
{
    if (fp == NULL || fp->fp == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    if (pos != NULL) {
        *pos = ftell(fp->fp);
    }
    return (*pos > -1L) ? TURFS_RET_OK : TURFS_RET_UNKNOWN;
}

TURFSAPI turfs_file_close(turfs_fp *fp)
{
    if (fp == NULL || *fp == NULL || (*fp)->fp)  {
        return TURFS_RET_INVALID_ARGS;
    }
    if (fclose((*fp)->fp)) {
        return TURFS_RET_UNKNOWN;
    }
    free(*fp);
    *fp = NULL;
    return TURFS_RET_OK;
}
