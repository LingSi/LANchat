#include "wrapstdio.h"

char *Fgets(char *ptr, int n, FILE *stream)
{
	char *rptr;
	if ((rptr = fgets(ptr, n, stream)) == NULL)
		err_quit("fgets error");
	return (rptr);
}

void Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
		err_quit("fputs error");
}
