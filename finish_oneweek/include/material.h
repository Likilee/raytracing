#ifndef MATERIAL_H
# define MATERIAL_H

# include "bool.h"
# include "ray.h"
# include "vec3.h"
# include "hittable.h"

# define LAMBERTIAN 0
# define METAL 1
# define DIELECTRIC 2

typedef struct s_hit_record t_hit_record;
typedef int	t_material_kind;

#
// 오브젝트가 어떤 메테리얼 속성인지 식별해야함. -> 메크로로 구현해볼까?
// 속성에 따라 산란(scatter)을 다르게 적용할 수 있어야한다.
// scatter 함수가 오브젝트의 메테리얼 속성에 따라 자식 함수로 일나눠주기

// typedef struct	s_hit_record t_hit_record;

// typedef int		t_bool;

// typedef	struct	s_vec3 t_color;

// typedef struct	s_ray t_ray;

typedef	struct	s_material
{
	t_material_kind	mat_kind;
	t_color			albedo;
	double			option1;
}				t_material;

t_bool	scatter(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered);
t_bool	scatter_lambertian(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered);
t_bool	scatter_metal(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered);
t_material	*make_material(t_material_kind kind, t_color albedo, double option1);
t_bool	scatter_dielectric(t_ray *r_in, t_hit_record *rec, t_color *attenuation, t_ray * scattered);

#endif