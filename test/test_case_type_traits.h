#include "catch.hpp"
#include "type_traits.h"



TEST_CASE("type_traits_is_integeral") {

    class A {};

    REQUIRE(!HxSTL::is_integeral<float>::value);
    REQUIRE(!HxSTL::is_integeral<A>::value);
    REQUIRE(HxSTL::is_integeral<bool>::value);
    REQUIRE(HxSTL::is_integeral<int>::value);
    REQUIRE(HxSTL::is_integeral<const int>::value);
    REQUIRE(HxSTL::is_integeral<unsigned int>::value);

}

TEST_CASE("type_traits_is_floating_point") {

    class A {};

    REQUIRE(HxSTL::is_floating_point<double>::value);
    REQUIRE(HxSTL::is_floating_point<long double>::value);
    REQUIRE(HxSTL::is_floating_point<const double>::value);
    REQUIRE(!HxSTL::is_floating_point<A>::value);
    REQUIRE(!HxSTL::is_floating_point<int>::value);

}

TEST_CASE("type_traits_is_array") {

    class A {};

    REQUIRE(!HxSTL::is_array<int>::value);
    REQUIRE(HxSTL::is_array<int[]>::value);
    REQUIRE(HxSTL::is_array<int[5]>::value);
    REQUIRE(!HxSTL::is_array<A>::value);
    REQUIRE(HxSTL::is_array<A[]>::value);
    REQUIRE(HxSTL::is_array<A[5]>::value);

}

TEST_CASE("type_traits_is_enum") {

    class A {};
    enum E {};
    enum class Ec: int {};

    REQUIRE(!HxSTL::is_enum<A>::value);
    REQUIRE(HxSTL::is_enum<E>::value);
    REQUIRE(HxSTL::is_enum<Ec>::value);
    REQUIRE(!HxSTL::is_enum<int>::value);

}

TEST_CASE("type_traits_is_union") {

    struct A {};
    typedef union {
        int a;
        float b;
    } B;
    struct C {
        B d;
    };

    REQUIRE(!HxSTL::is_union<A>::value);
    REQUIRE(HxSTL::is_union<B>::value);
    REQUIRE(!HxSTL::is_union<C>::value);
    REQUIRE(!HxSTL::is_union<int>::value);

}

TEST_CASE("type_traits_is_class") {

    struct A {};
    class B {};
    enum class C {};
    enum D {};
    union E {};

    REQUIRE(HxSTL::is_class<A>::value);
    REQUIRE(HxSTL::is_class<B>::value);
    REQUIRE(!HxSTL::is_class<C>::value);
    REQUIRE(!HxSTL::is_class<D>::value);
    REQUIRE(!HxSTL::is_class<E>::value);
    REQUIRE(!HxSTL::is_class<int>::value);
    REQUIRE(HxSTL::is_class<const A>::value);

}

TEST_CASE("type_traits_is_function") {

    struct A {};
    extern int f();

    REQUIRE(!HxSTL::is_function<A>::value);
    REQUIRE(HxSTL::is_function<int(int)>::value);
    REQUIRE(HxSTL::is_function<int(int) const>::value);
    REQUIRE(HxSTL::is_function<decltype(f)>::value);
    REQUIRE(!HxSTL::is_function<int>::value);

}

TEST_CASE("type_traits_is_pointer") {

    class A {};

    REQUIRE(!HxSTL::is_pointer<A>::value);
    REQUIRE(HxSTL::is_pointer<A*>::value);
    REQUIRE(!HxSTL::is_pointer<A&>::value);
    REQUIRE(!HxSTL::is_pointer<int>::value);
    REQUIRE(HxSTL::is_pointer<int*>::value);
    REQUIRE(HxSTL::is_pointer<int**>::value);
    REQUIRE(!HxSTL::is_pointer<int[10]>::value);
    REQUIRE(!HxSTL::is_pointer<HxSTL::nullptr_t>::value);

}

TEST_CASE("type_traits_is_lvalue_reference") {

    class A {};

    REQUIRE(!HxSTL::is_lvalue_reference<A>::value);
    REQUIRE(HxSTL::is_lvalue_reference<A&>::value);
    REQUIRE(!HxSTL::is_lvalue_reference<A&&>::value);
    REQUIRE(!HxSTL::is_lvalue_reference<int>::value);
    REQUIRE(HxSTL::is_lvalue_reference<int&>::value);
    REQUIRE(HxSTL::is_lvalue_reference<const int&>::value);
    REQUIRE(!HxSTL::is_lvalue_reference<int&&>::value);

}

TEST_CASE("type_traits_is_rvalue_reference") {

    class A {};

    REQUIRE(!HxSTL::is_rvalue_reference<A>::value);
    REQUIRE(!HxSTL::is_rvalue_reference<A&>::value);
    REQUIRE(HxSTL::is_rvalue_reference<A&&>::value);
    REQUIRE(!HxSTL::is_rvalue_reference<int>::value);
    REQUIRE(!HxSTL::is_rvalue_reference<int&>::value);
    REQUIRE(HxSTL::is_rvalue_reference<int&&>::value);
    REQUIRE(HxSTL::is_rvalue_reference<const int&&>::value);

}

