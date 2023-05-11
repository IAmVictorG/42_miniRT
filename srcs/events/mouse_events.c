#include "../../includes/header.h"

int	mouse_press(int button, int x, int y, t_utils *utils)
{
	(void) button,
	(void) x;
	(void) y;
	(void) utils;

	printf("key = [%d]\n", button);
    if (button == 2)
    {
		utils->move->init_position.x = x;
		utils->move->init_position.y = y;
        utils->move->mouse_is_pressed = 1;
    }
	return (1);
}

int mouse_release(int button, int x, int y, t_utils *utils)
{
    (void) x;
    (void) y;
	if (button == 2)
	{
		utils->move->mouse_is_pressed = 0;
	}
	return (0);
}

int mouse_move(int x, int y, t_utils *utils)
{
    (void) x;
    (void) y;

    printf("Camera direction %f, %f\n", utils->scene->camera->dir.x, utils->scene->camera->dir.y);
	if (utils->move->mouse_is_pressed)
	{
        printf("Mouse move and is pressed\n");
		change_camera_direction(utils, x, y);
	}
    else
    {
        printf("Mouse move\n");
    }
	return (0);
}
