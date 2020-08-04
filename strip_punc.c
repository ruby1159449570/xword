/* /Users/moonpie/xword/strip_punc.c Sat18Jul2020 {fcG} */

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#define extern
//#include "xword.h"
#include "defs.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		STRIP_PUNC          	#
#	Strip punctation from input	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
	int c; 

/* 	while(!feof(stdin)) */
	while((c = getchar()) > 0 )
	{

		if (isalpha(c) || ( c < ' ' ))
#ifdef	SNARK
		&& c < ':')
		|| ( c > '@' && c < '[')
		|| ( c > '`' && c < '~'))
#endif	/* SNARK */
		{
			putchar(c);
		}
	}
}
/* End of /Users/moonpie/xword/strip_punc.c  */
