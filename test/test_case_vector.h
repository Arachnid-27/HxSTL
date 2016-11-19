#include "catch.hpp"
#include "vector.h"

TEST_CASE("vector_default_constructor") {

    HxSTL::vector<int> v1;

    REQUIRE(v1.size() == 0);
    REQUIRE(v1.capacity() == 0);
    REQUIRE(v1.begin() == nullptr);
    REQUIRE(v1.end() == nullptr);

}

TEST_CASE("vector_fill_constructor") {

    HxSTL::vector<int> v1(5, 5);

    REQUIRE(v1.size() == 5);
    REQUIRE(v1.capacity() == 5);
    REQUIRE(v1.front() == 5);
    REQUIRE(v1.back() == 5);

}

TEST_CASE("vector_range_constructor") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1(a1, a1 + 10);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.capacity() == 10);
    REQUIRE(v1.front() == a1[0]);
    REQUIRE(v1.back() == a1[9]);

}

TEST_CASE("vector_copy_constructor") {

    HxSTL::vector<int> v1(5, 5);

    v1.push_back(6);

    HxSTL::vector<int> v2(v1);

    REQUIRE(v2.size() == 6);
    REQUIRE(v2.capacity() == 6);
    REQUIRE(v2.front() == v1.front());
    REQUIRE(v2.back() == v2.back());

}

TEST_CASE("vector_move_constructor") {

    HxSTL::vector<int> v1(5, 5);

    v1.push_back(6);

    int c1 = v1.capacity();
    HxSTL::vector<int> v2(HxSTL::move(v1));

    REQUIRE(v1.size() == 0);
    REQUIRE(v1.capacity() == 0);
    REQUIRE(v1.begin() == nullptr);
    REQUIRE(v1.end() == nullptr);
    REQUIRE(v2.size() == 6);
    REQUIRE(v2.capacity() == c1);
    REQUIRE(v2.front() == 5);
    REQUIRE(v2.back() == 6);

}

TEST_CASE("vector_copy_assignment") {

    HxSTL::vector<int> v1(5, 5);
    HxSTL::vector<int> v2(10);

    v2 = v1;

    REQUIRE(v2.size() == 5);
    REQUIRE(v2.capacity() == 10);
    REQUIRE(v2.front() == v1.front());
    REQUIRE(v2.back() == v1.back());

}

TEST_CASE("vector_move_assignment") {

    HxSTL::vector<int> v1(5, 5);
    HxSTL::vector<int> v2(10);

    v2 = HxSTL::move(v1);

    REQUIRE(v1.size() == 0);
    REQUIRE(v1.capacity() == 0);
    REQUIRE(v1.begin() == nullptr);
    REQUIRE(v1.end() == nullptr);
    REQUIRE(v2.size() == 5);
    REQUIRE(v2.capacity() == 5);
    REQUIRE(v2.front() == 5);
    REQUIRE(v2.back() == 5);

}

TEST_CASE("vector_member_assign_1") {

    HxSTL::vector<int> v1(10);

    v1.assign(5, 5);

    REQUIRE(v1.size() == 5);
    REQUIRE(v1.capacity() == 10);
    REQUIRE(v1.front() == 5);
    REQUIRE(v1.back() == 5);

}

TEST_CASE("vector_member_assign_2") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1(5);

    v1.assign(a1, a1 + 10);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.capacity() == 10);
    REQUIRE(v1.front() == a1[0]);
    REQUIRE(v1.back() == a1[9]);

}

TEST_CASE("vector_member_at") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1(a1, a1 + 10);

    for (int i = 0; i != 10; ++i) {
        REQUIRE(v1.at(i) == a1[i]);
    }

    bool except = false;

    try {
        v1.at(11);
    } catch (HxSTL::out_of_range) {
        except = true;
    }

    REQUIRE(except);

};

TEST_CASE("vector_member_operator[]") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1(a1, a1 + 10);

    for (int i = 0; i != 10; ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

    bool except = false;

    try {
        int i1 = v1[11];
    } catch (HxSTL::out_of_range) {
        except = true;
    }

    REQUIRE(!except);

};

TEST_CASE("vector_member_front") {

    HxSTL::vector<int> v1(5, 5);

    REQUIRE(v1.back() == 5);

}

TEST_CASE("vector_member_back") {

    HxSTL::vector<int> v1(5, 5);

    REQUIRE(v1.back() == 5);

}

TEST_CASE("vector_member_data") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1(a1, a1 + 10);

    int* p1 = v1.data();
    for (int i = 0; i != 10; ++i) {
        REQUIRE(*(p1 + i) == a1[i]);
    }

}

TEST_CASE("vector_member_begin vector_member_cbegin") {

    HxSTL::vector<int> v1(5, 5);

    REQUIRE(*v1.begin() == 5);
    REQUIRE(*v1.cbegin() == 5);

}

TEST_CASE("vector_member_end vector_member_cend") {

    HxSTL::vector<int> v1(5, 5);

    REQUIRE(*(v1.end() - 1) == 5);
    REQUIRE(*(v1.cend() - 1) == 5);

}

TEST_CASE("vector_member_empty") {

    HxSTL::vector<int> v1(5, 5);
    HxSTL::vector<int> v2;

    REQUIRE(!v1.empty());
    REQUIRE(v2.empty());

}

TEST_CASE("vector_member_size") {

    HxSTL::vector<int> v1(5, 5);
    HxSTL::vector<int> v2;

    REQUIRE(v1.size() == 5);
    REQUIRE(v2.size() == 0);

}

TEST_CASE("vector_member_reserve") {

    HxSTL::vector<int> v1(10);

    v1.reserve(5);

    REQUIRE(v1.capacity() == 10);

    v1.reserve(20);

    REQUIRE(v1.capacity() == 20);

}

TEST_CASE("vector_member_capacity") {

    HxSTL::vector<int> v1(10);

    REQUIRE(v1.capacity() == 10);

}

TEST_CASE("vector_member_shrink_to_fit") {

    HxSTL::vector<int> v1(10);

    v1.reserve(20);

    REQUIRE(v1.capacity() == 20);

    v1.shrink_to_fit();

    REQUIRE(v1.capacity() == 10);

}

