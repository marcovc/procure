dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(procure, 1.0, mvc@fct.unl.pt)
AC_PROG_CXX

AC_CANONICAL_SYSTEM

dnl AC_CONFIG_MACRO_DIR([m4])

dnl Initialize automake
AM_INIT_AUTOMAKE

dnl this allows us specify individual linking flags for each target
AM_PROG_CC_C_O 

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building shared libraries 
AC_PROG_LIBTOOL

AC_ARG_WITH([interval_lib],
    [AS_HELP_STRING([--with-interval-lib=(any,gaol,boost,profil)],
    [Specify the interval arithmetic library to use @<:@default=any@:>@])],
    use_interval_lib="${with_interval_lib}",use_interval_lib="any")

dnl Check for 3rd party libraries
OLD_LIBS=$LIBS
OLD_CXXFLAGS=$CXXFLAGS
AC_LANG_PUSH([C++])

dnl Check for librt
LIBS="-lrt"
AC_CHECK_LIB(rt,clock_gettime,have_librt=yes,have_librt=no)
LIBS=$OLD_LIBS

dnl Check for mpfr
LIBS=""
AC_CHECK_LIB(mpfr,mpfr_init,have_mpfr=yes,have_mpfr=no)
LIBS=$OLD_LIBS

if test ${have_mpfr} = "yes"; then
AC_DEFINE(HAVE_MPFR,1,
          [Define to 1 if mpfr library is detected.])
fi

dnl Check for mathlib
LIBS="-lm"
AC_CHECK_LIB(ultim,utan,have_mathlib=yes,have_mathlib=no)
LIBS=$OLD_LIBS

dnl Check for crlibm
LIBS="-lm"
AC_CHECK_LIB(crlibm,cos_rn,have_crlibm=yes,have_crlibm=no)
LIBS=$OLD_LIBS

if test ${have_crlibm} = "yes"; then
AC_DEFINE(HAVE_CRLIBM,1,
          [Define to 1 if crlibm library is detected.])
fi

if test ${have_mathlib} = "yes"; then
   GAOL_MATHLIB="-lultim -lm"
else
   GAOL_MATHLIB="-lcrlibm"
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
GAOL_LIBS=$GAOL_MATHLIB" -lgaol -lgdtoa"
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
else
	GAOL_LIBS=""
	if test "${use_interval_lib}" = "gaol"; then
		AC_MSG_ERROR([Gaol library was not detected. Please check the --with-interval-lib option.])
	fi          
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
	if test ${have_crlibm} = "yes"; then
		BOOSTINTERVAL_LIBS="-lcrlibm"
	elif test ${have_mpfr} = "yes"; then
		BOOSTINTERVAL_LIBS="-lmpfr"
	fi       
else
	BOOSTINTERVAL_LIBS=""
	if test "${use_interval_lib}" = "boost"; then
		AC_MSG_ERROR([Boost library was not detected. Please check the --with-interval-lib option.])
	fi          
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
else
	PROFIL_LIBS=""
	if test "${use_interval_lib}" = "profil"; then
		AC_MSG_ERROR([Profil library was not detected. Please check the --with-interval-lib option.])
	fi          
fi

AC_LANG_POP([C++])

if test "${use_interval_lib}" = "any"; then
	if test "${have_gaol}" = "yes"; then
		use_interval_lib="gaol"
	elif test "${have_profil}" = "yes"; then
		use_interval_lib="profil"
	elif test "${have_boostinterval}" = "yes"; then
		use_interval_lib="boost"
	else
		AC_MSG_ERROR([No suitable interval arithmetic library was detected.])
	fi
fi
	
if test "${use_interval_lib}" = "gaol"; then
  AC_DEFINE(PROCURE_INTERVAL_USE_GAOL,1,
          [Define to 1 if gaol is selected to implement interval arithmetic.])
  AC_SUBST([INTERVAL_LIBS],$GAOL_LIBS)
elif test "${use_interval_lib}" = "boost"; then
  AC_DEFINE(PROCURE_INTERVAL_USE_BOOST,1,
          [Define to 1 if boost is selected to implement interval arithmetic.])
  AC_SUBST([INTERVAL_LIBS],$BOOSTINTERVAL_LIBS)
elif test "${use_interval_lib}" = "profil"; then
  AC_DEFINE(PROCURE_INTERVAL_USE_PROFIL,1,
          [Define to 1 if profil is selected to implement interval arithmetic.])
  AC_SUBST([INTERVAL_LIBS],$PROFIL_LIBS)
fi

dnl Link all of them (some tests use multiple for comparison)
AC_SUBST([INTERVAL_ALL_LIBS],$GAOL_LIBS" "$BOOSTINTERVAL_LIBS" "$PROFIL_LIBS)

dnl Support for C++11
if test ${CXX} = "clang++"; then
	CXX11_CPPFLAGS="-std=c++11 -stdlib=libc++"
	CXX11_LDFLAGS="-stdlib=libc++"
elif test ${CXX} = "g++"; then
	CXX11_CPPFLAGS="-std=gnu++0x"
	CXX11_LDFLAGS=""
fi
AC_SUBST(CXX11_CPPFLAGS)
AC_SUBST(CXX11_LDFLAGS)

AC_CONFIG_HEADERS(procure/config.h)

AC_CONFIG_FILES(Makefile
                procure/Makefile
                tests/Makefile
                examples/Makefile)
AC_OUTPUT
