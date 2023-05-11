#include "../../includes/header.h"

static void parse_ambiant_light(t_scene *scene, char *line)
{
    char *r;

    scene->alight = (t_alight *)malloc(sizeof(t_alight));
    if (scene->alight == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->alight->intensity = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->alight->color);
    free(r);
    printf("Ambiant light Intensity = %f, r,g,b = %d,%d,%d\n", scene->alight->intensity, scene->alight->color.r, scene->alight->color.g, scene->alight->color.b);
}

static void parse_camera(t_scene *scene, char *line)
{
    char *r;

    scene->camera = (t_camera *)malloc(sizeof(t_camera));
    if (scene->camera == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_position(r, &scene->camera->pos);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);

    set_position(r, &scene->camera->dir);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->camera->fov = atof(r);
    free(r);
    printf("Camera pos %f,%f,%f | direction %f,%f,%f | FOV %f\n", scene->camera->pos.x, scene->camera->pos.y, scene->camera->pos.z, scene->camera->dir.x, scene->camera->dir.y, scene->camera->dir.z, scene->camera->fov);
}

static void parse_light(t_scene *scene, char *line)
{
    char *r;

    scene->lights = (t_light *)malloc(sizeof(t_light));
    if (scene->lights == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_position(r, &scene->lights->pos);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->lights->intensity = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->lights->color);
    free(r);
    printf("Light Intesity = %f, r,g,b = %d,%d,%d pos x,y,z = %f,%f,%f\n", scene->lights->intensity, scene->lights->color.r, scene->lights->color.g, scene->lights->color.b, scene->lights->pos.x, scene->lights->pos.y, scene->lights->pos.z);
}

static void parse_sphere(t_scene *scene, char *line)
{
    char *r;

    scene->spheres = (t_sphere *)malloc(sizeof(t_sphere));
    if (scene->lights == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_position(r, &scene->spheres->center);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->spheres->radius = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->spheres->color);
    printf("Sphere position %f,%f,%f | radius %f | Color %d,%d,%d\n", scene->spheres->center.x, scene->spheres->center.y, scene->spheres->center.z, scene->spheres->radius, scene->spheres->color.r, scene->spheres->color.g, scene->spheres->color.b);
    free(r);
}

static void parse_plane(t_scene *scene, char *line)
{
    char *r;

    scene->plans = (t_plan *)malloc(sizeof(t_plan));
    if (scene->plans == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_position(r, &scene->plans->pos);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);

    set_position(r, &scene->plans->normal);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->plans->color);
    free(r);
    printf("Plan Pos %f,%f,%f | Dir %f,%f,%f | Color %d,%d,%d\n", scene->plans->pos.x, scene->plans->pos.y, scene->plans->pos.z, scene->plans->normal.x, scene->plans->normal.y, scene->plans->normal.z, scene->plans->color.r, scene->plans->color.g, scene->plans->color.b);
}

/*static void parse_square(t_scene *scene, char *line)
{
    // Parse square properties and add to scene
}

static void parse_cylinder(t_scene *scene, char *line)
{
    // Parse cylinder properties and add to scene
}

static void parse_triangle(t_scene *scene, char *line)
{
    // Parse triangle properties and add to scene
}*/

void parse_line(t_scene *scene, char *line)
{
    if (line[0] == 'A' && is_space(line[1]))
    {
        parse_ambiant_light(scene, line);
    }
    else if (line[0] == 'C' && is_space(line[1])) // Camera
    {
        parse_camera(scene, line);
    }
    else if (line[0] == 'L' && is_space(line[1])) // Light
    {
        parse_light(scene, line);
    }
    else if (line[0] == 's' && line[1] == 'p' && is_space(line[2])) // Sphere
    {
        parse_sphere(scene, line);
    }
    else if (line[0] == 'p' && line[1] == 'l' && is_space(line[2])) // Plane
    {
        parse_plane(scene, line);
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