#########################################################
# File: Makefile
# Description: A more robust makefile for CS162.
# You need to edit the variables under "USER SECTION".
# To execute the makefile on unix-y systems, type 'make'.
# If that doesn't work, 'make -f makefile' might be useful.
#########################################################
# #'s are line-comments
# CXX is a standard variable name used for the compiler.
CXX = gcc
# CXXFLAGS is a standard variable name for compile flags.
# -std=c++0x specifies to use a certain language version.
#CXXFLAGS = -std=c++0x
# -Wall turns on all warnings
CXXFLAGS = -Wall
# -pedantic-errors strictly enforces the standard
#CXXFLAGS += -pedantic-errors
# -g turns on debug information
CXXFLAGS += -g
# turns on macro linking library
LIBS = -lm
####################
### USER SECTION ###
####################
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

# These will be your executable names.
# Edit as needed.
PROG1 = smallsh
PROG2 =
PROG3 =
PROGS = ${PROG1} ${PROG2} ${PROG3}
# This is the name of your document(s).
# Edit as needed.
DOC = README
# This is the name of your compressed file.
# Edit name as needed. Keep the format.
TAR = smallsh.tar.bz2
#####################
### BUILD SECTION ###
#####################
%.o : %.c $(HEADERS)
	${CXX} -c $< ${CXXFLAGS} -o $@

# Typing 'make' in terminal calls the first build availible.
# In this case, default.
default: ${PROG1} 

${PROG1}:${OBJS}
	${CXX} ${OBJS} -Wall ${LIBS} -o $@

# Typing 'make all' in terminal calls this build.
all: default

# Typing 'make tar' in terminal calls this build.
# This creates a compressed file for submission.
tar:
	tar cvjf ${TAR} ${SRCS} ${HEADERS} ${DOC} makefile

# Typing 'make clean' calls this build.
# It's designed to clean up the folder.
# Be careful with this, edit as needed.
.PHONY: clean
clean:
	rm -f ${PROGS} *.o *~
