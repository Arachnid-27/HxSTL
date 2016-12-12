#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "deque.h"
#include "list.h"

TEST_CASE("deque_default_constructor") {

    HxSTL::deque<int> d1;

    REQUIRE(d1.size() == 0);
    REQUIRE(d1.empty());

}

TEST_CASE("deque_fill_constructor") {

    HxSTL::deque<int> d1(10, 10);
    HxSTL::deque<int> d2(150, 10);

    REQUIRE(d1.size() == 10);
    REQUIRE(d2.size() == 150);

    for (auto i : d1) {
        REQUIRE(i == 10);
    }
    for (auto i : d2) {
        REQUIRE(i == 10);
    }

}

TEST_CASE("deque_range_constructor") {

    int a1[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    HxSTL::list<int> l1(150, 10);
    HxSTL::deque<int> d1(a1, a1 + 10);
    HxSTL::deque<int> d2(l1.begin(), l1.end());

    REQUIRE(d1 == HxSTL::deque<int>(10, 10));
    REQUIRE(d2 == HxSTL::deque<int>(150, 10));

}

TEST_CASE("deque_copy_constructor") {

    HxSTL::deque<int> d1(10, 10);
    HxSTL::deque<int> d2(d1);

    REQUIRE(d1 == d2);

}

TEST_CASE("deque_move_constructor") {

    HxSTL::deque<int> d1(10, 10);
    HxSTL::deque<int> d2(HxSTL::move(d1));

    REQUIRE(d1 == HxSTL::deque<int>(10, 10));

}

TEST_CASE("deque_init_constructor") {

    HxSTL::deque<int> d1({ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 });

    REQUIRE(d1 == HxSTL::deque<int>(10, 10));

}

TEST_CASE("deque_copy_assignment") {

    HxSTL::deque<int> d1(10, 10);
    HxSTL::deque<int> d2;

    d2 = d1;

    REQUIRE(d1 == d2);

}

TEST_CASE("deque_move_assignment") {

    HxSTL::deque<int> d1(10, 10);
    HxSTL::deque<int> d2;

    d2 = HxSTL::move(d1);

    REQUIRE(d2 == HxSTL::deque<int>(10, 10));

}

TEST_CASE("deque_init_assignment") {

    HxSTL::deque<int> d1;

    d1 = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };

    REQUIRE(d1 == HxSTL::deque<int>(10, 10));

}

TEST_CASE("deque_member_assign_1") {
}

TEST_CASE("deque_member_assign_2") {
}

TEST_CASE("deque_member_assign_3") {
}

TEST_CASE("deque_member_at") {

    HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
    const HxSTL::deque<int> d2(d1);

    for (int i = 0; i != 10; ++ i) {
        REQUIRE(d1.at(i) == i);
        REQUIRE(d2.at(i) == i);
    }

    bool except = false;

    try {
        d1.at(11);
    } catch (HxSTL::out_of_range) {
        except = true;
    }

    REQUIRE(except);

}

TEST_CASE("deque_member_operator[]") {

    HxSTL::deque<int> d1({ 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    d1[0] = 0;

    REQUIRE(d1[0] == 0);

    bool except = false;

    try {
        auto i = d1[11];
    } catch (HxSTL::out_of_range) {
        except = true;
    }

    REQUIRE(!except);

}

TEST_CASE("deque_member_front deque_member_back") {

    HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    REQUIRE(d1.front() == 0);
    REQUIRE(d1.back() == 9);

}

TEST_CASE("deque_member_begin deque_member_cbegin") {
    
    HxSTL::deque<int> d1({ 0, 1 });
    const HxSTL::deque<int> d2({ 1, 2 });

    REQUIRE(*d1.begin() == 0);
    REQUIRE(*d2.begin() == 1);
    REQUIRE(*d1.cbegin() == 0);
    REQUIRE(*d2.cbegin() == 1);

}

TEST_CASE("deque_member_end deque_member_cend") {
    
    HxSTL::deque<int> d1({ 0, 1 });
    const HxSTL::deque<int> d2({ 1, 2 });

    REQUIRE(*(d1.end() - 1) == 1);
    REQUIRE(*(d2.end() - 1) == 2);
    REQUIRE(*(d1.cend() - 1) == 1);
    REQUIRE(*(d2.cend() - 1) == 2);

}

TEST_CASE("deque_member_clear") {

    HxSTL::deque<int> d1(10, 10);

    d1.clear();

    REQUIRE(d1.empty());

}

TEST_CASE("deque_member_insert_1") {
    
    HxSTL::deque<int> d1({ 1, 2, 4, 5, 6, 8 });

    REQUIRE(*d1.insert(d1.begin(), 0) == 0);
    REQUIRE(*d1.insert(d1.begin() + 3, 3) == 3);
    REQUIRE(*d1.insert(d1.end() - 1, 7) == 7);
    REQUIRE(*d1.insert(d1.end(), 9) == 9);
    REQUIRE(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

    HxSTL::deque<int> d2(127, 1);

    REQUIRE(*d2.insert(d2.end(), 1) == 1);
    REQUIRE(*d2.insert(d2.end(), 1) == 1);
    REQUIRE(d2 == HxSTL::deque<int>(129, 1));

}

TEST_CASE("deque_member_insert_2") {
}

TEST_CASE("deque_member_insert_3") {

    SECTION("index < size / 2") {
        HxSTL::deque<int> d1({ 4, 4, 4, 4, 4, 4, 4, 4 });

        REQUIRE(*d1.insert(d1.begin(), 3, 3) == 3);
        REQUIRE(*d1.insert(d1.begin() + 2, 3, 2) == 2);
        REQUIRE(*d1.insert(d1.begin() + 5, 2, 1) == 1);
        REQUIRE(d1 == HxSTL::deque<int>({ 3, 3, 2, 2, 2, 1, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4 }));

        HxSTL::deque<int> d2(127, 1);

        REQUIRE(*d2.insert(d2.begin(), 10, 1) == 1);
        REQUIRE(d2 == HxSTL::deque<int>(137, 1));
    }

    SECTION("index >= size / 2") {
        HxSTL::deque<int> d1({ 4, 4, 4, 4, 4, 4, 4, 4 });

        REQUIRE(*d1.insert(d1.end(), 3, 3) == 3);
        REQUIRE(*d1.insert(d1.end() - 2, 3, 2) == 2);
        REQUIRE(*d1.insert(d1.end() - 5, 2, 1) == 1);
        REQUIRE(d1 == HxSTL::deque<int>({ 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 1, 2, 2, 2, 3, 3 }));

        HxSTL::deque<int> d2(127, 1);

        REQUIRE(*d2.insert(d2.end(), 10, 1) == 1);
        REQUIRE(d2 == HxSTL::deque<int>(137, 1));
    }

}

TEST_CASE("deque_member_insert_4") {

    SECTION("index < size / 2") {
        HxSTL::deque<int> d1({ 8, 9, 10, 11, 12, 13, 14, 15 });
        HxSTL::deque<int> d2({ 0, 1, 7 });
        HxSTL::deque<int> d3({ 2, 3, 4 });
        HxSTL::deque<int> d4({ 5, 6 });

        REQUIRE(*d1.insert(d1.begin(), d2.begin(), d2.end()) == 0);
        REQUIRE(*d1.insert(d1.begin() + 2, d3.begin(), d3.end()) == 2);
        REQUIRE(*d1.insert(d1.begin() + 5, d4.begin(), d4.end()) == 5);
        REQUIRE(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }));

        HxSTL::deque<int> d5(127, 1);
        HxSTL::deque<int> d6(10, 1);

        REQUIRE(*d5.insert(d5.begin(), d6.begin(), d6.end()) == 1);
        REQUIRE(d5 == HxSTL::deque<int>(137, 1));
    }

    SECTION("index >= size / 2") {
        HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7 });
        HxSTL::deque<int> d2({ 8, 14, 15 });
        HxSTL::deque<int> d3({ 11, 12, 13 });
        HxSTL::deque<int> d4({ 9, 10 });

        REQUIRE(*d1.insert(d1.end(), d2.begin(), d2.end()) == 8);
        REQUIRE(*d1.insert(d1.end() - 2, d3.begin(), d3.end()) == 11);
        REQUIRE(*d1.insert(d1.end() - 5, d4.begin(), d4.end()) == 9);
        REQUIRE(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }));

        HxSTL::deque<int> d5(127, 1);
        HxSTL::deque<int> d6(10, 1);

        REQUIRE(*d5.insert(d5.end(), d6.begin(), d6.end()) == 1);
        REQUIRE(d5 == HxSTL::deque<int>(137, 1));
    }

}

TEST_CASE("deque_member_erase_1") {

    HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    REQUIRE(*d1.erase(d1.begin() + 1) == 2);
    REQUIRE(*d1.erase(d1.begin() + 1) == 3);
    REQUIRE(*d1.erase(d1.begin()) == 3);
    REQUIRE(*d1.erase(d1.end() - 2) == 9);

    auto it = d1.erase(d1.end() - 1);

    REQUIRE(it == d1.end());
    REQUIRE(d1.erase(d1.end()) == d1.end());
    REQUIRE(d1 == HxSTL::deque<int>({ 3, 4, 5, 6, 7 }));

}

TEST_CASE("deque_member_erase_2") {

    HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    REQUIRE(*d1.erase(d1.begin(), d1.begin() + 2) == 2);
    REQUIRE(*d1.erase(d1.end() - 3, d1.end() - 1) == 9);
    REQUIRE(d1.erase(d1.end(), d1.end()) == d1.end());
    REQUIRE(d1 == HxSTL::deque<int>({ 2, 3, 4, 5, 6, 9 }));

    HxSTL::deque<int> d2(150, 1);

    d2.erase(d2.end() - 100, d2.end());

    REQUIRE(d2 == HxSTL::deque<int>(50, 1));

}
