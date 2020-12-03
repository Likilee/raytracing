#ifndef CAMERA_H
# define CAMERA_H

#include "ray.h"
#include "degree.h"

typedef struct	s_camera
{
	t_point3		origin;
	t_vec3			horizontal;
	t_vec3			vertical;
	t_vec3			lower_left_corner;
	t_ray			r;
	t_vec3 			w;
	t_vec3 			u;
	t_vec3 			v;
	double			lens_radius;
	double			time0; // 셔터 오픈 / 클로즈 타임
	double			time1;
}				t_camera;

void	camera(t_camera *c, t_point3 lookfrom, t_point3 lookat, t_vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist, double time0, double time1);
t_ray	get_ray(double u, double v, t_camera *c);

#endif