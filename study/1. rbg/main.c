/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hoslim <hoslim@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/02 15:35:50 by hoslim            #+#    #+#             */
/*   Updated: 2023/03/02 15:47:06 by hoslim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	main(void)
{
	int		i;
	int		j;
	double	r;
	double	g;
	double	b;
	int		canvas_width;
	int		canvas_height;

	canvas_width = 256;
	canvas_height = 256;

	printf("P3\n%d %d\n255\n", canvas_width, canvas_height);
	j = canvas_height - 1;
	while (j >= 0)
	{
		i = 0;
		while (i < canvas_width)
		{
			r = (double)i / (canvas_width - 1);
			g = (double)j / (canvas_height - 1);
			b = 0.25;
			printf("%d %d %d\n", (int)(255.999 * r), (int)(255.999 * g), (int)(255.999 * b));
			i++;
		}
		j--;
	}
	return (0);
}
