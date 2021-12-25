#pragma once

float sign(float n)
{
    return n > 0.0F ? 1.0F : -1.0F;
}

float approach(float n, float target, float amount)
{
    if (n > target)
    {
        return fmaxf(n - amount, target);
    }
    else
    {
        return fminf(n + amount, target);
    }
}
