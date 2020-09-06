#/Users/moonpie/xword/Makefile Wed29Aug2018 {fcG}
#/home/franx/xword/Makefile Sat31Jan2004 {fcG}

CC = clang
#######CC = gcc
CFLAGS = -g -O0
#######CFLAGS = -include version.h -Wall
#######CFLAGS = -g3 -idirafter ../source -Wall -DDEBUG=1 

OBJS = xw_buildpuzz.o xw_createlinks.o xw_error.o xw_html.o \
xw_initpuzz.o xw_listutil.o xw_matchpuzz.o xw_printpuzz.o \
xw_puzzutil.o xw_readsort.o xw_sizedict.o xw_tryword.o xw_fillpuzz.o \
xword.o

SOURCE = xword.c xw_buildpuzz.c xw_createlinks.c xw_error.c \
xw_html.c xw_initpuzz.c xw_listutil.c xw_matchpuzz.c \
xw_printpuzz.c xw_puzzutil.c xw_readsort.c xw_sizedict.c \
xw_tryword.c xw_fillpuzz.c xwindex.c version.c

xword: ${OBJS}
	${CC} ${CFLAGS} datecomp.c -o datecomp
	./datecomp ${CFLAGS} -c version.c -o version.o 
	${CC} ${CFLAGS} ${OBJS} version.o -o xword

datecomp: datecomp.c
	${CC} ${CFLAGS} datecomp.c -o datecomp

testdate:
	./datecomp -E ${CFLAGS} version.c
#######	./datecomp -E ${CFLAGS} -include version2.h version.c

xwindex: xwindex.o xw_error.o
	${CC} ${CFLAGS} xwindex.o xw_error.o  -o xwindex

testfopen: testfopen.o xw_error.o 
	${CC} ${CFLAGS} testfopen.o xw_error.o -o testfopen

xw_fillpuzz: xw_fillpuzz.o xw_error.o xw_sizedict.o  
	${CC} ${CFLAGS} xw_fillpuzz.o xw_error.o  xw_sizedict.o  -o xw_fillpuzz

testindex: testindex.o xw_error.o
	${CC} ${CFLAGS} testindex.o xw_error.o -o testindex

strip_punc: strip_punc.o
	${CC} ${CFLAGS} strip_punc.c -o strip_punc

${OBJS} xwindex.o: ../source/defs.h xword.h

bakup:
	mcopy -t -o Makefile xw*.h xw*.c a:
	mdir a:

clean:
	rm -f ${OBJS}

git:
	git add Makefile defs.h xword.h version.h datecomp.c ${SOURCE}
#######	git remote add origin https://github.com/ruby1159449570/xword.git
print:
	pr -f -e -n3 -l58 Makefile defs.h xword.h ${SOURCE} \
> xword.txt

######## End of /home/franx/Makefile ########
