# =========================================================
# Makefile for NFORenum
#
#	Don't put any local configuration in here
#	Change Makefile.local instead, it'll be
#	preserved when updating the sources
# =========================================================

MAKEFILELOCAL=Makefile.local

# Gnu compiler settings
SHELL = /bin/sh
CXX = g++
STRIP = strip
UPX = $(shell [ `which upx 2>/dev/null` ] && echo "upx")
AWK = awk
SRCZIP_FLAGS = -9
SRCZIP = gzip

# Default installation directories
INSTALL_DOCS_DIR := "$(INSTALL_DIR)/usr/share/doc/nforenum"
INSTALL_BINARY_DIR := "$(INSTALL_DIR)/usr/bin"
INSTALL_MAN_DIR := "$(INSTALL_DIR)/usr/share/man/man1"

# OS detection: Cygwin vs Linux
ISCYGWIN = $(shell [ ! -d /cygdrive/ ]; echo $$?)
ISMINGW = $(shell [ `$(CXX) -dumpmachine` != mingw32 ]; echo $$?)

# OS dependent variables
NFORENUM = $(shell [ \( $(ISCYGWIN) -eq 1 \) -o \( $(ISMINGW) -eq 1 \) ] && echo nforenum.exe || echo nforenum)

# use 386 instructions but optimize for pentium II/III
ifeq ($(ISCYGWIN),1)
FLAGS = -O1 -I $(BOOST_INCLUDE)
else
FLAGS = -O1 -idirafter$(BOOST_INCLUDE)
endif
FLAGS += -Wall -Wno-uninitialized -Wsign-compare -Wwrite-strings -Wpointer-arith -W -Wno-unused-parameter -Wformat=2
FLAGS += -D_FORTIFY_SOURCE=2

ifeq ($(shell uname),Darwin)
FLAGS += -isystem/opt/local/include
endif

CXXFLAGS := $(FLAGS) $(CXXFLAGS)

-include ${MAKEFILELOCAL}

ifeq ($(DEBUG),1)
CXXFLAGS += -g -DDEBUG
endif

# Somewhat automatic detection of the correct boost include folder
ifndef BOOST_INCLUDE
BOOST_INCLUDE=$(shell \
find /usr/include /usr/local/include /opt/local/include -maxdepth 1 -name 'boost-*' 2> /dev/null | sort -t - -k 2 | tail -n 1 )
ifeq ($(BOOST_INCLUDE),)
BOOST_INCLUDE=$(shell \
( [ -d /usr/include/boost/date_time ] && echo /usr/include ) || \
( [ -d /usr/local/include/boost/date_time ] && echo /usr/local/include ) || \
( [ -d /opt/local/include/boost/date_time ] && echo /opt/local/include ) )
endif
endif

ifeq ($(BOOST_INCLUDE),)
BOOST_ERROR = echo Error: Boost not found. Compilation will fail.
endif

ifndef V
V=0 # verbose build default off
endif

# =======================================================================
#           setup verbose/non-verbose make process
# =======================================================================

# _E = prefix for the echo [TYPE] TARGET
# _C = prefix for the actual command(s)
# _I = indentation for sub-make
# _Q = number of 'q's for UPX
# _S = sub-makes silent?
ifeq (${V},1)
	# verbose, set _C = nothing (print command), _E = comment (don't echo)
	_C=
	_E=@\#
	_Q=-qq
	_S=
else
	# not verbose, _C = @ (suppress cmd line), _E = @echo (echo type&target)
	_C=@
	_E:=@echo ${_I}
	_Q=-qqq
	_S=-s
endif

# increase indentation level for sub-makes
_I := ${_I}"	"
export _I

# standard compilation commands should be of the form
# target:	prerequisites
#	${_E} [CMD] $@
#	${_C}${CMD} ...arguments...
#
# non-standard commands (those not invoked by make all/dos/win) should
# use the regular syntax (without the ${_E} line and without the ${_C} prefix)
# because they'll be only used for testing special conditions
#
# =======================================================================

# sources to be compiled and linked
NFORENUMSRC=IDs.cpp act0.cpp act123.cpp act123_classes.cpp act5.cpp act6.cpp \
  act79D.cpp actB.cpp actF.cpp act14.cpp command.cpp data.cpp globals.cpp \
  inject.cpp messages.cpp pseudo.cpp rangedint.cpp nforenum.cpp sanity.cpp \
  strings.cpp utf8.cpp getopt.cpp help.cpp message_mgr.cpp language_mgr.cpp \
  mapescapes.cpp pseudo_seq.cpp

# targets
all: $(NFORENUM)
remake:
	$(_E) [CLEAN]
	$(_C)$(MAKE) ${_S} clean
	$(_E) [REBUILD]
	$(_C)$(MAKE) ${_S} all


${MAKEFILELOCAL}:
	@/bin/sh -c "export PATH=\"/bin\" && \
        echo ${MAKEFILELOCAL} did not exist, using defaults. Please edit it if compilation fails. && \
        cp ${MAKEFILELOCAL}.sample $@"

$(NFORENUM): $(NFORENUMSRC:%.cpp=objs/%.o)
	$(_E) [LD] $@
	$(_C)$(CXX) -o $@ $(CXXFLAGS) $^ $(LDOPT)


clean:
	rm -rf objs nforenum.* nforenum nforenum-* bundle bundles
	rm -f src/version.h

release: FORCE
	$(_E)[REBUILD] $(NFORENUM)
	$(_C)rm -f $(NFORENUM)
	$(_C)$(MAKE) $(_S)
	$(_E) [STRIP] $(NFORENUM)
	$(_C)$(STRIP) $(NFORENUM)
ifneq ($(UPX),)
	$(_E) [UPX] $(@:%_r=%)
	$(_C)$(UPX) $(_Q) --best  $(@:%_r=%)
endif

FORCE:
	@$(BOOST_ERROR)

include version.def

src/version.h: FORCE
	@echo // Autogenerated by make.  Do not edit.  Edit version.def or the Makefile instead. > $@.tmp
	@echo "#define VERSION \"$(VERSIONSTR)\"" >> $@.tmp
	@echo "#define YEARS \"2004-$(YEAR)\"" >> $@.tmp
	@(diff $@.tmp $@ > /dev/null 2>&1 && rm -f $@.tmp) || (rm -f $@ ; mv $@.tmp $@)


# Gnu compiler rules

objs/%.o : src/%.cpp
	$(_E) [CPP] $@
	$(_C)$(CXX) -c -o $@ $(CXXFLAGS) -MMD -MF $@.d -MT $@ $<

# On some installations a version.h exists in /usr/include. This one is then
# found by the dependency tracker and thus the dependencies do not contain
# a reference to version.h, so it isn't generated and compilation fails.
objs/%.o.d: src/%.cpp src/version.h
	$(_C)mkdir -p objs
	$(_E) [CPP DEP] $@
	$(_C)$(CXX) $(CXXFLAGS) -DMAKEDEP -MM -MG src/$*.cpp -MF $@

ifndef NO_MAKEFILE_DEP
-include $(NFORENUMSRC:%.cpp=objs/%.o.d)
endif

include Makefile.bundle
