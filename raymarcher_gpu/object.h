//
//  object.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "float3.h"
#include "sphere.hpp"
#include "cube.hpp"
#include "box2d.hpp"
#include "common.hpp"

enum class ObjectType {
    Unused,
    Sphere,
    Cube,
    Box2D
};

class Object {
public:
    Object() noexcept : type_{ObjectType::Unused} {}
    Object(Sphere&& sphere) noexcept : type_{ObjectType::Sphere}, sphere_{sphere} {}
    Object(Cube&& cube) noexcept : type_{ObjectType::Cube}, cube_{cube} {}
    Object(Box2D&& box2d) noexcept : type_{ObjectType::Box2D}, box2d_{box2d} {}
    FLOAT sdf(const Vec3& p) const noexcept;

private:
    ObjectType type_;
    union {
        Sphere sphere_;
        Cube cube_;
        Box2D box2d_;
    };
};
