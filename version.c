/* /Users/moonpie/xword/version.c Tue19May2020 {fcG} */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		VERSION			#
#     date compiled/version slave	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/* #include "defs.h" */
#include "version.h"

char *date_compiled()
{
	static char datecomp[] = DATE_COMPILED;
	return datecomp;
}

float version()
{
	static float	ver = _VERSION_;
	return ver;
}

/* End of /Users/moonpie/xword/version.c  */
