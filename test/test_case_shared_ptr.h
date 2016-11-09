#include "catch.hpp"
#include "allocator.h"
#include "shared_ptr.h"

struct Base {};
struct Derived: public Base {
    int n1, n2;
    Derived() = default;
    Derived(int a, int b): n1(a), n2(b) {}
};

TEST_CASE("shared_ptr constructor") {

    SECTION("default constructor") {
        HxSTL::shared_ptr<int> sp1;

        REQUIRE(sp1 == nullptr);
        REQUIRE(sp1.use_count() == 0);
    }

    SECTION("constructor with nullptr") {
        HxSTL::shared_ptr<int> sp1(nullptr);

        REQUIRE(sp1 == nullptr);
        REQUIRE(sp1.use_count() == 0);
    }

    SECTION("constructor with pointer") {
        HxSTL::shared_ptr<int> sp1(new int);

        REQUIRE(sp1.use_count() == 1);
    }

    SECTION("constructor with pointer and deleter") {
        int* p1 = new int(100);
        HxSTL::shared_ptr<int> sp1(p1, [](void*) {});

        REQUIRE(sp1.use_count() == 1);

        sp1.reset();

        REQUIRE(*p1 == 100);
    }

    SECTION("constructor with pointer, deleter and allocator") {
        HxSTL::shared_ptr<int> sp1(new int, [](void*) {}, HxSTL::allocator<int>());

        REQUIRE(sp1.use_count() == 1);
    }

    SECTION("aliasing constructor") {
        // same to owner_before
    }

    SECTION("copy constructor") {
        HxSTL::shared_ptr<Derived> sp1(new Derived);
        HxSTL::shared_ptr<Derived> sp2(sp1);

        REQUIRE(sp1 == sp2);
        REQUIRE(sp1.use_count() == 2);
        REQUIRE(sp2.use_count() == 2);
        REQUIRE(!sp1.unique());
        REQUIRE(!sp2.unique());

        HxSTL::shared_ptr<Base> sp3(sp2);

        REQUIRE(sp2 == sp3);
        REQUIRE(sp1.use_count() == 3);
        REQUIRE(sp2.use_count() == 3);
        REQUIRE(sp3.use_count() == 3);
    }

    SECTION("copy constructor with weak_ptr") {
        HxSTL::shared_ptr<int> sp1(new int);
        HxSTL::weak_ptr<int> wp1(sp1);
        HxSTL::shared_ptr<int> sp2(wp1);

        REQUIRE(sp1 == sp2);
        REQUIRE(sp1.use_count() == 2);
        REQUIRE(sp2.use_count() == 2);
        REQUIRE(wp1.use_count() == 2);
    }

    SECTION("move constructor") {
        HxSTL::shared_ptr<Derived> sp1(new Derived);
        HxSTL::shared_ptr<Derived> sp2(HxSTL::move(sp1));

        REQUIRE(sp1 == nullptr);
        REQUIRE(sp2.use_count() == 1);

        HxSTL::shared_ptr<Base> sp3(HxSTL::move(sp2));

        REQUIRE(sp2 == nullptr);
        REQUIRE(sp3.use_count() == 1);
    }

    SECTION("move constructor with unique_ptr") {
    }

}

TEST_CASE("shared_ptr assignment") {

    SECTION("copy assignment") {
        HxSTL::shared_ptr<Derived> sp1(new Derived);
        HxSTL::shared_ptr<Derived> sp2(new Derived);
        sp2 = sp1;

        REQUIRE(sp1 == sp2);
        REQUIRE(sp1.use_count() == 2);
        REQUIRE(sp2.use_count() == 2);
        REQUIRE(!sp1.unique());
        REQUIRE(!sp2.unique());

        HxSTL::shared_ptr<Base> sp3(new Base);
        sp3 = sp2;

        REQUIRE(sp2 == sp3);
        REQUIRE(sp1.use_count() == 3);
        REQUIRE(sp2.use_count() == 3);
        REQUIRE(sp3.use_count() == 3);
    }

    SECTION("move assignment") {
        HxSTL::shared_ptr<Derived> sp1(new Derived);
        HxSTL::shared_ptr<Derived> sp2(new Derived);
        sp2 = HxSTL::move(sp1);

        REQUIRE(sp1 == nullptr);
        REQUIRE(sp2.use_count() == 1);

        HxSTL::shared_ptr<Base> sp3(new Base);
        sp3 = HxSTL::move(sp2);

        REQUIRE(sp2 == nullptr);
        REQUIRE(sp3.use_count() == 1);
    }

    SECTION("move assignment with unique_ptr") {
    }

}

TEST_CASE("shared_ptr member operator") {

    SECTION("operator bool") {
        HxSTL::shared_ptr<int> sp1;
        HxSTL::shared_ptr<int> sp2(new int);

        REQUIRE(!sp1);
        REQUIRE((bool) sp2);
    }

    SECTION("operator*") {
        HxSTL::shared_ptr<int> sp1(new int(10));

        REQUIRE(*sp1 == 10);
    }

    SECTION("operator->") {
        HxSTL::shared_ptr<Derived> sp1(new Derived(100, 200));

        REQUIRE(sp1 -> n1 == 100);
        REQUIRE(sp1 -> n2 == 200);
    }

    SECTION("operator[]") {
        HxSTL::shared_ptr<int> sp1(new int[5]{ 1, 2, 3, 4, 5 });

        REQUIRE(sp1[0] == 1);
        REQUIRE(sp1[1] == 2);
        REQUIRE(sp1[2] == 3);
        REQUIRE(sp1[3] == 4);
        REQUIRE(sp1[4] == 5);
    }

}

TEST_CASE("shared_ptr member funciton") {

    SECTION("reset") {
        HxSTL::shared_ptr<int> sp1(new int);

        REQUIRE(sp1 != nullptr);

        sp1.reset();

        REQUIRE(sp1 == nullptr);
    }

    SECTION("reset with pointer") {
        HxSTL::shared_ptr<int> sp1(new int);
        sp1.reset(new int);

        REQUIRE(sp1 != nullptr);
        REQUIRE(sp1.use_count() == 1);
    }

    SECTION("swap") {
        HxSTL::shared_ptr<int> sp1(new int(1));
        HxSTL::shared_ptr<int> sp2(new int(2));

        REQUIRE(*sp1 == 1);
        REQUIRE(*sp2 == 2);

        sp1.swap(sp2);

        REQUIRE(*sp1 == 2);
        REQUIRE(*sp2 == 1);
        REQUIRE(sp1.use_count() == 1);
        REQUIRE(sp2.use_count() == 1);
    }

    SECTION("get") {
        int* p1 = new int;
        HxSTL::shared_ptr<int> sp1(p1);

        REQUIRE(p1 == sp1.get());
    }

    SECTION("use_count") {
        HxSTL::shared_ptr<int> sp1(new int);
        HxSTL::shared_ptr<int> sp2(sp1);
        HxSTL::shared_ptr<int> sp3(sp2);

        REQUIRE(sp1.use_count() == 3);
    }

    SECTION("unique") {
        HxSTL::shared_ptr<int> sp1(new int);

        REQUIRE(sp1.unique());

        HxSTL::shared_ptr<int> sp2(sp1);

        REQUIRE(!sp1.unique());
    }

    SECTION("owner_before") {
        HxSTL::shared_ptr<Derived> sp1(new Derived(1, 2));
        HxSTL::shared_ptr<int> sp2(sp1, &sp1->n1);
        HxSTL::shared_ptr<int> sp3(sp1, &sp1->n2);

        REQUIRE(sp2 < sp3);
        REQUIRE(!(sp3 < sp2));
        REQUIRE(!(sp2.owner_before(sp3)));
        REQUIRE(!(sp3.owner_before(sp2)));

        HxSTL::weak_ptr<int> wp1(sp2);
        HxSTL::weak_ptr<int> wp2(sp3);

        REQUIRE(!(wp1.owner_before(wp2)));
        REQUIRE(!(wp2.owner_before(wp1)));
    }

}

TEST_CASE("shared_ptr non-member operator") {

    SECTION("relational operators") {
        HxSTL::shared_ptr<int> sp1(new int);
        HxSTL::shared_ptr<int> sp2(sp1);
        HxSTL::shared_ptr<int> sp3(new int);
        HxSTL::shared_ptr<int> sp4(nullptr);

        REQUIRE(sp1 == sp2);
        REQUIRE(sp1 != sp3);
        REQUIRE(sp4 == nullptr);
        REQUIRE(sp1 != nullptr);
    }

    SECTION("operator<<") {
    }

}

TEST_CASE("shared_ptr non-member function") {

    SECTION("make_shared") {
        auto sp1 = HxSTL::make_shared<int>();
        auto sp2 = HxSTL::make_shared<int>(100);
        auto sp3 = HxSTL::make_shared<Derived>(200, 300);

        REQUIRE(sp1.use_count() == 1);
        REQUIRE(*sp2 == 100);
        REQUIRE(sp3 -> n1 == 200);
        REQUIRE(sp3 -> n2 == 300);
    }

    SECTION("allocate_shared") {
        auto sp1 = HxSTL::allocate_shared<int>(HxSTL::allocator<int>(), 100);
        auto sp2 = HxSTL::allocate_shared<Derived>(HxSTL::allocator<Derived>(), 200, 300);

        REQUIRE(*sp1 == 100);
        REQUIRE(sp2 -> n1 == 200);
        REQUIRE(sp2 -> n2 == 300);
    }

    SECTION("swap") {
        HxSTL::shared_ptr<int> sp1(new int(1));
        HxSTL::shared_ptr<int> sp2(new int(2));

        REQUIRE(*sp1 == 1);
        REQUIRE(*sp2 == 2);

        HxSTL::swap(sp1, sp2);

        REQUIRE(*sp1 == 2);
        REQUIRE(*sp2 == 1);
    }

}
