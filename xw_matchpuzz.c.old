/* /home/franx/xw_matchpuzz.c Wed21Jun2006 {fcG} */

// MODIFICATION HISTORY
// When		Who	What
// Wed08Jul2020 {fcG}	64-bit debug code.

#include <stdio.h>
/* #define extern */
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		xwmatchpuzz()		#
#returns TRUE if both puzzles identical	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

BOOLEAN xw_matchpuzz(PUZZHEAD *puzz1, PUZZHEAD *puzz2)
{
    PUZZLE *h1ptr, *h2ptr, *v1ptr, *v2ptr;
#ifdef	DEBUG
    ULONG checksum = 0;

    WHERE, WHEN;;
    printf(" xw_matchpuzz()::");
    PRINT2(#018lx, (ULONG)puzz1, (ULONG)puzz2);
    PRINT2(d, (UINT)puzz1->ph_checksum, (UINT)puzz2->ph_checksum);
#endif	/* DEBUG */
    if((puzz1->ph_numrows != puzz2->ph_numrows)
       || (puzz1->ph_numcols != puzz2->ph_numcols)
       || (puzz1->ph_checksum != puzz2->ph_checksum))
      {
	return FALSE;
      }
    h1ptr = puzz1->ph_puzzle;
    h2ptr = puzz2->ph_puzzle;
    if (puzz1->ph_numwords != 1)
      {
	h1ptr = h1ptr->pz_down;
      }
    if (puzz2->ph_numwords != 1)
      {
	h2ptr = h2ptr->pz_down;
      }
    while(h1ptr != NULL)
      {
	v1ptr = h1ptr->pz_right;
	v2ptr = h2ptr->pz_right;
	while(v1ptr != NULL)
	  {
	    if (v1ptr->pz_letter != v2ptr->pz_letter)
	      {
		return FALSE;
	      }
#ifdef	DEBUG
	    if ((v1ptr->pz_letter != EOWORD)
		&& (v1ptr->pz_letter != '\0'))
	      {
		TEST(PR(c, v1ptr->pz_letter));
		TEST(PRINT1(d, v1ptr->pz_letter));
		checksum += v1ptr->pz_letter;
	      }
#endif	/* DEBUG */

	    v1ptr = v1ptr->pz_right;
	    v2ptr = v2ptr->pz_right;
	  }
	h1ptr = h1ptr->pz_down;
	h2ptr = h2ptr->pz_down;
      }
    TEST(PRINT1(d, (UINT)checksum));
    return TRUE;
}

#ifdef	SNARK

WORDLIST word1 = { 8, "ELEPHANT", "", UNUSED, UNDEFINED, UNDEFINED, 0, 0, NULL };
WORDLIST word2 = { 8, "ANTEATER", "", UNUSED, UNDEFINED, UNDEFINED, 0, 0, NULL };
WORDLIST word3 = { 3, "PIG", "", UNUSED, UNDEFINED, UNDEFINED, 0, 0, NULL };

int main(int argc, char *argv[])
{
  PUZZHEAD *ph1, *ph2;

  ph1 = xw_initpuzz(&word1);
  ph2 = xw_initpuzz(&word1);
  xw_putwordin(ph1, &word2, DOWN, 5, 0);
  xw_putwordin(ph2, &word2, DOWN, 5, 0);
  xw_printpuzz(ph1);
  xw_printpuzz(ph2);
  if (xw_matchpuzz(ph1, ph2))
    {
      printf("xw_matchpuzz() returns TRUE\n");
    }
  else
    {
      printf("xw_matchpuzz() returns FALSE\n");
    } 
  xw_putwordin(ph1, &word3, DOWN, 3, 0);
  xw_printpuzz(ph1);
  xw_printpuzz(ph2);
  if (xw_matchpuzz(ph1, ph2))
    {
      printf("xw_matchpuzz() returns TRUE\n");
    }
  else
    {
      printf("xw_matchpuzz() returns FALSE\n");
    } 
}

#endif	/* SNARK */

/* End of /home/franx/xw_matchpuzz.c  */
