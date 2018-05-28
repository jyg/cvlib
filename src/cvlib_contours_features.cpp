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

#include "cvlib_contours_features.h"
CPPEXTERN_NEW_WITH_GIMME(cvlib_contours_features);

/////////////////////////////////////////////////////////
//
// cvlib_contours_features
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

// CONSTRUCTOR
cvlib_contours_features::cvlib_contours_features(int argc,t_atom *argv)
{
	t_symbol * parameter;
	if (argc<=MAX_NB_OF_CONTOUR_FEATURES)       
		nb_of_features=argc;
	else
		nb_of_features=MAX_NB_OF_CONTOUR_FEATURES;

	if (nb_of_features==0)
        {
		verbose(-1,"cvContoursFeatures : please specify at least one feature : Area BoundingRect IsHole HuMoments LogHuMoments IsConvex MinAreaRect Perimeter SegmentCount");
		return;
        }

        //////////////////////    INLETS & OUTLETS 

	// cvContours inlet already created by CPPExtern class
	
	// add an outlet for cvContours output
//	m_cvContoursOutlet = outlet_new(this->x_obj, 0);
   
        // add outlets for every method invoqued as arguments
        // make each pointer in array point to corresponding feature extraction functions
        for (int i=0;i<nb_of_features;i++)
        {
		// add an outlet for cvContours output
		m_featureOutlet[i] = outlet_new(this->x_obj, 0);   
           
		funcArr[i]=&cvlib_contours_features::get_Nothing;
		if (argv[i].a_type == A_SYMBOL)
		{
			parameter=atom_getsymbol(argv+i);
			
			if (parameter==gensym("Area"))
			{
			    funcArr[i]=&cvlib_contours_features::get_Area;
			    continue;
			}
			if (parameter==gensym("BoundingRect"))
			{
			    funcArr[i]=&cvlib_contours_features::get_BoundingRect;
			    continue;
			}
		//	if (parameter==gensym("DominantPoints"))
		//	{
  		//	    post ("outlet %d:DominantPoints",i+1);
		//          funcArr[i]=&cvlib_contours_features::get_MinAreaRect;
		//	    continue;
		//	}
			if (parameter==gensym("IsHole"))
			{
			    funcArr[i]=&cvlib_contours_features::get_IsHole;
			    continue;
			}
			if (parameter==gensym("HuMoments"))
			{
			    funcArr[i]=&cvlib_contours_features::get_HuMoments;
			    continue;
			}
			if (parameter==gensym("LogHuMoments"))
			{
			   funcArr[i]=&cvlib_contours_features::get_LogHuMoments;
			    continue;
			}
			if (parameter==gensym("IsConvex"))
			{
			    funcArr[i]=&cvlib_contours_features::get_IsConvex;
			    continue;
			}
			if (parameter==gensym("MinAreaRect"))
			{
			   funcArr[i]=&cvlib_contours_features::get_MinAreaRect;
			    continue;
			}
			if (parameter==gensym("Perimeter"))
			{
			    funcArr[i]=&cvlib_contours_features::get_Perimeter;
			    continue;
			}
			if (parameter==gensym("SegmentCount"))
			{
			    funcArr[i]=&cvlib_contours_features::get_SegmentCount;
			    continue;
			}
			char buf[30];
			atom_string(argv+i, buf, 30);
			verbose(-1,"error: %s is not a feature",buf);
			return;
		}
		else
		{
			verbose(-1,"error: incorrect parameter type (float)");
			return;
		}
	}
}

/////////////////////////////
// DESTRUCTOR
cvlib_contours_features::~cvlib_contours_features()
{
}

///////////////////////////////////////////////////////
void cvlib_contours_features::sendCvContoursSID()
{
	if (cvSeqSID>=0)
	{
		SETFLOAT(&my_output,cvSeqSID);
		//outlet_anything(m_cvContoursOutlet,gensym("cvContours"),1,&my_output);
	}
}



/////////////////////////////////////////////////////////
// processContours
//
/////////////////////////////////////////////////////////
void cvlib_contours_features::processCvContours(int i)
{
	cvSeqSID=-1;
	if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
	{
		cvSeqSID=i;
		contours=(CvSeq*)cvlib_get_pdata(i);
		int  count=0;
		while (contours)
		{
			for(int k=nb_of_features-1;k>=0;k--)
			{
				// appel du pointeur de fonction
				(*this.*funcArr[k])(contours,k);
			}
			
			count++;
			contours = contours->h_next;
			if (count > CVLIB_MAX_CONTOURS)
				continue;
		}
		// send cv contour SID
		sendCvContoursSID();

    }
}


///////////////////////////////////////////////////////////////
// features extraction functions
///////////////////////////////////////////////////////////////
void cvlib_contours_features::get_Area(CvSeq* contour,int outlet_index)
{
	outlet_float(m_featureOutlet[outlet_index], fabs(cvContourArea( contour, CV_WHOLE_SEQ)) );
}
void cvlib_contours_features::get_IsHole(CvSeq* contour,int outlet_index)
{
	outlet_float(m_featureOutlet[outlet_index], CV_IS_SEQ_HOLE( contour));
}
void cvlib_contours_features::get_BoundingRect(CvSeq* contour,int outlet_index)
{
	rect = cvContourBoundingRect(contour, 1);
	SETFLOAT(&result[0],rect.x);
	SETFLOAT(&result[1],rect.y);
	SETFLOAT(&result[2],rect.width);
	SETFLOAT(&result[3],rect.height);
	outlet_list(m_featureOutlet[outlet_index],&s_list,4, result);
}


void cvlib_contours_features::get_HuMoments(CvSeq* contour,int outlet_index)
{
	cvContourMoments(contours,&moments);
	cvGetHuMoments(&moments,&HuMoments);

	/// extract moments
	moment_value[0]=HuMoments.hu1;
	moment_value[1]=HuMoments.hu2;
	moment_value[2]=HuMoments.hu3;
	moment_value[3]=HuMoments.hu4;
	moment_value[4]=HuMoments.hu5;
	moment_value[5]=HuMoments.hu6;
	moment_value[6]=HuMoments.hu7;
	for (int k=0;k<7;k++)
		SETFLOAT(&result[k],moment_value[k]);
	outlet_list(m_featureOutlet[outlet_index],&s_list,7, result);
}

void cvlib_contours_features::get_LogHuMoments(CvSeq* contour,int outlet_index)
{
	float temp;
	cvContourMoments(contours,&moments);
	cvGetHuMoments(&moments,&HuMoments);
	moment_value[0]=HuMoments.hu1;
	moment_value[1]=HuMoments.hu2;
	moment_value[2]=HuMoments.hu3;
	moment_value[3]=HuMoments.hu4;
	moment_value[4]=HuMoments.hu5;
	moment_value[5]=HuMoments.hu6;
	moment_value[6]=HuMoments.hu7;

// convert into log values
	for (int k=0;k<7;k++)
	{
	    temp=moment_value[k];
	    moment_value[k]=((2*(temp>0)-1))*log(abs(temp));
	    SETFLOAT(&result[k],moment_value[k]);
	}
	outlet_list(m_featureOutlet[outlet_index],&s_list,7, result);
}

void cvlib_contours_features::get_IsConvex(CvSeq* contour,int outlet_index)
{
	if(cvCheckContourConvexity(contour))
		outlet_float(m_featureOutlet[outlet_index],1);
	else
		outlet_float(m_featureOutlet[outlet_index],0);
}

void cvlib_contours_features::get_MinAreaRect(CvSeq* contour,int outlet_index)
{
	min_area_rect = cvMinAreaRect2( contour, 0);
	SETFLOAT(&result[0],min_area_rect.center.x);
	SETFLOAT(&result[1],min_area_rect.center.y);
	SETFLOAT(&result[2],min_area_rect.size.width);
	SETFLOAT(&result[3],min_area_rect.size.height);
	SETFLOAT(&result[4],min_area_rect.angle);
	outlet_list(m_featureOutlet[outlet_index],&s_list,5, result);
}

void cvlib_contours_features::get_Nothing(CvSeq* contour,int outlet_index)
{
	// blank function
}

void cvlib_contours_features::get_Perimeter(CvSeq* contour,int outlet_index)
{
	outlet_float(m_featureOutlet[outlet_index], cvArcLength( contour, CV_WHOLE_SEQ, 1 ));
}

void cvlib_contours_features::get_SegmentCount(CvSeq* contour,int outlet_index)
{
	outlet_float(m_featureOutlet[outlet_index],contour->total);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_contours_features :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_contours_features),
                   gensym("cvContoursFeatures"),A_GIMME);  // with variable arguments number

	// create an alias (old name)
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_contours_features),
                   gensym("cvContoursFeature"),A_GIMME);  // with variable arguments number



	// define help-file
	class_sethelpsymbol(cvlib_contours_features_class,  gensym("cvContoursFeatures"));

	class_addmethod(classPtr, (t_method)&cvlib_contours_features::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	


}

void cvlib_contours_features :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}


