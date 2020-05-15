/* /home/franx/xword/xw_listutil.c Mon02Feb2004 {fcG} */

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

	  TEST(PRINT1(010x, (UINT)nextword));

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
		      printf("poplist: returned %#010x which points to %s\n",
		      (UINT)nextword, nextword->wl_word);
#endif /* DEBUG */
		      temp = nextword;
		      nextword = nextword->wl_next;
		      return temp;
	      }
	      else
		    {
	/* 	      xw_error(SV_ERROR,  "poplist: The list is exhausted\n"); */
		      return NULL;
		    }
	    }
	  else
	    {
/* 	      xw_error(SV_ERROR,  "poplist: The list is exhausted\n"); */
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
	  printf("pushlist: putting %#010x which points to %s back on\n",
		  (UINT)ref, ref->wl_word);
	  ptr = xw_start.wl_next;
	  while (ptr != NULL)
	    {
	      printf("%#010x\t[%s]\n", (UINT)ptr, ptr->wl_word);
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
		  /* 	      PRINT3(010x, (UINT)cur, (UINT)cur->wl_next, (UINT)nextword); */
		  /* 	      PRINT1(010x, (UINT)cur->wl_prev); */
		  
		  cur = cur->wl_prev;
#ifdef DEBUG
		  printf("pushlist: cur = %#010x\t\n", (UINT)cur);
#endif /* DEBUG */
		  if(cur EQ NULL)
		    {
/* 		      xw_error(SV_ERROR, */
/* 			       "pushlist: %#010x [%s] was not in wordlist\n", */
/* 			       (UINT)ref, ref->wl_word); */
		      break;
		    }
		}
	      if (cur != NULL)
		{
		  nextword = cur;
		}
	    }

	  TEST(PRINT1(010x, (UINT)nextword));

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
	      printf("xw_restartlist(): ref = %#010x[%s]\n", (UINT)ref, ref->wl_word);
#endif

	      ref->wl_status = UNUSED;
	      ref = ref ->wl_next;
	    }
}

/* End of /home/franx/xword/xw_listutil.c */
