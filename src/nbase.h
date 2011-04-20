#ifndef NBASE_H_
#define NBASE_H_

#include <string.h>
#include <math.h>

#define DEFAULT_CHARMAP	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define CHAR_NULL	'\0'

int chrpos(const unsigned char ch, const char *const pstr);

void strrvr(char *str);

int nbase_bchlen(const unsigned char val, const int base);

int nbase_atob(unsigned char ch, const int base, const char *const pmap, char *pbuf);

int nbase_btoa(char *str, const int base, const char *const pmap, unsigned char *const pbuf);

#endif	/* NBASE_H_ */

