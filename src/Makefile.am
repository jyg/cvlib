#~bin_PROGRAMS = cvlib
AUTOMAKE_OPTIONS = foreign
pkglib_LTLIBRARIES = cvlib.la

cvlib_la_CXXFLAGS = @GEM_CPPFLAGS@ @PD_CPPFLAGS@  @FACETRACKER_CPPFLAGS@ @OPENCV_CFLAGS@
cvlib_la_LDFLAGS = -module -avoid-version -shared -shrext .@EXTENSION@  @GEM_LDFLAGS@ @PD_LDFLAGS@ @FACETRACKER_LDFLAGS@ @OPENCV_LDFLAGS@
cvlib_la_LIBADD = @OPENCV_LIBS@
cvlib_la_includedir=$(includedir)/src
cvlib_la_include_HEADERS = 	cvlib_show_image.h \
					cvlib_circle.h \
					cvlib_find_contours.h \
					cvlib_draw_contours.h \
					cvlib_approx_poly.h \
					cvlib_contours_convex_hull.h \
					cvlib_contours_features.h \
					cvlib_route_contours.h \
					cvlib_sort_contours.h \
					cvlib_interpol_contours.h \
					cvlib_contours_get_points.h \
					cvlib_contours_convexity_defects.h

OVERVIEW=../cvlib-help.pd

cvlib_la_SOURCES = cvlib.cc \
			cvlib_show_image.cpp \
			cvlib_circle.cpp \
			cvlib_find_contours.cpp \
			cvlib_draw_contours.cpp \
			cvlib_approx_poly.cpp \
			cvlib_contours_convex_hull.cpp \
			cvlib_contours_features.cpp \
			cvlib_route_contours.cpp \
			cvlib_sort_contours.cpp \
			cvlib_interpol_contours.cpp \
			cvlib_contours_get_points.cpp \
			cvlib_contours_convexity_defects.cpp

#~ Override cvlib.la target to add ".WAIT" but it doesn't seems to work...
#~cvlib.la: .WAIT $(cvlib_la_OBJECTS) $(cvlib_la_DEPENDENCIES) $(EXTRA_cvlib_la_DEPENDENCIES) 
#~$(AM_V_CXXLD)$(cvlib_la_LINK) -rpath $(pkglibdir) $(cvlib_la_OBJECTS) $(cvlib_la_LIBADD) $(LIBS)
#~
#~.WAIT: 

## uha, this is ugly
$(abs_builddir)/.libs/cvlib.@GEXTENSION@: cvlib.la

$(top_builddir)/cvlib.@EXTENSION@: $(abs_builddir)/.libs/cvlib.@EXTENSION@ 
		rm -f $@ && cd $(top_builddir) && test -e $< && $(LN_S) $< cvlib.@EXTENSION@ || true

.PHONY: clean-conveniencelink help

clean-conveniencelink:
	test -L $(top_builddir)/cvlib.@EXTENSION@ && rm -f $(top_builddir)/cvlib.@EXTENSION@ || true


all-local:: $(top_builddir)/cvlib.@EXTENSION@ help

clean-local:: clean-conveniencelink

.o: .cc
	$(CXX) $(ALL_CFLAGS) -o "$*.o" -c "$*.cc"

help:
	echo "#N canvas 147 197 1566 537 10;" > $(OVERVIEW)
	echo "#X text 126 15 overview of all available cvlib objects;" >> $(OVERVIEW)
	echo "#X obj 30 20 cvlib;" >> $(OVERVIEW)
	ID=0 ; \
	for extern in $(cvlib_la_SOURCES:.cc=""); do \
		echo "#X obj `expr $$ID % 5 \* 300 + 50` `expr $$ID / 5 \* 30 + 40` $$extern;" >> $(OVERVIEW) && ID=`expr $$ID + 1` ; \
	done ;
