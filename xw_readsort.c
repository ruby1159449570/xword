/* /home/franx/xword/xw_readsort.c Mon02Feb2004 {fcG} */

// MODIFICATION HISTORY
// When		Who	What
// Wed08Jul2020 {fcG}	64-bit debug code.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "defs.h"
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		READSORT		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int xw_readsort()
{
  FILE	*fp;
  int	c;
  int return_code = 0, linenum = 0;
  BOOLEAN alpha_word, printable_clue;
  register	int i;
  register	WORDLIST	*ref, *new;
  
  fp = fopen(xw_inputfile, "r");
  if (fp EQ NULL)
    {
      xw_error(SV_ERROR, "Error opening \"%s\"", xw_inputfile);
    }
  c = getc(fp);
  while(c != EOF)
    {
      new = (WORDLIST *)xw_malloc(sizeof(WORDLIST));
      i = 0;
      new->wl_numchar = 0;
      linenum++;
      alpha_word = printable_clue = TRUE;
      while(c != ' ' && c != '\n' && c != '\t' && c != EOF)
	{
	  if(!isalpha(c))
	    {
	      alpha_word = FALSE;
	    }
	  c = islower(c) ? toupper(c) : c;
#ifdef DEBUG
	  printf("readsort: word[%d] is  %c\n", i, c);
#endif /* DEBUG */
	  new->wl_word[i++] = c;
	  new->wl_numchar++;
	  xw_totletters++;
	  if (i > WORDLENGTH)
	    {
	      new->wl_word[i] = '\0';
	      xw_error(SV_INFO,"line %d: %s... too long for word (limit %d)",
		       linenum,new->wl_word, WORDLENGTH);
	      return_code++;
	      while(c != ' ' && c != '\n' && c != '\t' && c != EOF)
		{
		  c = getc(fp);
		}
	      break;
	    }
	  c = getc(fp);
	}
      new->wl_word[i] = '\0';
      if(!alpha_word)
	{
	  xw_error(SV_WARNING,"line %d: Words must be alphabetic(\"%s\")",
		   linenum,new->wl_word);
	  return_code++;
	}
      if (i < 3)
	{
	  xw_error(SV_INFO,"line %d: %s not long enough for word",linenum,new->wl_word);
	  return_code++;
	  /* break */;
	}
      xw_totwords++;

/* Input the Clue. */
      
      i = 0;
      if(c EQ ' ' || c EQ '\t')
	{
	  c = getc(fp);
	}
      while((c != EOF) && (c != '\n'))
	{
	  new->wl_clue[i++] = c;
	  if(!isprint(c) && c != '\t')
	    {
	      printable_clue = FALSE;
	    }
	  if(i > CLUELENGTH)
	    {
	      if (CLUELENGTH > 30)
		{
		  new->wl_clue[30] = '\0';
		}
	      else
		{
		  new->wl_clue[i] = '\0';
		}
	      xw_error(SV_INFO, "line %d: %30s... too long for clue (limit %d)",
		       linenum, new->wl_clue, CLUELENGTH);
	      return_code++;
	      while(c != '\n')
		{
		  c = getc(fp);
		}
	      break;
	    }
	  c = getc(fp);
	}
      new->wl_clue[i] = '\0';
      if (!printable_clue)
	{
	  xw_error(SV_WARNING,"line %d: Clues must be printable(\"%s\")",
		   linenum,new->wl_clue);
	  return_code++;
	}
      if(i < 2)
	{
	  xw_error(SV_INFO, "line %d: clue for %s not long enough for clue",linenum,new->wl_word);
	  return_code++;
	}
      
      c = getc(fp);
#ifdef DEBUG
      printf("readsort: inserting  %s of length %d\n",
	     new->wl_word, new->wl_numchar);
#endif /* DEBUG */

/* Now, sort according to wordlength (ie. numchar field). */
			
      ref = &xw_start;
      for(EVER)
	{
	  if ((ref->wl_next EQ 0) || (ref->wl_next->wl_numchar < new->wl_numchar))
	    {
#ifdef DEBUG
	      printf("readsort: inserting after %s of length %d\n",
		     ref->wl_word, ref->wl_numchar);
#endif /* DEBUG */

/* Put it in. */
	      new->wl_next = ref->wl_next;
	      ref->wl_next = new;
	      break;
	    }
	  else
	    {
#ifdef DEBUG
	      printf("readsort: moving past %s of length %d\n",
		     ref->wl_word, ref->wl_numchar);
#endif /* DEBUG */

/* Move along */

	      ref = ref->wl_next;
	    }
	}
    }
  if (xw_totwords > NUMWORDS)
	{
	xw_error(SV_INFO,
	"line %d: Word limit of %d exceeded; %d words encountered",
	linenum,NUMWORDS, xw_totwords);
	return_code++;
	}
  fclose(fp);
  return return_code;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		KILLIST			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_killlist()
{
  WORDLIST	*ref, *tmp;

#ifdef DEBUG
  printf("killist: STARTING UP\n");
#endif /* DEBUG */

  ref = xw_start.wl_next;
  while(ref != 0)
    {
      tmp = ref->wl_next;
      TEST(PR(#018lx, (ULONG)ref));
      TEST(PRINT2(s, ref->wl_word, ref->wl_clue));
      free((char *)ref);
      ref = tmp;
    }
}
	
/* End of /home/franx/xword/xw_readsort.c */
