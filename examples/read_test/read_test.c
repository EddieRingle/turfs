#include <stdlib.h>
#include <stdio.h>

#include <turfs.h>

int main(int argc, char **argv)
{
    char buf[2048] = {0};
    int ret;
    turfs_fp fp;
    turfs_init("turfs", "read_test", NULL);
    ret = turfs_file_open("/assets/read_this.txt", "rb", &fp);
    if (ret) {
        fprintf(stderr, "read_test: File open failed.\n");
    } else {
        ret = turfs_file_read(fp, buf, 2048, NULL);
        if (ret) {
            fprintf(stderr, "read_test: File read failed.\n");
        } else {
            printf("Read success, file contents:\n%s\n", buf);
        }
        turfs_file_close(&fp);
    }
    turfs_quit();
}