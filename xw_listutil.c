/* /home/franx/xword/xw_listutil.c Mon02Feb2004 {fcG} */

// MODIFICATION HISTORY
// When		Who	What
// Wed08Jul2020 {fcG}	64-bit debug code.

#include <stdio.h>
#include "defs.h"
#include "xword.h"

static WORDLIST *nextword = &xw_start;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_POPLIST		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

WORDLIST *xw_poplist()
{
    TEST(PRINT1(#018lx, (ULONG)nextword));
    if(nextword != NULL)
      {
	WORDLIST *temp;
	
	while (nextword != NULL && nextword->wl_status != UNUSED)
	  {
	    nextword = nextword->wl_next;
	  }
	if (nextword != NULL)
	  {
#ifdef DEBUG
	    printf("poplist: returned %#018lx which points to %s\n",
		   (ULONG)nextword, nextword->wl_word);
#endif /* DEBUG */
	    temp = nextword;
	    nextword = nextword->wl_next;
	    return temp;
	  }
	else
	  {
	    return NULL;
	  }
      }
    else
      {
/* 	      xw_error(SV_ERROR,"poplist: The list is exhausted"); */
	return NULL;
      }
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_PUSHLIST		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_pushlist(struct wordlist *ref)
{
	  register WORDLIST *cur, *ptr;

	  cur = nextword;

#ifdef DEBUG
	  printf("pushlist: putting %#018lx which points to %s back on\n",
		  (ULONG)ref, ref->wl_word);
	  ptr = xw_start.wl_next;
	  while (ptr != NULL)
	    {
	      printf("%#018lx\t[%s]\t%s\n", (ULONG)ptr, ptr->wl_word,
		DECODE(ptr->wl_status));
	      ptr = ptr->wl_next;
	    }
#endif /* DEBUG */
 
	  ref->wl_status = UNUSED;
	  ref->wl_xpos = ref->wl_ypos = 0;

	  if (cur EQ NULL)
	    {
	      nextword = ref;
	    }
	  else
	    {
	      while(cur != ref && cur != NULL)
		{
  /* PRINT3(#018lx, (ULONG)cur, (ULONG)cur->wl_next, (ULONG)nextword); */
  /* PRINT1(#018lx, (ULONG)cur->wl_prev); */
		  
		  cur = cur->wl_prev;
#ifdef DEBUG
		  printf("pushlist: cur = %#018lx\t\n", (ULONG)cur);
#endif /* DEBUG */
		  if(cur EQ NULL)
		    {
/* 		      xw_error(SV_ERROR, */
/* 		       "pushlist: %#018lx [%s] was not in wordlist", */
/* 		       (ULONG)ref, ref->wl_word); */

		      break;
		    }
		}
	      if (cur != NULL)
		{
		  nextword = cur;
		}
	    }

	  TEST(PRINT1(#018lx, (ULONG)nextword));

	  /* ref->wl_next = nextword; */
	  /* cur->wl_next = nextword = ref; */

}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_RESTARTLIST		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_restartlist()
{

/*  This resets the list. */
	  WORDLIST *ref;

#ifdef DEBUG
	  printf("xw_restartlist(): Starting up..\n");
#endif

	  nextword = &xw_start;
	  ref = nextword->wl_next;
	  while (ref != NULL)
	    {

#ifdef DEBUG
		printf("xw_restartlist(): ref = %#018lx[%s]\n",
		(ULONG)ref, ref->wl_word);
#endif

	      ref->wl_status = UNUSED;
	      ref = ref ->wl_next;
	    }
}

/* End of /home/franx/xword/xw_listutil.c */
