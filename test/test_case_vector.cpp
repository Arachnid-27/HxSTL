#include <cstdio>
#include <cassert>
#include "vector.h"

int main() {

    { // member
        { // default constructor

            HxSTL::vector<int> v1;

            assert(v1.size() == 0);
            assert(v1.capacity() == 0);
            assert(v1.begin() == nullptr);
            assert(v1.end() == nullptr);

        }

        { // fill constructor

            HxSTL::vector<int> v1(5, 5);

            assert(v1.size() == 5);
            assert(v1.capacity() == 5);
            assert(v1.front() == 5);
            assert(v1.back() == 5);

        }

        { // range constructor

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1(a1, a1 + 10);

            assert(v1.size() == 10);
            assert(v1.capacity() == 10);
            assert(v1.front() == a1[0]);
            assert(v1.back() == a1[9]);

        }

        { // copy constructor

            HxSTL::vector<int> v1(5, 5);

            v1.push_back(6);

            HxSTL::vector<int> v2(v1);

            assert(v2.size() == 6);
            assert(v2.capacity() == 6);
            assert(v2.front() == v1.front());
            assert(v2.back() == v2.back());

        }

        { // move constructor

            HxSTL::vector<int> v1(5, 5);

            v1.push_back(6);

            int c1 = v1.capacity();
            HxSTL::vector<int> v2(HxSTL::move(v1));

            assert(v1.size() == 0);
            assert(v1.capacity() == 0);
            assert(v1.begin() == nullptr);
            assert(v1.end() == nullptr);
            assert(v2.size() == 6);
            assert(v2.capacity() == c1);
            assert(v2.front() == 5);
            assert(v2.back() == 6);

        }

        { // copy assignment

            HxSTL::vector<int> v1(5, 5);
            HxSTL::vector<int> v2(10);

            v2 = v1;

            assert(v2.size() == 5);
            assert(v2.capacity() == 10);
            assert(v2.front() == v1.front());
            assert(v2.back() == v1.back());

        }

        { // move assignment

            HxSTL::vector<int> v1(5, 5);
            HxSTL::vector<int> v2(10);

            v2 = HxSTL::move(v1);

            assert(v1.size() == 0);
            assert(v1.capacity() == 0);
            assert(v1.begin() == nullptr);
            assert(v1.end() == nullptr);
            assert(v2.size() == 5);
            assert(v2.capacity() == 5);
            assert(v2.front() == 5);
            assert(v2.back() == 5);

        }

        { // assign 1

            HxSTL::vector<int> v1(10);

            v1.assign(5, 5);

            assert(v1.size() == 5);
            assert(v1.capacity() == 10);
            assert(v1.front() == 5);
            assert(v1.back() == 5);

            v1.reserve(20);

            v1.assign(10, 10);

            assert(v1.size() == 10);
            assert(v1.front() == 10);
            assert(v1.back() == 10);

        }

        { // assign 2

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1(5);

            v1.assign(a1, a1 + 10);

            assert(v1.size() == 10);
            assert(v1.capacity() == 10);
            assert(v1.front() == a1[0]);
            assert(v1.back() == a1[9]);

        }

        { // at

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1(a1, a1 + 10);

            for (int i = 0; i != 10; ++i) {
                assert(v1.at(i) == a1[i]);
            }

            bool except = false;

            try {
                v1.at(11);
            } catch (HxSTL::out_of_range) {
                except = true;
            }

            assert(except);

        }

        { // operator[]

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1(a1, a1 + 10);

            for (int i = 0; i != 10; ++i) {
                assert(v1[i] == a1[i]);
            }

            bool except = false;

            try {
                int i1 = v1[11];
            } catch (HxSTL::out_of_range) {
                except = true;
            }

            assert(!except);

        }

        { // front

            HxSTL::vector<int> v1(5, 5);

            assert(v1.back() == 5);

        }

        { // back

            HxSTL::vector<int> v1(5, 5);

            assert(v1.back() == 5);

        }

        { // data

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1(a1, a1 + 10);

            int* p1 = v1.data();
            for (int i = 0; i != 10; ++i) {
                assert(*(p1 + i) == a1[i]);
            }

        }

        { // begin cbegin

            HxSTL::vector<int> v1(5, 5);

            assert(*v1.begin() == 5);
            assert(*v1.cbegin() == 5);

        }

        { // end cend

            HxSTL::vector<int> v1(5, 5);

            assert(*(v1.end() - 1) == 5);
            assert(*(v1.cend() - 1) == 5);

        }

        { // empty

            HxSTL::vector<int> v1(5, 5);
            HxSTL::vector<int> v2;

            assert(!v1.empty());
            assert(v2.empty());

        }

        { // size

            HxSTL::vector<int> v1(5, 5);
            HxSTL::vector<int> v2;

            assert(v1.size() == 5);
            assert(v2.size() == 0);

        }

        { // reserve

            HxSTL::vector<int> v1(10);

            v1.reserve(5);

            assert(v1.capacity() == 10);

            v1.reserve(20);

            assert(v1.capacity() == 20);

        }

        { // capacity

            HxSTL::vector<int> v1(10);

            assert(v1.capacity() == 10);

        }

        { // shrink_to_fit

            HxSTL::vector<int> v1(10);

            v1.reserve(20);

            assert(v1.capacity() == 20);

            v1.shrink_to_fit();

            assert(v1.capacity() == 10);

        }

        { // clear

            HxSTL::vector<int> v1(10);

            assert(v1.size() == 10);

            v1.clear();

            assert(v1.empty());

        }

        { // insert 1

            int a1[4] = { 1, 2, 3, 4 };
            HxSTL::vector<int> v1;
            HxSTL::vector<int>::iterator it1;

            it1 = v1.insert(v1.begin(), a1[0]);

            assert(*it1 == 1);

            it1 = v1.insert(v1.end(), a1[2]);
            it1 = v1.insert(v1.end(), a1[3]);

            assert(*it1 == 4);

            v1.insert(v1.begin() + 1, a1[1]);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // insert 2

            int a1[4] = { 1, 2, 3, 4 };
            HxSTL::vector<int> v1;
            HxSTL::vector<int>::iterator it1;

            it1 = v1.insert(v1.begin(), 1);

            assert(*it1 == 1);

            it1 = v1.insert(v1.end(), 3);
            it1 = v1.insert(v1.end(), 4);

            assert(*it1 == 4);

            v1.insert(v1.begin() + 1, 2);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // insert 3

            int a1[7] = { 1, 1, 2, 2, 2, 3, 3 };
            HxSTL::vector<int> v1;
            HxSTL::vector<int>::iterator it1;

            it1 = v1.insert(v1.begin(), 2, 1);

            assert(*it1 == 1);
            assert(*(it1 + 1) == 1);

            it1 = v1.insert(v1.end(), 2, 3);

            assert(*it1 == 3);
            assert(*(it1 + 1) == 3);

            v1.reserve(10);
            v1.insert(v1.begin() + 2, 3, 2);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // insert 4

            int a1[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            HxSTL::vector<int> v1;
            HxSTL::vector<int>::iterator it1;

            it1 = v1.insert(v1.begin(), a1, a1 + 2);

            assert(*it1 == 1);
            assert(*(it1 + 1) == 2);

            it1 = v1.insert(v1.end(), a1 + 7, a1 + 10);

            assert(*it1 == 8);
            assert(*(it1 + 1) == 9);
            assert(*(it1 + 2) == 10);

            v1.reserve(20);
            it1 = v1.insert(v1.begin() + 2, a1 + 2, a1 + 3);

            assert(*it1 == 3);

            v1.insert(v1.begin() + 3, a1 + 3, a1 + 7);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // emplace

            int a1[4] = { 1, 2, 3, 4 };
            HxSTL::vector<int> v1;
            HxSTL::vector<int>::iterator it1;

            it1 = v1.emplace(v1.begin(), a1[0]);

            assert(*it1 == 1);

            it1 = v1.emplace(v1.end(), a1[2]);
            it1 = v1.emplace(v1.end(), a1[3]);

            assert(*it1 == 4);

            v1.emplace(v1.begin() + 1, a1[1]);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // erase 1

            int a1[5] = { 1, 2, 3, 4, 5 };
            HxSTL::vector<int> v1(a1, a1 + 5);
            HxSTL::vector<int>::iterator it1;

            it1 = v1.erase(v1.begin());

            assert(v1.size() == 4);
            assert(*it1 == 2);

            it1 = v1.erase(v1.end() - 1);
            assert(v1.size() == 3);
            assert(it1 == v1.end());

        }

        { // erase 2

            int a1[5] = { 1, 2, 3, 4, 5 };
            HxSTL::vector<int> v1(a1, a1 + 5);
            HxSTL::vector<int>::iterator it1;

            it1 = v1.erase(v1.begin(), v1.begin() + 2);

            assert(v1.size() == 3);
            assert(*it1 == 3);

            it1 = v1.erase(v1.begin(), v1.end());
            assert(v1.size() == 0);
            assert(it1 == v1.end());

        }

        { // push_back 1

            int a1[5] = { 1, 2, 3, 4, 5 };
            HxSTL::vector<int> v1;

            for (int i = 0; i != 5; ++i) {
                v1.push_back(a1[i]);
            }

            assert(v1.size() == 5);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // push_back 2

            int a1[5] = { 1, 2, 3, 4, 5 };
            int a2[5] = { 1, 2, 3, 4, 5 };
            HxSTL::vector<int> v1;

            for (int i = 0; i != 5; ++i) {
                v1.push_back(HxSTL::move(a1[i]));
            }

            assert(v1.size() == 5);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a2[i]);
            }

        }

        { // emplace_back

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

            assert(v1.size() == 4);

            for (HxSTL::vector<A>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i]() == i);
            }

        }

        { // pop_back

            int a1[5] = { 1, 2, 3, 4, 5 };
            HxSTL::vector<int> v1(a1, a1 + 5);

            v1.pop_back();
            v1.pop_back();

            assert(v1.size() == 3);

            for (HxSTL::vector<int>::size_type i = 0; i != v1.size(); ++i) {
                assert(v1[i] == a1[i]);
            }

        }

        { // resize 1

            HxSTL::vector<int> v1(10, 10);
            auto c1 = v1.capacity();

            v1.resize(5);

            assert(v1.size() == 5);
            assert(v1.capacity() == c1);

            v1.resize(10);

            assert(v1.size() == 10);
            assert(v1.capacity() == c1);
            assert(v1.back() == 0);

            v1.resize(20);

            assert(v1.size() == 20);
            assert(v1.front() == 10);
            assert(v1.back() == 0);

        }

        { // resize 2

            HxSTL::vector<int> v1(10, 10);
            auto c1 = v1.capacity();

            v1.resize(5, 5);

            assert(v1.size() == 5);
            assert(v1.capacity() == c1);

            v1.resize(10, 5);

            assert(v1.size() == 10);
            assert(v1.capacity() == c1);
            assert(v1.back() == 5);

            v1.resize(20, 20);

            assert(v1.size() == 20);
            assert(v1.front() == 10);
            assert(v1.back() == 20);

        }

        { // swap

            HxSTL::vector<int> v1(10);
            HxSTL::vector<int> v2(20);

            assert(v1.size() == 10);
            assert(v1.capacity() == 10);
            assert(v2.size() == 20);
            assert(v2.capacity() == 20);

            v1.swap(v2);

            assert(v1.size() == 20);
            assert(v1.capacity() == 20);
            assert(v2.size() == 10);
            assert(v2.capacity() == 10);

        }
    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}
