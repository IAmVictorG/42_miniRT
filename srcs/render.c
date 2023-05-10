#include "../includes/header.h"

t_vec3 calculate_ray_direction(t_camera camera, int pixel_x, int pixel_y, int image_width, int image_height) {
    
    
    /*t_vec3 ray_direction;

    ray_direction.x = camera.pos.x + pixel_x / image_width * 2 - 1;
    ray_direction.y = camera.pos.y + pixel_y / image_height * 2 - 1;
    ray_direction.z = -1;*/
    float aspect_ratio = (float)image_width / (float)image_height;
    float scale = tan(camera.fov * 0.5 * 3.1415 / 180);
    float x = (2 * (pixel_x + 0.5) / (float)image_width - 1) * aspect_ratio * scale;
    float y = (1 - 2 * (pixel_y + 0.5) / (float)image_height) * scale;
    
    t_vec3 image_plane_point = {
        x, y, -1 // -1 car on considère que la direction de la caméra est alignée avec l'axe Z négatif
    };
    
    t_vec3 ray_direction;
    ray_direction.x = image_plane_point.x;
    ray_direction.y = image_plane_point.y;
    ray_direction.z = image_plane_point.z;
    
    // TODO: appliquer la rotation de la caméra si nécessaire

    return ray_direction;
}

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

void render_image(t_utils *utils)
{
    int width = WIDTH;
    int height = HEIGHT;
    t_ray ray;
    float t[2];
    t_camera *camera = utils->scene->camera;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ray.origin = camera->pos;
            ray.direction = calculate_ray_direction(*camera, x, y, WIDTH, HEIGHT);
            if (intersect_sphere(ray, *(utils->scene->spheres), t) == true)
            {
                t_vec3 hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, t[1]));
                t_vec3 normal = vec3_normalize(vec3_add(hit_point, vec3_multiply_scalar(utils->scene->spheres->center, -1)));
                t_vec3 light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(hit_point, -1)));

                float d = vec3_dot_product(normal, light_direction);

                d = fmax(0.0, d); // Clamp the dot product result to the range [0, 1]

                int color = create_trgb(0, 255 * d, 250 * d, 255 * d);
                my_mlx_pixel_put(utils->img, x, y, color);
            }
            else
            {
                my_mlx_pixel_put(utils->img, x, y, BLACK);
            }
        }
    }
}


