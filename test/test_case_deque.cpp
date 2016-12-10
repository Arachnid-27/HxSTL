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

TEST_CASE('deque_member_insert_3') {
}
