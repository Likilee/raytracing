#include "camera.h"

void	camera(t_camera *c, t_point3 lookfrom, t_point3 lookat, t_vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
{
	double theta;
	double h;
	double viewport_height;
	double viewport_width;
	double focal_length;

	theta = deg_to_rad(vfov);
	h = tan(theta/2);
	viewport_height = 2.0 * h;
	viewport_width = aspect_ratio * viewport_height;
	focal_length = 1.0;

	c->w = vunit(vminus(lookfrom, lookat));
	c->u = vunit(vcross(vup, c->w));
	c->v = vcross(c->w, c->u);
	c->origin = lookfrom;
	c->horizontal = vmult(vmult(c->u ,viewport_width), focus_dist);
	c->vertical = vmult(vmult(c->v, viewport_height), focus_dist);
	c->lower_left_corner = vminus(vminus(vminus(c->origin, vdevide(c->horizontal, 2)), vdevide(c->vertical, 2)), vmult(c->w, focus_dist));
	c->lens_radius = aperture / 2;
};

t_vec3	random_in_unit_disk(void)
{
	t_vec3	p;

	while (TRUE)
	{
		p = vec3(random_double_(-1,1), random_double_(-1,1), 0);
		if (vlength2(p) < 1)
			return (p);
	}
}

t_ray	get_ray(double s, double t, t_camera *c)
{
	t_ray r;
	t_vec3 rd;
	t_vec3 offset;

	rd = vmult(random_in_unit_disk(), c->lens_radius);
	offset = vplus(vmult(c->u, rd.x), vmult(c->v, rd.y));
	r.orig = vplus(c->origin, offset);
	r.dir = vminus(vminus(vplus(vplus(c->lower_left_corner, vmult(c->horizontal, s)),
										 vmult(c->vertical, t)), c->origin), offset);
	return (r);
}