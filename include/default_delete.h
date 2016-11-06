#ifndef _DEFAULT_DELETE_
#define _DEFAULT_DELETE_


namespace HxSTL {
    
    template <class T>
    struct default_delete {
        default_delete() = default;

        void operator()(T* ptr) const { delete ptr; }
    };

    template <class T>
    struct default_delete<T[]> {
        default_delete() = default;

        void operator()(T* ptr) const { delete[] ptr; }
    };

}


#endif
