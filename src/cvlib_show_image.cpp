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


#include "cvlib_show_image.h"
#include <stdio.h>

CPPEXTERN_NEW_WITH_GIMME(cvlib_show_image)

/////////////////////////////////////////////////////////
//
// cvlib_show_image
//
/////////////////////////////////////////////////////////

// Constructor
//
/////////////////////////////////////////////////////////



cvlib_show_image :: cvlib_show_image(int argc, t_atom*argv)
    {
	 cvlib_show_image_instance_counter++;
	fixed_size=0;
        vis=1;
	window_created=0;
	t_symbol wname;
        imagesize.width=imagesize.height=256;

	rgba=NULL;
	gray=NULL;
	rgb=NULL;
	
	// set cvWindow default name
	sprintf (wndname, "ShowImage:untitled%d",cvlib_show_image_instance_counter);
	
	if(argc >= 1 && A_SYMBOL==argv[0].a_type)  // first argument is window title
	{ 
//		verbose(-1,"%d arguments de création",argc);
		t_symbol* s=atom_getsymbol(argv+0);
		if (s!=0)
		{
			// convertion t_symbol* en char*
			atom_string(argv, wndname, 24);
			
		}
		if(argc ==3)
			if(A_FLOAT==argv[1].a_type && A_FLOAT==argv[2].a_type)  
				// second & third args for specifying window fixed size
			{
				fixed_size=1;
                    		imagesize.width= atom_getint(argv+1);
                    		imagesize.height= atom_getint(argv+2);
			
			}
	
		
	}
	else 	
		if(argc == 2 && A_FLOAT==argv[0].a_type && A_FLOAT==argv[1].a_type)
		
		// first & second  args for specifying window fixed size
	{
		fixed_size=1;
    		imagesize.width= atom_getint(argv+0);
    		imagesize.height= atom_getint(argv+1);
	
	}
	
	if (imagesize.width < 1 && imagesize.height < 1)
	{
		verbose(-1,"cvShowImage : incorrect window size (%d,%d)",imagesize.width,imagesize.height);
		fixed_size=0;
    		imagesize.width= 256;
    		imagesize.height= 256;
	}
    

 
//	verbose(-1,"nom de la fenetre =%s",wndname);
//	verbose(-1,"dimensions = %d x %d",imagesize.width,imagesize.height);
        cvNamedWindow(wndname, CV_WINDOW_NORMAL);
	window_created=1;
        cvResizeWindow( wndname, imagesize.width, imagesize.height );

    }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////

cvlib_show_image::~cvlib_show_image()
    {
   	//Destroy cv_images to clean memory
        if (rgba)
		cvReleaseImage(&rgba);
	if (gray)
		cvReleaseImage(&gray);
        if (rgb)
		cvReleaseImage(&rgb);
 
	// Destroy Window
	if(window_created)
      		cvDestroyWindow(wndname);
	window_created=0;
    }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////

void cvlib_show_image::processRGBAImage(imageStruct &image)

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

		// resize the window
		 if(!fixed_size)
		{
	        	if (window_created)
				cvResizeWindow(wndname, image.xsize, image.ysize);
			imagesize.width= image.xsize;
			imagesize.height= image.ysize;
    		} 
	}
	//  memcpy( rgba->imageData, image.data, image.xsize*image.ysize*4 );
       rgba->imageData = (char*) image.data;
	if (vis)
	{
	if (!window_created)
		{
			cvNamedWindow(wndname, CV_WINDOW_NORMAL);
			cvResizeWindow(wndname, imagesize.width, imagesize.height);
			window_created=1;
		}
            cvShowImage(wndname, rgba);
	  cvWaitKey(1);
	}
}

void cvlib_show_image :: processRGBImage(imageStruct &image)
{
  post( "cvlib_show_image : RGB format not supported" );
}

void cvlib_show_image :: processYUVImage(imageStruct &image)
{
  post( "cvlib_show_image : yuv format not supported" );
}

void cvlib_show_image :: processGrayImage(imageStruct &image)
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
	        //rgba = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 4);
	        //rgb = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 3);
    		gray = cvCreateImage(cvSize(image.xsize,image.ysize), IPL_DEPTH_8U, 1);

		// resize the window
		 if(!fixed_size)
		{
	        	if (window_created)
				cvResizeWindow(wndname, image.xsize, image.ysize);
			imagesize.width= image.xsize;
			imagesize.height= image.ysize;
    		} 
	}
	//  memcpy( gray->imageData, image.data, image.xsize*image.ysize*4 );
       gray->imageData = (char*) image.data;
	if (vis)
	{
	if (!window_created)
		{
			cvNamedWindow(wndname, CV_WINDOW_NORMAL);
			cvResizeWindow(wndname, imagesize.width, imagesize.height);
			window_created=1;
		}
            cvShowImage(wndname, gray);
	  cvWaitKey(1);
	}
}



void cvlib_show_image::floatShowHideMess (float flag)
    {
      vis=flag>0;
      if (vis)
        {
        cvNamedWindow(wndname, CV_WINDOW_NORMAL);
        cvResizeWindow( wndname, imagesize.width, imagesize.height );
        }
      else
        cvDestroyWindow(wndname);
      cvWaitKey(1);
    }



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_show_image :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_show_image),
                   gensym("cvShowImage"),
                   A_GIMME);  // with variable arguments number

	// define help-file
	class_sethelpsymbol(cvlib_show_image_class,  gensym("cvShowImage"));

	class_addmethod(classPtr, (t_method)&cvlib_show_image::floatShowHideMessCallback,
  		  gensym("vis"), A_FLOAT, A_NULL);	
	
	cvlib_show_image_instance_counter=0;
}

void cvlib_show_image :: floatShowHideMessCallback(void *data, t_floatarg flag)
{
	GetMyClass(data)->floatShowHideMess((float)flag);
}

