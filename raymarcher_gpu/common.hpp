//
//  core_types.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#pragma once

#include <algorithm>
#include <cmath>
#include <numbers>
#include <optional>
#include <string>
#include <iostream>

#include "core_types.hpp"
#include "vec3.hpp"
#include "mat3.hpp"

struct RectF {
    FLOAT width;
    FLOAT height;
};

struct RectI {
    int32_t width;
    int32_t height;
};

struct EulerAngles {
    FLOAT x;
    FLOAT y;
    FLOAT z;
};

// A 4x4 coordinate transformation matrix, split out into its rotation matrix and translation vector.
struct CoordTransform {
    Mat3 rotation;
    Vec3 translation;
};
