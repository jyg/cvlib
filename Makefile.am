AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src help examples

ALL_CFLAGS = -I"$(PD_INCLUDE)"

cvlibdir=$(pkglibdir)

dist_cvlib_DATA = \
	cvlib-meta.pd \
	COPYING.txt \
	LICENSE.txt \
	README.txt
# cvlib-help.pd

EXTRA_DIST = \
 m4 \
 configure.ac \
 cvlib-meta.pd.in \
 Makefile.am \
 autogen.sh

DISTDIR=$(PACKAGE_NAME)-$(PACKAGE_VERSION)
ORIGDIR=pd-$(PACKAGE_NAME)_$(PACKAGE_VERSION)
OS=$(shell uname -s)
DISTBINDIR=$(DISTDIR)-$(OS)

libdir: all
	$(INSTALL) -p -m 755 -d $(DISTBINDIR)
	$(INSTALL_DATA) $(PACKAGE_NAME)-meta.pd  $(DISTBINDIR)
	$(INSTALL_DATA) $(PACKAGE_NAME).$(EXTENSION)  $(DISTBINDIR)
	$(INSTALL_DATA) $(dist_cvlib_DATA) $(DISTBINDIR)
	test -z "$(strip $(EXTRA_DIST))" || \
		rsync -a $(EXTRA_DIST)    $(DISTBINDIR)
	test -z "$(strip $(SUBDIRS))" || \
	  rsync -a --exclude 'Makefile' --exclude 'Makefile.in' $(SUBDIRS) $(DISTBINDIR)
#	tar --exclude-vcs -czpf $(DISTBINDIR).tar.gz $(DISTBINDIR)
