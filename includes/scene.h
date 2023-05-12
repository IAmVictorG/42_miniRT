#ifndef SCENE_H
# define SCENE_H

typedef struct s_vec2
{
	float x;
	float y;
} t_vec2;

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

typedef struct s_camera
{
	t_vec3 pos;
	t_vec3 dir;
	float yaw;
	float pitch;
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
	t_vec3 normal;
	t_color color;
} t_plan;

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

#endif