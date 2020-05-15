/* /home/franx/xword/xw_buildpuzz.c Wed04Feb2004 {fcG} */

#include <stdio.h>
#include "xword.h"

/* WORDFIT best = { UNDEFINED, 0, 0, UNUSED}; */
/* WORDFIT ret  = { UNDEFINED, 0, 0, UNUSED}; */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_BUILDPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

WORDLIST *xw_buildpuzz(PUZZHEAD *ph, WORDLIST *word)
{

/*  	register WORDLIST *word; */

	register int i;
	BOOLEAN  couldntbuild;
/*  	WORDLIST *failure; */

	couldntbuild = FALSE;
	if (word != NULL)
	  {
	    xw_best.wf_size = UNDEFINED;
	    xw_ret.wf_size = UNDEFINED;
	    for (i=0; i < word->wl_numchar; i++)
	      {
		if(word->wl_sameletter[i] != NULL)
		  {
		    TEST(PRINT1(#010x, (UINT)word->wl_sameletter[i]));
		    TEST(PRINT1(s, word->wl_sameletter[i]->wl_word));
		    xw_tryword(ph, word, i, word->wl_sameletter[i]);
		    if(xw_best.wf_size > xw_ret.wf_size)
		      {
			xw_best.wf_size = xw_ret.wf_size;
			xw_best.wf_x = xw_ret.wf_x;
			xw_best.wf_y = xw_ret.wf_y;
			xw_best.wf_status = xw_ret.wf_status;
		      }
		  }
	      }
#ifdef DEBUG
	    printf("xw_buildpuzz(): tryword returns ");
	    PRINT3(d, xw_best.wf_size, xw_best.wf_x, xw_best.wf_y);
	    printf("xw_best.wf_status = %d[%s]\n", xw_best.wf_status,
		   DECODE(xw_best.wf_status));
#endif /* DEBUG */
	    ph->ph_compact = (ph->ph_numrows - 2) * (ph->ph_numcols - 2) + xw_best.wf_size;
#if DEBUG
	      PRINT2(6f, (double)ph->ph_compact, (double)xw_puzz_compact_req);
#endif /* DEBUG */
	    if(xw_best.wf_size EQ UNDEFINED)

/*|| ((ph->ph_numrows - 2) * (ph->ph_numcols - 2) + xw_best.wf_size */
/* > ((xw_totletters - 2) * (xw_totwords + 2)) * 100 / xw_puzz_compact_req)) */

	      {
		couldntbuild = TRUE;
/*  		    goto GETOUTOFHERE; */
	      }
	    else
	      {
		xw_putwordin(ph, word, xw_best.wf_status, xw_best.wf_x, xw_best.wf_y);

		xw_printpuzz(ph);
	  
	      }
	  }

/*  GETOUTOFHERE: */

/*  	for(;index > 0; index--) */
/*  	{ */
/*  	  xw_pushlist(failure[index]); */
/*  	} */
	if(couldntbuild)
	  {
	/*      xw_error(SV_INFO, "xw_buildpuzz(): couldn't find a spot for %s\n", */
/*  		     word); */
	    return word;
	}
	else
	{
#ifdef DEBUG
	  printf("Success!!!\n");
#endif /* DEBUG */
	  return NULL;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_PUTWORDIN		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_putwordin(PUZZHEAD *p, WORDLIST *w, STATUS s, int x, int y)
{
	int i;
	PUZZLE	*ref;

#ifdef	DEBUG
	printf("xw_putwordin: putting %s in at s = %d[%s], x = %d, y = %d\n",
                w->wl_word, s, DECODE(s), x, y);
#endif	/* DEBUG*/

	if (s EQ ACCROSS)
	{
		xw_createcolumn(p, x-1);
		xw_createcolumn(p, x+w->wl_numchar);
		ref = xw_pointpuzz(p, x-1, y);
		ref->pz_letter = EOWORD;
		ref = ref->pz_right;
		ref->pz_ofaccrossword = w;
	}
	else
	{
		xw_createrow(p, y-1);
		xw_createrow(p, y+w->wl_numchar);
		ref = xw_pointpuzz(p, x, y-1);
		ref->pz_letter = EOWORD;
		ref = ref->pz_down;
		ref->pz_ofdownword = w;
	}
	for (i=0; i<w->wl_numchar; i++)
	{
		ref->pz_letter = w->wl_word[i];
		if (s EQ ACCROSS)
		{
			ref = ref->pz_right;
		}
		else
		{
			ref = ref->pz_down;
		}
	}
	p->ph_numwords++;
	p->ph_lastword = w;
	ref->pz_letter = EOWORD;
	w->wl_status = s;
	w->wl_xpos = x;
	w->wl_ypos = y;
}

/* End of /home/franx/xword/xw_buildpuzz.c */
