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

#include "cvlib_contours_convex_hull.h"

CPPEXTERN_NEW(cvlib_contours_convex_hull);

/////////////////////////////////////////////////////////
//
// cvlib_contours_convex_hull
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

cvlib_contours_convex_hull::cvlib_contours_convex_hull()
{
	//first inlet for cvContours input
	// (created within CPPextern class)
	
	// add an outlet for cvContours output
	m_cvContoursOutlet = outlet_new(this->x_obj, 0);    
	
	
	// create storage
	storage = cvCreateMemStorage(0);
	
	// create contour
	cvSeqSID= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);
	
    }

/////////////////////////////
// DESTRUCTOR
cvlib_contours_convex_hull::~cvlib_contours_convex_hull()
{
	cvlib_freeHeader(cvSeqSID);
	// free storage
	cvReleaseMemStorage(&storage);
}


void cvlib_contours_convex_hull::sendCvContoursSID()
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
void cvlib_contours_convex_hull::processCvContours(int i)
{
  
	int contours_count=0;
	int* hull;
	int hullsize;
	CvPoint* PointArray;
	CvPoint* ConvexPointArray;
	
	if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
	{
		contours=(CvSeq*)cvlib_get_pdata(i);
		
		convex_contours=convex_contours_head=0;
		while (contours)
		{
		    contours_count++;
		    int count=contours->total;
		
		    // Alloc memory for contour point set.
		
		    PointArray = (CvPoint*)malloc(count * sizeof(CvPoint) );
		
		    // Alloc memory for indices of convex hull vertices.
		    hull = (int*)malloc(sizeof(int) * count);
		
		    // Get contour point set.
		    cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);
		
		    // Find convex hull for curent contour.
		    cvConvexHull( PointArray,
		              count,
		              NULL,
		              CV_COUNTER_CLOCKWISE,
		              hull,
		              &hullsize);
		      // creating cvseq contour from convex hull for further purposes (cvPointPolygonTest)
		    ConvexPointArray = (CvPoint*)malloc(sizeof(CvPoint)*hullsize);
		    CvSeqWriter writer;
		    cvStartWriteSeq( CV_SEQ_ELTYPE_POINT|CV_SEQ_KIND_CURVE|CV_SEQ_FLAG_CLOSED, sizeof(CvSeq), sizeof(CvPoint), storage, &writer );
		    for( int ii=0;ii<hullsize; ii++ )
		    {
		        CV_WRITE_SEQ_ELEM( PointArray[hull[ii]], writer );
		    }
		    convex_seq = cvEndWriteSeq( &writer );
		
		// store result in approximated_contours sequence
		    if (contours_count==1)
		    {
		        // first occurence : store in head of dynamic structure
		        convex_contours_head=convex_seq;
		        convex_contours=convex_seq;
		
		    }
		    else
		    {
		        // link to dynamic structure
		        convex_contours->h_next=convex_seq;
		        // change current poointer
		        convex_contours=convex_seq;
		
		    }
		    // go to next contour
		    contours = contours->h_next;
		
		          // Free memory.
		    free(PointArray);
		    free(ConvexPointArray);
		    free(hull);
		
		}
	
		// set instance pointer to contours
		cvlib_set_pdata(cvSeqSID,convex_contours_head);
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
void cvlib_contours_convex_hull :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_contours_convex_hull),
                   gensym("cvContoursConvexHull"),A_DEFFLOAT,A_NULL);  // with one argument

	// define help-file
	class_sethelpsymbol(cvlib_contours_convex_hull_class,  gensym("cvContoursConvexHull"));

	class_addmethod(classPtr, (t_method)&cvlib_contours_convex_hull::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	


}

void cvlib_contours_convex_hull :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

