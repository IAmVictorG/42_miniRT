#include "../../includes/miniRT.h"

t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

t_vec3 vec3_subtract(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

t_vec3 vec3_multiply_scalar(t_vec3 a, float scalar)
{
    t_vec3 result;
    result.x = a.x * scalar;
    result.y = a.y * scalar;
    result.z = a.z * scalar;
    return result;
}

t_vec3 vec3_normalize(t_vec3 v)
{
    float vec3_length;    
    t_vec3 normalized;

    vec3_length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (vec3_length == 0.0)
    {
        normalized.x = 0.0;
        normalized.y = 0.0;
        normalized.z = 0.0;
    }
    else
    {
        normalized.x = v.x / vec3_length;
        normalized.y = v.y / vec3_length;
        normalized.z = v.z / vec3_length;
    }
    return normalized;
}

t_vec3 vec3_cross_product(t_vec3 a, t_vec3 b)
{
    t_vec3 result;

    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;

    return result;
}

float vec3_dot_product(t_vec3 a, t_vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(t_vec3 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

t_vec3 perturb_vector(t_vec3 v, float roughness)
{

    t_vec3 random_vector;
    float dx = (float)rand() / RAND_MAX * 2.0f - 1.0f;
    float dy = (float)rand() / RAND_MAX * 2.0f - 1.0f;
    float dz = (float)rand() / RAND_MAX * 2.0f - 1.0f;

    random_vector = vec3_multiply_scalar((t_vec3){dx, dy, dz}, roughness);
    return vec3_add(v, random_vector);
}

t_vec3 reflect(t_vec3 v, t_vec3 n, float roughness)
{
    t_vec3 perfect_reflection;

    perfect_reflection = vec3_subtract(v, vec3_multiply_scalar(n, 2.0f * vec3_dot_product(v, n)));
    return perturb_vector(perfect_reflection, roughness);
}
