/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CVLIB_CONTOURS_CONVEX_HULL_H_
#define INCLUDE_CVLIB_CONTOURS_CONVEX_HULL_H_


#ifndef _EiC
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_contours_convex_hull

    get convex hull of cv_contours

DESCRIPTION


-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_contours_convex_hull : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_contours_convex_hull, CPPExtern);
	
	public:
		
		//////////
		// Constructor
		cvlib_contours_convex_hull();

	protected:

		//////////
		// Destructor
		~cvlib_contours_convex_hull();

		int iterations;
		
		void processCvContours (int i);
		void sendCvContoursSID();


	private:
	
		CvSeq* contours;
		CvSeq* result;
		CvSeq* convex_contours;
		CvSeq* convex_contours_head;
		CvSeq* convex_seq;
		int cvSeqSID;

		CvMemStorage* storage;
		t_atom my_output;
		t_outlet *m_cvContoursOutlet;

		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);

};



// end of #ifndef statement
#endif

