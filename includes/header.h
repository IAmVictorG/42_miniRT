#ifndef HEADER_H
# define HEADER_H


# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>
# include <time.h>
# include "minilibx-linux/mlx.h"
# include "libft.h"
//# include "../libft/libft.h"
# define WIDTH	700
# define HEIGHT 700

# define KEY_LEFT   123
# define KEY_RIGHT  124
# define KEY_DOWN   125
# define KEY_UP     126
# define KEY_SPACE  49
# define KEY_ESCAPE 53
# define E 14
# define R 15
# define W 13
# define A 0
# define S 1
# define D 2
# define F 3
# define G 5

# define WHITE 0xFFFFFF
# define BLACK 0x000000

enum {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};


typedef struct s_vec3
{
	float x;
	float y;
	float z;
} t_vec3;

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 direction;
} t_ray;

typedef struct s_camera
{
	t_vec3 pos;
	t_vec3 dir;
	float fov;
} t_camera;

typedef struct s_sphere
{
	t_vec3 center;
	float radius;
	t_color color;
} t_sphere;

typedef struct s_plan
{
	t_vec3 pos;
	t_vec3 dir;
	t_color color;
} t_plan;

typedef struct		s_vars
{
	void			*mlx_ptr;
	void			*win_ptr;
}	t_vars;

typedef struct		s_img
{
	void			*img_ptr;
 	int				bits_per_pixel;
	int				size_line;
	int				endian;
	unsigned char	*addr;
}					t_img;

typedef struct s_alight
{
	float intensity;
	t_color color;
} t_alight;

typedef struct s_light
{
	t_vec3 pos;
	float intensity;
	t_color color;
} t_light;

typedef struct s_scene
{
	t_alight *alight;

	t_camera *camera;
	int num_cameras;

	t_light *lights;
	int num_lights;

	t_sphere *spheres;
	int	num_spheres;

	t_plan *plans;
	int num_plans;

} t_scene;


typedef struct s_utils
{
	t_vars	*vars;
	t_img	*img;
	t_scene	*scene;
} t_utils;


void	init_utils(t_utils *utils);
void	event_handler(t_utils *utils);
void	parse_line(t_scene *scene, char *line);
int		read_rt_file(const char *filename, t_scene *scene);
void	render_scene(t_scene *scene, t_utils *utils);
void	display_image(t_utils *utils);
int		is_space(char c);

//Render
void	render_image(t_utils *window);

//Parse
char	*get_arg(char *line);
void	go_to_next_arg(char **line);
void	set_rgb(char *str, t_color *color);
void	set_position(char *str, t_vec3 *pos);

//Utils
int		create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);


t_vec3  vec3_multiply_scalar(t_vec3 a, float scalar);
t_vec3  vec3_add(t_vec3 a, t_vec3 b);
t_vec3  vec3_normalize(t_vec3 v);
t_vec3	vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross_product(t_vec3 a, t_vec3 b);
float	vec3_dot_product(t_vec3 a, t_vec3 b);

//Camera
void    change_camera_position(int key, t_utils *utils);
void	change_camera_direction(t_utils *utils, int delta_x, int delta_y);

#endif