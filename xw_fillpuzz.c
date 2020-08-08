/* /home/franx/xword/xw_fillpuzz.c Fri28Jul2006 {fcG} */
//
//  Created by Frank Charles Gallacher on 22/5/20.
//  Copyleft © 2020 Frank Charles Gallacher. All rights reserved.
//
// MODIFICATION HISTORY
// When		Who	What
// Sat18Jul2020 {fcG}	xw_search_blanks() implemented...
// Fri31Jul2020 {fcG}	debugged clear_word(), changed AND(&&) to OR(||).

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
//#define	extern
#include "xword.h"

#define	NOT_WHITE(x)	((x) != '\0' && (x) != EOWORD)
#define	SZ_MAXRETSEARCH 30

typedef struct spot
{
  char sp_letter;
  int  sp_pos;
} SPOT;

typedef struct wordhole
{
  int wh_key, wh_rownum, wh_colnum, wh_length, wh_spots;
  STATUS wh_status;
  struct wordhole *wh_next;
  struct spot wh_spot[WORDLENGTH];
 
} WORDHOLE;

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
			if (numspots != 0 && numspots != numletters)
			{
				wh_ptr = xw_inserthole(numspots, numletters,
				wh_ptr, pzptr, stat);
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
					wh_ptr->wh_spot[j].sp_letter = '\0';
					wh_ptr->wh_spot[j].sp_pos = 0;
				}
			}
			if (stat EQ ACROSS)
			{
				wh_ptr->wh_rownum = pzptr->pz_rownum;
				wh_ptr->wh_colnum = pzptr->pz_colnum+1;
			}
			else
			{
				wh_ptr->wh_rownum = pzptr->pz_rownum+1;
				wh_ptr->wh_colnum = pzptr->pz_colnum;
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
				wh_ptr->wh_spot[numspots++].sp_letter
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
	PUZZLE *hptr, *vptr, *ptr_ptr;
	register int i, j;

	hptr = ph->ph_puzzle;
	if(ph->ph_numrows != 1)
	{
		hptr = hptr->pz_down;
	}
	vptr = hptr->pz_right;
	for(i = 0; i < ph->ph_numcols - 2; i++)
	{
		ptr_ptr = vptr;
		for(j = 0; j < ph->ph_numrows - i - 2; j++)
		{
			xw_searchrank(ptr_ptr, DOWN, ph->ph_numrows-j-2);
			ptr_ptr = ptr_ptr->pz_down;
		}
		vptr = vptr->pz_right;
	}
	vptr = hptr->pz_right;
	for(i = 0; i < ph->ph_numrows - 2; i++)
	{
		ptr_ptr = vptr;
		for(j = 0; j < ph->ph_numcols - i - 2; j++)
		{
			xw_searchrank(ptr_ptr, ACROSS, ph->ph_numcols-j-2);
			ptr_ptr = ptr_ptr->pz_down;
		}
		vptr = vptr->pz_down;
	}
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		CLEAR			#
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

int equivalence_check(int length, long offset[20])
{
register int i,j,k;
long min;

//	TEST(PRINT2(#018lx, offset[0], offset[1]));
//	TEST(PRINT3(#06lx, offset[0], offset[1], offset[2]));

min = MAXLONG;
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
#ifdef	SNARK

		if (ref->pz_right != 0)
		{
			TEST(WHERE); TEST(PRINT1(c,
				ref->pz_right->pz_letter));
			TEST(PRINT1(d, ref->pz_right->pz_letter));
			if (NOT_WHITE(ref->pz_right->pz_letter))
			{
				TEST(printf("Across end bombed\n"));
				search = FALSE;
			}	
		}	
#endif	/* SNARK */

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
#ifdef	SNARK

		if (ref->pz_down != 0)
		{
			TEST(WHERE); TEST(PRINT1(c,
				ref->pz_down->pz_letter));
			TEST(PRINT1(d, ref->pz_down->pz_letter));
			if (NOT_WHITE(ref->pz_down->pz_letter))
			{
				TEST(printf("Down end bombed!\n"));
				search = FALSE;
			}	
		}	
#endif	/* SNARK */

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
	minoffset = MAXLONG;
	equivalent = FALSE;
	search = TRUE;
	search_count = 0;

	for(count = 0; count < wh_ptr->wh_spots; count++)
	{
		INDEXDIR(wh_ptr->wh_spot[count].sp_pos);
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
			fread(&offset[count],
			sizeof(offset[count]), 1, fp[count]);
			if (ferror(fp[count]))
			{
				INDEXDIR(wh_ptr->wh_spot[count].sp_pos);
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
			if ((min_count = equivalence_check(wh_ptr->wh_spots,
			&offset)) EQ wh_ptr->wh_spots)
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
				sprintf(buf, "%s/%c", xw_indexdir, 
					wh_ptr->wh_spot[min_count].sp_letter);
				xw_error(SV_FATAL, "Error reading %s\n",
						buf);
			}		
			if (feof(fp[min_count]))
			{
				search = FALSE;
				TEST(WHERE); TEST(PRINT1(d, min_count));
				TEST(fprintf(stderr, "Search failed! EOF\n"));
				break;
			}
			if ((min_count =
			equivalence_check(wh_ptr->wh_spots, &offset))
			EQ wh_ptr->wh_spots)
			{
				equivalent = TRUE;
			}
		}
		if (equivalent && search)
		{
			TEST(WHERE); TEST(PRINT1(#018lx, offset[0]));
			if (fseek(fp_dict, offset[0], SEEK_SET) != 0)
			{
				TEST(WHERE);xw_error(SV_ERROR, "fseek() error");
			}
			if(!fgets(buf, sizeof(buf), fp_dict))
			{
				TEST(WHERE);xw_error(SV_ERROR, "fgets() error");
			}
			clean(buf);
			if(search_count < SZ_MAXRETSEARCH
			&& clear_word(ph, buf, wh_ptr))
			{
				strncpy(&return_buf[search_count][0], buf,
					sizeof(return_buf[0]) - 1);
				search_count++;
			}
			TEST(fprintf(stderr, "\"%s\"[%#018lx]==>%s\n",xw_indexfile,
				offset[0],buf));
			do
			{
				for(count = 0; count < wh_ptr->wh_spots; count++)
				{
					fread(&offset[count],
					sizeof(offset[count]), 1, fp[count]);
					if (ferror(fp[count]))
					{
						TEST(WHERE); INDEXDIR
						(wh_ptr->wh_spot[count].sp_pos);

						sprintf(buf, "%s/%c", xw_indexdir, 
						wh_ptr->wh_spot[count].sp_letter);
						xw_error(SV_FATAL,
						"Error reading %s\n", buf);
					}		
					if (feof(fp[count]))
					{
						search = FALSE;
						TEST(WHERE); TEST(fprintf(stderr,
						"Search failed! EOF\n"));
						break;
					}
				}
				if (equivalence_check(wh_ptr->wh_spots, &offset)
				EQ wh_ptr->wh_spots && search)
				{
					equivalent = TRUE;
				}
				else
				{
					equivalent = FALSE;
					break;
				}

				if (fseek(fp_dict, offset[0], SEEK_SET) != 0)
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
					strncpy(&return_buf[search_count][0], buf,
						sizeof(return_buf[0]) - 1);
					search_count++;
				}
				TEST(fprintf(stderr, "\"%s\"[%#020lx]==>%s\n",
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
	TEST(WHERE); TEST(PRINT1(d, search_count));
	return search_count;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_FILLPUZZ		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

PUZZHEAD *xw_fillpuzz(PUZZHEAD *ph)
{
	register WORDHOLE *wh_ptr = &xw_whstart;
	FILE *fp_dict;
	int	count, min_count, num_matches;
//	register WORDHOLE *oldwh_ptr;
	int	oldrownum, oldcolnum, oldlength;
	STATUS	oldstatus;

TRY_AGAIN:
	max = 0;
	if((max = xw_sizedict(xw_indexfile)) > WORDLENGTH)
	{
		xw_error(SV_FATAL,
		"Words in dictionary are too long... %d > %d\n",
		max, WORDLENGTH);
	}
	if(!max)	
	{
		xw_error(SV_FATAL, "Error in Dictionary %s\n",	
			xw_indexfile);
	}
	WHERE; PRINT1(d, max);
	xw_findgaps(ph);

	wh_ptr = &xw_whstart;
	TEST(WHERE); TEST(PR(#010lx, wh_ptr));
	wh_ptr = wh_ptr->wh_next;
	while (wh_ptr != NULL)
	{
		register int i,j, k;

		TEST(printf("%d %s (%d,%d) ", wh_ptr->wh_key,
		DECODE(wh_ptr->wh_status), wh_ptr->wh_colnum,
		wh_ptr->wh_rownum));

		for(i=0; i < wh_ptr->wh_key / 100; i++)
		{
			TEST(printf("\"%c\"[%#04x]->%d ",
			wh_ptr->wh_spot[i].sp_letter,
			wh_ptr->wh_spot[i].sp_letter,
			wh_ptr->wh_spot[i].sp_pos));
		}
		for(j = k = 0; j < wh_ptr->wh_key % 100; j++)
		{
			if (wh_ptr->wh_spot[k].sp_pos - 1 EQ j)
			{
				buf[j] = wh_ptr->wh_spot[k++].sp_letter;
			}
			else
			{
			  buf[j] = '?';
			}
		}
		buf[j] = '\0';
		TEST(printf("%s\n", buf));

		wh_ptr->wh_spots = k;
		wh_ptr->wh_length = j;
		TEST(PRINT2(d, wh_ptr->wh_spots, wh_ptr->wh_length));
		wh_ptr = wh_ptr->wh_next;
	}

	wh_ptr = &xw_whstart;
	wh_ptr = wh_ptr->wh_next;
	TEST(WHERE); TEST(PR(#010lx, wh_ptr));
	if (!(fp_dict = fopen(xw_indexfile,"rb")))
	{
		TEST(WHERE);xw_error(SV_ERROR, "index file open error/n");
	}

	num_matches = 0;
//	oldwh_ptr = 0;
	oldcolnum = ph->ph_lastword->wl_xpos;
	oldrownum = ph->ph_lastword->wl_ypos;
	oldlength = 0;
//	oldlength = ph->ph_lastword->wl_numchar;
//	xw_printpuzz(ph);
	while (wh_ptr != NULL)
//	while (num_matches EQ 0)
	{
		register int	i;

		if((num_matches
		= xw_search_blanks(ph, fp_dict, wh_ptr)) > 0)
		{
			int	choice;
			PUZZLE	*ref, *oldref;
			WORDLIST	word; 
			
			TEST(WHERE); TEST(PRINT1(d, num_matches));

			oldref = xw_pointpuzz(ph, oldcolnum, oldrownum);
			for (i = 0; i < oldlength; i++)
			{
				if (oldref != 0)
				{
					oldref->pz_color = RED;
					if (oldstatus EQ ACROSS)
					{
						oldref = oldref->pz_right;
					}
					else
					{
						oldref = oldref->pz_down;
					}
	
				}
			}
			ref = xw_pointpuzz(ph, wh_ptr->wh_colnum,
				wh_ptr->wh_rownum);
			if(ref != 0)
			{
				ref->pz_color = GREEN;
			}
			xw_printpuzz(ph);

			ref = xw_pointpuzz(ph, wh_ptr->wh_colnum,
				wh_ptr->wh_rownum);
			if(ref != 0)
			{
				ref->pz_color = 0;
			}
			oldref = xw_pointpuzz(ph, oldcolnum, oldrownum);
			for (i = 0; i < oldlength; i++)
			{
				if (oldref != 0)
				{
					oldref->pz_color = 0;
					if (oldstatus EQ ACROSS)
					{
						oldref = oldref->pz_right;
					}
					else
					{
						oldref = oldref->pz_down;
					}
				}
			}
	
			printf("\033[7mEnter # of the word to insert,");
			printf("0 to skip, <CTRL-C> to save & exit\n");
			printf("\033[0m");
			for (i = 0; i < num_matches; i++)
			{
//				TEST(PRINT1(s, &return_buf[i][0]));
				printf("(%d) %s\t", i + 1, return_buf[i]);
				if (((i + 1) % 4) EQ 0)
				{
					nL;
				}
			}
			do
			{
				printf("\n\007xword> ");
				i = scanf("%d", &choice);
			}	while(i != 1 && (choice < 0
				&& choice > num_matches));
			TEST(PRINT1(d, choice));
			if(choice EQ 0 || choice > num_matches)
			{
				wh_ptr = wh_ptr->wh_next;
				continue;
/* 				return ph; */
			}
			else
			{
				choice--;
			}
			word.wl_numchar = strlen(return_buf[choice]);
			strncpy(&word.wl_word[0], &return_buf[choice][0],
				sizeof(return_buf[choice]) - 1);
			sprintf(word.wl_clue, "# %s #",return_buf[choice]);
			word.wl_status = wh_ptr->wh_status;

			xw_putwordin(ph, &word, wh_ptr->wh_status,
			wh_ptr->wh_colnum,  wh_ptr->wh_rownum);
			
//			oldwh_ptr = wh_ptr;
			oldcolnum = wh_ptr->wh_colnum,  
			oldrownum = wh_ptr->wh_rownum;
			oldlength = word.wl_numchar;
			oldstatus = wh_ptr->wh_status;
		}
		wh_ptr = wh_ptr->wh_next;
		TEST(PR(#010lx, wh_ptr));
	}
	xw_printpuzz(ph);

	if(fclose(fp_dict) EQ EOF)
	{
		TEST(WHERE); xw_error(SV_FATAL,"fclose() bombed!");
	}
	goto TRY_AGAIN;
	return ph;
}	

#ifdef	SNARK
main()
{
	BOOLEAN	james;
	FILE	*fp_dict;
	long	offset[WORDLENGTH],minoffset;
	PUZZHEAD	puzzle_header;
	WORDHOLE testdata;
	register WORDHOLE *wh_ptr = &xw_whstart;

	offset[0] = 4;
	offset[1] = 3;
	offset[2] = 2;
	offset[3] = 4;
	PRINT1(d, equivalence_check(4, &offset));

	offset[0] = 5;
	offset[1] = 5;
	offset[2] = 5;
	offset[3] = 5;
	PRINT1(d, equivalence_check(4, &offset));

	wh_ptr = &xw_whstart;
	wh_ptr->wh_next = &testdata;
	testdata.wh_key = 1010;
	testdata.wh_rownum = testdata.wh_colnum = 10;
	testdata.wh_status = ACROSS;
	testdata.wh_next = NULL;

	testdata.wh_spots = 1;
	testdata.wh_length = testdata.wh_colnum = 10;
	testdata.wh_status = ACROSS;
	testdata.wh_spot[0].sp_pos = 1;
	testdata.wh_spot[0].sp_letter = 'B';
	testdata.wh_spot[1].sp_pos = 2;
	testdata.wh_spot[1].sp_letter = 'C';
	testdata.wh_spot[2].sp_pos = 3;
	testdata.wh_spot[2].sp_letter = 'C';

	strncpy(xw_indexfile, "john.sort", 16);

	if (!(fp_dict = fopen(xw_indexfile,"rb")))
	{
		TEST(WHERE);xw_error(SV_ERROR, "index file open error/n");
	}

	wh_ptr = wh_ptr->wh_next;
	james = xw_search_blanks(&puzzle_header, fp_dict, wh_ptr);

	if(fclose(fp_dict) EQ EOF)
	{
		TEST(WHERE); xw_error(SV_FATAL,"fclose() bombed!");
	}
}
#endif	/* SNARK */ 

/* End of /home/franx/xword/xw_fillpuzz.c */
