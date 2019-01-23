#include "./base.h"


static struct escseq _escseqs[ESCCHRS_SIZE] = {
	{'a', '\a'},
	{'b', '\b'},
	{'f', '\f'},
	{'r', '\r'},
	{'n', '\n'},
	{'t', '\t'},
	{'v', '\v'},
	{'\'', '\''},
	{'\"', '\"'},
	{'\?', '\?'},
	{'\\', '\\'},
	{'0', '\0'}
};


int strtouint(char *const str, unsigned int *const pres, const int base, char **pend)
{
	unsigned long int tmpres = 0UL;

	assert(str != NULL);
	assert(pres != NULL);
	assert(pend != NULL);

	*pres = 0U;
	errno = 0;
	tmpres = strtoul(str, pend, base);

	if(errno != 0) {
		*pend = str;
		return -1;
	}

	if(tmpres > UINT_MAX) {
		errno = ERANGE;
		return -1;
	}

	if(*pend == str || **pend != '\0') {
		return -2;
	}

	*pend = NULL;
	*pres = (unsigned int)tmpres;
	return 0;
}

char *strrvr(char *str, const size_t len)
{
	char *pstriter = str, *pstrriter = str + (len - 1U);

	assert(str != NULL);

	if(len > 1U) {
		while(pstriter < pstrriter) {
			*pstriter ^= *pstrriter;
			*pstrriter ^= *pstriter;
			*pstriter ^= *pstrriter;

			pstriter++;
			pstrriter--;
		}
	}

	return str;
}

const struct escseq *findescseqbyvalue(const uint32_t chr)
{
	size_t i = 0U;

	for(i=0U; i<ESCCHRS_SIZE; i++) {
		if(chr == _escseqs[i].value) {
			return &_escseqs[i];
		}
	}

	return NULL;
}

char *esctostr(const char *str, char *res, size_t *const len)
{
	size_t i = 0U;
	char *resiter = res;

	assert(str != NULL);
	assert(res != NULL);
	assert(len != NULL);

	*len = 0U;
	while(*str != '\0') {
		*resiter = *str;

		if(*str == '\\') {
			if((strspn(str + 1, CHARS_OCTAL) >= 3 && sscanf(str + 1, "%3hho", (unsigned char *)resiter) > 0) || (*(str + 1) == 'x' && strspn(str + 2, CHARS_HEX) >= 2 && sscanf(str + 2, "%2hhx", (unsigned char *)resiter) > 0)) {
				str += 3;
			}
			else {
				for(i=0; i<ESCCHRS_SIZE; i++) {
					if(*(str + 1) == (char)_escseqs[i].id) {
						*resiter = (char)_escseqs[i].value;
						str++;

						break;
					}
				}
			}
		}

		str++;
		resiter++;
		(*len)++;
	}

	*resiter = '\0';
	return res;
}
