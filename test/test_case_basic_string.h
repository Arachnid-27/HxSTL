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
    REQUIRE(s1 == s2);

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

    // -> "basic_string_member_assign_2"

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2(20, 'b');

    s2 = s1;

    REQUIRE(s2.size() == 10);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s2 == s1);

}

TEST_CASE("basic_string_move_assignment") {

    // -> "basic_string_member_assign_4"

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2(20, 'b');

    s2 = HxSTL::move(s1);

    REQUIRE(s2.size() == 10);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s2 == HxSTL::basic_string<char>(10, 'a'));

}

TEST_CASE("basic_string_c_str_assignment"){

    // -> "basic_string_member_assign_6"

    HxSTL::basic_string<char> s1(20, 'x');

    s1 = "HxSTL";

    REQUIRE(s1.size() == 5);
    REQUIRE(s1.capacity() == 20);
    REQUIRE(s1 == HxSTL::basic_string<char>("HxSTL"));

}

TEST_CASE("basic_string_char_assignment"){

    // -> "basic_string_member_assign_1"

    HxSTL::basic_string<char> s1;

    s1 = 'x';

    REQUIRE(s1.size() == 1);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1[0] == 'x');

}

TEST_CASE("basic_string_member_assign_1") {

    SECTION("count > capacity") {
        HxSTL::basic_string<char> s1(10, 'a');

        s1.assign(20, 'b');

        REQUIRE(s1.size() == 20);
        REQUIRE(s1.capacity() == 30);
        REQUIRE(s1 == HxSTL::basic_string<char>(20, 'b'));

        s1.assign(100, 'c');

        REQUIRE(s1.size() == 100);
        REQUIRE(s1.capacity() == 100);
        REQUIRE(s1 == HxSTL::basic_string<char>(100, 'c'));
    }

    SECTION("copy on write") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2(s1);

        REQUIRE(s1.data() == s2.data());

        s2.assign(20, 'b');

        REQUIRE(s1.data() != s2.data());
        REQUIRE(s2.size() == 20);
        REQUIRE(s2.capacity() == 30);
        REQUIRE(s2 == HxSTL::basic_string<char>(20, 'b'));
    }

    SECTION("count <= size") {
        HxSTL::basic_string<char> s1(20, 'a');

        s1.assign(10, 'b');

        REQUIRE(s1.size() == 10);
        REQUIRE(s1.capacity() == 20);
        REQUIRE(s1 == HxSTL::basic_string<char>(10, 'b'));
    }

    SECTION("size < count <= capacity") {
        HxSTL::basic_string<char> s1(10, 'a');

        s1.assign(12, 'b');

        REQUIRE(s1.size() == 12);
        REQUIRE(s1.capacity() == 15);
        REQUIRE(s1 == HxSTL::basic_string<char>(12, 'b'));
    }

}

TEST_CASE("basic_string_member_assign_2") {

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2(20, 'b');

    s2.assign(s1);

    REQUIRE(s1.size() == s2.size());
    REQUIRE(s1.capacity() == s2.capacity());
    REQUIRE(s1.data() == s2.data());

}

TEST_CASE("basic_string_member_assign_3") {

    // -> "basic_string_member_assign_7"

    SECTION("count == npos") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2;

        s1.assign(s1, 5);

        REQUIRE(s1.size() == 5);
        REQUIRE(s1.capacity() == 15);
        REQUIRE(s1 == HxSTL::basic_string<char>(5, 'a'));
    }

    SECTION("count != npos") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2;

        s1.assign(s1, 5, 2);

        REQUIRE(s1.size() == 2);
        REQUIRE(s1.capacity() == 15);
        REQUIRE(s1 == HxSTL::basic_string<char>(2, 'a'));
    }

}

TEST_CASE("basic_string_member_assign_4") {

    HxSTL::basic_string<char> s1(20, 'a');
    HxSTL::basic_string<char> s2;

    s2.assign(HxSTL::move(s1));

    REQUIRE(s2.size() == 20);
    REQUIRE(s2.capacity() == 20);

}

TEST_CASE("basic_string_member_assign_5") {

    // -> "basic_string_member_assign_7"

    HxSTL::basic_string<char> s1;

    s1.assign("HxSTL---", 5);

    REQUIRE(s1.size() == 5);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1 == HxSTL::basic_string<char>("HxSTL"));
    
}

TEST_CASE("basic_string_member_assign_6") {

    // -> "basic_string_member_assign_7"

    HxSTL::basic_string<char> s1;

    s1.assign("HxSTL");

    REQUIRE(s1.size() == 5);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1 == HxSTL::basic_string<char>("HxSTL"));

}

TEST_CASE("basic_string_member_assign_7") {

    SECTION("count > capacity") {
        HxSTL::basic_string<char> s1(30, 'a');
        HxSTL::basic_string<char> s2(20, 'b');

        s2.assign(s1.cbegin(), s1.cend());

        REQUIRE(s2.size() == 30);
        REQUIRE(s2.capacity() == 40);
        REQUIRE(s1 == s2);
    }

    SECTION("copy_on_write") {
        HxSTL::basic_string<char> s1(20, 'a');
        HxSTL::basic_string<char> s2(s1);
        const char *s3 = "HxSTL";

        REQUIRE(s1.data() == s2.data());

        s2.assign(s3, s3 + 5);

        REQUIRE(s2.size() == 5);
        REQUIRE(s2.capacity() == 20);
        REQUIRE(s1.data() != s2.data());
        REQUIRE(s2 == HxSTL::basic_string<char>(s3));
    }

    SECTION("count <= size") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2(20, 'b');

        s2.assign(s1.cbegin(), s1.cend());

        REQUIRE(s2.size() == 10);
        REQUIRE(s2.capacity() == 20);
        REQUIRE(s2 == s1);
    }

    SECTION("size < count <= capacity") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2(5, 'b');

        s2.assign(s1.cbegin(), s1.cend());

        REQUIRE(s2.size() == 10);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == s1);
    }

}

TEST_CASE("basic_string_member_at") {

    SECTION("pos >= size") {
        HxSTL::basic_string<char> s1(10, 'a'); 
        bool flag = false;

        try {
            s1.at(10);
        } catch (HxSTL::out_of_range) {
            flag = true;
        }

        REQUIRE(flag);

        try {
            s1.at(-1);
        } catch (HxSTL::out_of_range) {
            flag = false;
        }

        REQUIRE(!flag);
    }

    SECTION("copy_on_write") {
        HxSTL::basic_string<char> s1(10, 'a');
        const HxSTL::basic_string<char> s2(s1);

        REQUIRE(s1.data() == s2.data());

        const char &c1 = s2.at(0);

        REQUIRE(s1.data() == s2.data());

        char &c2 = s1.at(0);

        REQUIRE(s1.data() != s2.data());
    }

    SECTION("pos < size") {
        HxSTL::basic_string<char> s1("HxSTL");

        REQUIRE(s1.at(0) == 'H');
        REQUIRE(s1.at(1) == 'x');
        REQUIRE(s1.at(2) == 'S');
        REQUIRE(s1.at(3) == 'T');
        REQUIRE(s1.at(4) == 'L');
    }

}

TEST_CASE("basic_string_member_operator[]") {

    SECTION("copy_on_write") {
        HxSTL::basic_string<char> s1(10, 'a');
        const HxSTL::basic_string<char> s2(s1);

        REQUIRE(s1.data() == s2.data());

        const char &c1 = s2[0];

        REQUIRE(s1.data() == s2.data());

        char &c2 = s1[0];

        REQUIRE(s1.data() != s2.data());
    }

    SECTION("value") {
        HxSTL::basic_string<char> s1("HxSTL");

        REQUIRE(s1[0] == 'H');
        REQUIRE(s1[1] == 'x');
        REQUIRE(s1[2] == 'S');
        REQUIRE(s1[3] == 'T');
        REQUIRE(s1[4] == 'L');

        s1[5];
        s1[6];
    }

}

TEST_CASE("basic_string_member_compare_1") {

    HxSTL::basic_string<char> s1(10, 'b');
    HxSTL::basic_string<char> s2;
    HxSTL::basic_string<char> s3(5, 'b');
    HxSTL::basic_string<char> s4(20, 'b');
    HxSTL::basic_string<char> s5("a");
    HxSTL::basic_string<char> s6("c");

    REQUIRE(s1.compare(s2) == 10);
    REQUIRE(s1.compare(s3) == 5);
    REQUIRE(s1.compare(s4) == -10);
    REQUIRE(s1.compare(s5) == 1);
    REQUIRE(s1.compare(s6) == -1);

}
