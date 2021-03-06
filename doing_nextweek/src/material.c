#include "material.h"

t_bool	scatter(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered)
{
	t_bool	result;

	if (rec->mat_ptr->mat_kind == LAMBERTIAN)
		result = scatter_lambertian(r_in, rec, attenuation, scattered);
	else if (rec->mat_ptr->mat_kind == METAL)
		result = scatter_metal(r_in, rec, attenuation, scattered);
	else if (rec->mat_ptr->mat_kind == DIELECTRIC)
		result = scatter_dielectric(r_in, rec, attenuation, scattered);
	return (result);
}

t_bool	scatter_lambertian(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered)
{
	t_vec3 scatter_dir;

	scatter_dir = vplus(rec->normal, random_unit_vector());
	if (near_zero(&scatter_dir))
		scatter_dir = rec->normal;
	*scattered = ray(rec->p, scatter_dir, r_in->time);
	*attenuation = rec->mat_ptr->albedo;
	return (TRUE);
}

t_bool	scatter_metal(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered)
{
	t_vec3	reflected;

	// option1 == fuzz
	reflected = reflect(vunit(r_in->dir), rec->normal);
	*scattered = ray(rec->p, vplus(reflected, vmult(random_in_unit_sphere(), rec->mat_ptr->option1)), r_in->time);
	*attenuation = rec->mat_ptr->albedo;
	return (vdot(scattered->dir, rec->normal) > 0);
}

static double	reflectance(double cosine, double ref_idx)
{
	// Use Schlick's approximation for reflectance.
	double	r0;

	r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * pow((1 - cosine), 5));
}

t_bool	scatter_dielectric(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered)
{
	double	refraction_ratio;
	t_vec3	unit_dir;
	double	cos_theta;
	double	sin_theta;
	t_bool	cannot_refract;
	t_vec3	direction;

	// option1 == ir (Index of Refraction)
	*attenuation = color(1.0, 1.0, 1.0);
	refraction_ratio = (rec->front_face) ? (1.0 / rec->mat_ptr->option1) : rec->mat_ptr->option1;
	unit_dir = vunit(r_in->dir);
	cos_theta = fmin(vdot(vmult(unit_dir, -1), rec->normal), 1.0);
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	cannot_refract = refraction_ratio * sin_theta > 1.0;
	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
		direction = reflect(unit_dir, rec->normal);
	else
		direction = refract(&unit_dir, &rec->normal, refraction_ratio);
	*scattered = ray(rec->p, direction, r_in -> time);
	return (TRUE);
}


t_material	*make_material(t_material_kind kind, t_color albedo, double option1)
{
	t_material *mat;

	if(!(mat = (t_material *)malloc(sizeof(t_material))))
		exit(0);
	mat->mat_kind = kind;
	mat->albedo = albedo;
	if (kind == METAL)
	{
		if (option1 > 1)
			option1 = 1.0;
		mat->option1 = option1;
	}
	if (kind == DIELECTRIC)
		mat->option1 = option1;
	return (mat);
}
