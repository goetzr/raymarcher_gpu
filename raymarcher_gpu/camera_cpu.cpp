//
//  camera_cpu.cpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/14/26.
//

#include "camera_cpu.h"
#include "util.h"

void init(Camera& camera, FLOAT3 pos, FLOAT3 rotation,
          FLOAT fov_horiz, FLOAT sensor_aspect_ratio, FLOAT output_aspect_ratio, SensorFit sensor_fit,
          FLOAT clip_near, FLOAT clip_far) noexcept
{
    camera.pos = pos;
    camera.clip_near = clip_near;
    camera.clip_far = clip_far;
    
    // Calculate the basis vectors from the rotation.
    rotation.x = deg2rad(rotation.x);
    rotation.y = deg2rad(rotation.y);
    rotation.z = deg2rad(rotation.z);
    camera.basis = generate_basis(rotation);
    
    // Calculate the world to camera matrix from the basis vectors and the position.
    camera.world_to_camera = inverse_coord_transform(camera.basis, camera.pos);
    
    // Calculate the vertical FOV.
    camera.fov.horiz = deg2rad(fov_horiz);
    camera.fov.vert = 2 * std::atan(std::tan(camera.fov.horiz * 0.5f) / sensor_aspect_ratio);
    // Adjust the FOV for an aspect ratio mismatch between the sensor and the output raster.
    adjust_fov(camera, sensor_aspect_ratio, output_aspect_ratio, sensor_fit);

    // NOTE: This is unnecessary for a raymarcher. The desired FOV is specified directly.
    // fov_.horiz = 2 * std::atan2(sensor_sz_.width  * kInchesToMm / 2,  focal_len_);
    // fov_.vert  = 2 * std::atan2(sensor_sz_.height * kInchesToMm / 2, focal_len_);

    // NOTE: Unnecessary for a raymarcher.
    // canvas_sz_.width  = 2 * std::tan(fov_horiz_ / 2) * clip_near_;
    // canvas_sz_.height = 2 * std::tan(fov_vert_  / 2) * clip_near_;
}

void adjust_fov(Camera& camera, FLOAT sensor_aspect_ratio, FLOAT output_aspect_ratio, SensorFit sensor_fit) noexcept {
    switch (sensor_fit) {
        case SensorFit::Fill:
            if (sensor_aspect_ratio > output_aspect_ratio) {
                camera.fov.horiz = 2 * std::atan(std::tan(camera.fov.vert * 0.5) * output_aspect_ratio);
            } else if (sensor_aspect_ratio < output_aspect_ratio) {
                camera.fov.vert = 2 * std::atan(std::tan(camera.fov.horiz * 0.5) / output_aspect_ratio);
            }
            break;
        case SensorFit::Overscan:
            if (sensor_aspect_ratio > output_aspect_ratio) {
                camera.fov.vert = 2 * std::atan(std::tan(camera.fov.horiz * 0.5) / output_aspect_ratio);
            } else if (sensor_aspect_ratio < output_aspect_ratio) {
                camera.fov.horiz = 2 * std::atan(std::tan(camera.fov.vert * 0.5) * output_aspect_ratio);
            }
            break;
        default:
            // Never get here.
    }
}
