/* /home/franx/xword/xword.h Mon26Jan2004 {fcG} */

#ifndef _XWORD_H
#define _XWORD_H 1
#include <setjmp.h>
#include <time.h>
#include "defs.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		DEFINES			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define	CLUELENGTH	200
#define	WORDLENGTH	20
#define	NUMLETTERS	26
#define	EOWORD		'@'
#define	NUMWORDS	100
#define	UNDEFINED	32767
#define	ORD(x)		((x) - 'A')
#define	INV(x)		((x) EQ ACCROSS ? DOWN : ACCROSS)
#define	DECODE(x)	((x) EQ UNUSED ? "UNUSED"  : \
                        (x) EQ ACCROSS ? "ACCROSS" : \
                        (x) EQ DOWN ? "DOWN"       : \
                        "???")

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		TYPEDEFS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

typedef	enum severity { SV_INFO, SV_WARNING, SV_ERROR, SV_FATAL } SEVERITY;

typedef	enum status { UNUSED = 0, ACCROSS = 1, DOWN = 2 } STATUS;

typedef	struct	cluedescriptor
{
	char	*cd_clue;
	UINT	cd_number;
}	CLUEDESCRIPTOR;

typedef	struct	puzzle
{
	char	pz_letter;
	int	pz_rownum, pz_colnum;
	struct	puzzle	*pz_up, *pz_down, *pz_left, *pz_right;
	struct	wordlist *pz_ofaccrossword, *pz_ofdownword;
}	PUZZLE;

typedef	struct	puzzhead
{
	struct puzzhead	*ph_prevpuzz;
        struct puzzle   *ph_puzzle;
	struct wordlist *ph_lastword;
        UINT		ph_numcols, ph_numrows, ph_numwords, ph_numletters;
        float		ph_compact;
}       PUZZHEAD;

typedef	 struct	wordfit
{
        int	wf_size, wf_x, wf_y;
	STATUS	wf_status;
}	WORDFIT;

typedef	struct	wordlist
{
	int wl_numchar;
	char	wl_word[WORDLENGTH+1], wl_clue[CLUELENGTH+1];
	STATUS	wl_status;
	int	wl_xpos, wl_ypos;
	struct	wordlist *wl_prev, *wl_next, *wl_sameletter[WORDLENGTH];
}	WORDLIST;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		EXTERNS			#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/*  extern	PUZZHEAD *header; */
extern	WORDFIT xw_best
#ifdef	extern
=
{
	0, 0, 0, UNUSED
};
#else
	;
#endif

extern	WORDFIT xw_ret
#ifdef	extern
=
{
	0, 0, 0, UNUSED
};
#else
	;
#endif

extern	WORDLIST xw_start
#ifdef	extern
=
{
  WORDLENGTH+1, "", "", UNUSED, UNDEFINED, UNDEFINED, 0, 0, NULL
/*    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, */
/*     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL} */
};
#else
	;
#endif
extern	char	xw_inputfile[SZ_FILENAME+1],xw_outputfile[SZ_FILENAME+1];
extern	int	xw_puzz_compact_req, xw_totletters, xw_totwords;
extern	time_t	xw_starttime;
extern	jmp_buf	xw_env;

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\
#		FUNCTIONS		#
#					#
\*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

int	main(int, char*[]);
PUZZHEAD *xw_copypuzz(PUZZHEAD *);
PUZZHEAD *xw_initpuzz(WORDLIST *);
PUZZLE	*xw_pointpuzz(PUZZHEAD *, int, int);
void	xw_createcolumn(PUZZHEAD *, int);
void	xw_createlinks();
void	xw_createrow(PUZZHEAD *, int);
void	*xw_error(SEVERITY, char *, ...);
void	xw_feeldown(PUZZHEAD *, WORDLIST *, int, PUZZLE *);
void	xw_feelleft(PUZZHEAD *, WORDLIST *, int, PUZZLE *);
void	xw_feelright(PUZZHEAD *, WORDLIST *, int, PUZZLE *);
void	xw_feelup(PUZZHEAD *, WORDLIST *, int, PUZZLE *);
void	xw_killlist();
void	xw_killpuzz(PUZZHEAD *);
void	*xw_malloc(size_t);
void	xw_printpuzz(PUZZHEAD *);
void	xw_pushlist(WORDLIST *);
void	xw_putwordin(PUZZHEAD *,WORDLIST *, STATUS, int, int);
void	xw_readsort();
void	xw_restartlist();
void	xw_tryword(PUZZHEAD *,WORDLIST *, int, WORDLIST *);
void	xw_html(PUZZHEAD *);
WORDLIST	*xw_buildpuzz(PUZZHEAD *, WORDLIST *);
WORDLIST	*xw_poplist();

#endif /* _XWORD_H */

/* End of /home/franx/xword/xword.h */

