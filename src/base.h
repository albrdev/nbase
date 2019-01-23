#ifndef BASE_H_
#define BASE_H_


#include <stdlib.h>		/* NULL, size_t, strtoul() */
#include <stdio.h>		/* NULL, size_t, sscanf() */
#include <string.h>		/* NULL, size_t, strncmp(), strspn() */
#include <stdint.h>		/* uint32_t */
#include <limits.h>		/* UINT_MAX */
#include <errno.h>		/* ERANGE, errno */
#include <assert.h>		/* assert() */


#define ESCCHRS_SIZE	(12U)

#define CHARS_OCTAL	("01234567")
#define CHARS_HEX	("0123456789ABCDEFabcdef")


#ifdef __cplusplus
extern "C" {
#endif


struct escseq
{
	uint32_t id;
	uint32_t value;
};


int strtouint(char *const str, unsigned int *const pres, const int base, char **pend);

char *strrvr(char *str, const size_t len);

const struct escseq *findescseqbyvalue(const uint32_t chr);
char *esctostr(const char *str, char *res, size_t *const len);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef BASE_H_ */
