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

void    write_color(t_color3 pixel_color)
{
    printf("%d %d %d\n", (int)(255.999 * pixel_color.x),\
                        (int)(255.999 * pixel_color.y),\
                        (int)(255.999 * pixel_color.z));
}

t_scene	*scene_init(void)
{
	t_scene		*scene;
	t_object	*world;
	t_object	*lights;
	double		ka;

	if (!(scene = malloc(sizeof(t_scene))))
		return (NULL);
	scene->canvas = canvas(400, 300);
	scene->camera = camera(&scene->canvas, point3(0, 0, 0));
	world = object(SP, sphere(point3(-2, 0, -5), 2), color3(0.5, 0, 0));
	oadd(&world, object(SP, sphere(point3(0, -1010, 0), 995), color3(1, 1, 1)));
	oadd(&world, object(SP, sphere(point3(2, 0, -5), 2), color3(0, 0.5, 0)));
	scene->world = world;
	lights = object(LIGHT_POINT, light_point(point3(0, 5, 0), color3(1, 1, 1), 0.5), color3(0, 0, 0));
	scene->light = lights;
	ka = 0.2;
	scene->ambient = vmult(color3(1, 1, 1), ka);
	return (scene);
}

int	main(void)
{
	int		    i;
	int		    j;
	int			clr;
	double		u;
	double		v;
	t_game		*game;
    t_color3    pixel_color;
	t_scene		*scene;

	game = malloc(sizeof(t_game));
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, 400, 300, "Minirt");
	scene = scene_init();
	j = scene->canvas.height - 1;
	while (j >= 0)
	{
		i = 0;
		while (i < scene->canvas.width)
		{
			u = (double)i / (scene->canvas.width - 1);
			v = (double)j / (scene->canvas.height - 1);
			scene->ray = ray_primary(&scene->camera, u, v);
			pixel_color = ray_color(scene);
			// write_color(pixel_color);
			clr = encode_rgb(255.999 * pixel_color.x, 255.999 * pixel_color.y, 255.999 * pixel_color.z);
			mlx_pixel_put(game->mlx, game->win, i, (scene->canvas.height - 1 - j), clr);
			i++;
		}
		j--;
	}
	mlx_hook(game->win, 17, 0, goodbye, game);
	mlx_loop(game->mlx);
	return (0);
}
