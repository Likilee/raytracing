#ifndef COLOR_H
# define COLOR_H

# include "vec3.h"
# include "rtweekend.h"
# include "../src/libft/include/libftprintf.h"

void	write_color(t_color pixel_color, int samples_per_pixel);
t_color	color(double r, double g, double b);

#endif