//
//  util.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#pragma once

#include "core_types.h"
#include "common.h"

constexpr float deg2rad(float d) noexcept {
    return d * std::numbers::pi_v<float> / 180;
}

constexpr float rad2deg(FLOAT r) noexcept {
    return r * 180 / std::numbers::pi_v<float>;
}

FLOAT3x3 generate_basis(FLOAT3 rotation) noexcept;
CoordTransform inverse_coord_transform(FLOAT3x3 rotation, FLOAT3 translation) noexcept;
