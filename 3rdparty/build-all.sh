#!/bin/sh
###########################################################################
#	
#	This file is part of Procure.
#
#	Copyright:                                      
#	2013 - Marco Correia <marco.v.correia@gmail.com>
#
# 	Build and installs (in the current dir) the following libraries:
#
# gmp (http://gmplib.org/)
#	mpfr	(http://www.mpfr.org/)
#	crlibm 	(http://lipforge.ens-lyon.fr/www/crlibm/)
# 	mathlib (http://sourceforge.net/projects/gaol/files/mathlib/)
#	gaol	(http://sourceforge.net/projects/gaol/)
#	realpaver	(http://pagesperso.lina.univ-nantes.fr/~granvilliers-l/realpaver/)
#	Profil/BIAS	(http://www.ti3.tuhh.de/keil/profil/index_e.html)
#	boost::interval	(http://www.boost.org/doc/libs/)
#
#	NOTES: 
# - This should be run as "make --ignore-errors" so that one failed build
# does not prevent others to build.
#	- The build process for Profil/Bias asks for user input.
#	- I have patched most packages to make them build and these patches
#	may or may not work with versions other than the provided.
# - Gaol's "make check" fails on MacOS(64 bits)
# - Realpaver's "make check" does not compile
#
#	TODO: 
#	- make doc 
#
###########################################################################

gmp=gmp-5.1.2
mpfr=mpfr-3.1.2
crlibm=crlibm-1.0beta4
mathlib=mathlib-2.0.0
gaol=gaol
realpaver=realpaver-1.1.0
profil=Profil-2.0.8
boost=boost_1_53_0

gmp_package=${gmp}.tar.bz2  
mpfr_package=${mpfr}.tar.gz
crlibm_package=${crlibm}.tar.gz 
mathlib_package=${mathlib}.tar.gz 
gaol_package=${gaol}.tar.gz 
realpaver_package=${realpaver}.tar.gz 
profil_package=${profil}.tgz
boost_package=${boost}.tar.gz

crlibm_patch=${crlibm}.patch
mathlib_patch=${mathlib}.patch
gaol_patch=${gaol}.patch 
realpaver_patch=${realpaver}.patch
profil_patch=${profil}.patch
boost_patch=${boost}.patch

top_dir=$(pwd)
cppflags=-I${top_dir}/include
ldflags=-L${top_dir}/lib
ldlibrarypath=${top_dir}/lib
buildenv="env CPPFLAGS=${cppflags} LDFLAGS=${ldflags} 
		LD_LIBRARY_PATH=${ldlibrarypath} DYLD_LIBRARY_PATH=${ldlibrarypath}"


install_gmp()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xjvf ${top_dir}/packages/${gmp_package} &&
	cd ${gmp} &&
	${buildenv} ./configure --prefix=${top_dir} &&
	make &&
	make install &&
	gmp_install="success" || gmp_install="failed";
	make check && gmp_check="success" || gmp_check="failed";
	make docs && gmp_docs="success" || gmp_docs="failed";
}

install_mpfr()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${mpfr_package} &&
	cd ${mpfr} &&
	${buildenv} ./configure --prefix=${top_dir} &&
	make &&
	make install &&
	mpfr_install="success" || mpfr_install="failed";
	make check && mpfr_check="success" || mpfr_check="failed";
	make docs && mpfr_docs="success" || mpfr_docs="failed";
}

install_crlibm()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${crlibm_package} &&
	cd ${crlibm} &&
	patch -p1 < ${top_dir}/patch/${crlibm_patch} &&
	${buildenv} ./configure --prefix=${top_dir} &&
	make &&
	make install &&
	crlibm_install="success" || crlibm_install="failed";
	make check && crlibm_check="success" || crlibm_check="failed";
	make docs && crlibm_docs="success" || crlibm_docs="failed";
}	

install_mathlib()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${mathlib_package} &&
	cd ${mathlib} &&
	patch -p1 < ${top_dir}/patch/${mathlib_patch} &&
	${buildenv} ./configure --prefix=${top_dir} &&
	make &&
	make install &&
	mathlib_install="success" || mathlib_install="failed";
	make check && mathlib_check="success" || mathlib_check="failed";
	make docs && mathlib_docs="success" || mathlib_docs="failed";
}

install_gaol()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${gaol_package} &&
	cd ${gaol} &&
	patch -p1 < ${top_dir}/patch/${gaol_patch} &&
	(${buildenv} ./configure --prefix=${top_dir} --with-mathlib=apmath ||
	 ${buildenv} ./configure --prefix=${top_dir} --with-mathlib=crlibm) &&
	make &&
	make install &&
	gaol_install="success" || gaol_install="failed";
	make check && gaol_check="success" || gaol_check="failed";
	make docs && gaol_docs="success" || gaol_docs="failed";
}

install_realpaver()
{
	mkdir -p ${top_dir}/src && 
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${realpaver_package} &&
	cd ${realpaver} &&
	patch -p1 < ${top_dir}/patch/${realpaver_patch} &&
	env CPPFLAGS="${cppflags} -I${top_dir}/src/${realpaver} -Wno-long-long" \
		LDFLAGS="${ldflags}" LD_LIBRARY_PATH="${ldlibrarypath}" \
		DYLD_LIBRARY_PATH="${ldlibrarypath}"  ./configure --prefix=${top_dir} &&
	make &&
	make install &&
	realpaver_install="success" || realpaver_install="failed";
	make check && realpaver_check="success" || realpaver_check="failed";
	make docs && realpaver_docs="success" || realpaver_docs="failed";
}

install_profil()
{
	mkdir -p ${top_dir}/src &&
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${profil_package} &&
	cd ${profil} &&
	patch -p1 < ${top_dir}/patch/${profil_patch} &&
	./Configure &&
	make &&
	make install && 
	make check && 
	cp -r lib ${top_dir} && 
	cp -r include ${top_dir}/include/profil &&
	profil_install="success" || profil_install="failed";
	profile_check="success";
	make docs && profil_docs="success" || profil_docs="failed";
}	

install_boost()
{
	mkdir -p ${top_dir}/src &&
	cd ${top_dir}/src &&
	tar -xzvf ${top_dir}/packages/${boost_package} &&
	cd ${boost} &&
	patch -p1 < ${top_dir}/patch/${boost_patch} &&
	cp -r boost ${top_dir}/include &&
	boost_install="success";
}

clean()
{
	rm -fr ${top_dir}/include ${top_dir}/lib ${top_dir}/share \
		${top_dir}/bin ${top_dir}/src;
}

for i in gmp mpfr crlibm mathlib gaol realpaver profil boost
do
	eval ${i}_install="skipped";
	eval ${i}_check="skipped";
	eval ${i}_docs="skipped";
done
	
case $1 in
	gmp)		install_gmp;;
	mpfr) install_mpfr;;
	crlibm) install_crlibm;;
	mathlib) install_mathlib;;
	gaol) install_gaol;;
	realpaver) install_realpaver;;
	profil) install_profil;;
	boost) install_boost;;
	clean) clean;;
	*) clean; install_gmp; install_mpfr; install_mathlib; install_crlibm;
	install_gaol; install_realpaver; install_profil; install_boost;
esac

echo "SUMMARY:"
printf "==================================\n"
printf "Library   | Installed Checked Docs\n"
for i in gmp mpfr crlibm mathlib gaol realpaver profil boost; do
	eval install_status=\${${i}_install}
	eval check_status=\${${i}_check}
	eval docs_status=\${${i}_docs}
	printf "%9s | %9s %7s %s\n" $i $install_status $check_status $docs_status	;
done;
printf "==================================\n"
