/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef INCLUDE_CVLIB_APPROX_POLY_H_
#define INCLUDE_CVLIB_APPROX_POLY_H_

#ifndef _EiC
//	#include "opencv2/legacy/legacy.hpp"
	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_approx_contours

    approximation of cv_contours

DESCRIPTION


-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_approx_contours : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_approx_contours, CPPExtern);
	
	public:
		
		//////////
		// Constructor
		cvlib_approx_contours(t_float);

	protected:

		//////////
		// Destructor
		~cvlib_approx_contours();
		
		int iterations;
		
		
		void processCvContours (int i);
		void sendCvContoursSID();
		void setApproxMess(float f);

	private:

		struct  CvMemStorage *storage;
		CvSeq* contours;
		CvSeq* result;
		CvSeq* approximated_contours;
		CvSeq* approximated_contours_head;
		int cvSeqSID;
		//  CvSeq* result;
		float tolerance;
		
		t_atom my_output;

		t_outlet *m_cvContoursOutlet;
		t_outlet *m_nbOfContoursOutlet;


		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void setApproxMessCallback(void *data, t_floatarg _value);



};



// end of #ifndef statement
#endif

