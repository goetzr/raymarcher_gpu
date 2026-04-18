//
//  raymarch_pixel.metal
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/11/26.
//

#include <metal_stdlib>
using namespace metal;

#include "camera.hpp"

kernel void raymarch_pixel(constant uint2 output_sz[[buffer(0)]]
                           constant Camera& camera [[buffer(1)]],
                           constant Fov& fov [[buffer(2)]],
                           constant Object* objects [[buffer(3)]],
                           constant int32_t& num_objs [[buffer(4)]],
                           texture2d<uint32_t, access::write> pixels_tex [[texture(0)],
                           uint2 gid [[thread_position_in_grid]])
{
    // A ray that has marched further than this distance is defined to have missed all objects in the scene.
    static constexpr float kMaxDist = 200;
    // A ray that has been marched this many steps is defined to have missed all objects in the scene.
    static constexpr int kMaxSteps = 50;
    // A ray has hit an object in the scene if its distance to that object is less than Epsilon.
    static constexpr float kEpsilon = 0.001;
    
    // NOTE: The raster is sampled from the portion of the scene visible on the virtual canvas.
    
    // NOTE: Graphics APIs follow the convention used in the calculations that follow.
    //       scratchapixel.com follows a different convention:
    //         pipeline: raster space -> NDC space -> screen space, where
    //         NDC space has its origin at the bottom-left and coordinates are in the range [0, 1]
    //         screen space has its origin at the center and coordinates are in the range [-1, 1]
    //       Both conventions arrive at the same result.
    
    // Check for out-of-bounds if the texture dimensions aren't a multiple of threadgroup size.
    if (gid.x >= pixels_tex.get_width() || gid.y >= pixels_tex.get_height()) {
        return;
    }
    
    // Convert from raster coordinates to Normalized Screen Coordinates (NSC).
    // NSC space has its origin at the top-left and coordinates are in the range [0, 1].
    float u = (gid.x + 0.5f) / output_sz.x;
    float v = (gid.y + 0.5f) / kOutputSize.y;
    
    // Convert from NSC to Normalized Device Coordinates (NDC).
    // NDC space has its origin at the center and coordinates are in the range [-1, 1].
    float x_ndc = 2 * u - 1;
    float y_ndc = 1 - 2 * v;
    
    // Apply the NDC coordinates to the FOV to get a ray direction vector in camera space.
    // Assume the virtual canvas is 1 unit away from the camera along the z-axis (simplifies tan).
    float x_cam = x_ndc * std::tan(fov.horiz * 0.5f);
    float y_cam = y_ndc * std::tan(fov.vert  * 0.5f);
    float z_cam = -1;
    
    // Normalize the ray direction vector.
    Vec3 ray_dir_cam = Vec3 { x_cam, y_cam, z_cam }.normalize();
    
    // Convert the ray direction vector into world coordinates.
    Vec3 ray_dir = camera.basis() * ray_dir_cam;
    
    // Use the distance to the closest object as the initial ray length.
    Vec3 current_position = camera.position();
    auto closest_obj = scene.closest_object(camera.position());
    if (!closest_obj) {
        return;
    }
    float ray_len = closest_obj.value().distance;
    
    // Return a black pixel if the ray doesn't hit an object.
    uint32_t pixel_argb = 0xff000000;
    
    // March the ray until it hits an object in the scene or misses all objects.
    for (int step = 0; step != kMaxSteps && ray_len <= kMaxDist; ++step) {
        current_position = camera.position() + ray_dir * ray_len;
        
        closest_obj = scene.closest_object(current_position);
        if (!closest_obj) {
            return;
        }
        
        if (closest_obj.value().distance < kEpsilon) {
            // Ray hit an object.
            
            // Determine if the point should be visible by converting it back into camera coordinates
            // and checking it against the near/far clipping planes.
            Vec3 cur_pos_cam = camera.world_to_camera().rotation * current_position +
            camera.world_to_camera().translation;
            
            // Negate the z coordinate to get the point's distance from the camera.
            float dist_z = -cur_pos_cam.z;
            if (dist_z >= camera.clip_near() && dist_z <= camera.clip_far()) {
                // The position should be visible. Return a color of red.
                pixel_argb = 0xffff0000;
            }
            
            break;
        }
        
        ray_len += closest_obj.value().distance;
    }
    
    pixels_text.write(pixel_argb, gid);
}
