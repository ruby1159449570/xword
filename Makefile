#/Users/moonpie/xword/Makefile Wed29Aug2018 {fcG}
#/home/franx/xword/Makefile Sat31Jan2004 {fcG}

#######CC = gcc
CC = clang
CFLAGS = -idirafter ../source -Wall
#######CFLAGS = -g3 -idirafter ../source -Wall -DDEBUG=1 
OBJS = xw_buildpuzz.o xw_createlinks.o xw_error.o xw_html.o \
xw_initpuzz.o xw_listutil.o xw_matchpuzz.o xw_printpuzz.o xw_puzzutil.o \
xw_readsort.o xw_tryword.o xw_fillpuzz.o xword.o

SOURCE = xword.c xw_buildpuzz.c xw_createlinks.c xw_error.c xw_html.c \
xw_initpuzz.c xw_listutil.c xw_matchpuzz.c xw_printpuzz.c xw_puzzutil.c \
xw_readsort.c xw_tryword.c xw_fillpuzz.c

xword: ${OBJS}
	${CC} ${CFLAGS} ${OBJS}  -o xword

xwindex: xwindex.o xw_error.o
	${CC} ${CFLAGS} xwindex.o xw_error.o  -o xwindex

testindex: testindex.o xw_error.o
	${CC} ${CFLAGS} testindex.o xw_error.o -o testindex

${OBJS} xwindex.o: ../source/defs.h xword.h

bakup:
	mcopy -t -o Makefile xw*.h xw*.c a:
	mdir a:

clean:
	rm -f ${OBJS}

git:
	git add xword.h ${SOURCE} 
print:
	pr -f -l 44 Makefile ../source/defs.h xword.h ${SOURCE} > xword.txt

######## End of /home/franx/Makefile ########
