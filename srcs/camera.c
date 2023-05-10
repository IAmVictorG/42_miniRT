#include "../includes/header.h"

void    change_camera_position(int key, t_utils *utils)
{
	t_vec3 up = {0, 1, 0};
	int		move_speed = 3;

    if (key == W || key == 119)
	{
		printf("Camera go FORWARD\n");
		utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(utils->scene->camera->dir, move_speed));
	}
	else if (key == A || key == 97)
	{
		printf("Camera go LEFT\n");
		t_vec3 left = vec3_cross_product(up, utils->scene->camera->dir);
		left = vec3_normalize(left);
		utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(left, move_speed));
	}
	else if (key == S || key == 115)
	{
		printf("Camera go BACKWARD\n");
		utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(utils->scene->camera->dir, move_speed));
	}
	else if (key == D || key == 100)
	{
		printf("Camera go RIGHT\n");
		t_vec3 right = vec3_cross_product(up, utils->scene->camera->dir);
		right = vec3_normalize(right);
		utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(right, move_speed));
	}
	else if (key == 113)
	{
		printf("Camera go UP\n");
		utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(up, move_speed));
	}
	else if (key == 101)
	{
		printf("Camera go DOWN\n");
		utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(up, move_speed));
	}
}

void change_camera_direction(t_utils *utils, int delta_x, int delta_y)
{
	(void) utils;
	(void) delta_x;
	(void) delta_y;
}