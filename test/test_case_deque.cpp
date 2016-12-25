#include <cstdio>
#include <cassert>
#include "deque.h"
#include "list.h"

int main() {

    { // member
        { // default constructor
            HxSTL::deque<int> d1;

            assert(d1.size() == 0);
            assert(d1.empty());
        }

        { // fill constructor
            HxSTL::deque<int> d1(10, 10);
            HxSTL::deque<int> d2(150, 10);

            assert(d1.size() == 10);
            assert(d2.size() == 150);

            for (auto i : d1) {
                assert(i == 10);
            }
            for (auto i : d2) {
                assert(i == 10);
            }
        }

        { // range constructor
            int a1[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
            HxSTL::list<int> l1(150, 10);
            HxSTL::deque<int> d1(a1, a1 + 10);
            HxSTL::deque<int> d2(l1.begin(), l1.end());

            assert(d1 == HxSTL::deque<int>(10, 10));
            assert(d2 == HxSTL::deque<int>(150, 10));
        }

        { // copy constructor
            HxSTL::deque<int> d1(10, 10);
            HxSTL::deque<int> d2(d1);

            assert(d1 == d2);
        }

        { // move constructor
            HxSTL::deque<int> d1(10, 10);
            HxSTL::deque<int> d2(HxSTL::move(d1));

            assert(d1 == HxSTL::deque<int>(10, 10));
        }

        { // init constructor
            HxSTL::deque<int> d1({ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 });

            assert(d1 == HxSTL::deque<int>(10, 10));
        }

        { // copy assignment
            HxSTL::deque<int> d1(10, 10);
            HxSTL::deque<int> d2;

            d2 = d1;
            d2 = d2;

            assert(d1 == d2);
        }

        { // move assignment
            HxSTL::deque<int> d1(10, 10);
            HxSTL::deque<int> d2;

            d2 = HxSTL::move(d1);
            d2 = HxSTL::move(d2);

            assert(d2 == HxSTL::deque<int>(10, 10));
        }

        { // init assignment
            HxSTL::deque<int> d1;

            d1 = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };

            assert(d1 == HxSTL::deque<int>(10, 10));
        }

        { // assign 1
            HxSTL::deque<int> d1;
            HxSTL::deque<int> d2(10, 10);

            d1.assign(5, 5);
            d2.assign(15, 15);

            assert(d1 == HxSTL::deque<int>(5, 5));
            assert(d2 == HxSTL::deque<int>(15, 15));
        }

        { // assign 2
            HxSTL::deque<int> d1;
            HxSTL::deque<int> d2({ 0, 1, 2, 3, 4, 5 });

            d1.assign(d2.begin(), d2.end());
            d2.assign(d1.begin(), d1.begin() + 3);

            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5 }));
            assert(d2 == HxSTL::deque<int>({ 0, 1, 2 }));

            d1.assign(d1.begin(), d1.end());

            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5 }));
        }

        { // assign 3
            HxSTL::deque<int> d1;

            d1.assign({ 0, 1, 2, 3, 4 });

            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4 }));
        }

        { // at
            HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            const HxSTL::deque<int> d2(d1);

            for (int i = 0; i != 10; ++ i) {
                assert(d1.at(i) == i);
                assert(d2.at(i) == i);
            }

            bool except = false;

            try {
                d1.at(11);
            } catch (HxSTL::out_of_range) {
                except = true;
            }

            assert(except);
        }

        { // operator[]
            HxSTL::deque<int> d1({ 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert((d1[0] = 0) == 0);

            bool except = false;

            try {
                d1[11];
            } catch (HxSTL::out_of_range) {
                except = true;
            }

            assert(!except);
        }

        { // front_back
            HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(d1.front() == 0);
            assert(d1.back() == 9);
        }

        { // begin cbegin
            HxSTL::deque<int> d1({ 0, 1 });
            const HxSTL::deque<int> d2({ 1, 2 });

            assert(*d1.begin() == 0);
            assert(*d2.begin() == 1);
            assert(*d1.cbegin() == 0);
            assert(*d2.cbegin() == 1);
        }

        { // end cend
            HxSTL::deque<int> d1({ 0, 1 });
            const HxSTL::deque<int> d2({ 1, 2 });

            assert(*(d1.end() - 1) == 1);
            assert(*(d2.end() - 1) == 2);
            assert(*(d1.cend() - 1) == 1);
            assert(*(d2.cend() - 1) == 2);
        }

        { // clear
            HxSTL::deque<int> d1(10, 10);

            d1.clear();

            assert(d1.empty());
        }

        { // insert 1
            HxSTL::deque<int> d1({ 1, 2, 4, 5, 6, 8 });

            assert(*d1.insert(d1.begin(), 0) == 0);
            assert(*d1.insert(d1.begin() + 3, 3) == 3);
            assert(*d1.insert(d1.end() - 1, 7) == 7);
            assert(*d1.insert(d1.end(), 9) == 9);
            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));

            HxSTL::deque<int> d2(127, 1);

            assert(*d2.insert(d2.end(), 1) == 1);
            assert(*d2.insert(d2.end(), 1) == 1);
            assert(d2 == HxSTL::deque<int>(129, 1));
        }

        { // insert 2
        }

        { // insert 3
            { // index < size / 2
                HxSTL::deque<int> d1({ 4, 4, 4, 4, 4, 4, 4, 4 });

                assert(*d1.insert(d1.begin(), 3, 3) == 3);
                assert(*d1.insert(d1.begin() + 2, 3, 2) == 2);
                assert(*d1.insert(d1.begin() + 5, 2, 1) == 1);
                assert(d1 == HxSTL::deque<int>({ 3, 3, 2, 2, 2, 1, 1, 3, 4, 4, 4, 4, 4, 4, 4, 4 }));

                HxSTL::deque<int> d2(127, 1);

                assert(*d2.insert(d2.begin(), 10, 1) == 1);
                assert(d2 == HxSTL::deque<int>(137, 1));
            }

            { // index >= size / 2
                HxSTL::deque<int> d1({ 4, 4, 4, 4, 4, 4, 4, 4 });

                assert(*d1.insert(d1.end(), 3, 3) == 3);
                assert(*d1.insert(d1.end() - 2, 3, 2) == 2);
                assert(*d1.insert(d1.end() - 5, 2, 1) == 1);
                assert(d1 == HxSTL::deque<int>({ 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 1, 2, 2, 2, 3, 3 }));

                HxSTL::deque<int> d2(127, 1);

                assert(*d2.insert(d2.end(), 10, 1) == 1);
                assert(d2 == HxSTL::deque<int>(137, 1));
            }

        }

        { // insert 4

            { // index < size / 2
                HxSTL::deque<int> d1({ 8, 9, 10, 11, 12, 13, 14, 15 });
                HxSTL::deque<int> d2({ 0, 1, 7 });
                HxSTL::deque<int> d3({ 2, 3, 4 });
                HxSTL::deque<int> d4({ 5, 6 });

                assert(*d1.insert(d1.begin(), d2.begin(), d2.end()) == 0);
                assert(*d1.insert(d1.begin() + 2, d3.begin(), d3.end()) == 2);
                assert(*d1.insert(d1.begin() + 5, d4.begin(), d4.end()) == 5);
                assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }));

                HxSTL::deque<int> d5(127, 1);
                HxSTL::deque<int> d6(10, 1);

                assert(*d5.insert(d5.begin(), d6.begin(), d6.end()) == 1);
                assert(d5 == HxSTL::deque<int>(137, 1));
            }

            { // index >= size / 2
                HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7 });
                HxSTL::deque<int> d2({ 8, 14, 15 });
                HxSTL::deque<int> d3({ 11, 12, 13 });
                HxSTL::deque<int> d4({ 9, 10 });

                assert(*d1.insert(d1.end(), d2.begin(), d2.end()) == 8);
                assert(*d1.insert(d1.end() - 2, d3.begin(), d3.end()) == 11);
                assert(*d1.insert(d1.end() - 5, d4.begin(), d4.end()) == 9);
                assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }));

                HxSTL::deque<int> d5(127, 1);
                HxSTL::deque<int> d6(10, 1);

                assert(*d5.insert(d5.end(), d6.begin(), d6.end()) == 1);
                assert(d5 == HxSTL::deque<int>(137, 1));
            }

        }

        { // erase 1
            HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*d1.erase(d1.begin() + 1) == 2);
            assert(*d1.erase(d1.begin() + 1) == 3);
            assert(*d1.erase(d1.begin()) == 3);
            assert(*d1.erase(d1.end() - 2) == 9);

            auto it = d1.erase(d1.end() - 1);

            assert(it == d1.end());
            assert(d1.erase(d1.end()) == d1.end());
            assert(d1 == HxSTL::deque<int>({ 3, 4, 5, 6, 7 }));
        }

        { // erase 2
            HxSTL::deque<int> d1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*d1.erase(d1.begin(), d1.begin() + 2) == 2);
            assert(*d1.erase(d1.end() - 3, d1.end() - 1) == 9);
            assert(d1.erase(d1.end(), d1.end()) == d1.end());
            assert(d1 == HxSTL::deque<int>({ 2, 3, 4, 5, 6, 9 }));

            HxSTL::deque<int> d2(150, 1);

            d2.erase(d2.end() - 100, d2.end());

            assert(d2 == HxSTL::deque<int>(50, 1));
        }

        { // resize
            HxSTL::deque<int> d1({ 0, 1, 2 });

            d1.resize(5);

            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 0, 0 }));

            d1.resize(8, 8);

            assert(d1 == HxSTL::deque<int>({ 0, 1, 2, 0, 0, 8, 8, 8 }));

            d1.resize(2);

            assert(d1 == HxSTL::deque<int>({ 0, 1 }));
        }
    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}
