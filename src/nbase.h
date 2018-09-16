#ifndef NBASE_H_
#define NBASE_H_


#include <string.h>	/* NULL, size_t, strchr() */
#include <stdint.h>	/* uint64_t, uint32_t */
#include <math.h>	/* floor(), log(), pow() */
#include <errno.h>	/* EDOM, errno */
#include <assert.h>	/* assert() */


#ifdef __cplusplus
extern "C" {
#endif


uint32_t nbase_cblen(uint32_t chr, uint32_t base);
size_t nbase_ctob(uint32_t chr, const unsigned int base, const char *const pmap, char *pres);
size_t nbase_btoc(const char *str, const unsigned int base, const char *const map, uint32_t *pres);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef NBASE_H_ */
