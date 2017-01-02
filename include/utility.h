#ifndef _UTILITY_H_
#define _UTILITY_H_


#include "type_traits.h"
#include <initializer_list>


namespace HxSTL {

    template <class T>
    using initializer_list = std::initializer_list<T>;

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

    template <class T>
    T&& forward(typename remove_reference<T>::type& t) {
        return static_cast<T&&>(t);
    }

    template <class T>
    T&& forward(typename remove_reference<T>::type&& t) {
        return static_cast<T&&>(t);
    }

    template <class T1, class T2>
    struct pair {
        typedef T1      first_type;
        typedef T2      second_type;

        T1 first;
        T2 second;

        pair(): first(T1()), second(T2()) {}

        pair(const T1& x, const T2& y): first(x), second(y) {}

        template <class U1, class U2>
        pair(U1&& x, U2&& y): first(HxSTL::forward<U1>(x)), second(HxSTL::forward<U2>(y)) {}

        pair(const pair& other): first(other.first), second(other.second) {}

        template <class U1, class U2>
        pair(const pair<U1, U2>& other): first(other.first), second(other.second) {}

        template <class U1, class U2>
        pair(pair<U1, U2>&& other): first(HxSTL::forward<U1>(other.first)), second(HxSTL::forward<U2>(other.second)) {}

        pair& operator=(const pair& other) = default;

        pair& operator=(pair&& other) = default;
    };

    template <class T1, class T2>
    bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) { 
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }

    template <class T1, class T2>
    bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs == rhs); 
    }

    template <class T1, class T2>
    bool operator< (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) { 
        return lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second);
    }

    template <class T1, class T2>
    bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(rhs < lhs);
    }

    template <class T1, class T2>
    bool operator> (const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return rhs < lhs;
    }

    template <class T1, class T2>
    bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
        return !(lhs < rhs);
    }

    template <class T>
    using __decay_t = typename HxSTL::decay<T>::type;

    template <class T1, class T2>
    pair<__decay_t<T1>, __decay_t<T2>> make_pair(T1&& x, T2&& y) {
        return pair<__decay_t<T1>, __decay_t<T2>>(HxSTL::forward<T1>(x), HxSTL::forward<T2>(y));
    }

}


#endif
