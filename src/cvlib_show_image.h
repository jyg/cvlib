#ifndef INCLUDE_CVLIB_SHOW_IMAGE_H_
#define INCLUDE_CVLIB_SHOW_IMAGE_H_

#ifndef _EiC
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#endif


#include "Base/GemPixObj.h"
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_show_image
    
    previsualization window object

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/
int cvlib_show_image_instance_counter;
class GEM_EXPORT cvlib_show_image: public GemPixObj
{
	CPPEXTERN_HEADER(cvlib_show_image,GemPixObj)

	public:

		//////////
	    	// Constructor
    		cvlib_show_image(int,t_atom*);
    
	protected:
		//////////
    		// Destructor
    		virtual ~cvlib_show_image();

    		//IplImage * tempImg;
    		//CvSize previous_imagesize;
		CvSize imagesize;
    		int fixed_size;
    		int vis;

		// to detect changes in the image size
		int 		comp_xsize;
		int		comp_ysize;
	
        
		//Window popup Name & properties
    		char wndname[25];
    		int window_created;

		//////////
    		// Do the processing
		virtual void 	processRGBAImage(imageStruct &image);
    		virtual void 	processRGBImage(imageStruct &image);
		virtual void 	processYUVImage(imageStruct &image);
    		virtual void 	processGrayImage(imageStruct &image); 

		//////////
    		// Set the new edge threshold
    		void floatShowHideMess(float flag);


	private:

	   	//////////
	    	// Static member functions
 
 		static void floatShowHideMessCallback(void *data, float vis);

		/////////
		// IplImage needed
    		IplImage *rgba, *rgb, *gray;
};

#endif    //  for header file



