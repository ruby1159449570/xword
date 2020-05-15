/* /home/franx/xword/xw_printpuzz.c Tue03Feb2004 {fcG} */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>
#include "defs.h"
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_PRINTPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_printpuzz(PUZZHEAD *puzz_hdr)
{
	  PUZZLE	*hptr, *vptr;
	  register	int i;

#ifndef DEBUG
	  printf("\033[2J\033[0;0H");
#else
	  printf("xw_printpuzz(%#010x)\n", (UINT)puzz_hdr);
#endif  /*  DEBUG */

	  TEST(WHEN;nL);
	  puzz_hdr->ph_numletters = 0;
	  hptr = puzz_hdr->ph_puzzle;

	  if(puzz_hdr->ph_numwords != 1)
	    {
	      hptr = hptr->pz_down;
	    }
	  vptr = hptr->pz_right;
#ifdef DEBUG
	  printf("   ");
	  while(vptr != NULL)
	    {
	      printf("%2d", vptr->pz_colnum);
	      vptr = vptr->pz_right;
	    }
	  vptr = hptr->pz_right;
	  printf("\n    ");
#endif /* DEBUG */
	  printf("+");
	   for(i = 0; i < puzz_hdr->ph_numcols - 2; i++)
	    {
	      printf("-+");
	    }
	  printf("\n");

	  while(((hptr != NULL) && (puzz_hdr->ph_numwords EQ 1))
		|| ((hptr->pz_down != NULL) && (puzz_hdr->ph_numwords != 1)))
	    {
#ifdef DEBUG

/*            PRINT2(010x, (UINT)hptr, (UINT)vptr); */

	      printf("%4d", hptr->pz_rownum);
#endif /* DEBUG */
	      printf("|");

	      while(vptr->pz_right != NULL)
		{
#ifdef DEBUG
		  if(vptr->pz_letter == EOWORD)
		    {
		      printf("%c|", EOWORD);
		    }
		  else
#endif /* DEBUG */
		    if((vptr->pz_letter != '\0') && (vptr->pz_letter != EOWORD))
		      {
			puzz_hdr->ph_numletters++;
#ifdef DEBUG
			printf("%c|", vptr->pz_letter);
#else
			printf("\033[0;4m%c|\033[0m", vptr->pz_letter);
#endif /* DEBUG */
		      }
		    else
		      {
#if DEBUG
			printf(" |");
#else
			printf("\033[40m  \033[0m");
#endif /* DEBUG */
		      }
		  vptr = vptr->pz_right;
		}
	      printf("\n");
	      hptr = hptr->pz_down;
	      if (hptr != NULL)
		{
		  vptr = hptr->pz_right;
		}
	      else
		{
		  break;
		}
	    }
	  TEST(PRINT1(d, puzz_hdr->ph_numcols));
#ifdef DEBUG
	  printf("  ");
#endif /* DEBUG */
	  printf("+");
	  for(i = 0; i < puzz_hdr->ph_numcols - 2; i++)
	    {
	      printf("-+");
	    }
	  printf("\n");

	  if (puzz_hdr->ph_numwords != 1)
	    {
	      puzz_hdr->ph_compact = puzz_hdr->ph_numletters/(((float)puzz_hdr->ph_numrows-2.0)
			      * ((float)puzz_hdr->ph_numcols - 2.0)) * 100.0;
	      printf("Puzzle Compactness: %6.2f%%\n", puzz_hdr->ph_compact);
	    }
	  printf("Time taken to insert %d words: %6.2f secs.\n",
		 puzz_hdr->ph_numwords, difftime(time(NULL), xw_starttime));
#if DEBUG
	  PR(s, puzz_hdr->ph_lastword->wl_word);
	  PRINT2(010x, (UINT)puzz_hdr->ph_lastword, (UINT)puzz_hdr->ph_prevpuzz);
#endif /* DEBUG */

}

/* End of /home/franx/xword/xw_printpuzz.c */
