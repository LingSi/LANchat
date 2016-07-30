#include <stdio.h>
#include <stdlib.h>

#define err_quit(x) {\
	printf("%s!\n", x);\
	exit(-1);\
}


char *Fgets(char *ptr, int n, FILE *stream);
void Fputs(const char *ptr, FILE *stream);

