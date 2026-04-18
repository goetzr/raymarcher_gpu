//
//  main.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/5/26.
//

#include "common.hpp"
#include "raymarcher.hpp"

int main(int argc, const char * argv[]) {
    Raymarcher marcher;
    std::string error;
    if (!marcher.initialize(error)) {
        std::cerr << "ERROR: " << error << "\n";
        return 1;
    }
    return 0;
}
