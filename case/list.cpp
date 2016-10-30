#include <iostream>
#include "list.h"


void print_list(const HxSTL::list<int>& li) {
    for (typename HxSTL::list<int>::iterator first = li.begin(); first != li.end(); ++first) {
        std::cout << *first << " ";
    }
    std::cout << std::endl;
}


void test_case_list() {
    std::cout << "\n\n-------------list--------------" << std::endl;

    std::cout << "\n-------------------fill initialize" << std::endl;
    HxSTL::list<int> li1(4, 8);
    print_list(li1);

    std::cout << "\n-------------------push pop" << std::endl;
    li1.push_back(10);
    li1.push_back(11);
    li1.push_front(100);
    li1.push_front(86);
    print_list(li1);
    li1.pop_back();
    li1.pop_back();
    li1.pop_back();
    li1.pop_front();
    print_list(li1);

    std::cout << "\n-------------------front back" << std::endl;
    std::cout << li1.front() << " " << li1.back() << std::endl;

    std::cout << "\n-------------------single insert" << std::endl;
    li1.insert(++li1.begin(), 55);
    print_list(li1);

    std::cout << "\n-------------------fill insert" << std::endl;
    li1.insert(li1.end(), 5, 10);
    print_list(li1);

    std::cout << "\n-------------------resize" << std::endl;
    li1.resize(15, 5);
    print_list(li1);
    li1.resize(12);
    print_list(li1);


    std::cout << "\n-------------------copy initialize" << std::endl;
    HxSTL::list<int> li2(li1);
    print_list(li2);

    std::cout << "\n-------------------operator=" << std::endl;
    HxSTL::list<int> li3;
    li3 = li2;
    print_list(li3);

    std::cout << "\n-------------------range erase" << std::endl;
    li3.erase(++li3.begin(), --li3.end());
    print_list(li3);

    std::cout << "\n-------------------range assign" << std::endl;
    li2.assign(li3.begin(), li3.end());
    print_list(li2);

    std::cout << "\n-------------------clear" << std::endl;
    li2.clear();
    print_list(li2);
}
