#include "../includes/header.h"

t_matrix4x4 create_rotation_matrix_axis_angle(t_vec3 axis, float angle)
{
    t_matrix4x4 mat;
    float c = cos(angle);
    float s = sin(angle);
    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    mat.m[0][0]  = x * x * t + c;
    mat.m[0][1]  = x * y * t + z * s;
    mat.m[0][2]  = x * z * t - y * s;
    mat.m[0][3]  = 0;
    mat.m[1][0]  = x * y * t - z * s;
    mat.m[1][1]  = y * y * t + c;
    mat.m[1][2]  = y * z * t + x * s;
    mat.m[1][3]  = 0;
    mat.m[2][0]  = x * z * t + y * s;
    mat.m[2][1]  = y * z * t - x * s;
    mat.m[2][2]  = z * z * t + c;
    mat.m[2][3]  = 0;
    mat.m[3][0]  = 0;
    mat.m[3][1]  = 0;
    mat.m[3][2]  = 0;
    mat.m[3][3]  = 1;
    
    return mat;
}


t_matrix4x4 create_rotation_matrix_x(float angle_rad)
{
    t_matrix4x4 rot_x;
    float c = cos(angle_rad);
    float s = sin(angle_rad);

    rot_x.m[0][0] = 1.0f; rot_x.m[0][1] = 0.0f; rot_x.m[0][2] = 0.0f; rot_x.m[0][3] = 0.0f;
    rot_x.m[1][0] = 0.0f; rot_x.m[1][1] = c;    rot_x.m[1][2] = -s;   rot_x.m[1][3] = 0.0f;
    rot_x.m[2][0] = 0.0f; rot_x.m[2][1] = s;    rot_x.m[2][2] = c;    rot_x.m[2][3] = 0.0f;
    rot_x.m[3][0] = 0.0f; rot_x.m[3][1] = 0.0f; rot_x.m[3][2] = 0.0f; rot_x.m[3][3] = 1.0f;

    return rot_x;
}

t_matrix4x4 create_rotation_matrix_y(float angle_rad)
{
    t_matrix4x4 rot_y;
    float c = cos(angle_rad);
    float s = sin(angle_rad);

    rot_y.m[0][0] = c;    rot_y.m[0][1] = 0.0f; rot_y.m[0][2] = s;    rot_y.m[0][3] = 0.0f;
    rot_y.m[1][0] = 0.0f; rot_y.m[1][1] = 1.0f; rot_y.m[1][2] = 0.0f; rot_y.m[1][3] = 0.0f;
    rot_y.m[2][0] = -s;   rot_y.m[2][1] = 0.0f; rot_y.m[2][2] = c;    rot_y.m[2][3] = 0.0f;
    rot_y.m[3][0] = 0.0f; rot_y.m[3][1] = 0.0f; rot_y.m[3][2] = 0.0f; rot_y.m[3][3] = 1.0f;

    return rot_y;
}


t_vec3 mat4x4_mul_vec3(t_matrix4x4 m, t_vec3 v)
{
    t_vec3 result;

    float w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3];

    result.x = (m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3]) / w;
    result.y = (m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3]) / w;
    result.z = (m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3]) / w;

    return result;
}


t_matrix4x4 mat4x4_multiply(t_matrix4x4 a, t_matrix4x4 b)
{
    t_matrix4x4 result;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m[i][j] = a.m[i][0] * b.m[0][j] +
                             a.m[i][1] * b.m[1][j] +
                             a.m[i][2] * b.m[2][j] +
                             a.m[i][3] * b.m[3][j];
        }
    }

    return result;
}