#include "ray.h"

t_point3 at(t_ray *ray, double t)
{
	t_point3 at;

	at = vplus(ray->orig, vmult(ray->dir, t));
	return (at);
}

t_ray	ray(t_point3 orig, t_vec3 dir)
{
	t_ray new;

	new.orig = orig;
	new.dir = dir;
	return (new);
}
