/* /home/franx/xword/xw_initpuzz.c Mon02Feb2004 {fcG} */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#undef DEBUG

#include "defs.h"
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_INITPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

PUZZHEAD *xw_initpuzz(WORDLIST *w)
{

	PUZZHEAD *puzzle_header;

#if SNARK
	register WORDLIST *w;
#endif /* SNARK */

	register PUZZLE *ref, *prev;
	register int i;

#ifdef DEBUG
	printf("xw_initpuzz(): Starting up..\n");
#endif /* DEBUG */

	/* w = xw_poplist(); */
	puzzle_header = (PUZZHEAD *)xw_malloc(sizeof(PUZZHEAD));

#ifdef DEBUG
	printf("initpuzz: xw_malloc returned %#010x\n",
	       (UINT)puzzle_header);
#endif /* DEBUG */

	puzzle_header->ph_numrows = 1;
	puzzle_header->ph_numcols = w->wl_numchar+2;
	puzzle_header->ph_numwords = 1;
	puzzle_header->ph_lastword = w;
	puzzle_header->ph_prevpuzz = NULL;

	ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));

#ifdef DEBUG
	printf("initpuzz: xw_malloc returned %#010x\n", (UINT)ref);
#endif /* DEBUG */

	puzzle_header->ph_puzzle = ref;
	ref->pz_letter = EOWORD;
	ref->pz_rownum = 0;
	ref->pz_colnum = -1;
	prev = ref;
	ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));

#ifdef DEBUG
	printf("initpuzz: xw_malloc returned %#010x\n", (UINT)ref);
#endif /* DEBUG */

	ref->pz_ofaccrossword = w;
	for(i = 0; i < w->wl_numchar; i++)
	{
		prev->pz_right = ref;
		ref->pz_left = prev;
		ref->pz_letter = w->wl_word[i];

#ifdef DEBUG
		TEST(PR(c, ref->pz_letter));
#endif /* DEBUG */

		ref->pz_rownum = 0;
		ref->pz_colnum = i;
		prev = ref;
		ref = (PUZZLE *)xw_malloc(sizeof(PUZZLE));

#ifdef DEBUG
		printf("initpuzz: xw_malloc returned %#010x\n", (UINT)ref);
#endif /* DEBUG */

	}
	ref->pz_letter = EOWORD;
	ref->pz_rownum = 0;
	ref->pz_colnum = w->wl_numchar;
	prev->pz_right = ref;
	ref->pz_left = prev;
	w->wl_status = ACCROSS;
	w->wl_xpos = 0;
	w->wl_ypos = 0;

	TEST(WHERE);
	TEST(PRINT1(#010x, (UINT)puzzle_header));
	return puzzle_header;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_COPYPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

PUZZHEAD *xw_copypuzz(PUZZHEAD *old)
{
	PUZZHEAD *new;
	PUZZLE *hptr, *vptr, *prevcol, **prevrow;
	register int i,j;

#ifdef DEBUG
	printf("xw_copypuzz(%#010x): Starting up..\n", (UINT)old);
#endif /* DEBUG */

	new = (PUZZHEAD *)xw_malloc(sizeof(PUZZHEAD));
	memcpy(new, old, sizeof(PUZZHEAD));
	new->ph_lastword = NULL;

	prevrow = (PUZZLE **)xw_malloc(old->ph_numcols * sizeof(PUZZLE *));

	hptr = vptr = old->ph_puzzle;
	prevcol = NULL;
#ifdef DEBUG
	PRINT2(d, (UINT)old->ph_numrows,(UINT)old->ph_numcols);
#endif /* DEBUG */
	for(j=0; j < old->ph_numrows; j++)
	  {
	    for(i=0; i < old->ph_numcols; i++)
	      {
		PUZZLE *temp;
		
		temp = (PUZZLE *)xw_malloc(sizeof(PUZZLE));

		memcpy(temp, vptr, sizeof(PUZZLE));

		if (i EQ 0)
		  {
		    if (j EQ 0)
		      {
			new->ph_puzzle = temp;
		      }
		  }
		else
		  {
		    temp->pz_left = prevcol;
		    prevcol->pz_right = temp;
		  }
		if (j != 0)
		  {
		    temp->pz_up = prevrow[i];
		    prevrow[i]->pz_down = temp;
		  }
#ifdef DEBUG
		PRINT2(d, (UINT)i, (UINT)j);
		PRINT3(010x, (UINT)prevrow[i], (UINT)prevcol,
		       (UINT)temp);
#endif /* DEBUG */
		vptr = vptr->pz_right;
		prevrow[i] = prevcol = temp;

	      }

	    hptr = vptr = hptr->pz_down;
	}
	free(prevrow);
	return new;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_KILLPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_killpuzz(PUZZHEAD *puzzle_header)
{
	PUZZLE *hptr, *vptr, *temp;


#ifdef DEBUG
	printf("xw_killpuzz(%#010x): Starting up..\n", (UINT)puzzle_header);
#endif /* DEBUG */

	hptr = puzzle_header->ph_puzzle;
	while (hptr != NULL)
	{
		vptr = hptr;
		hptr = hptr->pz_down;
		while(vptr != NULL)
		{
			temp = vptr;
			vptr = vptr->pz_right;
			free(temp);
		}
	}
	free(puzzle_header);
}

/* End of /home/franx/xword/xw_initpuzz.c */



