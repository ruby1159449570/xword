/* /home/franx/xword/xw_html.c Mon24May2004 {fcG} */

/* MODIFACTION HISTORY */
/* When		Who	What */
/* Tue17May2011 {fcG}	cd_word added to CLUEDESCIPTOR... */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include "defs.h"
#include "xword.h"

#define BORDER 0

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		XW_HTML			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

void xw_html(PUZZHEAD *puzz_hdr)
{
	FILE	*fp;
	PUZZLE	*hptr, *vptr;
	BOOLEAN new_word = FALSE;
	register	UINT i=0, j=0, k=0, numword=0;
	CLUEDESCRIPTOR  accrosslist[NUMWORDS], downlist[NUMWORDS];

	strncpy(xw_outputfile, xw_inputfile, SZ_FILENAME);
	strncat(xw_outputfile, ".html", SZ_FILENAME);
	printf("Writing Output to %s...\n", xw_outputfile);
	if ((fp = freopen( xw_outputfile, "w", stdout)) EQ NULL)
		{
			xw_error(SV_ERROR, "Error opening \"%s\"", xw_outputfile);
		}

	printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Final//EN\">\n");
	printf("<!-- %s %s %s {fcG} -->\n", xw_outputfile, __DATE__, __TIME__);
	printf("<HTML>\n");
	printf("<HEAD>\n");
	/* printf("<LINK REL=\"StyleSheet\" TYPE=\"text/css\" HREF=\"xword_style.css\">\n"); */
	printf("<LINK REV=\"made\" HREF=\"mailto:franxg@bigpond.com\">\n");
	printf("<META NAME=\"Author\" CONTENT=\"Frank C.Gallacher\">\n");
	printf("<TITLE> Franx X-word Generator. </TITLE>\n");
	printf("</HEAD>\n");

	printf("<BODY BGCOLOR=\"White\">\n");

	printf("<TABLE BORDER=\"%d\" BGCOLOR=\"Black\">", BORDER);

	hptr = puzz_hdr->ph_puzzle;

	if(puzz_hdr->ph_numwords != 1)
	{
		hptr = hptr->pz_down;
	}
	vptr = hptr->pz_right;
	while(((hptr != NULL) && (puzz_hdr->ph_numwords EQ 1))
		|| ((hptr->pz_down != NULL) && (puzz_hdr->ph_numwords != 1)))
	{
	  printf("<TR>\n");
	  while(vptr->pz_right != NULL)
	    {
	      new_word = FALSE;
	      if((vptr->pz_letter != '\0') && (vptr->pz_letter != EOWORD))
		{
		  if (vptr->pz_ofaccrossword != NULL)
		    {
		      accrosslist[j].cd_clue = vptr->pz_ofaccrossword->wl_clue;
			accrosslist[j].cd_word = vptr->pz_ofaccrossword->wl_word;
		      accrosslist[j++].cd_number = ++numword;
		      new_word = TRUE;
		    }
		  if (vptr->pz_ofdownword != NULL)
		    {
		      if (!new_word)
			{
			  numword++;
			}
		      downlist[k].cd_clue = vptr->pz_ofdownword->wl_clue;
			downlist[k].cd_word = vptr->pz_ofdownword->wl_word;
		      downlist[k++].cd_number = numword;
		      new_word = TRUE;
		    }
		  
		  printf("<TD BGCOLOR=\"White\" ");
		  
		  printf("<FONT COLOR=\"Black\" FACE=\"Courier\" SIZE=\"2\">");
		  if (new_word)
		    {
		      printf("%d<BR>&nbsp;&nbsp;&nbsp;", numword);
		    }
		  else
		    {
		      printf("&nbsp;&nbsp;&nbsp;<BR>&nbsp;&nbsp;&nbsp;");
		    }
		  printf("</FONT>\n");
		  
		  
		  printf("</TD>");
		}
	      else
		{
		  printf("<TD>");
		  printf("&nbsp;");
		  printf("</TD>");
		}
	      vptr = vptr->pz_right;
	    }
	  printf("</TR>\n");
	  hptr = hptr->pz_down;
	  if (hptr != NULL)
	    {
	      vptr = hptr->pz_right;
	    }
	  else
	    {
	      break;
	    }
	}
	printf("\n");

	printf("</TABLE>\n");

	printf("<TABLE BORDER=\"0\" BGCOLOR=\"White\">");
	printf("<TH> Across </TH>\n");
	printf("<TH> Down </TH>\n");
	
	for (i=0;i<MAX(j,k); i++)
	{
	  printf("<TR>\n");
	  
	  if (i < j)
	    {
	      printf("<TD>%d.%s</TD>", accrosslist[i].cd_number, accrosslist[i].cd_clue);
		fprintf(stderr, "%s %dA\n", 
			accrosslist[i].cd_word,
			accrosslist[i].cd_number);
	    }
	  else
	    {
	      printf("<TD>&nbsp;</TD>\n");
	    }

	  if (i < k)
	    {
	      printf("<TD>%d.%s</TD>", downlist[i].cd_number, downlist[i].cd_clue );
		fprintf(stderr, "%s %dD\n", 
			downlist[i].cd_word,
			downlist[i].cd_number);
	    }
	  else
	    {
	      printf("<TD>&nbsp;</TD>\n");
	    }
	  printf("</TR>\n");
	}

		     
	printf("</TABLE>\n");

	printf("<HR>\n");


	printf("<TABLE BORDER=\"%d\" BGCOLOR=\"Black\">", BORDER);
	hptr = puzz_hdr->ph_puzzle;

	if(puzz_hdr->ph_numwords != 1)
	{
		hptr = hptr->pz_down;
	}
	vptr = hptr->pz_right;
	while(((hptr != NULL) && (puzz_hdr->ph_numwords EQ 1))
		|| ((hptr->pz_down != NULL) && (puzz_hdr->ph_numwords != 1)))
	{
	  printf("<TR>\n");
	  while(vptr->pz_right != NULL)
	    {
	      if((vptr->pz_letter != '\0') && (vptr->pz_letter != EOWORD))
		{
		  printf("<TD BGCOLOR=\"White\" ");
		  
		  printf("<FONT COLOR=\"Black\" FACE=\"Arial\" SIZE=\"2\">");
		  
		  printf("%c", vptr->pz_letter);
		  printf("</FONT>\n");
		  
		  
		  printf("</TD>");
		}
	      else
		{
		  printf("<TD>");
		  printf("&nbsp;");
		  printf("</TD>");
		}
	      vptr = vptr->pz_right;
	    }
	  printf("</TR>\n");
	  hptr = hptr->pz_down;
	  if (hptr != NULL)
	    {
	      vptr = hptr->pz_right;
	    }
	  else
	    {
	      break;
	    }
	}
	printf("\n");

	printf("</TABLE>\n");

	printf("</BODY>\n");
	printf("</HTML>\n");
	fclose(fp);
}

/* End of /home/franx/xword/xw_html.c */
