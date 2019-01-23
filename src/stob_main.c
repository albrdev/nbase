#include <stdlib.h>	/* NULL, EXIT_SUCCESS, EXIT_FAILURE, size_t, exit() */
#include <stdio.h>	/* NULL, size_t, fprintf(), printf() */
#include <string.h>	/* NULL, size_t, strlen() */
#include <stdint.h>	/* UINT8_MAX, uint64_t, uint32_t */
#include <errno.h>	/* errno */
#include <inttypes.h>	/* PRIu64 */

#include "./types.h"	/* nbase_options */
#include "./base.h"	/* strrvr() */
#include "./args.h"	/* parseargs() */
#include "./nbase.h"	/* nbase_cblen(), nbase_ctob() */


#ifdef __cplusplus
extern "C" {
#endif


int main(int argc, char *argv[])
{
	struct nbase_options opts = {{'\0'}, 0U, OPTDEF_BASE, OPTDEF_FILL, OPTDEF_DELIM, OPTDEF_MAP, OPTDEF_FLAGS};

	char tmpbuf[128U + 1U] = {'\0'}, trgbuf[1024U] = {'\0'};
	size_t i = 0U, tmplen = 0U, baselen = 0U, filllen = 0U, delimlen = 0U;
	size_t maxfilllen = 0U;

	parseargs(argc, argv, &opts);

	maxfilllen = nbase_cblen(UINT8_MAX, opts.base);
	esctostr(opts.trgstr, trgbuf, &opts.trglen);
	for(i=0U; i<opts.trglen; i++) {
		tmplen = nbase_ctob((unsigned char)trgbuf[i], opts.base, opts.map, tmpbuf);
		strrvr(tmpbuf, tmplen);
		baselen += tmplen;

		if(opts.flags & of_fill) {
			while(tmplen < maxfilllen) {
				putchar(*opts.fillchr);
				filllen++;
				tmplen++;
			}
		}

		printf("%s", tmpbuf);

		if(opts.flags & of_delim && i + 1 < opts.trglen) {
			putchar(*opts.delimchr);
			delimlen++;
		}
	}

	printf("\n\n");
	if(opts.flags & of_verbose) {
		printf("[stats]\n");

		printf("trglen:\t\t%lu\n\n", (unsigned long int)opts.trglen);

		printf("baselen:\t%lu\n", (unsigned long int)baselen);
		printf("filllen:\t%lu\n", (unsigned long int)filllen);
		printf("total:\t\t%lu\n\n", (unsigned long int)(baselen + filllen));

		printf("delimlen:\t%lu\n", (unsigned long int)delimlen);
		printf("total:\t\t%lu\n", (unsigned long int)(baselen + filllen + delimlen));
	}

	exit(EXIT_SUCCESS);
}


#ifdef __cplusplus
}
#endif
