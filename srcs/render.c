#include "../includes/header.h"

t_vec3 calculate_ray_direction(t_utils *utils, int pixel_x, int pixel_y, int image_width, int image_height)
{
    //t_camera *camera;
    (void) utils;
    float aspect_ratio;

    aspect_ratio = (float) image_width / (float) image_height;

    //camera = &utils->scene->camera;
    /*float aspect_ratio;
    float scale; 
    float x;
    float y;

    aspect_ratio = (float)image_width / (float)image_height;
    scale = tan(utils->scene->camera->fov * 0.5 * 3.1415 / 180);
    x = (2 * (pixel_x + 0.5) / (float)image_width - 1) * aspect_ratio * scale;
    y = (1 - 2 * (pixel_y + 0.5) / (float)image_height) * scale;

    t_vec3 image_plane_point = {x, y, -1};
    t_vec3 ray_direction;

    ray_direction.x = image_plane_point.x;
    ray_direction.y = image_plane_point.y;
    ray_direction.z = image_plane_point.z;*/
    
    t_vec3 ray_origine;
    t_vec3 ray_direction;
    t_vec2 coord;

    ray_origine = (t_vec3) {0,0,0}; //vec3_normalize(utils->scene->camera->pos);

    coord.x = (float) pixel_x / (float) image_width;
    coord.y = (float) pixel_y / (float) image_height;

    coord.x = coord.x * 2.0f - 1.0f;
    coord.y = coord.y  * 2.0f - 1.0f;

    coord.x *= aspect_ratio;
    /*ray_origine.x = camera->pos.x;
    ray_origine.y = camera->pos.y;
    ray_origine.z = camera->pos.z;*/
    


    ray_direction.x = coord.x - ray_origine.x;
    ray_direction.y = coord.y - ray_origine.y;
    ray_direction.z = -1.0f - ray_origine.z;

    t_matrix4x4 rot_x = create_rotation_matrix_x(utils->scene->camera->pitch);
    t_matrix4x4 rot_y = create_rotation_matrix_y(utils->scene->camera->yaw);

    // Combine the rotation matrices
    t_matrix4x4 combined_rot = mat4x4_multiply(rot_y, rot_x);

    // Apply the combined rotation matrix to the ray direction
    ray_direction = mat4x4_mul_vec3(combined_rot, ray_direction);

    return ray_direction;
}

void render_image(t_utils *utils)
{
    t_ray ray;
    float t[2];
    t_camera *camera = utils->scene->camera;
    bool has_intersect;
    int closest;
    float hit_distance;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            hit_distance = -1.0f;
            has_intersect = false;
            ray.origin = camera->pos;
            ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
            for (int i = 0; i < utils->scene->num_spheres; i++)
            {
                if (intersect_sphere(ray, utils->scene->spheres[i], t) == true)
                {
                    has_intersect = true;
                    if (hit_distance > t[1] || hit_distance < 0.0f)
                    {
                        hit_distance = t[1];
                        closest = i;
                    }

                }
            }            
            if (has_intersect == true)
            {
                t_vec3 hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, t[1]));
                t_vec3 normal = vec3_normalize(vec3_add(hit_point, vec3_multiply_scalar(utils->scene->spheres[closest].center, -1)));
                t_vec3 light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(hit_point, -1)));

                float d = vec3_dot_product(normal, light_direction);
                //d = fmax(0, d);
                d = fmax(utils->scene->alight->intensity, d);
                int color = create_trgb(0, utils->scene->spheres[closest].color.r * d, utils->scene->spheres[closest].color.g * d, utils->scene->spheres[closest].color.b * d);
                my_mlx_pixel_put(utils->img, x, y, color);
            }
            else
            {
                my_mlx_pixel_put(utils->img, x, y, BLACK);
            }
        }
    }
}
