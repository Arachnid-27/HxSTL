#include "catch.hpp"
#include "unique_ptr.h"

TEST_CASE("unique_ptr constructor") {

    SECTION("default constructor") {
        HxSTL::unique_ptr<int> up1;
        
        REQUIRE(!up1);
    }

    SECTION("constructor with nullptr") {
        HxSTL::unique_ptr<int> up1(nullptr);
        
        REQUIRE(!up1);
    }

    SECTION("constructor with pointer") {
        HxSTL::unique_ptr<int> up1(new int(100));

        REQUIRE(*up1 == 100);
    }

    SECTION("move constructor") {
        HxSTL::unique_ptr<int> up1(new int(100));
        HxSTL::unique_ptr<int> up2(move(up1));

        REQUIRE(!up1);
        REQUIRE(*up2 == 100);
    }

}

TEST_CASE("unique_ptr assignment") {

    SECTION("move assignment") {
        HxSTL::unique_ptr<int> up1(new int(100));
        HxSTL::unique_ptr<int> up2 = move(up1);

        REQUIRE(!up1);
        REQUIRE(*up2 == 100);
    }

    SECTION("assignment with nullptr") {
        HxSTL::unique_ptr<int> up1(new int);
        up1 = nullptr;

        REQUIRE(!up1);
    }

}

TEST_CASE("unique_ptr member operator") {

    SECTION("operator bool") {
        HxSTL::unique_ptr<int> up1(nullptr);
        HxSTL::unique_ptr<int> up2(new int);

        REQUIRE(!up1);
        REQUIRE((bool) up2);
    }

    SECTION("operator*") {
        HxSTL::unique_ptr<int> up1(new int(100));

        REQUIRE(*up1 == 100);
    }

    SECTION("operator->") {
    }

}

TEST_CASE("unique_ptr member function") {

    SECTION("release") {
        HxSTL::unique_ptr<int> up1(new int(100));

        REQUIRE(up1);

        int *p1 = up1.release();

        REQUIRE(*p1 == 100);
        REQUIRE(!up1);
    }

    SECTION("reset") {
        HxSTL::unique_ptr<int> up1(new int(100));
        up1.reset(new int(200));

        REQUIRE(*up1 == 200);
    }

    SECTION("swap") {
        HxSTL::unique_ptr<int> up1(new int(100));
        HxSTL::unique_ptr<int> up2(new int(200));

        REQUIRE(*up1 == 100);
        REQUIRE(*up2 == 200);

        up1.swap(up2);

        REQUIRE(*up1 == 200);
        REQUIRE(*up2 == 100);
    }

    SECTION("get") {
        int* p1 = new int;
        HxSTL::unique_ptr<int> up1(p1);

        REQUIRE(up1.get() == p1);
    }

    SECTION("get_deleter") {
    }

}
