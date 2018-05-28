/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/



#ifndef INCLUDE_CVLIB_CONTOURS_CONVEXITY_DEFECTS_H_
#define INCLUDE_CVLIB_CONTOURS_CONVEXITY_DEFECTS_H_

#ifndef _EiC
	#include "opencv2/legacy/legacy.hpp"
	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours
#include <stdio.h>

typedef struct convexity_point {
    int x;
    int y;
    float depth;
    };
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_contours_convexity_defects

    Contours convexity defects detection

KEYWORDS


DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN cvlib_contours_convexity_defects: public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_contours_convexity_defects,CPPExtern)
	
	public:
		//////////
		// Constructor
		cvlib_contours_convexity_defects(t_float);

	protected:
  
		//////////
		// Destructor
		~cvlib_contours_convexity_defects();

		float margin;
		int max_convex_defects;
		int method;  // method used for computing convexity defects (default=0);
		CvSeq* convex_seq;
		CvConvexityDefect* defectArray;

		void processCvContours (int i);
		void set_Margin(float f);
		void set_Method(int i);
		void set_MaxConvexDefects(int i);
		CvMemStorage* stor01;
		CvMemStorage* stor02;
		CvMemStorage* stor03;
		CvMemStorage* stor04;
		CvMemStorage* stor05;
		
		CvSeq* defects;
		CvSeq* contours;
		
		CvSeq* approx_result;
		CvSeq* seqhull;
		CvPoint2D32f depth_point32F;
		
		
		
		std::vector<convexity_point> sorted_defects;
		int* hull;
		int hullsize;
		int* HullPointArray;
		CvPoint* PointArray;
		CvPoint* DefectPointArray;
		CvPoint* ConvexPointArray;

    		float depth_distance;

	private:

		t_outlet *m_leftOutlet;
		t_outlet *m_nbOfPointsOutlet;
		t_atom rlist[4];

		//////////
		// Static member functions

		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void set_MethodCallback(void *data, t_floatarg _sid);
		static void set_MarginCallback(void *data, t_floatarg _sid);
		static void set_MaxConvexDefectsCallback(void *data, t_floatarg _sid);

};

// for header file
#endif

