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

    v1.reserve(20);

    v1.assign(10, 10);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.front() == 10);
    REQUIRE(v1.back() == 10);

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

TEST_CASE("vector_member_clear") {

    HxSTL::vector<int> v1(10);

    REQUIRE(v1.size() == 10);

    v1.clear();

    REQUIRE(v1.empty());

}

TEST_CASE("vector_member_insert_1") {

    int a1[4] = { 1, 2, 3, 4 };
    HxSTL::vector<int> v1;
    HxSTL::vector<int>::iterator it1;
    
    it1 = v1.insert(v1.begin(), a1[0]);

    REQUIRE(*it1 == 1);

    it1 = v1.insert(v1.end(), a1[2]);
    it1 = v1.insert(v1.end(), a1[3]);

    REQUIRE(*it1 == 4);

    v1.insert(v1.begin() + 1, a1[1]);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_insert_2") {

    int a1[4] = { 1, 2, 3, 4 };
    HxSTL::vector<int> v1;
    HxSTL::vector<int>::iterator it1;
    
    it1 = v1.insert(v1.begin(), 1);

    REQUIRE(*it1 == 1);

    it1 = v1.insert(v1.end(), 3);
    it1 = v1.insert(v1.end(), 4);

    REQUIRE(*it1 == 4);

    v1.insert(v1.begin() + 1, 2);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_insert_3") {

    int a1[7] = { 1, 1, 2, 2, 2, 3, 3 };
    HxSTL::vector<int> v1;
    HxSTL::vector<int>::iterator it1;
    
    it1 = v1.insert(v1.begin(), 2, 1);

    REQUIRE(*it1 == 1);
    REQUIRE(*(it1 + 1) == 1);

    it1 = v1.insert(v1.end(), 2, 3);

    REQUIRE(*it1 == 3);
    REQUIRE(*(it1 + 1) == 3);

    v1.reserve(10);
    v1.insert(v1.begin() + 2, 3, 2);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_insert_4") {

    int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    HxSTL::vector<int> v1;
    HxSTL::vector<int>::iterator it1;
    
    it1 = v1.insert(v1.begin(), a1, a1 + 2);

    REQUIRE(*it1 == 1);
    REQUIRE(*(it1 + 1) == 2);

    it1 = v1.insert(v1.end(), a1 + 7, a1 + 10);

    REQUIRE(*it1 == 8);
    REQUIRE(*(it1 + 1) == 9);
    REQUIRE(*(it1 + 2) == 10);

    v1.reserve(20);
    it1 = v1.insert(v1.begin() + 2, a1 + 2, a1 + 3);

    REQUIRE(*it1 == 3);

    v1.insert(v1.begin() + 3, a1 + 3, a1 + 7);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_emplace") {

    int a1[4] = { 1, 2, 3, 4 };
    HxSTL::vector<int> v1;
    HxSTL::vector<int>::iterator it1;
    
    it1 = v1.emplace(v1.begin(), a1[0]);

    REQUIRE(*it1 == 1);

    it1 = v1.emplace(v1.end(), a1[2]);
    it1 = v1.emplace(v1.end(), a1[3]);

    REQUIRE(*it1 == 4);

    v1.emplace(v1.begin() + 1, a1[1]);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_erase_1") {

    int a1[5] = { 1, 2, 3, 4, 5 };
    HxSTL::vector<int> v1(a1, a1 + 5);
    HxSTL::vector<int>::iterator it1;

    it1 = v1.erase(v1.begin());

    REQUIRE(v1.size() == 4);
    REQUIRE(*it1 == 2);

    it1 = v1.erase(v1.end() - 1);
    REQUIRE(v1.size() == 3);
    REQUIRE(it1 == v1.end());

}

TEST_CASE("vector_member_erase_2") {

    int a1[5] = { 1, 2, 3, 4, 5 };
    HxSTL::vector<int> v1(a1, a1 + 5);
    HxSTL::vector<int>::iterator it1;

    it1 = v1.erase(v1.begin(), v1.begin() + 2);

    REQUIRE(v1.size() == 3);
    REQUIRE(*it1 == 3);

    it1 = v1.erase(v1.begin(), v1.end());
    REQUIRE(v1.size() == 0);
    REQUIRE(it1 == v1.end());

}

TEST_CASE("vector_member_push_back_1") {

    int a1[5] = { 1, 2, 3, 4, 5 };
    HxSTL::vector<int> v1;

    for (int i = 0; i != 5; ++i) {
        v1.push_back(a1[i]);
    }

    REQUIRE(v1.size() == 5);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_push_back_2") {

    int a1[5] = { 1, 2, 3, 4, 5 };
    int a2[5] = { 1, 2, 3, 4, 5 };
    HxSTL::vector<int> v1;

    for (int i = 0; i != 5; ++i) {
        v1.push_back(HxSTL::move(a1[i]));
    }

    REQUIRE(v1.size() == 5);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a2[i]);
    }

}

TEST_CASE("vector_member_emplace_back") {

    class A {
    public:
        A(int i1, int i0): b0(i0), b1(i1) {}
        int operator()() { return (b1 << 1) + b0; }
    private:
        int b0, b1;
    };

    HxSTL::vector<A> v1;

    v1.emplace_back(0, 0);
    v1.emplace_back(0, 1);
    v1.emplace_back(1, 0);
    v1.emplace_back(1, 1);

    REQUIRE(v1.size() == 4);

    for (HxSTL::vector<A>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i]() == i);
    }

}

TEST_CASE("vector_member_pop_back") {

    int a1[5] = { 1, 2, 3, 4, 5 };
    HxSTL::vector<int> v1(a1, a1 + 5);

    v1.pop_back();
    v1.pop_back();

    REQUIRE(v1.size() == 3);

    for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
        REQUIRE(v1[i] == a1[i]);
    }

}

TEST_CASE("vector_member_resize_1") {

    HxSTL::vector<int> v1(10, 10);
    auto c1 = v1.capacity();

    v1.resize(5);

    REQUIRE(v1.size() == 5);
    REQUIRE(v1.capacity() == c1);

    v1.resize(10);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.capacity() == c1);
    REQUIRE(v1.back() == 0);

    v1.resize(20);

    REQUIRE(v1.size() == 20);
    REQUIRE(v1.front() == 10);
    REQUIRE(v1.back() == 0);

}

TEST_CASE("vector_member_resize_2") {

    HxSTL::vector<int> v1(10, 10);
    auto c1 = v1.capacity();

    v1.resize(5, 5);

    REQUIRE(v1.size() == 5);
    REQUIRE(v1.capacity() == c1);

    v1.resize(10, 5);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.capacity() == c1);
    REQUIRE(v1.back() == 5);

    v1.resize(20, 20);

    REQUIRE(v1.size() == 20);
    REQUIRE(v1.front() == 10);
    REQUIRE(v1.back() == 20);

}

TEST_CASE("vector_member_swap") {

    HxSTL::vector<int> v1(10);
    HxSTL::vector<int> v2(20);

    REQUIRE(v1.size() == 10);
    REQUIRE(v1.capacity() == 10);
    REQUIRE(v2.size() == 20);
    REQUIRE(v2.capacity() == 20);

    v1.swap(v2);

    REQUIRE(v1.size() == 20);
    REQUIRE(v1.capacity() == 20);
    REQUIRE(v2.size() == 10);
    REQUIRE(v2.capacity() == 10);

}
