#ifndef SPHERE_H
# define SPHERE_H

# include "bool.h"
# include "vec3.h"

typedef	struct	s_material t_material;

typedef struct	s_sphere
{
	t_point3	center;
	double		radius;
	double		radius2;
	t_material	*mat_ptr;
}				t_sphere;

t_sphere	*new_sphere(t_point3 center, double radius, t_material *mat_ptr);

#endif
