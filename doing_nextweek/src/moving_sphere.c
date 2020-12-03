#include "moving_sphere.h"

t_moving_sphere	*moving_sphere(t_point3 center0, t_point3 center1, double time0, double time1, double radius, t_material *mat_ptr)
{
	t_moving_sphere *sphere;

	if(!(sphere = (t_moving_sphere *)malloc(sizeof(t_moving_sphere))))
		exit(0);
	sphere->center0 = center0;
	sphere->center1 = center1;
	sphere->time0 = time0;
	sphere->time1 = time1;
	sphere->radius = radius;
	sphere->radius2 = radius * radius;
	sphere->mat_ptr = mat_ptr;
	return (sphere);
}

t_point3	center(t_moving_sphere *sp, double time)
{
	return (vplus(sp->center0, vmult(vminus(sp->center1, sp->center0), (time - sp->time0) / (sp->time1 - sp->time0))));
}
