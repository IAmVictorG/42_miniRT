#include "../includes/miniRT.h"

t_vec3 calculate_ray_direction(t_utils *utils, int pixel_x, int pixel_y, int image_width, int image_height)
{
	t_vec3 camera_space;
	float aspect_ratio;
	float screen_x;
	float screen_y;

	aspect_ratio = (float)image_width / (float)image_height;
	screen_x = ((float)pixel_x + 0.5f) / (float)image_width * 2.0f - 1.0f;
	screen_y = 1.0f - ((float)pixel_y + 0.5f) / (float)image_height * 2.0f;

	camera_space.x = screen_x * aspect_ratio * tanf(utils->scene->camera->fov * 0.5f * PI / 180.0f);
	camera_space.y = screen_y * tanf(utils->scene->camera->fov * 0.5f * PI / 180.0f);
	camera_space.z = -1.0f;

	// Calculate rotation matrices for pitch and yaw
	t_matrix4x4 rot_pitch = create_rotation_matrix_x(utils->scene->camera->pitch);
	t_matrix4x4 rot_yaw = create_rotation_matrix_y(utils->scene->camera->yaw);

	// Combine the rotation matrices
	t_matrix4x4 rot_combined = mat4x4_multiply(rot_yaw, rot_pitch);

	// Apply the rotation to the camera space coordinate to get the ray direction
	t_vec3 ray_dir = mat4x4_mul_vec3(rot_combined, camera_space);

	t_vec3 ray_dir_normalized = vec3_normalize(ray_dir);

	return ray_dir_normalized;
}

int is_in_shadow(t_utils *utils, t_sphere *sphere, t_vec3 point)
{
    t_ray shadow_ray;
	float t;

	for (int i = 0; i < utils->scene->num_lights; i++)
	{
		shadow_ray.direction = vec3_normalize(vec3_subtract(utils->scene->lights[i].pos, point));
		shadow_ray.origin = (point);

		for (int i = 0; i < utils->scene->num_spheres; i++)
		{
			if (&utils->scene->spheres[i] == sphere)
				continue ;
			if (intersect_sphere(shadow_ray, utils->scene->spheres[i], &t) && t > 0.0f)
			{
				if (vec3_distance(point, utils->scene->lights[i].pos) > vec3_distance(point, vec3_add(shadow_ray.origin, vec3_multiply_scalar(shadow_ray.direction, t))))
				{
					return 1;
				}
			}
		}
	}
    return 0;
}

t_color trace_path(t_utils *utils, t_ray ray, int depth)
{
	float t;
	int closest_index;
	float hit_distance;

	closest_index = -1;
	hit_distance = FLT_MAX;
	if (depth == 0)
		return ((t_color){200, 190, 240});
	for (int i = 0; i < utils->scene->num_spheres; i++)
	{
		if (intersect_sphere(ray, utils->scene->spheres[i], &t) && t > 0.0f)
		{
			if (t < hit_distance)
			{
				hit_distance = t;
				closest_index = i;
			}
		}
	}
	if (closest_index != -1)
	{
		t_vec3 hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, hit_distance));
		t_vec3 normal = vec3_normalize(vec3_add(hit_point, vec3_multiply_scalar(utils->scene->spheres[closest_index].center, -1)));
		t_vec3 light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(hit_point, -1)));
		t_color	color = utils->scene->spheres[closest_index].color;
		if (is_in_shadow(utils, &(utils->scene->spheres[closest_index]), hit_point))
		{
			color = color_multiply_scalar(utils->scene->spheres[closest_index].color, utils->scene->alight->intensity);
			return (color);
		}
		else
		{
			float d = fmax(utils->scene->alight->intensity, vec3_dot_product(normal, light_direction));
			color = color_multiply_scalar(utils->scene->spheres[closest_index].color, d);

			return (color);
		}
	}
	return ((t_color){200, 190, 240});
}

/*t_color trace_path(t_utils *utils, t_ray ray, int depth)
{
	float t[2];
	bool has_intersect;
	int closest;
	float hit_distance = -1.0f;
	float reflection_coef;
	t_color color = {0, 0, 0}; // sky color

	reflection_coef = 0.6f;
	closest = -1;
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
	if (closest != -1 && has_intersect == true && t[1] > 0.0f)
	{
		t_vec3 hit_point = vec3_add(ray.origin, vec3_multiply_scalar(ray.direction, t[1]));
		t_vec3 normal = vec3_normalize(vec3_add(hit_point, vec3_multiply_scalar(utils->scene->spheres[closest].center, -1)));
		t_vec3 light_direction = vec3_normalize(vec3_add(utils->scene->lights->pos, vec3_multiply_scalar(hit_point, -1)));


		float d = fmax(utils->scene->alight->intensity, vec3_dot_product(normal, light_direction));
		color = color_add(color, color_multiply_scalar(utils->scene->spheres[closest].color, d));
		// Reflection
		if(depth > 0)
		{
			t_ray reflected_ray;
			reflected_ray.origin = hit_point;
			reflected_ray.direction = reflect(ray.direction, normal, 0.4f);
			t_color reflected_color = trace_path(utils, reflected_ray, depth - 1);
			color = color_add(color, color_multiply_scalar(reflected_color, reflection_coef));
		}
	}
	return color;
}*/

void render_image(t_utils *utils)
{
	t_ray ray;
	t_camera *camera = utils->scene->camera;
	int depth;
	t_color old_color;
	t_color new_color;

	depth = 5;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			ray.origin = camera->pos;
			ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
			t_color color = trace_path(utils, ray, depth);
			if (utils->img->accumulate == 1)
			{
				if (utils->img->frames == 1)
				{
					utils->img->accumulator[y * WIDTH + x] = color;
					my_mlx_pixel_put(utils->img, x, y, create_trgb(0, color.r, color.g, color.b));
				}
				else
				{
					old_color = utils->img->accumulator[y * WIDTH + x];
					new_color = color_multiply_scalar(old_color, utils->img->frames - 1);
					new_color = color_add(new_color, color);
					new_color = color_multiply_scalar(new_color, 1.0f / (float)utils->img->frames);
					utils->img->accumulator[y * WIDTH + x] = new_color;
					my_mlx_pixel_put(utils->img, x, y, create_trgb(0, new_color.r, new_color.g, new_color.b));
				}
			}
		}
	}
	utils->img->frames++;
}
