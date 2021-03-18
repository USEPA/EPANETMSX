# Example Linux Makefile for EPANET-MSX

# This will build EPANET-MSX as a shared object
# library (libepanetmsx.so) under Linux,
# and a standalone executable (epanetmsx) that is linked
# to this shared object library.

# Assumes that an Epanet2 shared object library (libepanet2.so) has been built
# and is located in ~/lib (change below as needed).
# Assumes that an EPANET2 header (epanet2.h) is located in
# ~/include (change below as needed).

# The following targets are defined (for execution in
# the build directory):
# make
#     -Builds libepanetmsx.so, epanetmsx
# make install
#     -Creates shell wrapper runepanetmsx.sh that
#      executes epanetmsx.
#      The runepanetmsx.sh wrapper simply exports
#      environment variables so that the library is found at runtime,
#      allowing you to specify your own locations for installing
#      the libraries.
#     -Installs epanetmsx, and runepanetmsx.sh
#      in <prefix>/bin, where <prefix> defaults to ~ (and can be set
#      below to something different - see "Install directories")
#     -Installs libepanetmsx.so (shared library) in <prefix>/lib
#     -Installs epanetmsx.h in <prefix>/include.  This is the required
#      header file for the EPANETMSX programmer's toolkit, and thus
#      <prefix>/include should be on your CPP include search path
#      for subsequent use of the toolkit and linking with the shared
#      library libepanetmsx.so
# make clean
#     -Removes object and library files, returning the build directory
#      to its pristine state.

SHELL = /bin/sh

# C H A N G E  H E R E  A S  N E E D E D
# Install path 'prefix'
prefix = ~
# Location of libepanet2.so (Epanet2 shared object library, built from Epanet2 source)
epanetlibdir = ~/lib
# Location of epanet2.h (EPANET2 header file)
epanetincludedir = ~/include

# Install directories
exec_prefix = $(prefix)
libdir = $(exec_prefix)/lib
bindir = $(exec_prefix)/bin
includedir = $(prefix)/include
datarootdir = $(prefix)/share
docdir = $(datarootdir)/doc/epanetmsx

# Target filenames
exename := epanetmsx
soname := lib$(exename).so

# Location of EPANET-MSX files
epanetmsxsrcdir = ../../Src
epanetmsxincludedir = ../../Include

# Search path for sources
VPATH = $(epanetmsxsrcdir):$(epanetmsxincludedir)

# Compiler and flags
CC = gcc
CFLAGS = -g -O -fPIC
CPPFLAGS = -I $(epanetmsxsrcdir) -I $(epanetmsxincludedir) -I $(epanetincludedir)
LDFLAGS = -L . -L $(epanetlibdir) -W1,-rpath,$(libdir)

# Installer
INSTALL = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644

# Files for the shared object library
epanetmsx_objs=hash.o mempool.o mathexpr.o msxchem.o msxfile.o msxinp.o msxout.o msxproj.o \
	msxqual.o msxrpt.o msxtank.o msxtoolkit.o msxutils.o newton.o rk5.o ros2.o msxerr.o
# Epanetmsx main program
epanetmsx_main_obj=msxmain.o
# Epanetmsx header files
epanetmsx_heads=hash.h mempool.h mathexpr.h msxdict.h msxtypes.h msxutils.h \
	               newton.h rk5.h ros2.h epanetmsx.h
# Epanetmsx main program header files
epanetmsx_main_heads = epanetmsx.h

.PHONY: all
all: $(soname) $(exename)

$(soname): $(epanetmsx_objs)
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

$(exename): $(soname) $(epanetmsx_main_obj) $(epanetmsx_main_heads)
	$(CC) $(CFLAGS) -o $@ $(epanetmsx_main_obj) $(LDFLAGS) -lepanet2 -l$(exename)

$(epanetmsx_objs): $(epanetmsx_heads)

.PHONY: install
install:
	$(INSTALL_PROGRAM) -D $(exename) $(bindir)/$(exename)
	$(INSTALL_PROGRAM) -D $(soname) $(libdir)/$(soname)
	$(INSTALL_DATA) -D $(epanetmsxincludedir)/epanetmsx.h $(includedir)/epanetmsx.h
	$(INSTALL_PROGRAM) -D runepanetmsx.sh $(bindir)/runepanetmsx.sh

.PHONY: clean
clean:
	-/bin/rm *.o $(soname) $(exename)
