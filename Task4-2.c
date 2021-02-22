#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int fd;
    int result;
    size_t size;
    char str_res[14];
    char path[] = "text.fifo";

    (void)umask(0);
    if ((fd = open(path, O_RDONLY)) < 0) {
        printf("Can not open fifo for reading\n");
        exit(-1);
    }
    size = read(fd, str_res, 14);
    if (size < 0) {
        printf("Can not read string from fifo\n");
        exit(-1);
    }
    printf("Result: %s\n", str_res);
    if (close(fd) < 0) {
        printf("Can not close fifo\n"); exit(-1);
    }
    return 0;
}