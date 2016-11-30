#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "list.h"

TEST_CASE("list_default_constructor") {

    HxSTL::list<int> l1;
    const HxSTL::list<int> l2;

    REQUIRE(l1.size() == 0);
    REQUIRE(l1.empty());
    REQUIRE(l2.size() == 0);
    REQUIRE(l2.empty());

}

TEST_CASE("list_fill_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(10);

    REQUIRE(l1.size() == 10);
    REQUIRE(l2.size() == 10);

    for (auto it = l1.begin(); it != l1.end(); ++it) {
        REQUIRE(*it == 10);
    }

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        REQUIRE(*it == 0);
    }

}

TEST_CASE("list_range_constructor") {

    int a1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; 
    HxSTL::list<int> l1(a1, a1 + 10);

    REQUIRE(l1.size() == 10);

    auto it = l1.begin();
    for (int i = 0; i != 10; ++i, ++it) {
        REQUIRE(a1[i] == *it);
    }

}

TEST_CASE("list_copy_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(l1);

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

}

TEST_CASE("list_move_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(HxSTL::move(l1));

    REQUIRE(l2.size() == 10);

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        REQUIRE(*it == 10);
    }

}

TEST_CASE("list_copy_assignment") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(5, 5);

    l2 = l1;

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

    l2 = l2;

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

}

TEST_CASE("list_move_assignment") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(5, 5);

    l2 = HxSTL::move(l1);

    REQUIRE(l2.size() == 10);

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        REQUIRE(*it == 10);
    }

}

TEST_CASE("list_assign_1") {

    SECTION("count > size") {
        HxSTL::list<int> l1(5, 5);

        l1.assign(10, 10);

        REQUIRE(l1.size() == 10);

        for (auto it = l1.begin(); it != l1.end(); ++it) {
            REQUIRE(*it == 10);
        }
    }

    SECTION("count <= size") {
        HxSTL::list<int> l1(10, 10);

        l1.assign(5, 5);

        REQUIRE(l1.size() == 5);

        for (auto it = l1.begin(); it != l1.end(); ++it) {
            REQUIRE(*it == 5);
        }
    }

}
