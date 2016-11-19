#ifndef _STDEXCEPT_H_
#define _STDEXCEPT_H_


#include "exception.h"


namespace HxSTL {

    // Todo 完成 basci_string 后再实现

    class logic_error: public exception {};

    class invalid_argument: public logic_error {};

    class domain_error: public logic_error {};

    class length_error: public logic_error {};

    class out_of_range: public logic_error {};

    class runtime_error: public exception {};

    class range_error: public runtime_error {};

    class overflow_error: public runtime_error {};

    class underflow_error: public runtime_error {};

    class bad_exception: public exception {};

}


#endif
