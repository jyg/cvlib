/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef INCLUDE_CVLIB_ROUTE_CONTOURS_H_
#define INCLUDE_CVLIB_ROUTE_CONTOURS_H_
#define CVROUTE_MAX_NB_OF_OUTLETS 10
#define CVROUTE_CLONE_INPUT_CONTOURS 1

#ifndef _EiC
//	#include "opencv2/legacy/legacy.hpp"
//	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_route_contours
 
    routing cv_contours on multiple outlets

DESCRIPTION



-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_route_contours : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_route_contours,CPPExtern);

	public:
		
		//////////
		// Constructor
		cvlib_route_contours(int argc,t_atom *argv);
 
	protected:

		//////////
		// Destructor
		~cvlib_route_contours();

  		void processCvContours (int i);
		void sendCvContourSidToOutlet(int outlet, int sid);

		void processList(int argc,t_atom *argv);

	private:
		struct  CvMemStorage *storage1;
		// struct  CvMemStorage *storage2;
		
		CvSeq* current_contour;
		CvSeq* head_contour;
		
		CvSeq* head_contour_left;
		CvSeq* head_contour_right;
		CvSeq* current_contour_left;
		CvSeq* current_contour_right;
		
		
		int cvSeqSID_right;
		int parameters_count;
		int cloneInputContours;
		
		CvSeq* contour_head[CVROUTE_MAX_NB_OF_OUTLETS];
		CvSeq* contour_current[CVROUTE_MAX_NB_OF_OUTLETS];
		int cvSeqSID[CVROUTE_MAX_NB_OF_OUTLETS];
		int matching_values[CVROUTE_MAX_NB_OF_OUTLETS];
		
		int routing_vector_size;
		int contour_processed;
		int routing_vector[CVLIB_MAX_CONTOURS];
		
		t_atom my_output;
		t_outlet *m_cvContoursOutlet;
		t_outlet *m_routingOutlet[CVROUTE_MAX_NB_OF_OUTLETS+1];
	//	t_outlet *m_noMatchOutlet;

		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void processListCallback(void *data, t_symbol*s,int argc,t_atom *argv);
};



// end of #ifndef statement
#endif

