#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "functional.h"

auto func1 = [](int i) { return i; };

float func2() {
    return 100.00f;
}

struct func3 {
    bool operator()(long f) { return f % 2 == 0; }
};

int func4(int i) {
    return i / 2;
}

TEST_CASE("function construtor") {

    SECTION("default constructor") {
        auto f1 = HxSTL::function<void()>();
    }

    SECTION("constructor with nullptr") {
        auto f1 = HxSTL::function<void()>(nullptr);
    }

    SECTION("copy constructor") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2(f1);

        REQUIRE(f1(100) == 100);
        REQUIRE(f2(100) == 100);
    }

    SECTION("move constructor") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>(move(f1));

        REQUIRE(!f1);
        REQUIRE(f2(100) == 100);
    }
    
    SECTION("constructor with func") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<float()>(func2);
        auto f3 = HxSTL::function<bool(long)>(func3());

        REQUIRE(f1(100) == 100);
        REQUIRE(f2() == 100.00f);
        REQUIRE(!f3(99l));
        REQUIRE(f3(100l));
    }

}

TEST_CASE("function assignment") {

    SECTION("copy assignment") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>();
        f2 = f1;

        REQUIRE(f1(100) == 100);
        REQUIRE(f2(100) == 100);
    }

    SECTION("move assignment") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>();
        f2 = HxSTL::move(f1);

        REQUIRE(f1 == nullptr);
        REQUIRE(f2(100) == 100);
    }

    SECTION("copy assignment with nullptr") {
        auto f1 = HxSTL::function<int(int)>(func1);

        REQUIRE(f1 != nullptr);

        f1 = nullptr;

        REQUIRE(f1 == nullptr);
    }

}

TEST_CASE("member operator") {

    SECTION("operator bool") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>();

        REQUIRE(f1);
        REQUIRE(!f2);
    }

    SECTION("operator()") {
        // same to "constructor with func"
    }

}

TEST_CASE("member function") {

    SECTION("swap") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>(func4);

        REQUIRE(f1(100) == 100);
        REQUIRE(f2(100) == 50);

        f1.swap(f2);

        REQUIRE(f1(100) == 50);
        REQUIRE(f2(100) == 100);
    }

    SECTION("target_type") {
        auto f1 = HxSTL::function<int(int)>(func1);
        auto f2 = HxSTL::function<int(int)>();

        REQUIRE(f1.target_type() == typeid(func1));
        REQUIRE(f2.target_type() == typeid(void));
    }

    SECTION("target") {
        auto f1 = HxSTL::function<bool(long)>(func3());

        REQUIRE((*f1.target<func3>())(100l));
        REQUIRE(f1.target<void()>() == nullptr);
    }

}

