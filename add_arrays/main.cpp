//
//  main.cpp
//  add_arrays
//
//  Created by Russ Goetz on 4/8/26.
//

#include <iostream>

#include "add_arrays.hpp"

int main(int argc, const char * argv[]) {
    float arr1[] = { 1, 2, 3 };
    float arr2[] = { 4, 5, 6 };
    float* results = nullptr;
    bool success = add_arrays(3, arr1, arr2, &results);
    if (success) {
        std::cout << "Succeeded\n";
        std::cout << "results[0] = " << results[0] << "\n";
    } else {
        std::cerr << "Failed\n";
    }
}
