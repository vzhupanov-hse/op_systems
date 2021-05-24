#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void addOne(char* s, int index) {
    s[index] += 1;
    if (s[index] > 'z') {
        s[index] = 'a';
        addOne(s, index - 1);
    }
}

int main()
{
    int fd;
    char name[] = "aa";
    char symlink[] = "test/aa";
    (void)umask(0);
    // ������ ���������� ��� �����
    if (mkdir("test", 0777) < 0) {
        printf("Unable to create directory test. If it exists, delete it.\n");
        exit(-1);
    }
    // ������ ��������� ����
    if ((fd = open("test/aa", O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can not open file\n");
        exit(-1);
    }
    if (close(fd) < 0) {
        printf("Can not close file\n");
    }
    int max = 0;
    while (1) {
        addOne(symlink, 6);
        if (symlink(name, symlink) != 0) {
            printf("Unable to create symlink\n");
            exit(-1);
        }
        // ���������, ����� �� �� ��� ������� ����
        if ((fd = open(symlink, O_WRONLY, 0666)) < 0) {
            // ���� ������, ������ �� �������� ������������� ������ ��������
            break;
        }
        else {
            max++;
        }
        if (close(fd) < 0) {
            printf("Can not close file\n");
        }
        addOne(name, 1);
    }
    printf("maxRecursion: %d\n", max);
    return 0;
}