//
//  object.h
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"
#include "sphere.h"
#include "cube.h"
#include "box2d.h"

enum class ObjectType {
    Unused,
    Sphere,
    Cube,
    Box2D
};

class Object {
public:
    Object() noexcept : type_{ObjectType::Unused} {}
    Object(THREAD_ADDR_SPACE Sphere&& sphere) NOEXCEPT : type_{ObjectType::Sphere}, sphere_{sphere} {}
    Object(THREAD_ADDR_SPACE Cube&& cube) NOEXCEPT : type_{ObjectType::Cube}, cube_{cube} {}
    Object(THREAD_ADDR_SPACE Box2D&& box2d) NOEXCEPT : type_{ObjectType::Box2D}, box2d_{box2d} {}
    FLOAT sdf(THREAD_ADDR_SPACE const FLOAT3& p) const CONST_ADDR_SPACE;

private:
    ObjectType type_;
    union {
        Sphere sphere_;
        Cube cube_;
        Box2D box2d_;
    };
};
