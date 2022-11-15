#ifndef MTIO_HPP
#define MTIO_HPP

#include <ctype.h>

#define MT_ESCAPE "quit"
#define MT_ESCAPE_SIZE 4
#define MT_QUIT 9

#define MT_LD_NO_ERROR 0
#define MT_LD_ERROR 1

long double promptAndGetLD(const char *message, char *buff, size_t buffSize);
short LDErrorCheck(long double, const char *);

#endif