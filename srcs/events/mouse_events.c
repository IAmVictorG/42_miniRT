#include "../../includes/miniRT.h"

int	mouse_press(int button, int x, int y, t_utils *utils)
{

	(void) button,
	(void) x;
	(void) y;
	(void) utils;

	printf("key = [%d]\n", button);
    if (button == 3)
    {
		utils->move->init_position.x = x;
		utils->move->init_position.y = y;
        utils->move->mouse_right_click = 1;
    }
	if (button == 1)
	{
		utils->move->init_position.x = x;
		utils->move->init_position.y = y;
		utils->move->mouse_left_click = 1;
	}
	return (1);
}

int mouse_release(int button, int x, int y, t_utils *utils)
{
    (void) x;
    (void) y;
	if (button == 1)
	{
		utils->move->mouse_left_click = 0;
		utils->move->object_is_selected = 0;
		utils->move->sphere = NULL;
	}
	if (button == 3)
	{
		utils->move->mouse_right_click = 0;
	}
	return (0);
}

int mouse_move(int x, int y, t_utils *utils)
{
    (void) x;
    (void) y;


    //printf("Camera direction %f, %f, %f\n", utils->scene->camera->dir.x, utils->scene->camera->dir.y, utils->scene->camera->dir.z);
	if (utils->move->mouse_right_click)
	{
        //printf("Mouse move and is pressed\n");
		has_moved(utils);
		change_camera_direction(utils, x, y);
	
	}
	if (utils->move->mouse_left_click)
	{
		has_moved(utils);
		object_move(utils, x, y);
	}
	return (0);
}
