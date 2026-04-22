//
//  raymarcher.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

class Raymarcher {
public:
    bool init(std::string& error_msg) noexcept;
    
private:
    constexpr static const char* kRaymarchPixelFuncName = "raymarch_pixel";
    
    bool init_ok_ = false;
    NS::SharedPtr<MTL::Device> device_{};
    NS::SharedPtr<MTL::ComputePipelineState> raymarch_px_pso_{};
    NS::SharedPtr<MTL::CommandQueue> cmd_queue_{};
};
