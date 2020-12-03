#include "main.h"


int	main(void)
{
	//Image
	const double	aspect_ratio = 16.0 / 9.0;
	const int		image_width = 400;
	const int		image_height = (int)(image_width / aspect_ratio);
	//Camera
	double			viewport_height = 2.0;
	double			viewport_width = aspect_ratio * viewport_height;
	double			focal_length = 1.0;

	t_point3		origin;
	t_vec3			horizontal;
	t_vec3			vertical;
	t_vec3			lower_left_corner;

	int				i;
	int				j;
	t_color			pixel_color;
	double			u;
	double			v;
	t_ray			r;

	vset(&origin, 0, 0, 0);
	vset(&horizontal, viewport_width, 0, 0);
	vset(&vertical, 0, viewport_height, 0);
	lower_left_corner = vminus_(vminus(vminus(origin, vdevide(horizontal, 2)), vdevide(vertical, 2)), 0, 0, focal_length);
	ft_printf("P3\n%d  %d\n255\n", image_width, image_height);
	j = image_height - 1;
	while (j >= 0)
	{
		ft_putstr_fd("\rScanlines remaining: ", 2);
		ft_putnbr_fd(j, 2);
		ft_putchar_fd('\n', 2);
		i = 0;
		while (i < image_width)
		{
			u = (double)i / (image_width - 1);
			v = (double)j / (image_height -1);
			r.orig = origin;
			r.dir = vminus(vplus(vplus(lower_left_corner, vmult(horizontal, u)),
										 vmult(vertical, v)), origin);
			pixel_color = ray_color(&r);
			write_color(pixel_color);
			++i;
		}
		--j;
	}
	ft_putstr_fd("Done.\n", 2);
}
