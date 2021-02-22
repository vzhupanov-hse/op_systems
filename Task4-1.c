#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int fd;
    int res;
    size_t size;
    char str_res[14];
    char path[] = "text.fifo"; 
    (void)umask(0);
    if ((fd = open(path, O_WRONLY)) < 0) {
        printf("Can not open fifo for writing\n");
        exit(-1);
    }
    size = write(fd, "Hello world!", 14);
    if (size != 14) {
        printf("Can not write symbols to fifo\n");
        exit(-1);
    }
    close(fd);
    return 0;
}
