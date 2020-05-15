/* /home/franx/xword/xw_tryword.c Tue10Feb2004 {fcG} */

#include <stdio.h>
#include "xword.h"

BOOLEAN	spotfound = FALSE;
int	sizeincrease = UNDEFINED;
/* WORDFIT	ret = { 0, 0, 0, UNUSED}; */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_TRYWORD		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_tryword(PUZZHEAD *ph, WORDLIST *w, int i, WORDLIST *x)
{
	int j;
	int	newx, newy;
	PUZZLE	*ref;

#if DEBUG
	printf("xw_tryword: w = %s, i = %d, x = %s[%s]\n",
		w->wl_word, i, x->wl_word, DECODE(x->wl_status));
#endif /* DEBUG */

	if (x->wl_status != UNUSED)
	  {
	    for(j=0; j < x->wl_numchar; j++)
	      {
		if(x->wl_word[j] EQ w->wl_word[i])
		  {
		    spotfound = TRUE;
		    sizeincrease = UNDEFINED;
		    if(x->wl_status EQ ACCROSS)
		      {
			ref = xw_pointpuzz(ph, x->wl_xpos + j, x->wl_ypos);
			newx = x->wl_xpos + j;
			newy = x->wl_ypos - i;
			xw_feelup(ph, w, i-1, ref->pz_up);
			if (spotfound)
			  {
			    xw_feeldown(ph, w, i+1, ref->pz_down);
			  }
		      }
		    else
		      {
			ref = xw_pointpuzz(ph, x->wl_xpos, x->wl_ypos + j);
			newx = x->wl_xpos - i;   ;
			newy = x->wl_ypos + j;
			xw_feelleft(ph, w, i - 1, ref->pz_left);
			if (spotfound)
			  {
			    xw_feelright(ph, w, i + 1, ref->pz_right);
			  }
		      }
		    if (spotfound && (sizeincrease < xw_ret.wf_size))
		      {
			xw_ret.wf_size = sizeincrease;
			xw_ret.wf_x = newx;
			xw_ret.wf_y = newy;
			xw_ret.wf_status = INV(x->wl_status);
		      }
		  }
	      }
	  }
	for(j = 0; x->wl_word[j] != w->wl_word[i]
	      && x->wl_word != '\0'; j++)
	  {
	    ; /*  Do NOTHING!!! */
	  }
	if (x->wl_sameletter[j] != w && x->wl_sameletter[j] != NULL)
	  {
	    xw_tryword(ph, w, i, x->wl_sameletter[j]);
	  }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FEELUP		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_feelup(PUZZHEAD *ph, WORDLIST *w, int i, PUZZLE *p)
{
#if DEBUG
	printf("xw_feelup: w->word[i] = %c, i = %d, p = %#010x\n",
		w->wl_word[i], i, (UINT)p);
#endif /* DEBUG */

	if (p EQ NULL)
	{
		sizeincrease = (i + 2) * (ph->ph_numcols - 2);
	}
	else if (i < 0)
	{
	  if ((p->pz_letter != EOWORD) && (p->pz_letter != '\0'))
	    {
	      spotfound = FALSE;
	    }
	  else
	    {
	      sizeincrease = 0;
	    }
	}
	else if(p->pz_letter EQ EOWORD)
	{
		spotfound = FALSE;
	}
	else if(p->pz_letter EQ w->wl_word[i]
		|| (p->pz_letter EQ '\0'
		    && (p->pz_left->pz_letter EQ '\0'
			|| p->pz_left->pz_letter EQ EOWORD)
		    && (p->pz_right->pz_letter EQ '\0'
			|| p->pz_right->pz_letter EQ EOWORD)))
	{
		xw_feelup(ph, w, i - 1, p->pz_up);
	}
	else
	{
		spotfound = FALSE;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FEELDOWN		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_feeldown(PUZZHEAD *ph, WORDLIST *w, int i, PUZZLE *p)
{
#if DEBUG
	printf("xw_feeldown: w->word[i] = %c, i = %d, p = %#010x\n",
		w->wl_word[i], i,  (UINT)p);
#endif /* DEBUG */

	if (p EQ NULL)
	{
	  sizeincrease += (w->wl_numchar - i) * (ph->ph_numcols - 2);
	}
	else if (i >= w->wl_numchar)
	  {
	    if ((p->pz_letter != EOWORD) && (p->pz_letter != '\0'))
	      {
		spotfound = FALSE;
	      }
	  }
	else if (p->pz_letter EQ EOWORD)
	  {
	    spotfound = FALSE;
	  }
	else if (p->pz_letter EQ w->wl_word[i]
		 || (p->pz_letter EQ '\0'
		     && (p->pz_left->pz_letter EQ '\0'
			 || p->pz_left->pz_letter EQ EOWORD)
		     && (p->pz_right->pz_letter EQ '\0'
			 || p->pz_right->pz_letter EQ EOWORD)))
	  {
	    xw_feeldown(ph, w, i + 1, p->pz_down);
	  }
	else
	  {
	    spotfound = FALSE;
	  }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FEELLEFT		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_feelleft(PUZZHEAD *ph, WORDLIST *w, int i, PUZZLE *p)
{
#if DEBUG
	printf("xw_feelleft: w->word[i] = %c, i = %d, p = %#010x\n",
		w->wl_word[i], i,  (UINT)p);
#endif /* DEBUG */

	if (p EQ NULL)
	  {
		sizeincrease = (i + 2) * (ph->ph_numrows - 2);
	  }
	else if (i < 0)
	  {
	    if (p->pz_letter != '\0' && p->pz_letter != EOWORD)
	      {
		spotfound = FALSE;
	      }
	    else
	      {
		sizeincrease = 0;
	      }
	  }
	else if (p->pz_letter EQ EOWORD)
	  {
	    spotfound = FALSE;
	  }
	else if (p->pz_letter EQ w->wl_word[i]
		 || (p->pz_letter EQ '\0'
		     && (p->pz_up->pz_letter EQ '\0'
			 || p->pz_up->pz_letter EQ EOWORD)
		     && (p->pz_down->pz_letter EQ '\0'
			 || p->pz_down->pz_letter EQ EOWORD)))
	{
	  xw_feelleft(ph, w, i - 1, p->pz_left);
	}
		 else
	{
	  spotfound = FALSE;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FEELRIGHT		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	xw_feelright(PUZZHEAD *ph, WORDLIST *w, int i, PUZZLE *p)
{
#if DEBUG
        printf("xw_feelright: w->word[i] = %c, i = %d, p = %#010x\n",
		w->wl_word[i], i,  (UINT)p);
	PRINT1(d, w->wl_numchar);
#endif /* DEBUG */

	if (p EQ NULL)
	{
		sizeincrease += (w->wl_numchar - i) * (ph->ph_numcols - 2);
	}
	else if (i >= w->wl_numchar)
	{
	  if (p->pz_letter != '\0' && p->pz_letter != EOWORD)
	    {
	      spotfound = FALSE;
	    }
	}
	else if (p->pz_letter EQ EOWORD)
	  {
	    spotfound = FALSE;
	  }
	else if (p->pz_letter EQ w->wl_word[i]
		 ||(p->pz_letter EQ '\0'
		    && (p->pz_up->pz_letter EQ '\0'
			|| p->pz_up->pz_letter EQ EOWORD)
		    && (p->pz_down->pz_letter EQ '\0'
			|| p->pz_down->pz_letter EQ EOWORD)))
	{
		xw_feelright(ph, w, i + 1, p->pz_right);
	}
	else
	{
		spotfound = FALSE;
	}
}

/* End of /home/franx/xword/xw_tryword.c  */
