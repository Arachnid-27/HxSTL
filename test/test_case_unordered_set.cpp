#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "unordered_set.h"

int main() {

    { // member
        { // default constructor
            HxSTL::unordered_set<int> s1;
            HxSTL::unordered_set<int> s2(1000);

            assert(s1.empty());
            assert(s1.size() == 0);
            assert(s1.bucket_count() != s2.bucket_count());
        }

        { // range construcotr
            int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
            HxSTL::unordered_set<int> s1(a1, a1 + 15);

            assert(s1 == HxSTL::unordered_set<int>({ 0, 7, 9, 2, 3, 5, 6, 4, 8, 1 }));
        }

        { // init constructor
            int a1[] = { 1, 8, 4, 6, 5, 3, 2, 9, 7, 0 };
            HxSTL::unordered_set<int> s1({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

            assert(HxSTL::equal(s1.begin(), s1.end(), a1));
        }

        { // copy constructor
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::unordered_set<int> s2(s1);

            assert(s1 == s2);
        }

        { // move constructor
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::unordered_set<int> s2(s1);

            assert(s2 == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }
        
        { // copy assignment
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::unordered_set<int> s2;

            s2 = s1;
            s2 = s2;

            assert(s2 == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // move assignment
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::unordered_set<int> s2;

            s2 = HxSTL::move(s1);
            s2 = HxSTL::move(s2);

            assert(s2 == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // init assignment
            HxSTL::unordered_set<int> s1;

            assert((s1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // begin cbegin
            HxSTL::unordered_set<int> s1({ 0, 1 });
            const HxSTL::unordered_set<int> s2({ 1, 2 });

            assert(*s1.begin() == 1);
            assert(*s2.begin() == 2);
            assert(*s1.cbegin() == 1);
            assert(*s2.cbegin() == 2);
        }

        { // end cend
            HxSTL::unordered_set<int> s1({ 0 });
            const HxSTL::unordered_set<int> s2({ 1 });

            assert(s1.end() == ++s1.begin());
            assert(s2.end() == ++s2.begin());
            assert(s1.cend() == ++s1.begin());
            assert(s2.cend() == ++s2.begin());
        }

        { // empty
            HxSTL::unordered_set<int> s1;
            HxSTL::unordered_set<int> s2({ 0 });

            assert(s1.empty());
            assert(!s2.empty());
        }

        { // size
            HxSTL::unordered_set<int> s1;
            HxSTL::unordered_set<int> s2({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.size() == 0);
            assert(s2.size() == 10);
        }

        { // clear
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            s1.clear();

            assert(s1.empty());
        }

        { // insert 1
            HxSTL::unordered_set<int> s1;

            assert(*(s1.insert(1).first) == 1);
            assert(!(s1.insert(1).second));
            assert(*(s1.insert(3).first) == 3);
            assert((s1.insert(2).second));
            assert(s1 == HxSTL::unordered_set<int>({ 1, 3, 2 }));

            HxSTL::unordered_set<int> s2;

            srand((unsigned) time(NULL));
            for (int i = 0; i != 100000; ++i) {
                s2.insert(rand() % 50000);
            }

            assert(s2.size() <= 50000);
        }

        { // insert 2
            HxSTL::unordered_set<int> s1;

            assert(*s1.insert(s1.begin(), 1) == 1);
            assert(*s1.insert(s1.begin(), 1) == 1);
            assert(*s1.insert(s1.begin(), 3) == 3);
            assert(*s1.insert(s1.begin(), 2) == 2);
            assert(s1 == HxSTL::unordered_set<int>({ 1, 3, 2 }));
        }

        { // insert 3
            int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
            HxSTL::unordered_set<int> s1;

            s1.insert(a1, a1 + 15);

            assert(s1 == HxSTL::unordered_set<int>({ 0, 7, 9, 2, 3, 5, 6, 4, 8, 1 }));
        }

        { // insert 4
            HxSTL::unordered_set<int> s1;

            s1.insert({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

            assert(s1 == HxSTL::unordered_set<int>({ 0, 7, 9, 2, 3, 5, 6, 4, 8, 1 }));
        }

        { // emplace
            HxSTL::unordered_set<int> s1;

            assert(*(s1.emplace(1).first) == 1);
            assert(!(s1.emplace(1).second));
            assert(*(s1.emplace(3).first) == 3);
            assert((s1.emplace(2).second));
            assert(s1 == HxSTL::unordered_set<int>({ 1, 3, 2 }));
        }

        { // emplace_hint
            HxSTL::unordered_set<int> s1;

            assert(*s1.emplace_hint(s1.begin(), 1) == 1);
            assert(*s1.emplace_hint(s1.begin(), 1) == 1);
            assert(*s1.emplace_hint(s1.begin(), 3) == 3);
            assert(*s1.emplace_hint(s1.begin(), 2) == 2);
            assert(s1 == HxSTL::unordered_set<int>({ 1, 3, 2 }));
        }

        { // erase 1
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*s1.erase(++s1.begin()) == 7);
            assert(*s1.erase(++s1.begin()) == 6);
            assert(s1 == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5, 6, 9 }));
        }

        { // erase 2
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*s1.erase(s1.begin(), ++++++++s1.begin()) == 5);
            assert(s1 == HxSTL::unordered_set<int>({ 0, 1, 2, 3, 4, 5 }));
            assert(s1.erase(s1.begin(), s1.begin()) == s1.begin());
            assert(s1.erase(s1.begin(), s1.end()) == s1.end());
        }

        { // erase 3
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.erase(3) == 1);
            assert(s1.erase(3) == 0);
            assert(s1.erase(5) == 1);
            assert(s1.erase(7) == 1);
            assert(s1 == HxSTL::unordered_set<int>({ 0, 1, 2, 4, 6, 8, 9 }));
        }

        { // swap
            HxSTL::unordered_set<int> s1({ 0, 2, 4, 6, 8 });
            HxSTL::unordered_set<int> s2({ 1, 3, 5, 7, 9 });

            s1.swap(s2);

            assert(s1 == HxSTL::unordered_set<int>({ 1, 3, 5, 7, 9 }));
            assert(s2 == HxSTL::unordered_set<int>({ 0, 2, 4, 6, 8 }));
        }

        { // count
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.count(3) == 1);
            assert(s1.count(5) == 1);
            assert(s1.count(7) == 1);
        }

        { // find
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5 });
            const HxSTL::unordered_set<int> s2({ 0, 1, 2, 3, 4, 5 });

            assert(*s1.find(0) == 0);
            assert(*s1.find(3) == 3);
            assert(*s1.find(5) == 5);
            assert(s1.find(6) == s1.end());
            assert(*s2.find(0) == 0);
            assert(*s2.find(3) == 3);
            assert(*s2.find(5) == 5);
            assert(s2.find(6) == s2.end());
        }

        { // equal_range
            HxSTL::unordered_set<int> s1({ 0, 1, 2, 3, 4, 5 });
            const HxSTL::unordered_set<int> s2({ 0, 1, 2, 3, 4, 5 });

            assert(*s1.equal_range(0).first == 0);
            assert(*s1.equal_range(3).second == 2);
            assert(*s1.equal_range(5).first == 5);
            assert(s1.equal_range(6).first == s1.end());
            assert(*s2.equal_range(0).first == 0);
            assert(*s2.equal_range(3).second == 2);
            assert(*s2.equal_range(5).first == 5);
            assert(s2.equal_range(6).first == s2.end());
        }

    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}
