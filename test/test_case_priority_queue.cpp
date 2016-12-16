#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "priority_queue.h"

TEST_CASE("priority_queue_default_constructor") {

    HxSTL::priority_queue<int> q1;
    HxSTL::vector<int> v1({ 4, 5, 3, 0, 1, 2, 8, 9, 7, 6 });
    HxSTL::priority_queue<int> q2(HxSTL::less<int>(), v1);

    REQUIRE(q1.size() == 0);
    REQUIRE(q2.size() == 10);

}

TEST_CASE("priority_queue_range_constructor") {
}

TEST_CASE("priority_queue_copy_constructor") {
}

TEST_CASE("priority_queue_move_constructor") {
}

TEST_CASE("priority_queue_member_top") {

    HxSTL::vector<int> v1({ 4, 5, 3, 0, 1, 2, 8, 9, 7, 6 });
    HxSTL::priority_queue<int> q1(HxSTL::less<int>(), v1);
    HxSTL::priority_queue<int, HxSTL::vector<int>, HxSTL::greater<int>> q2(HxSTL::greater<int>(), v1);

    REQUIRE(q1.top() == 9);
    REQUIRE(q2.top() == 0);

}

TEST_CASE("priority_queue_member_empty") {

    HxSTL::priority_queue<int> q1;
    HxSTL::vector<int> v1({ 4, 5, 3, 0, 1, 2, 8, 9, 7, 6 });
    HxSTL::priority_queue<int> q2(HxSTL::less<int>(), v1);

    REQUIRE(q1.empty());
    REQUIRE(!q2.empty());

}

TEST_CASE("priority_queue_member_push") {

    HxSTL::priority_queue<int> q1;

    q1.push(0);
    q1.push(2);
    q1.push(3);
    q1.push(1);

    REQUIRE(q1.size() == 4);
    REQUIRE(q1.top() == 3);

}

TEST_CASE("priority_queue_member_emplace") {

    HxSTL::priority_queue<int> q1;

    q1.emplace(0);
    q1.emplace(2);
    q1.emplace(3);
    q1.emplace(1);

    REQUIRE(q1.size() == 4);
    REQUIRE(q1.top() == 3);

}

TEST_CASE("priority_queue_member_pop") {

    HxSTL::vector<int> v1({ 4, 5, 3, 0, 1, 2, 8, 9, 7, 6 });
    HxSTL::priority_queue<int> q1(HxSTL::less<int>(), v1);

    q1.pop();
    q1.pop();
    q1.pop();

    REQUIRE(q1.size() == 7);
    REQUIRE(q1.top() == 6);

}
