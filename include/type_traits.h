#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


namespace HxSTL {

    struct true_type {};
    struct false_type {};

    template <class T>
    struct is_pod {
        typedef false_type      value;
    };

    template <>
    struct is_pod<char> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<signed char> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<unsigned char> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<short> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<unsigned short> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<int> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<unsigned int> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<long> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<unsigned long> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<float> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<double> {
        typedef true_type       value;
    };

    template <>
    struct is_pod<long double> {
        typedef true_type       value;
    };

    template <class T>
    struct is_pod<T *> {
        typedef true_type       value;
    };

}


#endif
