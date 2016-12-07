#define CATCH_CONFIG_MAIN
#include <ctime>
#include <cstdlib>
#include "catch.hpp"
#include "list.h"

TEST_CASE("list_default_constructor") {

    HxSTL::list<int> l1;
    const HxSTL::list<int> l2;

    REQUIRE(l1.size() == 0);
    REQUIRE(l1.empty());
    REQUIRE(l2.size() == 0);
    REQUIRE(l2.empty());

}

TEST_CASE("list_fill_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(10);

    REQUIRE(l1.size() == 10);
    REQUIRE(l2.size() == 10);

    for (auto it = l1.begin(); it != l1.end(); ++it) {
        REQUIRE(*it == 10);
    }

    for (auto it = l2.begin(); it != l2.end(); ++it) {
        REQUIRE(*it == 0);
    }

}

TEST_CASE("list_range_constructor") {

    int a1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; 
    HxSTL::list<int> l1(a1, a1 + 10);

    REQUIRE(l1.size() == 10);
    REQUIRE(l1 == HxSTL::list<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

}

TEST_CASE("list_copy_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(l1);

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

}

TEST_CASE("list_move_constructor") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(HxSTL::move(l1));

    REQUIRE(l2.size() == 10);
    REQUIRE(l2 == HxSTL::list<int>(10, 10));

}

TEST_CASE("list_initializer_list_constructor") {

    int a1[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }; 
    HxSTL::list<int> l1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

    REQUIRE(l1 == HxSTL::list<int>(a1, a1 + 10));

}

TEST_CASE("list_copy_assignment") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(5, 5);

    l2 = l1;

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

    l2 = l2;

    REQUIRE(l2.size() == 10);
    REQUIRE(l1 == l2);

}

TEST_CASE("list_move_assignment") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(5, 5);

    l2 = HxSTL::move(l1);

    REQUIRE(l2.size() == 10);
    REQUIRE(l2 == HxSTL::list<int>(10, 10));

}

TEST_CASE("list_member_assign_1") {

    SECTION("count > size") {
        HxSTL::list<int> l1(5, 5);

        l1.assign(10, 10);

        REQUIRE(l1.size() == 10);
        REQUIRE(l1 == HxSTL::list<int>(10, 10));
    }

    SECTION("count <= size") {
        HxSTL::list<int> l1(10, 10);

        l1.assign(5, 5);

        REQUIRE(l1.size() == 5);
        REQUIRE(l1 == HxSTL::list<int>(5, 5));
    }

}

TEST_CASE("list_member_assign_2") {

    SECTION("last - first > size") {
        HxSTL::list<int> l1(10, 10);
        HxSTL::list<int> l2(5, 5);

        l2.assign(l1.begin(), l1.end());
        
        REQUIRE(l2.size() == 10);
        REQUIRE(l1 == l2);
    }

    SECTION("last - first <= size") {
        int a1[5] = { 5, 5, 5, 5, 5 };
        HxSTL::list<int> l1(10, 10);

        l1.assign(a1, a1 + 5);

        REQUIRE(l1.size() == 5);
        REQUIRE(l1 == HxSTL::list<int>(5, 5));
    }

}

TEST_CASE("list_member_front list_member_back") {

    SECTION("non-const") {
        HxSTL::list<int> l1({ 0, 4, 4, 1 });

        REQUIRE(l1.front() == 0);
        REQUIRE(l1.back() == 1);

        l1.front() = 4;
        l1.back() = 4;

        REQUIRE(l1.size() == 4);
        REQUIRE(l1 == HxSTL::list<int>(4, 4));
    }

    SECTION("const") {
        const HxSTL::list<int> l1({ 0, 1, 2, 3, 4 });

        REQUIRE(l1.front() == 0);
        REQUIRE(l1.back() == 4);
    }

}

TEST_CASE("list_member_begin list_member_end") {

    SECTION("non-const") {
        HxSTL::list<int> l1(5, 5);

        for (auto it = l1.begin(); it != l1.end(); ++it) {
            REQUIRE(*it == 5);
        }
    }

    SECTION("const") {
        const HxSTL::list<int> l1(5, 5);

        for (auto it = l1.begin(); it != l1.end(); ++it) {
            REQUIRE(*it == 5);
        }
    }

}

TEST_CASE("list_member_cbegin list_member_cend") {

    HxSTL::list<int> l1(5, 5);

    for (auto it = l1.cbegin(); it != l1.cend(); ++it) {
        REQUIRE(*it == 5);
    }

}

TEST_CASE("list_member_empty") {

    HxSTL::list<int> l1;
    HxSTL::list<int> l2(1, 1);

    REQUIRE(l1.empty());
    REQUIRE(!l2.empty());

}

TEST_CASE("list_member_size") {

    HxSTL::list<int> l1;
    HxSTL::list<int> l2(5, 5);

    REQUIRE(l1.size() == 0);
    REQUIRE(l2.size() == 5);

}

TEST_CASE("list_member_clear") {

    HxSTL::list<int> l1(5, 5);

    l1.clear();

    REQUIRE(l1.empty());

}

TEST_CASE("list_member_insert_1 list_member_insert_2") {

    HxSTL::list<int> l1(10, 10);
    auto it = l1.begin();
    int i1 = 10;

    HxSTL::advance(it, 5);
    l1.insert(it, i1);
    HxSTL::advance(it, 5);
    l1.insert(l1.begin(), HxSTL::move(i1));

    REQUIRE(l1.size() == 12);
    REQUIRE(l1 == HxSTL::list<int>(12, 10));
    REQUIRE(*(l1.insert(it, 5)) == 5);

}

TEST_CASE("list_member_insert_3") {

    HxSTL::list<int> l1(10, 10);

    l1.insert(l1.begin(), 2, 10);

    REQUIRE(l1 == HxSTL::list<int>(12, 10));
    REQUIRE(*(l1.insert(l1.begin(), 5, 5)) == 5);

}

TEST_CASE("list_member_insert_4") {

    int a1[5] = { 0, 1, 2, 3, 4 };
    HxSTL::list<int> l1(a1 + 3, a1 + 5);

    REQUIRE(*(l1.insert(l1.begin(), a1, a1 + 3)) == 0);
    REQUIRE(l1 == HxSTL::list<int>(a1, a1 + 5));

}

TEST_CASE("list_member_emplace") {

    class A {
    public:
        A(int i1, int i0): b0(i0), b1(i1) {}
        operator int() { return (b1 << 1) + b0; }
    private:
        int b0, b1;
    };

    HxSTL::list<A> l1;

    l1.emplace(l1.begin(), 1, 1);
    l1.emplace(l1.begin(), 1, 0);
    l1.emplace(l1.begin(), 0, 1);
    l1.emplace(l1.begin(), 0, 0);

    REQUIRE(l1.size() == 4);
    REQUIRE(HxSTL::list<int>(l1.begin(), l1.end()) == HxSTL::list<int>({ 0, 1, 2, 3 }));

}

TEST_CASE("list_member_erase_1") {

    SECTION("pos == end()") {
        HxSTL::list<int> l1(5, 5);

        REQUIRE(l1.erase(l1.end()) == l1.end());
        REQUIRE(l1.size() == 5);
    }

    SECTION("pos != end()") {
        HxSTL::list<int> l1({ 0, 1, 2, 3, 4 });

        REQUIRE(*l1.erase(++l1.begin()) == 2);
        REQUIRE(l1 == HxSTL::list<int>({ 0, 2, 3, 4 }));
    }

}

TEST_CASE("list_member_erase_2") {
    
    SECTION("first == last") {
        HxSTL::list<int> l1(5, 5);

        REQUIRE(l1.erase(l1.end(), l1.end()) == l1.end());
        REQUIRE(l1.size() == 5);
    }

    SECTION("first != last") {
        HxSTL::list<int> l1({ 0, 1, 2, 3, 4 });

        REQUIRE(*l1.erase(l1.begin(), --l1.end()) == 4);
        REQUIRE(l1 == HxSTL::list<int>({ 4 }));
    }

}

TEST_CASE("list_member_push_back") {

    HxSTL::list<int> l1({ 0, 1, 2, 3, 4 });
    int i1 = 5;

    l1.push_back(i1);
    l1.push_back(HxSTL::move(i1));

    REQUIRE(l1 == HxSTL::list<int>({ 0, 1, 2, 3, 4, 5, 5 }));

}

TEST_CASE("list_member_emplace_back list_member_emplace_front") {

    HxSTL::list<HxSTL::list<int>> l1;

    l1.emplace_back(5, 5);

    REQUIRE(l1.front() == HxSTL::list<int>(5, 5));

    l1.emplace_front(10, 10);

    REQUIRE(l1.size() == 2);
    REQUIRE(l1.front() == HxSTL::list<int>(10, 10));

}

TEST_CASE("list_member_push_front") {

    HxSTL::list<int> l1({ 1, 2, 3, 4, 5 });
    int i1 = 0;

    l1.push_front(i1);
    l1.push_front(HxSTL::move(i1));

    REQUIRE(l1 == HxSTL::list<int>({ 0, 0, 1, 2, 3, 4, 5 }));

}

TEST_CASE("list_member_pop_back list_member_pop_front") {

    HxSTL::list<int> l1({ 0, 1, 2, 3, 4 });

    l1.pop_back();
    l1.pop_back();

    REQUIRE(l1 == HxSTL::list<int>({ 0, 1, 2 }));

    l1.pop_front();

    REQUIRE(l1 == HxSTL::list<int>({ 1, 2 }));

}

TEST_CASE("list_member_resize") {
    
    SECTION("count > size") {
        HxSTL::list<int> l1(2, 2);

        l1.resize(5);

        REQUIRE(l1 == HxSTL::list<int>({ 2, 2, 0, 0, 0 }));

        l1.resize(8, 8);

        REQUIRE(l1 == HxSTL::list<int>({ 2, 2, 0, 0, 0, 8, 8, 8 }));
    }

    SECTION("count <= size") {
        HxSTL::list<int> l1(10, 10);

        l1.resize(5);

        REQUIRE(l1 == HxSTL::list<int>(5, 10));
    }

}

TEST_CASE("list_member_swap") {

    HxSTL::list<int> l1(10, 10);
    HxSTL::list<int> l2(5, 5);

    l1.swap(l2);

    REQUIRE(l1 == HxSTL::list<int>(5, 5));
    REQUIRE(l2 == HxSTL::list<int>(10, 10));

    l2.swap(l1);

    REQUIRE(l1 == HxSTL::list<int>(10, 10));
    REQUIRE(l2 == HxSTL::list<int>(5, 5));

}

TEST_CASE("list_member_merge") {

    HxSTL::list<int> l1({ 1, 3, 5 });
    HxSTL::list<int> l2({ 0, 2, 4 });

    l1.merge(l2);

    REQUIRE(l2.empty());
    REQUIRE(l1 == HxSTL::list<int>({ 0, 1, 2, 3, 4, 5 }));

}

TEST_CASE("list_member_remove") {

    HxSTL::list<int> l1;

    srand((unsigned) time(NULL));

    for (int i = 0; i != 100000; ++i) {
        l1.push_back(rand() % 10);
    }

    l1.remove(5);

    REQUIRE(HxSTL::find(l1.begin(), l1.end(), 5) == l1.end());

}

TEST_CASE("list_member_reverse") {

    HxSTL::list<int> l1;
    HxSTL::list<int> l2(1, 1);
    HxSTL::list<int> l3({ 0, 1, 2, 3, 4 });

    l1.reverse();
    l2.reverse();
    l3.reverse();

    REQUIRE(l1.size() == 0);
    REQUIRE(l2.size() == 1);
    REQUIRE(l3 == HxSTL::list<int>({ 4, 3, 2, 1, 0 }));

}

TEST_CASE("list_member_unique") {

    HxSTL::list<int> l1({ 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 4, 5, 1, 1 });

    l1.unique();

    REQUIRE(l1 == HxSTL::list<int>({ 0, 1, 2, 3, 4, 5, 1 }));

}

TEST_CASE("list_member_sort") {

    HxSTL::list<int> l1;

    srand((unsigned) time(NULL));
    
    for (int i = 0; i != 100000; ++i) {
        l1.push_back(rand() % 100000);
    }

    l1.sort();

    REQUIRE(HxSTL::is_sorted(l1.begin(), l1.end()));

}
