#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <unistd.h>	/* optarg, getopt() */

extern char *optarg;

#include "./nbase.h"

#define APP_NAME	"nbase_stob"
#define APP_VERSION	"0.1"
#define APP_AUTHOR	"Floff"

#define ARGS_MIN	5		/*4 + 1*/

#define ARG_ALL		"s:b:m:fF:dD:vh"
#define ARG_STR		's'
#define ARG_BASE	'b'
#define ARG_CHARMAP	'm'
#define ARG_BYTEFILL	'f'
#define ARG_FILLCHAR	'F'
#define ARG_DELIMIT	'd'
#define ARG_DELIMITCHAR	'D'
#define ARG_VERBOSE	'v'

#define ARG_HELP	"-h"
#define ARGL_HELP	"--help"

#define BASE_MIN	2

const char *nbase_pstr		= NULL;
int nbase_base			= 0;
const char *nbase_pcharmap	= DEFAULT_CHARMAP;
int nbase_bytefill		= 0;
char nbase_bytefillchar		= '0';
int nbase_delimit		= 0;
char nbase_delimitchar		= ' ';
int nbase_verbose		= 0;

#define BASELEN_MAX	9		/*8 + 1*/

void print_usage(void)
{
	printf("%s v%s by %s\n", APP_NAME, APP_VERSION, APP_AUTHOR);

	printf("\tUsage: %s -s <string> -b <base> || [OPTIONS]\n", APP_NAME);

	printf("\n\t[REQUIRED]\n");
	printf("\t\t-%c <string>\tThe ASCII string to convert\n", ARG_STR);
	printf("\t\t-%c <base>\tThe base to convert to (base >= %d && base <= charmap_length (%u))\n", ARG_BASE, BASE_MIN, (unsigned int)strlen(nbase_pcharmap));

	printf("\n\t[OPTIONAL]\n");
	printf("\t\t-%c <string>\tCustom charmap (length >= base) (0-9 + A-Z + a-z)\n", ARG_CHARMAP);
	printf("\t\t-%c\t\tFill out each byte (%d)\n", ARG_BYTEFILL, nbase_bytefill);
	printf("\t\t-%c <char>\tFill out byte with this character (\'%c\')\n", ARG_FILLCHAR, nbase_bytefillchar);
	printf("\t\t-%c\t\tPrint delimiter character between bytes (%d)\n", ARG_DELIMIT, nbase_delimit);
	printf("\t\t-%c <char>\tDelimit character (\'%c\')\n", ARG_DELIMITCHAR, nbase_delimitchar);
	printf("\t\t-%c\t\tPrint verbose (Converted lengths)\n", ARG_VERBOSE);
	printf("\t\t%s, %s\tPrints this help\n", ARG_HELP, ARGL_HELP);
}

void parse_args(int argc, char *argv[])
{
	int opt;

	if(argc > 1) {
		if(strcmp(argv[1], ARG_HELP) == 0 || strcmp(argv[1], ARGL_HELP) == 0) {
			print_usage();
			exit(EXIT_SUCCESS);
		}
	}

	if(argc < ARGS_MIN) {
		fprintf(stderr, "*** Error: Insufficient arguments: %d\n", argc - 1);
		exit(EXIT_FAILURE);
	}

	while((opt = getopt(argc, argv, ARG_ALL)) != -1) {
		switch(opt) {
			case ARG_STR:
				nbase_pstr = optarg;
				break;

			case ARG_BASE:
				sscanf(optarg, "%d", &nbase_base);
				break;

			case ARG_CHARMAP:
				nbase_pcharmap = optarg;
				break;

			case ARG_BYTEFILL:
				nbase_bytefill = 1;
				break;

			case ARG_FILLCHAR:
				if(strcmp(optarg, "\\n") == 0) {
					nbase_bytefillchar = '\n';
				}
				else if(strcmp(optarg, "\\t") == 0) {
					nbase_bytefillchar = '\t';
				}
				else if(strcmp(optarg, "\\v") == 0) {
					nbase_bytefillchar = '\v';
				}
				else {
					nbase_bytefillchar = optarg[0];
				}

				break;

			case ARG_DELIMIT:
				nbase_delimit = 1;
				break;

			case ARG_DELIMITCHAR:
				if(strcmp(optarg, "\\n") == 0) {
					nbase_delimitchar = '\n';
				}
				else if(strcmp(optarg, "\\t") == 0) {
					nbase_delimitchar = '\t';
				}
				else if(strcmp(optarg, "\\v") == 0) {
					nbase_delimitchar = '\v';
				}
				else {
					nbase_delimitchar = optarg[0];
				}

				break;

			case ARG_VERBOSE:
				nbase_verbose = 1;
				break;

			default:
				exit(EXIT_FAILURE);
		}
	}

	if(nbase_pstr == NULL) {
		fprintf(stderr, "*** Error: No ASCII-string specified\n");
		exit(EXIT_FAILURE);
	}

	if(nbase_base < BASE_MIN || nbase_base > strlen(nbase_pcharmap)) {
		fprintf(stderr, "*** Error: Invalid base: %d\n", nbase_base);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[])
{
	int templen, maxbaselen, baselen = 0, bytefilllen = 0, delimitlen = 0;
	char pbasebuf[BASELEN_MAX];

	parse_args(argc, argv);

	while(*nbase_pstr != CHAR_NULL) {
		templen = nbase_atob((unsigned char)(*nbase_pstr), nbase_base, nbase_pcharmap, pbasebuf);
		if(templen == -1) {
			fprintf(stderr, "*** Error: \'nbase_stob()\' failed\n");
			exit(EXIT_FAILURE);
		}

		baselen += templen;

		if(nbase_bytefill != 0) {
			maxbaselen = nbase_bchlen(UCHAR_MAX, nbase_base);
			strrvr(pbasebuf);
			while(templen < maxbaselen) {
				*(pbasebuf + templen) = nbase_bytefillchar;
				templen++;
				bytefilllen++;
			}

			*(pbasebuf + templen) = CHAR_NULL;
			strrvr(pbasebuf);
		}

		printf("%s", pbasebuf);
		if(nbase_delimit != 0 && *(nbase_pstr + 1) != CHAR_NULL) {
			printf("%c", nbase_delimitchar);
			delimitlen++;
		}

		nbase_pstr++;
	}

	if(nbase_verbose != 0) {
		printf("\n\n");
		printf("Base length:\t\t%d\n", baselen);
		printf("Bytefill length:\t%d\n", bytefilllen);
		printf("Delimiter length:\t%d\n", delimitlen);
		printf("Total length:\t\t%d\n", baselen + bytefilllen + delimitlen);
	}
	else {
		printf("\n");
	}

	exit(EXIT_SUCCESS);
}

