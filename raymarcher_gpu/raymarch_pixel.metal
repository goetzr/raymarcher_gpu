//
//  raymarch_pixel.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include <metal_stdlib>

#include "shared.h"

using namespace metal;

kernel void raymarch_pixel(constant int32_t& px [[buffer(0)]],
                           constant int32_t& py [[buffer(1)]],
                           constant Camera& camera [[buffer(2)]],
                           constant Fov& fov [[buffer(3)]],
                           constant Object* objects [[buffer(4)]],
                           constant int32_t& num_objs [[buffer(5)]]
                           device uint32_t* pixels [[buffer0]])
{
    // A ray that has marched further than this distance is defined to have missed all objects in the scene.
    static constexpr double kMaxDist = 200;
    // A ray that has been marched this many steps is defined to have missed all objects in the scene.
    static constexpr int kMaxSteps = 40;
    // A ray has hit an object in the scene if its distance to that object is less than Epsilon.
    static constexpr double kEpsilon = 0.001;

    // NOTE: The raster is sampled from the portion of the scene visible on the virtual canvas.

    // NOTE: Graphics APIs follow the convention used in the calculations that follow.
    //       scratchapixel.com follows a different convention:
    //         pipeline: raster space -> NDC space -> screen space, where
    //         NDC space has its origin at the bottom-left and coordinates are in the range [0, 1]
    //         screen space has its origin at the center and coordinates are in the range [-1, 1]
    //       Both conventions arrive at the same result.

    // Convert from raster coordinates to Normalized Screen Coordinates (NSC).
    // NSC space has its origin at the top-left and coordinates are in the range [0, 1].
    double u = (px + 0.5) / kOutputSize.width;
    double v = (py + 0.5) / kOutputSize.height;

    // Convert from NSC to Normalized Device Coordinates (NDC).
    // NDC space has its origin at the center and coordinates are in the range [-1, 1].
    double x_ndc = 2 * u - 1;
    double y_ndc = 1 - 2 * v;

    // Apply the NDC coordinates to the FOV to get a ray direction vector in camera space.
    // Assume the virtual canvas is 1 unit away from the camera along the z-axis (simplifies tan).
    double x_cam = x_ndc * std::tan(fov.horiz * 0.5);
    double y_cam = y_ndc * std::tan(fov.vert  * 0.5);
    double z_cam = -1;

    // Normalize the ray direction vector.
    Vec3 ray_dir_cam = Vec3 { x_cam, y_cam, z_cam }.normalize();

    // Convert the ray direction vector into world coordinates.
    Vec3 ray_dir = camera.basis() * ray_dir_cam;

    // Use the distance to the closest object as the initial ray length.
    Vec3 current_position = camera.position();
    auto closest_obj = scene.closest_object(camera.position());
    if (!closest_obj) {
        return std::nullopt;
    }
    double ray_len = closest_obj.value().distance;

    // Return a black pixel if the ray doesn't hit an object.
    uint32_t pixel_argb = 0xff000000;

    // March the ray until it hits an object in the scene or misses all objects.
    for (int step = 0; step != kMaxSteps && ray_len <= kMaxDist; ++step) {
        current_position = camera.position() + ray_dir * ray_len;

        closest_obj = scene.closest_object(current_position);
        if (!closest_obj) {
            return std::nullopt;
        }
        
        if (closest_obj.value().distance < kEpsilon) {
            // Ray hit an object.

            // Determine if the point should be visible by converting it back into camera coordinates
            // and checking it against the near/far clipping planes.
            Vec3 cur_pos_cam = camera.world_to_camera().rotation * current_position +
                camera.world_to_camera().translation;

            // Negate the z coordinate to get the point's distance from the camera.
            double dist_z = -cur_pos_cam.z;
            if (dist_z >= camera.clip_near() && dist_z <= camera.clip_far()) {
                // The position should be visible. Return a color of red.
                pixel_argb = 0xffff0000;
            }

            break;
        }

        ray_len += closest_obj.value().distance;
    }

    return std::make_optional(pixel_argb);
}
