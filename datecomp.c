/* /home/franx/source/datecomp.c Thu20Jul2006 {fcG} */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		DATECOMP		#
#	date compiled executive		#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include <stdlib.h>
#include <unistd.h>
#include "defs.h"

#ifdef DATE_COMPILED

char *date_compiled()
{
	static	char datecomp = DATE_COMPILED;

	return datecomp;

}

#else	/* DATE_COMPILED */

int main(int argc, char *argv[])
{
  WHERE;
  WHEN;
}

#endif	/* DATE_COMPILED */

/* End of /home/franx/source/datecomp.c */

