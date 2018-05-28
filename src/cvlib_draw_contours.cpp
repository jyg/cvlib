////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "cvlib_draw_contours.h"

CPPEXTERN_NEW(cvlib_draw_contours)

/////////////////////////////////////////////////////////
//
// cvlib_draw_contours
//
/////////////////////////////////////////////////////////

// Constructor
//
/////////////////////////////////////////////////////////

cvlib_draw_contours::cvlib_draw_contours()
    {
	//inlet for cvContour
	inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("cvContours"), gensym("cvContours"));

       	rgba=NULL;
	gray=NULL;
	rgb=NULL;

	contours_to_draw=0;
        thickness=1;

        outer_contour_color=inner_contour_color=CV_RGB(255,255,255);

    }

// DESTRUCTOR
cvlib_draw_contours::~cvlib_draw_contours()
{
   	//Destroy cv_images to clean memory
        if (rgba)
		cvReleaseImage(&rgba);
	if (gray)
		cvReleaseImage(&gray);
        if (rgb)
		cvReleaseImage(&rgb);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////


void cvlib_draw_contours::processRGBAImage(imageStruct &image)
{
	unsigned char *pixels = image.data;  

	if ((this->comp_xsize!=image.xsize)||(this->comp_ysize!=image.ysize)||(!rgba)) 
	{
  		//verbose(-1,"redimensionnement");
		this->comp_xsize = image.xsize;
		this->comp_ysize = image.ysize;

	    	//Destroy cv_images to clean memory
	        if (rgba)
			cvReleaseImage(&rgba);
 		if (gray)
			cvReleaseImage(&gray);
	        if (rgb)
			cvReleaseImage(&rgb);
 
		//create the orig image with new size
	        rgba = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 4);
//	        rgb = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 3);
//    		gray = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 1);

	}

	//  memcpy( rgba->imageData, image.data, image.xsize*image.ysize*4 );
       rgba->imageData = (char*) image.data;

	while (contours_to_draw)
	{
		// cvDrawContours args : ( CvArr *img, CvSeq* contour,
		// 	CvScalar external_color, CvScalar hole_color,
		//	int max_level, int thickness=1,
		// 	int line_type=8 )
	cvDrawContours(rgba, contours_to_draw, outer_contour_color, inner_contour_color, 0, thickness, 8 );
	contours_to_draw=contours_to_draw->h_next;
	}
}

void cvlib_draw_contours::processGrayImage(imageStruct &image)
{
	unsigned char *pixels = image.data;  

	if ((this->comp_xsize!=image.xsize)||(this->comp_ysize!=image.ysize)||(!gray)) 
	{
  		//verbose(-1,"redimensionnement");
		this->comp_xsize = image.xsize;
		this->comp_ysize = image.ysize;

	    	//Destroy cv_images to clean memory
	        if (rgba)
			cvReleaseImage(&rgba);
 		if (gray)
			cvReleaseImage(&gray);
	        if (rgb)
			cvReleaseImage(&rgb);
 
		//create the orig image with new size
//	        rgba = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 4);
//	        rgb = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 3);
    		gray = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 1);

	}

	//  memcpy( rgba->imageData, image.data, image.xsize*image.ysize );
       gray->imageData = (char*) image.data;

	while (contours_to_draw)
	{
		// cvDrawContours args : ( CvArr *img, CvSeq* contour,
		// 	CvScalar external_color, CvScalar hole_color,
		//	int max_level, int thickness=1,
		// 	int line_type=8 )
	cvDrawContours(gray, contours_to_draw, outer_contour_color, inner_contour_color, 0, thickness, 8 );
	contours_to_draw=contours_to_draw->h_next;
	}
}

void cvlib_draw_contours :: processRGBImage(imageStruct &image)
{
  post( "cvlib_draw_contours : RGB format not supported" );
}

void cvlib_draw_contours :: processYUVImage(imageStruct &image)
{
  post( "cvlib_draw_contours : yuv format not supported" );
}

void cvlib_draw_contours::setCvContoursMess(int i)
{
   if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR )
        contours_to_draw=(CvSeq*)cvlib_get_pdata(i);
}




void cvlib_draw_contours::setWidthMess(int i)
{
    thickness=i;
}

void cvlib_draw_contours::setColorMess (int _r, int _g, int _b)
{
	inner_contour_color=outer_contour_color=CV_RGB(_r & 255,_g & 255,_b & 255);
}

void cvlib_draw_contours::setInnerColorMess (int _r, int _g, int _b)
{
	inner_contour_color=CV_RGB(_r & 255,_g & 255,_b & 255);
}

void cvlib_draw_contours::setOuterColorMess (int _r, int _g, int _b)
{
	outer_contour_color=CV_RGB(_r & 255,_g & 255,_b & 255);
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_draw_contours :: obj_setupCallback(t_class *classPtr)
{ 
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_draw_contours),
                   gensym("cvDrawContours"),A_NULL);  // with no argument

	// define help-file
	class_sethelpsymbol(cvlib_draw_contours_class,  gensym("cvDrawContours"));

	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setWidthMessCallback,
  		  gensym("width"), A_DEFFLOAT, A_NULL);	
	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setWidthMessCallback,
  		  gensym("thickness"), A_DEFFLOAT, A_NULL);	

	
	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setCvContoursMessCallback,
  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	
	
	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setColorMessCallback,
  		  gensym("color"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setInnerColorMessCallback,
  		  gensym("int_color"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&cvlib_draw_contours::setOuterColorMessCallback,
  		  gensym("out_color"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);

}

void cvlib_draw_contours :: setWidthMessCallback(void *data, t_floatarg _w)
{
	GetMyClass(data)->setWidthMess((int)_w);
}


void cvlib_draw_contours :: setCvContoursMessCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->setCvContoursMess((int)_c);
}

void cvlib_draw_contours :: setInnerColorMessCallback(void *data, t_floatarg _r,t_floatarg _g,t_floatarg _b)
{
	GetMyClass(data)->setInnerColorMess((int)_r,(int)_g,(int)_b);
}

void cvlib_draw_contours :: setOuterColorMessCallback(void *data, t_floatarg _r,t_floatarg _g,t_floatarg _b)
{
	GetMyClass(data)->setOuterColorMess((int)_r,(int)_g,(int)_b);
}

void cvlib_draw_contours :: setColorMessCallback(void *data, t_floatarg _r,t_floatarg _g,t_floatarg _b)
{
	GetMyClass(data)->setColorMess((int)_r,(int)_g,(int)_b);
}

