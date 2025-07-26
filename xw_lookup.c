/* /Users/moonpie/xword/xw_lookup.c Fri09Dec2022 {fcG} */
//
//  Created by Frank Charles Gallacher on 22/5/20.
//  Copyleft © 2020 Frank Charles Gallacher. All rights reserved.
//
// MODIFICATION HISTORY
// When		Who	What
// Sun18Aug2024 {fcG}	debugged main()???

#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define	extern
#include "xword.h"

#define	NOT_WHITE(x)	((x) != '\0' && (x) != EOWORD)
/* #define	SZ_MAXRETSEARCH 40 */

WORDHOLE xw_whstart =
  {
    2020, UNDEFINED, UNDEFINED, 0, 0, UNUSED, NULL,
  };

int	max;

char buf[SZ_FILENAME+1];

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		xw_INSERTHOLE		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

WORDHOLE *xw_inserthole(int spots, int letters,
WORDHOLE *wh, PUZZLE *pzptr, STATUS stat) 
{
	register WORDHOLE *wh_ptr = &xw_whstart;

	wh->wh_key = (spots * 100) + letters;
	wh->wh_status = stat;
	for(EVER)
	{
		if((wh_ptr->wh_next EQ NULL)
		|| (wh_ptr->wh_next->wh_key < wh->wh_key))
		{
			wh->wh_next = wh_ptr->wh_next;
			wh_ptr->wh_next = wh;
			break;
		}
		else
		{
			wh_ptr = wh_ptr->wh_next;
		}
	}
	return (WORDHOLE *)xw_malloc(sizeof(WORDHOLE));
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_SEARCHRANK		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_searchrank(PUZZLE *pzptr, STATUS stat, int length)
{
	register int i, numletters, numspots;
	WORDHOLE *wh_ptr;

	numletters = numspots = 0;
	wh_ptr = (WORDHOLE *)xw_malloc(sizeof(WORDHOLE));
	wh_ptr->wh_rownum = pzptr->pz_rownum;
	wh_ptr->wh_colnum = pzptr->pz_colnum;

	for(i = 0; i < MIN(max, length); i++)
	{
		if (pzptr->pz_letter EQ EOWORD)
		{
			if (numspots != 0 && numspots
			!= numletters)
			{
				wh_ptr = xw_inserthole(numspots,
				numletters, wh_ptr, pzptr, stat);
				wh_ptr->wh_length = numletters;
				wh_ptr->wh_spots = numspots;
// 				break;
			}
			else
			{
				register int j;
				for(j = 0; j < SZ_TABLE(wh_ptr->wh_spot);
					j++)
				{
					wh_ptr->wh_spot[j].sp_letter
						= '\0';
					wh_ptr->wh_spot[j].sp_pos = 0;
				}
			}
			if (stat EQ ACROSS)
			{
				wh_ptr->wh_rownum
					= pzptr->pz_rownum;
				wh_ptr->wh_colnum
					= pzptr->pz_colnum+1;
			}
			else
			{
				wh_ptr->wh_rownum
					= pzptr->pz_rownum+1;
				wh_ptr->wh_colnum
					= pzptr->pz_colnum;
			}
			numletters = numspots = 0;
		}
		else
		{

			if (pzptr->pz_letter EQ '\0'
			|| pzptr->pz_letter EQ ' ')
			{
				numletters++;
			}
			else
			{
				wh_ptr->wh_spot[numspots].sp_pos
					= ++numletters;
				wh_ptr->
				wh_spot[numspots++].sp_letter
					= pzptr->pz_letter;
			}
		}
		if (stat EQ ACROSS)
		{
			pzptr = pzptr->pz_right;
		}
		else
		{
			pzptr = pzptr->pz_down;
		}
	}

/* #ifdef	SNARK */

	if (numspots != 0 && numspots != numletters)
	{
		wh_ptr = xw_inserthole(numspots, numletters,
			wh_ptr, pzptr, stat);
		wh_ptr->wh_length = numletters;
		wh_ptr->wh_spots = numspots;
	}
	else
	{
		free(wh_ptr);
	}
/* #endif	//SNARK */
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FINDGAPS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_findgaps(PUZZHEAD *ph)
{
	PUZZLE *hptr, *vptr, *z_ptr;
	register int i, j;

	hptr = ph->ph_puzzle;
	if(ph->ph_numrows != 1)
	{
		hptr = hptr->pz_down;
	}
	vptr = hptr->pz_right;
	for(i = 0; i < ph->ph_numcols - 2; i++)
	{
		z_ptr = vptr;
		for(j = 0; j < ph->ph_numrows - i - 2; j++)
		{
			xw_searchrank(z_ptr, DOWN, ph->ph_numrows-j-2);
			z_ptr = z_ptr->pz_down;
		}
		vptr = vptr->pz_right;
	}
	vptr = hptr->pz_right;
	for(i = 0; i < ph->ph_numrows - 2; i++)
	{
		z_ptr = vptr;
		for(j = 0; j < ph->ph_numcols - i - 2; j++)
		{
			xw_searchrank(z_ptr, ACROSS, ph->ph_numcols-j-2);
			z_ptr = z_ptr->pz_down;
		}
		vptr = vptr->pz_down;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		CLEAN			#
#	Strips out trailing newlines.	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void	clean(char *ptr)
{
	register	int	i;

	for(i = 0; i < strlen(ptr); i++)
	{
		if ( ptr[i] EQ '\n' || ptr[i] EQ '\r' )
		{	
			ptr[i] = '\0';
		}
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	EQUIVALENCE_CHECK		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int equivalence_check(int length, long *offset)
{
	register int i,j,k;
	long min;

	//	TEST(PRINT2(#018lx, offset[0], offset[1]));
	//	TEST(PRINT3(#06lx, offset[0], offset[1], offset[2]));

/* 	min = (MAXLONG); */
	min = 62767;

	for(i = 0; i < length; i++)
	{
		if (offset[i] < min)
		{
			min = offset[i];
			k = i;
		}
	}
	j = 0;
	for(i = 0; i < length; i++)
	{
		if (offset[i] EQ min)
		{
			j++;
		}
	}
	if (j EQ length)
	{
		return j;
	}
	else
	{
		return k;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		CLEAR_WORD		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

BOOLEAN clear_word(PUZZHEAD *ph, char *buf, WORDHOLE *wh_ptr)
{
	BOOLEAN	search;
	int	length = 0;
	register int	i, j;
	PUZZLE	*ref;

	search = TRUE;
	length = strlen(buf);
	TEST(PRINT2(d, wh_ptr->wh_colnum,  wh_ptr->wh_rownum));
	TEST(fprintf(stderr, "Status = %s\n", DECODE(wh_ptr->wh_status)));
	ref = xw_pointpuzz(ph, wh_ptr->wh_colnum,  wh_ptr->wh_rownum);
	if(ref EQ 0)
	{
		TEST(WHERE);TEST(printf("xw_pointpuzz() bombed!\n"));
		return FALSE;
	}
	if (wh_ptr->wh_status EQ ACROSS)
	{
		if (NOT_WHITE(ref->pz_left->pz_letter))
		{
			search = FALSE;
		}	
		else
		{
			j = 0;
			for (i = 0; i < length; i++)
			{
				TEST(WHERE); TEST(PRINT2(c,
				ref->pz_up->pz_letter,
				ref->pz_down->pz_letter));
				TEST(WHERE);
			TEST(PRINT2(d, wh_ptr->wh_spots, wh_ptr->wh_spot[j].sp_pos));

				if ((j < wh_ptr->wh_spots)
				&& (wh_ptr->wh_spot[j].sp_pos EQ (i + 1)))
				{
					TEST(WHERE);
					TEST(printf("Skipping j = %d\n",j));
					j++;
				}	
				else if (NOT_WHITE(ref->pz_up->pz_letter)
				|| NOT_WHITE(ref->pz_down->pz_letter))
				{
					TEST(WHERE);
					TEST(printf("Stopping\n"));
					search = FALSE;
				}
				else
				{
					TEST(WHERE);
					TEST(printf("Bombing\n"));
				}
				ref = ref->pz_right;
				if(ref EQ 0)
				{
						TEST(WHERE);
				TEST(printf("Across bombed!\n"));
					return FALSE;
				}
			}
			if (NOT_WHITE(ref->pz_letter))
			{
				TEST(printf("Across end bombed\n"));
				search = FALSE;
			}	
		}	
	}
	else if (wh_ptr->wh_status EQ DOWN)
	{
		if (NOT_WHITE(ref->pz_up->pz_letter))
		{
			search = FALSE;
		}	
		else
		{
			j = 0;
			for (i = 0; i < length; i++)
			{
				TEST(WHERE); TEST(PRINT2(c,
				ref->pz_left->pz_letter,
				ref->pz_right->pz_letter));
				TEST(WHERE); TEST(PRINT2(d,
				wh_ptr->wh_spots,
				wh_ptr->wh_spot[j].sp_pos));

				if ((j < wh_ptr->wh_spots)
				&& (wh_ptr->wh_spot[j].sp_pos EQ (i + 1)))

				{
					TEST(WHERE);
					TEST(printf("Skipping j = %d\n",j));
					j++;
				}	
				else if (NOT_WHITE(ref->pz_left->pz_letter)
				|| NOT_WHITE(ref->pz_right->pz_letter))
				{
					TEST(WHERE);
					TEST(printf("Stopping\n"));
					search = FALSE;
				}
				else
				{
					TEST(WHERE);
					TEST(printf("Bombing\n"));
				}
				ref = ref->pz_down;
				if(ref EQ 0)
				{
					TEST(WHERE);
				TEST(printf("Down bombed!\n"));
					return FALSE;
				}
			}
			if (NOT_WHITE(ref->pz_letter))
			{
				TEST(printf("Down end bombed!\n"));
				search = FALSE;
			}	
		}	
	}	
	else
	{
		TEST(WHERE); TEST(fprintf(stderr, "Bad Status!!!\n"));
		search = FALSE;
	}
	TEST(fprintf(stderr, "clear_word() returns %d\n", search));
	return search;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	XW_SEARCH_BLANKS		#
#	"All thats print to fit"	#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

char	return_buf[SZ_MAXRETSEARCH][WORDLENGTH + 2];

int	xw_search_blanks(PUZZHEAD *ph, FILE *fp_dict, WORDHOLE *wh_ptr)
{
	BOOLEAN	search, equivalent;
	FILE	*fp[WORDLENGTH], *minfp, *newfp;
	long	offset[WORDLENGTH],minoffset;
	int	count, min_count, search_count;

	TEST(PRINT2(d, wh_ptr->wh_key, wh_ptr->wh_length));
	TEST(PRINT3(d,wh_ptr->wh_spots,wh_ptr->wh_rownum,wh_ptr->wh_colnum));
	minoffset = (MAXLONG);

	equivalent = FALSE;
	search = TRUE;
	search_count = 0;

	for(count = 0; count < wh_ptr->wh_spots; count++)
	{
		INDEXDIR(wh_ptr->wh_spot[count].sp_pos);

//		sprintf(xw_indexdir,"%s.index/%06d",xw_indexfile,
//		wh_ptr->wh_spot[count].sp_pos);

		sprintf(buf, "%s/%c", xw_indexdir, 
			wh_ptr->wh_spot[count].sp_letter);
		TEST(WHERE); TEST(PRINT1(s,buf));
		if (!(fp[count] = fopen(buf,"rb")))
		{
			if(errno EQ ENOENT)
			{
				search = FALSE;
			}
			else
			{
				TEST(WHERE); xw_error(SV_FATAL,
				"Error opening %s\n", buf);
			}
		}
	}

	if (search)
	{
		for(count = 0; count < wh_ptr->wh_spots; count++)
		{
		fread(&offset[count], sizeof(offset[count]), 1, fp[count]);
			if (ferror(fp[count]))
			{
				INDEXDIR(wh_ptr->wh_spot[count].sp_pos);

//			sprintf(xw_indexdir,"%s.index/%06d",
//			xw_indexfile, wh_ptr->wh_spot[count].sp_pos);

				sprintf(buf, "%s/%c", xw_indexdir, 
				wh_ptr->wh_spot[count].sp_letter);
				xw_error(SV_FATAL, "Error reading %s\n",
						buf);
			}		
			if (feof(fp[count]))
			{
				search = FALSE;
				TEST(WHERE); TEST(fprintf(stderr,
				"Search failed! EOF\n"));
				break;
			}
		}
		if (wh_ptr->wh_spots < 2)
		{
			if (search)
			{
				equivalent = TRUE;
				min_count = 0;
			}
		}
		else
		{
			if ((min_count
			= equivalence_check(wh_ptr->wh_spots,
			(long *)offset)) EQ wh_ptr->wh_spots)
			{
				equivalent = TRUE;
			}		
		}		
		TEST(WHERE); TEST(PRINT2(d, min_count, wh_ptr->wh_spots));
		while(!equivalent && search)
		{

			fread(&offset[min_count], sizeof(offset[min_count]),
				1, fp[min_count]);
			if (ferror(fp[min_count]))
			{
				INDEXDIR(wh_ptr->wh_spot[min_count].sp_pos);

//		sprintf(xw_indexdir,"%s.index/%06d",
//		xw_indexfile, wh_ptr->wh_spot[min_count].sp_pos);
//		xwindexdir(wh_ptr->wh_spot[min_count].sp_pos);

				sprintf(buf, "%s/%c", xw_indexdir, 
				wh_ptr->wh_spot[min_count].sp_letter);
				xw_error(SV_FATAL, "Error reading %s\n",
						buf);
			}		
			if (feof(fp[min_count]))
			{
				search = FALSE;
				TEST(WHERE); TEST(PRINT1(d, min_count));
				TEST(fprintf(stderr,"Search failed! EOF\n"));
				break;
			}
			if ((min_count =
			equivalence_check(wh_ptr->wh_spots,
				(long *)&offset))
			EQ wh_ptr->wh_spots)
			{
				equivalent = TRUE;
			}
		}
		TEST(WHERE); TEST(PRINT2(d, equivalent, search));
		if (equivalent && search)
		{
			TEST(WHERE); TEST(PRINT1(#018lx, offset[0]));
			if (fseek(fp_dict, offset[0], SEEK_SET) != 0)
			{
				TEST(WHERE);xw_error(SV_ERROR, "fseek error");
			}
			if(!fgets(buf, sizeof(buf), fp_dict))
			{
				TEST(WHERE);xw_error(SV_ERROR,
					"fgets error");
			}
			clean(buf);
			if(search_count < SZ_MAXRETSEARCH
			&& clear_word(ph, buf, wh_ptr))
			{
				strncpy(&return_buf[search_count][0], buf,
					sizeof(return_buf[0]) - 1);
				search_count++;
			}
			TEST(fprintf(stderr, "\"%s\"[%#018lx]==>%s\n",
				xw_indexfile, offset[0],buf));
			do
			{
				for(count = 0; count < wh_ptr->wh_spots;
				count++)
				{
					fread(&offset[count],
					sizeof(offset[count]), 1,
					fp[count]);
					if (ferror(fp[count]))
					{
						TEST(WHERE);
				INDEXDIR(wh_ptr->wh_spot[count].sp_pos);

//		sprintf(xw_indexdir,"%s.index/%06d",
//		wh_ptr->wh_spot[count].sp_pos);

						sprintf(buf, "%s/%c",
						xw_indexdir, 
						wh_ptr->
						wh_spot[count].sp_letter);
						xw_error(SV_FATAL,
						"Error reading %s\n", buf);
					}		
					if (feof(fp[count]))
					{
						search = FALSE;
						TEST(WHERE);
						TEST(fprintf(stderr,
						"Search failed! EOF\n"));
						break;
					}
				}
				if (equivalence_check(wh_ptr->wh_spots,
					(long *)offset)
				EQ wh_ptr->wh_spots && search)
				{
					equivalent = TRUE;
				}
				else
				{
					equivalent = FALSE;
					break;
				}

				if(fseek(fp_dict,offset[0],SEEK_SET) != 0)
				{
					TEST(WHERE); xw_error(SV_ERROR,
					"fseek() error");
				}
				if(!fgets(buf, sizeof(buf), fp_dict))
				{
					TEST(WHERE); xw_error(SV_ERROR,
					"fgets() error");
				}
				clean(buf);
				if (search_count < SZ_MAXRETSEARCH
				&& clear_word(ph, buf, wh_ptr))
				{
					strncpy(
					&return_buf[search_count][0], buf,
					sizeof(return_buf[0]) - 1);
					search_count++;
				}
				TEST(fprintf(stderr,
					"\"%s\"[%#020lx]==>%s\n",
					xw_indexfile, offset[0],buf));
			}	
			while (equivalent && search);
		}	
		for(count = 0; count < wh_ptr->wh_spots; count++)
		{
			if(fp[count] != 0)
			{	
				TEST(WHERE); TEST(PR(d,count));
				TEST(PRINT1(#018lx,fp[count]));
				if(fclose(fp[count]) EQ EOF)
				{
					TEST(WHERE); xw_error(SV_FATAL,
					"fclose() bombed!");
				}
			}
		}
	}
	TEST(WHERE); TEST(PRINT1(d, search_count));
	return search_count;
}


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#	XW_LOOKUP (stub)		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int main(int argc, char *argv[])
{
	int	total;
	char	xw_wh_string[SZ_STRING];
	register WORDHOLE *wh_ptr = &xw_whstart;
	register int	i;
	int	count;
	BOOLEAN	james;
	FILE	*fp_dict;
	long	offset[WORDLENGTH],minoffset;
	PUZZHEAD	*puzzle_header;
	WORDLIST	wp;
	WORDHOLE testdata;

	if(setjmp(xw_env) != 0)
	{	
		printf("xwlookup(): exiting...\n");
		exit(-1);
	}
	if(argc >= 2)
	{
		PRINT1(s, argv[1]);
		strncpy(xw_wh_string, argv[1], SZ_STRING);
	}
	else
	{
		xw_error(SV_ERROR, 
		"Usage: xwlookup <search_string> [<indexfile>]");
	}
	if(argc EQ 3)
	{
		PRINT1(s, argv[2]);
		strncpy(xw_indexfile, argv[2], SZ_FILENAME);
	}
	else
	{
		strncpy(xw_indexfile, "johnlove", SZ_FILENAME);
	}
	time(&xw_starttime);

	wh_ptr = &xw_whstart;
	wh_ptr->wh_next = &testdata;
	testdata.wh_key = 1010;
	testdata.wh_rownum = testdata.wh_colnum = 1;
	testdata.wh_status = ACROSS;
	testdata.wh_next = NULL;

	count = strlen(argv[1]);
	testdata.wh_spots = count;

/* 	strncpy(xw_wh_string, argv[count); */

	wp.wl_numchar = count;
/* 	wp.wl_numcols = count + 2; */
/* 	wp.wl_numrows = 3; */
	strncpy(wp.wl_word, argv[count], count);
	puzzle_header = xw_initpuzz(&wp);
	xw_printpuzz(puzzle_header);
	xw_putwordin(puzzle_header, &wp, ACROSS, 0, 0);
	for (i = 0; i < count; i++)
	{
		if (wp.wl_word[count] EQ ' ')
		{	
			wp.wl_word[count] = DONTCARE;
		}	
		else
		{	
#ifdef	SNARK
			if (islower(wp.wl_word[count]))
			{	
				wp.wl_word[count]
				= toupper(wp.wl_word[count])
#endif	//SNARK
			if (isupper(wp.wl_word[count]))
			{	
				wp.wl_word[count]
				= tolower(wp.wl_word[count])
				;
			}	
		}	
	}	
	xw_printpuzz(puzzle_header);
	xw_makeborder(puzzle_header);

	testdata.wh_length = testdata.wh_colnum = 10;
///////	testdata.wh_status = down; */
	testdata.wh_spot[0].sp_pos = 1;

	strncpy(xw_indexfile, "johnlove", 16);

	INDEXDIR(000001);
  
	printf(">>>>%s", xw_indexdir);

	if((max = xw_sizedict(xw_indexfile)) > WORDLENGTH)

	if (!(fp_dict = fopen(xw_indexdir,"rb")))
	{
		TEST(WHERE);xw_error(SV_ERROR, "index file open error\n");
		exit(0);
///////		perror(errno);
	}

	register int j;

////// 	wh = wh->wh_next; */
 
	count = strlen(argv[1]);
	for (i=j=0; i<count; i++)
	{
		if (argv[1][i] != DONTCARE)
		{
			wh_ptr->wh_spot[j].sp_pos = i;
			wh_ptr->wh_spot[j].sp_letter = argv[1][i]; 
			TEST(WHERE);TEST(PRINT2(d,i,j));
		TEST(PRINT2(c,wh_ptr->wh_spot[j].sp_letter,argv[1][i])); 
			j++;
		}
				
	}
	
	total = xw_search_blanks(puzzle_header, fp_dict, wh_ptr);

	printf("\e[2J\e[0;0H");

	for(count = 0; count < total; count++)
	{
		printf("%s\n", return_buf[count]);
	}

	if(fclose(fp_dict) EQ EOF)
	{
		TEST(WHERE); xw_error(SV_FATAL,"fclose() bombed!");
	}
}

// End of /Users/moonpie/xword/xw_lookup.c */
