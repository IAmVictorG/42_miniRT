#include "../includes/header.h"

bool intersect_sphere(t_ray ray, t_sphere sphere, float t[2])
{
    float   a;
    float   b;
    float   c;
    float   discriminant;
    t_vec3 oc = vec3_subtract(ray.origin, sphere.center);

    a = vec3_dot_product(ray.direction, ray.direction);
    b = 2.0 * vec3_dot_product(ray.direction, oc);
    c = vec3_dot_product(oc, oc) - sphere.radius * sphere.radius;
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false; 
    else 
    {
        t[0] = (-b + sqrt(discriminant)) / (2.0f * a);
        t[1] = (-b - sqrt(discriminant)) / (2.0f * a);
        return true;
    }
}

//P = (0,0,0) + (1,1,0)
//d = (1,1,1) + (0,0,-1)
//d intersecte p en (1,1,0)

bool intersect_plane(t_ray ray, t_plan plane, float t[2])
{
    float denom = vec3_dot_product(ray.direction, plane.normal);
    
    if (fabs(denom) < 1e-6) // Check if ray is parallel to the plane
        return false;

    t_vec3 diff = vec3_subtract(plane.pos, ray.origin);
    float t_value = vec3_dot_product(diff, plane.normal) / denom;

    if (t_value < 0) // Check if the intersection is behind the ray origin
        return false;

    *t = t_value;
    return true;
}

