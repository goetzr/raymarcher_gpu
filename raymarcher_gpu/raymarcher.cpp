//
//  raymarcher.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include "raymarcher.h"
#include "shared.h"

bool Raymarcher::init(std::string& error_msg) noexcept {
    std::ostringstream oss_err;
    
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

bool Raymarcher::march_rays(const NS::SharedPtr<MTL::Buffer>& output_sz,
                            const NS::SharedPtr<MTL::Buffer>& camera,
                            const NS::SharedPtr<MTL::Buffer>& scene,
                            NS::SharedPtr<MTL::Buffer>& pixels,
                            std::string& error_msg) noexcept
{
    // Create a command buffer from the command queue to store GPU commands.
    // Committing the buffer allows the command queue to schedule it for execution on the GPU.
    auto cmd_buffer = NS::TransferPtr(cmd_queue_->commandBuffer());
    if (!cmd_buffer) {
        error_msg = "failed to create command buffer";
        return false;
    }
    
    // To write commands into a command buffer, you use a command encoder for the specific kind of commands you want to encode.
    // Create a compute command encoder, which encodes a compute pass.
    // A compute pass holds a list of commands that execute compute pipelines.
    // Each compute command causes the GPU to create a grid of threads to execute on the GPU.
    auto compute_encoder = NS::TransferPtr(cmd_buffer->computeCommandEncoder());
    if (!compute_encoder) {
        error_msg = "failed to create compute encoder";
        return false;
    }
    
    // To encode a command, a series of method calls is made on the encoder to set state information like
    // the pipeline state object (PSO) and the arguments passed to the pipeline.
    // After this state information is set, a command can be encoded to execute the pipeline.
    // The encoder writes all of the state changes and command parameters into the command buffer.
    compute_encoder->setComputePipelineState(raymarch_px_pso_.get());
    // Metal automatically assigns indices for the buffer arguments in the order that the arguments appear
    // in the function declaration in MSL example, starting with 0.
    // Provide arguments using the same indices.
    // An offset of 0 means the command accesses the data from the beginning of the buffer.
    compute_encoder->setBuffer(output_sz.get(), 0, 0);
    compute_encoder->setBuffer(camera.get(), 0, 1);
    compute_encoder->setBuffer(scene.get(), 0, 2);
    compute_encoder->setBuffer(pixels.get(), 0, 3);
    
    // Metal can create 1D, 2D, or 3D grids.
    // The raymarch_pixel function uses a 2D pixel array, so create a 2D grid of size width x height x 1,
    // from which Metal generates 2D indices in the range [0, width-1] and [0, height-1].
    RectI* p_output_sz = static_cast<RectI*>(output_sz->contents());
    auto grid_sz = MTL::Size(p_output_sz->width, p_output_sz->height, 1);
    
    // Metal subdivides the grid into smaller grids called threadgroups.
    // Each threadgroup is calculated separately.
    // Metal can dispatch threadgroups to different processing elements on the GPU to speed up processing.
    // Start with the largest possible thread group, then shrink it down if that size is larger than the size of the data set.
    auto tg_sz = raymarch_px_pso_->maxTotalThreadsPerThreadgroup();
    auto pixel_cnt = p_output_sz->width * p_output_sz->height;
    if (tg_sz > pixel_cnt) {
        tg_sz = pixel_cnt;
    }
    auto thread_grp_sz = MTL::Size(tg_sz, 1, 1);
    
    // Encode the compute command to the command queue to dispatch the grid of threads.
    // When the GPU executes this command, it uses the provided state and command arguments
    // to dispatch threads to perform the computation.
    // The same steps can be followed using the encoder to encode multiple compute commands into the compute pass
    // without performing any redundant steps.
    // For example, the pipeline state object might be set once, then arguments set and a command encoded
    // for each collection of buffers to process.
    compute_encoder->dispatchThreads(grid_sz, thread_grp_sz);
    
    // When finished adding commands to the compute pass, end the encoding process to close out the compute pass.
    compute_encoder->endEncoding();
    
    // Allow the command queue to schedule the compute pass by committing the command buffer to the command queue.
    // The command queue created the command buffer, so committing the buffer always places it on that queue.
    // After the command buffer is committed, Metal asynchronously prepares the commands for execution and then
    // schedules the command buffer to execute on the GPU.
    // After the GPU executes all the commands in the command buffer, Metal marks the command buffer as complete.
    cmd_buffer->commit();
    
    // Wait until the command buffer is complete.
    cmd_buffer->waitUntilCompleted();
    
    return true;
}
