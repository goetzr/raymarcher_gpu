//
//  metal.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/6/26.
//

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace metal {

MTL::Device* create_device() {
    auto device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "ERROR: failed to create Metal device\n";
        return nullptr;
    }
    return device;
}

}
