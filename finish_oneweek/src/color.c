#include "color.h"

t_color	color(double r, double g, double b)
{
	t_color color;

	color.x = r;
	color.y = g;
	color.z = b;
	return (color);
}

void	write_color(t_color pixel_color, int samples_per_pixel)
{
	double	r;
	double	g;
	double	b;
	double	scale;

	scale = 1.0 / samples_per_pixel;
	r = sqrt(pixel_color.x * scale);
	g = sqrt(pixel_color.y * scale);
	b = sqrt(pixel_color.z * scale);
	ft_printf("%d %d %d\n", (int)(256 * clamp(r, 0.0, 0.999)),
							(int)(256 * clamp(g, 0.0, 0.999)),
							(int)(256 * clamp(b, 0.0, 0.999)));
}
