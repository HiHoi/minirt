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

t_color3    ray_color(t_scene *scene)
{
	double          t;
	// t_vec3          n;

	scene->rec = record_init();
	if (hit(scene->world, &scene->ray, &scene->rec))
		return (phong_lighting(scene));
	else
	{
		t = 0.5 * (scene->ray.dir.y + 1.0);
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
