/* /home/franx/source/testcc.c Fri Jun 16 13:25:33 EST 2006 {fcG} */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	Test program for C compilers..	#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

int main(int argc, char *argv[])
{
	char testbuf[SZ_STRING];
	int return_code;
	char *ptr;
	int one, zero;
	void *nothing;
	
	WHERE;
	WHEN;
	if ( /*/**/FALSE/*/TRUE/**/ )
	{
		printf(" Comments nest\n");
	}
	else
	{
		printf(" Comments do NOT nest\n");
	}
	PRINT3(ld, sizeof(int),sizeof(short),sizeof(long));
	PR(d, MAXINT);
	PR(d, MAXSHORT);
	nL;
	PR(ld, MAXLONG);
	PR(d, FILENAME_MAX);nL;
	return_code = sprintf(testbuf, "%s", "This a test...");
	PR(d, return_code);
	PRINT1(s, testbuf);
	ptr = testbuf;
	PRINT1(#lx, (ULONG)ptr);
	PRINT1(#018lX, (ULONG)ptr);
	PRINT1(#018lx, (ULONG)ptr);
	PRINT1(lx, (ULONG)ptr);
	PRINT1(d, argc);
	PRINT1(#018lx, 0xfeedc0defeedc0de);
	PRINT1(#018lX, 0xfeedc0defeedc0de);
}

/* End of /home/franx/source/testcc.c */
