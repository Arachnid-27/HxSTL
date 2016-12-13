#ifndef _UTILITY_H_
#define _UTILITY_H_


#include "pair.h"
#include "type_traits.h"
#include <initializer_list>


namespace HxSTL {

    template <class T>
    typename remove_reference<T>::type&& move(T&& t) {
        return static_cast<typename remove_reference<T>::type&&>(t);
    }

    template <class T>
    void swap(T& a, T& b) {
        T c(move(a));
        a = move(b);
        b = move(c);
    }

    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 x, T2 y) {
        return pair<T1, T2>(x, y);
    }

    template <class T>
    T&& forward(typename remove_reference<T>::type& t) {
        return static_cast<T&&>(t);
    }

    template <class T>
    T&& forward(typename remove_reference<T>::type&& t) {
        return static_cast<T&&>(t);
    }

    template <class T>
    using initializer_list = std::initializer_list<T>;

}


#endif
