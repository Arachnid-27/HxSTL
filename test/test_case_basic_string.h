#include "catch.hpp"
#include "basic_string.h"
#include <iostream>

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

    auto t1 = s1.capacity();

    HxSTL::basic_string<char> s2(HxSTL::move(s1));

    REQUIRE(s2.size() == 10);
    REQUIRE(s2.capacity() == t1);

    for (auto it1 = s2.begin(), it2 = s2.end(); it1 != it2; ++it1) {
        REQUIRE(*it1 == 'x');
    }

}

TEST_CASE("basic_string_copy_assignment") {

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2(20, 'b');

    s2 = s1;

    REQUIRE(s1.size() == s2.size());
    REQUIRE(s1.capacity() == s2.capacity());

    for (auto it1 = s1.begin(), it2 = s1.end(), it3 = s2.begin(), it4 = s2.end(); 
            it1 != it2; ++it1, ++it3) {
        REQUIRE(*it1 == *it3);
    }

}

TEST_CASE("basic_string_move_assignment") {

    const char *c1 = "HxSTL";
    HxSTL::basic_string<char> s1(c1);
    HxSTL::basic_string<char> s2(20, 'x');
    auto t1 = s1.capacity();

    s2 = HxSTL::move(s1);

    REQUIRE(s2.size() == 5);
    REQUIRE(s2.capacity() == t1);

    for (decltype(s2.size()) i = 0; i != s2.size(); ++i) {
        REQUIRE(s2[i] == c1[i]);
    }

}

TEST_CASE("basic_string_c_str_assignment"){

    const char *c1 = "HxSTL";
    HxSTL::basic_string<char> s1(20, 'x');
    auto t1 = s1.capacity();

    s1 = c1;

    REQUIRE(s1.size() == 5);
    REQUIRE(s1.capacity() == t1);

    for (decltype(s1.size()) i = 0; i != s1.size(); ++i) {
        REQUIRE(s1[i] == c1[i]);
    }

}

TEST_CASE("basic_string_char_assignment"){

    HxSTL::basic_string<char> s1(20, 'x');
    auto t1 = s1.capacity();

    s1 = 'x';

    REQUIRE(s1.size() == 1);
    REQUIRE(s1.capacity() == t1);
    REQUIRE(s1[0] == 'x');

    HxSTL::basic_string<char> s2;

    s2 = 'x';

    REQUIRE(s2.size() == 1);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s2[0] == 'x');

}

TEST_CASE("basic_string_assign_1") {

    HxSTL::basic_string<char> s1(20, 'x');
    auto t1 = s1.capacity();

    s1.assign(10, 'r');

    REQUIRE(s1.size() == 10);
    REQUIRE(s1.capacity() == t1);

    s1.assign(15, 'r');

    REQUIRE(s1.size() == 15);
    REQUIRE(s1.capacity() == t1);

    s1.assign(30, 'r');
    REQUIRE(s1.size() == 30);
    REQUIRE(s1.capacity() == (2 * t1 > 30 ? 2 * t1 : 30));

}

TEST_CASE("basic_string_assign_2") {

    HxSTL::basic_string<char> s1("HxSTL");
    HxSTL::basic_string<char> s2(20, 'x');

    s2.assign(s1);

    REQUIRE(s1.size() == s2.size());
    REQUIRE(s1.capacity() == s2.capacity());

}

TEST_CASE("basic_string_assign_3") {

    HxSTL::basic_string<char> s1("HxSTLHxSTL");
    HxSTL::basic_string<char> s2(20, 'x');
    auto t1 = s2.capacity();

    s2.assign(s1, 5);

    REQUIRE(s2.size() == 5);
    REQUIRE(s2.capacity() == t1);

    for (auto it1 = s1.begin() + 5, it2 = s1.end(), it3 = s2.begin(), it4 = s2.end(); 
            it1 != it2; ++it1, ++it3) {
        REQUIRE(*it1 == *it3);
    }

    s2.assign(s1, 0, 5);

    REQUIRE(s2.size() == 5);
    REQUIRE(s2.capacity() == t1);

    for (auto it1 = s1.begin(), it2 = s1.begin() + 5, it3 = s2.begin(), it4 = s2.end(); 
            it1 != it2; ++it1, ++it3) {
        REQUIRE(*it1 == *it3);
    }

}

TEST_CASE("basic_string_assign_4") {

    HxSTL::basic_string<char> s1("HxSTL");
    HxSTL::basic_string<char> s2;
    auto t1 = s1.capacity();

    s2.assign(HxSTL::move(s1));

    REQUIRE(s2.size() == 5);
    REQUIRE(s2.capacity() == t1);

}

TEST_CASE("basic_string_assign_5") {
}
