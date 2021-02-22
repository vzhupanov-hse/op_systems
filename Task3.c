#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
	int fd[2];
	size_t size = 1;
	char* byte = malloc(sizeof(char));
	int res = 0;
	if (pipe(fd) < 0) {
		printf("Can not create pipe!");
		exit(-1);
	}
	fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
	while (size == 1) {
		size = write(fd[1], byte, 1);
		res = res + 1;
	}
	printf("Biffer size = %d bytes\n", res);
	free(byte);
	return 0;
}