#ifndef ARGS_H_
#define ARGS_H_


#include <stdlib.h>	/* NULL, EXIT_SUCCESS, EXIT_FAILURE, size_t, exit() */
#include <stdio.h>	/* NULL, size_t, stderr, fprintf(), printf() */
#include <string.h>	/* NULL, size_t, strerror() */
#include <errno.h>	/* errno */
#include <assert.h>	/* assert() */

#include <unistd.h>	/* optarg, opterr, optopt */
#include <getopt.h>	/* option, no_argument, optional_argument, required_argument, getopt_long() */

#include "./types.h"	/* nbase_options */
#include "./base.h"	/* strtouint() */


#ifdef __cplusplus
extern "C" {
#endif


#define ARGC_MIN	(1 + 1)

#define OPTSTR		("-b:f::d::m:vh")

#define ARGOPTS_SIZE	(10U + 1U)

#define OPTIDL_BASE	("base")
#define OPTIDS_BASE	('b')

#define OPTIDL_FILL	("fill")
#define OPTIDS_FILL	('f')

#define OPTIDL_DELIM	("delim")
#define OPTIDS_DELIM	('d')

#define OPTIDL_MAP	("map")
#define OPTIDS_MAP	('m')

#define OPTIDL_VERBOSE	("verbose")
#define OPTIDS_VERBOSE	('v')

#define OPTIDL_HELP	("help")
#define OPTIDS_HELP	('h')

#define OPTDEF_BASE	(2U)
#define OPTDEF_FILL	("0")
#define OPTDEF_DELIM	(" ")
#define OPTDEF_MAP	("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define OPTDEF_FLAGS	(of_none)


enum optflags
{
	of_none		= 0,
	of_fill		= 1,
	of_delim	= 2,
	of_verbose	= 4
};


/*void printusage(void);*/

void parseargs(const int argc, char *const argv[], struct nbase_options *const popts);


#ifdef __cplusplus
}
#endif


#endif	/* #ifndef ARGS_H_ */
