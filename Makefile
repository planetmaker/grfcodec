# Makefile for NFORenum

# A command to return the current SVN revision of the source tree; it should
# it in the format [##:]##, where the second set of digits is the current
# revision (used for adding the revision to the version string)
SVNVERSION = svnversion .	# standard SVN client (e.g. cygwin)
# SVNVERSION = SubWCRev.exe .	# TortoiseSVN

# Optional parts of NFORenum are dependent on boost::date_time
# If you do not have boost, make should detect this, and compile a version
# of grfcodec that does not include the boost-dependent parts.
# Get boost from www.boost.org
#
# Set these as appropriate; BOOST_VERSION is used to help automatically
# locate your boost include directory. The usual format is x_yy for x.yy.0
# versions and x_yy_z for x.yy.z releases
BOOST_VERSION = 1_33_1
# If that fails, set your boost include folder here.
#BOOST_INCLUDE = 

# OS detection: Cygwin vs Linux
ISCYGWIN = $(shell [ ! -d /cygdrive/ ]; echo $$?)

# OS dependent variables
NFORENUM = $(shell [ \( $(ISCYGWIN) -eq 1 \) ] && echo renum.exe || echo renum)

# Gnu compiler settings
SHELL = /bin/sh
CC = g++
CXX = g++

# use 386 instructions but optimize for pentium II/III
CFLAGS = -g -DWIN32 -O1 $(BOOST_CMD) -Wall -Wno-uninitialized $(CFLAGAPP)
CXXFLAGS = $(CFLAGS)
#LDOPT = -g -Wl,--subsystem,console -luser32 -lgdi32 -lwinmm -lcomdlg32 -lcomctl32
#LDOPT = -Wl,--subsystem,console,-s
#LDOPT += -Wl,-Map,$(@:%=%.map)		# to make map files

# for profiling
#CFLAGS += -pg
#LDOPT += -pg

# Somewhat automatic detection of the correct boost include folder
ifndef BOOST_INCLUDE
BOOST_INCLUDE=$(shell \
( [ -d /usr/local/include/boost-$(BOOST_VERSION)/boost ] && echo /usr/local/include/boost-$(BOOST_VERSION) ) || \
( [ -d /usr/include/boost-$(BOOST_VERSION)/boost ] && echo /usr/include/boost-$(BOOST_VERSION) ) || \
echo CANNOT_FIND_BOOST_INCLUDE_DIRECTORY )
endif


# sources to be compiled and linked
NFORENUMSRC=IDs.cpp act0.cpp act123.cpp act123_classes.cpp act5.cpp act6.cpp \
  act79D.cpp actB.cpp actF.cpp command.cpp data.cpp globals.cpp inject.cpp \
  messages.cpp pseudo.cpp rangedint.cpp renum.cpp sanity.cpp strings.cpp \
  utf8.cpp getopt.cpp

ifndef NOREV
NOREV = 0
endif

ifeq ($(BOOST_INCLUDE),CANNOT_FIND_BOOST_INCLUDE_DIRECTORY)
BOOST_CMD=-DNO_BOOST
BOOST_WARN = @echo " ** Warning: This NFORenum is being built without \\w<YMD> or \\w<DMY> support."
else
BOOST_CMD=-I$(BOOST_INCLUDE)
endif

# targets
all: renum
remake: clean all


renum:	$(NFORENUMSRC:%.cpp=%.o)
	$(CXX) -o $@ $(CFLAGS) $^ $(LDOPT)


clean:
	rm -rf *.o *.exe *.EXE renum

release:
	rm -f $(NFORENUM) version.h
	make NOREV=$(NOREV)
	strip $(NFORENUM)
	upx --best $(NFORENUM)

FORCE:
.rev: FORCE
	@ [ -e $@ ] || echo SVNREV=0 > $@
	@ REV=`${SVNVERSION}` perl rev.pl $@ < $@

-include .rev
include version.def

version.h: version.def .rev
	@echo // Autogenerated by make.  Do not edit.  Edit version.def or the Makefile instead. > $@
	@echo "#define VERSION \"v$(VERSIONSTR)\"" >> $@
	@echo "#define YEARS \"2004-$(YEAR)\"" >> $@


# Gnu compiler rules

%.o : %.c
	$(CC) -c -o $@ $(CFLAGS) $<

%.o : %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $<

pseudo.o: pseudo.cpp
	$(BOOST_WARN)
	$(CXX) -c -o $@ $(CXXFLAGS) $<	

.remake_deps:
	touch .remake_deps

Makefile.dep: .remake_deps
	$(CC) $(CFLAGS) -MM -MG *.c *.cpp | sed -e "s/[-a-zA-Z0-9_/]*boost\/[-a-zA-Z0-9_/]*\.hpp//g" -e "s/[a-zA-Z0-9_]*\.cpp//g" > $@

ifndef NO_MAKEFILE_DEP
-include Makefile.dep
endif
