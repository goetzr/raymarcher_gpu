//
//  raymarcher.hpp
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
    bool initialize(std::string& error_msg) noexcept;
    ~Raymarcher();
    
private:
    void release() noexcept;
    
private:
    bool init_ok_ = false;
    NS::SharedPtr<MTL::Device> device_;
};
