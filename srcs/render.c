#include "../includes/header.h"

t_vec3 calculate_ray_direction(t_utils *utils, int pixel_x, int pixel_y, int image_width, int image_height)
{
    float aspect_ratio;
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
    ray_direction.z = image_plane_point.z;

    return ray_direction;
}

void render_image(t_utils *utils)
{
    int width = WIDTH;
    int height = HEIGHT;
    t_ray ray;
    float t[2];
    t_camera *camera = utils->scene->camera;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            ray.origin = camera->pos;
            ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
            if (intersect_sphere(ray, *(utils->scene->spheres), t) == true)
            {
                t_vec3 hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, t[1]));
                t_vec3 normal = vec3_normalize(vec3_add(hit_point, vec3_multiply_scalar(utils->scene->spheres->center, -1)));
                t_vec3 light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(hit_point, -1)));

                float d = vec3_dot_product(normal, light_direction);
                //d = fmax(0, d);
                d = fmax(utils->scene->alight->intensity, d);
                int color = create_trgb(0, 255 * d, 250 * d, 255 * d);
                my_mlx_pixel_put(utils->img, x, y, color);
            }
            else if(intersect_plane(ray, *(utils->scene->plans), t))
            {
                my_mlx_pixel_put(utils->img, x, y, create_trgb(0, utils->scene->plans->color.r, utils->scene->plans->color.g, utils->scene->plans->color.b));
            }
            else
            {
                my_mlx_pixel_put(utils->img, x, y, BLACK);
            }
        }
    }
}
