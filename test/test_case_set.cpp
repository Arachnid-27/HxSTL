#define CATCH_CONFIG_MAIN
#include <ctime>
#include <cstdlib>
#include "catch.hpp"
#include "set.h"

TEST_CASE("set_default_constructor") {

    HxSTL::set<int> s1;

    REQUIRE(s1.empty());
    REQUIRE(s1.size() == 0);

}

TEST_CASE("set_range_constructor") {

    int a1[] = { 0, 7, 2, 9, 3, 5, 6, 4, 8, 1 };
    HxSTL::set<int> s1(a1, a1 + 10);

    REQUIRE(s1.size() == 10);

    int i = 0;
    for (auto v : s1) {
        REQUIRE(i++ == v);
    }

}
