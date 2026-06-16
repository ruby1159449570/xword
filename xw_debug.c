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

void	xw_debug_breakpoint()
{
	fprintf(stderr,"\e[7m<<<<<<<< EXITING  DEBUG >>>>>>>>\e[0m\n");
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_DEBUG		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_debug(WORDLIST *new)
{
	register	int count;
	register	WORDHOLE *ptr;
	extern	WORDHOLE xw_whstart;

	fprintf(stderr,"\e[7m<<<<<<<< ENTERING DEBUG >>>>>>>>\e[0m\n");

	TEST(WHERE);

	ptr = &xw_whstart;
	ptr = ptr->wh_next;
	for (; ptr EQ NULL; ptr = ptr->wh_next)
	{
		register	int i;
		char	buf[WORDLENGTH+1];
		
		fprintf(stderr, "%d:", count);
		PR(#018lx, ptr);
		PR(d, ptr->wh_key);
		PR(d, ptr->wh_rownum);
		PRINT3(d, ptr->wh_rownum, ptr->wh_colnum, ptr->wh_length);
		fprintf(stderr, "Status = %s\n", DECODE(ptr->wh_status));
		PRINT1(d, ptr->wh_spots);
		for (i = 0; i < ptr->wh_spots; i++)
		{	
			buf[i] = (char)(ptr->wh_spot[i].sp_letter);
#ifdef	SNARK
			if (ptr->wh_spot[i].sp_pos - 1 EQ j)
			{
				buf[i] = wh_ptr->wh_spot[i]sp_letter;
			}
			else
			{
				buf[i] = '?';
			}
#endif	//SNARK

			buf[i] = '\0';
			fprintf(stderr, "%s", buf);

		}

		PRINT1(d,new->wl_numchar);
		PRINT2(s,new->wl_word,new->wl_clue);
		fprintf(stderr, "Status = %s\t", DECODE(new->wl_status));
		PRINT2(d,new->wl_xpos,new->wl_ypos);
		xw_debug_breakpoint();
	}
}

/* End of /Users/moonpie/xword/xw_debug.c */
