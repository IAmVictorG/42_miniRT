#include "../includes/miniRT.h"




int read_rt_file(const char *filename, t_scene *scene)
{
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    file = fopen(filename, "r");
    if (file == NULL)
        return -1;
    init_objects_nb(file, scene);
    fclose(file);
    file = fopen(filename, "r");
    while ((read = getline(&line, &len, file)) != -1)
    {
        parse_line(scene, line);
    }

    fclose(file);
    if (line)
        free(line);

    return 0;
}

int main(int argc, char const *argv[])
{
    t_utils utils;

    utils.scene = malloc(sizeof(t_scene));
    if (utils.scene == NULL)
    {
        fprintf(stderr, "Error allocating memory for scene\n");
        return 1;
    }
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s scene.rt\n", argv[0]);
        return 1;
    }

    init_utils(&utils);

    if (read_rt_file(argv[1], utils.scene) != 0)
    {
        fprintf(stderr, "Error reading scene file: %s\n", argv[1]);
        return 1;
    }

    //render_scene(&scene, &utils);
    //display_image(&utils);


    printf("SPHERE 1 %f\n", utils.scene->spheres[0].radius);
    printf("SPHERE 2 %f\n", utils.scene->spheres[1].radius);
    event_handler(&utils);
    
    return 0;
}