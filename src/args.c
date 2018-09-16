#include "./args.h"


static const struct option argopts[ARGOPTS_SIZE] = {
	{OPTIDL_BASE,		required_argument,	NULL,	OPTIDS_BASE},

	{OPTIDL_FILL,		optional_argument,	NULL,	OPTIDS_FILL},
	{OPTIDL_DELIM,		optional_argument,	NULL,	OPTIDS_DELIM},

	{OPTIDL_VERBOSE,	no_argument,		NULL,	OPTIDS_VERBOSE},

	{OPTIDL_HELP,		no_argument,		NULL,	OPTIDS_HELP},

	{NULL,			0,			NULL,	0}
};


static void printusage(void)
{
	printf("%s v.%s\n",			APPINFO_NAME,		APPINFO_VERSION);
	printf("Author:\t\t%s\n",		APPINFO_AUTHOR);
	printf("Licence:\t%s\n",		APPINFO_LICENSE);
	printf("Website:\t%s\n\n\n",		APPINFO_WEBSITE);

	printf("Usage: %s %s\n",		APPINFO_BINARY,		APPINFO_USAGE);

	printf("[Options]\n");
	printf("\t-%c, --%s=BASE\t\t\t%s\n",	OPTIDS_BASE,		OPTIDL_BASE,		"Numeral base");

	printf("\t-%c, --%s=CHAR\t\t\t%s\n",	OPTIDS_FILL,		OPTIDL_FILL,		"Fill character");
	printf("\t-%c, --%s=CHAR\t\t%s\n\n",	OPTIDS_DELIM,		OPTIDL_DELIM,		"Delimiter character");

	printf("\t-%c, --%s=MAP\t\t\t%s\n\n",	OPTIDS_MAP,		OPTIDL_MAP,		"Custom character map");

	printf("\t-%c, --%s\t\t\t%s\n\n",	OPTIDS_VERBOSE,		OPTIDL_VERBOSE,		"Verbose output");

	printf("\t-%c, --%s\t\t\t%s\n",		OPTIDS_HELP,		OPTIDL_HELP,		"Prints this help");
}

void parseargs(const int argc, char *const argv[], struct nbase_options *const popts)
{
	int opt = 0, err = 0;
	char *perrpos;

	assert(argv != NULL && popts != NULL);

	if(argc < ARGC_MIN) {
		fprintf(stderr, "*** Error\n\targc: %s (%d/%d)\n", "Insufficient arguments", argc - 1, ARGC_MIN - 1);
		exit(EXIT_FAILURE);
	}

	opterr = 0U;
	while((opt = getopt_long(argc, argv, OPTSTR, argopts, NULL)) != -1) {
		switch(opt) {
			case '\001':
				strncpy(popts->trgstr, optarg, NO_TRGSTR_MAXLEN - 1);
				popts->trglen = strlen(optarg);

				break;

			case OPTIDS_BASE:
				if((err = strtouint(optarg, (unsigned int *)&popts->base, 10, &perrpos)) == -1) {
					fprintf(stderr, "*** Error - \"%s\"\n\tstrtouint(): %s (%d)\n", perrpos, strerror(errno), errno);
					exit(EXIT_FAILURE);
				}
				else if(err == -2) {
					fprintf(stderr, "*** Error - \"%s\"\n\tstrtouint(): %s\n", perrpos, "Invalid character(s) in numeral base value");
					exit(EXIT_FAILURE);
				}

				break;

			case OPTIDS_FILL:
				popts->flags |= of_fill;

				if(optarg != NULL) {
					popts->fillchr = optarg;
				}

				break;

			case OPTIDS_DELIM:
				popts->flags |= of_delim;

				if(optarg != NULL) {
					popts->delimchr = optarg;
				}

				break;

			case OPTIDS_MAP:
				popts->map = optarg;

				break;

			case OPTIDS_VERBOSE:
				popts->flags |= of_verbose;

				break;

			case OPTIDS_HELP:
				printusage();
				exit(EXIT_SUCCESS);

				/*break;*/

			default:
				fprintf(stderr, "*** Error - %s:%s:%d\n\t%s (\'%c\')\n", __FILE__, __func__, __LINE__, "Unrecognized option argument", (char)optopt);
				exit(EXIT_FAILURE);
		}
	}

	if(*popts->trgstr == '\0'/* || popts->trglen == 0U*/) {
		fprintf(stderr, "*** Error - %p\n\tparseargs(): %s\n", popts->trgstr, "No target string specified");
		exit(EXIT_FAILURE);
	}

	if((size_t)popts->base > strlen(popts->map) || popts->base > 63 || popts->base < 2) {
		fprintf(stderr, "*** Error - %u\n\tparseargs(): %s\n", popts->base, "Invalid numeral base");
		exit(EXIT_FAILURE);
	}
}
