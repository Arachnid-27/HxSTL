#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "basic_string.h"

TEST_CASE("basic_string_default_constructor") {

    HxSTL::basic_string<char> s1;

    REQUIRE(s1.size() == 0);
    REQUIRE(s1.capacity() == 15);
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
    REQUIRE(s2 == HxSTL::basic_string<char>(5, 'x'));
    REQUIRE(s3 == HxSTL::basic_string<char>(2, 'x'));

}

TEST_CASE("basic_string_c_str_n_constructor") {

    const char *c1 = "HxSTLHxSTL";
    HxSTL::basic_string<char> s1(c1, 5);

    REQUIRE(s1.size() == 5);
    REQUIRE(s1 == HxSTL::basic_string<char>("HxSTL"));

}

TEST_CASE("basic_string_c_str_constructor") {

    const char *c1 = "HxSTL";
    HxSTL::basic_string<char> s1(c1);

    REQUIRE(s1.size() == 5);
    REQUIRE(s1 == HxSTL::basic_string<char>("HxSTL"));

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
    HxSTL::basic_string<char> s2(HxSTL::move(s1));

    REQUIRE(s2.size() == 10);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s2 == HxSTL::basic_string<char>(10, 'x'));

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
    REQUIRE(s1 == s2);

}

TEST_CASE("basic_string_member_assign_3") {

    // -> "basic_string_member_assign_7"

    SECTION("count == npos") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2;

        s2.assign(s1, 5);

        REQUIRE(s2.size() == 5);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>(5, 'a'));
    }

    SECTION("count != npos") {
        HxSTL::basic_string<char> s1(10, 'a');
        HxSTL::basic_string<char> s2;

        s2.assign(s1, 5, 2);

        REQUIRE(s2.size() == 2);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>(2, 'a'));
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

    HxSTL::basic_string<char> s1("HxSTL");

    REQUIRE(s1[0] == 'H');
    REQUIRE(s1[1] == 'x');
    REQUIRE(s1[2] == 'S');
    REQUIRE(s1[3] == 'T');
    REQUIRE(s1[4] == 'L');

    s1[5];
    s1[6];

}

TEST_CASE("basic_string_member_front basic_string_member_back") {

    HxSTL::basic_string<char> s1("-HxSTL+");

    REQUIRE(s1.front() == '-');
    REQUIRE(s1.back() == '+');

    s1.front() = '+';
    s1.back() = '-';

    REQUIRE(s1 == HxSTL::basic_string<char>("+HxSTL-"));
    
}

TEST_CASE("basic_string_member_data basic_string_member_c_str") {

    const char *s1 = "HxSTL";
    HxSTL::basic_string<char> s2(s1);

    REQUIRE(s2.data() == s2.c_str());
    for (int i = 0; i != 5; ++i) {
        REQUIRE(s1[i] == s2.data()[i]);
        REQUIRE(s1[i] == s2.c_str()[i]);
    }

}

TEST_CASE("basic_string_member_begin basic_string_member_end") {

    const char *s1 = "HxSTL";
    HxSTL::basic_string<char> s2(s1);

    REQUIRE(*s2.begin() == 'H');
    REQUIRE(*s2.cbegin() == 'H');
    REQUIRE(*(s2.end() - 1) == 'L');
    REQUIRE(*(s2.cend() - 1) == 'L');

}

TEST_CASE("basic_string_member_empty") {

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2;

    REQUIRE(!s1.empty());
    REQUIRE(s2.empty());

}

TEST_CASE("basic_string_member_size basic_string_member_length") {

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2;

    REQUIRE(s1.size() == 10);
    REQUIRE(s1.length() == 10);
    REQUIRE(s2.size() == 0);
    REQUIRE(s2.length() == 0);

}

TEST_CASE("basic_string_member_reserve") {

    SECTION("new_cap > capacity") {
        HxSTL::basic_string<char> s1(10, 'x');

        s1.reserve(20);

        REQUIRE(s1.capacity() == 30);

        s1.reserve(100);

        REQUIRE(s1.capacity() == 100);
    }

    SECTION("new_cap <= size") {
        HxSTL::basic_string<char> s1(10, 'x');
        HxSTL::basic_string<char> s2(20, 'x');

        s1.reserve(5);
        s2.reserve(50);
        s2.reserve(10);

        REQUIRE(s1.capacity() == 15);
        REQUIRE(s2.capacity() == 20);
    }

    SECTION("size < new_cap <= capacity") {
        HxSTL::basic_string<char> s1(10, 'x');

        s1.resize(12);

        REQUIRE(s1.capacity() == 15);

        s1.reserve(30);
        s1.reserve(20);

        REQUIRE(s1.capacity() == 20);
    }

}

TEST_CASE("basic_string_member_capacity") {

    HxSTL::basic_string<char> s1(20, 'a');
    HxSTL::basic_string<char> s2(10, 'a');
    HxSTL::basic_string<char> s3;

    REQUIRE(s1.capacity() == 20);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s3.capacity() == 15);

}

TEST_CASE("basic_string_member_shrink_to_fit") {

    HxSTL::basic_string<char> s1(10, 'a');
    HxSTL::basic_string<char> s2(20, 'a');

    s2.append("aaaaa");

    REQUIRE(s1.size() == 10);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s2.size() == 25);
    REQUIRE(s2.capacity() == 40);

    s1.shrink_to_fit();
    s2.shrink_to_fit();

    REQUIRE(s1.size() == 10);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s2.size() == 25);
    REQUIRE(s2.capacity() == 25);

}

TEST_CASE("basic_string_member_clear") {

    HxSTL::basic_string<char> s1(20, 'a');

    s1.clear();

    REQUIRE(s1.size() == 0);
    REQUIRE(s1.capacity() == 20);

}

TEST_CASE("basic_string_member_insert_1") {

    SECTION("count > capacity - size") {
        HxSTL::basic_string<char> s1(10, 'a');

        s1.insert(5, 10, 'b');

        REQUIRE(s1.size() == 20);
        REQUIRE(s1.capacity() == 30);
        REQUIRE(s1 == HxSTL::basic_string<char>("aaaaabbbbbbbbbbaaaaa"));
    }

    SECTION("element_after < count <= capacity - size") {
        HxSTL::basic_string<char> s1(5, 'a');

        s1.insert(3, 5, 'b');

        REQUIRE(s1.size() == 10);
        REQUIRE(s1.capacity() == 15);
        REQUIRE(s1 == HxSTL::basic_string<char>("aaabbbbbaa"));
    }

    SECTION("count <= element_after <= capacity - size") {
        HxSTL::basic_string<char> s1(10, 'a');

        s1.insert(5, 5, 'b');

        REQUIRE(s1.size() == 15);
        REQUIRE(s1.capacity() == 15);
        REQUIRE(s1 == HxSTL::basic_string<char>("aaaaabbbbbaaaaa"));
    }

}

TEST_CASE("basic_string_member_insert_2") {

    // -> basic_string_member_insert_8

    HxSTL::basic_string<char> s1(10, 'a');

    s1.insert(2, "bbbbb");

    REQUIRE(s1.size() == 15);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1 == HxSTL::basic_string<char>("aabbbbbaaaaaaaa"));

}

TEST_CASE("basic_string_member_insert_3") {

    // -> basic_string_member_insert_8

    HxSTL::basic_string<char> s1(10, 'a');

    s1.insert(2, "bbbbb", 2);

    REQUIRE(s1.size() == 12);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1 == HxSTL::basic_string<char>("aabbaaaaaaaa"));

}

TEST_CASE("basic_string_member_insert_4") {

    // -> basic_string_member_insert_8

    HxSTL::basic_string<char> s1(5, 'b');
    HxSTL::basic_string<char> s2(10, 'a');

    s2.insert(8, s1);

    REQUIRE(s2.size() == 15);
    REQUIRE(s2.capacity() == 15);
    REQUIRE(s2 == HxSTL::basic_string<char>("aaaaaaaabbbbbaa"));

}

TEST_CASE("basic_string_member_insert_5") {

    // -> basic_string_member_insert_8

    SECTION("count == npos") {
        HxSTL::basic_string<char> s1(5, 'b');
        HxSTL::basic_string<char> s2(10, 'a');

        s2.insert(3, s1, 2);

        REQUIRE(s2.size() == 13);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>("aaabbbaaaaaaa"));
    }

    SECTION("count != npos") {
        HxSTL::basic_string<char> s1(5, 'b');
        HxSTL::basic_string<char> s2(10, 'a');

        s2.insert(3, s1, 2, 2);

        REQUIRE(s2.size() == 12);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>("aaabbaaaaaaa"));
    }

}

TEST_CASE("basic_string_member_insert_6") {

    // -> basic_string_member_insert_1

    HxSTL::basic_string<char> s1(20, 'a');

    REQUIRE(*(s1.insert(s1.begin() + 5, 'b')) == 'b');
    REQUIRE(s1.size() == 21);
    REQUIRE(s1.capacity() == 40);
    REQUIRE(s1 == HxSTL::basic_string<char>("aaaaabaaaaaaaaaaaaaaa"));

}

TEST_CASE("basic_string_member_insert_7") {

    // -> basic_string_member_insert_1

    HxSTL::basic_string<char> s1(10, 'a');

    REQUIRE(*(s1.insert(s1.begin() + 5, 5, 'b')) == 'b');
    REQUIRE(s1.size() == 15);
    REQUIRE(s1.capacity() == 15);
    REQUIRE(s1 == HxSTL::basic_string<char>("aaaaabbbbbaaaaa"));

}

TEST_CASE("basic_string_member_insert_8") {

    SECTION("count > capacity - size") {
        const char *s1 = "bbbbbbbbbb";
        HxSTL::basic_string<char> s2(10, 'a');

        REQUIRE(*(s2.insert(s2.begin() + 5, s1, s1 + 10)) == 'b');
        REQUIRE(s2.size() == 20);
        REQUIRE(s2.capacity() == 30);
        REQUIRE(s2 == HxSTL::basic_string<char>("aaaaabbbbbbbbbbaaaaa"));
    }

    SECTION("element_after < count <= capacity - size") {
        HxSTL::basic_string<char> s1(5, 'b');
        HxSTL::basic_string<char> s2(5, 'a');

        REQUIRE(*(s2.insert(s2.begin() + 3, s1.begin(), s1.end())) == 'b');
        REQUIRE(s2.size() == 10);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>("aaabbbbbaa"));
    }

    SECTION("count <= element_after <= capacity - size") {
        const char *s1 = "bbbbbbbbbb";
        HxSTL::basic_string<char> s2(10, 'a');

        REQUIRE(*(s2.insert(s2.begin() + 5, s1, s1 + 5)));
        REQUIRE(s2.size() == 15);
        REQUIRE(s2.capacity() == 15);
        REQUIRE(s2 == HxSTL::basic_string<char>("aaaaabbbbbaaaaa"));
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
