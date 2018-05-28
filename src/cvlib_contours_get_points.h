/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Respond to mouse events

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/



#ifndef INCLUDE_CVLIB_CONTOURS_GET_POINTS_H_
#define INCLUDE_CVLIB_CONTOURS_GET_POINTS_H_

#ifndef _EiC
//	#include "opencv2/legacy/legacy.hpp"
	#include "opencv2/legacy/compat.hpp"
#endif

#include "Base/CPPExtern.h"
#include "cvlib_find_contours.h"  // for cvContours
#include <stdio.h>

#define CVLIB_GET_MAX_POINTS 256

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_contours_get_points

    
DESCRIPTION
	extract points from contours

-----------------------------------------------------------------*/

class GEM_EXTERN cvlib_contours_get_points : public CPPExtern
{
	CPPEXTERN_HEADER (cvlib_contours_get_points, CPPExtern);
	
	public:
		
		//////////
		// Constructor
		cvlib_contours_get_points(int argc,t_atom *argv);

	protected:

		//////////
		// Destructor
		~cvlib_contours_get_points();

		void processCvContours (int i);
		void setNbPointsMess(float f);
		void setArraysMess(t_symbol* bufname_x,t_symbol* bufname_y);
		void setTabOutputMess(int mode);
		void setNormalizeMess(float width, float height);		
		void setOffsetMess(int _offset);


		int i;
		
		//data for output arrays
		int vecsize_x,vecsize_y;
		t_garray *a_x;
		t_garray *a_y;
		t_word *vec_x;
		t_word *vec_y;
		t_symbol *bufname_x;
		t_symbol *bufname_y;
		//buffer buf_x;
		//buffer buf_y;

		int offset;

		float normalize_x,normalize_y;

		// check and eventually update buffers references (return true if valid)
	//	bool Check(buffer &buf,const t_symbol *bufname);

	private:
		
		CvSeq* contours;
		CvPoint* PointArray;
		int max_nb_of_points;
		int outputmode;// 0= points, 1= tables

		t_outlet *m_leftOutlet;
		t_outlet *m_nbOfPointsOutlet;

		
		t_atom rlist[4];

		//////////
		// Static member functions
		static void processCvContoursCallback(void *data, t_floatarg _sid);
		static void setNbPointsCallback(void *data, t_floatarg _value);
		static void setArraysMessCallback(void *data, t_symbol* bufname_x,t_symbol* bufname_y);
		
		static void setTabOutputMessCallback(void *data, t_floatarg _value);
		static void setNormalizeMessCallback(void *data, t_floatarg _width,  t_floatarg _height );		
		static void setOffsetMessCallback(void *data, t_floatarg _offset);


   };



// end of #ifndef statement
#endif

