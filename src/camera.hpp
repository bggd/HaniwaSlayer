#pragma once

#include "gmath.hpp"

struct Camera {
    Vector3 position = vec3(0.0F, 0.0F, 1.0F);

    Matrix4 getMVP()
    {
        Matrix4 view = mat4LookAt(position, vec3(position.x, position.y, -1.0F), vec3(0.0F, 1.0F, 0.0));
        Matrix4 proj = mat4CreateOrthographicOffCenter(-320.0F, 320.0F, -240, 240, 0.05F, 100.0F);
        return mat4Multiply(view, proj);
    }
};
