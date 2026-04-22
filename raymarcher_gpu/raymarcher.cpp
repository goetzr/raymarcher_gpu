//
//  raymarcher.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include "raymarcher.h"

bool Raymarcher::init(std::string& error_msg) noexcept {
    std::ostringstream oss_err;
    
    // Metal creates a MTLDevice for each GPU.
    // If the system has multiple GPUs, Metal chooses one as the default.
    // Get the default GPU.
    device_ = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    if (!device_) {
        error_msg = "failed to create the system default device";
        return false;
    }
    
    // Nested scope to ensure library and funnction objects are released.
    {
        // When building the app, Xcode compiles shader functions found in .metal files,
        // then adds them to a default Metal library that it embeds in the app.
        // Get the default Metal library.
        auto library = NS::TransferPtr(device_->newDefaultLibrary());
        if (!library) {
            error_msg = "failed to find the default library";
            return false;
        }
        
        // Get the raymarch pixel shader function from the default Metal library.
        auto raymarch_px_fn = NS::TransferPtr(library->newFunction(NS::String::string(kRaymarchPixelFuncName, NS::UTF8StringEncoding)));
        if (!raymarch_px_fn) {
            oss_err << "failed to find the " << kRaymarchPixelFuncName << " shader function";
            error_msg = oss_err.str();
            return false;
        }
        
        // The function object is a proxy object for the MSL function, but it's not executable code.
        // The function is converted into executable code by creating a pipeline.
        // A pipeline specifies the steps the GPU performs to complete a specific task.
        // In Metal, a pipeline is represented by a pipeline state object.
        // The raymarch pixel shader function is a compute function, so a compute pipeline is created.
        // A compute pipeline runs a single compute function.
        // When a pipeline state object is created, the device object finishes compiling the compute function for its specific GPU.
        NS::Error *err = nullptr;
        raymarch_px_pso_ = NS::TransferPtr(device_->newComputePipelineState(raymarch_px_fn.get(), &err));
        if (!raymarch_px_pso_) {
            oss_err << "failed to create the pipeline state object for the " << kRaymarchPixelFuncName << " shader function: "
                    << err->localizedDescription()->utf8String();
            error_msg = oss_err.str();
            err->release();
            return false;
        }
    }
    
    // To send work to the GPU, a command queue is needed.
    // Metal uses command queues to schedule commands.
    cmd_queue_ = NS::TransferPtr(device_->newCommandQueue());
    if (!cmd_queue_) {
        error_msg = "failed to create command queue";
        return false;
    }
    
    init_ok_ = true;
    return true;
}
