#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "set.h"

int main() {

    { // member
        { // default constructor
            HxSTL::set<int> s1;

            assert(s1.empty());
            assert(s1.size() == 0);
        }

        { // range constructor
            int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
            HxSTL::set<int> s1(a1, a1 + 15);

            assert(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // init constructor
            int a1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            HxSTL::set<int> s1({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

            assert(HxSTL::equal(s1.begin(), s1.end(), a1));
        }

        { // copy constructor
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::set<int> s2(s1);

            assert(s1 == s2);
        }

        { // move constructor
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::set<int> s2(HxSTL::move(s1));

            assert(s2 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // copy assignment
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::set<int> s2;

            s2 = s1;
            s2 = s2;

            assert(s2 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // move assignment
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
            HxSTL::set<int> s2;

            s2 = HxSTL::move(s1);
            s2 = HxSTL::move(s2);

            assert(s2 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // init assignment
            HxSTL::set<int> s1;

            assert((s1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }) == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // begin cbegin
            HxSTL::set<int> s1({ 0, 1 });
            const HxSTL::set<int> s2({ 1, 2 });

            assert(*s1.begin() == 0);
            assert(*s2.begin() == 1);
            assert(*s1.cbegin() == 0);
            assert(*s2.cbegin() == 1);
        }

        { // end cend
            HxSTL::set<int> s1({ 0, 1 });
            const HxSTL::set<int> s2({ 1, 2 });

            assert(*(--s1.end()) == 1);
            assert(*(--s2.end()) == 2);
            assert(*(--s1.cend()) == 1);
            assert(*(--s2.cend()) == 2);
        }

        { // empty
            HxSTL::set<int> s1;
            HxSTL::set<int> s2({ 0 });

            assert(s1.empty());
            assert(!s2.empty());
        }

        { // size
            HxSTL::set<int> s1;
            HxSTL::set<int> s2({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.size() == 0);
            assert(s2.size() == 10);
        }

        { // clear
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            s1.clear();

            assert(s1.empty());
        }

        { // insert 1
            HxSTL::set<int> s1;

            assert(*(s1.insert(1).first) == 1);
            assert(!(s1.insert(1).second));
            assert(*(s1.insert(3).first) == 3);
            assert((s1.insert(2).second));
            assert(s1 == HxSTL::set<int>({ 1, 2, 3 }));

            HxSTL::set<int> s2;

            srand((unsigned) time(NULL));
            for (int i = 0; i != 100000; ++i) {
                s2.insert(rand() % 50000);
            }

            assert(s2.size() <= 50000);
        }

        { // insert 2
            { // header leftmost rightmost
                HxSTL::set<int> s1;

                assert(*(s1.insert(s1.end(), 5)) == 5);
                assert(*(s1.insert(s1.end(), 6)) == 6);
                assert(*(s1.insert(s1.begin(), 4)) == 4);
                assert(*(s1.insert(--s1.end(), 7)) == 7);
                assert(s1 == HxSTL::set<int>({ 4, 5, 6, 7 }));
            }

            { // before to right
                //      6(hint)
                //     / \
                //    4   7
                //   / \
                //  3  NIL
                HxSTL::set<int> s1({ 6, 4, 7, 3 });
                auto hint = s1.cbegin();

                ++hint, ++hint;

                assert(*(s1.insert(hint, 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // before to left
                //     4
                //    / \
                //   3   6(hint)
                //      / \
                //    NIL  7
                HxSTL::set<int> s1({ 4, 3, 6, 7 });
                auto hint = s1.cbegin();

                ++hint, ++hint;

                assert(*(s1.insert(hint, 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // after to right
                HxSTL::set<int> s1({ 6, 4, 7, 3 });

                assert(*(s1.insert(++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // after to left
                HxSTL::set<int> s1({ 4, 3, 6, 7 });

                assert(*(s1.insert(++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // equal
                HxSTL::set<int> s1({ 1, 2, 3 });

                assert(s1.insert(++s1.begin(), 2) == ++s1.begin());
                assert(s1 == HxSTL::set<int>({ 1, 2, 3 }));
            }

            { // other
                HxSTL::set<int> s1({ 1, 2, 3 });

                assert(*(s1.insert(s1.begin(), 4)) == 4);
                assert(s1 == HxSTL::set<int>({ 1, 2, 3, 4 }));
            }
        }

        { // insert 3
            int a1[] = { 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 };
            HxSTL::set<int> s1;

            s1.insert(a1, a1 + 15);

            assert(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // insert 4
            HxSTL::set<int> s1;

            s1.insert({ 0, 7, 9, 2, 9, 3, 5, 5, 6, 4, 8, 1, 1, 8, 2 });

            assert(s1 == HxSTL::set<int>({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
        }

        { // emplace
            HxSTL::set<int> s1;

            assert(*(s1.emplace(1).first) == 1);
            assert(!(s1.emplace(1).second));
            assert(*(s1.emplace(3).first) == 3);
            assert((s1.emplace(2).second));
            assert(s1 == HxSTL::set<int>({ 1, 2, 3 }));
        }

        { // emplace_hint
            { // header leftmost rightmost
                HxSTL::set<int> s1;

                assert(*(s1.emplace_hint(s1.end(), 5)) == 5);
                assert(*(s1.emplace_hint(s1.end(), 6)) == 6);
                assert(*(s1.emplace_hint(s1.begin(), 4)) == 4);
                assert(*(s1.emplace_hint(--s1.end(), 7)) == 7);
                assert(s1 == HxSTL::set<int>({ 4, 5, 6, 7 }));
            }

            { // before to right
                HxSTL::set<int> s1({ 6, 4, 7, 3 });

                assert(*(s1.emplace_hint(++++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // before to left
                HxSTL::set<int> s1({ 4, 3, 6, 7 });

                assert(*(s1.emplace_hint(++++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // after to right
                HxSTL::set<int> s1({ 6, 4, 7, 3 });

                assert(*(s1.emplace_hint(++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // after to left
                HxSTL::set<int> s1({ 4, 3, 6, 7 });

                assert(*(s1.emplace_hint(++s1.begin(), 5)) == 5);
                assert(s1 == HxSTL::set<int>({ 3, 4, 5, 6, 7 }));
            }

            { // equal
                HxSTL::set<int> s1({ 1, 2, 3 });

                assert(s1.emplace_hint(++s1.begin(), 2) == ++s1.begin());
                assert(s1 == HxSTL::set<int>({ 1, 2, 3 }));
            }

            { // other
                HxSTL::set<int> s1({ 1, 2, 3 });

                assert(*(s1.emplace_hint(s1.begin(), 4)) == 4);
                assert(s1 == HxSTL::set<int>({ 1, 2, 3, 4 }));
            }
        }

        { // erase 1
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*s1.erase(++s1.begin()) == 2);
            assert(*s1.erase(++s1.begin()) == 3);
            assert(s1.erase(--s1.end()) == s1.end());
            assert(s1 == HxSTL::set<int>({ 0, 3, 4, 5, 6, 7, 8 }));
        }

        { // erase 2
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(*s1.erase(++s1.begin(), --s1.end()) == 9);
            assert(s1 == HxSTL::set<int>({ 0, 9 }));
            assert(s1.erase(s1.begin(), s1.end()) == s1.end());
        }

        { // erase 3
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.erase(3) == 1);
            assert(s1.erase(5) == 1);
            assert(s1.erase(7) == 1);
            assert(s1 == HxSTL::set<int>({ 0, 1, 2, 4, 6, 8, 9 }));
        }

        { // swap
            HxSTL::set<int> s1({ 0, 2, 4, 6, 8 });
            HxSTL::set<int> s2({ 1, 3, 5, 7, 9 });

            s1.swap(s2);

            assert(s1 == HxSTL::set<int>({ 1, 3, 5, 7, 9 }));
            assert(s2 == HxSTL::set<int>({ 0, 2, 4, 6, 8 }));
        }

        { // count
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });

            assert(s1.count(3) == 1);
            assert(s1.count(5) == 1);
            assert(s1.count(7) == 1);
        }

        { // find
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5 });
            const HxSTL::set<int> s2({ 0, 1, 2, 3, 4, 5 });

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
            HxSTL::set<int> s1({ 0, 1, 2, 3, 4, 5 });
            const HxSTL::set<int> s2({ 0, 1, 2, 3, 4, 5 });

            assert(*s1.equal_range(0).first == 0);
            assert(*s1.equal_range(3).second == 4);
            assert(*s1.equal_range(5).first == 5);
            assert(s1.equal_range(6).first == s1.end());
            assert(*s2.equal_range(0).first == 0);
            assert(*s2.equal_range(3).second == 4);
            assert(*s2.equal_range(5).first == 5);
            assert(s2.equal_range(6).first == s2.end());
        }

        { // lower_bound
            HxSTL::set<int> s1({ 1, 3, 5, 7 });
            const HxSTL::set<int> s2({ 1, 3, 5, 7 });

            assert(*s1.lower_bound(0) == 1);
            assert(*s1.lower_bound(1) == 1);
            assert(*s1.lower_bound(6) == 7);
            assert(s1.lower_bound(8) == s1.end());
            assert(*s2.lower_bound(0) == 1);
            assert(*s2.lower_bound(1) == 1);
            assert(*s2.lower_bound(6) == 7);
            assert(s2.lower_bound(8) == s2.end());
        }

        { // upper_bound
            HxSTL::set<int> s1({ 1, 3, 5, 7 });
            const HxSTL::set<int> s2({ 1, 3, 5, 7 });

            assert(*s1.upper_bound(0) == 1);
            assert(*s1.upper_bound(1) == 3);
            assert(*s1.upper_bound(6) == 7);
            assert(s1.upper_bound(7) == s1.end());
            assert(*s2.upper_bound(0) == 1);
            assert(*s2.upper_bound(1) == 3);
            assert(*s2.upper_bound(6) == 7);
            assert(s2.upper_bound(7) == s2.end());
        }
    }

    { // non-member
        { // operator==
            HxSTL::set<int> s1;
            HxSTL::set<int> s2;
            HxSTL::set<int> s3{ 0, 1, 2, 3, 4 };
            HxSTL::set<int> s4{ 0, 1, 2, 3, 4 };

            assert(s1 == s2);
            assert(s3 == s4);
        }

        { // operator!=
            HxSTL::set<int> s1;
            HxSTL::set<int> s2{ 0, 1, 2, 3, 4 };
            HxSTL::set<int> s3{ 0, 1, 2, 3 };

            assert(s1 != s2);
            assert(s2 != s3);
        }
    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}
