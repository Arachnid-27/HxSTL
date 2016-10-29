#include <iostream>
#include "list.h"


void print_list(const HxSTL::list<int>& li) {
    for (typename HxSTL::list<int>::iterator first = li.begin(); first != li.end(); ++first) {
        std::cout << *first << " ";
    }
    std::cout << std::endl;
}


void test_case_list() {
    HxSTL::list<int> li(4, 8);
    print_list(li);
}
