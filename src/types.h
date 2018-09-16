#ifndef TYPES_H_
#define TYPES_H_


#include <stddef.h>		/* size_t */
#include <stdint.h>		/* uint32_t */


#define NO_TRGSTR_MAXLEN	(1024U + 1U)


#ifdef __cplusplus
extern "C" {
#endif


struct nbase_options
{
	char trgstr[NO_TRGSTR_MAXLEN];
	size_t trglen;

	uint32_t base;

	char *fillchr;
	char *delimchr;

	char *map;

	int flags;
};


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef TYPES_H_ */
