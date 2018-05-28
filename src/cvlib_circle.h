#ifndef INCLUDE_CVLIB_CIRCLE_H_
#define INCLUDE_CVLIB_CIRCLE_H_


#ifndef _EiC
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#endif

#define nb_max_of_circles 10000

#include "Base/GemPixObj.h"
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_circle
    
    draw circles in pix domain (no opengl)

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/

class GEM_EXPORT cvlib_circle: public GemPixObj
{
	CPPEXTERN_HEADER(cvlib_circle,GemPixObj)

	public:

		//////////
	    	// Constructor

    		cvlib_circle();

	protected:
		//////////
    		// DESTRUCTOR
    		virtual ~cvlib_circle();

		// Drawing functions
     		void addCircleMess(int _x, int _y, int _r);
    		void setWidthMess(int _w);
    		void setColorMess(int _r,int _g, int _b);
		void clearMess();

		// drawing structs
		typedef struct circle_data
		{
		CvPoint center;
		int r;
		int width;
		CvScalar color;
		};

		// circles to draw
		circle_data circles[nb_max_of_circles];
		int nb_of_circles;
    		int width,color_r,color_g,color_b;  // current width and color

		// to detect changes in the image size
		int 		comp_xsize;
		int		comp_ysize;

		//////////
    		// Do the processing
		virtual void 	processRGBAImage(imageStruct &image);
    		virtual void 	processRGBImage(imageStruct &image);
		virtual void 	processYUVImage(imageStruct &image);
    		virtual void 	processGrayImage(imageStruct &image); 



	private:
		//////////
	    	// Static member functions
 
    		static void addCircleMessCallback(void *data,t_floatarg _x,t_floatarg _y,t_floatarg _r);  	// center.x,y,r 
		static void setWidthMessCallback(void *data, t_floatarg _w);
    	 	static void setColorMessCallback(void *data, t_floatarg _r,t_floatarg _g,t_floatarg _b);
		static void clearMessCallback(void *data);

		/////////
		// IplImage needed
    		IplImage *rgba, *rgb, *gray;
 
};



// end of #ifndef statement
#endif

