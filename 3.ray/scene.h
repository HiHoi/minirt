#ifndef SCENE_H
# define SCENE_H

# include "structures.h"
# include "utils.h"

t_canvas    canvas(int width, int height);
t_camera    camera(t_canvas *canvas, t_point3 origin);
t_object    *object(t_object_type type, void *element, t_color3 albedo);
t_sphere    *sphere(t_point3 center, double radius);
t_cylinder  *cylinder(t_point3 center, t_vec3 dir, double radius, double height);
t_plane     *plane(t_point3 center, t_vec3 dir);
t_light     *light_point(t_point3 light_origin, t_color3 light_color, double bright_ratio);

void        oadd(t_object **list, t_object *new);
t_object    *olast(t_object *list);

#endif
