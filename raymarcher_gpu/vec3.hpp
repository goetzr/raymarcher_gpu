//
//  vec3.hpp
//  raymarcher_gpu
//
//  Created by Russ Goetz on 4/15/26.
//

#pragma once

#include "shared.h"

struct Vec3 {
    constexpr FLOAT length_squared() const noexcept {
        return x * x + y * y + z * z;
    }

    FLOAT length() const noexcept {
        return std::sqrt(length_squared());
    }

    Vec3 normalize() const noexcept {
        FLOAT len = length();
        if (len == 0) {
            return { 0, 0, 0 };
        }
        return { x / len, y / len, z / len };
    }

    FLOAT x;
    FLOAT y;
    FLOAT z;
};

constexpr Vec3 operator-(const Vec3& v) noexcept {
    return { -v.x, -v.y, -v.z };
}

constexpr Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

constexpr Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

constexpr Vec3 operator*(const Vec3& v, FLOAT d) {
    return { v.x * d, v.y * d, v.z * d };
}

constexpr Vec3 operator*(FLOAT d, const Vec3& v) {
    return v * d;
}

constexpr FLOAT dot(const Vec3& a, const Vec3& b) noexcept {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
