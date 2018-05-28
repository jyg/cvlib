#ifndef INCLUDE_CVLIB_FIND_CONTOURS__H_
#define INCLUDE_CVLIB_FIND_CONTOURS_H_


#ifndef _EiC
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#endif


#include "Base/GemPixObj.h"
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cvlib_find_contours
    
    cvFindContours implementation

KEYWORDS
    
    
DESCRIPTION
   
-----------------------------------------------------------------*/
class GEM_EXPORT cvlib_find_contours: public GemPixObj
{
	CPPEXTERN_HEADER(cvlib_find_contours,GemPixObj)

	public:

		//////////
	    	// Constructor
		cvlib_find_contours();


	protected:

		//////////
    		// Destructor
  		virtual ~cvlib_find_contours();

		//////////
    		// Set the drawing_status
		void setDrawingStatusMess(int value);

		//////////
    		// Do the processing
		virtual void 	processRGBAImage(imageStruct &image);
    		virtual void 	processRGBImage(imageStruct &image);
		virtual void 	processYUVImage(imageStruct &image);
    		virtual void 	processGrayImage(imageStruct &image); 


	private:
		struct  CvMemStorage *storage;
		CvSeq* contours;
		CvSeq* current_contour;
		CvSeq* result;
		int cvSeqSID;
		int draw_mode;
		t_atom my_output;

		t_outlet *m_cvContoursOutlet;
		t_outlet *m_nbOfContoursOutlet;
		// to detect changes in the image size
		int 		comp_xsize;
		int		comp_ysize;

		/////////
		// IplImage needed
    		IplImage *rgba, *rgb, *gray;
	//	IplImage * tempImg;
	//	IplImage * tempImg2;

	
};

/////////////////////////////////////
//////////////// cvlib globals

// defines

// size of cvlib_pool container
#define CVLIB_POOL_SIZE 1000
// define max number of contours
#define CVLIB_MAX_CONTOURS 10000

// status of objects
#define CVLIB_STATUS_NULL 0
#define CVLIB_STATUS_HEADER_AND_DATA 1
#define CVLIB_STATUS_DATA_NOT_OWNED 2
#define CVLIB_STATUS_EXTERNAL_POINTER 3
#define CVLIB_STATUS_ERROR 0   /// value to change....

// types of objects
#define CVLIB_TYPE_NULL 0
#define CVLIB_TYPE_IPLIMAGE 1
#define CVLIB_TYPE_CONTOUR 2

// return functions status
#define CVLIB_OK 1
#define CVLIB_FAILED 0
#define CVLIB_RESIZED 2

///////////////////////////////////// container struct for image and object pointers
typedef struct cvlib_pool_tag {
	int type;
	int status;
	void * pInstance;
} cvlib_pool_struct;


//////////////   service functions

// reset cvlib global variables
void cvlib_reset();

// check if current position points to an IplImage
//	bool cvlib_isIplImage(int index);

 // create new <anything> header
int cvlib_createHeader (int type,int status);

// free array position
int cvlib_freeHeader(int index);

//  get array position type
int cvlib_get_type(int index);

//  get array position status
int cvlib_get_status(int index);

//  get array position data pointer
void* cvlib_get_pdata(int index);

//  set data pointer at array[index] position
int cvlib_set_pdata(int index, void* pointer);


/// find next free position in array and set cvlib_blank_index;
int cvlib_find_next_blank();

//**************************************************************




#endif     //  for header file

