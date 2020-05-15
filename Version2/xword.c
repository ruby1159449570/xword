/* /home/franx/xword/xword.c Mon26Jan2004 {fcG} */

#include <stdio.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
#define	extern
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		MAIN			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
	if(setjmp(xw_env) EQ 0)
	{
	  PUZZHEAD *puzzle_header;
	  WORDLIST *failword, *word, *firstfail;
	  
	  printf("Welcome to Franx X-word Generator!\n");
	  WHEN;

	  if(argc < 2)
	    {
	      char buf[10];
	      
	      WHEN, WHERE;
	      sprintf(buf, "%%%ds", SZ_FILENAME);
	      /* PR(s, buf); */
	      
	      printf("Input file of words?\n");
	      scanf(buf, xw_inputfile);

#ifdef SNARK
	      fprintf(stderr, "Output file for final puzzle?\n");
	      scanf(buf, xw_outputfile);
	      fprintf(stderr, "Puzzle compactness?\n");
	      scanf("%2d", &xw_puzz_compact_req);
	      TEST(PR(d, xw_puzz_compact_req));
	      TEST(PRINT2(s, xw_inputfile, xw_outputfile));
#endif /* SNARK */
 
	    }
	  else
	    {
	      PRINT1(s, argv[1]);
	      strncpy(xw_inputfile, argv[1], SZ_FILENAME);
	    }

	  time(&xw_starttime);

	  xw_readsort();
	  xw_createlinks();

	  xw_poplist(); /* pop xw_start off wordlist?! */

	  word = xw_poplist();
	  puzzle_header = xw_initpuzz(word);
	  WHEN, WHERE; TEST(PRINT1(010x, (UINT)puzzle_header));

GOBACK:

       	  do
	    {
	      PUZZHEAD *temp;

/*  	      xw_printpuzz(puzzle_header); */

	      temp = xw_copypuzz(puzzle_header);

/*  	      xw_printpuzz(temp); */

	      word = xw_poplist();
#ifdef DEBUG
	      PRINT3(010x, (UINT)word, (UINT)temp, (UINT)puzzle_header);
#endif /* DEBUG */
	      temp->ph_prevpuzz = puzzle_header;
	      puzzle_header = temp;

	    }
	  while((word != NULL)
	  && (failword = xw_buildpuzz(puzzle_header, word)) EQ NULL);
	  
	  if (failword != NULL)
	    {
	      PUZZHEAD *temp;
	      
#ifdef SNARK
	      temp = puzzle_header->ph_prevpuzz;
/* 	      xw_killpuzz(puzzle_header); */
	      puzzle_header = temp;
#endif /* SNARK */

	      firstfail = failword;
#ifdef DEBUG
	      printf("\033[32;40m");
	      PR(010x, (UINT)firstfail);
	      PR(s, firstfail->wl_word);
	      printf("\033[0m");
#endif
	      do
		{	
		  do
		    {
		      puzzle_header = puzzle_header->ph_prevpuzz;
		      if (puzzle_header EQ NULL)
			{
			  puzzle_header = xw_initpuzz(failword);
			  break;
			}
		      temp = xw_copypuzz(puzzle_header);
#ifdef DEBUG
		      TEST(WHERE);
		      PRINT2(010x, (UINT)temp, (UINT)puzzle_header);
		      
		      /* PRINT2(s, puzzle_header->ph_lastword->wl_word, */
		      /* temp->ph_lastword->wl_word); */
		      xw_printpuzz(puzzle_header);
#endif /* DEBUG */
		      
		      if(puzzle_header->ph_lastword != NULL)
			{
			  xw_pushlist(puzzle_header->ph_lastword);
			}

		      puzzle_header = temp;

		    }
		  while((word != NULL)
			&& (xw_buildpuzz(puzzle_header,failword) != NULL));

#ifdef DEBUG
		  printf("\033[32;40m");
		  WHERE; printf("Found a spot for %s\n", failword->wl_word);
		  printf("\033[0m");

/* 		  xw_printpuzz(puzzle_header); */

#endif /* DEBUG */

		  do
		    {
		      temp = xw_copypuzz(puzzle_header);

		      /* xw_printpuzz(temp); */

		      temp->ph_prevpuzz = puzzle_header;
		      puzzle_header = temp;
		      word = xw_poplist();
		    }
		  while((word != NULL)
			&& (failword = xw_buildpuzz(puzzle_header,word)) EQ NULL);
		 
		  if (failword EQ firstfail)
		    {
		      static WORDLIST *newhead = NULL;

#ifdef DEBUG
		      PR(s, firstfail->wl_word);
		      PRINT2(010x, (UINT)newhead, (UINT)firstfail);
#endif

		      if (newhead EQ NULL || firstfail != newhead)
			{   
			  newhead = firstfail;

			  xw_restartlist();
			  xw_poplist(); /* pop xw_start off wordlist?! */

/* 			  xw_pushlist(failword); */
/* 			  word = xw_poplist(); */

			  puzzle_header = xw_initpuzz(failword);

/* 			  xw_printpuzz(puzzle_header); */

			  goto GOBACK;

			}
		      else
			{
/* 			  xw_printpuzz(puzzle_header); */

			  xw_error(SV_FATAL, "Could not find spot for %s\n",
			       failword->wl_word);
			}
		    }
		}
		  while((word != NULL) && (failword != NULL));
	    }
/* 	  xw_printpuzz(puzzle_header); */
	  xw_html(puzzle_header);
	  xw_killlist();
/* 	  xw_killpuzz(puzzle_header); */
	  exit(0);
	}
	else
	  {
	    fprintf(stderr, "xword: aborting due to error...\n");
	    if (errno != 0);
	    {
	      fprintf(stderr, "errno = %d(%s)\n", errno, strerror(errno));
	      exit(-1);
	    }
	  }
}

/* End of /home/franx/xword/xword.c */
