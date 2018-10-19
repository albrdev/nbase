#include "./nbase.h"


uint32_t nbase_cblen(uint32_t chr, uint32_t base)
{
	return (chr > 0U ? (uint32_t)floor((log((double)chr * (double)base) / log((double)base))) : 0U);
}

size_t nbase_ctob(uint32_t chr, const unsigned int base, const char *const map, char *pres)
{
	size_t len = 0U;

	assert(map != NULL);
	assert(pres != NULL);

	while(chr > 0U) {
		*pres = map[chr % base];

		len++;
		pres++;
		chr /= base;
	}

	*pres = '\0';
	return len;
}

size_t nbase_btoc(const char *str, const unsigned int base, const char *const pmap, uint32_t *pres)
{
	size_t len = 0U;
	uintptr_t pos = 0U;

	assert(str != NULL);
	assert(pmap != NULL);
	assert(pres != NULL);

	errno = 0;

	*pres = 0U;
	while(*str != '\0') {
		if((pos = (uintptr_t)strchr(pmap, *str)) == 0U) {
			errno = EDOM;
			break;
		}

		pos -= (uintptr_t)pmap;
		*pres += (uint32_t)(pos * (uintptr_t)(base << len));

		len++;
		str++;
	}

	return len;
}
