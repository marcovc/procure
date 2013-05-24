###########################################################################
#	
#	This file is part of Procure.
#
#	Copyright:                                      
#	2013 - Marco Correia <marco.v.correia@gmail.com>
#
# 	Build and installs (in the current dir) the following libraries:
#
#	crlibm 	(http://lipforge.ens-lyon.fr/www/crlibm/)
# 	mathlib (http://sourceforge.net/projects/gaol/files/mathlib/)
#	gaol	(http://sourceforge.net/projects/gaol/)
#	realpaver	(http://pagesperso.lina.univ-nantes.fr/~granvilliers-l/realpaver/)
#	mpfr	(http://www.mpfr.org/)
#	Profil/BIAS	(http://www.ti3.tuhh.de/keil/profil/index_e.html)
#	boost::interval	(http://www.boost.org/doc/libs/)
#
#	NOTES: 
# - This should be run as "make --ignore-errors" so that one failed build
# does not prevent others to build.
#	- The build process for Profil/Bias asks for user input.
#	- I have patched most packages to make them build and these patches
#	may or may not work with versions other than these ones.
# - Gaol's "make check" fails on MacOS(64 bits)
# - Realpaver's "make check" does not compile
#
#	TODO: 
#	- make doc 
#
###########################################################################

crlibm=crlibm-1.0beta4
mathlib=mathlib-2.0.0
gaol=gaol
realpaver=realpaver-1.1.0
mpfr=mpfr-3.1.2
profil=Profil-2.0.8
boost=boost_1_53_0

crlibm_package=$(crlibm).tar.gz 
mathlib_package=$(mathlib).tar.gz 
gaol_package=$(gaol).tar.gz 
realpaver_package=$(realpaver).tar.gz 
mpfr_package=$(mpfr).tar.gz 
profil_package=$(profil).tgz
boost_package=$(boost).tar.gz

crlibm_patch=$(crlibm).patch 
gaol_patch=$(gaol).patch 
realpaver_patch=$(realpaver).patch
profil_patch=$(profil).patch
boost_patch=$(boost).patch

top_dir=$(shell pwd)

all: crlibm mathlib gaol realpaver mpfr profil boost

clean:
	rm -fr $(top_dir)/include $(top_dir)/lib $(top_dir)/share \
	$(top_dir)/bin $(top_dir)/src

crlibm:
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(crlibm_package) &&\
	cd $(crlibm) &&\
	patch -p1 < $(top_dir)/patch/$(crlibm_patch) &&\
	./configure --prefix=$(top_dir) &&\
	make &&\
	make check &&\
	make install;

mathlib:
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(mathlib_package) &&\
	cd $(mathlib) &&\
	./configure --prefix=$(top_dir) &&\
	make &&\
	make install;

gaol: $(top_dir)/packages/$(gaol_package)
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(gaol_package) &&\
	cd $(gaol) &&\
	patch -p1 < $(top_dir)/patch/$(gaol_patch) &&\
	(CPPFLAGS=-I$(top_dir)/include LDFLAGS=-L$(top_dir)/lib \
		./configure --prefix=$(top_dir) --with-mathlib=apmath ||\
	 CPPFLAGS=-I$(top_dir)/include LDFLAGS=-L$(top_dir)/lib \
		./configure --prefix=$(top_dir) --with-mathlib=crlibm) &&\
	make &&\
	make install;

realpaver: $(top_dir)/packages/$(realpaver_package) gaol
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(realpaver_package) &&\
	cd $(realpaver) &&\
	patch -p1 < $(top_dir)/patch/$(realpaver_patch) &&\
	CPPFLAGS="-I$(top_dir)/include -I$(top_dir)/src/$(realpaver) -Wno-long-long" \
	LDFLAGS=-L$(top_dir)/lib \
	LD_LIBRARY_PATH=$(top_dir)/lib \
	DYLD_LIBRARY_PATH=$(top_dir)/lib \
		./configure --prefix=$(top_dir) &&\
	make &&\
	make install;

mpfr: $(top_dir)/packages/$(mpfr_package)
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(mpfr_package) &&\
	cd $(mpfr) &&\
	./configure --prefix=$(top_dir) &&\
	make &&\
	make check &&\
	make install;
	
profil: $(top_dir)/packages/$(profil_package)
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(profil_package) &&\
	cd $(profil) &&\
	patch -p1 < $(top_dir)/patch/$(profil_patch) &&\
	./Configure &&\
	make &&\
	make install && \
	make check && \
	cp -r lib $(top_dir) && \
	cp -r include $(top_dir)/include/profil;

boost: $(top_dir)/packages/$(boost_package)
	mkdir -p $(top_dir)/src && \
	cd $(top_dir)/src &&\
	tar -xzvf $(top_dir)/packages/$(boost_package) &&\
	cd $(boost) &&\
	patch -p1 < $(top_dir)/patch/$(boost_patch) &&\
	cp -r boost $(top_dir)/include;
