#include "../../includes/miniRT.h"

t_payload get_object(t_utils *utils, int x, int y)
{
	t_ray ray;
	t_payload payload;

	ray.origin = utils->scene->camera->pos;
	ray.direction = calculate_ray_direction(utils, x, y, WIDTH, HEIGHT);
	if (intersect_object(utils, ray, &payload) == true)
		printf("Object found\n");
	else
		printf("No object found\n");
	return (payload);
}


void object_move(t_utils *utils, int new_x, int new_y)
{
	t_plane move_plane;
	t_payload payload;
	t_sphere *sphere;
	t_ray mouse_ray;

	utils->move->init_position.x = new_x;
	utils->move->init_position.y = new_y;

	if (utils->move->object_is_selected == 0)
	{
		payload = get_object(utils, new_x, new_y);
		if (payload.object_type == SPHERE)
		{
			utils->move->object_is_selected = 1;
			utils->move->sphere = &utils->scene->spheres[payload.object_index];
		}
	}
	sphere = utils->move->sphere;
	if (!sphere)
		return ;

	move_plane.normal = utils->scene->camera->dir;
	move_plane.pos = sphere->center;
	mouse_ray.origin = utils->scene->camera->pos;
	mouse_ray.direction = calculate_ray_direction(utils, new_x, new_y, WIDTH, HEIGHT);

	float t;
	if (intersect_plane(mouse_ray, move_plane, &t))
	{
		t_vec3 intersection = vec3_add(mouse_ray.origin, vec3_multiply_scalar(mouse_ray.direction, t));

		sphere->center = intersection;
	}

}


void object_change_radius(t_utils *utils, int x, int y, float delta_radius)
{
	t_payload payload;
	t_sphere *sphere;

	payload = get_object(utils, x, y);
	if (payload.object_type != SPHERE)
		return;

	sphere = &utils->scene->spheres[payload.object_index];

	sphere->radius += delta_radius;
}
