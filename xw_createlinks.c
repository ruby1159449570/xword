/* /home/franx/xword/xw_createlinks.c Mon02Feb2004 {fcG} */

/*  #include <stdio.h> */
/*  #include <stdlib.h> */
/*  #include <ctype.h> */
/*  #include "defs.h" */
#include "xword.h"

WORDLIST	*letterlink[NUMLETTERS] =
{
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL
};

#ifdef SNARK
int	*freqletters[NUMLETTERS];
#endif /* SNARK */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_CREATELINKS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/* This routine sets up rings to words that have the sameletter. */

void xw_createlinks()
{
	register WORDLIST *ref, *prev = NULL;
	register int i,j;

	ref = xw_start.wl_next;
	while(ref != NULL)
	{
	  if (prev != NULL)
	  {
	    ref->wl_prev = prev;
	  }
	  prev = ref;

	  for(i = 0; i < ref->wl_numchar; i++)
	    {
	      j = ORD(ref->wl_word[i]);
	      ref->wl_sameletter[i] = letterlink[j];
	    }
	  for(i = 0; i < ref->wl_numchar; i++)
	    {
	      j = ORD(ref->wl_word[i]);
	      letterlink[j] = ref;
	    }
	  ref = ref->wl_next;
	}

	/*  Now link the head to the tail. */

	ref = xw_start.wl_next;
	while(ref != NULL)
	  {
#ifdef DEBUG
	    printf("xw_createlinks: ref = %#010x\n", (UINT)ref);
#endif /* DEBUG */

	    for(i = 0; i < ref->wl_numchar; i++)
	      {
		j = ORD(ref->wl_word[i]);
		if (ref->wl_sameletter[i] EQ NULL
		    && ref != letterlink[j])
		  {
		    ref->wl_sameletter[i] = letterlink[j];
#ifdef DEBUG
		    printf("xw_createlinks: %c evaluated to %2d, sameletter is %#010x\n",
			   ref->wl_word[i],j,(UINT)ref->wl_sameletter[i]);
#endif /* DEBUG */
		  }
	      }
	    ref = ref->wl_next;
	  }

	/*  Now check for words that do not have letters in other words. */

	ref = xw_start.wl_next;
	while(ref != NULL)
	  {
	    for(i = 0; i < ref->wl_numchar; i++)
	      {
		if(ref->wl_sameletter[i] != NULL)
		  {
		    break;
		  }
		if(i EQ ref->wl_numchar - 1)
		  {
		    xw_error(SV_FATAL,"%s does not have letters in any other words",
			     ref->wl_word);
		  }
	      }
	    ref = ref->wl_next;
	  }
}

/* End of /home/franx/xword/xw_createlinks.c */
