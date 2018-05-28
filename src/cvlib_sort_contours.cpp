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

#include "cvlib_sort_contours.h"

bool compare_contours_weights(weighted_contour contour_A, weighted_contour contour_B )
{

    return contour_A.weight > contour_B.weight;
}

CPPEXTERN_NEW_WITH_ONE_ARG(cvlib_sort_contours,t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// cvlib_sort_contours
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

cvlib_sort_contours::cvlib_sort_contours(t_float n)
{
	// First inlet for cvContours messages is already created within CPPExtern class

	//verbose(-1,"nb max=%d",(int)n);
	// we add a right inlet for matching lists
	inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("list"), gensym("pattern"));

	m_cvContoursOutlet = outlet_new(this->x_obj, 0);    // contours output
	
	output_cvSeqSID= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);
	
	// init flags
	
	contour_weight_vector_size=0;
	contour_processed=0;        // flag for resetting routing vector
	
	nb_of_sorted_items=CVLIB_MAX_CONTOURS;
	if (n>0 && n<CVLIB_MAX_CONTOURS)
		nb_of_sorted_items=n;
	
	// create storage
	storage1 = cvCreateMemStorage(0);
}

// DESTRUCTOR
cvlib_sort_contours::~cvlib_sort_contours()
    {

        cvlib_freeHeader(output_cvSeqSID);


     // free storage
        cvReleaseMemStorage(&storage1);
    }

/// TO DO : add method for ramdom access

void cvlib_sort_contours::processList(int argc,t_atom *argv)
{
    if (argc==0)   // bang : reset contour_weight vector
    {
        for (int k=0; k<CVLIB_MAX_CONTOURS;k++)
            contour_weight_vector[k]=0;
        contour_weight_vector_size=0;

    }
            //routing_vector_size+=argc;
    if (argc)
    {
        if (contour_processed)  // reset routing vector
        {
            contour_weight_vector_size=0;
            contour_processed=0;
        }
        if(argc+contour_weight_vector_size<CVLIB_MAX_CONTOURS)
        {
            for (int i=0;i<argc; i++)
            {
                if (argv[i].a_type == A_FLOAT)
                    contour_weight_vector[contour_weight_vector_size+i]=atom_getfloat(argv+i);
                else
                    {
                        verbose(-1,"cvSortContours error : incorrect vector element type");

                        break;
                    }
            }
            contour_weight_vector_size+=argc;
        }
        else
            verbose(-1,"cvSortContours error : max vector length overriden");



    }
 }


void cvlib_sort_contours::processCvContours(int sid)
{
	//post ("process contours");
	
	weighted_contour my_weighted_contour;
	if (cvlib_get_type(sid)==CVLIB_TYPE_CONTOUR)
	{
		current_contour=(CvSeq*)cvlib_get_pdata(sid);
		if(current_contour&&contour_weight_vector_size)
		
		{
		    int index=0;
		
		    while (current_contour)
			{
		                if (index< contour_weight_vector_size)
				{
					sorted_contours.push_back(my_weighted_contour);
					// copy values in new vector element
					sorted_contours[index].contour=(CvSeq*)cvCloneSeq(current_contour,storage1);
					sorted_contours[index].weight=contour_weight_vector[index];
					
					//   post ("%d: pushback values weight = %d pointer = %d", index,sorted_contours[index].weight,(int)sorted_contours[index].contour);			
					index++;
				}
				current_contour=current_contour->h_next;
			}
			
			// Sort contours
			std::sort(sorted_contours.begin(), sorted_contours.end(),compare_contours_weights);
			
			head_output_contour=sorted_contours[0].contour;
			head_output_contour->h_next=0;
			current_output_contour=head_output_contour;
			if (contour_weight_vector_size> 1)
			{
				//post("contour_weight_vector_size=%d",contour_weight_vector_size);
				int max_items=contour_weight_vector_size;
				if (max_items>nb_of_sorted_items)
					max_items=nb_of_sorted_items;
				for(int i=1; i<max_items; i++)
				{
					current_output_contour->h_next=sorted_contours[i].contour;
					current_output_contour=current_output_contour->h_next;
					// post ("i=%d weight=%d sorted pointer=%d",i,(int)sorted_contours.at(i).weight,(int)sorted_contours.at(i).contour);
				}
				// post("sortie boucle");
				current_output_contour->h_next=0;
			}
			/// output sorted contours chain
			cvlib_set_pdata(output_cvSeqSID,head_output_contour);
			// send cv contour SID
			SETFLOAT(&my_output,output_cvSeqSID);
			outlet_anything(m_cvContoursOutlet, gensym("cvContours"), 1, &my_output);
			
			// clear memory storage - reset free space position
			cvClearMemStorage(storage1);
			sorted_contours.clear();
		
		}
		contour_processed=1;
		contour_weight_vector_size=0;
	}


}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_sort_contours :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_sort_contours),
                   gensym("cvSortContours"),A_DEFFLOAT,A_NULL);  // with one argument


	// define help-file
	class_sethelpsymbol(cvlib_sort_contours_class,  gensym("cvSortContours"));

	class_addmethod(classPtr, (t_method)&cvlib_sort_contours::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_sort_contours::processListCallback,
	  		  gensym("pattern"),A_GIMME, A_NULL);	


}

void cvlib_sort_contours :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

void cvlib_sort_contours :: processListCallback(void *data, t_symbol*s, int argc,t_atom *argv)
{
	GetMyClass(data)->processList(argc, argv);
}





