#ifndef MINIRT_H
# define MINIRT_H


# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <pthread.h>
# include <stdbool.h>
# include <time.h>
# include <limits.h>
# include <float.h>
# include "minilibx-openGL/mlx.h"
# include "get_next_line/get_next_line.h"
# include "libft.h"
# include "scene.h"

# define WIDTH	450
# define HEIGHT 450

# define MAC_KEY_LEFT   123
# define MAC_KEY_RIGHT  124
# define MAC_KEY_DOWN   125
# define MAC_KEY_UP     126
# define MAC_KEY_SPACE  49
# define MAC_KEY_ESCAPE 53
# define MAC_R 15
# define MAC_E 14
# define MAC_W 13
# define MAC_Q 12
# define MAC_A 0
# define MAC_S 1
# define MAC_D 2
# define MAC_F 3
# define MAC_G 5

# define WHITE 0xFFFFFF
# define BLACK 0x000000

# define PI	3.14159265358979323846

enum {
	ON_KEYDOWN = 2,
	ON_KEYUP = 3,
	ON_MOUSEDOWN = 4,
	ON_MOUSEUP = 5,
	ON_MOUSEMOVE = 6,
	ON_EXPOSE = 12,
	ON_DESTROY = 17
};

typedef struct  s_matrix4x4
{
    float m[4][4];
}               t_matrix4x4;

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
	int				accumulate;
	int				frames;
	t_color			*accumulator;
}					t_img;

typedef struct s_move_mouse
{
	t_vec2	init_position;
	int		mouse_right_click;
	int		mouse_left_click;
	int		object_is_selected;
	int		moved;
	t_sphere *sphere;
}	t_move;

typedef struct s_utils
{
	t_vars	*vars;
	t_img	*img;
	t_scene	*scene;
	t_move	*move;
}	t_utils;

typedef enum
{
    SPHERE,
    PLANE,
    CYLINDER,
	TRIANGLE
} ObjectType;

typedef struct s_payload
{
	ObjectType	object_type;
	int			object_index;
	t_vec3		hit_point;
	t_vec3		light_direction;
	t_vec3		normal;
	t_color		object_color;
	float		t;
	float		hit_distance;
} t_payload;



void	init_utils(t_utils *utils);
void    init_objects_nb(FILE *file, t_scene *scene);
void	event_handler(t_utils *utils);
void	parse_line(t_scene *scene, char *line);
int		read_rt_file(const char *filename, t_scene *scene);
void	render_scene(t_scene *scene, t_utils *utils);
void	display_image(t_utils *utils);
int		is_space(char c);

//Render
void	render_image(t_utils *window);
t_vec3 calculate_ray_direction(t_utils *utils, int pixel_x, int pixel_y, int image_width, int image_height);

//Parse
char	*get_arg(char *line);
void	go_to_next_arg(char **line);
void	set_rgb(char *str, t_color *color);
void	set_vector(char *str, t_vec3 *vec);

//Utils
int		create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
t_color color_multiply_scalar(t_color color, float scalar);
void	has_moved(t_utils *utils);

t_vec3  vec3_multiply_scalar(t_vec3 a, float scalar);
t_vec3  vec3_add(t_vec3 a, t_vec3 b);
t_vec3  vec3_normalize(t_vec3 v);
t_vec3	vec3_subtract(t_vec3 a, t_vec3 b);
t_vec3	vec3_cross_product(t_vec3 a, t_vec3 b);
t_vec3	perturb_vector(t_vec3 v, float roughness);
t_vec3	reflect(t_vec3 v, t_vec3 n, float roughness);
float	vec3_dot_product(t_vec3 a, t_vec3 b);
float	vec3_length(t_vec3 v);
float	vec3_distance(t_vec3 v1, t_vec3 v2);
t_vec3 vec3_multiply(t_vec3 a, t_vec3 b);

t_color color_add(t_color color1, t_color color2);
t_payload get_object(t_utils *utils, int x, int y);
void object_move(t_utils *utils, int x, int y);

//Intersect
bool intersect_object(t_utils *utils, t_ray ray, t_payload *payload);
bool intersect_plane(t_ray ray, t_plane plane, float *t);
//bool intersect_sphere(t_ray ray, t_sphere sphere, float *t);


//Camera
void    change_camera_position(int key, t_utils *utils);
void	change_camera_direction(t_utils *utils, int new_x, int new_y);

//Mouse events
int		mouse_press(int key, int x, int y, t_utils *utils);
int		mouse_release(int button, int x, int y, t_utils *utils);
int		mouse_move(int x, int y, t_utils *utils);


t_matrix4x4 create_rotation_matrix_x(float angle_rad);
t_matrix4x4 create_rotation_matrix_y(float angle_rad);
t_vec3 mat4x4_mul_vec3(t_matrix4x4 m, t_vec3 v);
t_matrix4x4 mat4x4_multiply(t_matrix4x4 a, t_matrix4x4 b);
t_matrix4x4 create_rotation_matrix_axis_angle(t_vec3 axis, float angle);



t_color PBR(t_utils *utils, t_vec3 F0, t_vec3 V, t_vec3 H, t_payload payload);


#endif