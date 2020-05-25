#include <iostream>

#include "tuple_operations.h"

int main(){
    auto tup1 = std::make_tuple(1.5, 1.2, 2);
    auto tup2 = std::make_tuple(2.0,   2, 3);
    
    auto tup3 = tuple_operations::add(tup1, tup2);
    auto tup4 = tuple_cast<int>::all(tup3);
    auto tup5 = tup1 + tup4;
    
    std::cout << "tup1 + tup2:  " << std::get<0>(tup3) << " " << std::get<1>(tup3) << " " << std::get<2>(tup3) << std::endl;
    std::cout << "(int)tup3:    " << std::get<0>(tup4) << " " << std::get<1>(tup4) << " " << std::get<2>(tup4) << std::endl;
    std::cout << "tup1 + tup4:  " << std::get<0>(tup5) << " " << std::get<1>(tup5) << " " << std::get<2>(tup5) << std::endl;

    return 0;
}

