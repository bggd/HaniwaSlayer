#pragma once

#include "gmath.hpp"

struct Camera {
    Vector3 position = vec3(0.0F, 0.0F, 1.0F);
    Matrix4 proj = mat4Identity();

    void setProjection(const Matrix4& projection)
    {
        proj = projection;
    }

    Matrix4 getMVP()
    {
        Matrix4 view = mat4LookAt(position, vec3(position.x, position.y, -1.0F), vec3(0.0F, 1.0F, 0.0));
        return mat4Multiply(view, proj);
    }
};
