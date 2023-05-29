#include "../includes/miniRT.h"

t_vec3 clamp_color(t_vec3 color)
{
	if (color.x > 1.0f)
		color.x = 1.0f;
	if (color.y > 1.0f)
		color.y = 1.0f;
	if (color.z > 1.0f)
		color.z = 1.0f;
	return (color);
}

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

int is_in_shadow(t_utils *utils, t_vec3 point)
{
    t_ray shadow_ray;
	t_payload payload;

	for (int i = 0; i < utils->scene->num_lights; i++)
	{
		shadow_ray.direction = vec3_normalize(vec3_subtract(utils->scene->lights[i].pos, point));
		shadow_ray.origin = (point);

		for (int i = 0; i < utils->scene->num_spheres; i++)
		{
			if (intersect_object(utils, shadow_ray, &payload) == true && payload.hit_distance > 0.0f)
			{
				if (vec3_distance(point, utils->scene->lights[i].pos) > vec3_distance(point, vec3_add(shadow_ray.origin, vec3_multiply_scalar(shadow_ray.direction, payload.hit_distance))))
				{
					return 1;
				}
			}
		}
		for (int i = 0; i < utils->scene->num_plans; i++)
		{
			if (intersect_object(utils, shadow_ray, &payload) == true && payload.hit_distance > 0.0f)
			{
				if (vec3_distance(point, utils->scene->lights[i].pos) > vec3_distance(point, vec3_add(shadow_ray.origin, vec3_multiply_scalar(shadow_ray.direction, payload.hit_distance))))
				{
					return 1;
				}
			}
		}
		
	}
    return 0;
}

/*t_vec3 Lambertian(t_payload payload, t_vec3 light_direction, t_vec3 light_color) {
    t_vec3 N = payload.normal;
    t_vec3 object_color = {
		(float) payload.object_color.x / 255.0f,
		(float) payload.object_color.y / 255.0f,
		(float) payload.object_color.z / 255.0f
    };
    
    // Calculate the dot product of the normal and the light direction
    float NdL = vec3_dot_product(N, light_direction);
    
    // Clamp the dot product to the range [0, 1]
    NdL = fmax(0.0, fmin(1.0, NdL));
    // Calculate the final color
    t_vec3 final_color = {
        object_color.x * light_color.x * NdL,
        object_color.y * light_color.y * NdL,
        object_color.z * light_color.z * NdL
    };

    return final_color;
}*/

t_vec3 trace_path(t_utils *utils, t_ray ray, int depth)
{
	t_payload	payload;
	t_vec3		color;
	t_vec3		sky_color;
	t_vec3 V;
	t_vec3 H;

	sky_color = (t_vec3){200.0f / 255.0f, 190.0f / 255.0f, 240.0f / 255.0f};

	if (depth == 0)
		return sky_color;

	if (intersect_object(utils, ray, &payload) == true)
	{
		t_vec3 hit_point = payload.hit_point;

		void *object;
		t_material object_material;

		if (payload.object_type == SPHERE)
		{
			object = &utils->scene->spheres[payload.object_index];
			object_material = ((t_sphere *)object)->material;
		}
		if (payload.object_type == PLANE)
		{
			object = &utils->scene->plans[payload.object_index];
			object_material = ((t_plane *)object)->material;
		}


		V = vec3_normalize(vec3_subtract(utils->scene->camera->pos, hit_point));  // View direction for the first bounce
		H = vec3_normalize(vec3_add(V, payload.light_direction));  // Halfway vector

		color = PBR(utils, object_material, V, H, payload);  // Call to PBR function.

		if (depth > 1)
		{
			t_ray reflected_ray;
			reflected_ray.origin = vec3_add(hit_point, vec3_multiply_scalar(payload.normal, 0.001f));
			reflected_ray.direction = reflect(ray.direction, payload.normal, 0.1f);

			// Recalculate V after reflection
			V = reflected_ray.direction;
			H = vec3_normalize(vec3_add(V, payload.light_direction));

			t_vec3 reflected_color = trace_path(utils, reflected_ray, depth - 1);

			// Adding reflection contribution to the color based on the object material's reflectivity
			color = vec3_add(vec3_multiply_scalar(color, 1.0f - object_material.reflectivity), 
                             vec3_multiply_scalar(reflected_color, object_material.reflectivity));
		}

	}
	else
	{
		color = sky_color;
	}
	
	return color;
}


void render_image(t_utils *utils)
{
    t_ray ray;
    t_camera *camera = utils->scene->camera;
    int depth;
    t_color old_color;
    t_color new_color;

	//light_color = utils->scene->lights->color;

    depth = 5;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            ray.origin = camera->pos;
            ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
            t_vec3 color = trace_path(utils, ray, depth);
			/*if (color.x >= 1.0f || color.y >= 1.0f || color.z >= 1.0f)
			{
				printf("Merde«n");
			}*/

			//color = reinhard_tone_mapping(color);
			color = clamp_color(color);
            t_color color_converted = (t_color){color.x * 255.0f, color.y * 255.0f, color.z * 255.0f};
			//printf("%d,%d,%d\n", color_converted.r, color_converted.g, color_converted.b);
            if (utils->img->accumulate == 1)
            {
                if (utils->img->frames == 1)
                {
                    utils->img->accumulator[y * WIDTH + x] = color_converted;
                    my_mlx_pixel_put(utils->img, x, y, create_trgb(0, color_converted.r, color_converted.g, color_converted.b));
                }
                else
                {
                    old_color = utils->img->accumulator[y * WIDTH + x];
                    new_color = color_multiply_scalar(old_color, utils->img->frames - 1);
                    new_color = color_add(new_color, color_converted);
                    new_color = color_multiply_scalar(new_color, 1.0f / (float)utils->img->frames);
                    utils->img->accumulator[y * WIDTH + x] = new_color;
                    my_mlx_pixel_put(utils->img, x, y, create_trgb(0, new_color.r, new_color.g, new_color.b));
                }
            }
        }
    }
    utils->img->frames++;
}
