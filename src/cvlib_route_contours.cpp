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

#include "cvlib_route_contours.h"
CPPEXTERN_NEW_WITH_GIMME(cvlib_route_contours);

/////////////////////////////////////////////////////////
//
// cvlib_route_contours
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

// CONSTRUCTOR
cvlib_route_contours::cvlib_route_contours(int argc,t_atom *argv)
{

        parameters_count=0;
	cvSeqSID_right=-1;

 	// argument extraction

	cloneInputContours=CVROUTE_CLONE_INPUT_CONTOURS;  // prevoir un argument -flag pour désactiver cette option 

        int ok=0;
        if((argc >= 1)&&(argc <= CVROUTE_MAX_NB_OF_OUTLETS))
        {
            ok=1;
            parameters_count=argc;
            for(int i= 0; i<parameters_count;i++)
            {
                if  (argv[i].a_type != A_FLOAT)
                {
                    ok=0;
                    break;
                }
                else
                    matching_values[i]=atom_getint(argv+i);

            }
        }
        if (!ok)
	{
		 //Signal a construction problem
		parameters_count=0;
		verbose(-1, "error: routing values must be integers");
		return;
	}        
    	
        else
        {
		// First inlet for cvContours messages is already created within CPPExtern class
            	
		// we add a right inlet for matching lists
		inlet_new(this->x_obj, &this->x_obj->ob_pd,gensym("list"), gensym("pattern"));

		      // what about matrix messages ?

            for(int i=0;i<(parameters_count+1);i++)     // create as much outlets as (arguments+1)
            { 						// the last outlet is for unmatched contours
		// add an outlet for cvContours output
		m_routingOutlet[i] = outlet_new(this->x_obj, 0);   
                cvSeqSID[i]= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);
            }
           
        //    m_noMatchOutlet = outlet_new(this->x_obj, 0); 

  
             // init flags
            routing_vector_size=0;
            contour_processed=0;        // flag for resetting routing vector
		// init routing vector
	      for (int k=0; k<CVLIB_MAX_CONTOURS;k++)   
            		routing_vector[k]=0;		

            // create storage
            storage1 = cvCreateMemStorage(0);

            // create contour sid for rejected contours
            cvSeqSID_right= cvlib_createHeader(CVLIB_TYPE_CONTOUR,CVLIB_STATUS_EXTERNAL_POINTER);

	
        
    }
}

/////////////////////////////
// DESTRUCTOR
cvlib_route_contours::~cvlib_route_contours()
{
        cvlib_freeHeader(cvSeqSID_right);

        if (parameters_count)   // check if initialization succeeded
	{
		for(int i=0;i<parameters_count;i++)
		    cvlib_freeHeader(cvSeqSID[i]);

     		// free storage
        	cvReleaseMemStorage(&storage1);
	}
}


void cvlib_route_contours::processList(int argc,t_atom *argv)
{
    if (argc==0)   // bang : reset routing vector
    {
        for (int k=0; k<routing_vector_size;k++)    /////  TO DO : Iterer seulement jusqu'au 
		routing_vector[k]=0;   		////// (routing_vector_size-1) ième élément
        routing_vector_size=0;

    }
            //routing_vector_size+=argc;

    if (argc)
    {
        if (contour_processed)  // reset routing vector if contours processed, else append to it
        {
            routing_vector_size=0;
            contour_processed=0;     

		// TO DO : remplacxer routine_vector_size par genre routing_vector_start_index 
        }

        if(argc+routing_vector_size<CVLIB_MAX_CONTOURS)
        {
            for (int i=0;i<argc; i++)
            {
                if (argv[i].a_type == A_FLOAT)
                    routing_vector[routing_vector_size+i]=atom_getint(argv+i);
                else
                    {
                        verbose(-1,"cvRouteContours error : incorrect vector element type");

                        break;
                    }
            }
            routing_vector_size+=argc;
        }
        else
            verbose(-1,"cvRouteContours error : max vector length overriden");



    }
 }




void cvlib_route_contours::sendCvContourSidToOutlet(int outlet_index, int sid)
{
    if (sid>=0)
        {
		SETFLOAT(&my_output,sid);
		outlet_anything(m_routingOutlet[outlet_index],gensym("cvContours"),1,&my_output);   
        }
}



void cvlib_route_contours::processCvContours(int sid)
{

    if (cvlib_get_type(sid)==CVLIB_TYPE_CONTOUR)
    {
        current_contour=(CvSeq*)cvlib_get_pdata(sid);

        // init values
        for(int i=0;i<parameters_count;i++)
        {
            contour_head[i]=0;
            contour_current[i]=0;
        }
        head_contour_right=0;
        current_contour_right=0;

        int index=0;
        while (current_contour)
        {
            int matched=-1;
            //look if routing vector element matches with one of the arguments
            if (index< routing_vector_size)
            {
                for(int k=0;k<parameters_count;k++)
                {
                    if(routing_vector[index]==matching_values[k])
                    {
                        matched=k;
                        break;
                    }
                }
            }
            if(matched==-1)
            // push contour into rightmost sequence
            {

                if(head_contour_right==0)
		{
			if(cloneInputContours)
				current_contour_right=head_contour_right=(CvSeq*)cvCloneSeq(current_contour,storage1);
			else
				current_contour_right=head_contour_right=current_contour; 

			// go to next contour
			current_contour=current_contour->h_next;
		}  
             else
                {
			if(cloneInputContours)
				current_contour_right->h_next=(CvSeq*)cvCloneSeq(current_contour,storage1);
			else
				current_contour_right->h_next=current_contour;   

			current_contour_right=current_contour_right->h_next;

			// go to next contour
			current_contour=current_contour->h_next;

			
                }
            }
            else
                // push contour into  sequence [matched]
            {
                if(contour_head[matched]==0)
		{
			if(cloneInputContours)
				contour_current[matched]=contour_head[matched]=(CvSeq*)cvCloneSeq(current_contour,storage1);
			else
				contour_current[matched]=contour_head[matched]=current_contour;
 
            		// go to next contour
			current_contour=current_contour->h_next;
		}
                else
                {
			if(cloneInputContours)
				contour_current[matched]->h_next=(CvSeq*)cvCloneSeq(current_contour,storage1);
			else
				contour_current[matched]->h_next=current_contour;

			contour_current[matched]=contour_current[matched]->h_next;

			// go to next contour
			current_contour=current_contour->h_next;
                }
            }
        
            index++;
        }
            // close properly cvsequences
            for(int k=0;k<parameters_count;k++)
            {
                if(contour_current[k]!=0)
                contour_current[k]->h_next=0;
            }
            // close properly rightmost cvsequence
            if(current_contour_right!=0)
                current_contour_right->h_next=0;

            // set instance pointer to contours
            for(int k=0;k<parameters_count;k++)
            {
                cvlib_set_pdata(cvSeqSID[k],contour_head[k]);

            }
            cvlib_set_pdata(cvSeqSID_right,head_contour_right);

            // send cv contour SIDs
            /// rajout test if non vide
            if (head_contour_right!=0)
                sendCvContourSidToOutlet(parameters_count,cvSeqSID_right);

            for(int k=parameters_count-1;k>=0;k--)
            {
                if (contour_head[k]!=0)
			{
				sendCvContourSidToOutlet(k,cvSeqSID[k]);
			}
                    

            // clear memory storage - reset free space position
            cvClearMemStorage(storage1);
        }
    contour_processed=1;
	

    }

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void cvlib_route_contours :: obj_setupCallback(t_class *classPtr)
{
	// create an alias
	class_addcreator(reinterpret_cast<t_newmethod>(create_cvlib_route_contours),
                   gensym("cvRouteContours"),A_GIMME);  // with variable arguments number


	// define help-file
	class_sethelpsymbol(cvlib_route_contours_class,  gensym("cvRouteContours"));

	class_addmethod(classPtr, (t_method)&cvlib_route_contours::processCvContoursCallback,
	  		  gensym("cvContours"), A_DEFFLOAT, A_NULL);	

	class_addmethod(classPtr, (t_method)&cvlib_route_contours::processListCallback,
	  		  gensym("pattern"),A_GIMME, A_NULL);	


}

void cvlib_route_contours :: processCvContoursCallback(void *data, t_floatarg _c)
{
	GetMyClass(data)->processCvContours((int)_c);
}

void cvlib_route_contours :: processListCallback(void *data, t_symbol*s, int argc,t_atom *argv)
{
	GetMyClass(data)->processList(argc, argv);
}


