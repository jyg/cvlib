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

#include "cvlib_contours_get_points.h"
CPPEXTERN_NEW_WITH_GIMME(cvlib_contours_get_points);


/////////////////////////////////////////////////////////
//
// cvlib_contours_get_points
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

cvlib_contours_get_points::cvlib_contours_get_points(int argc,t_atom *argv)
    {
        

	//first inlet for cvContour already created by CPPExtern class

		// second inlet for setting max number of points 
		//inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("float"), gensym("max_size"));
	
	m_leftOutlet = outlet_new(this->x_obj, 0);    // left outlet for bang (done) and list (points) messages
	m_nbOfPointsOutlet = outlet_new(this->x_obj, 0);    // nb of points output

        outputmode=0;  // give results as lists of points
	normalize_x=normalize_y=1.;  // disable normalization
	offset=0;

        max_nb_of_points=10;   // default value

        if (argc==1)
		if(argv[0].a_type == A_FLOAT)
		{
			max_nb_of_points=atom_getint(argv+0);
			if (max_nb_of_points<1)
				max_nb_of_points=1;

			if (max_nb_of_points>CVLIB_GET_MAX_POINTS)
				max_nb_of_points=CVLIB_GET_MAX_POINTS;
		}
        if (argc==3)
        {
		outputmode=1;// give results into pd-arrays
		
		//retrieve x_buffer
		if(argv[0].a_type == A_SYMBOL)
			bufname_x = atom_getsymbol(argv+0);
		
		//retrieve y_buffer
		if(argv[1].a_type == A_SYMBOL)
			bufname_y = atom_getsymbol(argv+1);
		
		// retrieve max nmb of points
		if(argv[2].a_type == A_FLOAT)
		{
			max_nb_of_points=atom_getint(argv+2);
			if (max_nb_of_points<0)
				max_nb_of_points=1;	// min value
			if (max_nb_of_points==0)
				max_nb_of_points=10;   // default value
			if (max_nb_of_points>CVLIB_GET_MAX_POINTS)
				max_nb_of_points=CVLIB_GET_MAX_POINTS;
		}
		else
			verbose(-1,"error: 3rd parameter must be a number") ;
        }

	// Alloc memory for contour point set.
	PointArray = (CvPoint*)malloc((max_nb_of_points+2) * sizeof(CvPoint) );
    }

// DESTRUCTOR
cvlib_contours_get_points::~cvlib_contours_get_points()
    {
                    // Free memory.
            free(PointArray);
    }




void cvlib_contours_get_points::setNbPointsMess(float _nb_points)
{
      	max_nb_of_points=(int)_nb_points;
	if (max_nb_of_points<1)
		max_nb_of_points=1;	// min value
	if (max_nb_of_points>CVLIB_GET_MAX_POINTS)
		max_nb_of_points=CVLIB_GET_MAX_POINTS;
 }

void cvlib_contours_get_points::setArraysMess(t_symbol* _bufname_x,t_symbol* _bufname_y)
{
	outputmode=1;		
	bufname_x = _bufname_x;
	bufname_y = _bufname_y;
}

void cvlib_contours_get_points::setTabOutputMess(int mode)
{
	outputmode=mode;
}

void cvlib_contours_get_points::setNormalizeMess(float width, float height)
{
	if(width==0)
		normalize_x=1.;
	else
		normalize_x=1./width;

	if(height==0)
		normalize_y=1.;
	else
		normalize_y=1./height;
}
		
void cvlib_contours_get_points::setOffsetMess(int _offset)
{
	if (_offset>0)
		offset=_offset;
	else
		offset=0;
}



/////////////////////////////////////////////////////////
// process cvContours
//
/////////////////////////////////////////////////////////

void cvlib_contours_get_points::processCvContours(int i)
{

    int contours_count=0;


   if (cvlib_get_type(i)==CVLIB_TYPE_CONTOUR)
    {
        contours=(CvSeq*)cvlib_get_pdata(i);

        while (contours)
        {
            contours_count++;
            int count=contours->total;
	    if (count>	max_nb_of_points)
		count = max_nb_of_points;

             // Get contour point set.
            cvCvtSeqToArray(contours, PointArray,cvSlice(0, count));

 
            if (outputmode==0)// output points of contour
            {
		// output number of points 
		outlet_float(m_nbOfPointsOutlet,count);
	
                for(i=0; i<count; i++)
		{
			SETFLOAT(&rlist[0], i);
			SETFLOAT(&rlist[1], PointArray[i].x  * normalize_x);
			SETFLOAT(&rlist[2], PointArray[i].y  * normalize_y);
			outlet_list(m_leftOutlet,&s_list,3, rlist);
                }
		//signals that current contour has been processed
		outlet_bang(m_leftOutlet);
            }
            else
                if (outputmode==1)  // write into tables
                {
			// check arrays names  (see d_array.c, lines 995...)

			if (!(a_x = (t_garray *)pd_findbyclass(bufname_x, garray_class)))
				pd_error(this, "%s: no such array", bufname_x->s_name);
			else if (!garray_getfloatwords(a_x, &vecsize_x, &vec_x))
				pd_error(this, "%s: bad template for tabwrite", bufname_x->s_name);
			else if (!(a_y = (t_garray *)pd_findbyclass(bufname_y, garray_class)))
				pd_error(this, "%s: no such array", bufname_y->s_name);
			else if (!garray_getfloatwords(a_y, &vecsize_y, &vec_y))
				pd_error(this, "%s: bad template for tabwrite", bufname_y->s_name);
			else

			{
				// check i against bounds vecsize_x   / vecsize_y
				int limit=count;
				if ((limit+offset) > vecsize_x)
					limit = vecsize_x-offset;
				if ((limit+offset) > vecsize_y)
					limit = vecsize_y-offset;

				if (limit >0)
				{	
					// output number of points 
					outlet_float(m_nbOfPointsOutlet,limit);
					for(i=0; i<limit; i++)
					{
					   vec_x[offset+i].w_float = PointArray[i].x * normalize_x;
					   vec_y[offset+i].w_float = PointArray[i].y * normalize_y;
					}
					//signals that current contour has been processed
					outlet_bang(m_leftOutlet);
				}
			}
                }
		
          // go to next contour
            contours = contours->h_next;
        }
    }
 }

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_contours_get_points :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_contours_get_points),
                   gensym("cvContoursPoints"),
                   A_GIMME);  // with variable arguments number

	// define help-file
	class_sethelpsymbol(cvlib_contours_get_points_class,  gensym("cvContoursPoints"));


	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	


	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::setNbPointsCallback,
  		  gensym("size"), A_FLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::setArraysMessCallback,
	  		  gensym("settab"), A_DEFSYMBOL, A_DEFSYMBOL, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::setTabOutputMessCallback,
  		  gensym("taboutput"), A_FLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::setNormalizeMessCallback,
	  		  gensym("normalize"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_contours_get_points::setOffsetMessCallback,
  		  gensym("offset"), A_FLOAT, A_NULL);	

	


}

void cvlib_contours_get_points :: setNbPointsCallback(void *data, t_floatarg max_size)
{
	GetMyClass(data)->setNbPointsMess((float)max_size);
}

void cvlib_contours_get_points :: setArraysMessCallback(void *data, t_symbol* _bufname_x,t_symbol* _bufname_y)
{
	GetMyClass(data)->setArraysMess(_bufname_x,_bufname_y);
}

void cvlib_contours_get_points :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

void cvlib_contours_get_points :: setTabOutputMessCallback(void *data, t_floatarg _mode)
{
	GetMyClass(data)->setTabOutputMess((int)_mode);
}

void cvlib_contours_get_points :: setNormalizeMessCallback(void *data, t_floatarg _w, t_floatarg _h)
{
	GetMyClass(data)->setNormalizeMess(_w, _h);
}

void cvlib_contours_get_points :: setOffsetMessCallback(void *data, t_floatarg _o)
{
	GetMyClass(data)->setOffsetMess((int)_o);
}
