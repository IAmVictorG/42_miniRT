#include "../../includes/miniRT.h"

int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\f');
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	unsigned char	*dst;

	dst = img->addr + (y * img->size_line + x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	has_moved(t_utils *utils)
{
	utils->move->moved = 1;
	utils->img->frames = 1;
	free(utils->img->accumulator);
	utils->img->accumulator = malloc(WIDTH * HEIGHT * sizeof(t_color));
}