
if HAVE_PROFIL

#######################################
# The list of executables we are building seperated by spaces
# the 'bin_' indicates that these build products will be installed
# in the $(bindir) directory. For example /usr/bin
#bin_PROGRAMS=exampleProgram

# Because a.out is only a sample program we don't want it to be installed.
# The 'noinst_' prefix indicates that the following targets are not to be
# installed.
noinst_PROGRAMS=quadv2

#######################################
# Build information for each executable. The variable name is derived
# by use the name of the executable with each non alpha-numeric character is
# replaced by '_'. So a.out becomes a_out and the appropriate suffex added.
# '_SOURCES' for example.

# Sources for the a.out 
quadv2_SOURCES= inverse.cpp io.cpp main.cpp Menu.cpp Tensor.cpp

# Linker options for a.out
quadv2_LDFLAGS = -L$(top_srcdir)/procure \
										-L$(top_srcdir)/3rdparty/include/profil \
										 $(INTERVAL_LIBS)

# Compiler options for a.out
quadv2_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/3rdparty/include/profil

endif
