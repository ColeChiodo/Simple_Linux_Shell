# File: Standard Makefile Template for C Programs
#
# ROOTNAME should the name of the program you are writing.
#
# FOPTION can be set to blank (nothing) or to any thing starting with an 
# underscore (_).  This is the suffix of your file name.
#
# With these three options above set your filename 
# to look like:  SimpleShell_main.c 
#
# RUNOPTIONS can be set to default values you want passed into the program
# this can also be overridden on the command line
#
# OBJ - You can append to this line for additional files necessary for
# your program, but only when you have multiple files.  Follow the convention
# but hard code the suffix as needed.
#
# To Use the Makefile - Edit as above
# then from the command line run:  make
# That command will build your program, and the program will be named the same
# as your main c file without an extension.
#
# You can then execute from the command line: make run
# This will actually run your program
#
# Using the command: make clean
# will delete the executable and any object files in your directory.
#

ROOTNAME=SimpleShell
FOPTION=_main
RUNOPTIONS="Prompt> "
CC=gcc
CFLAGS= -g -I.
LIBS =-l pthread
DEPS = 
OBJ = $(ROOTNAME)$(FOPTION).o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(ROOTNAME)$(FOPTION): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o $(ROOTNAME)$(FOPTION)

run: $(ROOTNAME)$(FOPTION)
	./$(ROOTNAME)$(FOPTION) $(RUNOPTIONS)

vrun: $(ROOTNAME)$(FOPTION)
	valgrind ./$(ROOTNAME)$(FOPTION) $(RUNOPTIONS)


