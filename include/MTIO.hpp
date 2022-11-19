#ifndef MTIO_HPP
#define MTIO_HPP

#include <ctype.h>

#define MT_ESCAPE "QUIT"
#define MT_ESCAPE_SIZE 4

#define MT_NO_ERROR 0
#define MT_TYPE_ERROR 1

long double promptAndGetLD(const char *, char *, size_t, bool *);
long long promptAndGetLL(const char *, char *, size_t, bool *);
char *getInput(char *, size_t);
bool quitCheck(char *);
short errorCheckLD(long double);
short errorCheckLL(long long);

#endif