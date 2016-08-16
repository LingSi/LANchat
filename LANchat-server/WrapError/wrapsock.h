#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>

#include <poll.h>

#define  err_sys(x) {\
	printf("%s! errno = %d\n", x, errno);\
	exit(-1);\
}

inline int Socket(int family, int type, int protocol)
{
	int	n;
	if ( (n = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return(n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Listen(int fd, int backlog);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);

ssize_t Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
				struct sockaddr *sa, socklen_t *salenptr);
ssize_t Sendto(int fd, const void *ptr, size_t nbytes, int flags,
				const struct sockaddr *sa, socklen_t salen);

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
void Shutdown(int fd, int how);

const char * Inet_ntop(int family, const void *addrptr, char *strptr, size_t len);
void Inet_pton(int family, const char *strptr, void *addrptr);
