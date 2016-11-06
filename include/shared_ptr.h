#ifndef _SHARED_PTR_
#define _SHARED_PTR_


#include "type_traits.h"
#include "default_delete.h"


namespace HxSTL {

    template <class T>
    class shared_ptr {
    public:
        typedef T           element_type;
    protected:
        element_type* _ptr;

    };

}


#endif
