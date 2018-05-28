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

#include "cvlib_find_contours.h"

CPPEXTERN_NEW(cvlib_find_contours)

/////////////////////////////////////////////////////////
//
// cvlib_find_contours
//
/////////////////////////////////////////////////////////

// Constructor
//
/////////////////////////////////////////////////////////


// CONSTRUCTOR
cvlib_find_contours::cvlib_find_contours()
    {
       	rgba=NULL;
	gray=NULL;
	rgb=NULL;

        // create storage
        storage = cvCreateMemStorage(0);
        // create contour
        cvSeqSID= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);

        // set drawing mode  (better : call set_drawing_status)
        draw_mode=0;
	m_cvContoursOutlet = outlet_new(this->x_obj, 0);    // contours output
	m_nbOfContoursOutlet = outlet_new(this->x_obj, 0);    // nb of contours output
    }

// DESTRUCTOR
cvlib_find_contours::~cvlib_find_contours()
    {
        cvlib_freeHeader(cvSeqSID);

    // free storage
        cvReleaseMemStorage(&storage);
    }

void cvlib_find_contours::setDrawingStatusMess(int i)
{
    draw_mode=i;
//// TO DO : set dirty flag to reprocess image
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////

void cvlib_find_contours::processRGBAImage(imageStruct &image)

{
		post( "cvlib_find_contours : RGBA format not supported" ); 	
}

void cvlib_find_contours :: processRGBImage(imageStruct &image)
{
  post( "cvlib_find_contours : RGB format not supported" );
}

void cvlib_find_contours :: processYUVImage(imageStruct &image)
{
  post( "cvlib_find_contours : yuv format not supported" );
}

void cvlib_find_contours :: processGrayImage(imageStruct &image)
{
	int foundContours=0;
	unsigned char *pixels = image.data;  
	if ((this->comp_xsize!=image.xsize)||(this->comp_ysize!=image.ysize)||(!gray)) 
	{
		this->comp_xsize = image.xsize;
		this->comp_ysize = image.ysize;

	    	//Destroy cv_images to clean memory
	        if ( gray )
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
	 // reset contours/memstorage if necessary
	if(contours!=0)
        {
		contours=0;
		cvClearMemStorage(storage);
        }
	
	memcpy( gray->imageData, image.data, image.xsize*image.ysize );
 			//      gray->imageData = (char*) image.data;

	// find contours and store them all as a list
	cvFindContours( gray, storage, &contours, sizeof(CvContour),
	        CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );

	
	{
		
		current_contour=contours;	
		while (current_contour)
		{
			foundContours++;
			if(draw_mode) 
				cvDrawContours( gray, current_contour, CV_RGB(255,255,255), CV_RGB(255,0,255), 0, -1, 8 );
			current_contour=current_contour->h_next;
		}
	}
	// set instance pointer to contours
	cvlib_set_pdata(cvSeqSID,contours);
	
// send nb of found contours to rightmost outlet
	
	outlet_float(m_nbOfContoursOutlet,(float)foundContours);

// send cv contour SID
	SETFLOAT(&my_output,cvSeqSID);
	outlet_anything(m_cvContoursOutlet, gensym("cvContours"), 1, &my_output);

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_find_contours :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_find_contours),
                   gensym("cvFindContours"),
                   A_NULL);  // with no argument

	// define help-file
	class_sethelpsymbol(cvlib_find_contours_class,  gensym("cvfindContours"));

//	class_addmethod(classPtr, (t_method)&cvlib_show_image::floatShowHideMessCallback,
//  		  gensym("vis"), A_FLOAT, A_NULL);	
	
//	cvlib_show_image_instance_counter=0;

// init cvlib
	cvlib_reset();
}


/////////////////////////////////////////////////////////////////////////
//////////////// cvlib globals //////////////////////////////////////////

// global variables





// the array that stores type/status flags and pointers to cvDataStructures



cvlib_pool_struct cvlib_pool[CVLIB_POOL_SIZE];

// index pointing to next free position in array cvlib_pool

int cvlib_blank_index;



// counter for remaining free positions in array

int cvlib_remaining_blank_indexes;


// global method called during cvlib setup
void cvlib_reset()
{
 	//   verbose(-1,"Appel de cvlib_reset()");
	cvlib_remaining_blank_indexes=CVLIB_POOL_SIZE;
	cvlib_blank_index=0;
	int i =0;
	while (i< CVLIB_POOL_SIZE)
	{
		cvlib_pool[i].type = CVLIB_TYPE_NULL;
		cvlib_pool[i].status = CVLIB_STATUS_NULL;
		cvlib_pool[i].pInstance = 0;
		i++;
	}
}

int cvlib_find_next_blank()
{
    if (cvlib_remaining_blank_indexes == 0)
        return 0;   //  failed

        // search for next blank index
    int i=0;
    int found_blank_index = 0;
    while (i< CVLIB_POOL_SIZE)   // anti infinite loop mechanism
        {
            if (CVLIB_POOL_SIZE <= ++cvlib_blank_index)  // check next position
                cvlib_blank_index = 0;
            if (cvlib_pool[cvlib_blank_index].status == CVLIB_STATUS_NULL)  // check for status  (0 = blank)
                 {
                     found_blank_index = 1;
                     break;  // quit the loop
                 }
            i++;
        }
    return found_blank_index;
}

// get pointer to data
void* cvlib_get_pdata(int i)
{
    if ((i>=0) && (i<CVLIB_POOL_SIZE))
        return((void*)cvlib_pool[i].pInstance);
    else
        return 0;
}

// set pointer to data

int cvlib_set_pdata(int index, void* pointer)
{
    if ((index>=0)&&(index<CVLIB_POOL_SIZE))
    {
        if (cvlib_pool[index].status)
        {
            cvlib_pool[index].pInstance=pointer;
            return 1;
        }
    }
    else
        return 0;
}



// get status
int cvlib_get_status(int i)
{
    if ((i>=0) && (i<CVLIB_POOL_SIZE))
        return(cvlib_pool[i].status);
    else return CVLIB_STATUS_ERROR;
}




int cvlib_createHeader(int type,int status)
{

    if (cvlib_find_next_blank())
        {
            cvlib_pool[cvlib_blank_index].pInstance = 0;
            cvlib_pool[cvlib_blank_index].status = status;
            cvlib_pool[cvlib_blank_index].type = type;
            cvlib_remaining_blank_indexes--;
            ///verbose(-1,"cvlib_createHeader Success!");
            return cvlib_blank_index;
        }
        else return -1;
}

bool cvlib_isIplImage(int index)
    {
        if ((index>=0) && (index<CVLIB_POOL_SIZE))
           if (cvlib_pool[index].type == CVLIB_TYPE_IPLIMAGE)
                return (true);
        return (false);
    }

int cvlib_get_type(int index)
{
        if ((index>=0) && (index<CVLIB_POOL_SIZE))
           return (cvlib_pool[index].type);
        else
            return (-1);
    }
///  modify
int cvlib_freeHeader(int index)
{
    ///verbose(-1,"Appel de cvlib_freeHeader(%d)...", index);
    if ((index < 0) || (index>=CVLIB_POOL_SIZE))
        {
           verbose(-1,"index %d invalide", index);
        }
    else if (cvlib_pool[index].status != CVLIB_STATUS_NULL)
    {
        cvlib_pool[index].type = CVLIB_TYPE_NULL;
        cvlib_pool[index].status = CVLIB_STATUS_NULL;
        cvlib_pool[index].pInstance = NULL;
        cvlib_remaining_blank_indexes++;
        ///verbose(-1,"cvlib_freeHeader = success!");
        return 1;
    }
    else
    {
        ///verbose(-1,"cvlib_freeHeader  : position %d already free");
        return 0;
    }

}


