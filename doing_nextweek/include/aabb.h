#ifndef AABB_H
# define AABB_H

#include "vec3.h"

typedef struct	s_aabb
{
	t_point3	min;
	t_point3	max;
}				t_aabb;

t_aabb	aabb(t_point3 a, t_point3 b);
t_aabb	surrounding_box(t_aabb box0, t_aabb box1);

#endif