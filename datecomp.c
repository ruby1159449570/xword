/* /home/franx/source/datecomp.c Thu20Jul2006 {fcG} */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		DATECOMP		#
#	date compiled executive		#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include "defs.h"
/* #include "version.h" */
#include "version.h"

/* #define	_INCREMENT_	0.001 */

void	clean(char *ptr)
{
	register	int	i;

	for(i = 0; i < strlen(ptr); i++)
	{
		if ( ptr[i] EQ '\n' || ptr[i] EQ '\r' )
		{	
			ptr[i] = ' ';
		}
	}
}

char	buf[SZ_FILENAME], cbuf[SZ_FILENAME], sbuf[SZ_FILENAME];

int main(int argc, char *argv[])
{
	FILE	*fp;
	time_t	clock;
	float	ver, increment;
	register	char	*ptr;
	register	int	i;

/* 	PRINT2(f,_VERSION_,_INCREMENT_); */
	increment = _INCREMENT_;
	ver = _VERSION_;
	PRINT2(f,ver,increment);
	ver += increment;

#undef	DATE_COMPILED
#undef	_VERSION_
#undef	_INCREMENT_

	time(&clock);
/* 	PRINT1(s, ctime(&clock)); */
	ptr=buf;
	for(i = 1; i < argc; i++)
	{
/* 		PRINT1(010x, ptr); */
/* 		PRINT1(s, argv[i]); */
		strncat(buf, argv[i], sizeof(buf) - strlen(buf) - 1);
		ptr += strlen(argv[i]);
		*ptr++ = ' ';
	}
	strncpy(cbuf, ctime(&clock), sizeof(cbuf));
/* 	printf("%s\n",cbuf); */
	clean(cbuf);
	fp = fopen("version.h", "w");
	if (fp != NULL)
	{	
/* 		PRINT1(#4.2f, ver); */
		fprintf(fp, "#define\t_VERSION_\t%#4.2f\n", ver);
		fprintf(fp, "#define\t_INCREMENT_\t%#4.2f\n",
				increment);
		fprintf(fp, "#define\tDATE_COMPILED\t\"%s\"\n", cbuf);
		fclose(fp);
	}
	else
	{	
		fprintf(stderr, "Version file open stuffed\n");
		if (errno != 0)
		{
		fprintf(stderr, "errno = %d (%s)\n", errno,
		      (char *)strerror(errno));
		}
	}
	sprintf(sbuf,
		"cc -DDATE_COMPILED='\"%s\"' -D_VERSION_=%4.2f %s",
		cbuf, ver, buf);
	printf("%s\n",sbuf);
	if (system("sync"))
	{	
		fprintf(stderr,"sync stuffed!!!\n");
		if (errno != 0)
		{
		fprintf(stderr, "errno = %d (%s)\n", errno,
		      (char *)strerror(errno));
		}
	}	
	if (system(sbuf))
	{	
		fprintf(stderr,"System stuffed!!!\n");
		if (errno != 0)
		{
		fprintf(stderr, "errno = %d (%s)\n", errno,
		      (char *)strerror(errno));
		}
	}
}

/* End of /home/franx/source/datecomp.c */
