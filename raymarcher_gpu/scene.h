//
//  scene.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "object.hpp"

constexpr int MaxObjects = 100;

struct ClosestObject {
    int index;
    double distance;
};

class Scene {
public:
    Scene(std::array<Object, MaxObjects> objects)
        : objects_{objects}
    {}
    std::optional<ClosestObject> closest_object(const Vec3& p) const noexcept;

private:
    std::array<Object, MaxObjects> objects_;
};
