#include "hittable.h"

t_bool	hit_list(t_hittable_list *obj, t_ray *r, t_hit_record *rec)
{
	t_hit_record	temp_rec;
	t_bool			hit_anything;

	temp_rec.tmin = rec->tmin;
	temp_rec.tmax = rec->tmax;
	hit_anything = FALSE;
	while(obj)
	{
		if (hit(obj, r, &temp_rec))
		{
			hit_anything = TRUE;
			temp_rec.tmax = temp_rec.t;
			*rec = temp_rec;
		}
		obj = obj->next;
	}
	return (hit_anything);
}

t_bool	hit(t_hittable_list *obj, t_ray *r, t_hit_record *rec)
{
	t_bool	is_hit;

	is_hit = FALSE;
	if (obj->object_type == SP)
		is_hit = hit_sphere((t_sphere *)(obj->data), r, rec);
	// if (obj->object_type == PL)
	// if (obj->object_type == SQ)
	// if (obj->object_type == CY)
	// if (obj->object_type == TR)
	return (is_hit);
}

t_bool hit_sphere(t_sphere *sphere, t_ray *r, t_hit_record *rec)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	half_b;
	double	sqrtd;
	double	root;

	oc = vminus(r->orig, sphere->center);
	a = vlength2(r->dir);
	half_b = vdot(oc, r->dir);

	c = vlength2(oc) - sphere->radius2;
	discriminant = half_b * half_b - a * c; // 판별식
	if (discriminant < 0)
		return FALSE;
	sqrtd = sqrt(discriminant);
	root = (-half_b - sqrtd) / a;
	if (root < rec->tmin || root > rec->tmax)
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || root > rec->tmax)
			return FALSE;
	}
	rec->t = root;
	rec->p = at(r, rec->t);
	rec->normal = vdevide(vminus(rec->p, sphere->center), sphere->radius);
	set_face_normal(r, rec);
	rec->mat_ptr = sphere->mat_ptr;
	return (TRUE);
}

void	set_face_normal(t_ray *r, t_hit_record *rec)
{
	rec->front_face = vdot(r->dir, rec->normal) < 0;
	rec->normal = rec->front_face ? rec->normal : vmult(rec->normal, -1);
}

t_color	ray_color(t_ray *r, t_hittable_list *world, int depth)
{
	double			t;
	t_vec3			unit_dir;
	t_point3 		center;
	t_vec3			n;
	t_hit_record 	rec;
	t_point3		target;
	t_ray			r_current;

	if (depth <= 0)
		return (color(0, 0, 0));
	rec.tmax = INFINITY;
	rec.tmin = 0.001;
	if (hit_list(world, r, &rec))
	{
		t_ray	scattered;
		t_color	attenuation;
		if (scatter(r, &rec, &attenuation, &scattered))
			return (vmult_(ray_color(&scattered, world, depth - 1), attenuation));
		return color(0, 0, 0);
	}
	unit_dir = vunit(r->dir);
	t = 0.5 * (unit_dir.y + 1.0);
	return (vplus(vmult(color(1, 1, 1), 1.0 - t), vmult(color(0.5, 0.7, 1.0), t)));
}