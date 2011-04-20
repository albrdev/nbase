#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

#include <unistd.h>	/* execl() */

#include "./nbase.h"

#define APP_NAME	"nbase"
#define APP_VERSION	"0.1"
#define APP_AUTHOR	"Floff"

/*#define DEBUG_MODE*/

#ifdef DEBUG_MODE
	#define BIN_NBASE	"./bin/nbase"
	#define BIN_STOB	"./bin/nbase-stob"
	#define BIN_BTOS	"./bin/nbase-btos"
#else
	#define BIN_NBASE	"/usr/bin/nbase"
	#define BIN_STOB	"/usr/bin/nbase-stob"
	#define BIN_BTOS	"/usr/bin/nbase-btos"
#endif	/* DEBUG_MODE */

#define ARGS_MIN	2		/*1 + 1*/

#define ARGL_STOB	"stob"
#define ARGL_BTOS	"btos"

#define ARG_HELP	"-h"
#define ARGL_HELP	"--help"

#define BASE_MIN	2

char **nbase_pargs	= NULL;

void print_usage(void)
{
	printf("%s v%s by %s\n", APP_NAME, APP_VERSION, APP_AUTHOR);

	printf("\tUsage: %s (%s || %s) && [ADDITIONAL OPTIONS]\n", APP_NAME, ARGL_STOB, ARGL_BTOS);

	printf("\n\t[OPTIONS]\n");
	printf("\t\t%s, %s\tConvert mode (\"string to base\" or \"base to string\")\n", ARGL_STOB, ARGL_BTOS);

	printf("\n\t[ADDITIONAL OPTIONS]\n");
	printf("\t\tDepends on converting mode\n");
	printf("\t\tRun following commands for additional usage:\n\n");
	printf("\t\t%s %s %s\n", APP_NAME, ARGL_STOB, ARGL_HELP);
	printf("\t\t%s %s %s\n", APP_NAME, ARGL_BTOS, ARGL_HELP);
}

void cleanup(void)
{
	if(nbase_pargs != NULL) {
		free(nbase_pargs);
	}
}

int main(int argc, char* argv[])
{
	int i, j;
	char *pbinfile;

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

	if(strcmp(argv[1], ARGL_STOB) == 0) {
		pbinfile = BIN_STOB;
	}
	else if(strcmp(argv[1], ARGL_BTOS) == 0) {
		pbinfile = BIN_BTOS;
	}

	atexit(cleanup);
	nbase_pargs = malloc(argc * sizeof(char *));
	if(nbase_pargs == NULL) {
		fprintf(stderr, "*** Error: Could not allocate memory\n");
		exit(EXIT_FAILURE);
	}

	for(i=2, j=1; i<argc; i++, j++) {
		nbase_pargs[j] = argv[i];
	}

	nbase_pargs[0] = pbinfile;
	nbase_pargs[j] = NULL;

	if(execv(pbinfile, nbase_pargs) == -1) {
		fprintf(stderr, "*** Error (\'execv()\'): %s (%d): %s\n", strerror(errno), errno, pbinfile);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

