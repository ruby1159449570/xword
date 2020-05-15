/* /home/franx/xword/xw_puzzutil.c Tue10Feb2004 {fcG} */

#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_CREATEROW		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_createrow(PUZZHEAD *ph, int i)
{
        register	int j,k;
	register	PUZZLE *ref, *prev, *prevrow, *edge;

#if DEBUG
	printf("xw_createrow: i = %d\n", i);
#endif /* DEBUG */

	if(i < ph->ph_puzzle->pz_rownum)
	  {
	    prevrow = ph->ph_puzzle;
	    for(j = ph->ph_puzzle->pz_rownum - 1; j >= i; j--)
	      {
		ph->ph_numrows++;
		edge = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		ref = edge;
		prev = NULL;
		for (k = 0; k < ph->ph_numcols; k++)
		  {
		    ref->pz_letter = '\0';
		    ref->pz_rownum = j;
		    ref->pz_colnum = prevrow->pz_colnum;
		    ref->pz_left = prev;
		    if(prev != NULL)
		      {
			prev->pz_right = ref;
		      }
		    ref->pz_down = prevrow;
		    prevrow->pz_up = ref;
		    prev = ref;
		    ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		    prevrow = prevrow->pz_right;
		  }
		prevrow = edge;
	      }
	    ph->ph_puzzle = edge;
	  }
	else
	  {
	    prevrow = ph->ph_puzzle;
	    while(prevrow->pz_down != NULL)
	      {
		prevrow = prevrow->pz_down;
	      }
	    if (prevrow->pz_down != NULL)
	      {
		prevrow = prevrow->pz_down;
	      }
	    if (prevrow->pz_rownum < i)
	      {
		for (j = prevrow->pz_rownum + 1; j <= i; j++)
		  {
		    ph->ph_numrows++;
		    edge = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		    ref = edge;
		    prev = NULL;
		    for (k = 0; k < ph->ph_numcols; k++)
		      {
			ref->pz_letter = '\0';
			ref->pz_rownum = j;
			ref->pz_colnum = prevrow->pz_colnum;
			ref->pz_left = prev;
			if(prev != NULL)
			  {
			    prev->pz_right = ref;
			  }
			ref->pz_up = prevrow;
			prevrow->pz_down = ref;
			prev = ref;
			ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
			prevrow = prevrow->pz_right;
		      }
		    prevrow = edge;
		  }
	      }
	  }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_CREATECOLUMN       	#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_createcolumn(PUZZHEAD *ph, int i)
{
        register	int j, k;
	register	PUZZLE *ref, *prev, *prevcol, *edge;

#if DEBUG
	printf("xw_createcolumn: i = %d\n", i);
	PRINT1(d, ph->ph_puzzle->pz_colnum);
#endif /* DEBUG */

	if(i < ph->ph_puzzle->pz_colnum)
	  {
	    prevcol = ph->ph_puzzle;
	    for(j = ph->ph_puzzle->pz_colnum - 1; j >= i; j--)
	      {
		ph->ph_numcols++;
		edge = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		ref = edge;
		prev = NULL;
		for (k = 0; k < ph->ph_numrows; k++)
		  {
		    ref->pz_letter = '\0';
		    ref->pz_colnum = j;
#ifdef DEBUG
		    PR(#010x, (UINT)prevcol);
		    PRINT2(d, (UINT)prevcol->pz_rownum,
			   (UINT)ph->ph_numcols);
#endif /* DEBUG */
		    ref->pz_rownum = prevcol->pz_rownum;
		    ref->pz_up = prev;
		    if(prev != NULL)
		      {
			prev->pz_down = ref;
		      }
		    ref->pz_right = prevcol;
		    prevcol->pz_left = ref;
		    prev = ref;
		    ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		    prevcol = prevcol->pz_down;
		  }
		prevcol = edge;
	      }
	    ph->ph_puzzle = edge;
	  }
	else
	  {
	    prevcol = ph->ph_puzzle;
	    while(prevcol->pz_right != NULL)
	      {
		prevcol = prevcol->pz_right;
	      }
	    if (prevcol->pz_colnum < i)
	      {
		for (j = prevcol->pz_colnum + 1; j <= i; j++)
		  {
		    ph->ph_numcols++;
		    edge = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
		    prev = NULL;
		    ref = edge;
		    for (k = 0; k < ph->ph_numrows; k++)
		      {
			ref->pz_letter = '\0';
			ref->pz_colnum = j;
			ref->pz_rownum = prevcol->pz_rownum;
			ref->pz_up = prev;
			if(prev != NULL)
			  {
			    prev->pz_down = ref;
			  }
			ref->pz_left = prevcol;
			prevcol->pz_right = ref;
			prev = ref;
			ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));
			prevcol = prevcol->pz_down;
		      }
		    prevcol = edge;
		  }
	      }
	  }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_POINTPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

PUZZLE	*xw_pointpuzz(PUZZHEAD *ph, int x, int y)
{
  register PUZZLE *ref;

#if DEBUG
	printf("xw_pointpuzz: x = %d, y = %d\n", (int)x, (int)y);
	WHERE, PRINT1(#010x, (UINT)ph);
/* 	PRINT2(d, ph->ph_puzzle->pz_rownum, ph->ph_puzzle->pz_colnum); */
#endif /* DEBUG */
	ref = ph->ph_puzzle;
	if((ref->pz_colnum > x) || (ref->pz_rownum > y))
	{
	  xw_error(SV_INFO, "pointpuzz: setting ref to NULL");
	  ref = NULL;
	}
	else
	{
	  while((ref != NULL) && (ref->pz_colnum < x))
	    {
	      ref = ref->pz_right;
	    }
	  while ((ref != NULL) && (ref->pz_rownum < y))
	    {
	      ref = ref->pz_down;
	    }
	}
#if DEBUG
	  printf("xw_pointpuzz: returning %#010x\n", (UINT)ref);
#endif /* DEBUG */
	  return ref;
}

/* End of /home/franx/xword/xw_puzzutil.c */
