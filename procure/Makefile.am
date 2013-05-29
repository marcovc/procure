#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libprocure.la

# These files will end up in the install include directory
# For example, /usr/include
include_HEADERS = procure.hpp kernel/common.hpp kernel/interval.hpp \
									kernel/interval/interval_gaol.hpp \
									kernel/interval/interval_boost.hpp \
									kernel/var.hpp kernel/expr.hpp kernel/func.hpp \
									util/util.hpp quadrature/taylor.hpp

#######################################
# Build information for each library

# Sources for libProcure
libprocure_la_SOURCES = procure.cpp kernel/interval.cpp kernel/var.cpp \
												kernel/expr.cpp kernel/func.cpp util/util.cpp \
												quadrature/taylor.cpp

# Linker options libTestProgram
libprocure_la_LDFLAGS = $(CXX11_LDFLAGS)

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libprocure_la_CPPFLAGS = -I$(top_srcdir)  $(CXX11_CPPFLAGS)

