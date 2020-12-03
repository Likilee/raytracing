#ifndef HITTABLE_H
# define HITTABLE_H

# include "ray.h"
# include "vec3.h"
# include "bool.h"
# include "sphere.h"
# include "material.h"

/*
** 추상화! > 구말고 다른 객체들이 생겨도 함수자체를 수정하지 않아도 되게.
** 그러려면
** 1. 교점 / 법선 / 매개변수 t / 법선과 광선의 방향(front or back) 일치 여부/를 데이터로 가지고있어야한다.
** 2. 서로 다른 형태의 오브젝트를 하나의 리스트로 관리해야한다.
** 3. 광선과 오브젝트 리스트의 요소들 중 교점이 존재하는지. 존재한다면 가장 먼저 (t가 가장 작은) 만난 오브젝트는 누구인지를 판단해서
** hit_record 에 기록한다.
** 4. 리스트 요소는 자신의 오브젝트 타입을 식별자로 가진다.
*/

# define SP 0
# define PL 1
# define SQ 2
# define CY 3
# define TR 4

typedef int		t_object;

typedef	struct	s_material t_material;
typedef struct	s_hit_record
{
	double		tmin;
	double		tmax;
	t_point3	p;
	t_vec3		normal;
	double		t;
	t_bool		front_face;
	t_material	*mat_ptr;
}				t_hit_record;

typedef struct	s_hittable_list t_hittable_list;

struct	s_hittable_list
{
	t_object	object_type;
	void 		*data;
	void 		*next;
};

void	recdup(t_hit_record *dst, t_hit_record *src);
t_bool	hit_list(t_hittable_list *obj, t_ray *r, t_hit_record *rec);
t_bool	hit(t_hittable_list *obj, t_ray *r, t_hit_record *rec);
void	set_face_normal(t_ray *r, t_hit_record *rec);
t_bool	hit_sphere(t_sphere *sphere, t_ray *r, t_hit_record *rec);
t_color	ray_color(t_ray *r, t_hittable_list *world, int depth);

#endif