#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int fd;
    int res;
    size_t size;
    char str_res[14];
    char path[] = "text.fifo"; 
    (void)umask(0);
    if (mknod(path, S_IFIFO | 0666, 0) < 0) {
        printf("file is not working\n");
        exit(-1);
    }
    if ((fd = open(path, O_RDONLY)) < 0) {
        printf("Can not open fifo for reading\n");
        exit(-1);
    }
    size = read(fd, str_res, 14);
    if (size < 0) {
        printf("Can not reading\n");
        exit(-1);
    }
    printf("Result: %s\n", str_res);
    close(fd);
    return 0;
}
