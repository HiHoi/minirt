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
    t_hit_record    *temp_rec;

    temp_rec = rec;
    hit_anything = FALSE;
    while (world)
    {
        if (hit_obj(world, ray, temp_rec))
        {
            hit_anything = TRUE;
            temp_rec->tmax = temp_rec->t;
            rec = temp_rec;
        }
        world = world->next;
    }
    return (hit_anything);
}

double	cy_boundary(t_cylinder *cy, t_vec3 at_point)
{
	double	hit_height;
	double	max_height;

	hit_height = vdot(vminus(at_point, cy->center), cy->dir);
	max_height = cy->height / 2;
	if (fabs(hit_height) > max_height)
		return (0);
	return (1);
}

t_vec3      get_cylinder_normal(t_cylinder *cy, t_vec3 at_point, double hit_height)
{
    t_point3 hit_center;
    t_vec3 normal;

    hit_center = vplus(cy->center, vmult(cy->dir, hit_height));
    normal = vminus(at_point, hit_center);
    return (vunit(normal));
}

t_bool      hit_cylinder_cap(t_object *cy_obj, t_ray *ray, t_hit_record *rec, double height)
{
    t_cylinder *cy = cy_obj->element;
    double r = cy->radius;
    t_vec3    circle_center = vplus(cy->center, vmult(cy->dir, height));
    double root = vdot(vminus(circle_center, ray->orig), cy->dir);
    double diameter = vlength(vminus(circle_center, ray_at(ray, root)));

	if (fabs(r) < fabs(diameter))
		return (FALSE);
    if (root < rec->tmin || rec->tmax < root)
       return (FALSE);
    rec->t = root; 
    rec->p = ray_at(ray, root);
    rec->tmax = rec->t;
    if (height > 0)
        rec->normal = cy->dir;
    else
        rec->normal = vmult(cy->dir, -1);
    // rec->normal = vunit(vminus(circle_center, ray->orig)); 
    set_face_normal(ray, rec);
    rec->albedo = cy_obj->albedo;
    return (TRUE);
}

t_bool      hit_cylinder_side(t_object *cy_obj, t_ray *ray, t_hit_record *rec)
{
    t_cylinder *cy;
    double  a;
	double  half_b;
    double  c;
    t_vec3  u;
    t_vec3  o;
    t_vec3  delta_P;
    double r;
    double discriminant; // 판별식
    double sqrtd;
    double root;
    double hit_height;
    
    cy = cy_obj->element;
    u = ray->dir;
    o = cy->dir;
    r = cy->radius;
    delta_P = vminus(ray->orig, cy->center);
    a = vlength2(vcross(u, o));
    half_b = vdot(vcross(u, o), vcross(delta_P, o));
    c = vlength2(vcross(delta_P, o)) - pow(r, 2);
    discriminant = half_b * half_b - a * c;
    if (discriminant < 0) 
        return (FALSE);
    // 이 시점에서 판별식이 참이 나왔기에 근이 존재한다고 판단한다.
    sqrtd = sqrt(discriminant); 
    root = (-half_b - sqrtd) / a;  // 근의 공식 해, 작은 근부터 고려.
    if (root < rec->tmin || rec->tmax < root)
    {
        root = (-half_b + sqrtd) / a; 
        if (root < rec->tmin || rec->tmax < root)
            return (FALSE);
    }
    if (!(hit_height = cy_boundary(cy, ray_at(ray, root))))
        return (FALSE);
    rec->t = root; // 광선의 원점과 교점까지의 거리를 rec에 저장한다.
    rec->p = ray_at(ray, root); // 교점의 좌표를 rec에 저장한다.
    rec->normal = get_cylinder_normal(cy, rec->p, hit_height);
    set_face_normal(ray, rec);
    rec->albedo = cy_obj->albedo;
    return (TRUE);
}

t_bool      hit_cylinder(t_object *cy_obj, t_ray *ray, t_hit_record *rec)
{
    const t_cylinder *cy = cy_obj->element;
    int result;

    result = FALSE;
    result += hit_cylinder_cap(cy_obj, ray, rec, cy->height / 2);
    result += hit_cylinder_cap(cy_obj, ray, rec, -1 * (cy->height / 2));
    result += hit_cylinder_side(cy_obj, ray, rec);
    return (result);
}

t_bool  hit_plane(t_object *world, t_ray *ray, t_hit_record *rec)
{
    t_plane *pl;
    float   numrator;
    float   denominator;
    float   root;

    pl = world->element;
    denominator = vdot(ray->dir, pl->dir);
    if (fabs(denominator) < EPSILON)
        return (FALSE);
    numrator = vdot(vminus(pl->center, ray->orig), pl->dir);
    root = numrator / denominator;
    if (root < rec->tmin || rec->tmax < root)
        return (FALSE);
    rec->t = root;
    rec->p = ray_at(ray, root);
    rec->normal = pl->dir;
	set_face_normal(ray, rec);
    rec->albedo = world->albedo;
    return (TRUE);
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

t_bool  hit_obj(t_object *world, t_ray *ray, t_hit_record *rec)
{
    t_bool  hit_result;

    hit_result = FALSE;
    if (world->type == SP)
        hit_result = hit_sphere(world, ray, rec);
    else if (world->type == PLANE)
        hit_result = hit_plane(world, ray, rec);
    else if (world->type == CYLINDER)
        hit_result = hit_cylinder(world, ray, rec);
    return (hit_result);
}
