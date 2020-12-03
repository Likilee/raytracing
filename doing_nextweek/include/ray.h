#ifndef RAY_H
# define RAY_H

# include "vec3.h"
# include "color.h"

typedef struct	s_ray
{
	t_point3	orig;
	t_vec3		dir;
}				t_ray;

t_point3 at(t_ray *ray, double t);
t_ray	ray(t_point3 orig, t_vec3 dir);

#endif
