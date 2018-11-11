#----------------------------------------------------------------
# project ....: LTI Digital Image/Signal Processing Library
# file .......: Template Makefile for Examples
# authors ....: Pablo Alvarado, Jochen Wickel
# organization: LTI, RWTH Aachen
# creation ...: 09.02.2003
# revisions ..: $Id: Makefile.in,v 1.2 2008/09/23 19:25:58 alvarado Exp $
#----------------------------------------------------------------

#Please modify the following two variables if necessary:

#Base Directory
LTIBASE:=$(HOME)/ltilib-2

#Path to LTI configuration script
LTICMD:=$(LTIBASE)/linux/lti-local-config
#LTICMD:=/usr/local/bin/lti-config

#Example name
PACKAGE:=$(shell basename $$PWD)

# If you want to generate a debug version, uncomment the next line
# BUILDRELEASE=yes

# Compiler to be used
CXX:=g++

# For new versions of gcc, <limits> already exists, but in older
# versions a replacement is needed
CXX_MAJOR:=$(shell echo `$(CXX) --version | sed -e 's/\..*//;'`)

ifeq "$(CXX_MAJOR)" "2"
  VPATHADDON=:g++
  CPUARCH = -march=i686 -ftemplate-depth-35
  CPUARCHD = -march=i686 -ftemplate-depth-35
else
  ifeq "$(CXX_MAJOR)" "3"
  VPATHADDON=
  CPUARCH = -march=pentium4
  CPUARCHD = -march=pentium4
  else
  VPATHADDON=
  CPUARCH = -march=native
  CPUARCHD = 
  endif
endif

# Directories with source file code (.h and .cpp)
VPATH:=$(VPATHADDON)

# Destination directories for the debug and release versions of the code

OBJDIR  = ./

# Extra include directories and library directories for hardware specific stuff

EXTRAINCLUDEPATH =
EXTRALIBPATH =
EXTRALIBS    =

#EXTRAINCLUDEPATH = -I/usr/src/menable/include
#EXTRALIBPATH = -L/usr/src/menable/lib
#EXTRALIBS =  -lpulnixchanneltmc6700 -lmenable


# PROFILE = -p
PROFILE=

# compiler flags
CXXINCLUDE:=$(EXTRAINCLUDEPATH) $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LINKDIR:=-L$(LTIBASE)/lib
CPPFILES=$(wildcard ./*.cpp)
OBJFILES=$(patsubst %.cpp,$(OBJDIR)%.o,$(notdir $(CPPFILES)))

# set the compiler/linker flags depending on the debug/release flag
ifeq "$(BUILDRELEASE)" "yes"
  LTICXXFLAGS:=$(shell $(LTICMD) --cxxflags)
  CXXFLAGSREL:=-c -O3 $(CPUARCH) -Wall -ansi $(LTICXXFLAGS) $(CXXINCLUDE)
  GCC:=$(CXX) $(CXXFLAGSREL) $(PROFILE)
  LIBS:=$(shell $(LTICMD) --libs) $(EXTRALIBPATH) $(EXTRALIBS)
else
  LTICXXFLAGS:=$(shell $(LTICMD) --cxxflags debug)
  CXXFLAGSDEB:=-c -g $(CPUARCH) -Wall -ansi $(LTICXXFLAGS) $(CXXINCLUDE)
  GCC:=$(CXX) $(CXXFLAGSDEB) $(PROFILE)
  LIBS:=$(shell $(LTICMD) --libs debug) $(EXTRALIBPATH) $(EXTRALIBS)
endif

LNALL = $(CXX) $(PROFILE) 

# implicit rules 
$(OBJDIR)%.o : %.cpp
	@echo "Compiling $<..."
	@$(GCC) $< -o $@

all: $(PACKAGE)

# example
$(PACKAGE): $(OBJFILES) ltiBilinearMapping.h ltiBilinearMapping_template.h
	@echo "Linking $(PACKAGE)..."
	@$(LNALL) -o $(PACKAGE) $(OBJFILES) $(LIBS)

clean:
	@echo "Removing *.o files..."
	@rm -f *.o
	@echo "Ready."

clean-all:
	@echo "Removing files..."
	@echo "  removing obj, core and binary files..."  
	@rm -f ./core* $(PACKAGE) $(OBJDIR)*.o 
	@echo "  removing emacs backup files..."  
	@find $$PWD \( -name '*\~' -or -name '\#*' \) -exec rm -f {} \;
	@echo "  removing other automatic created backup files..."  
	@find $$PWD \( -name '\.\#*' -or -name '\#*' \) -exec rm -f {} \;
	@rm -fv nohup.out
	@echo "Ready."

debug:
	@echo "Package: $(PACKAGE)"
