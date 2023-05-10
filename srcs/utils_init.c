#include "../includes/header.h"

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
    if (!utils->vars) {
        perror("Error allocating memory for t_vars");
        exit(1);
    }
    utils->vars->mlx_ptr = mlx_init();
    utils->vars->win_ptr = mlx_new_window(utils->vars->mlx_ptr, WIDTH, HEIGHT, "miniRT");
    utils->img = init_img(utils->vars, WIDTH, HEIGHT);

}