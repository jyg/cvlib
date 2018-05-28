#ifndef INCLUDE_CVLIB_DRAW_CONTOURS_H_
#define INCLUDE_CVLIB_DRAW_CONTOURS_H_


#ifndef _EiC
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#endif

#include "Base/GemPixObj.h"
#include "cvlib_find_contours.h"    // for definition of cvContours methods

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_draw_contours
    
    previsualization window object

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/


class GEM_EXPORT cvlib_draw_contours: public GemPixObj
{
	CPPEXTERN_HEADER(cvlib_draw_contours,GemPixObj)
	
	public:

		//////////
	    	// Constructor

		cvlib_draw_contours();

	protected:
		//////////
    		// DESTRUCTOR
		virtual ~cvlib_draw_contours();

 		void setCvContoursMess(int i);

		void setColorMess(int _r, int _g, int _b);
		void setInnerColorMess(int _r, int _g, int _b);
		void setOuterColorMess(int _r, int _g, int _b);
		void setWidthMess(int i);
		int iterations;
		int thickness;
		CvScalar outer_contour_color;
		CvScalar inner_contour_color;
		CvSeq* contours_to_draw;

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

   		static void setColorMessCallback(void *data,t_floatarg _r,t_floatarg _g,t_floatarg _b);  	// center.x,y,r 
  		static void setInnerColorMessCallback(void *data,t_floatarg _r,t_floatarg _g,t_floatarg _b);  	// center.x,y,r 
  		static void setOuterColorMessCallback(void *data,t_floatarg _r,t_floatarg _g,t_floatarg _b);  	// center.x,y,r 
		static void setWidthMessCallback(void *data, t_floatarg _w);
 		static void setCvContoursMessCallback(void *data, t_floatarg _sid);

		/////////
		// IplImage needed
    		IplImage *rgba, *rgb, *gray;


};




#endif   // end of #ifndef statement

