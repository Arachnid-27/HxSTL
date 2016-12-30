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
        
    }

    printf("\033[1;32m=================================================\033[0m\n");
    printf("\033[1;32mAll tests passed\033[0m\n");

}

