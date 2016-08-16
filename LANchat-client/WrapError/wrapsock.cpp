#include "wrapsock.h"

/*
inline int Socket(int family, int type, int protocol)
{
	int	n;
	if ( (n = socket(family, type, protocol)) < 0)
		err_sys("socket error")
	return(n);
}
*/
void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}
void Listen(int fd, int backlog)
{
	char *ptr;
	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int n;
	if ( (n = accept(fd, sa, salenptr)) < 0) 
		err_sys("accept error");
	return(n);
}
void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}

ssize_t Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
				struct sockaddr *sa, socklen_t *salenptr)
{
	ssize_t n;
	if ((n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
		err_sys("recvfrom error");
	return(n);
}
ssize_t Sendto(int fd, const void *ptr, size_t nbytes, int flags,
			const struct sockaddr *sa, socklen_t salen)
{
	ssize_t n;
	if ((n = sendto(fd, ptr, nbytes, flags, sa, salen)) != (ssize_t)nbytes)
		err_sys("sendto error");
	return(n);
}


int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,struct timeval *timeout)
{
	int n;

	if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
	{
		printf("select error! errno = %d\n", errno);
		exit(-1);
	}
	return(n);		/* can return 0 on timeout */
}
int Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int n;

	if ((n = poll(fdarray, nfds, timeout)) < 0)
	{
		printf("poll error! errno = %d\n", errno);
		exit(-1);
	}
	return(n);
}

void Shutdown(int fd, int how)
{
	if (shutdown(fd, how) < 0)
	{
		printf("shutdown error! errno = %d\n", errno);
		exit(-1);
	}
}


const char * Inet_ntop(int family, const void *addrptr, char *strptr, size_t len)
{
	const char	*ptr;
	
	if ( (ptr = inet_ntop(family, addrptr, strptr, len)) == NULL)
	{
		printf("inet_ntop error!");
		exit(-1);
	}
	return(ptr);
}

void Inet_pton(int family, const char *strptr, void *addrptr)
{
	int		n;

	if ( (n = inet_pton(family, strptr, addrptr)) != 1)
	{
		printf("inet_pton error for %s", strptr);
		exit(-1);
	}
}
