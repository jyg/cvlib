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

#include "cvlib_interpol_contours.h"
CPPEXTERN_NEW_WITH_ONE_ARG(cvlib_interpol_contours,t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// cvlib_interpol_contours
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
cvlib_interpol_contours::cvlib_interpol_contours(t_float _nb_points)
    {
       	// first inlet for cvContour already created by CPPExtern class

	// second inlet for specifying nb of interpolation points
	inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("float"), gensym("interpol"));


 	// outlet for processed contour
	m_cvContoursOutlet = outlet_new(this->x_obj, 0);    // contours output

        nb_of_interpolation_points=64;
        if (_nb_points>0)
                nb_of_interpolation_points=_nb_points;
	if (_nb_points>CVINTERPOL_MAX_POINTS)
		nb_of_interpolation_points=CVINTERPOL_MAX_POINTS;

         // create storage
        storage = cvCreateMemStorage(0);
        storage2 = cvCreateMemStorage(0);

    // create contour pointer
        cvSeqSID= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);

    }

///////////////////////////////////
// DESTRUCTOR
cvlib_interpol_contours::~cvlib_interpol_contours()
{
        cvlib_freeHeader(cvSeqSID);
     // free storage
        cvReleaseMemStorage(&storage);
	cvReleaseMemStorage(&storage2);
}

void cvlib_interpol_contours::setInterpolMess(float f)
{
       
        if (f>0)
                nb_of_interpolation_points=(int)f;
	if (nb_of_interpolation_points>CVINTERPOL_MAX_POINTS)
		nb_of_interpolation_points=CVINTERPOL_MAX_POINTS;
 }



/////////////////////////////////////////////////////////
// processContours
//
/////////////////////////////////////////////////////////

void cvlib_interpol_contours::processCvContours(int i)
{

   if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
    {
        contours=(CvSeq*)cvlib_get_pdata(i);
        int contours_count=0;
        while (contours)
	{
		if (contours->total>1)    // skip smaller contours
		{
		            contours_count++;
		            CvPoint* pcurrent_point;
		            CvPoint* pprevious_point;
		            CvPoint interpol_point;
		            //current_contour=(CvSeq*)cvCloneSeq(contours,storage2);
		            current_contour=contours;
		            int j;
		            float arc_element=cvContourPerimeter(contours)/nb_of_interpolation_points;
		            float cumuled_perimeter=0;
		            float current_arc=0;
		         //   float index_step=(float)contours->total/nb_of_interpolation_points;
		
		            CvSeqWriter writer;
		            cvStartWriteSeq( CV_SEQ_ELTYPE_POINT|CV_SEQ_KIND_CURVE|CV_SEQ_FLAG_CLOSED, sizeof(CvSeq), sizeof(CvPoint), storage2, &writer );
		
		            // output first point
		            pcurrent_point = (CvPoint*)cvGetSeqElem ( current_contour, 0 );
		
		            CV_WRITE_SEQ_ELEM( *pcurrent_point, writer );
		
		
		
		           j=1;
		           
			// begin loop
		            for(int i=1; i< nb_of_interpolation_points; i++)
		            {
		
		
			            // parcourir les points jusqu'à obtenir une longueur d'arc suffisante
			            while((cumuled_perimeter+(current_arc=cvArcLength(current_contour,cvSlice(j-1,j))))<(arc_element*i))
			            {
			                // add current arc to cumulated perimeter and go to next point of contour
			                cumuled_perimeter+=current_arc;
					j++;
			                if (j>=current_contour->total)
			                    break;
			            }
		
			
			            float interpol_ratio=((cumuled_perimeter+current_arc)-(arc_element*i))/current_arc;
			            /// this test may be unnecessary
			            pcurrent_point = (CvPoint*)cvGetSeqElem ( current_contour, j);
			
				
			            pprevious_point = (CvPoint*)cvGetSeqElem ( current_contour, j-1);
		
				
			            interpol_point.x=(pprevious_point->x)*interpol_ratio+(1-interpol_ratio)*(pcurrent_point->x);
		        	    interpol_point.y=(pprevious_point->y)*interpol_ratio+(1-interpol_ratio)*(pcurrent_point->y);
			
			
		           	 CV_WRITE_SEQ_ELEM( interpol_point, writer );
		
		            }
		            new_seq = cvEndWriteSeq( &writer );
		
		
		// store result in approximated_contours sequence
		            if (contours_count==1)
		            {
		                // first occurence : store in head of dynamic structure
		                interpolated_contours_head=new_seq;
		                interpolated_contours=new_seq;
		
		            }
		            else
		            {
		                // link to dynamic structure
		                interpolated_contours->h_next=new_seq;
		                // change current poointer
		                interpolated_contours=new_seq;
		
		            }
		            // go to next contour
		            contours = contours->h_next;	
	    	    }
	else	// skip contour with only one point
		contours = contours->h_next;
	}  // end while loop

                // set instance pointer to contours
        cvlib_set_pdata(cvSeqSID,interpolated_contours_head);
        // send cv contour SID
        SETFLOAT(&my_output,cvSeqSID);
	outlet_anything(m_cvContoursOutlet,gensym("cvContours"),1,&my_output);
    }
    cvClearMemStorage(storage2);
 }


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_interpol_contours :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_interpol_contours),
                   gensym("cvInterpolContours"),A_DEFFLOAT,A_NULL);  // with one argument

	// old alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_interpol_contours),
                   gensym("cvContoursInterpolPoints"),A_DEFFLOAT,A_NULL);  // with one argument

	// define help-file
	class_sethelpsymbol(cvlib_interpol_contours_class,  gensym("cvInterpolContours"));

	class_addmethod(classPtr, (t_method)&cvlib_interpol_contours::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_interpol_contours::setInterpolMessCallback,
  		  gensym("interpol"), A_DEFFLOAT, A_NULL);	

}

void cvlib_interpol_contours :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

void cvlib_interpol_contours :: setInterpolMessCallback(void *data, t_floatarg _f)
{
	GetMyClass(data)->setInterpolMess((float)_f);
}
