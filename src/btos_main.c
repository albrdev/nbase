#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <unistd.h>	/* optarg, getopt() */

extern char *optarg;

#include "./nbase.h"

#define APP_NAME	"nbase_btos"
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

#define BASELEN_MAX		9	/*8 + 1*/

void print_usage(void)
{
	printf("%s v%s by %s\n", APP_NAME, APP_VERSION, APP_AUTHOR);

	printf("\tUsage: %s -s <string> -b <base> || [OPTIONS]\n", APP_NAME);

	printf("\n\t[REQUIRED]\n");
	printf("\t\t-%c <string>\tThe base-n string to convert\n", ARG_STR);
	printf("\t\t-%c <base>\tThe base to convert from (base >= %d && base <= charmap_length (%u))\n", ARG_BASE, BASE_MIN, (unsigned int)strlen(nbase_pcharmap));

	printf("\n\t[OPTIONAL]\n");
	printf("\t\t-%c <string>\tCustom charmap (length >= base) (0-9 + A-Z + a-z)\n", ARG_CHARMAP);
	printf("\t\t-%c\t\tIf bytes were filled out (%d)\n", ARG_BYTEFILL, nbase_bytefill);
	printf("\t\t-%c <char>\tBytes are filled with this character (\'%c\')\n", ARG_FILLCHAR, nbase_bytefillchar);
	printf("\t\t-%c\t\tThe string has delimiter characters in it (%d)\n", ARG_DELIMIT, nbase_delimit);
	printf("\t\t-%c <char>\tDelimit character to look for (\'%c\')\n", ARG_DELIMITCHAR, nbase_delimitchar);
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
	int templen, maxbaselen, splitpos = 0, tempstrlen, baselen = 0, bytefilllen = 0, delimitlen = 0;
	char pbasebuf[BASELEN_MAX];
	unsigned char asciichar;

	parse_args(argc, argv);

	maxbaselen = nbase_bchlen(UCHAR_MAX, nbase_base);
	if(nbase_delimit != 0) {
		while(1) {
			splitpos = chrpos(nbase_delimitchar, nbase_pstr);
			memset(pbasebuf, '\0', BASELEN_MAX * sizeof(char));
			strncpy(pbasebuf, nbase_pstr, (splitpos != -1 ? splitpos : maxbaselen));

			templen = nbase_btoa(pbasebuf, nbase_base, nbase_pcharmap, &asciichar);
			if(templen == -1) {
				fprintf(stderr, "*** Error: \'nbase_stob()\' failed on address %p\n", pbasebuf);
				exit(EXIT_FAILURE);
			}

			baselen += templen;
			printf("%c", asciichar);

			if(nbase_bytefill != 0 && splitpos != -1) {
				bytefilllen += splitpos - nbase_bchlen(asciichar, nbase_base);
			}

			if(splitpos == -1) {
				break;
			}

			delimitlen++;
			nbase_pstr += splitpos + 1;
		}
	}
	else if(nbase_bytefill != 0) {
		tempstrlen = strlen(nbase_pstr);
		if(tempstrlen % maxbaselen != 0) {
			fprintf(stderr, "*** Error: String length does not match: (%d %% %d != 0)\n", tempstrlen, maxbaselen);
			exit(EXIT_FAILURE);
		}

		while(*nbase_pstr != CHAR_NULL) {
			memset(pbasebuf, '\0', BASELEN_MAX * sizeof(char));
			strncpy(pbasebuf, nbase_pstr, maxbaselen);

			templen = nbase_btoa(pbasebuf, nbase_base, nbase_pcharmap, &asciichar);
			if(templen == -1) {
				fprintf(stderr, "*** Error: \'nbase_stob()\' failed\n");
				exit(EXIT_FAILURE);
			}

			baselen += templen;
			printf("%c", asciichar);

			bytefilllen += maxbaselen - nbase_bchlen(asciichar, nbase_base);
			nbase_pstr += maxbaselen;
		}

		baselen -= bytefilllen;
	}
	else {
		fprintf(stderr, "*** Error: Need more arguments (-fF, -dD)\n");
		exit(EXIT_FAILURE);
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

