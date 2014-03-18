#ifndef __included_turfs_h
#define __included_turfs_h

/* Needed for size_t and off_t */
#include <sys/types.h>

typedef enum {
    /*
     * Path to application (read-only) assets,
     * typically relative to the executing
     * binary's directory.
     *
     * Default virtual path: /assets
     */
    TURFS_PATH_ASSETS,

    /*
     * Path to data stored by the application
     * after it's installed, like user saves
     * and preferences.
     *
     * Default virtual path: /userdata
     */
    TURFS_PATH_USERDATA
} turfs_path_type;

/*
 * turfs return codes
 */
typedef enum {
    TURFS_RET_OK = 0,
    TURFS_RET_INVALID_ARGS,
    TURFS_RET_NOT_INITIALIZED,
    TURFS_RET_NOT_FOUND,
    TURFS_RET_UNKNOWN
} turfs_ret;

/**
 * turfs File pointer type
 */
typedef void* turfs_fp;

#define TURFSAPI turfs_ret

/**
 * Initialize turfs
 *
 * This must be called prior to doing anything else with
 * this library.
 *
 * @param[in]   author  Author of the application, no special characters
 * @param[in]   name    Name of the application, no special characters
 *
 * @returns TURFS_RET_OK if initialization succeeded.
 */
TURFSAPI turfs_init(const char *author, const char *name);

/**
 * De-initialize turfs
 *
 * This should be called when you are finished using
 * this library in your application.
 *
 * @returns TURFS_RET_OK if turfs quit successfully, or
 * TURFS_RET_NOT_INITIALIZED if turfs_init had not yet been
 * called.
 */
TURFSAPI turfs_quit(void);

/**
 * Resolves a turfs_path_type to a virtual path
 *
 * @param[in]   type    Path type to resolve virtually
 * @param[out]  path    Location where path will be stored
 * @param[out]  len     Location where path length will be stored
 */
TURFSAPI turfs_virtual_path(turfs_path_type type, char **path, size_t *len);

/**
 * Resolves a turfs_path_type to its real system path
 *
 * @param[in]   type    Path type to resolve
 * @param[out]  path    Location where path will be stored
 * @param[out]  len     Location where path length will be stored
 */
TURFSAPI turfs_resolve_path(turfs_path_type type, char **path, size_t *len);

/**
 * Opens a file given an absolute virtual path.
 *
 * @param[in]   file    Absolute virtual path to file
 * @param[in]   mode    C string containing file access mode
 * @param[out]  fp      File pointer
 *
 * @returns TURFS_RET_OK if the file was opened successfully,
 * TURFS_RET_NOT_INITIALIZED if this function is called prior
 * to turfs_init, TURFS_RET_NOT_FOUND if the specified file
 * does not exist, or TURFS_RET_UNKNOWN if the file could not
 * be opened for some unspecified reason.
 */
TURFSAPI turfs_file_open(const char *file, const char *mode, turfs_fp *fp);

TURFSAPI turfs_file_read(turfs_fp fp, char *buffer, size_t count, size_t *bytes_read);

TURFSAPI turfs_file_seek(turfs_fp fp, off_t offset, int whence);

TURFSAPI turfs_file_length(turfs_fp fp, size_t *len);

TURFSAPI turfs_file_tell(turfs_fp fp, long int *pos);

TURFSAPI turfs_file_close(turfs_fp *fp);

#endif /* __included_turfs_h */
