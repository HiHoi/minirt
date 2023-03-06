#include    "structures.h"
#include    "util.h"
#include <stdlib.h>

void    oadd(t_object **list, t_object *new)
{
    t_object    *cur;

    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

t_object    *olast(t_object *list)
{
    if (list == NULL)
        return (NULL);
    while (list->next)
        list = list->next;
    return (list);
}

t_object    *object(t_object_type type, void *element, t_color3 albedo)
{
    t_object    *new;

    if (!(new = malloc(sizeof(t_object))))
        return (NULL);
    new->type = type;
    new->element = element;
    new->albedo = albedo;
    new->next = NULL;
    return (new);
}

t_sphere    *sphere(t_point3 center, double radius)
{
    t_sphere    *sp;

    if (!(sp = malloc(sizeof(t_sphere))))
        return (NULL);
    sp->center = center;
    sp->radius = radius;
    sp->radius2 = radius * radius;
    return (sp);
}

t_plane     *plane(t_point3 center, t_vec3 dir)
{
    t_plane *pl;

    pl = malloc(sizeof(t_plane));
    if (pl == NULL)
        return (NULL);
    pl->center = center;
    pl->dir = dir;
    return (pl);
}

t_cylinder  *cylinder(t_point3 center, t_vec3 dir, double radius, double height)
{
    t_cylinder      *cyl;

    cyl = malloc(sizeof(t_cylinder));
    if (cyl == NULL)
        return (NULL);
    cyl->center = center;
    cyl->height = height;
    cyl->radius = radius;
    cyl->dir = dir;
    return (cyl);
}

t_light *light_point(t_point3 light_origin, t_color3 light_color, double bright_ratio)
{
    t_light *light;

    if (!(light = malloc(sizeof(t_light))))
        return (NULL);
    light->origin = light_origin;
    light->light_color = light_color;
    light->bright_ratio = bright_ratio;
    return (light);
}
