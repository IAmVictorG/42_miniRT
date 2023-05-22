#include "../includes/miniRT.h"

t_vec3 rotate_vector_with_pitch_yaw(t_vec3 vec, float pitch, float yaw)
{
    t_matrix4x4 rot_x = create_rotation_matrix_x(pitch);
    t_matrix4x4 rot_y = create_rotation_matrix_y(yaw);

    t_matrix4x4 rot_matrix = mat4x4_multiply(rot_y, rot_x);
    t_vec3 rotated_vec = mat4x4_mul_vec3(rot_matrix, vec);

    return rotated_vec;
}


void change_camera_position(int key, t_utils *utils)
{
    int move_speed = 2;

    t_vec3 up = vec3_cross_product(utils->scene->camera->dir, vec3_cross_product((t_vec3){0, 1, 0}, utils->scene->camera->dir));

    if (key == MAC_W || key == 119)
    {
        has_moved(utils);
        printf("Camera go DOWN\n");
        utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(up, move_speed));      
    }
    else if (key == MAC_A || key == 97)
    {
        has_moved(utils);
        printf("Camera go LEFT\n");
        t_vec3 left = vec3_cross_product(up, utils->scene->camera->dir);
        left = vec3_normalize(left);
        utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(left, move_speed));
    }
    else if (key == MAC_S || key == 115)
    {
        has_moved(utils);
		printf("Camera go UP\n");
        utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(up, move_speed));

    }
    else if (key == MAC_D || key == 100)
    {
        has_moved(utils);
        printf("Camera go RIGHT\n");
        t_vec3 right = vec3_cross_product(up, utils->scene->camera->dir);
        right = vec3_normalize(right);
        utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(right, move_speed));
    }
    else if (key == MAC_Q || key == 113)
    {
        has_moved(utils);
		printf("Camera go FORWARD\n");
        utils->scene->camera->pos = vec3_subtract(utils->scene->camera->pos, vec3_multiply_scalar(utils->scene->camera->dir, move_speed));
    }
    else if (MAC_E || key == 101)
    {
        has_moved(utils);
		printf("Camera go BACKWARD\n");
        utils->scene->camera->pos = vec3_add(utils->scene->camera->pos, vec3_multiply_scalar(utils->scene->camera->dir, move_speed));
    }
    else
    {
        printf("Do nth");
    }
}


void change_camera_direction(t_utils *utils, int new_x, int new_y)
{

	float sensitivity = 0.5f;
    int delta_x;
    int delta_y;

    delta_x = utils->move->init_position.x - new_x;
    delta_y = utils->move->init_position.y - new_y;

    utils->move->init_position.x = new_x;
    utils->move->init_position.y = new_y;

    float pitch = delta_y * sensitivity * PI / 180.0f;
    float yaw = delta_x * sensitivity * PI / 180.0f;

    utils->scene->camera->pitch += pitch;
    utils->scene->camera->yaw += yaw;

    t_vec3 initial_dir = {0, 0, -1}; // Assuming the initial direction is aligned with the negative Z-axis
    utils->scene->camera->dir = vec3_multiply_scalar(rotate_vector_with_pitch_yaw(initial_dir, utils->scene->camera->pitch, utils->scene->camera->yaw), -1);
}

