#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=example

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

# Sources for the a.out 
example_SOURCES= example.cpp

# Linker options for a.out
example_LDFLAGS = -L$(top_srcdir)/procure -lprocure $(INTERVAL_LIBS) $(CXX11_LDFLAGS)

# Compiler options for a.out
example_CPPFLAGS = -I$(top_srcdir) $(CXX11_CPPFLAGS)
