/* /home/franx/xword/xword.c Mon26Jan2004 {fcG} */
//
//  Created by Frank Charles Gallacher on 22/5/20.
//  Copyleft © 2020 Frank Charles Gallacher. All rights reserved.
//
// MODIFICATION HISTORY
// When		Who	What
// Wed08Jul2020 {fcG}	64-bit debug code.
// Sun12Jul2020 {fcG}	"Thousand island" do-do bug squashed!!!

#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "defs.h"
#define	extern
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	SIGINT_HANDLER			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

static PUZZHEAD *sigint_puzzle_header=NULL;

static void sigint_handler()
{
	printf(
	"\033[7;31m<CTRL-C> received, Hanging up...\033[0m\n");
	if (sigint_puzzle_header != NULL)
	{
		xw_printpuzz(sigint_puzzle_header);
		xw_html(sigint_puzzle_header);
	}
	xw_error(SV_ERROR, "<CTRL-C> received, Hanging up...");
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		MAIN			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
  if(setjmp(xw_env) EQ 0)
    {
      int count;
      PUZZHEAD *puzzle_header, *old_puzzhead;
      WORDLIST *failword, *word, *firstfail;
      
      printf(
"\033[7m#*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*#\033[0m\n");
      printf(
"\033[7m#  Welcome to Franx X-word Generator! #\033[0m\n");
      printf(
"\033[7m#             Version: %#4.2f           #\033[0m\n",
	version());
      printf(
"\033[7m# Link Date: %s#\033[0m\n", date_compiled());
      printf(
"\033[7m#*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*#\033[0m\n");
/* 	WHEN; */
      if(argc < 2)
	{
	  char buf[10];
	  
	  sprintf(buf, "%%%ds", SZ_FILENAME);
	  printf("\nxword: Input file of words? ");
	  scanf(buf, xw_inputfile);

	  sprintf(buf, "%%%ds", SZ_FILENAME);
	  printf("\nxword: Dictionary file of words? ");
	  scanf(buf, xw_indexfile);

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

		if(argc EQ 3)
		{
			PRINT1(s, argv[2]);
			strncpy(xw_indexfile, argv[2], SZ_FILENAME);
		}
		else
		{
			strncpy(xw_indexfile, "", SZ_FILENAME);
			
		}
	}

time(&xw_starttime);
if ((count = xw_readsort()) > 0)
{
  xw_error(SV_ERROR,
"%d Errors encountered in parsing %s", count, xw_inputfile);
}
if (xw_totwords <= 1)
{
  xw_error(SV_FATAL,
"Don't be a Smartarse, a xword must have at least 2 words");
}
xw_createlinks();

xw_poplist(); /* pop xw_start off wordlist?! */

word = xw_poplist();
puzzle_header = xw_initpuzz(word);
TEST(PRINT1(#018lx, (ULONG)puzzle_header));

#ifdef	SNARK

GOBACK:

do
{
	PUZZHEAD *temp;

	temp = xw_copypuzz(puzzle_header);
	word = xw_poplist();

	TEST(PRINT3(#018lx,
	(ULONG)word, (ULONG)temp, (ULONG)puzzle_header));

	temp->ph_prevpuzz = puzzle_header;
	puzzle_header = temp;
}
while((word != NULL)
&& (failword = xw_buildpuzz(puzzle_header, word)) EQ NULL);
#endif	/* SNARK */
  
while((word = xw_poplist()))
{
	PUZZHEAD *temp;

	temp = xw_copypuzz(puzzle_header);
	temp->ph_prevpuzz = puzzle_header;
	puzzle_header = temp;
	
	while((failword = xw_buildpuzz(puzzle_header, word)))
	{

		TEST(PRINT2(#018lx,
		(ULONG)failword, (ULONG)puzzle_header));
		puzzle_header->ph_failpuzz = failword->wl_failpuzz;
		failword->wl_failpuzz = puzzle_header;
		puzzle_header = puzzle_header->ph_prevpuzz;
		if(!puzzle_header)
		{
			xw_error(SV_FATAL,
			"Could not find spot for %s",
			failword->wl_word);
		}
	}
}
	
#ifdef	SNARK


if (failword != NULL)
	{
	  PUZZHEAD *temp;
	      
	  firstfail = failword;
	  puzzle_header->ph_failpuzz = failword->wl_failpuzz;
	  failword->wl_failpuzz = puzzle_header;
#ifdef DEBUG
		printf("\033[32;40m");
		PR(#018lx, (ULONG)firstfail);
		PR(s, firstfail->wl_word);
	printf("\033[0m\n");
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
		  PRINT2(#018lx, (ULONG)temp,
			(ULONG)puzzle_header);
/* 		  xw_printpuzz(puzzle_header); */
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
	      WHERE; printf("Found a spot for %s",
			failword->wl_word);
	      printf("\033[0m\n");
#endif /* DEBUG */
	      do
		{
		  temp = xw_copypuzz(puzzle_header); 
		  temp->ph_prevpuzz = puzzle_header;
		  puzzle_header = temp;
		  word = xw_poplist();
		}
	      while((word != NULL)
		&& (failword = xw_buildpuzz(puzzle_header,word)) EQ NULL);
	      if (failword != NULL)
		{
		  old_puzzhead = failword->wl_failpuzz;
		  while(old_puzzhead != NULL)
		    {
		      if (xw_matchpuzz(old_puzzhead, puzzle_header))
			
			
			xw_error(SV_FATAL, "Could not find spot for %s",
			failword->wl_word);
			}
		      old_puzzhead = old_puzzhead->ph_failpuzz;
		    }
		  if (failword EQ firstfail)
		    {
		      static WORDLIST *newhead = NULL;
#ifdef DEBUG
		      PR(s, firstfail->wl_word);
		      PRINT2(#018lx, (ULONG)newhead, (ULONG)firstfail);
#endif
		      if (newhead EQ NULL || firstfail != newhead)
			{   
			  newhead = firstfail;

			  xw_restartlist();
			  xw_poplist(); /* pop xw_start off wordlist?! */

			  puzzle_header = xw_initpuzz(failword);

#ifdef DEBUG
				printf("\033[32;40m");
				WHERE; printf("Going to GOBACK...");
				printf("\033[0m\n");
#endif /* DEBUG */
			  goto GOBACK;
			}
		    }
	    }
	    while((word != NULL) && (failword != NULL));
	}

#endif	/* SNARK */

	sigint_puzzle_header = puzzle_header;
	if (signal(SIGINT, (void (*)(int))sigint_handler) EQ SIG_ERR)
	{
		xw_error(SV_ERROR, "signal snarked!!!");
	}

#ifdef	SNARK
	for(EVER)
	{
	  ;	// Do *NOTHING*
	}
#endif	/* SNARK */

		if (strncmp(xw_indexfile, "", SZ_FILENAME))
		{
			xw_printpuzz(puzzle_header);
			xw_makeborder(puzzle_header);
			xw_printpuzz(puzzle_header);
			xw_fillpuzz(puzzle_header);
		}
		xw_html(puzzle_header);
		xw_killlist();
		exit(0);
	}
	else
	{
		fprintf(stderr, "xword: aborting due to errors...\n");
		exit(-1);
	}
}
/* End of /home/franx/xword/xword.c */
