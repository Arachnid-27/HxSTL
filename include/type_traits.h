#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_


#include <cstddef>


namespace HxSTL {

    typedef decltype(nullptr) nullptr_t;

    typedef char __one;
    typedef struct { char c[2]; } __two;

    template <class T, T v>
    struct integeral_constant {
        static constexpr T value = v;
        typedef integeral_constant<T, v> type;
    };

    typedef integeral_constant<bool, true>      true_type;
    typedef integeral_constant<bool, false>     false_type;

    template <bool Cond, class T, class F>
    struct conditional {
        typedef F       type;
    };

    template <class T, class F>
    struct conditional<true, T, F> {
        typedef T       type;
    };

    template <class T>
    struct remove_const {
        typedef T       type;
    };

    template <class T>
    struct remove_const<const T> {
        typedef T       type;
    };

    template <class T>
    struct remove_volatile {
        typedef T       type;
    };

    template <class T>
    struct remove_volatile<volatile T> {
        typedef T       type;
    };

    template <class T>
    struct remove_cv {
        typedef typename remove_volatile<typename remove_const<T>::type>::type      type;
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

    template <class T>
    struct remove_all_extents {
        typedef T       type;
    };

    template <class T>
    struct remove_all_extents<T[]> {
        typedef typename remove_all_extents<T>::type        type;
    };

    template <class T, size_t N>
    struct remove_all_extents<T[N]> {
        typedef typename remove_all_extents<T>::type        type;
    };

    template <class T, class U>
    struct is_same: public false_type {};

    template <class T>
    struct is_same<T, T>: public true_type {};

    template <class T>
    struct is_pod: public false_type {};

    template <>
    struct is_pod<char>: public true_type {};

    template <>
    struct is_pod<signed char>: public true_type {};

    template <>
    struct is_pod<unsigned char>: public true_type {};

    template <>
    struct is_pod<short>: public true_type {}; 

    template <>
    struct is_pod<unsigned short>: public true_type {};

    template <>
    struct is_pod<int>: public true_type {};

    template <>
    struct is_pod<unsigned int>: public true_type {};

    template <>
    struct is_pod<long>: public true_type {};

    template <>
    struct is_pod<unsigned long>: public true_type {};

    template <>
    struct is_pod<float>: public true_type {};

    template <>
    struct is_pod<double>: public true_type {};

    template <>
    struct is_pod<long double>: public true_type {};

    template <class T>
    struct is_pod<T*>: public true_type {};

    template <class T>
    struct is_function: public false_type {};

    template <class R, class... Args>
    struct is_function<R(Args...)>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......)>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) volatile>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) volatile>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const volatile>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const volatile>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) volatile &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) volatile &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) volatile &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) volatile &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const volatile &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const volatile &>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args...) const volatile &&>: public true_type {};

    template <class R, class... Args>
    struct is_function<R(Args......) const volatile &&>: public true_type {};

    template <class T>
    struct is_integeral: 
        public integeral_constant<bool, 
        is_same<bool, typename remove_cv<T>::type>::value || 
        is_same<char, typename remove_cv<T>::type>::value || 
        is_same<signed char, typename remove_cv<T>::type>::value || 
        is_same<unsigned char, typename remove_cv<T>::type>::value || 
        is_same<char16_t, typename remove_cv<T>::type>::value || 
        is_same<char32_t, typename remove_cv<T>::type>::value || 
        is_same<wchar_t, typename remove_cv<T>::type>::value || 
        is_same<short, typename remove_cv<T>::type>::value || 
        is_same<signed short, typename remove_cv<T>::type>::value || 
        is_same<unsigned short, typename remove_cv<T>::type>::value || 
        is_same<int, typename remove_cv<T>::type>::value || 
        is_same<signed int, typename remove_cv<T>::type>::value || 
        is_same<unsigned int, typename remove_cv<T>::type>::value || 
        is_same<long, typename remove_cv<T>::type>::value || 
        is_same<signed long, typename remove_cv<T>::type>::value || 
        is_same<unsigned long, typename remove_cv<T>::type>::value ||
        is_same<long long, typename remove_cv<T>::type>::value || 
        is_same<signed long long, typename remove_cv<T>::type>::value || 
        is_same<unsigned long long, typename remove_cv<T>::type>::value> {};  

    template <class T>
    struct is_floating_point: 
        public integeral_constant<bool, 
        is_same<float, typename remove_cv<T>::type>::value || 
        is_same<double, typename remove_cv<T>::type>::value || 
        is_same<long double, typename remove_cv<T>::type>::value> {};

    template <class T>
    struct is_arithmetic: 
        public integeral_constant<bool, 
        is_integeral<T>::value || 
        is_floating_point<T>::value> {};

    template <class T>
    struct is_enum: public integeral_constant<bool, __is_enum(T)> {};

    template <class T>
    struct is_pointer: public false_type {};

    template <class T>
    struct is_pointer<T*>: public true_type {};

    template <class T>
    struct is_member_pointer: public false_type {};

    template <class T, class U>
    struct is_member_pointer<T U::*>: public true_type {};

    template <class T>
    struct is_null_pointer: 
        public integeral_constant<bool, 
        is_same<nullptr_t, typename remove_cv<T>::type>::value> {};

    template <class T>
    struct is_scalar: 
        public integeral_constant<bool, 
        is_arithmetic<T>::value || 
        is_enum<T>::value || 
        is_pointer<T>::value || 
        is_member_pointer<T>::value || 
        is_null_pointer<T>::value> {};

    template <class T>
    struct is_array: public false_type {};

    template <class T>
    struct is_array<T[]>: public true_type {};

    template <class T, size_t N>
    struct is_array<T[N]>: public true_type {};

    template <class T>
    struct is_union: public integeral_constant<bool, __is_union(T)> {};

    template <class T>
    static __one __is_class_test(int T::*);

    template <class>
    static __two __is_class_test(...);

    template <class T>
    struct is_class: 
        public integeral_constant<bool, 
        sizeof(__is_class_test<T>(0)) == sizeof(__one) && 
        !is_union<T>::value> {};

    template <class T>
    struct is_object: 
        public integeral_constant<bool, 
        is_scalar<T>::value || 
        is_array<T>::value || 
        is_union<T>::value || 
        is_class<T>::value> {};

    template <class T>
    struct is_lvalue_reference: public false_type {};

    template <class T>
    struct is_lvalue_reference<T&>: public true_type {};

    template <class T>
    struct is_rvalue_reference: public false_type {};

    template <class T>
    struct is_rvalue_reference<T&&>: public true_type {};

    template <class T>
    struct is_reference: 
        public integeral_constant<bool, 
        is_lvalue_reference<T>::value || 
        is_rvalue_reference<T>::value> {};

    template <class T>
    struct __is_no_qualified_function: public false_type {};

    template <class R, class... Args>
    struct __is_no_qualified_function<R(Args...)>: public true_type {};

    template <class R, class... Args>
    struct __is_no_qualified_function<R(Args......)>: public true_type {};

    // 为什么要是 no-qualified function
    template <class T>
    struct __is_referenable: 
        public integeral_constant<bool, 
        __is_no_qualified_function<T>::value || 
        is_object<T>::value> {};

    template <class T, bool = __is_referenable<T>::value>
    struct add_lvalue_reference {
        typedef T       type;
    };

    template <class T>
    struct add_lvalue_reference<T, true> {
        typedef T&      type;
    };

    template <class T, bool = __is_referenable<T>::value>
    struct add_rvalue_reference {
        typedef T       type;
    };

    template <class T>
    struct add_rvalue_reference<T, true> {
        typedef T&&     type;
    };

    // add_lvalue_reference 可行吗
    template <class T>
    typename add_rvalue_reference<T>::type declval();

    template <class T, class U, class = decltype(declval<T>() = declval<U>())>
    static __one __test_is_assignable(int);

    template <class T, class U>
    static __two __test_is_assignable(...);

    template <class T, class U>
    struct is_assignable: 
        public integeral_constant<bool, 
        sizeof(__test_is_assignable<T, U>) == sizeof(__one)> {};

    template <class T, class U>
    struct is_trivially_assignable: 
        public integeral_constant<bool, 
        is_assignable<T, U>::value && 
        __is_trivially_assignable(T, U)> {};

    template <class T>
    struct is_copy_assignable: 
        public integeral_constant<bool, 
        !__is_referenable<T>::value && 
        is_assignable<T&, const T&>::value> {};

    template <class T>
    struct is_trivially_copy_assignable: 
        public integeral_constant<bool, 
        !__is_referenable<T>::value && 
        is_trivially_assignable<T&, const T&>::value> {};

    template <class T>
    struct is_move_assignable: 
        public integeral_constant<bool, 
        !__is_referenable<T>::value && 
        is_assignable<T&, T&&>::value> {};
    
    template <class T>
    struct is_trivially_move_assignable: 
        public integeral_constant<bool, 
        !__is_referenable<T>::value && 
        is_trivially_assignable<T&, T&&>::value> {};

    template <class T, class = decltype(declval<T&>().~T())>
    static __one __test_is_destructible(int);

    template <class T>
    static __two __test_is_destructible(...);

    template <class T>
    struct is_destructible: 
        public integeral_constant<bool, 
        !is_same<void, T>::value && 
        !is_function<T>::value && 
        (is_reference<T>::value || 
        sizeof(__test_is_destructible<typename remove_all_extents<T>::type>(0)) == sizeof(__one))> {}; 

    template <class T>
    struct is_trivially_destructible: 
        public integeral_constant<bool, 
        is_destructible<T>::value && 
        __has_trivial_destructor(T)> {};

}


#endif
