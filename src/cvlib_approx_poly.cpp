////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "cvlib_approx_poly.h"

CPPEXTERN_NEW_WITH_ONE_ARG(cvlib_approx_contours,t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// cvlib_approx_contours
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

cvlib_approx_contours::cvlib_approx_contours(t_float _approx)
{
	//first inlet for cvContour
	
	// second inlet for approximation  /// to do : passive inlet ?
	inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("float"), gensym("approx"));
	
	// create storage
	storage = cvCreateMemStorage(0);
	
	// create contour
	cvSeqSID= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);
	
	if(_approx <=0) 
		setApproxMess(0.01);
	else
		setApproxMess(_approx);

	m_cvContoursOutlet = outlet_new(this->x_obj, 0);    // contours output
	m_nbOfContoursOutlet = outlet_new(this->x_obj, 0);    // nb of contours output
}

/////////////////////////////
// DESTRUCTOR

cvlib_approx_contours::~cvlib_approx_contours()
    {
        cvlib_freeHeader(cvSeqSID);
     // free storage
        cvReleaseMemStorage(&storage);
    }


void cvlib_approx_contours::setApproxMess(float f)
{
    	if(f<= 0) 
		tolerance=0;
	else 
		tolerance = f;
 }


void cvlib_approx_contours::sendCvContoursSID()
{
	if (cvSeqSID>=0)
	{
		SETFLOAT(&my_output,cvSeqSID);
		outlet_anything(m_cvContoursOutlet,gensym("cvContours"),1,&my_output);
	}
}



/////////////////////////////////////////////////////////
// processContours
//
/////////////////////////////////////////////////////////
void cvlib_approx_contours::processCvContours(int i)
{
	if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
	{
		contours=(CvSeq*)cvlib_get_pdata(i);
		approximated_contours=approximated_contours_head=0;
		int  count=0;
		while (contours)
		{
			count++;
			if (tolerance< 1.)
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, cvContourPerimeter(contours)*tolerance, 0 );
			else
				result = cvApproxPoly( contours, sizeof(CvContour), storage,
					CV_POLY_APPROX_DP, tolerance, 0 );
			
			// copy CV_SEQ_FLAG_HOLE
			result->flags|=contours->flags & CV_SEQ_FLAG_HOLE;

			
			/// TO DO : add matching criteria here (such as min size, etc...)
			//is a external or internal contour ?
			// the test is : (contour->flags & CV_SEQ_FLAG_HOLE)
			
			// store result in approximated_contours sequence
		
			if (count==1)
			{
				// first occurence
				approximated_contours_head=result;
				approximated_contours=result;
			}
			else
			{
				// append
				approximated_contours->h_next=result;
				approximated_contours=result;
			}
	
			contours = contours->h_next;
		}
	
	// output found_contours
	outlet_float(m_nbOfContoursOutlet,(float)count);
	
	// set instance pointer to contours
	cvlib_set_pdata(cvSeqSID,approximated_contours_head);
	
	// send cv contour SID
	sendCvContoursSID();
	
	
	}
	
	// clear memory storage - reset free space position
	cvClearMemStorage(storage);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_approx_contours :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_approx_contours),
                   gensym("cvApproxContours"),A_DEFFLOAT,A_NULL);  // with one argument

	// define help-file
	class_sethelpsymbol(cvlib_approx_contours_class,  gensym("cvApproxContours"));

	class_addmethod(classPtr, (t_method)&cvlib_approx_contours::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	
	class_addmethod(classPtr, (t_method)&cvlib_approx_contours::setApproxMessCallback,
  		  gensym("approx"), A_DEFFLOAT, A_NULL);	
	

}

void cvlib_approx_contours :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}


void cvlib_approx_contours :: setApproxMessCallback(void *data, t_floatarg _f)
{
	GetMyClass(data)->setApproxMess((float)_f);
}

