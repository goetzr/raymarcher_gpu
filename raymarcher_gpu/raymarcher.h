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
    Raymarcher(NS::SharedPtr<MTL::Device> device, NS::SharedPtr<MTL::Buffer> pixels)
        : device_{device}, pixels_{pixels}
    {};
    bool init(std::string& error_msg) noexcept;
    bool march_rays(const NS::SharedPtr<MTL::Buffer>& output_sz,
                    const NS::SharedPtr<MTL::Buffer>& camera,
                    const NS::SharedPtr<MTL::Buffer>& scene,
                    NS::SharedPtr<MTL::Buffer>& pixels,
                    std::string& error_msg
    ) noexcept;
    
private:
    constexpr static const char* kRaymarchPixelFuncName = "raymarch_pixel";
    
    NS::SharedPtr<MTL::Device> device_;
    NS::SharedPtr<MTL::Buffer> pixels_;
    
    bool init_ok_ = false;
    NS::SharedPtr<MTL::ComputePipelineState> raymarch_px_pso_{};
    NS::SharedPtr<MTL::CommandQueue> cmd_queue_{};
};
