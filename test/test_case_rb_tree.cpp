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

TEST_CASE("rb_tree_member_insert_hint_equal") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(1)) == 1);
    REQUIRE(*(t1.insert_equal(3)) == 3);
    REQUIRE(*(t1.insert_equal(2)) == 2);
    REQUIRE(*(t1.insert_equal(t1.begin(), 0)) == 0);
    REQUIRE(*(t1.insert_equal(t1.end(), 4)) == 4);

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

TEST_CASE("rb_tree_member_erase") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    t1.insert_unique(5);
    t1.insert_unique(9);
    t1.insert_unique(0);
    t1.insert_unique(7);
    t1.insert_unique(8);
    t1.insert_unique(6);
    t1.insert_unique(3);
    t1.insert_unique(4);
    t1.insert_unique(1);
    t1.insert_unique(2);

    t1.erase(t1.begin());
    t1.erase(t1.begin());
    t1.erase(t1.begin());
    t1.erase(t1.begin());
    t1.erase(t1.begin());

    REQUIRE(t1.size() == 5);

}

TEST_CASE("rb_tree_member_lower_bound") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    REQUIRE(t1.lower_bound(0) == t1.end());

    t1.insert_unique(5);
    t1.insert_unique(9);
    t1.insert_unique(1);
    t1.insert_unique(7);
    t1.insert_unique(3);

    REQUIRE(*t1.lower_bound(4) == 5);
    REQUIRE(*t1.lower_bound(5) == 5);
    REQUIRE(*t1.lower_bound(6) == 7);
    REQUIRE(*t1.lower_bound(7) == 7);
    REQUIRE(t1.lower_bound(10) == t1.end());

}

TEST_CASE("rb_tree_member_upper_bound") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    t1.insert_unique(5);
    t1.insert_unique(9);
    t1.insert_unique(1);
    t1.insert_unique(7);
    t1.insert_unique(3);

    REQUIRE(*t1.upper_bound(4) == 5);
    REQUIRE(*t1.upper_bound(5) == 7);
    REQUIRE(*t1.upper_bound(6) == 7);
    REQUIRE(*t1.upper_bound(7) == 9);
    REQUIRE(t1.upper_bound(10) == t1.end());

}

TEST_CASE("rb_tree_member_find") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    t1.insert_unique(5);
    t1.insert_unique(9);
    t1.insert_unique(1);
    t1.insert_unique(7);
    t1.insert_unique(3);

    REQUIRE(t1.find(4) == t1.end());
    REQUIRE(t1.upper_bound(10) == t1.end());
    REQUIRE(*t1.find(5) == 5);
    REQUIRE(*t1.find(9) == 9);
    REQUIRE(*t1.find(1) == 1);
    REQUIRE(*t1.find(7) == 7);

}

TEST_CASE("rb_tree_member_clear") {

    HxSTL::rb_tree<int, int, identity<int>, HxSTL::less<int>> t1;

    t1.insert_unique(5);
    t1.insert_unique(9);
    t1.insert_unique(1);
    t1.insert_unique(7);
    t1.insert_unique(3);
    t1.clear();

    REQUIRE(t1.size() == 0);

}
