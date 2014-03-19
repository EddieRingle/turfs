#include <stdlib.h>
#include <stdio.h>

#include <turfs.h>

int main(int argc, char **argv)
{
    char *test;
    turfs_fp fp;
    int ret = 0;
    turfs_init("turfsdev", "turfs");
    ret = turfs_resolve_path(TURFS_PATH_ASSETS, &test, NULL);
    printf("Assets (%d): %s\n", ret, test);
    free(test);
    ret = turfs_resolve_path(TURFS_PATH_USERDATA, &test, NULL);
    printf("Userdata (%d): %s\n", ret, test);
    free(test);
    ret = turfs_silph_scope("/userdata/subdir/test1.sav", &test, 0);
    if (ret) {
        printf("SILPH1 FAILED!\n");
    } else {
        printf("SILPH1 = %s\n", test);
    }
    free(test);
    ret = turfs_silph_scope("/assets/subdir/test1.txt", &test, 0);
    if (ret) {
        printf("SILPH2 FAILED!\n");
    } else {
        printf("SILPH2 = %s\n", test);
    }
    free(test);
    ret = turfs_silph_scope("/notreal/rofl.copter", &test, 0);
    if (ret) {
        printf("SILPH3 FAILED! (and it should! :D)\n");
    } else {
        printf("SILPH3 = %s\n", test);
        free(test);
    }
    ret = turfs_file_open("/assets/test.txt", "r", &fp);
    if (ret) {
        printf("FILE OPEN FAILED\n");
    } else {
        printf("FILE OPEN SUCCEEDED\n");
        turfs_file_close(&fp);
    }
}