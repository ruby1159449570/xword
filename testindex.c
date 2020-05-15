/* /home/franx/xword/testindex.c Tue25Jul2006 {fcG} */

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
#		TESTINDEX          	#
# Test indexfile created from dictionary#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
  FILE	*fp;
  BOOLEAN alpha_word, printable_clue;
  int	c,i, linenum = 0, return_code = 0;
  unsigned long offset;
	
  char sysbuf[SZ_STRING], buf[WORDLENGTH+1];
  
  if(setjmp(xw_env) EQ 0)
    {
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
      
      /*       sprintf(xw_indexdir, "%s.index", xw_indexfile); */
      
      i = 1;
      for(EVER)
	{
	  INDEXDIR(i++);
	  sprintf(sysbuf, "[ -d  %s ]", xw_indexdir);
	  if (system(sysbuf) != 0)
	    {
	      break;
	    }
	  
	  for(c='A'; c <= 'Z';c++)
	    {
	      FILE *newfp;
	      char newfname[SZ_FILENAME];
	      
	      sprintf(newfname, "%s/%c", xw_indexdir, c);
	      
	      newfp = fopen(newfname, "rb");
	      if (newfp EQ NULL)
		{
		  continue;
		}
	      
	      while(fread(&offset, sizeof(offset), 1, newfp) > 0)
		{
		 /*  PRINT2(d, return_code, offset); */
		  if (ferror(newfp))
		    {
		      xw_error(SV_ERROR, "Error reading %s", newfname);
		    }
;
		  if (fseek(fp, offset, SEEK_SET) != 0)
		    {
		      xw_error(SV_ERROR, "fseek() error");
		    }
		  fgets(buf, sizeof(buf), fp);
		  printf("\"%s\"[%#020lx]==>%s",newfname,offset,buf);
       		}
	      while(!feof(newfp));

	      if (fclose(newfp) EQ EOF)
		{
		  xw_error(SV_ERROR, "Error closing %s", newfname);
		}
	    }
	}
      fclose(fp);
      printf("Time elapsed %6.2f secs.\n", difftime(time(NULL), xw_starttime));      
      exit(0);
    }
  else
    {
      fprintf(stderr, "testindex: aborting due to errors...\n");
      exit(-1);
    }
}
	
/* End of /home/franx/xword/testindex.c  */
