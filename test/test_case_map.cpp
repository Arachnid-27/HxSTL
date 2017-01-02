#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "map.h"

int main() {

    { // member
        { // default constructor
            HxSTL::map<int, int> s1;

            assert(s1.empty());
            assert(s1.size() == 0);
        }

        { // range constructor
            HxSTL::pair<const int, int> a1[] = { HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3), HxSTL::make_pair(1, 3) };
            HxSTL::map<int, int> s1(a1, a1 + 3);

            assert((s1 == HxSTL::map<int, int>({ HxSTL::make_pair(1,2), HxSTL::make_pair(2,3) })));
        }

        { // init constructor
            HxSTL::pair<const int, int> a1[] = { HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) };
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(HxSTL::equal(s1.begin(), s1.end(), a1));
        }

        { // copy constructor
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s2(s1);

            assert(s1 == s2);
        }

        { // move constructor
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s2(HxSTL::move(s1));

            assert((s2 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));
        }

        { // copy assignment
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s2;

            s2 = s1;
            s2 = s2;

            assert((s2 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));
        }

        { // move assignment
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s2;

            s2 = HxSTL::move(s1);
            s2 = HxSTL::move(s2);

            assert((s2 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));
        }

        { // init assignment
            HxSTL::map<int, int> s1;

            assert(((s1 = { HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }) == 
                    HxSTL::map<int, int>({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) })));
        }

        { // at
            int flag = 0;
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            try {
                s1.at(2) = 4;
                s1.at(0) = 0;
            } catch (HxSTL::out_of_range) {
                flag = 1;
            }

            assert(flag = 1);
            assert(s1.at(1) == 2);
            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 4) }));
        }

        { // operator []
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            s1[2] = 4;
            s1[0] = 0;

            assert(s1.at(1) == 2);
            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 4), HxSTL::make_pair(0, 0) }));
        }

        { // begin cbegin
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            const HxSTL::map<int, int> s2({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.begin() -> first == 1);
            assert(s2.begin() -> first == 1);
            assert(s1.cbegin() -> first == 1);
            assert(s2.cbegin() -> first == 1);
        }

        { // end cend
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            const HxSTL::map<int, int> s2({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert((--s1.end()) -> second == 3);
            assert((--s2.end()) -> second == 3);
            assert((--s1.cend()) -> second == 3);
            assert((--s2.cend()) -> second == 3);
        }

        { // empty
            HxSTL::map<int, int> s1;
            const HxSTL::map<int, int> s2({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.empty());
            assert(!s2.empty());
        }

        { // size
            HxSTL::map<int, int> s1;
            const HxSTL::map<int, int> s2({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.size() == 0);
            assert(s2.size() == 2);
        }

        { // clear
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            s1.clear();

            assert(s1.empty());
        }

        { // insert 1
            HxSTL::map<int, int> s1;

            assert(s1.insert(HxSTL::make_pair(1, 2)).first -> first == 1);
            assert(!(s1.insert(HxSTL::make_pair(1, 4)).second));
            assert(s1.insert(HxSTL::make_pair(2, 3)).first -> second == 3);
            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));

            HxSTL::map<int, int> s2;

            srand((unsigned) time(NULL));
            for (int i = 0; i != 100000; ++i) {
                s2.insert(HxSTL::make_pair(i % 50000, i));
            }

            assert(s2.size() <= 50000);
        }

        { // insert 2
        }

        { // insert 3
            HxSTL::pair<const int, int> a1[] = { HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3), HxSTL::make_pair(1, 3) };
            HxSTL::map<int, int> s1;

            s1.insert(a1, a1 + 3);

            assert((s1 == HxSTL::map<int, int>({ HxSTL::make_pair(1,2), HxSTL::make_pair(2,3) })));
        }

        { // insert 4
            HxSTL::map<int, int> s1;

            s1.insert({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3), HxSTL::make_pair(1, 3) });

            assert((s1 == HxSTL::map<int, int>({ HxSTL::make_pair(1,2), HxSTL::make_pair(2,3) })));
        }

        { // emplace
            HxSTL::map<int, int> s1;

            assert(s1.emplace(1, 2).first -> first == 1);
            assert(!(s1.emplace(1, 4).second));
            assert(s1.emplace(2, 3).first -> second == 3);
            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));
        }

        { // emplace_hint
        }

        { // erase 1
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.erase(s1.begin()) -> first == 2);
            assert(s1.erase(--s1.end()) == s1.end());
            assert(s1.empty());
        }

        { // erase 2
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.erase(s1.begin(), ++s1.begin()) -> second == 3);
            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(2, 3) }));
            assert(s1.erase(s1.begin(), s1.end()) == s1.end());
        }

        { // erase 3
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.erase(1) == 1);
            assert(s1.erase(2) == 1);
            assert(s1.empty());
        }

        { // swap
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s2({ HxSTL::make_pair(0, 1), HxSTL::make_pair(1, 2) });

            s1.swap(s2);

            assert((s1 == HxSTL::map<int, int>{ HxSTL::make_pair(0, 1), HxSTL::make_pair(1, 2) }));
            assert((s2 == HxSTL::map<int, int>{ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) }));
        }

        { // count
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.count(1) == 1);
            assert(s1.count(2) == 1);
            assert(s1.count(3) == 0);
        }

        { // find
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.find(1) -> first == 1);
            assert(s1.find(2) -> second == 3);
            assert(s1.find(3) == s1.end());
        }

        { // equal_range
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.equal_range(1).second -> first == 2);
            assert(s1.equal_range(2).first -> second == 3);
            assert(s1.equal_range(3).first == s1.end());
        }

        { // lower_bound
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.lower_bound(0) -> first == 1);
            assert(s1.lower_bound(1) -> second == 2);
            assert(s1.lower_bound(2) -> first == 2);
            assert(s1.lower_bound(3) == s1.end());
        }

        { // upper_bound
            HxSTL::map<int, int> s1({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1.upper_bound(0) -> first == 1);
            assert(s1.upper_bound(1) -> second == 3);
            assert(s1.upper_bound(2) == s1.end());
        }
    }

    { // non-member
        { // operator==
            HxSTL::map<int, int> s1;
            HxSTL::map<int, int> s2;
            HxSTL::map<int, int> s3({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s4({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });

            assert(s1 == s2);
            assert(s3 == s4);
        }

        { // operator!=
            HxSTL::map<int, int> s1;
            HxSTL::map<int, int> s2({ HxSTL::make_pair(1, 2), HxSTL::make_pair(2, 3) });
            HxSTL::map<int, int> s3({ HxSTL::make_pair(0, 1), HxSTL::make_pair(1, 2) });

            assert(s1 != s2);
            assert(s2 != s3);
        }
    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}
