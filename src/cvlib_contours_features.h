/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef INCLUDE_CVLIB_CONTOURS_FEATURES_H_
#define INCLUDE_CVLIB_CONTOURS_FEATURES_H_

//   nb of contour features
#define MAX_NB_OF_CONTOUR_FEATURES 10

#ifndef _EiC
//	#include "opencv2/legacy/legacy.hpp"
	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_contours_features

    getting properties of cv_contours

DESCRIPTION


-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_contours_features : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_contours_features,CPPExtern);

	public:
		
		//////////
		// Constructor
		cvlib_contours_features(int argc,t_atom *argv);

	protected:

		//////////
		// Destructor
		~cvlib_contours_features();


   		void processCvContours (int i);
		void sendCvContoursSID();


		// feature extraction functions
		void get_Area(CvSeq* contour,int feature_outlet);
		void get_BoundingRect(CvSeq* contour,int feature_outlet);
		void get_IsHole(CvSeq* contour,int feature_outlet);
		void get_HuMoments(CvSeq* contour,int feature_outlet);
		void get_LogHuMoments(CvSeq* contour,int feature_outlet);
		void get_IsConvex(CvSeq* contour,int feature_outlet);
		void get_MinAreaRect(CvSeq* contour,int feature_outlet);
		void get_Nothing(CvSeq* contour,int feature_outlet);
		void get_Perimeter(CvSeq* contour,int feature_outlet);
		void get_SegmentCount(CvSeq* contour,int feature_outlet);

		typedef  void  (cvlib_contours_features::*pt2Member)(CvSeq* contour,int feature_outlet);
		// array of pointers to feature extraction functions
		pt2Member  funcArr[MAX_NB_OF_CONTOUR_FEATURES];

	private:

		CvSeq* contours;
		int nb_of_features;
		int cvSeqSID;
		t_atom my_output;


		// variables used by feature extraction functions
		CvRect rect;
		CvBox2D min_area_rect;
		t_atom result[8];
		float moment_value[7];
		CvMoments moments;
		CvHuMoments HuMoments;

		t_outlet *m_cvContoursOutlet;
		t_outlet *m_featureOutlet[MAX_NB_OF_CONTOUR_FEATURES];

		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);

};



// end of #ifndef statement
#endif

