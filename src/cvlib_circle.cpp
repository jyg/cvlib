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

#include "cvlib_circle.h"

CPPEXTERN_NEW(cvlib_circle)

/////////////////////////////////////////////////////////
//
// cvlib_circle
//
/////////////////////////////////////////////////////////

// Constructor
//
/////////////////////////////////////////////////////////

cvlib_circle::cvlib_circle()
    {
	rgba=NULL;
	gray=NULL;
	rgb=NULL;

        nb_of_circles=0;
  
        // set  default values
 
        width=1;
        color_r=255;
        color_g=255;
        color_b=255;

    }

// DESTRUCTOR
cvlib_circle::~cvlib_circle()
    {
// free extra ressources

    }


void cvlib_circle::addCircleMess(int _x,int _y, int _r)
{
    if(nb_of_circles<nb_max_of_circles)
    {
        circles[nb_of_circles].center=cvPoint(_x,_y);
	if (_r < 0) 
		circles[nb_of_circles].r=0;
	else
        	circles[nb_of_circles].r=_r;
        circles[nb_of_circles].width=width;
        circles[nb_of_circles].color=CV_RGB(color_r,color_g,color_b);
        // force image processing on next gemstate   ?? TO DO : COMMENT FORCER IMG-PROCESSING ?
   // output_dirty_flag=1;
    nb_of_circles++;
    }
}


void cvlib_circle::setWidthMess(int _w)
{
   width=_w;
	if (width > 255)
		width=255;
	if (width < -1)
		width = -1;
        // force image processing on next gemstate
   // output_dirty_flag=1;
}

void cvlib_circle::setColorMess(int _r,int _g, int _b)
{
   	color_r=((int)_r)&255;
	color_g=((int)_g)&255;
	color_b=((int)_b)&255;
}

void cvlib_circle::clearMess()
{
   	nb_of_circles=0;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////

void cvlib_circle::processRGBAImage(imageStruct &image)

{
   	unsigned char *pixels = image.data;  

	if ((this->comp_xsize!=image.xsize)||(this->comp_ysize!=image.ysize)||(!rgba)) 
	{
  		this->comp_xsize = image.xsize;
		this->comp_ysize = image.ysize;

	    	//Destroy cv_images to clean memory
	        if ( rgba )
	        {
			cvReleaseImage(&rgba);
		    	cvReleaseImage(&gray);
		    	cvReleaseImage(&rgb);
	        }

		//create the orig image with new size
	        rgba = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 4);
	        rgb = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 3);
    		gray = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 1);


	}
	//  memcpy( rgba->imageData, image.data, image.xsize*image.ysize*4 );
        rgba->imageData = (char*) image.data;

        for(int i=0;i<nb_of_circles;i++)
            cvCircle( rgba, circles[i].center, circles[i].r, circles[i].color, circles[i].width, 8,0);

       // reset for next iteration
       // nb_of_circles=0;
}

void cvlib_circle :: processRGBImage(imageStruct &image)
{
  post( "cvlib_circle : RGB format not supported" );
}

void cvlib_circle :: processYUVImage(imageStruct &image)
{
  post( "cvlib_circle : yuv format not supported" );
}

void cvlib_circle :: processGrayImage(imageStruct &image)
{
  post( "cvlib_circle : gray format not supported" );
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_circle :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_circle),
                   gensym("cvDrawCircles"),A_NULL);  // with no argument

	// define help-file
	class_sethelpsymbol(cvlib_circle_class,  gensym("cvDrawCircles"));

	class_addmethod(classPtr, (t_method)&cvlib_circle::setWidthMessCallback,
  		  gensym("width"), A_DEFFLOAT, A_NULL);	
	
	class_addmethod(classPtr, (t_method)&cvlib_circle::addCircleMessCallback,
  		  gensym("add_circle"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);	
	
	class_addmethod(classPtr, (t_method)&cvlib_circle::setColorMessCallback,
  		  gensym("color"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);

	class_addmethod(classPtr, (t_method)&cvlib_circle::clearMessCallback,
  		  gensym("clear"), A_NULL);	
	
}

void cvlib_circle :: setWidthMessCallback(void *data, t_floatarg _w)
{
	GetMyClass(data)->setWidthMess((int)_w);
}


void cvlib_circle :: addCircleMessCallback(void *data, t_floatarg _x,t_floatarg _y,t_floatarg _r)
{
	GetMyClass(data)->addCircleMess((int)_x,(int)_y,(int)_r);
}

void cvlib_circle :: setColorMessCallback(void *data, t_floatarg _r,t_floatarg _g,t_floatarg _b)
{
	GetMyClass(data)->setColorMess((int)_r,(int)_g,(int)_b);
}

void cvlib_circle :: clearMessCallback(void *data)
{
	GetMyClass(data)->clearMess();
}
