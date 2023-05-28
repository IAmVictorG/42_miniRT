#include "../../includes/miniRT.h"

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
    set_vector(r, &scene->camera->pos);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);

    set_vector(r, &scene->camera->dir);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->camera->fov = atof(r);
    scene->camera->pitch = 0.0f;
    scene->camera->yaw = 0.0f;
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
    set_vector(r, &scene->lights->pos);
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
    static int i = 0;

    if (i == 0)
    {
        scene->spheres = (t_sphere *)malloc(sizeof(t_sphere) * scene->num_spheres);
        if (scene->spheres == NULL)
        {
            printf("Malloc error\n");
            return;
        }
    }
    printf("I %d\n", i);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_vector(r, &scene->spheres[i].center);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->spheres[i].radius = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->spheres[i].material.color);
    printf("Sphere position %f,%f,%f | radius %f | Color %d,%d,%d\n", scene->spheres[i].center.x, scene->spheres[i].center.y, scene->spheres[i].center.z, scene->spheres[i].radius, scene->spheres[i].material.color.r, scene->spheres[i].material.color.g, scene->spheres[i].material.color.b);
    free(r);
    i++;
}

static void parse_plane(t_scene *scene, char *line)
{
    char *r;
    static int i = 0;

    scene->plans = (t_plane *)malloc(sizeof(t_plane) * scene->num_plans);
    if (scene->plans == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_vector(r, &scene->plans[i].pos);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);

    set_vector(r, &scene->plans[i].normal);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->plans[i].color);
    free(r);
    printf("Plan Pos %f,%f,%f | Dir %f,%f,%f | Color %d,%d,%d | Normal %f,%f,%f\n", scene->plans[i].pos.x, scene->plans[i].pos.y, scene->plans[i].pos.z, scene->plans[i].normal.x, scene->plans[i].normal.y, scene->plans[i].normal.z, scene->plans[i].color.r, scene->plans[i].color.g, scene->plans[i].color.b, scene->plans[i].normal.x, scene->plans[i].normal.y, scene->plans[i].normal.z);
    i++;
}

static void parse_cylinder(t_scene *scene, char *line)
{
    char *r;
    static int i = 0;    


    scene->cylinders = (t_cylinder *)malloc(sizeof(t_cylinder) * scene->num_cylinders);
    if (scene->cylinders == NULL)
    {
        printf("Malloc error\n");
        return ;
    }
    go_to_next_arg(&line);
    r = get_arg(line);
    set_vector(r, &scene->cylinders[i].bottom);
    go_to_next_arg(&line);
    free(r);
    r = get_arg(line);
    set_vector(r, &scene->cylinders[i].direction);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->cylinders[i].radius = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    scene->cylinders[i].height = atof(r);
    free(r);
    go_to_next_arg(&line);
    r = get_arg(line);
    set_rgb(r, &scene->cylinders[i].color);
    free(r);
    printf("Cylinder center base %f,%f,%f | radius %f | height %f | color %d,%d,%d\n", scene->cylinders[i].bottom.x, scene->cylinders[i].bottom.y,  scene->cylinders[i].bottom.z, scene->cylinders[i].radius, scene->cylinders->height, scene->cylinders[i].color.r, scene->cylinders[i].color.g, scene->cylinders[i].color.b);
    i++;
}


/*static void parse_square(t_scene *scene, char *line)
{
    // Parse square properties and add to scene
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
    else if (line[0] == 'c' && line[1] == 'y' && is_space(line[2])) // Cylinder
    {
        parse_cylinder(scene, line);
    }
    /*else if (line[0] == 's' && line[1] == 'q' && is_space(line[2])) // Square
    {
        parse_square(scene, line);
    }
    else if (line[0] == 't' && line[1] == 'r' && is_space(line[2])) // Triangle
    {
        parse_triangle(scene, line);
    }*/
}