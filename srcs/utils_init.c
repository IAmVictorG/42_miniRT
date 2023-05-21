#include "../includes/miniRT.h"

void    init_objects_nb(FILE *file, t_scene *scene)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    scene->num_spheres = 0;
    scene->num_cameras = 1;
    scene->num_lights = 0;
    scene->num_plans = 0;
    while ((read = getline(&line, &len, file)) != -1)
    {
        if (line[0] == 'L' && is_space(line[1])) // Light
        {
            scene->num_lights++;
        }
        else if (line[0] == 's' && line[1] == 'p' && is_space(line[2])) // Sphere
        {
            scene->num_spheres++;            
        }
        else if (line[0] == 'p' && line[1] == 'l' && is_space(line[2])) // Plane
        {
            scene->num_plans++;
        }
        /*else if (line[0] == 's' && line[1] == 'q' && is_space(line[2])) // Square
        {
            parse_square(scene, line);
        }
        else if (line[0] == 'c' && line[1] == 'y' && is_space(line[2])) // Cylinder
        {
            parse_cylinder(scene, line);
        }
        else if (line[0] == 't' && line[1] == 'r' && is_space(line[2])) // Triangle
        {
            parse_triangle(scene, line);
        }*/
    }
    if (line)
        free(line);
    printf("NUM sphere %d\n", scene->num_spheres);
}

t_img *init_img(t_vars *vars, int width, int height)
{
    t_img *img = malloc(sizeof(t_img));
    if (!img)
        return NULL;

    img->img_ptr = mlx_new_image(vars->mlx_ptr, width, height);
    if (!img->img_ptr)
    {
        free(img);
        return NULL;
    }

    img->addr = (unsigned char *)mlx_get_data_addr(img->img_ptr, &img->bits_per_pixel, &img->size_line, &img->endian);
    if (!img->addr)
    {
        mlx_destroy_image(vars->mlx_ptr, img->img_ptr);
        free(img);
        return NULL;
    }
    return img;
}

void    init_utils(t_utils *utils)
{
    utils->vars = (t_vars *)malloc(sizeof(t_vars));
    if (!utils->vars)
    {
        perror("Error allocating memory for t_vars");
        exit(1);
    }
    utils->move = (t_move *)malloc(sizeof(t_move));
    if (!utils->move)
    {
        perror("Error allocating memory for t_vars");
        exit(1); 
    }
    utils->vars->mlx_ptr = mlx_init();
    utils->vars->win_ptr = mlx_new_window(utils->vars->mlx_ptr, WIDTH, HEIGHT, "miniRT");
    utils->img = init_img(utils->vars, WIDTH, HEIGHT);
    utils->img->accumulate = 1;
    utils->img->frames = 1;
    utils->img->accumulator = malloc(WIDTH * HEIGHT * sizeof(t_color));
    utils->move->init_position = ((t_vec2) {0,0});
    utils->move->mouse_right_click = 0;
    utils->move->mouse_left_click = 0;
    utils->move->moved = 0;
    utils->move->object_is_selected = 0;
}