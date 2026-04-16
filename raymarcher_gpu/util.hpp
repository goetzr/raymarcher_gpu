//
//  util.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#pragma once

#include "common.hpp"

constexpr FLOAT deg2rad(FLOAT d) noexcept SH_ADDR_SPACE {
    return d * std::numbers::pi / 180;
}

constexpr FLOAT rad2deg(FLOAT r) noexcept SH_ADDR_SPACE {
    return r * 180 / std::numbers::pi;
}

Mat3 generate_basis(EulerAngles rotation) noexcept SH_ADDR_SPACE;
CoordTransform inverse_coord_transform(Mat3 rotation, Vec3 translation) noexcept SH_ADDR_SPACE;
