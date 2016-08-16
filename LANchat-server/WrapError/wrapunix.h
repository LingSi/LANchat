#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pid_t Fork(void);
pid_t Wait(int *iptr);
pid_t Waitpid(pid_t pid, int *iptr, int options);

int Open(const char *pathname, int oflag, mode_t mode);
void Close(int fd);
void Write(int fd, void *ptr, size_t nbytes);
ssize_t Read(int fd, void *ptr, size_t nbytes);

