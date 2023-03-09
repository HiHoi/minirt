#include "vector.h"
#include "scene.h"
#include "trace.h"
#include "structures.h"
#include "./mlx/mlx.h"

int	goodbye(t_game *game)
{
	mlx_destroy_window(game->mlx, game->win);
	exit(0);
	return (0);
}

int	encode_rgb(int r, int g, int b)
{
	return (r << 16 | g << 8 | b);
}

double	clamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

void	write_color(t_color3 pixel_color)
{
	printf("%d %d %d\n", (int)(255.999 * pixel_color.x),\
	(int)(255.999 * pixel_color.y), (int)(255.999 * pixel_color.z));
}

t_scene	*scene_init(void)
{
	t_scene		*scene;
	t_object	*world;
	t_object	*lights;
	double		ka;

	if (!(scene = malloc(sizeof(t_scene))))
		return (NULL);
	scene->canvas = canvas(1200, 800);
	scene->camera = camera(&scene->canvas, point3(0, 0, 10));
	world = object(PLANE, plane(point3(0, -2, 0), vec3(0, 10, 0)), color3(0.1, 0.4, 0.2));
	//바닥
	// oadd(&world, object(PLANE, plane(point3(0, -2, -10), vec3(0, 0, 1)), color3(0.1, 0.4, 0.2)));
	//면
	//원기둥
	oadd(&world, object(CYLINDER, cylinder(point3(2, 0, -3), vec3(0, 1, -0.5), 1, 3), color3(0.5, 0, 0)));
	//구 1
	oadd(&world, object(SP, sphere(point3(0, 0, -5), 2), color3(0.6, 0.3, 0)));
	//구 2
	oadd(&world, object(SP, sphere(point3(0, 4, -5), 2), color3(0, 0.5, 0)));
	scene->world = world;
	lights = object(LIGHT_POINT, light_point(point3(-100, 100, 0), color3(1, 1, 1), 0.1), color3(0, 0, 0));
	scene->light = lights;
	ka = 0.11;
	scene->ambient = vmult(color3(1, 1, 1), ka);
	return (scene);
}

int	main(void)
{
	int		    i;
	int		    j;
	int			clr;
	int			depth;
	double		u;
	double		v;
	double		r;
	double		g;
	double		b;
	t_game		*game;
    t_color3    pixel_color;
	t_scene		*scene;

	game = malloc(sizeof(t_game));
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, 1200, 800, "Minirt");
	scene = scene_init();
	depth = 50;
	j = scene->canvas.height - 1;
	while (j >= 0)
	{
		i = 0;
		while (i < scene->canvas.width)
		{
			pixel_color.x = 0;
			pixel_color.y = 0;
			pixel_color.z = 0;
			for (int s = 0 ; s < 100; s++)
			{
				u = (double)(i + random_double_void()) / (scene->canvas.width - 1);
				v = (double)(j + random_double_void()) / (scene->canvas.height - 1);
				scene->ray = ray_primary(&scene->camera, u, v);
				pixel_color = vplus(pixel_color, ray_color(scene, depth));
				// write_color(pixel_color);
			}
			r = sqrtf(pixel_color.x * (1.0 / 100));
			g = sqrtf(pixel_color.y * (1.0 / 100));
			b = sqrtf(pixel_color.z * (1.0 / 100));
			clr = encode_rgb(256 * clamp(r, 0.0, 0.999), 256 * clamp(g, 0.0, 0.999), 256 * clamp(b, 0.0, 0.999));
			mlx_pixel_put(game->mlx, game->win, i, (scene->canvas.height - 1 - j), clr);
			i++;
		}
		j--;
	}
	mlx_hook(game->win, 17, 0, goodbye, game);
	mlx_loop(game->mlx);
	return (0);
}
