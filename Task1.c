#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main()
{
    int fd;
    (void)umask(0);
    if ((fd = open("myfile", O_RDONLY)) < 0) {
        printf("File could not be opened\n");
        exit(-1);
    }
    char str[14];
    size_t size = read(fd, str, 14);
    if (size != 14) {
        printf("Can not write all symbols\n");
        exit(-1);
    }
    printf("%s\n", str);
    if (close(fd) < 0) {
        printf("File could not be closed\n");
    }
    return 0;
}