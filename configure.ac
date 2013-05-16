dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(procure, 1.0, mvc@fct.unl.pt)
AC_PROG_CXX

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual linking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building shared libraries 
AC_PROG_LIBTOOL

dnl Check for 3rd party libraries
OLD_LIBS=$LIBS
OLD_CXXFLAGS=$CXXFLAGS
AC_LANG_PUSH([C++])

dnl Check for librt
LIBS="-lrt"
AC_CHECK_LIB(rt,clock_gettime,have_librt=yes,have_librt=no)
LIBS=$OLD_LIBS

dnl Check for mathlib
LIBS="-lm"
AC_CHECK_LIB(ultim,utan,have_mathlib=yes,have_mathlib=no)
LIBS=$OLD_LIBS

dnl Check for crlibm
LIBS="-lm"
AC_CHECK_LIB(crlibm,cos_rn,have_crlibm=yes,have_crlibm=no)
LIBS=$OLD_LIBS

if test ${have_mathlib} = "yes"; then
   GAOL_MATHLIB="-lultim -lm"
else
   GAOL_MATHLIB="-lcrlib"
fi

dnl Check for sse2 instructions with gaol
AC_MSG_CHECKING([if Gaol library uses sse2 instructions])
AC_RUN_IFELSE([
                  AC_LANG_PROGRAM( [ #include <gaol/gaol_config.h>
                                     #include <cassert>
                                     #if USING_SSE2_INSTRUCTIONS 
                                     int have_sse2 = 1;
                                     #else
                                     int have_sse2 = 0;
                                     #endif ],
                                   [ assert( have_sse2 == 1 ); ] )
              ],
              [gaol_uses_sse2=yes],
              [gaol_uses_sse2=no] )
AC_MSG_RESULT(${gaol_uses_sse2})

if test ${gaol_uses_sse2} = "yes"; then
   GAOL_CXXFLAGS="-msse2 -msse3"
fi

dnl Check for gaol
GAOL_LIBS="-lgaol -lgdtoa "$GAOL_MATHLIB
LIBS=$LIBS" "$GAOL_LIBS
CXXFLAGS=$CXXFLAGS" "$GAOL_CXXFLAGS
AC_MSG_CHECKING([for Gaol library])
AC_RUN_IFELSE( [
                  AC_LANG_PROGRAM( [ #include <gaol/gaol> ],
                                   [ gaol::interval itv;  ] )
               ],
               [have_gaol=yes],
               [have_gaol=no])
AC_MSG_RESULT(${have_gaol})
LIBS=$OLD_LIBS
CXXFLAGS=$OLD_CXXFLAGS

if test ${have_gaol} = "yes"; then
   AC_SUBST([GAOL_LIBS],$GAOL_LIBS)
   AC_SUBST([GAOL_CXXFLAGS],$GAOL_CXXFLAGS)
   AC_DEFINE(HAVE_GAOL,1,
          [Define to 1 if Gaol is detected.])
fi

dnl Check for realpaver
REALPAVER_LIBS=$GAOL_LIBS" -lrealpaver"
if test ${have_librt} = "yes"; then
   REALPAVER_LIBS=$REALPAVER_LIBS" -lrt"
fi
REALPAVER_CXXFLAGS=$GAOL_CXXFLAGS
LIBS=$LIBS" "$REALPAVER_LIBS
CXXFLAGS=$CXXFLAGS" "$REALPAVER_CXXFLAGS
AC_MSG_CHECKING([for Realpaver library])
AC_RUN_IFELSE( [
                  AC_LANG_PROGRAM( [ #include <realpaver/Realpaver.h> ],
                                   [ Realpaver::Expression dummy;  ] )
               ],
               [have_realpaver=yes],
               [have_realpaver=no])
AC_MSG_RESULT(${have_realpaver})
LIBS=$OLD_LIBS
CXXFLAGS=$OLD_CXXFLAGS

if test ${have_realpaver} = "yes"; then
   AC_SUBST([REALPAVER_LIBS],$LIBS)
   AC_SUBST([REALPAVER_CXXFLAGS],$REALPAVER_CXXFLAGS)
   AC_DEFINE(HAVE_REALPAVER,1,
          [Define to 1 if Realpaver is detected.])
fi

dnl Check for boost interval
AC_MSG_CHECKING([for boost interval library])
AC_RUN_IFELSE([
                  AC_LANG_PROGRAM( [ #include <boost/numeric/interval.hpp>],
                                   [ boost::numeric::interval<double> dummy; ] )
              ],
              [have_boostinterval=yes],
              [have_boostinterval=no] )
AC_MSG_RESULT(${have_boostinterval})

if test ${have_boostinterval} = "yes"; then
   AC_DEFINE(HAVE_BOOSTINTERVAL,1,
          [Define to 1 if boost interval library is detected.])
fi

dnl Check for Profil
PROFIL_LIBS="-lProfil"
LIBS=$LIBS" "$PROFIL_LIBS
AC_MSG_CHECKING([for Profil library])
AC_RUN_IFELSE( [
                  AC_LANG_PROGRAM( [ #include <Interval.h> ],
                                   [ INTERVAL a;  ] )
               ],
               [have_profil=yes],
               [have_profil=no])
AC_MSG_RESULT(${have_profil})
LIBS=$OLD_LIBS

if test ${have_profil} = "yes"; then
   AC_SUBST([PROFIL_LIBS],$LIBS)
   AC_DEFINE(HAVE_PROFIL,1,
          [Define to 1 if Profil is detected.])
fi

AC_LANG_POP([C++])

AC_CONFIG_HEADERS(config.h)

AC_CONFIG_FILES(Makefile
                examples/Makefile
                procure/Makefile)
AC_OUTPUT
