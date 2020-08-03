/* /home/franx/xword/xw_error.c Sat31Jan2004 {fcG} */

// MODIFICATION HISTORY
// When		Who	What
// Wed08Jul2020 {fcG}	64-bit debug code.

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

void xw_error(SEVERITY sv, char *fmt, ...)
{
  va_list	ap;
  
  va_start(ap, fmt);
  switch(sv)
    {
    case SV_FATAL:
      fprintf(stderr,"\033[4;31mFATAL-");
      break;
    case SV_ERROR:
      fprintf(stderr,"\033[31mERROR-");
      break;
    case SV_WARNING:
      fprintf(stderr,"\033[35mWARN-");
      break;
    case SV_INFO:
      fprintf(stderr,"\033[32mINFO-");
    }
  vfprintf(stderr, fmt, ap);
  fprintf(stderr,"\033[0m\n");
  va_end(ap);
/*perror("xw_error()");  */
  if (errno != 0 && errno != ENOENT)
    {
      fprintf(stderr, "errno = %d (%s)\n", errno,
	      (char *)strerror(errno));
    }
  if (sv >= SV_ERROR)
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
	xw_error(SV_FATAL, "xw_malloc: OUT OF MEMORY!!!");
      }

#ifdef DEBUG
  printf("xw_malloc(%d): ==> %#018lx\t", (UINT)size, (ULONG)return_code);
#endif /* DEBUG */

  return return_code;
}

/* End of /home/franx/xword/xw_error.c */



