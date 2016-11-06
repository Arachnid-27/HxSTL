#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


namespace HxSTL {

    struct true_type {};
    struct false_type {};

    template <class T>
    struct is_pod {
        typedef false_type      type;
    };

    template <>
    struct is_pod<char> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<signed char> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<unsigned char> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<short> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<unsigned short> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<int> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<unsigned int> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<long> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<unsigned long> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<float> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<double> {
        typedef true_type       type;
    };

    template <>
    struct is_pod<long double> {
        typedef true_type       type;
    };

    template <class T>
    struct is_pod<T*> {
        typedef true_type       type;
    };

    template <class T>
    struct is_integer {
        typedef false_type      type;
    };

    template <>
    struct is_integer<char> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<signed char> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<unsigned char> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<short> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<unsigned short> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<int> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<unsigned int> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<long> {
        typedef true_type       type;
    };

    template <>
    struct is_integer<unsigned long> {
        typedef true_type       type;
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
