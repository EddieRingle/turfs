#include <stddef.h>
#include <string.h>

#include <turfs.h>

#include "internal.h"

/* Fixed virtual paths for now */
#define ASSETS_PATH_STR     "/assets"
#define USERDATA_PATH_STR   "/userdata"

TURFSAPI turfs_virtual_path(turfs_path_type type, char **path, size_t *len)
{
    if (!g_initialized) {
        return TURFS_RET_NOT_INITIALIZED;
    }
    switch (type) {
        case TURFS_PATH_ASSETS:
            if (path != NULL) {
                *path = turfs__strdup(ASSETS_PATH_STR);
            }
            if (len != NULL) {
                *len = strlen(ASSETS_PATH_STR);
            }
            break;
        case TURFS_PATH_USERDATA:
            if (path != NULL) {
                *path = turfs__strdup(USERDATA_PATH_STR);
            }
            if (len != NULL) {
                *len = strlen(USERDATA_PATH_STR);
            }
            break;
    }
    return TURFS_RET_OK;
}
