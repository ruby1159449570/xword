/* /Users/moonpie/xword/xw_makeborder.c Sat10Jul2021 {fcG} */
//
//  Created by Frank Charles Gallacher on 10/7/21.
//  Copyleft © 2020 Frank Charles Gallacher. All rights reserved.
//
// MODIFICATION HISTORY
// When		Who	What
// Tue06May2025 {fcG}	borders debugged???

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
//#define	extern
#include "xword.h"
 
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	XW_MAKEBORDER			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_makeborder(PUZZHEAD *ph)
{  
	int	x1, x2, y1, y2;
	PUZZLE	*ptr1, *ptr2;

	ptr1 = ptr2 = ph->ph_puzzle;
	x1 = ph->ph_puzzle->pz_rownum;
	x2 = ph->ph_numrows;
	y1 = ph->ph_puzzle->pz_colnum;
	y2 = ph->ph_numcols;

	WHERE, PRINT3(#018lx, (ULONG)ph, (ULONG)ptr1, (ULONG)ptr2);
	WHERE, PRINT4(d, ph->ph_numrows, ph->ph_numcols,
	 ph->ph_puzzle->pz_rownum,ph->ph_puzzle->pz_colnum);

#ifdef	SNARK
	WHERE, PRINT2(d, x1, x2, y1, y2);
#endif	//SNARK	

	ptr2 = ph->ph_puzzle;
	while(ptr2->pz_right != NULL)
	{
#ifdef	DEBUG
		WHERE, PR(c, ptr2->pz_letter);
		PRINT2(#018lx, ptr1, ptr2);
#endif	//DEBUG
		ptr2->pz_letter = EOWORD;
		ptr2 = ptr2->pz_right;
	}
	ptr1 = ph->ph_puzzle;
	while (ptr1->pz_down != NULL)
	{
#ifdef	DEBUG
		WHERE, PR(c, ptr1->pz_letter);
		PRINT2(#018lx, ptr1, ptr2);
#endif	//DEBUG
		ptr1->pz_letter = EOWORD;
		ptr1 = ptr1->pz_down;
	}
	while(ptr1->pz_right != NULL)
	{
#ifdef	DEBUG
	WHERE, PR(c, ptr1->pz_letter);
	PRINT2(#018lx, ptr1, ptr2);
#endif	//DEBUG
		ptr1->pz_letter = EOWORD;
		ptr1 = ptr1->pz_right;
	}
	while(ptr2->pz_down != NULL)
	{
#ifdef	DEBUG
	WHERE, PR(c, ptr2->pz_letter);
	PRINT2(#018lx, ptr1, ptr2);
#endif	//DEBUG
		ptr2->pz_letter = EOWORD;
		ptr2 = ptr2->pz_down;
	}
}

#ifdef	SNARK

main()
{
}
#endif	/* SNARK */ 

/* End of /Users/moonpie/xword/xw_makeborder.c */
