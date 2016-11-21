#include "catch.hpp"
#include "basic_string.h"

TEST_CASE("basic_string_default_constructor") {

    HxSTL::basic_string<char> s1;

    REQUIRE(s1.size() == 0);
    REQUIRE(s1.capacity() == 0);
    REQUIRE(*s1.data() == 0);

}

TEST_CASE("basic_string_fill_constructor") {

    HxSTL::basic_string<char> s1(10, 'x');

    REQUIRE(s1.size() == 10);

    for (auto it1 = s1.begin(), it2 = s1.end(); it1 != it2; ++it1) {
        REQUIRE(*it1 == 'x');
    }

}

TEST_CASE("basic_string_copy_n_constructor") {

    HxSTL::basic_string<char> s1(10, 'x');
    HxSTL::basic_string<char> s2(s1, 5);
    HxSTL::basic_string<char> s3(s1, 3, 2);

    REQUIRE(s2.size() == 5);
    REQUIRE(s3.size() == 2);

    for (auto it1 = s2.begin(), it2 = s2.end(); it1 != it2; ++it1) {
        REQUIRE(*it1 == 'x');
    }

    for (auto it1 = s3.begin(), it2 = s3.end(); it1 != it2; ++it1) {
        REQUIRE(*it1 == 'x');
    }

}

TEST_CASE("basic_string_c_str_n_constructor") {

    const char *c1 = "HxSTLHxSTL";
    HxSTL::basic_string<char> s1(c1, 5);

    REQUIRE(s1.size() == 5);

    for (decltype(s1.size()) i = 0; i != s1.size(); ++i) {
        REQUIRE(s1[i] == c1[i]);
    }

}

TEST_CASE("basic_string_c_str_constructor") {

    const char *c1 = "HxSTL";
    HxSTL::basic_string<char> s1(c1);

    REQUIRE(s1.size() == 5);

    for (decltype(s1.size()) i = 0; i != s1.size(); ++i) {
        REQUIRE(s1[i] == c1[i]);
    }

}

TEST_CASE("basic_string_copy_constructor") {

    HxSTL::basic_string<char> s1("HxSTL");
    HxSTL::basic_string<char> s2(s1);

    REQUIRE(s2.size() == s1.size());
    REQUIRE(s2.capacity() == s1.capacity());

    for (decltype(s1.size()) i = 0; i != s1.size(); ++i) {
        REQUIRE(s1[i] == s2[i]);
    }

}

TEST_CASE("basic_string_move_constructor") {

    HxSTL::basic_string<char> s1(10, 'x');

    auto t1 = s1.size();
    auto t2 = s1.capacity();

    HxSTL::basic_string<char> s2(HxSTL::move(s1));

    REQUIRE(s2.size() == t1);
    REQUIRE(s2.capacity() == t2);
    REQUIRE(s1.size() == 0);
    REQUIRE(s1.capacity() == 0);

    for (auto it1 = s2.begin(), it2 = s2.end(); it1 != it2; ++it1) {
        REQUIRE(*it1 == 'x');
    }

}
