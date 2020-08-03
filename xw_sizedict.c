/* /home/franxg/debianstuff/xword/xw_sizedict.c Wed06Jul2016 {fcG} */

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
/* #define extern */
#include "xword.h"

char sysbuf[SZ_STRING], buf[WORDLENGTH+1];

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	SIZEDICT returns length of	#
#	longest word in dictionary	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int xw_sizedict(char index[])
{
	UINT count;

	count = 0;
	for(EVER)
	{
#ifdef	DEBUG
		PRINT2(s, xw_indexfile, index);
#endif	/* DEBUG */

		INDEXDIR(++count);
/* 		sprintf(sysbuf, "[ -d  %s ]", xw_index); */
		sprintf(sysbuf, "test -d  %s", xw_indexdir);

#ifdef	DEBUG
		PR(s, sysbuf);
		PRINT1(d, count);
#endif	/* DEBUG */

		if (system(sysbuf) != 0)
		{
			break;
		}
		else
		{
			TEST(PR(s, sysbuf));
			TEST(PRINT1(d, count));
		}
	}
	TEST(PR(s,fprintf(stderr,"xw_sizedict: returning %d\n",count-1)));
	return count - 1;
}

#ifdef	SNARK
int main(int argc, char *argv[])
{
	xw_sizedict("john.sort");
}
#endif	/* SNARK */
/* /End of /home/franxg/debianstuff/xword/xw_sizedict.c */
