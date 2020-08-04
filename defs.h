/* /home/franx/source/defs.h Thu29Jan2004 {fcG} */

#ifndef _DEFS_H
#define _DEFS_H 1
#include	<stdio.h>

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		DEFINES			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define	EVER		;;
#define	EQ		==

#define	MAX(x,y)	((x) > (y) ? (x) : (y))
#define	MIN(x,y)	((x) < (y) ? (x) : (y))

#define	MAXINT		(~(int)(1<<(sizeof(int)*8-1)))
#define	MAXLONG		(~(1L<<(sizeof(long)*8-1)))
#define	MAXSHORT	(~(short)(1<<(sizeof(short)*8-1)))

#define	SZ_FILENAME	FILENAME_MAX
#define	SZ_STRING	100
#define	SZ_TABLE(x)	(sizeof(x)/sizeof(x[0]))

#ifdef	__STDC__
#define	PR(fmt,value)	printf(#value " = %" #fmt "\t", (value))
#define	WHEN		printf("%s %s", __DATE__, __TIME__) 
#define	WHERE		printf("%s[%d]", __FILE__, __LINE__) 
#else
#define	PR(fmt,value)	printf("value = %fmt\t", (value))
#define	WHEN
#define	WHERE
#endif	/* __STDC__ */

#define	nL			putchar('\n')
#define	PRINT1(f,x1)		PR(f,x1),nL
#define	PRINT2(f,x1,x2)		PR(f,x1),PRINT1(f,x2)
#define	PRINT3(f,x1,x2,x3)	PR(f,x1),PRINT2(f,x2,x3)
#define	PRINT4(f,x1,x2,x3,x4)	PR(f,x1),PRINT3(f,x2,x3,x4)

#ifdef DEBUG
#define	TEST(x)		x
#else
#define	TEST(x)
#endif /* DEBUG */

#ifdef extern
#define	INIT(x)		=x
#else
#define	INIT(x)
#endif /* extern */

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		TYPEDEFS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef	enum bool { FALSE = 0, TRUE = 1 }	BOOLEAN;

typedef unsigned char	UCHAR;
typedef unsigned int	UINT;
typedef unsigned long	ULONG;
typedef unsigned short	USHORT;

#endif /* _DEFS_H */

/* End of /home/franx/defs.h */
