#include "aabb.h"

t_aabb	aabb(t_point3 a, t_point3 b)
{
	t_aabb	aabb;

	aabb.min = a;
	aabb.max = b;
	return (aabb);
}

t_aabb	surrounding_box(t_aabb box0, t_aabb box1)
{
	t_point3 small;
	t_point3 big;

	small = point(fmin(box0.min.x, box1.min.x),
					fmin(box0.min.y, box1.min.y),
					fmin(box0.min.z, box1.min.z));
	big = point(fmin(box0.max.x, box1.max.x),
				fmin(box0.max.y, box1.max.y),
				fmin(box0.max.z, box1.max.z));
	return (aabb(small, big));
}