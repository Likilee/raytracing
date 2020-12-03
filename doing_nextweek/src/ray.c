#include "ray.h"

t_point3 at(t_ray *ray, double t)
{
	t_point3 at;

	at = vplus(ray->orig, vmult(ray->dir, t));
	return (at);
}

t_ray	ray(t_point3 orig, t_vec3 dir, double time)
{
	t_ray ray;

	ray.orig = orig;
	ray.dir = dir;
	ray.time = time;
	return (ray);
}
