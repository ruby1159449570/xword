/* /Users/moonpie/xword/xw_debug.c Sat09Oct2021 {fcG} */

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "defs.h"
#include "xword.h"

#undef 	nL
#undef 	PR
#undef 	PRINT1
#undef 	PRINT2
#undef 	PRINT3
#undef 	PRINT4

#ifdef	__STDC__
#define	PR(fmt,value)	fprintf(stderr, #value " = %" #fmt "\t", (value))
#else
#define	PR(fmt,value)	fprintf(stderr,"value = %fmt\t", (value))
#endif	/* __STDC__ */
#define	nL			fprintf(stderr,"\n")
#define	PRINT1(f,x1)		PR(f,x1),nL
#define	PRINT2(f,x1,x2)		PR(f,x1),PRINT1(f,x2)
#define	PRINT3(f,x1,x2,x3)	PR(f,x1),PRINT2(f,x2,x3)
#define	PRINT4(f,x1,x2,x3,x4)	PR(f,x1),PRINT3(f,x2,x3,x4)

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	XW_DEBUG_BREAKPOINT		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_ddebug_breakpoint()
{
	fprintf(stderr,"\033[7m<<<<<<<< EXITING  DEBUG >>>>>>>>\033[0m\n");
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_DEBUG		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_debug(WORDLIST *new)
{
	fprintf(stderr,"\033[7m<<<<<<<< ENTERING DEBUG >>>>>>>>\033[0m\n");

	PRINT1(d,new->wl_numchar);
	PRINT2(s,new->wl_word,new->wl_clue);
//	PRINT1(s,DECODE(new->wl_status));
	fprintf(stderr, "Status = %s\t", DECODE(new->wl_status));
	PRINT2(d,new->wl_xpos,new->wl_ypos);
	xw_ddebug_breakpoint();

}
/* End of /Users/moonpie/xword/xw_debug.c */
