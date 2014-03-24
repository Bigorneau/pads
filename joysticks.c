#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/apm_bios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct s_joystick {
	char device[30];
	int fd;
};

static struct s_joystick joystks[30];

static int open_joysticks(fd_set *set)
{
	int i, fd, biggest, size;
	char * buff;

	if (!set)
		return -2;
	size = 30;
	buff = malloc(size * sizeof(char) + 1);
	biggest = -1;

	for (i = 0, fd = -1; ; i++) {
		sprintf(buff, "/dev/input/js%d", i);
		if ((fd = open(buff, O_RDWR)) < 0) {
			break;
		}
		if (fd > biggest)
			biggest = fd;
		FD_SET(fd, set);

		strncpy(joystks[i].device, buff, strlen(buff));
		joystks[i].fd = fd;
	}
	free(buff);
	return biggest;
}

static void close_joysticks()
{
	int i;
	for (i = 0; joystks[i].fd != -1; i++)
		close(joystks[i].fd);
}

static int read_input(int id, int size)
{
	int n, fd;
	struct input_event buff[32];
	fd = joystks[id].fd;
	if ((n = read(fd, &buff, sizeof(buff))) < 0) {
		printf("read: fd=%d, '%s'\n", fd, strerror(errno));
		return -1;
	}
	n /= sizeof(buff[0]);
	printf("%s:  ", joystks[id].device);
	printf("time=%ld.%06ld     ",
			buff[0].time.tv_sec,
			buff[0].time.tv_usec);
	printf("type=%u, code=%u, value=%d\n",
			buff[0].type,
			buff[0].code,
			buff[0].value);
	return 0;
}

int main(void)
{
	int i, biggest, fd, size;
	fd_set set;

	FD_ZERO(&set);
	memset(&joystks, 0, sizeof(joystks));

	if ((biggest = open_joysticks(&set)) < 0) {
		printf("open_joysticks: failed\n");
		return -1;
	}

	while(1) {
		if ((size = select(biggest + 1, &set, NULL, NULL, NULL)) == -1) {
			printf("select: %d, '%s'\n", size, strerror(errno));
			goto exit;
		}
		if (!size)
			continue;
		for (i = 0; joystks[i].fd; i++) {
			fd = joystks[i].fd;
			if (FD_ISSET(fd, &set))
				read_input(i, size);
		}
	}
exit:
close:
	close_joysticks();

	return 0;
}
