#include "structures.h"
#include "utils.h"
#include "trace.h"

void    set_face_normal(t_ray *r, t_hit_record *rec)
{
    rec->front_face = vdot(r->dir, rec->normal) < 0;
    rec->normal = (rec->front_face) ? rec->normal : vmult(rec->normal, -1);
}

t_bool  hit(t_object *world, t_ray *ray, t_hit_record *rec)
{
    t_bool          hit_anything;
    t_hit_record    temp_rec;

    temp_rec = *rec;
    hit_anything = FALSE;
    while (world)
    {
        if (hit_obj(world, ray, &temp_rec))
        {
            hit_anything = TRUE;
            temp_rec.tmax = temp_rec.t;
            *rec = temp_rec;
        }
        world = world->next;
    }
    return (hit_anything);
}

t_bool  hit_obj(t_object *world, t_ray *ray, t_hit_record *rec)
{
    t_bool  hit_result;

    hit_result = FALSE;
    if (world->type == SP)
        hit_result = hit_sphere(world, ray, rec);
    return (hit_result);
}

t_bool  hit_sphere(t_object *world, t_ray *ray, t_hit_record *rec)
{
    t_sphere    *sp;
    t_vec3      oc;
    double      a;
    double      half_b;
    double      c;
    double      discriminant;
    double      sqrtd;
    double      root;

    sp = world->element;
    oc = vminus(ray->orig, sp->center);
    a = vlength2(ray->dir);
    half_b = vdot(oc, ray->dir);
    c = vlength2(oc) - sp->radius2;
    discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return (FALSE);
    sqrtd = sqrt(discriminant);
    root = (-half_b - sqrtd) / a;
    if (root < rec->tmin || rec->tmax < root)
    {
        root = (-half_b + sqrtd) / a;
        if (root < rec->tmin || rec->tmax < root)
            return (FALSE);
    }
    rec->t = root;
    rec->p = ray_at(ray, root);
    rec->normal = vdivide(vminus(rec->p, sp->center), sp->radius);
    rec->albedo = world->albedo;
    set_face_normal(ray, rec);
    return (TRUE);
}
