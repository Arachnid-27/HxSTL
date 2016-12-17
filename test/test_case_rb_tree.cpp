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

TEST_CASE("rb_tree_default_constructor") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(t1.size() == 0);

}

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

TEST_CASE("rb_tree_member_insert_unique") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(*(t1.insert_unique(1).first) == 1);
    REQUIRE(!(t1.insert_unique(1).second));
    REQUIRE(*(t1.insert_unique(3).first) == 3);
    REQUIRE((t1.insert_unique(2).second));
    REQUIRE(t1.size() == 3);

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t2;

    srand((unsigned) time(NULL));
    for (int i = 0; i != 100000; ++i) {
        t2.insert_unique(rand() % 50000);
    }

    REQUIRE(t2.size() <= 50000);

}
