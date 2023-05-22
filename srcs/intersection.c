#include "../includes/miniRT.h"

static void calcul_payload(t_utils *utils, t_payload *payload, t_ray ray)
{
    payload->hit_distance = payload->t;
    payload->hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, payload->hit_distance));
    payload->light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(payload->hit_point, -1)));
    if (payload->object_type == SPHERE)
	    payload->normal = vec3_normalize(vec3_add(payload->hit_point, vec3_multiply_scalar(utils->scene->spheres[payload->object_index].center, -1)));
    else if (payload->object_type == PLANE)
	    payload->normal = utils->scene->plans[payload->object_index].normal;
    else if (payload->object_type == CYLINDER)
    {
        t_vec3 p = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, payload->hit_distance));
        t_vec3 v = vec3_subtract(p, utils->scene->cylinders[payload->object_index].bottom);
        t_vec3 proj = vec3_multiply_scalar(utils->scene->cylinders[payload->object_index].direction, vec3_dot_product(v, utils->scene->cylinders[payload->object_index].direction));
        payload->normal = vec3_normalize(vec3_subtract(v, proj));
    }
    else if (payload->object_type == TRIANGLE)
    {
        t_vec3 edge1 = vec3_subtract(utils->scene->triangles[payload->object_index].vertices[1], utils->scene->triangles[payload->object_index].vertices[0]);
        t_vec3 edge2 = vec3_subtract(utils->scene->triangles[payload->object_index].vertices[2], utils->scene->triangles[payload->object_index].vertices[0]);
        payload->normal = vec3_normalize(vec3_cross_product(edge1, edge2));
    }


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

bool intersect_triangle(t_ray ray, t_triangle triangle, float *t)
{
    t_vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vec3_subtract(triangle.vertices[1], triangle.vertices[0]);
    edge2 = vec3_subtract(triangle.vertices[2], triangle.vertices[0]);
    h = vec3_cross_product(ray.direction, edge2);
    a = vec3_dot_product(edge1, h);

    if (a > -0.00001 && a < 0.00001)
        return false;

    f = 1 / a;
    s = vec3_subtract(ray.origin, triangle.vertices[0]);
    u = f * vec3_dot_product(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    q = vec3_cross_product(s, edge1);
    v = f * vec3_dot_product(ray.direction, q);

    if (v < 0.0 || u + v > 1.0)
        return false;
    float temp = f * vec3_dot_product(edge2, q);
    if (temp > 0.00001) // ray intersection
    {
        *t = temp;
        return true;
    }
    else
        return false;
}


bool intersect_cylinder(t_ray ray, t_cylinder cylinder, float *t)
{
    t_vec3 oc = vec3_subtract(ray.origin, cylinder.bottom);
    t_vec3 dir = vec3_normalize(cylinder.direction);
    t_vec3 dp = vec3_subtract(ray.direction, vec3_multiply_scalar(dir, vec3_dot_product(ray.direction, dir)));
    t_vec3 dop = vec3_subtract(oc, vec3_multiply_scalar(dir, vec3_dot_product(oc, dir)));
    float a = vec3_dot_product(dp, dp);
    float b = 2.0 * vec3_dot_product(dp, dop);
    float c = vec3_dot_product(dop, dop) - pow(cylinder.radius, 2);
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false;
    else
    {
        float t0 = (-b - sqrtf(discriminant)) / (2.0f * a);
        //float t1 = (-b + sqrtf(discriminant)) / (2.0f * a);
        *t = t0;
        return true;
    }
}


//P = (0,0,0) + (1,1,0)
//d = (1,1,1) + (0,0,-1)
//d intersecte p en (1,1,0)

bool intersect_plane(t_ray ray, t_plane plane, float *t)
{
    float denom = vec3_dot_product(ray.direction, plane.normal);
    
    if (fabs(denom) < 1e-6)
        return false;

    t_vec3 diff = vec3_subtract(plane.pos, ray.origin);
    float t_value = vec3_dot_product(diff, plane.normal) / denom;

    if (t_value < 0)
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
                closest_object_payload.object_type = PLANE;
                closest_object_payload.object_index = i;
                closest_object_payload.object_color = utils->scene->plans[i].color;

            }
        }
        i++;
    }
    i = 0;
    while (i < utils->scene->num_cylinders)
    {
        if (intersect_cylinder(ray, utils->scene->cylinders[i], &payload->t) == true)
        {
            closest_object_payload.t = payload->t;
            closest_object_payload.object_type = CYLINDER;
            closest_object_payload.object_index = i;
            closest_object_payload.object_color = utils->scene->cylinders[i].color;
        }
        i++;
    }
    i = 0;
    while (i < utils->scene->num_triangles)
    {
        if (intersect_triangle(ray, utils->scene->triangles[i], &payload->t) == true)
        {
            closest_object_payload.t = payload->t;
            closest_object_payload.object_type = TRIANGLE;
            closest_object_payload.object_index = i;
            closest_object_payload.object_color = utils->scene->triangles[i].color;
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

