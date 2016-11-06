#ifndef _UTILITY_H_
#define _UTILITY_H_


#include "pair.h"


namespace HxSTL {

    typedef decltype(nullptr) nullptr_t;

    template <class T>
    void swap(T& a, T& b) {
        T c(a);
        a = b;
        b = c;
    }

    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 x, T2 y) {
        return pair<T1, T2>(x, y);
    }

}


#endif
