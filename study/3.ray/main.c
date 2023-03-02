#include "vector.h"
#include "scene.h"
#include "trace.h"
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
    printf("%d %d %d\n", (int)(255.999 * pixel_color.x),
                        (int)(255.999 * pixel_color.y),
                        (int)(255.999 * pixel_color.z));
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
	t_canvas	canv;
	t_camera	cam;
	t_ray		ray;
	t_sphere	sp;

	game = malloc(sizeof(t_game));
	game->mlx = mlx_init();
	game->win = mlx_new_window(game->mlx, 400, 300, "Minirt");
	canv = canvas(400, 300);
	cam = camera(&canv, point3(0, 0, 0));
	sp = sphere(point3(0, 0, -5), 2);
	j = canv.height - 1;
	while (j >= 0)
	{
		i = 0;
		while (i < canv.width)
		{
			u = (double)i / (canv.width - 1);
			v = (double)j / (canv.height - 1);
			ray = ray_primary(&cam, u, v);
			pixel_color = ray_color(&ray, &sp);
			// write_color(pixel_color);
			clr = encode_rgb(255.999 * pixel_color.x, 255.999 * pixel_color.y, 255.999 * pixel_color.z);
			mlx_pixel_put(game->mlx, game->win, i, j, clr);
			i++;
		}
		j--;
	}
	mlx_hook(game->win, 17, 0, goodbye, game);
	mlx_loop(game->mlx);
	return (0);
}
