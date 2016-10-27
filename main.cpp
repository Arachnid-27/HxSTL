#include <iostream>
#include "vector.h"


void print(HxSTL::vector<int> &vec) {
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    for (typename HxSTL::vector<int>::size_type i = 0; i != vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}


int main() {
    HxSTL::vector<int> vec(5, 5);
    print(vec);
    vec.push_back(10);
    print(vec);
    vec.push_back(11);
    print(vec);
    vec.pop_back();
    print(vec);
    vec.erase(vec.begin() + 1);
    print(vec);
    vec.reserve(20);
    print(vec);
    vec.resize(15, 6);
    print(vec);
    vec.resize(25, 8);
    print(vec);
    return 0;
}
