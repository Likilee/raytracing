#include "main.h"

int	main(void)
{
	const int	image_width = 256;
	const int	image_height = 256;
	int			i;
	int			j;
	t_color		pixel_color;

	ft_printf("P3\n%d  %d\n255\n", image_width, image_height);
	j = image_width - 1;
	while (j >= 0)
	{
		ft_putstr_fd("\rScanlines remaining: ", 2);
		ft_putnbr_fd(j, 2);
		ft_putchar_fd('\n', 2);
		i = 0;
		while (i < image_width)
		{
			vset(&pixel_color, (double)i / (image_width - 1), (double)j / (image_height - 1), 0.25);
			write_color(pixel_color);
			++i;
		}
		--j;
	}
	ft_putstr_fd("Done.\n", 2);
}