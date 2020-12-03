#include "sphere.h"

t_sphere	*new_sphere(t_point3 center, double radius, t_material *mat_ptr)
{
	t_sphere *sphere;

	if(!(sphere = (t_sphere *)malloc(sizeof(t_sphere))))
		exit(0);
	sphere->center = center;
	sphere->radius = radius;
	sphere->radius2 = radius * radius;
	sphere->mat_ptr = mat_ptr;
	return (sphere);
}