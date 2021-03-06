dnl Process this file with autoconf to produce a configure script.
AC_INIT([cvlib], 0.4, jyg@gumo.fr, [cvlib])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_FILES([help/Makefile])
AC_CONFIG_FILES([cvlib-meta.pd])
AC_CONFIG_SRCDIR(src)

AC_SUBST(PD_CPPFLAGS)
AC_SUBST(PD_LDFLAGS)
AC_SUBST(GEM_CPPFLAGS)
AC_SUBST(GEM_LDFLAGS)
AC_SUBST(OPENCV_CFLAGS)
AC_SUBST(OPENCV_LDFLAGS)
AC_SUBST(OPENCV_LIBS)
AC_SUBST(FACETRACKER_CPPFLAGS)
AC_SUBST(FACETRACKER_LDFLAGS)
AC_SUBST(EXTENSION)

AM_INIT_AUTOMAKE([1.10 foreign])

AC_ARG_WITH([pd],       [  --with-pd=</path/to/pd> where to look for pd-headers and and -libs])
AC_ARG_WITH([gem],       [  --with-gem=</path/to/gem> where to look for gem-headers and and -libs])
AC_ARG_WITH([extension],[  --with-extension=<ext>  enforce a certain extension for the dynamic library (e.g. dll)])
AC_ARG_WITH([facetracker],[  --with-facetracker=</path/to/gem>  where to look for facetracker-headers and -libs])

ARCH=$(uname -m)
KERN=$(uname -s)

AC_PROG_CXX
AC_PROG_LIBTOOL

if test "x${libdir}" = "x\${exec_prefix}/lib"; then
 libdir='${exec_prefix}/lib/pd/extra'
fi

if test "x$with_pd" != "x"; then
 if test -d "${with_pd}"; then
   PDPATH=${with_pd}
 fi
 if test -d "${PDPATH}/src"; then
  AC_MSG_RESULT([adding ${PDPATH}/src to INCLUDES])
  PD_CPPFLAGS="-I${PDPATH}/src ${CPPFLAGS}"

  AC_MSG_RESULT([adding ${PDPATH}/src to LDFLAGS])
  PD_LDFLAGS="-L${PDPATH}/src ${LDFLAGS}"
 else
  if test -d "${PDPATH}"; then
   AC_MSG_RESULT([adding ${PDPATH} to INCLUDES])
   PD_CPPFLAGS="-I${PDPATH} ${CPPFLAGS}"
  fi
 fi
 if test -d "${PDPATH}/bin"; then
  AC_MSG_RESULT([adding ${PDPATH}/bin to LDFLAGS])
  PD_LDFLAGS="-L${PDPATH}/bin ${LDFLAGS}"
 else
  if test -d "${PDPATH}"; then
   AC_MSG_RESULT([adding ${PDPATH} to LDFLAGS])
   PD_LDFLAGS="-L${PDPATH} ${LDFLAGS}"
  fi
 fi
else 
  AC_MSG_RESULT([no pd path given, trying with pkg-config data])
  PD_CPPFLAGS=`pkg-config --cflags pd`
  PD_LDFLAGS=`pkg-config --libs pd`
fi

if test "x$with_gem" != "x"; then
 if test -d "${with_gem}"; then
   GEMPATH=${with_gem}
 fi
 if test -d "${GEMPATH}/src"; then
  AC_MSG_RESULT([adding ${GEMPATH}/src to INCLUDES])
  GEM_CPPFLAGS="-I${GEMPATH}/src ${CPPFLAGS}"

  AC_MSG_RESULT([adding ${GEMPATH}/src to LDFLAGS])
  GEM_LDFLAGS="-L${GEMPATH}/src ${LDFLAGS}"
 else
  if test -d "${GEMPATH}"; then
   AC_MSG_RESULT([adding ${GEMPATH} to INCLUDES])
   GEM_CPPFLAGS="-I${GEMPATH} ${CPPFLAGS}"
  fi
 fi
 if test -d "${GEMPATH}/bin"; then
  AC_MSG_RESULT([adding ${GEMPATH}/bin to LDFLAGS])
  GEM_LDFLAGS="-L${GEMPATH}/bin ${LDFLAGS}"
 else
  if test -d "${GEMPATH}"; then
   AC_MSG_RESULT([adding ${GEMPATH} to LDFLAGS])
   GEM_LDFLAGS="-L${GEMPATH} ${LDFLAGS}"
  fi
 fi
else 
  AC_MSG_RESULT([no Gem path given, trying with pkg-config data])
  GEM_CPPFLAGS=`pkg-config --cflags Gem`
  GEM_LDFLAGS=`pkg-config --libs Gem`
fi

if test "x$with_facetracker" != "x"; then
 if test -d "${with_facetracker}"; then
   FACETRACKERPATH=${with_facetracker}
 fi
else 
  if test -d "FaceTracker/src"; then
    #~ ouch ! I don't know how to do that in a cleaner way...
    FACETRACKERPATH=$(pwd)/FaceTracker
  fi
fi

if test "x$FACETRACKERPATH" != "x"; then
 AC_MSG_RESULT([FaceTracker library detected])
 if test -d "${FACETRACKERPATH}/include"; then
  AC_MSG_RESULT([adding ${FACETRACKERPATH}/include to INCLUDES])
  FACETRACKER_CPPFLAGS="-I${FACETRACKERPATH}/include ${CPPFLAGS} -DHAVE_FACETRACKER"
 fi
 if test -d "${FACETRACKERPATH}/src/lib"; then
  for obj in "${FACETRACKERPATH}/src/lib/*.o"; do
    AC_MSG_RESULT([adding $obj to LDFLAGS])
    FACETRACKER_LDFLAGS+="$obj"
  done
 fi
fi

AC_MSG_CHECKING([extension])
if test "x$with_extension" != "x"
then
 EXTENSION=$with_extension
else
 case "$KERN" in
 Darwin)
   EXTENSION=pd_darwin
   ;;
 Linux)
   if test "$ARCH" == x86_64
   then
     EXTENSION=pd_linux
   else
     EXTENSION=l_i386
   fi
   ;;
 *CYGWIN*)
   EXTENSION=dll
   ;;
 *MINGW*)
   EXTENSION=dll
   ;;
 *)
   EXTENSION=pd_linux
   ;;
 esac
fi
AC_MSG_RESULT([$EXTENSION])


PKG_CHECK_EXISTS([opencv >= 2.4],[AC_DEFINE([HAVE_BGSUB],[1], [Define if opencv version >= 2.4])]);
PKG_CHECK_EXISTS([opencv >= 2.4.5],[AC_DEFINE([HAVE_CLAHE],[1], [Define if opencv version >= 2.4.5])]);

AC_CHECK_LIB([opencv_core], [error],,[PKG_CHECK_MODULES([OPENCV],[opencv > 2.0],,[AC_MSG_RESULT(["warning ! can't find OpenCV !"])])])
AC_CHECK_LIB([opencv_imgproc], [cvCvtColor],,)
AC_CHECK_LIB([opencv_legacy], [_Z38cvCreateModuleBlobTrackAnalysisHistPVSv],,)
AC_CHECK_LIB([opencv_objdetect], [cvHaarDetectObjects],,)
AC_CHECK_LIB([opencv_ml], [_ZN10CvKNearestC1EPK5CvMatS2_S2_bi],,)
AC_CHECK_LIB([opencv_ocl], [_init],,)
AC_CHECK_LIB([opencv_calib3d], [cvFindChessboardCorners],,)
AC_CHECK_LIB([opencv_video], [_ZTSN2cv20BackgroundSubtractorE],,)
AC_CHECK_LIB([opencv_nonfree],[_ZN2cv18initModule_nonfreeEv],,)


AC_OUTPUT(Makefile src/Makefile)
