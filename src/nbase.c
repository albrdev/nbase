#include "./nbase.h"


int chrpos(const unsigned char ch, const char *const pstr)
{
	const char *ppos = pstr;

	while(*ppos != CHAR_NULL) {
		if(ch == *ppos) {
			return (int)(ppos - pstr);
		}

		ppos++;
	}

	return -1;
}

void strrvr(char *str)
{
	char *rstr;
	char swap;

	rstr = str + strlen(str) - 1;
	while(str < rstr) {
		swap = *str;
		*str = *rstr;
		*rstr = swap;

		str++;
		rstr--;
	}
}

int nbase_bchlen(const unsigned char val, const int base)
{
	return floor(log(base * val) / log(base));
}

int nbase_atob(unsigned char ch, const int base, const char *const pmap, char *pbuf)
{
	int len = 0;

	if(pmap == NULL || pbuf == NULL) {
		return -1;
	}

	while(ch > 0) {
		*(pbuf + len++) = *(pmap + (ch % base));
		ch /= base;
	}

	*(pbuf + len) = CHAR_NULL;
	strrvr(pbuf);

	return len;
}

int nbase_btoa(char *str, const int base, const char *const pmap, unsigned char *const pbuf)
{
	int len = 0, pos;

	if(str == NULL || pmap == NULL || pbuf == NULL) {
		return -1;
	}

	*pbuf = 0;
	strrvr(str);
	while(*str != CHAR_NULL) {
		pos = chrpos(*str, pmap);
		if(pos == -1) {
			return -1;
		}

		*pbuf += pos * pow(base, len++);
		str++;
	}

	return len;
}

