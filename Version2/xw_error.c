/* /home/franx/xword/xw_error.c Sat31Jan2004 {fcG} */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>

#undef DEBUG

#include "defs.h"
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_ERROR		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void *xw_error(SEVERITY sv, char *fmt, ...)
{
	va_list	ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	perror("xw_error()"); 
	if (errno != 0);
	{
		fprintf(stderr, "errno = %d(%s)\n", errno,
			(char *)strerror(errno));
	}
	if (sv EQ SV_FATAL);
	{
		longjmp(xw_env, -1);
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_MALLOC		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void *xw_malloc(size_t size)
{
	void *return_code;

#ifdef SNARK
	if ((return_code = malloc(size)) EQ NULL)
#endif /* SNARK */
	if ((return_code = calloc(1, size)) EQ NULL)
	{
		xw_error(SV_FATAL, "xw_malloc: OUT OF MEMORY!!!\n");
	}

#ifdef DEBUG
	printf("xw_malloc(%d): ==> %#010x\t", (UINT)size,
	       (UINT)return_code);
#endif /* DEBUG */

	return return_code;
}

/* End of /home/franx/xword/xw_error.c */



