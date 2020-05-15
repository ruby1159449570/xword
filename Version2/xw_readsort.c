/* /home/franx/xword/xw_readsort.c Mon02Feb2004 {fcG} */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "defs.h"
#include "xword.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		READSORT		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_readsort()
{
	FILE	*fp;
	int	c;
	register	int i;
	register	WORDLIST	*ref, *new;
	
	fp = fopen(xw_inputfile, "r");
	if (fp EQ NULL)
	{
		xw_error(SV_FATAL, "Error opening %s\n", xw_inputfile);
	}
	c = getc(fp);
	while(c != EOF)
	{
		new = (WORDLIST *)xw_malloc(sizeof(WORDLIST));
		i = 0;
		new->wl_numchar = 0;
		while(c != ' ' && c != '\n' && c != '\t' && c != EOF)
		{
			if(!isalpha(c))
			{
				xw_error(SV_FATAL,
					 "Words must be alphabetic\n");
			}
			c = islower(c) ? toupper(c) : c;
#ifdef DEBUG
			printf("readsort: word[%d] is  %c\n", i, c);
#endif /* DEBUG */
			new->wl_word[i++] = c;
			new->wl_numchar++;
			xw_totletters++;
			if (i > WORDLENGTH)
			  {
			    new->wl_word[i] = '\0';
			    xw_error(SV_FATAL,"%s too long for word\n",
				     new->wl_word);
			  }
			c = getc(fp);
		}
		new->wl_word[i] = '\0';
		if (i < 3)
		{
		  xw_error(SV_FATAL,"%s not long enough for word\n",
				new->wl_word);
		}
		if (++xw_totwords > NUMWORDS)
		  {
		    xw_error(SV_FATAL, "Word limit of %d exceeded\n", NUMWORDS);
		  }

/* Input the Clue. */

		i = 0;
		if(c EQ ' ' || c EQ '\t')
		{
			c = getc(fp);
		}
		while((c != EOF) && (c != '\n'))
		{
			new->wl_clue[i++] = c;
			if(i > CLUELENGTH)
			  {
			    new->wl_clue[i] = '\0';
			    xw_error(SV_FATAL, "%s too long for clue\n",
				    new->wl_clue);
			  }
			c = getc(fp);
		}
		new->wl_clue[i] = '\0';
		if(i < 2)
		{
			xw_error(SV_FATAL, "%s not long enough for clue\n",
				new->wl_clue);
		}

		c = getc(fp);
#ifdef DEBUG
		printf("readsort: inserting  %s of length %d\n",
			new->wl_word, new->wl_numchar);
#endif /* DEBUG */

/* Now, sort according to wordlength (ie. numchar field). */
			

		ref = &xw_start;
		for(EVER)
		{
			if ((ref->wl_next EQ 0) || (ref->wl_next->wl_numchar
						    < new->wl_numchar))
			{
#ifdef DEBUG
				printf("readsort: inserting after %s of length %d\n",
					ref->wl_word, ref->wl_numchar);
#endif /* DEBUG */

/* Put it in. */
				new->wl_next = ref->wl_next;
				ref->wl_next = new;
				break;
			}
			else
			{
#ifdef DEBUG
				printf("readsort: moving past %s of length %d\n",
					ref->wl_word, ref->wl_numchar);
#endif /* DEBUG */

/* Move along */

				ref = ref->wl_next;
			}
		}
	}
	fclose(fp);

}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		KILLIST			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_killlist()
{
	WORDLIST	*ref, *tmp;

#ifdef DEBUG
	printf("killist: STARTING UP\n");
#endif /* DEBUG */

	ref = xw_start.wl_next;
	while(ref != 0)
	{
		tmp = ref->wl_next;
		TEST(PR(#010x, (UINT)ref));
		TEST(PRINT2(s, ref->wl_word, ref->wl_clue));
		free((char *)ref);
		ref = tmp;
	}
}
	
/* End of /home/franx/xword/xw_readsort.c */
