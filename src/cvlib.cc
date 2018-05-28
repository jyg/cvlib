#include "m_pd.h"

extern "C" {

typedef struct cvlib
{
  t_object x_ob;
} t_cvlib;

t_class *cvlib_class;

    /* this is called when a new "cvlib" object is created. */
void *cvlib_new(void)
{
    t_cvlib *x = (t_cvlib *)pd_new(cvlib_class);
    return (void *)x;
}

void cvlib_setup(void)
{
    verbose(-1,"cvlib %s library by Jean-Yves Gratius 2012-2018",VERSION);
    verbose(-1,"\tbased on work by Yves Degoyon, Lluis Gomez i Bigorda and Antoine Villeret");
    verbose(-1,"\tbuilt on %s",__DATE__);

    cvlib_class = class_new(gensym("cvlib"), (t_newmethod)cvlib_new, 0,
    	sizeof(t_cvlib), 0, (t_atomtype) 0);
}

} // extern "C"
