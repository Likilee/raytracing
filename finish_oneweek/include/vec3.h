#ifndef VEC3_H
# define VEC3_H

#include <math.h>
#include <stdlib.h>
#include "random.h"
#include "bool.h"

typedef struct	s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef	t_vec3 t_point3;
typedef	t_vec3 t_color;


t_vec3	*new_vec(void);
t_vec3	vec3(double x, double y, double z);
t_point3 point(double x, double y, double z);
t_vec3	vset(t_vec3 *vec, double x, double y, double z);
double	vlength(t_vec3 vec);
double	vlength2(t_vec3 vec);
t_vec3	vnormalize(t_vec3 vec);
t_vec3	vplus(t_vec3 vec, t_vec3 vec2);
t_vec3	vplus_(t_vec3 vec, double x, double y, double z);
t_vec3	vminus(t_vec3 vec, t_vec3 vec2);
t_vec3	vminus_(t_vec3 vec, double x, double y, double z);
t_vec3	vmult(t_vec3 vec, double t);
t_vec3	vmult_(t_vec3 vec, t_vec3 vec2);
t_vec3	vdevide(t_vec3 vec, double t);
double	vdot(t_vec3 vec, t_vec3 vec2);
t_vec3	vcross(t_vec3 vec, t_vec3 vec2);
t_vec3	vunit(t_vec3 vec);
void	del_vec(t_vec3 *vec);
t_vec3	random_in_unit_sphere();
t_vec3	random_vec_(double min, double max);
t_vec3		random_unit_vector(void);
t_vec3 		random_in_hemisphere(t_vec3 *normal);
t_bool	near_zero(t_vec3 *v);
t_vec3	reflect(t_vec3 v, t_vec3 n);
t_vec3	refract(t_vec3 *uv, t_vec3 *n, double etai_over_etat);

#endif