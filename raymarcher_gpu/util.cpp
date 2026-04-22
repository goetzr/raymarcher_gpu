//
//  util.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#include "util.h"

FLOAT3x3 generate_basis(FLOAT3 rotation) noexcept {
    // Point your thumb along the positive direction of the axis of rotation
    // then curl your fingers in the direction of the rotation.
    FLOAT3x3 rot_x {
        FLOAT3 { 1, 0, 0 },
        FLOAT3 { 0, std::cos(rotation.x), std::sin(rotation.x) },
        FLOAT3 { 0, -std::sin(rotation.x), std::cos(rotation.x) }
    };

    FLOAT3x3 rot_y {
        FLOAT3 { std::cos(rotation.y), 0, -std::sin(rotation.y) },
        FLOAT3 { 0, 1, 0 },
        FLOAT3 { std::sin(rotation.y), 0, std::cos(rotation.y) }
    };

    FLOAT3x3 rot_z {
        FLOAT3 { std::cos(rotation.z), std::sin(rotation.z), 0 },
        FLOAT3 { -std::sin(rotation.z), std::cos(rotation.z), 0 },
        FLOAT3 { 0, 0, 1 }
    };

    // Standard, unrotated basis vectors.
    FLOAT3x3 basis  = {
        FLOAT3 { 1, 0, 0 },
        FLOAT3 { 0, 1, 0 },
        FLOAT3 { 0, 0, 1 }
    };

    // Use an XYZ Euler rotation to rotate the standard basis vectors.
    return rot_z * rot_y * rot_x * basis;
}

CoordTransform inverse_coord_transform(FLOAT3x3 rotation, FLOAT3 translation) noexcept {
    // Because the rotation matrix is orthonormal, its inverse is its transpose.
    FLOAT3x3 inv_rotation = transpose(rotation);

    // Invert the translation using the inverted rotation matrix.
    FLOAT3 inv_translation = -(inv_rotation * translation);

    return { inv_rotation, inv_translation };
}
