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

bool add_arrays(int count, const float* inA, const float* inB, float** result) {
    // Metal creates a MTLDevice for each GPU.
    // If the system has multiple GPUs, Metal chooses one as the default.
    // Get the default GPU.
    auto device = MTL::CreateSystemDefaultDevice();
    if (!device) {
        std::cerr << "ERROR: failed to create the system default device\n";
        return false;
    }
    
    // When building the app, Xcode compiles shader functions found in .metal files,
    // then adds them to a default Metal library that it embeds in the app.
    // Get the default Metal library.
    auto library = device->newDefaultLibrary();
    if (!library) {
        std::cerr << "ERROR: failed to find the default library\n";
        return false;
    }
    
    // Get the "add_arrays" shader function from the default Metal library.
    auto add_func = library->newFunction(NS::String::string("add_arrays", NS::UTF8StringEncoding));
    if (!add_func) {
        std::cerr << "ERROR: failed to find the add_arrays shader function\n";
        return false;
    }
    
    // The function object is a proxy object for the MSL function, but it's not executable code.
    // The function is converted into executable code by creating a pipeline.
    // A pipeline specifies the steps the GPU performs to complete a specific task.
    // In Metal, a pipeline is represented by a pipeline state object.
    // The "add_arrays" shader function is a compute function, so compute pipeline is created.
    // A compute pipeline runs a single compute function.
    // When a pipeline state object is created, the device object finishes compiling the compute function for its specific GPU.
    NS::Error *err = nullptr;
    auto add_func_pso = device->newComputePipelineState(add_func, &err);
    if (!add_func_pso) {
        std::cerr << "ERROR: failed to create the pipeline state object: " << err->localizedDescription()->utf8String() << "\n";
        err->release();
        return false;
    }
    
    // To send work to the GPU, a command queue is needed.
    // Metal uses command queues to schedule commands.
    auto cmd_queue = device->newCommandQueue();
    if (!cmd_queue) {
        std::cerr << "ERROR: failed to create command queue\n";
        return false;
    }
    
    // Create an initialized buffer for the first input array accessible by both the GPU and CPU.
    // Metal manages each buffer as an opaque collection of bytes. The format is specified when it's used in a shader.
    auto bufA = device->newBuffer(inA, count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufA) {
        std::cerr << "ERROR: failed to create bufA\n";
        return false;
    }
    
    // Create an initialized buffer for the second input array accessible by both the GPU and CPU.
    // Metal manages each buffer as an opaque collection of bytes. The format is specified when it's used in a shader.
    auto bufB = device->newBuffer(inB, count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufB) {
        std::cerr << "ERROR: failed to create bufB\n";
        return false;
    }
    
    // Create an uninitialized buffer for the results array accessible by both the GPU and CPU.
    // Metal manages each buffer as an opaque collection of bytes. The format is specified when it's used in a shader.
    auto bufRes = device->newBuffer(count * sizeof(float), MTL::ResourceStorageModeShared);
    if (!bufRes) {
        std::cerr << "ERROR: failed to create bufRes\n";
        return false;
    }
    
    // Create a command buffer from the command queue to store GPU commands.
    // Committing the buffer allows the command queue to schedule it for execution on the GPU.
    auto cmd_buffer = cmd_queue->commandBuffer();
    if (!cmd_buffer) {
        std::cerr << "ERROR: failed to create command buffer\n";
        return false;
    }
    
    // To write commands into a command buffer, you use a command encoder for the specific kind of commands you want to encode.
    // Create a compute command encoder, which encodes a compute pass.
    // A compute pass holds a list of commands that execute compute pipelines.
    // Each compute command causes the GPU to create a grid of threads to execute on the GPU.
    auto compute_encoder = cmd_buffer->computeCommandEncoder();
    if (!compute_encoder) {
        std::cerr << "ERROR: failed to create compute encoder\n";
        return false;
    }
    
    // To encode a command, a series of method calls is made on the encoder to set state information like
    // the pipeline state object (PSO) and the arguments passed to the pipeline.
    // After this state information is set, a command can be encoded to execute the pipeline.
    // The encoder writes all of the state changes and command parameters into the command buffer.
    compute_encoder->setComputePipelineState(add_func_pso);
    // Metal automatically assigns indices for the buffer arguments in the order that the arguments appear
    // in the function declaration in MSL example, starting with 0.
    // Provide arguments using the same indices.
    // An offset of 0 means the command accesses the data from the beginning of the buffer.
    compute_encoder->setBuffer(bufA, 0, 0);
    compute_encoder->setBuffer(bufB, 0, 1);
    compute_encoder->setBuffer(bufRes, 0, 2);
    
    // Metal can create 1D, 2D, or 3D grids.
    // The add_arrays function uses a 1D array, so create a 1D grid of size cout x 1 x 1,
    // from which Metal generates indices in the range [0, count-1].
    auto grid_sz = MTL::Size(count, 1, 1);
    
    // Metal subdivides the grid into smaller grids called threadgroups.
    // Each threadgroup is calculated separately.
    // Metal can dispatch threadgroups to different processing elements on the GPU to speed up processing.
    // Start with the largest possible thread group, then shrink it down if that size is larger than the size of the data set.
    auto tg_sz = add_func_pso->maxTotalThreadsPerThreadgroup();
    if (tg_sz > count) {
        tg_sz = count;
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
    
    // Copy just a pointer to the results into the return argument.
    *result = static_cast<float*>(bufRes->contents());
    
    return true;
}
