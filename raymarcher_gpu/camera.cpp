//
//  camera.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#include "camera.hpp"
#include "util.hpp"

Camera::Camera(FLOAT sensor_aspect_ratio, FLOAT fov_horiz, SensorFit sensor_fit, FLOAT clip_near, FLOAT clip_far,
               FLOAT3 pos, EulerAngles rotation) noexcept
        : sensor_aspect_ratio_{sensor_aspect_ratio}, fov_{}, sensor_fit_{sensor_fit},
          clip_near_{clip_near}, clip_far_{clip_far}, pos_{pos}, basis_{},
          world_to_camera_{}
{
    fov_.horiz = deg2rad(fov_horiz);
    fov_.vert = 2 * std::atan(std::tan(fov_.horiz * 0.5f) / sensor_aspect_ratio);

    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    basis_ = generate_basis(rotation);

    // NOTE: This is unnecessary for a raymarcher. The desired FOV is specified directly.
    // fov_.horiz = 2 * std::atan2(sensor_sz_.width  * kInchesToMm / 2,  focal_len_);
    // fov_.vert  = 2 * std::atan2(sensor_sz_.height * kInchesToMm / 2, focal_len_);

    // NOTE: Unnecessary for a raymarcher.
    // canvas_sz_.width  = 2 * std::tan(fov_horiz_ / 2) * clip_near_;
    // canvas_sz_.height = 2 * std::tan(fov_vert_  / 2) * clip_near_;

    // Calculate the world to camera matrix from the basis vectors and the position.
    world_to_camera_ = inverse_coord_transform(basis_, pos_);
}
