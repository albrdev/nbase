#include <stdlib.h>	/* NULL, EXIT_SUCCESS, EXIT_FAILURE, size_t, exit() */
#include <stdio.h>	/* NULL, size_t, fprintf(), printf() */
#include <string.h>	/* NULL, size_t, strlen(), strspn(), strcspn(), strerror() */
#include <ctype.h>	/* isprint() */
#include <stdint.h>	/* UINT8_MAX, uint64_t, uint32_t */
#include <errno.h>	/* errno */
#include <inttypes.h>	/* PRIu64 */

#include "./types.h"	/* nbase_options */
#include "./base.h"	/* strrev() */
#include "./args.h"	/* parseargs() */
#include "./nbase.h"	/* nbase_cblen(), nbase_ctob() */


#ifdef __cplusplus
extern "C" {
#endif


int main(int argc, char *argv[])
{
	struct nbase_options opts = {{'\0'}, 0U, OPTDEF_BASE, OPTDEF_FILL, OPTDEF_DELIM, OPTDEF_MAP, OPTDEF_FLAGS};

	char tmpbuf[128U + 1U] = {'\0'}, *ptrgstriter = NULL;
	uint32_t tmpchr = '\0';
	size_t tmplen = 0U;
	const struct escseq *pescseq = NULL;

	size_t baselen = 0U, filllen = 0U, delimlen = 0U;
	size_t delimspn, fillspn, maxfilllen = 0U;

	parseargs(argc, argv, &opts);

	maxfilllen = nbase_cblen(UINT8_MAX, opts.base);
	ptrgstriter = opts.trgstr;

	while(*ptrgstriter != '\0') {
		delimspn = (opts.flags & of_delim ? strcspn(ptrgstriter, opts.delimchr) : maxfilllen);
		strncpy(tmpbuf, ptrgstriter, delimspn);
		delimspn = strlen(tmpbuf);
		fillspn = strspn(tmpbuf, opts.fillchr);

		strrvr(tmpbuf + fillspn, delimspn - fillspn);
		tmplen = nbase_btoc(tmpbuf + fillspn, opts.base, opts.map, &tmpchr);
		if(errno == 0 && (opts.flags & of_fill && delimspn == maxfilllen)) {
			if(isprint((int)tmpchr) != 0) {
				printf("%c", (unsigned char)tmpchr);
			}
			else {
				if((pescseq = findescseqbyvalue(tmpchr)) != NULL) {
					printf("\\%c", pescseq->id);
				}
				else {
					printf("\\%hhu", (unsigned char)tmpchr);
				}
			}

			fflush(stdout);

			baselen += tmplen;
			filllen += fillspn;
		}

		ptrgstriter += delimspn;
		if(opts.flags & of_delim) {
			delimspn = strspn(ptrgstriter, opts.delimchr);
			delimlen += delimspn;
			ptrgstriter += delimspn;
		}
	}

	printf("\n");
	if(opts.flags & of_verbose) {
		printf("\n");
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
