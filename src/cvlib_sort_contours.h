/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifndef INCLUDE_CVLIB_SORT_CONTOURS_H_
#define INCLUDE_CVLIB_SORT_CONTOURS_H_

#ifndef _EiC
//	#include "opencv2/legacy/legacy.hpp"
//	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours

struct weighted_contour {
    float weight;
    CvSeq* contour;
    };

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_sort_contours
    sort cv_contours on multiple outlets

DESCRIPTION



-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_sort_contours : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_sort_contours,CPPExtern);

	public:
		
		//////////
		// Constructor
    		cvlib_sort_contours(t_float n);

	protected:
		
		//////////
		// Destructor
		~cvlib_sort_contours();
		
		void processCvContours (int i);		
		void processList(int argc,t_atom *argv);

	private:
		struct  CvMemStorage *storage1;
		// struct  CvMemStorage *storage2;
		
		CvSeq* current_contour;
		CvSeq* head_output_contour;
		CvSeq* current_output_contour;
		int output_cvSeqSID;
		int nb_of_sorted_items;
		
		int contour_weight_vector_size;
		int contour_processed;
		std::vector<weighted_contour> sorted_contours;
		float contour_weight_vector[CVLIB_MAX_CONTOURS];
		
		t_atom my_output;
		t_outlet *m_cvContoursOutlet;
	
		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void processListCallback(void *data, t_symbol*s,int argc,t_atom *argv);

};



// end of #ifndef statement
#endif

