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

    int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
    HxSTL::set<int> s1(a1, a1 + 15);

    REQUIRE(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

}

TEST_CASE("set_init_constructor") {

    HxSTL::set<int> s1({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

    int i = 0;
    for (auto v : s1) {
        REQUIRE(i++ == v);
    }

}

TEST_CASE("set_copy_constructor") {

    HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    HxSTL::set<int> s2(s1);

    REQUIRE(s1 == s2);

}

TEST_CASE("set_move_constructor") {

    HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    HxSTL::set<int> s2(HxSTL::move(s1));

    REQUIRE(s2 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

}

TEST_CASE("set_member_begin set_member_cbegin") {
    
    HxSTL::set<int> s1({ 0, 1 });
    const HxSTL::set<int> s2({ 1, 2 });

    REQUIRE(*s1.begin() == 0);
    REQUIRE(*s2.begin() == 1);
    REQUIRE(*s1.cbegin() == 0);
    REQUIRE(*s2.cbegin() == 1);

}

TEST_CASE("set_member_end set_member_cend") {
    
    HxSTL::set<int> s1({ 0, 1 });
    const HxSTL::set<int> s2({ 1, 2 });

    REQUIRE(*(--s1.end()) == 1);
    REQUIRE(*(--s2.end()) == 2);
    REQUIRE(*(--s1.cend()) == 1);
    REQUIRE(*(--s2.cend()) == 2);

}

TEST_CASE("set_member_empty") {

    HxSTL::set<int> s1;
    HxSTL::set<int> s2({ 0 });

    REQUIRE(s1.empty());
    REQUIRE(!s2.empty());

}

TEST_CASE("set_member_size") {

    HxSTL::set<int> s1;
    HxSTL::set<int> s2({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    REQUIRE(s1.size() == 0);
    REQUIRE(s2.size() == 10);

}

TEST_CASE("set_member_clear") {

    HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    s1.clear();

    REQUIRE(s1.empty());

}

TEST_CASE("set_member_insert_1") {

    HxSTL::set<int> s1;

    REQUIRE(*(s1.insert(1).first) == 1);
    REQUIRE(!(s1.insert(1).second));
    REQUIRE(*(s1.insert(3).first) == 3);
    REQUIRE((s1.insert(2).second));
    REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3 }));

    HxSTL::set<int> s2;

    srand((unsigned) time(NULL));
    for (int i = 0; i != 100000; ++i) {
        s2.insert(rand() % 50000);
    }

    REQUIRE(s2.size() <= 50000);

}

TEST_CASE("set_member_insert_2") {

    SECTION("header leftmost rightmost") {
        HxSTL::set<int> s1;

        REQUIRE(*(s1.insert(s1.end(), 5)) == 5);
        REQUIRE(*(s1.insert(s1.end(), 6)) == 6);
        REQUIRE(*(s1.insert(s1.begin(), 4)) == 4);
        REQUIRE(*(s1.insert(--s1.end(), 7)) == 7);
        REQUIRE(s1 == HxSTL::set<int>({ 4, 5, 6, 7 }));
    }

    SECTION("before to right") {
        //      6(hint)
        //     / \
        //    4   7
        //   / \
        //  3  NIL
        HxSTL::set<int> s1({ 6, 4, 7, 3 });
        auto hint = s1.cbegin();

        ++hint, ++hint;

        REQUIRE(*(s1.insert(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("before to left") {
        //     4
        //    / \
        //   3   6(hint)
        //      / \
        //    NIL  7
        HxSTL::set<int> s1({ 4, 3, 6, 7 });
        auto hint = s1.cbegin();

        ++hint, ++hint;

        REQUIRE(*(s1.insert(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("after to right") {
        HxSTL::set<int> s1({ 6, 4, 7, 3 });
        auto hint = s1.cbegin();

        ++hint;

        REQUIRE(*(s1.insert(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("after to left") {
        HxSTL::set<int> s1({ 4, 3, 6, 7 });
        auto hint = s1.cbegin();

        ++hint;

        REQUIRE(*(s1.insert(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("equal") {
        HxSTL::set<int> s1({ 1, 2, 3 });

        REQUIRE(s1.insert(++s1.begin(), 2) == ++s1.begin());
        REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3 }));
    }

    SECTION("other") {
        HxSTL::set<int> s1({ 1, 2, 3 });

        REQUIRE(*(s1.insert(s1.begin(), 4)) == 4);
        REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3, 4 }));
    }

}

TEST_CASE("set_member_insert_3") {

    int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
    HxSTL::set<int> s1;

    s1.insert(a1, a1 + 15);

    REQUIRE(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

}

TEST_CASE("set_member_insert_4") {

    HxSTL::set<int> s1;

    s1.insert({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

    REQUIRE(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

}

TEST_CASE("set_member_emplace") {

    HxSTL::set<int> s1;

    REQUIRE(*(s1.emplace(1).first) == 1);
    REQUIRE(!(s1.emplace(1).second));
    REQUIRE(*(s1.emplace(3).first) == 3);
    REQUIRE((s1.emplace(2).second));
    REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3 }));

}

TEST_CASE("set_member_emplace_hint") {

    SECTION("header leftmost rightmost") {
        HxSTL::set<int> s1;

        REQUIRE(*(s1.emplace_hint(s1.end(), 5)) == 5);
        REQUIRE(*(s1.emplace_hint(s1.end(), 6)) == 6);
        REQUIRE(*(s1.emplace_hint(s1.begin(), 4)) == 4);
        REQUIRE(*(s1.emplace_hint(--s1.end(), 7)) == 7);
        REQUIRE(s1 == HxSTL::set<int>({ 4, 5, 6, 7 }));
    }

    SECTION("before to right") {
        HxSTL::set<int> s1({ 6, 4, 7, 3 });
        auto hint = s1.cbegin();

        ++hint, ++hint;

        REQUIRE(*(s1.emplace_hint(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("before to left") {
        HxSTL::set<int> s1({ 4, 3, 6, 7 });
        auto hint = s1.cbegin();

        ++hint, ++hint;

        REQUIRE(*(s1.emplace_hint(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("after to right") {
        HxSTL::set<int> s1({ 6, 4, 7, 3 });
        auto hint = s1.cbegin();

        ++hint;

        REQUIRE(*(s1.emplace_hint(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("after to left") {
        HxSTL::set<int> s1({ 4, 3, 6, 7 });
        auto hint = s1.cbegin();

        ++hint;

        REQUIRE(*(s1.emplace_hint(hint, 5)) == 5);
        REQUIRE(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
    }

    SECTION("equal") {
        HxSTL::set<int> s1({ 1, 2, 3 });

        REQUIRE(s1.emplace_hint(++s1.begin(), 2) == ++s1.begin());
        REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3 }));
    }

    SECTION("other") {
        HxSTL::set<int> s1({ 1, 2, 3 });

        REQUIRE(*(s1.emplace_hint(s1.begin(), 4)) == 4);
        REQUIRE(s1 == HxSTL::set<int>({ 1, 2, 3, 4 }));
    }

}
