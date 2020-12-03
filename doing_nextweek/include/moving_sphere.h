#ifndef MOVING_SPHERE_H
# define MOVING_SPHERE_H

# include "bool.h"
# include "vec3.h"

typedef	struct	s_material t_material;

typedef struct	s_moving_sphere
{
	t_point3	center0;
	t_point3	center1;
	double		time0;
	double		time1;
	double		radius;
	double		radius2;
	t_material	*mat_ptr;
}				t_moving_sphere;

t_moving_sphere	*moving_sphere(t_point3 center0, t_point3 center1, double time0, double time1, double radius, t_material *mat_ptr);
t_point3	center(t_moving_sphere *sp, double time);

#endif