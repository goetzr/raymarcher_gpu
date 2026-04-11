//
//  raymarcher.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include "raymarcher.hpp"

bool Raymarcher::initialize(std::string& error_msg) noexcept {
    std::ostringstream oss_err;
    
    // Metal creates a MTLDevice for each GPU.
    // If the system has multiple GPUs, Metal chooses one as the default.
    // Get the default GPU.
    device_ = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    if (!device_) {
        oss_err << "failed to create the system default device";
        error_msg = oss_err.str();
        return false;
    }
    
    // When building the app, Xcode compiles shader functions found in .metal files,
    // then adds them to a default Metal library that it embeds in the app.
    // Get the default Metal library.
    auto library = device_->newDefaultLibrary();
    if (!library) {
        oss_err << "failed to find the default library";
        error_msg = oss_err.str();
        return false;
    }
    
    // Get the "add_arrays" shader function from the default Metal library.
    auto add_func = library->newFunction(NS::String::string("add_arrays", NS::UTF8StringEncoding));
    if (!add_func) {
        oss_err << "failed to find the add_arrays shader function";
        error_msg = oss_err.str();
        return false;
    }
    
    // The function object is a proxy object for the MSL function, but it's not executable code.
    // The function is converted into executable code by creating a pipeline.
    // A pipeline specifies the steps the GPU performs to complete a specific task.
    // In Metal, a pipeline is represented by a pipeline state object.
    // The "add_arrays" shader function is a compute function, so a compute pipeline is created.
    // A compute pipeline runs a single compute function.
    // When a pipeline state object is created, the device object finishes compiling the compute function for its specific GPU.
    NS::Error *err = nullptr;
    auto add_func_pso = device_->newComputePipelineState(add_func, &err);
    if (!add_func_pso) {
        oss_err << "failed to create the pipeline state object: " << err->localizedDescription()->utf8String() << "";
        error_msg = oss_err.str();
        err->release();
        return false;
    }
    
    // To send work to the GPU, a command queue is needed.
    // Metal uses command queues to schedule commands.
    auto cmd_queue = device_->newCommandQueue();
    if (!cmd_queue) {
        oss_err << "failed to create command queue";
        error_msg = oss_err.str();
        return false;
    }
    
    init_ok_ = true;
    return true;
}
