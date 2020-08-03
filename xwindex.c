/* /home/franx/xword/xwindex.c Fri21Jul2006 {fcG} */

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define extern
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XWINDEX	          	#
# Create indexes from given dictionary	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
  FILE	*fp;
  BOOLEAN alpha_word, printable_clue;
  int	c,i, linenum = 0, return_code = 0;
  long offset;
  char indexdir[SZ_FILENAME], sysbuf[SZ_STRING],
	word[WORDLENGTH+1], clue[CLUELENGTH+1];

  if(setjmp(xw_env) EQ 0)
    {
      printf(
"\033[7m#*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*#\033[0m\n");
      printf(
"\033[7m#                                     #\033[0m\n");
      printf(
"\033[7m#  Welcome to Franx X-word Indexer!   #\033[0m\n");
      printf
("\033[7m#                                     #\033[0m\n");
      printf(
"\033[7m#*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*#\033[0m\n");
      WHEN;

      if(argc < 2)
	{
	  char buf[10];
	  
	  sprintf(buf, "%%%ds", SZ_FILENAME);
	  printf("\nInput file of dictonary words? ");
	  scanf(buf, xw_indexfile);
	}
      else
	{
	  PRINT1(s, argv[1]);
	  strncpy(xw_indexfile, argv[1], SZ_FILENAME);
	}
      
      time(&xw_starttime);
      system("date");
      
      fp = fopen(xw_indexfile, "r");
      if (fp EQ NULL)
	{
	  xw_error(SV_ERROR, "Error opening \"%s\"", xw_indexfile);
	}

      sprintf(xw_indexdir, "%s.index", xw_indexfile);

      sprintf(sysbuf, "if [ -d  %s ]; then rm -rf %s; fi;",
		xw_indexdir, xw_indexdir);
      if (system(sysbuf))
	{
	  xw_error(SV_ERROR, "Error nuking %s\n", xw_indexdir);
	}
      xw_error(SV_INFO, "XWINDEX: Nuked %s\n", xw_indexdir);

      sprintf(sysbuf, "mkdir %s", xw_indexdir);
      if (system(sysbuf))
	{
	  xw_error(SV_ERROR, "Error recreating %s\n", xw_indexdir);
	}

      c = getc(fp);
      offset = 0;
      while(c != EOF)
	{
	  FILE *newfp;
	  char newfname[SZ_FILENAME];

	  i = 0;
	  linenum++;
	  alpha_word = printable_clue = TRUE;
	  while(c != ' ' && c != '\n' && c != '\t' && c != EOF)
	    {
	      if(!isalpha(c))
		{
		  alpha_word = FALSE;
		}
	      c = islower(c) ? toupper(c) : c;
	      word[i++] = c;
	      xw_totletters++;
	      if (i > WORDLENGTH)
		{
		  word[i] = '\0';
		  xw_error(SV_INFO,
			"line %d: %s... too long for word (limit %d)",
			linenum, word, WORDLENGTH);
		  return_code++;
		  while(c != ' ' && c != '\n' && c != '\t' && c != EOF)
		    {
		      c = getc(fp);
		      i++;
		    }
		  break;
		}
	      INDEXDIR(i);
	      sprintf(sysbuf, "if [ ! -d  %s ]; then mkdir %s; fi;",
		      xw_indexdir, xw_indexdir);
	      if (system(sysbuf))
		{
		  xw_error(SV_ERROR,
			"Error creating %s/%c\n", xw_indexdir,c);
		}

	      sprintf(newfname, "%s/%c", xw_indexdir, c);

	      newfp = fopen(newfname, "ab");
	      if (newfp EQ NULL)
		{
		  xw_error(SV_ERROR, "Error opening %s", newfname);
		};
	      if (fwrite(&offset, sizeof(offset), 1, newfp) < 1)
		{
		  xw_error(SV_ERROR, "Error writing %s", newfname);
		}
	      if (fclose(newfp) EQ EOF)
		{
		  xw_error(SV_ERROR, "Error closing %s", newfname);
		}

	      c = getc(fp);
	    }
	  word[i] = '\0';
	  if(!alpha_word)
		{
		xw_error(SV_WARNING,
		"line %d: Words must be alphabetic(\"%s\")",
		       linenum,word);
		return_code++;
		}

	  if (i < 3)
	    {
	      xw_error(SV_INFO,
		"line %d: %s not long enough for word",linenum,word);
	      return_code++;
	      /* break */;
	    }

	  xw_totwords++;
	
	  offset += i+1;
	  i = 0;
	  if(c EQ ' ' || c EQ '\t')
	    {
	      c = getc(fp);
	      i++;
	    }
	  while((c != EOF) && (c != '\n'))
	    {
	      clue[i++] = c;
	      if(!isprint(c) && c != '\t')
		{
		  printable_clue = FALSE;
		}
	      if(i > CLUELENGTH)
		{
		  if (CLUELENGTH > 30)
		    {
		      clue[30] = '\0';
		    }
		  else
		    {
		      clue[i] = '\0';
		    }
#ifdef	SNARK

		  xw_error(SV_INFO,
			"line %d: %30s... too long for clue (limit %d)",
			   linenum, clue, CLUELENGTH);
		  return_code++;
#endif	/* SNARK */

		  while(c != '\n')
		    {
		      c = getc(fp);
		      i++;
		    }
		  break;
		}
	      c = getc(fp);
	    }
	  clue[i] = '\0';
	  if (!printable_clue)
	    {
	      xw_error(SV_WARNING,
		"line %d: Clues must be printable(\"%s\")",
		       linenum,clue);
	      return_code++;
	    }

#ifdef	SNARK
	  if(i < 2)
	    {
	      xw_error(SV_INFO,
		"line %d: clue for %s not long enough for clue",
		linenum,word);
	      return_code++;
	    }
#endif	/* SNARK */
	  
	  c = getc(fp);
	  offset += i;
	}
      if (xw_totwords > NUMWORDS)
	{
		xw_error(SV_INFO,
		"line %d: Word limit of %d exceeded; %d words encountered",
		linenum,NUMWORDS, xw_totwords);
		return_code++;
	}
	fclose(fp);
	printf("Time elapsed %6.2f secs.\n",
	difftime(time(NULL), xw_starttime));      
	exit(0);
    }
  else
    {
      fprintf(stderr, "xwindex: aborting due to errors...\n");
      exit(-1);
    }
}
	
/* End of /home/franx/xword/xwindex.c  */
