#define CATCH_CONFIG_MAIN
#include <ctime>
#include <cstdlib>
#include "catch.hpp"
#include "rb_tree.h"
#include "functional.h"

template <class T>
struct identity {
    T operator()(T x) { return x; }
};

TEST_CASE("rb_tree_member_insert_equal") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(3)) == 3);
    REQUIRE(*(t1.insert_equal(2)) == 2);
    REQUIRE(t1.size() == 4);

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t2;

    srand((unsigned) time(NULL));
    for (int i = 0; i != 100000; ++i) {
        t2.insert_equal(rand() % 50000);
    }

    REQUIRE(t2.size() == 100000);

}

TEST_CASE("rb_tree_member_insert_hint_equal") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(3)) == 3);
    REQUIRE(*(t1.insert_equal(2)) == 2);
    REQUIRE(*(t1.insert_equal(t1.begin(), 0)) == 0);
    REQUIRE(*(t1.insert_equal(t1.end(), 4)) == 4);

}
