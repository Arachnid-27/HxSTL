#define CATCH_CONFIG_MAIN
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

    REQUIRE(!HxSTL::is_function<A>::value);
    REQUIRE(HxSTL::is_function<int(int)>::value);
    REQUIRE(HxSTL::is_function<int(int) const>::value);
    REQUIRE(!HxSTL::is_function<int(*)()>::value);
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

TEST_CASE("type_traits_is_arithmetic") {

    class A {};

    REQUIRE(!HxSTL::is_arithmetic<A>::value);
    REQUIRE(HxSTL::is_arithmetic<int>::value);
    REQUIRE(HxSTL::is_arithmetic<const int>::value);
    REQUIRE(!HxSTL::is_arithmetic<int*>::value);
    REQUIRE(!HxSTL::is_arithmetic<int&>::value);
    REQUIRE(HxSTL::is_arithmetic<float>::value);
    REQUIRE(HxSTL::is_arithmetic<const float>::value);
    REQUIRE(!HxSTL::is_arithmetic<float*>::value);
    REQUIRE(!HxSTL::is_arithmetic<float&>::value);

}

TEST_CASE("type_traits_is_scalar") {
    
    class A {};
    enum E {};

    REQUIRE(!HxSTL::is_scalar<A>::value);
    REQUIRE(HxSTL::is_scalar<int>::value);
    REQUIRE(HxSTL::is_scalar<const int>::value);
    REQUIRE(HxSTL::is_scalar<int*>::value);
    REQUIRE(!HxSTL::is_scalar<int&>::value);
    REQUIRE(HxSTL::is_scalar<E>::value);
    REQUIRE(!HxSTL::is_scalar<int()>::value);
    REQUIRE(HxSTL::is_scalar<int(*)()>::value);
    REQUIRE(HxSTL::is_scalar<int A::*>::value);
    REQUIRE(HxSTL::is_scalar<HxSTL::nullptr_t>::value);

}

TEST_CASE("type_traits_is_object") {

    class A {};
    union U {};

    REQUIRE(HxSTL::is_object<A>::value);
    REQUIRE(HxSTL::is_object<U>::value);
    REQUIRE(HxSTL::is_object<int>::value);
    REQUIRE(HxSTL::is_object<int*>::value);
    REQUIRE(HxSTL::is_object<int[]>::value);
    REQUIRE(!HxSTL::is_object<int&>::value);
    REQUIRE(!HxSTL::is_object<int&&>::value);

}

TEST_CASE("type_traits_is_reference") {

    REQUIRE(!HxSTL::is_reference<int>::value);
    REQUIRE(HxSTL::is_reference<int&>::value);
    REQUIRE(HxSTL::is_reference<int&&>::value);

}

TEST_CASE("type_traits_is_member_pointer") {

    class A {};

    REQUIRE(HxSTL::is_member_pointer<int A::*>::value);
    REQUIRE(!HxSTL::is_member_pointer<int(*)()>::value);

}

TEST_CASE("type_traits_is_assignable") {

    class A {};
    class B {
    public:
        B& operator=(const A&);
    };

    REQUIRE(!(HxSTL::is_assignable<int, int>::value));
    REQUIRE((HxSTL::is_assignable<int&, int>::value));
    REQUIRE((HxSTL::is_assignable<int&, double>::value));
    // 不太懂
    REQUIRE((HxSTL::is_assignable<B, B>::value));
    REQUIRE((HxSTL::is_assignable<B, A>::value));

}

TEST_CASE("type_traits_is_trivially_assignable") {

    class A {};
    class B {
    public:
        B& operator=(const B&);
    };

    REQUIRE((HxSTL::is_trivially_assignable<A, A>::value));
    REQUIRE(!(HxSTL::is_trivially_assignable<B, B>::value));

}

TEST_CASE("type_traits_is_copy_assignable") {

    class A {};

    REQUIRE(HxSTL::is_copy_assignable<A>::value);
    REQUIRE(HxSTL::is_copy_assignable<int>::value);
    REQUIRE(!HxSTL::is_copy_assignable<int[]>::value);

}

TEST_CASE("type_traits_is_trivially_copy_assignable") {

    class A {};
    class B {
    public:
        B& operator=(const B&);
    };

    REQUIRE(HxSTL::is_trivially_copy_assignable<A>::value);
    REQUIRE(HxSTL::is_trivially_copy_assignable<int>::value);
    REQUIRE(!HxSTL::is_trivially_copy_assignable<B>::value);
    REQUIRE(!HxSTL::is_trivially_copy_assignable<int[]>::value);

}

TEST_CASE("type_traits_is_move_assignable") {

    class A {};
    class B {
    public:
        B& operator=(const B&);
    };
    class C {
    public:
        C& operator=(C&&);
    };

    REQUIRE(HxSTL::is_move_assignable<A>::value);
    REQUIRE(HxSTL::is_move_assignable<int>::value);
    REQUIRE(HxSTL::is_move_assignable<B>::value);
    REQUIRE(HxSTL::is_move_assignable<C>::value);
    REQUIRE(!HxSTL::is_move_assignable<int[]>::value);

}

TEST_CASE("type_traits_is_trivially_move_assignable") {

    class A {};
    class B {
    public:
        B& operator=(const B&);
    };
    class C {
    public:
        C& operator=(C&&);
    };

    REQUIRE(HxSTL::is_trivially_move_assignable<A>::value);
    REQUIRE(HxSTL::is_trivially_move_assignable<int>::value);
    REQUIRE(!HxSTL::is_trivially_move_assignable<B>::value);
    REQUIRE(!HxSTL::is_trivially_move_assignable<C>::value);
    REQUIRE(!HxSTL::is_trivially_move_assignable<int[]>::value);

}

TEST_CASE("type_traits_is_destructible") {

    class A {};
    class B {
    public:
        ~B() = delete;
    };

    REQUIRE(HxSTL::is_destructible<int>::value);
    REQUIRE(HxSTL::is_destructible<A>::value);
    REQUIRE(!HxSTL::is_destructible<B>::value);
    REQUIRE(!HxSTL::is_destructible<void>::value);
    REQUIRE(!HxSTL::is_destructible<int()>::value);

}

TEST_CASE("type_traits_is_trivially_destructible") {

    class A {};
    class B {
    public:
        ~B() = delete;
    };
    class C {
    public:
        ~C();
    };

    REQUIRE(HxSTL::is_trivially_destructible<int>::value);
    REQUIRE(HxSTL::is_trivially_destructible<A>::value);
    REQUIRE(!HxSTL::is_trivially_destructible<B>::value);
    REQUIRE(!HxSTL::is_trivially_destructible<C>::value);
    REQUIRE(!HxSTL::is_trivially_destructible<void>::value);
    REQUIRE(!HxSTL::is_trivially_destructible<int()>::value);

}
