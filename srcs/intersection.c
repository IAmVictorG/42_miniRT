#include "../includes/miniRT.h"

static void calcul_payload(t_utils *utils, t_payload *payload, t_ray ray)
{
    payload->hit_distance = payload->t;
    payload->hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, payload->hit_distance));
    payload->light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(payload->hit_point, -1)));
    if (payload->object_type == SPHERE)
	    payload->normal = vec3_normalize(vec3_add(payload->hit_point, vec3_multiply_scalar(utils->scene->spheres[payload->object_index].center, -1)));
    if (payload->object_type == PLANE)
	    payload->normal = utils->scene->plans[payload->object_index].normal;
}

static bool intersect_sphere(t_ray ray, t_sphere sphere, float *t)
{
    float   a;
    float   b;
    float   c;
    float   discriminant;
    t_vec3 oc;

    oc = vec3_subtract(ray.origin, sphere.center);
    a = vec3_dot_product(ray.direction, ray.direction);
    b = 2.0 * vec3_dot_product(ray.direction, oc);
    c = vec3_dot_product(oc, oc) - sphere.radius * sphere.radius;
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false; 
    else 
    {
        *t = (-b - sqrtf(discriminant)) / (2.0f * a);
        return true;
    }
}

//P = (0,0,0) + (1,1,0)
//d = (1,1,1) + (0,0,-1)
//d intersecte p en (1,1,0)

bool intersect_plane(t_ray ray, t_plan plane, float *t)
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

bool intersect_object(t_utils *utils, t_ray ray, t_payload *payload)
{
    int i;
    t_payload closest_object_payload;

    closest_object_payload.t = FLT_MAX;

    i = 0;
    while (i < utils->scene->num_spheres)
    {
        if (intersect_sphere(ray, utils->scene->spheres[i], &payload->t) == true)
        {
            if (payload->t < closest_object_payload.t && payload->t > 0.0f)
            {
                closest_object_payload.t = payload->t;
                closest_object_payload.object_type = SPHERE;
                closest_object_payload.object_index = i;
                closest_object_payload.object_color = utils->scene->spheres[i].color;
            }
        }
        i++;
    }
    i = 0;
    while (i < utils->scene->num_plans)
    {
        if (intersect_plane(ray, utils->scene->plans[i], &payload->t))
        {
            if (payload->t < closest_object_payload.t && payload->t > 0.0f)
            {
                closest_object_payload.t = payload->t;
                closest_object_payload.object_type = CYLINDER;
                closest_object_payload.object_index = i;
                closest_object_payload.object_color = utils->scene->plans[i].color;

            }
        }
        i++;
    }
    if (closest_object_payload.t < FLT_MAX)
    {
        calcul_payload(utils, &closest_object_payload, ray);
        *payload = closest_object_payload;
        return (true);
    }
    return (false);
}

