#include "trace.h"

t_hit_record    record_init(void)
{
	t_hit_record    record;

	record.tmax = INFINITY;
	record.tmin = EPSILON;
	return (record);
}

t_ray   ray(t_point3 orig, t_vec3 dir)
{
	t_ray   ray;

	ray.orig = orig;
	ray.dir = vunit(dir);
	return (ray);
}

t_point3    ray_at(t_ray *ray, double t)
{
	t_point3    at;

	at = vplus(ray->orig, vmult(ray->dir, t));
	return (at);
}

t_ray   ray_primary(t_camera *cam, double u, double v)
{
	t_ray   ray;

	ray.orig = cam->orig;
	ray.dir = vunit(vminus(vplus(vplus(cam->left_bottom, vmult(cam->horizontal, u)), vmult(cam->vertical, v)), cam->orig));
	return (ray);
}

t_vec3	random_vecter_void(void)
{
	return (vec3(random_double_void(), random_double_void(), random_double_void()));
}

t_vec3	random_vecter(double min, double max)
{
	return (vec3(random_double(min, max), random_double(min, max), random_double(min, max)));
}

t_vec3	random_in_unit_sphere(void)
{
	t_vec3	p;

	while (TRUE)
	{
		p = random_vecter(-1, 1);
		if (vlength2(p) >= 1)
			continue ;
		else
			break ;
	}
	return (p);
}

/*
color ray_color(const ray& r, const hittable& world) {
  hit_record rec;

  if (world.hit(r, 0, infinity, rec)) {
    point3 target = rec.p + rec.normal + random_in_unit_sphere();
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world);
  }

  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}
*/

t_vec3	random_unit_vector(void)
{
	double	a;
	double	z;
	double	r;

	a = random_double(0, 2 * 3.14);
	z = random_double(-1, 1);
	r = sqrt(1 - z * z);
	return (vec3(r * cos(a), r * sin(a), z));
}

t_vec3	random_in_hemisphere(t_vec3 normal)
{
	t_vec3	in_hemi = random_in_unit_sphere();

	if (vdot(in_hemi, normal) > 0.0)
		return (in_hemi);
	else
		return(vmult(in_hemi, -1.0));
}

t_color3    ray_color(t_scene *scene, int depth)
{
	double          t;
	t_vec3			unit_dir;
	// t_point3		target;

	scene->rec = record_init();
	if (depth <= 0)
		return (color3(0, 0, 0));
	if (hit(scene->world, &scene->ray, &scene->rec))
	{
		// target = vplus(scene->rec.p, random_in_hemisphere(scene->rec.normal));
		// scene->ray = ray(scene->rec.p, vminus(target, scene->rec.p));
		// return (vmult(ray_color(scene, depth - 1), 0.5));
		return (phong_lighting(scene));
	}
	else
	{
		unit_dir = vunit(scene->ray.dir);
		t = 0.5 * (unit_dir.y + 1.0);
		return (vplus(vmult(color3(1, 1, 1), 1.0 - t), vmult(color3(0.5, 0.7, 1.0), t)));
	}
}

t_vec3  reflect(t_vec3 v, t_vec3 n)
{
	return (vminus(v, vmult(n, vdot(v, n) * 2)));
}

t_color3    point_light_get(t_scene *scene, t_light *light)
{
	t_color3    diffuse;
	t_vec3      light_dir;
	double      kd;
	t_color3    specular;
	t_vec3      view_dir;
	t_vec3      reflect_dir;
	double      spec;
	double      ksn;
	double      ks;
	double      brightness;
	double      light_len;
	t_ray       light_ray;

	light_dir = vminus(light->origin, scene->rec.p);
	light_len = vlength(light_dir);
	light_ray = ray(vplus(scene->rec.p, vmult(scene->rec.normal, EPSILON)), light_dir);
	if (in_shadow(scene->world, light_ray, light_len))
		return (color3(0, 0, 0));
	light_dir = vunit(light_dir);
	kd = fmax(vdot(scene->rec.normal, light_dir), 0.0);
	diffuse = vmult(light->light_color, kd);
	view_dir = vunit(vmult(scene->ray.dir, -1));
	reflect_dir = reflect(vmult(light_dir, -1), scene->rec.normal);
	ksn = 64;
	ks = 0.5;
	spec = pow(fmax(vdot(view_dir, reflect_dir), 0.0), ksn);
	specular = vmult(vmult(light->light_color, ks), spec);
	brightness = light->bright_ratio * LUMEN;
	return (vmult(vplus(vplus(diffuse, specular), specular), brightness));
}

t_color3    phong_lighting(t_scene *scene)
{
	t_color3    light_color;
	t_object    *lights;

	light_color = color3(0, 0, 0);
	lights = scene->light;
	while (lights)
	{
		if (lights->type == LIGHT_POINT)
			light_color = vplus(light_color, point_light_get(scene, lights->element));
		lights = lights->next;
	}
	light_color = vplus(light_color, scene->ambient);
	return (vmin(vmult_(light_color, scene->rec.albedo), color3(1, 1, 1)));
}

t_bool  in_shadow(t_object *objs, t_ray light_ray, double light_len)
{
	t_hit_record    rec;

	rec.tmin = 0;
	rec.tmax = light_len;
	if (hit(objs, &light_ray, &rec))
		return (TRUE);
	return (FALSE);
}
