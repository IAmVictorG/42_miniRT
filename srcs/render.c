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

/*t_vec3 trace_path(t_utils *utils, t_ray ray, t_vec3 light_color, int depth)
{
	t_payload	payload;
	t_vec3		color;

	if (depth == 0)
		return ((t_vec3){0.8, 0.6, 0.9});

	if (intersect_object(utils, ray, &payload) == true)
	{
		t_vec3 hit_point = payload.hit_point;
		t_vec3 light_direction = payload.light_direction;
		color = (t_vec3) {
            (float) payload.object_color.x / 255.0f,
            (float) payload.object_color.y / 255.0f,
            (float) payload.object_color.z / 255.0f
        };

		void *object;
		t_vec3 object_color;
		(void) object_color;
		t_vec3 V = vec3_normalize(vec3_subtract(utils->scene->camera->pos, hit_point));  // View direction
		t_vec3 H = vec3_normalize(vec3_add(V, light_direction));  // Halfway vector
		t_vec3 F0 = {0.03f, 0.03f, 0.03f};  // Fresnel reflectance at normal incidence for a dielectric material. You should change it depending on your material properties.


		if (payload.object_type == SPHERE)
		{
			object = &utils->scene->spheres[payload.object_index];
			object_color = ((t_sphere *)object)->color;
		}

		if (is_in_shadow(utils, hit_point))
		{
			//color = (t_vec3) {0.0f, 0.0f, 0.0f};
			//color = color_multiply_scalar(object_color, utils->scene->alight->intensity);
			color = vec3_multiply_scalar(color, utils->scene->alight->intensity);

		}
		else
		{	
			color = PBR(utils, F0, V, H, payload);  // Call to PBR function.
			//color = Lambertian(payload, light_direction, light_color);
		}

		if (depth > 1)
		{
			t_ray reflected_ray;
			reflected_ray.origin = hit_point;
			reflected_ray.direction = reflect(ray.direction, payload.normal, 0.1f);
			t_vec3 reflected_light_color = vec3_multiply(color, light_color);
			t_vec3 reflected_color = trace_path(utils, reflected_ray, reflected_light_color, depth - 1);

			color = vec3_add(color, vec3_multiply_scalar(reflected_color, 0.5f)); // assuming reflectivity is 0.5
		}

	}
	else
	{
		color = (t_vec3){200.0f / 255.0f, 190.0f / 255.0f, 240.0f / 255.0f};
	}
	
	return (t_vec3){color.x, color.y, color.z};
}*/

t_vec3 fresnelSchlick(float cosTheta, t_vec3 F0)
{
    return vec3_multiply_scalar(vec3_add(F0, vec3_subtract((t_vec3) {1.0f, 1.0f, 1.0f}, F0)), pow((1.0f + 0.000001f) - cosTheta, 5.0));
}

float DistributionGGX(t_vec3 N, t_vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = fmax(vec3_dot_product(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(t_vec3 N, t_vec3 V, t_vec3 L, float roughness)
{
    float NdotV = fmax(vec3_dot_product(N, V), 0.0);
    float NdotL = fmax(vec3_dot_product(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

t_ray lightRay(t_vec3 ro, t_light l) //computes ro to light source ray
{
	t_ray r;

	r.origin = ro;
	r.direction = vec3_normalize(vec3_subtract(l.pos, ro));
    return r;
 }

 float lightDist(t_vec3 ro, t_light l) //computes distance to light
{ 
	return vec3_distance(l.pos, ro);
}

t_vec3 mix2(t_vec3 x, t_vec3 y, float a) {
    t_vec3 result;

    result.x = (1.0f - a) * x.x + a * y.x;
    result.y = (1.0f - a) * x.y + a * y.y;
    result.z = (1.0f - a) * x.z + a * y.z;

    return result;
}
t_vec3 computeReflectance(t_vec3 N, t_vec3 Ve, t_vec3 F0, t_vec3 albedo, t_vec3 L, t_vec3 H, t_vec3 light_col, float intensity, float metallic, float roughness)
{
	(void) metallic;
    t_vec3 radiance =  vec3_multiply_scalar(light_col, intensity); //Incoming Radiance

    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);
    float G   = GeometrySmith(N, Ve, L,roughness);
    t_vec3 F    = fresnelSchlick(fmax(vec3_dot_product(H, Ve), 0.0f), F0);

    t_vec3 kS = F;
    t_vec3 kD = vec3_subtract((t_vec3) {1.0f, 1.0f, 1.0f}, kS);

   // kD *= 1.0f - metallic;

    t_vec3 nominator    = vec3_multiply_scalar(F, NDF * G);
    float denominator = 4 * fmax(vec3_dot_product(N, Ve), 0.0f) * fmax(vec3_dot_product(N, L), 0.0f) + 0.00001f/* avoid divide by zero*/;
    t_vec3 specular     = vec3_multiply_scalar(nominator, 1.0f / denominator);


    // add to outgoing radiance Lo
    float NdotL = fmax(vec3_dot_product(N, L), 0.0f);
    t_vec3 diffuse_radiance = vec3_multiply_scalar(vec3_multiply(kD, albedo),  1.0f /  PI);

    return vec3_multiply_scalar(vec3_multiply(vec3_add(diffuse_radiance, specular), radiance), NdotL);
}

t_vec3 PBR2(t_payload payload, t_ray r,  t_light l)
{
    t_vec3 ambient = vec3_multiply((t_vec3) {0.03f, 0.03f, 0.03f}, payload.object_color);
    //Average F0 for dielectric materials
    t_vec3 F0 = (t_vec3) {0.04f, 0.04f, 0.04f};
    // Get Proper F0 if material is not dielectric
    F0 = mix2(F0, payload.object_color, 1.0f);
    t_vec3 N = vec3_normalize(payload.normal);
    t_vec3 Ve = vec3_normalize(vec3_subtract(r.origin, payload.hit_point));

    float intensity = 1.0f;

	float l_dist = lightDist(payload.hit_point,l);
	intensity = l.intensity/(l_dist*l_dist);
    
    t_vec3 l_dir = lightRay(payload.hit_point,l).direction;
    t_vec3 H = vec3_normalize(vec3_add(Ve, l_dir));
    return vec3_add(ambient, computeReflectance(N,Ve,F0,payload.object_color,l_dir,H,l.color,intensity,1.0f,0.2f));
}


float length(t_vec3 v)
{
	return (sqrtf(v.x*v.x + v.y*v.y + v.z*v.z));
}

t_vec3 directIllumination(t_utils *utils, t_payload payload, t_ray r, float refl)
{
	(void) refl;
    t_vec3 color = (t_vec3) {0.0f, 0.0f, 0.0f};
    for(int i = 0 ; i < utils->scene->num_lights ; i++)
    {
        t_ray l_ray = lightRay(payload.hit_point, utils->scene->lights[i]);
		l_ray.origin = vec3_add(payload.hit_point, vec3_multiply_scalar(payload.normal, 0.0001f));
		//curr_ray.direction = reflect(curr_ray.direction, payload.normal, 0.0f);
		intersect_object(utils, r, &payload);
        //float d_light = lightDist(payload.hit_point,utils->scene->lights[i]);

        if(payload.t < 0 || (payload.t >= 0))
        {
			color = vec3_add(color, PBR2(payload, r, utils->scene->lights[i]));
        }
        else
        {
			color = vec3_multiply((t_vec3) {0.03f, 0.03f, 0.03f}, payload.object_color);
            //color +=  (t_vec3) {0.03f, 0.03f, 0.03f} * payload.material.color * hit.material.ao;
        }


        t_vec3 Ve = vec3_normalize(vec3_subtract(r.origin, payload.hit_point));
        t_vec3 H = vec3_normalize(vec3_add(Ve, l_ray.direction));
        refl = length(fresnelSchlick(fmax(vec3_dot_product(H, Ve), 0.0f),  mix2((t_vec3){0.04f,0.04f,0.04f}, payload.object_color, 0.0f)));
    }

    return color;
}

t_vec3 trace(t_utils *utils, t_ray r)
{
    t_vec3 accum = (t_vec3) {0.0f, 0.0f, 0.0f};
    t_vec3 mask = (t_vec3) {0.0f, 0.0f, 0.0f};
	t_payload payload;

    int nb_refl = 2; // Bounce number
    float c_refl = 1.0f;

    t_ray curr_ray = r;
    for(int i = 0 ; i <= nb_refl ; i++)
    {
		intersect_object(utils, r, &payload);
        if(payload.t >= 0)
        {
            //PBRMat mat = pbr_mat[io.i];

            //t_payload hs = HitSurface(curr_ray.ro + io.d*curr_ray.rd, computeNormal(io,curr_ray),mat);               

            t_vec3 color = directIllumination(utils, payload,curr_ray,c_refl);
            accum = vec3_add(accum, vec3_multiply(mask, color));
            mask = vec3_multiply_scalar(mask, c_refl);
			curr_ray.origin = vec3_add(payload.hit_point, vec3_multiply_scalar(payload.normal, 0.0001f));
			curr_ray.direction = reflect(curr_ray.direction, payload.normal, 0.0f);
            //curr_ray  = Ray(hs.hit_point + 0.001*hs.normal,reflect(curr_ray.rd,hs.normal));
        }
        else
        {
            break;
        }
    }

    return accum;

}
/*t_color trace_path(t_utils *utils, t_ray ray, int depth)
{
	t_payload	payload;
	t_color		color;

	if (depth == 0)
		return ((t_color){200, 190, 240});

	if (intersect_object(utils, ray, &payload) == true)
	{
		t_vec3 hit_point = payload.hit_point;
		//t_vec3 normal = payload.normal;
		t_vec3 light_direction = payload.light_direction;
		color = payload.object_color;

		void *object;
		t_color object_color;

		if (payload.object_type == SPHERE)
		{
			object = &utils->scene->spheres[payload.object_index];
			object_color = ((t_sphere *)object)->color;
		}
		else if (payload.object_type == CYLINDER)
		{
			object = &utils->scene->cylinders[payload.object_index];
			object_color = ((t_cylinder *)object)->color;
		}
		else if (payload.object_type == PLANE)
		{
			object = &utils->scene->plans[payload.object_index];
			object_color = ((t_plane *)object)->color;
		}
		//t_vec3 V = vec3_normalize(vec3_subtract(utils->scene->camera->pos, hit_point));  // View direction
		//t_vec3 H = vec3_normalize(vec3_add(V, light_direction));  // Halfway vector
		//t_vec3 F0 = {0.01f, 0.01f, 0.01f};  // Fresnel reflectance at normal incidence for a dielectric material. You should change it depending on your material properties.

		if (is_in_shadow(utils, hit_point))
		{
			color = color_multiply_scalar(object_color, utils->scene->alight->intensity);
		}
		else
		{	
			color = Lambertian(payload, light_direction, (t_color) {10, 20, 30});
			//color = PBR(utils, F0, V, H, payload);  // Call to PBR function.
		}
	}
	else
	{
		color = (t_color){200, 190, 240};
	}
	return (color)};*/

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
    //int depth;
    t_color old_color;
    t_color new_color;
	//t_vec3 light_color;

	//light_color = utils->scene->lights->color;

    //depth = 5;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            ray.origin = camera->pos;
            ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
            t_vec3 color = trace(utils, ray);
			color = reinhard_tone_mapping(color);
            t_color color_converted = (t_color){color.x * 255.0f, color.y * 255.0f, color.z * 255.0f};

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
