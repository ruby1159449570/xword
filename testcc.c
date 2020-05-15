/* /home/franx/source/testcc.c Fri Jun 16 13:25:33 EST 2006 {fcG} */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	Test program for C compilers..	#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "defs.h"

int main(int argc, char *argv[])
{
  char testbuf[SZ_STRING];
  int return_code;

  WHERE;
  WHEN;
  if ( /*/**/FALSE/*/TRUE/**/ )
    {
      printf(" Comments nest\n");
    }
  else
    {
      printf(" Comments do NOT nest\n");
    }
  PR(d, MAXINT);
  PR(d, MAXSHORT);
  PRINT1(ld, MAXLONG);
  return_code = sprintf(testbuf, "%s", "This a test...");
  PR(d, return_code);
  PRINT1(s, testbuf);

/*   PR(10x, testbuf); */
/*   PR(#010x, sprintf(testbuf, "%s", "This a test...\n")); */
}

/* End of /home/franx/source/testcc.c */
