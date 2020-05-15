/* /home/franx/xword/testdir.c Tue25Jul2006 {fcG} */

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
  return_code = system("[ -d testar.index ]");
  PRINT1(d, return_code);
  if (system("[ -d testar.index/000000 ]") EQ 0)
    {
      printf("Returns true\n");
    }
  else
    {
      printf("Returns false\n");
    }
}

/* End of /home/franx/xword/testdir.c  */
