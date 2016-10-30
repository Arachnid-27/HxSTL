#include <iostream>
#include "vector.h"

void print_vector(HxSTL::vector<int> &vec) {
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    for (typename HxSTL::vector<int>::size_type i = 0; i != vec.size(); ++i) {
            std::cout << vec[i] << " ";
        }
    std::cout << std::endl;
}


void test_case_vector() {
    std::cout << "\n-------------------fill initialize" << std::endl;
    HxSTL::vector<int> vec1(5, 5);
    print_vector(vec1);

    std::cout << "\n-------------------push pop" << std::endl;
    vec1.push_back(10);
    vec1.push_back(11);
    print_vector(vec1);
    vec1.pop_back();
    print_vector(vec1);

    std::cout << "\n-------------------single erase" << std::endl;
    vec1.erase(vec1.begin() + 1);
    print_vector(vec1);

    std::cout << "\n-------------------fill insert" << std::endl;
    vec1.insert(vec1.end() - 3, 2, 3);
    print_vector(vec1);

    std::cout << "\n-------------------reserve" << std::endl;
    vec1.reserve(20);
    print_vector(vec1);

    std::cout << "\n-------------------resize" << std::endl;
    vec1.resize(15, 6);
    print_vector(vec1);
    vec1.resize(25, 8);
    print_vector(vec1);

    std::cout << "\n-------------------range initialize" << std::endl;
    HxSTL::vector<int> vec2(vec1);
    print_vector(vec2);

    std::cout << "\n-------------------range insert" << std::endl;
    vec2.insert(vec2.begin() + 5, vec1.begin() + 2, vec1.end() - 2);
    print_vector(vec2);

    std::cout << "\n-------------------swap" << std::endl;
    print_vector(vec1);
    print_vector(vec2);
    vec1.swap(vec2);
    print_vector(vec1);
    print_vector(vec2);

    std::cout << "\n-------------------operator=" << std::endl;
    print_vector(vec1);
    HxSTL::vector<int> vec3;
    vec3 = vec1;
    print_vector(vec3);
}
