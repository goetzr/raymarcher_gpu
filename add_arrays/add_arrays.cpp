//
//  add_arrays.cpp
//  add_arrays
//
//  Created by Russ Goetz on 4/8/26.
//

#include "add_arrays.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

bool add_arrays(int count, const float* inA, const float* inB, float* result) {
    auto device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "ERROR: failed to create system default device\n";
        return false;
    }
    
    auto library = device->newDefaultLibrary();
    if (!library) {
        std::cerr << "ERROR: failed to find the default library\n";
        return false;
    }
    
    auto add_func = library->newFunction(NS::String::string("add_arrays", NS::UTF8StringEncoding));
    if (!add_func) {
        std::cerr << "ERROR: failed to find the add_arrays function\n";
        return false;
    }
    
    NS::Error *err = nullptr;
    auto add_func_pso = device->newComputePipelineState(add_func, &err);
    if (!add_func_pso) {
        std::cerr << "ERROR: failed to create pipeline: " << err->localizedDescription()->utf8String() << "\n";
        err->release();
        return false;
    }
    
    auto cmd_queue = device->newCommandQueue();
    if (!cmd_queue) {
        std::cerr << "ERROR: failed to create command queue\n";
        return false;
    }
    
    auto bufA = device->newBuffer(inA, count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufA) {
        std::cerr << "ERROR: failed to create bufA\n";
        return false;
    }
    
    auto bufB = device->newBuffer(inB, count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufB) {
        std::cerr << "ERROR: failed to create bufB\n";
        return false;
    }
    
    auto bufRes = device->newBuffer(count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufRes) {
        std::cerr << "ERROR: failed to create bufRes\n";
        return false;
    }
    
    auto cmd_buffer = cmd_queue->commandBuffer();
    if (!cmd_buffer) {
        std::cerr << "ERROR: failed to create command buffer\n";
        return false;
    }
    
    auto compute_encoder = cmd_buffer->computeCommandEncoder();
    if (!compute_encoder) {
        std::cerr << "ERROR: failed to create compute encoder\n";
        return false;
    }
    
    return true;
}
