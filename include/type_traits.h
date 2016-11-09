#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


namespace HxSTL {

    template <class T, T v>
    struct integeral_constant {
        static constexpr T value = v;
        typedef integeral_constant<T, v> type;
    };

    typedef integeral_constant<bool, true>      true_type;
    typedef integeral_constant<bool, false>     false_type;

    template <class T>
    struct is_pod: public integeral_constant<bool, false> {};

    template <>
    struct is_pod<char>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<signed char>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<unsigned char>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<short>: public integeral_constant<bool, true> {}; 

    template <>
    struct is_pod<unsigned short>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<int>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<unsigned int>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<long>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<unsigned long>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<float>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<double>: public integeral_constant<bool, true> {};

    template <>
    struct is_pod<long double>: public integeral_constant<bool, true> {};

    template <class T>
    struct is_pod<T*>: public integeral_constant<bool, true> {};

    template <class T>
    struct is_integer: public integeral_constant<bool, false> {};

    template <>
    struct is_integer<char>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<signed char>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<unsigned char>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<short>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<unsigned short>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<int>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<unsigned int>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<long>: public integeral_constant<bool, true> {};

    template <>
    struct is_integer<unsigned long>: public integeral_constant<bool, true> {};

    template <class T, class U>
    struct is_same: public integeral_constant<bool, false> {};

    template <class T>
    struct is_same<T, T>: public integeral_constant<bool, true> {};

    template <class T>
    struct is_lvalue_reference: public integeral_constant<bool, false> {};

    template <class T>
    struct is_lvalue_reference<T&>: public integeral_constant<bool, true> {};

    template <class T>
    struct is_rvalue_reference: public integeral_constant<bool, false> {};

    template <class T>
    struct is_rvalue_reference<T&&>: public integeral_constant<bool, true> {};

    template <class T>
    struct is_reference: public integeral_constant<bool, 
            is_lvalue_reference<T>::value || is_rvalue_reference<T>::value> {};

    template <class T>
    struct remove_const {
        typedef T       type;
    };

    template <class T>
    struct remove_const<const T> {
        typedef T       type;
    };

    template <class T>
    struct remove_reference {
        typedef T       type;
    };

    template <class T>
    struct remove_reference<T&> {
        typedef T       type;
    };

    template <class T>
    struct remove_reference<T&&> {
        typedef T       type;
    };

    template <bool Cond, class T, class F>
    struct conditional {};

    template <class T, class F>
    struct conditional<true, T, F> {
        typedef T       type;
    };

    template <class T, class F>
    struct conditional<false, T, F> {
        typedef F       type;
    };

}


#endif
