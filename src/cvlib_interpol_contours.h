/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CVLIB_INTERPOLCONTOURS_H_
#define INCLUDE_CVLIB_INTERPOLCONTOURS_H_


#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours


#define CVINTERPOL_MAX_POINTS 1024


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_interpol_contours

    
DESCRIPTION
	interpolation of cv_contours


-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_interpol_contours : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_interpol_contours, CPPExtern);
	
	public:
		
		//////////
		// Constructor
		cvlib_interpol_contours(t_float);

	protected:

		//////////
		// Destructor
		~cvlib_interpol_contours();
		
		void processCvContours (int i);
		void setInterpolMess(float f);


	private:
		struct  CvMemStorage *storage;
		struct  CvMemStorage *storage2;
		int cvSeqSID;
		CvSeq* contours;
		CvSeq* current_contour;
		CvSeq* new_seq;
		CvSeq* interpolated_contours;
		CvSeq* interpolated_contours_head;
		int nb_of_interpolation_points;
		t_atom my_output;
		t_outlet *m_cvContoursOutlet;


		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void setInterpolMessCallback(void *data, t_floatarg _value);

};



// end of #ifndef statement
#endif

