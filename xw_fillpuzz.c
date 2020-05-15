/* /home/franx/xword/xw_fillpuzz.c Fri28Jul2006 {fcG} */

#include "xword.h"

typedef struct spot
{
  char sp_letter;
  int  sp_pos;
} SPOT;

typedef struct wordhole
{
  int wh_key, wh_rownum, wh_colnum;
  STATUS wh_status;
  struct wordhole *wh_next;
  struct spot wh_spot[WORDLENGTH];
 
} WORDHOLE;

WORDHOLE xw_whstart =
  {
    2020, UNDEFINED, UNDEFINED, UNUSED, NULL,
  };

WORDHOLE *xw_inserthole(int spots, int letters, WORDHOLE *wh, PUZZLE *pzptr, STATUS stat) 
{
  register WORDHOLE *wh_ptr = &xw_whstart;

  wh->wh_key = (spots * 100) + letters;
  wh->wh_status = stat;
  for(EVER)
    {
      if((wh_ptr->wh_next EQ NULL) || (wh_ptr->wh_next->wh_key < wh->wh_key))
	{
	  wh->wh_next = wh_ptr->wh_next;
	  wh_ptr->wh_next = wh;
	  break;
	}
      else
	{
	  wh_ptr = wh_ptr->wh_next;
	}
    }
  return (WORDHOLE *)xw_malloc(sizeof(WORDHOLE));
}

void xw_searchrank(PUZZLE *pzptr, STATUS stat, int length)
{
  register int i, numletters, numspots;
  WORDHOLE *wh_ptr;

  numletters = numspots = 0;
  wh_ptr = (WORDHOLE *)xw_malloc(sizeof(WORDHOLE));
  wh_ptr->wh_rownum = pzptr->pz_rownum;
  wh_ptr->wh_colnum = pzptr->pz_colnum;

  for(i = 0; i < length; i++)
    {
      if (pzptr->pz_letter EQ EOWORD)
	{
	  if (numspots != 0 && numspots != numletters)
	    {
	      wh_ptr = xw_inserthole(numspots, numletters, wh_ptr, pzptr, stat);
	    }
	  else
	    {
	      register int j;
	      
/* 	      PRINT1(d, sizeof(SPOT)); */
/* 	      PRINT2(d, sizeof(wh_ptr->wh_spot), SZ_TABLE(wh_ptr->wh_spot)); */

	      for(j = 0; j < SZ_TABLE(wh_ptr->wh_spot); j++)
		{
		  wh_ptr->wh_spot[j].sp_letter = '\0';
		  wh_ptr->wh_spot[j].sp_pos = 0;
		}
	    }
	  if (stat EQ ACROSS)
	    {
	      wh_ptr->wh_rownum = pzptr->pz_rownum;
	      wh_ptr->wh_colnum = pzptr->pz_colnum+1;
	    }
	  else
	    {
	      wh_ptr->wh_rownum = pzptr->pz_rownum+1;
	      wh_ptr->wh_colnum = pzptr->pz_colnum;
	    }
	  numletters = numspots = 0;
	}
      else
	{
/* 	  TEST(PR(#06x,pzptr->pz_letter)); */
/* 	  TEST(PR(c,pzptr->pz_letter)); */

	  if (pzptr->pz_letter EQ '\0' || pzptr->pz_letter EQ ' ')
	    {
	      numletters++;
	    }
	  else
	    {
	      wh_ptr->wh_spot[numspots].sp_pos = ++numletters;
	      wh_ptr->wh_spot[numspots++].sp_letter = pzptr->pz_letter;

/* 	      TEST(PR(c,pzptr->pz_letter)); */
/* 	      TEST(PRINT2(d,numletters,numspots)); */
	    }
	}
      if (stat EQ ACROSS)
	{
	  pzptr = pzptr->pz_right;
	}
      else
	{
	  pzptr = pzptr->pz_down;
	}
    }
  if (numspots != 0 && numspots != numletters)
    {
      wh_ptr = xw_inserthole(numspots, numletters, wh_ptr, pzptr, stat);
    }
  else
    {
      free(wh_ptr);
    }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FINDGAPS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_findgaps(PUZZHEAD *ph)
{
  PUZZLE *hptr, *vptr;
  register int i;

  hptr = ph->ph_puzzle;
  if(ph->ph_numrows != 1)
    {
      hptr = hptr->pz_down;
    }
  vptr = hptr->pz_right;
  for(i = 0; i < ph->ph_numcols - 2; i++)
  {
    xw_searchrank(vptr, DOWN, ph->ph_numrows-2);
    vptr = vptr->pz_right;
  }
  vptr = hptr->pz_right;
  for(i = 0; i < ph->ph_numrows - 2; i++)
  {
    xw_searchrank(vptr, ACROSS, ph->ph_numcols-2);
    vptr = vptr->pz_down;
  }
}	

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FILLPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

PUZZHEAD *xw_fillpuzz(PUZZHEAD *ph)
{
  register WORDHOLE *wh_ptr = &xw_whstart;
  
  xw_findgaps(ph);
  
#ifdef DEBUG

  WHEN; WHERE, nL;
  
  wh_ptr =wh_ptr->wh_next;
  while (wh_ptr != NULL)
    {
      register int i,j, k;
      char buf[SZ_STRING+1];
      
      printf("%d %s (%d,%d) ", wh_ptr->wh_key, DECODE(wh_ptr->wh_status),
	     wh_ptr->wh_colnum,  wh_ptr->wh_rownum);
      for(i=0; i < wh_ptr->wh_key / 100; i++)
	{
	  printf("\"%c\"[%#04x]->%d ", wh_ptr->wh_spot[i].sp_letter,
		 wh_ptr->wh_spot[i].sp_letter, wh_ptr->wh_spot[i].sp_pos);
	}
      for(j = k = 0; j < wh_ptr->wh_key % 100; j++)
	{
	  if (wh_ptr->wh_spot[k].sp_pos - 1 EQ j)
	    {
	      buf[j] = wh_ptr->wh_spot[k++].sp_letter;
	    }
	  else
	    {
		  buf[j] = '?';
	    }
	}
      buf[j] = '\0';
      printf("%s\n", buf);
      wh_ptr =wh_ptr->wh_next;
    }

#endif /* DEBUG */

  return ph;
}	

/* End of /home/franx/xword/xw_fillpuzz.c */
