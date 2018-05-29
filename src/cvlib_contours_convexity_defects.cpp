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


#include "cvlib_contours_convexity_defects.h"
CPPEXTERN_NEW_WITH_ONE_ARG(cvlib_contours_convexity_defects,t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// cvlib_contours_convexity_defects
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
cvlib_contours_convexity_defects :: cvlib_contours_convexity_defects(t_float max_defects)
{
	//first inlet for cvContour and other methods  already created by CPPExtern class


	sorted_defects.reserve(200);
	
	if (max_defects>=1)
		set_MaxConvexDefects((int)max_defects);
	else 
		set_MaxConvexDefects(200);
	
  	m_leftOutlet = outlet_new(this->x_obj, 0);    // left outlet for bang (done) and list (points) messages
	m_nbOfPointsOutlet = outlet_new(this->x_obj, 0);    // nb of convexity defects output
	
	
	margin=2;
	method=0;  // method used for computing convexity defects (default=0);

	stor01 = cvCreateMemStorage(0);
	stor02 = cvCreateMemStorage(0);
	stor03 = cvCreateMemStorage(0);
	stor04 = cvCreateMemStorage(0);
	stor05 = cvCreateMemStorage(0);

	defects=0;
	contours=0;
	convex_seq=0;
	approx_result=0;
	seqhull=0;
	
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
cvlib_contours_convexity_defects :: ~cvlib_contours_convexity_defects()
{
	//Destroy cv_images to clean memory
	cvReleaseMemStorage( &stor01 );
	cvReleaseMemStorage( &stor02 );
	cvReleaseMemStorage( &stor03 );
	cvReleaseMemStorage( &stor04 );
	cvReleaseMemStorage( &stor05 );
}

bool compare_depth_distance( convexity_point point_A, convexity_point point_B )
{
	return point_A.depth > point_B.depth;
}



/////////////////////////////////////////////////////////
// process cvContours
//
/////////////////////////////////////////////////////////


void cvlib_contours_convexity_defects::processCvContours(int i)
{
	convexity_point my_convex_point;
	int nbofsegments=0;

	// clear mem storage
	if(contours!=0)
	{
		    contours=0;
		    cvClearMemStorage(stor01);
        }
	if(approx_result!=0)
	{
		    approx_result=0;
		    cvClearMemStorage(stor02);
        }

	if(defects!=0)
	{
		    defects=0;
		    cvClearMemStorage(stor03);
        }

	if(convex_seq!=0)
	{
		    convex_seq=0;
		    cvClearMemStorage(stor04);
	}
	if(seqhull!=0)
	{
		    seqhull=0;
		    cvClearMemStorage(stor05);
	}
	
	if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
	{
		contours=(CvSeq*)cvlib_get_pdata(i);
		
		int  count=0;
		
		while (contours)
		{
			count= contours->total;
			
			// Alloc memory for contour point set.
			PointArray = (CvPoint*)malloc(count * sizeof(CvPoint) );
			
			// Alloc memory for indices of convex hull vertices.
			hull = (int*)malloc(sizeof(int) * count);
			
			// Get contour point set.
			cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ);
			
			// Find convex hull for curent contour.
			cvConvexHull( PointArray,count,NULL,CV_COUNTER_CLOCKWISE,hull,&hullsize);
			
			// Find convex hull for current contour.
			// This required for cvConvexityDefects().
			seqhull = cvConvexHull2(contours,stor05,CV_COUNTER_CLOCKWISE,0);
			
			//   post("hullsize=%d, seqhull->total=%d",hullsize, seqhull->total);/// the same
			
			//    ConvexPointArray = (CvPoint*)malloc(sizeof(CvPoint)*hullsize);
			
			// creating cvseq contour from convex hull for further purposes (cvPointPolygonTest)
			CvSeqWriter writer;
			cvStartWriteSeq( CV_SEQ_ELTYPE_POINT|CV_SEQ_KIND_CURVE|CV_SEQ_FLAG_CLOSED, sizeof(CvSeq), sizeof(CvPoint), stor04, &writer );
			for( int ii=0;ii<hullsize; ii++ )
			{
			CV_WRITE_SEQ_ELEM( PointArray[hull[ii]], writer );
			}
			convex_seq = cvEndWriteSeq( &writer );
			// post("hullsize=%d", hullsize);
			
			if (method==0)
			{
				//   post("method==0");
				if( hullsize >= 4 )   ///  >=3 ???
				    // This required for cvConvexityDefects().
				    // Otherwise cvConvexityDefects() falled.
				{
					// Find defects of convexity of current contours.
					defects = cvConvexityDefects(contours,seqhull,stor03);
					
					int number_of_defects = defects->total; // defect amount
					//   post("defects->total=%d",defects->total);
					
					// Alloc memory for defect set.
					defectArray = (CvConvexityDefect*)malloc(sizeof(CvConvexityDefect) * number_of_defects);
					
					if(number_of_defects > 0)
					{
						// convert defect set to Array, in order to do polygontest.
						cvCvtSeqToArray(defects,defectArray, CV_WHOLE_SEQ);
						int matched_defects=0;
						//          post ("nb of defects=%d",number_of_defects);
						for(i=0; i<number_of_defects; i++)
						{
							// do polygon test to  see if depth_point is not part of contour
							depth_point32F.x=defectArray[i].depth_point->x;
							depth_point32F.y=defectArray[i].depth_point->y;
							depth_distance= cvPointPolygonTest(convex_seq,depth_point32F,1);
							if(depth_distance>margin)
							{
								matched_defects++;
								depth_distance=sqrt((float)((defectArray[i].start->x-defectArray[i].depth_point->x)*(defectArray[i].start->x-defectArray[i].depth_point->x)
								    +
								    (defectArray[i].start->y-defectArray[i].depth_point->y)*(defectArray[i].start->y-defectArray[i].depth_point->y)
								    +
								    (defectArray[i].end->x-defectArray[i].depth_point->x)*(defectArray[i].end->x-defectArray[i].depth_point->x)
								    +
								    (defectArray[i].end->y-defectArray[i].depth_point->y)*(defectArray[i].end->y-defectArray[i].depth_point->y)));
								my_convex_point.x=depth_point32F.x;
								my_convex_point.y=depth_point32F.y;
								my_convex_point.depth=depth_distance;
								
								sorted_defects.push_back(my_convex_point);
								//       post ("index=%d x=%d y=%d distance=%d"  ,matched_defects-1
								  //                                  ,sorted_defects[matched_defects-1].x
								    //                                ,sorted_defects[matched_defects-1].y
								      //                              ,(int)sorted_defects[matched_defects-1].depth);
								//
							}
			
						}	//end for
			
						// Sort defectArray
						std::sort( sorted_defects.begin(), sorted_defects.end(),compare_depth_distance);
					
						//int j=min(min(max_convex_defects,number_of_defects),matched_defects);
						int _min=(max_convex_defects>number_of_defects ?  number_of_defects : max_convex_defects);
						int j=(_min<matched_defects ? _min:matched_defects);
						

						// output number of convexity defects
						outlet_float(m_nbOfPointsOutlet,j);

						// output sorted convexity defects
						for(i=0; i<j; i++)
						{
							SETFLOAT(&rlist[0], i);
							SETFLOAT(&rlist[1], sorted_defects[i].x);
							SETFLOAT(&rlist[2], sorted_defects[i].y);
							SETFLOAT(&rlist[3], sorted_defects[i].depth);
							outlet_list(m_leftOutlet,&s_list, 4, rlist);
						}
					}	// end of if(number_of_defects > 0)
			
					
					// Free memory.
					free(defectArray);
					sorted_defects.clear();
				}   // end of if( hullsize >= 4 )
			}   // end of method_0
			else
			{
			//method_1
				if( hullsize >= 4 )   ///  >=3 ???
				// This required for cvConvexityDefects().
				// Otherwise cvConvexityDefects() falled.
				{
					// how many points of contour do not belong to convex hull.
					int number_of_defects = count-hullsize;
					// Alloc memory for defect set.
					DefectPointArray = (CvPoint*)malloc(sizeof(CvPoint) * number_of_defects);
					
					// Alloc memory for HullPointArray
					// (we need to know for each point if it belongs or not to convexhull)
					HullPointArray=(int*)malloc(sizeof(int)* count);
					// post("nb of points=%d nb of defects=%d hullsize=%d",count,number_of_defects , hullsize);
					if(number_of_defects > 0)
					{
						// reset HullPointArray
						for (int k=0;k<count;k++)
						{
						    HullPointArray[k]=0;
						}
						// mark points that belong to convexhull
						for(int hull_index=0; hull_index<hullsize;hull_index++)
						{
							HullPointArray[hull[hull_index]]=1;
						}
						// retrieve points that do not belong to convexhull
						int defect_index=0;
						for(int k=0;k<count;k++)
						{
							if(!HullPointArray[k])
							{
								//post("DefectPointArray[%d]=PointArray[%d]",defect_index,k);
								DefectPointArray[defect_index]=PointArray[k];
								defect_index++;
							}
						}
						
						int matched_defects=0;
						for(i=0; i<number_of_defects; i++)
						{
							// do polygon test to  see if depth_point is not too close from contour
							depth_point32F.x=DefectPointArray[i].x;
							depth_point32F.y=DefectPointArray[i].y;
							depth_distance= cvPointPolygonTest(convex_seq,depth_point32F,1);
							if(depth_distance>margin)
							{
								matched_defects++;
								my_convex_point.x=depth_point32F.x;
								my_convex_point.y=depth_point32F.y;
								my_convex_point.depth=depth_distance;
								
								sorted_defects.push_back(my_convex_point);
							}
						
						}
						
						// Sort defectArray
						std::sort( sorted_defects.begin(), sorted_defects.end(),compare_depth_distance);
						int _min=(max_convex_defects>number_of_defects ?  number_of_defects : max_convex_defects);
						int j=(_min<matched_defects ? _min:matched_defects);

						// output number of convexity defects
						outlet_float(m_nbOfPointsOutlet, j);

						// output sorted convexity defects
						for(i=0; i<j; i++)
						{
							SETFLOAT(&rlist[0], i);
							SETFLOAT(&rlist[1], sorted_defects[i].x);
							SETFLOAT(&rlist[2], sorted_defects[i].y);
							SETFLOAT(&rlist[3], sorted_defects[i].depth);
							outlet_list(m_leftOutlet,&s_list, 4, rlist);
						}
					}
				
				// Free memory.
				free(DefectPointArray);
				free(HullPointArray);
				//    sorted_defects.clear();
		
			
				}
			} // end of method_1
			
			// Free memory.
			free(PointArray);
			free(hull);
			
			// go to next contour
			contours = contours->h_next;
		}
	}
}



void cvlib_contours_convexity_defects :: set_Margin (float f)
{
  margin=f;
}
void cvlib_contours_convexity_defects :: set_Method (int i)
{
  method=(i==1);
}
void cvlib_contours_convexity_defects ::set_MaxConvexDefects (int i)
{
  max_convex_defects=i;
}


////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_contours_convexity_defects :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_contours_convexity_defects),
                   gensym("cvContoursConvexityDefects"),
                   A_DEFFLOAT,A_NULL);  // with one arguments

	// define help-file
	class_sethelpsymbol(cvlib_contours_convexity_defects_class,  gensym("cvContoursConvexityDefects"));


	class_addmethod(classPtr, (t_method)&cvlib_contours_convexity_defects::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	


	class_addmethod(classPtr, (t_method)&cvlib_contours_convexity_defects::set_MethodCallback,
  		  gensym("method"), A_DEFFLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_contours_convexity_defects::set_MarginCallback,
  		  gensym("margin"), A_DEFFLOAT, A_NULL);	


	class_addmethod(classPtr, (t_method)&cvlib_contours_convexity_defects::set_MaxConvexDefectsCallback,
  		  gensym("maxpoints"), A_DEFFLOAT, A_NULL);	

}

void cvlib_contours_convexity_defects :: set_MaxConvexDefectsCallback(void *data, t_floatarg max_size)
{
	GetMyClass(data)->set_MaxConvexDefects((int)max_size);
}


void cvlib_contours_convexity_defects :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

void cvlib_contours_convexity_defects :: set_MarginCallback(void *data, t_floatarg _margin)
{
	GetMyClass(data)->set_Margin(_margin);
}

void cvlib_contours_convexity_defects :: set_MethodCallback(void *data, t_floatarg _method)
{
	GetMyClass(data)->set_Method((int)_method);
}
