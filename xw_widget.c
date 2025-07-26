/* /Users/moonpie/xword/xw_widget.c Sun19Jun2022 {fcG} */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "defs.h"
#include "xword.h"

char buf[SZ_FILENAME+1] = "#!/bin/sh\n"\
"# the next line restarts using wish \\\n"\
"exec wish8.5 \"$0\" \"$@\"\n"\
"\n"\
"# hello --\n"\
"# Simple Tk script to create a button that prints \"Hello, world\".\n"\
"# Click on the button to terminate the program.\n"\
"#\n"\
"# RCS: @(#) $Id$\n"\
"\n"\
"package require Tk\n"\
"\n"\
"# The first line below creates the button, and the second line\n"\
"# asks the packer to shrink-wrap the application's main window\n"\
"# around the button.\n"\
"\n"\
"button .hello -text \"Goodbye, world\" -command {\n"\
"    puts stdout \"Goodbye, world\"; destroy .\n"\
"}\n"\
"pack .hello\n"\
"# Local Variables:\n"
"# mode: tcl\n"
"# End:";

int main(int argc, char *argv[])
{	
	FILE *fp;
	fp = fopen("hello_widget", "wb");
	fputs(buf, fp);
	fclose(fp);

	system("tclsh hello_widget");
/* 	system("tclsh hello_widget&"); */
}

/* End of /Users/moonpie/xword/xw_widget.c */
