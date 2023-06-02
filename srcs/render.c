#include "../includes/miniRT.h"

float r2()
{
    return (2.0f * (float)rand()) / (3.0f*(float)RAND_MAX) ;
}

t_vec3 clamp_color(t_vec3 color)
{
	if (color.x > 1.0f)
		color.x = 1.0f;
	if (color.y > 1.0f)
		color.y = 1.0f;
	if (color.z > 1.0f)
		color.z = 1.0f;
	if (color.x < 0.0f)
		color.x = 0.0f;
	if (color.y < 0.0f)
		color.y = 0.0f;
	if (color.z < 0.0f)
		color.z = 0.0f;
	return (color);
}

t_vec3 calculate_ray_direction(t_utils *utils, int pixel_x, int pixel_y, int image_width, int image_height)
{
	t_vec3 camera_space;
	float aspect_ratio;
	float screen_x;
	float screen_y;

	aspect_ratio = (float)image_width / (float)image_height;
	screen_x = ((float)pixel_x + r2()) / (float)image_width * 2.0f - 1.0f;
	screen_y = 1.0f - ((float)pixel_y + r2()) / (float)image_height * 2.0f;

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

    for (int j = 0; j < utils->scene->num_lights; j++)
    {
        shadow_ray.direction = vec3_normalize(vec3_subtract(utils->scene->lights[j].pos, point));
        shadow_ray.origin = point;

        if (intersect_object(utils, shadow_ray, &payload) == true && payload.hit_distance >= 0.0f)
        {
            if (vec3_distance(point, utils->scene->lights[j].pos) > vec3_distance(point, vec3_add(shadow_ray.origin, vec3_multiply_scalar(shadow_ray.direction, payload.hit_distance))))
            {
                return 1;
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
	t_vec3		albedo;
	t_vec3		light_vector;
	t_material	material;
	t_vec3		hit_point;
	float		factor;

	factor = 1.0f;
	color = (t_vec3) {0.0f, 0.0f, 0.0f};
	sky_color = (t_vec3) {0.7f, 0.6f, 0.8f};
	if (depth == 0)
		return (sky_color);
	for (int i = 0; i < depth; i++)
	{
		if(intersect_object(utils, ray, &payload) == true)
		{
			light_vector = payload.light_direction;
			hit_point = vec3_add(payload.hit_point, vec3_multiply_scalar(payload.normal, 0.001f));
			if (payload.object_type == SPHERE)
				material = utils->scene->spheres[payload.object_index].material;
			else if (payload.object_type == PLANE)
				material = utils->scene->plans[payload.object_index].material;
			albedo = material.color;

			if (is_in_shadow(utils, hit_point))
			{
				//color = (t_vec3) {0.0f, 0.0f, 0.0f};

				color = vec3_multiply_scalar(albedo, utils->scene->alight->intensity);

			}
			else
			{
				/*t_vec3 V = vec3_normalize(vec3_subtract(utils->scene->camera->pos, hit_point));
				t_vec3 H = vec3_normalize(vec3_add(V, light_vector));
				color = PBR(utils, material, V, H, payload);*/
				/*if (color.x < vec3_multiply_scalar(albedo, utils->scene->alight->intensity).x)
				{
					color.x = vec3_multiply_scalar(albedo, utils->scene->alight->intensity).x;
				}*/

				//color = vec3_multiply_scalar(albedo, vec3_dot_product(payload.normal, light_vector));

				//color = vec3_multiply_scalar(color, factor);
				color = vec3_multiply_scalar(albedo, (fmin(vec3_dot_product(payload.normal, light_vector) + utils->scene->alight->intensity, 1.0f)));



				//material.roughness = 0.0f;

				//color = vec3_add(color, vec3_multiply_scalar(albedo, utils->scene->alight->intensity));
			}
			if (payload.object_type == SPHERE)
				material.roughness = 0.01f;
			if (payload.object_type == PLANE)
				material.roughness = 1.0f;
			ray.direction = reflect(ray.direction, payload.normal, material.roughness);
			ray.origin = hit_point;
		}
		else
		{
			color = sky_color;
			//color = vec3_multiply_scalar(color, factor);
			//color = vec3_add(color, vec3_multiply_scalar(sky_color, 1.0f - factor));
			break;
		}
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

    depth = 1;

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
