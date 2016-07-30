#include "wrapunix.h"

pid_t Fork(void)
{
	pid_t pid;

	if ((pid = fork()) == -1)
	{
		printf("fork error! errno = %d\n",errno);
		exit(-1);
	}
	return(pid);
}

pid_t Wait(int *iptr)
{
	pid_t	pid;

	if ((pid = wait(iptr)) == -1)
	{
		printf("wait error! errno = %d\n", errno);
		exit(-1);
	}
	return(pid);
}
pid_t Waitpid(pid_t pid, int *iptr, int options)
{
	pid_t retpid;

	if ((retpid = waitpid(pid, iptr, options)) == -1)
	{
		printf("waitpid error! errno = %d\n", errno);
		exit(-1);
	}
	return(retpid);
}

int Open(const char *pathname, int oflag, mode_t mode)
{
	int fd;

	if ((fd = open(pathname, oflag, mode)) == -1)
	{
		printf("open error! errno = %d\n", errno);
		exit(-1);
	}
	return(fd);
}
void Close(int fd)
{
	if (close(fd) == -1)
	{
		printf("close error! errno = %d\n", errno);
		exit(-1);
	}
}

void Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
	{
		printf("write error! errno = %d\n", errno);
		exit(-1);
	}
}
ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t n;

	if ((n = read(fd, ptr, nbytes)) == -1)
	{
		printf("read error! errno = %d\n", errno);
		exit(-1);
	}
	return(n);
}
