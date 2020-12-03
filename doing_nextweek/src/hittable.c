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
	if (obj->object_type == MOV_SP)
		is_hit = hit_moving_sphere((t_moving_sphere *)(obj->data), r, rec);
	// if (obj->object_type == TR)
	return (is_hit);
}

static t_bool hit_sphere(t_sphere *sphere, t_ray *r, t_hit_record *rec)
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

static t_bool hit_moving_sphere(t_moving_sphere *sphere, t_ray *r, t_hit_record *rec)
{
	t_vec3	oc;
	t_point3 cent;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	half_b;
	double	sqrtd;
	double	root;

	cent = center(sphere, r->time);
	oc = vminus(r->orig, cent);
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
	rec->normal = vdevide(vminus(rec->p, cent), sphere->radius);
	set_face_normal(r, rec);
	rec->mat_ptr = sphere->mat_ptr;
	return (TRUE);
}

static t_bool hit_bvh_node(t_bvh_node *node, t_ray *r, t_hit_record *rec)
{
	t_bool hit_left;
	t_bool hit_right;

	if (hit_aabb(&node->box, r, rec) == FALSE)
		return (FALSE);
	if (node->l_type == NODE)
		hit_left = hit_bvh_node((t_bvh_node *)node->left, r, rec);
	else
		hit_left = hit_list((t_hittable_list *)node->left, r, rec);
	if (hit_left == TRUE)
		rec->tmax = rec->t;
	if (node->r_type == NODE)
		hit_right = hit_bvh_node((t_bvh_node *)node->right, r, rec);
	else
		hit_right = hit_list((t_hittable_list *)node->right, r, rec);
	return (hit_left || hit_right);
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

static void	swap_d(double *a, double *b)
{
	double temp;

	temp = *a;
	*a = *b;
	*b = *a;
}

static t_bool	hit_aabb_xyz(double min, double max, t_ray *r, double *t_min, double *t_max)
{
	double	t0;
	double	t1;
	double	invD;

	invD = 1.0f / r->dir.x;
	t0 = (min - r->orig.x) * invD;
	t1 = (max - r->orig.x) * invD;
	if (invD < 0.0f)
		swap_d(&t0, &t1);
	*t_min = t0 > *t_min ? t0 : *t_min;
	*t_max = t1 < *t_max ? t1 : *t_max;
	if (*t_max <= *t_min)
		return FALSE;
	return TRUE;
}

t_bool	hit_aabb(t_aabb *aabb, t_ray *r, t_hit_record *rec)
{
	t_bool result;

	result = hit_aabb_xyz(aabb->min.x, aabb->max.x, r, &rec->tmin, &rec->tmax);
	if (result == FALSE)
		return (FALSE);
	result = hit_aabb_xyz(aabb->min.y, aabb->max.y, r, &rec->tmin, &rec->tmax);
	if (result == FALSE)
		return (FALSE);
	result = hit_aabb_xyz(aabb->min.z, aabb->max.z, r, &rec->tmin, &rec->tmax);
	if (result == FALSE)
		return (FALSE);
	return (TRUE);
}

t_bool	bounding_box_h_list(t_hittable_list *obj, double time0, double time1, t_aabb *output_box)
{
	t_aabb	temp_box;
	t_bool	first_box;

	if (obj == NULL)
		return (FALSE);
	first_box = TRUE;
	while (obj)
	{
		if (bounding_box(obj, time0, time1, &temp_box) == FALSE)
			return (FALSE);
		*output_box = first_box ? temp_box : surrounding_box(*output_box, temp_box);
		first_box = FALSE;
		obj = obj->next;
	}
	return (TRUE);
}

t_bool	bounding_box(t_hittable_list *obj, double time0, double time1, t_aabb *output_box)
{
	t_bool	have_aabb;

	have_aabb = FALSE;
	if (obj->object_type == SP)
		have_aabb = bounding_box_sphere((t_sphere *)(obj->data), time0, time1, output_box);
	if (obj->object_type == MOV_SP)
		have_aabb = bounding_box_moving_sphere((t_moving_sphere *)(obj->data), time0, time1, output_box);
	// if (obj->object_type == SQ)
	// if (obj->object_type == CY)
	// if (obj->object_type == TR)
	return (TRUE);
}

static t_bool	bounding_box_sphere(t_sphere *sp, double time0, double time1, t_aabb *output_box)
{
	*output_box = aabb(vminus(sp->center, vec3(sp->radius, sp->radius, sp->radius)),
						vplus(sp->center, vec3(sp->radius, sp->radius, sp->radius)));
	return (TRUE);
}

static t_bool	bounding_box_moving_sphere(t_moving_sphere *sp, double time0, double time1, t_aabb *output_box)
{
	t_aabb box0;
	t_aabb box1;

	box0 = aabb(vminus(center(sp, time0), vec3(sp->radius, sp->radius, sp->radius)),
				vplus(center(sp, time0), vec3(sp->radius, sp->radius, sp->radius)));
	box1 = aabb(vminus(center(sp, time1), vec3(sp->radius, sp->radius, sp->radius)),
			vplus(center(sp, time1), vec3(sp->radius, sp->radius, sp->radius)));
	*output_box = surrounding_box(box0, box1);
	return (TRUE);
}

t_bool	bounding_box_bvh(t_bvh_node *node, double time0, double time1, t_aabb *output_box)
{
	*output_box = node->box;
	return (TRUE);
}
