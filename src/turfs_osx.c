#include <stddef.h>
#include <stdlib.h>

#include <turfs.h>

#include "internal.h"

extern char **NXArgv;

static char *s_author = NULL;
static char *s_name = NULL;

static char *s_assets_path = NULL;
static char *s_userdata_path = NULL;

int g_initialized = 0;

TURFSAPI turfs_init(const char *author, const char *name)
{
    if (author == NULL || name == NULL) {
        return TURFS_RET_INVALID_ARGS;
    }
    s_author = turfs__strdup(author);
    s_name = turfs__strdup(name);
    g_initialized = 1;
    return TURFS_RET_OK;
}

TURFSAPI turfs_quit(void)
{
    if (!g_initialized) {
        return TURFS_RET_NOT_INITIALIZED;
    }
    if (s_author != NULL) {
        free(s_author);
        s_author = NULL;
    }
    if (s_name != NULL) {
        free(s_name);
        s_name = NULL;
    }
    g_initialized = 0;
    return TURFS_RET_OK;
}
