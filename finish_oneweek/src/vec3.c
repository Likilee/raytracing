#include "vec3.h"

//벡터 생성 및 값 초기화
t_vec3	*new_vec(void)
{
	t_vec3 *new;

	if(!(new = (t_vec3 *)malloc(sizeof(t_vec3))))
		return (NULL);
	new->x = 0;
	new->y = 0;
	new->z = 0;
	return (new);
}

t_vec3	vec3(double x, double y, double z)
{
	t_vec3 vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	return (vec);
}

t_point3 point(double x, double y, double z)
{
	t_point3 point;

	point.x = x;
	point.y = y;
	point.z = z;
	return (point);
}

// 벡터 값 설정
t_vec3	vset(t_vec3 *vec, double x, double y, double z)
{
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return (*vec);
}

double	vlength(t_vec3 vec)
{
	return (sqrt(vlength2(vec)));
}

double	vlength2(t_vec3 vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

// 벡터 정규화
t_vec3	vnormalize(t_vec3 vec)
{
	double	nor2;
	double	inv_nor;

	nor2 = vlength2(vec);
	if (nor2 > 0)
	{
		inv_nor = 1 / sqrt(nor2);
		vec.x *= inv_nor;
		vec.y *= inv_nor;
		vec.z *= inv_nor;
	}
	return (vec);
}

// 벡터합
t_vec3	vplus(t_vec3 vec, t_vec3 vec2)
{
	vec.x += vec2.x;
	vec.y += vec2.y;
	vec.z += vec2.z;
	return (vec);
}

// 벡터합2
t_vec3	vplus_(t_vec3 vec, double x, double y, double z)
{
	vec.x += x;
	vec.y += y;
	vec.z += z;
	return (vec);
}

// 벡터차
t_vec3	vminus(t_vec3 vec, t_vec3 vec2)
{
	vec.x -= vec2.x;
	vec.y -= vec2.y;
	vec.z -= vec2.z;
	return (vec);
}

t_vec3	vminus_(t_vec3 vec, double x, double y, double z)
{
	vec.x -= x;
	vec.y -= y;
	vec.z -= z;
	return (vec);
}

// 스칼라 곱

t_vec3	vmult(t_vec3 vec, double t)
{
	vec.x *= t;
	vec.y *= t;
	vec.z *= t;
	return (vec);
}

// 축 값 곲
t_vec3	vmult_(t_vec3 vec, t_vec3 vec2)
{
	vec.x *= vec2.x;
	vec.y *= vec2.y;
	vec.z *= vec2.z;
	return (vec);
}
// 벡터 스칼라 나누기
t_vec3	vdevide(t_vec3 vec, double t)
{
	vec.x *= 1 / t;
	vec.y *= 1 / t;
	vec.z *= 1 / t;

	return vec;
}

// 벡터 내적
double	vdot(t_vec3 vec, t_vec3 vec2)
{
	return (vec.x * vec2.x + vec.y * vec2.y + vec.z * vec2.z);
}

t_vec3	vcross(t_vec3 vec, t_vec3 vec2)
{
	t_vec3 new;

	new.x = vec.y * vec2.z - vec.z * vec2.y;
	new.y = vec.z * vec2.x - vec.x * vec2.z;
	new.z = vec.x * vec2.y - vec.y * vec2.x;
	return (new);
}

t_vec3	vunit(t_vec3 vec)
{
	double len = vlength(vec);

	vec.x /= len;
	vec.y /= len;
	vec.z /= len;
	return (vec);
}

void	del_vec(t_vec3 *vec)
{
	if (vec != NULL)
		free(vec);
}

t_vec3		random_vec(void)
{
	return (vec3(random_double(), random_double(), random_double()));
}

t_vec3		random_vec_(double min, double max)
{
	return (vec3(random_double_(min, max), random_double_(min, max), random_double_(min, max)));
}

t_vec3		random_in_unit_sphere()
{
	t_vec3	p;

	while(1)
	{
		p = random_vec_(-1, 1);
		if (vlength2(p) < 1)
			return (p);
	}
}

t_vec3		random_unit_vector(void)
{
	double	a;
	double	z;
	double	r;

	a = random_double_(0, 2 * M_PI);
	z = random_double_(-1, 1);
	r = sqrt(1 - z * z);
	return (vec3(r * cos(a), r * sin(a), z));
}

t_vec3 		random_in_hemisphere(t_vec3 *normal)
{
	t_vec3	in_unit_sphere;

	in_unit_sphere = random_in_unit_sphere();
	if (vdot(in_unit_sphere, *normal) > 0.0) // 법선과 같은 반구에 있으면
		return (in_unit_sphere);
	else
		return (vmult(in_unit_sphere, -1));
}

t_bool	near_zero(t_vec3 *v)
{
	// 벡타가 모든 차원으로 0에 근접하면 트루 리턴
	double s = 1e-8;
	if ((v->x < s) && (v->y < s) && (v->z < s))
		return (TRUE);
	else
		return (FALSE);
}

t_vec3	reflect(t_vec3 v, t_vec3 n)
{
	//v - 2 * dot(v, n) * n;
	return (vminus(v, vmult(n ,vdot(v, n) * 2)));
}

//굴절된 방향벡터 돌려주기 함수
//쎄타가 일정 범위 이상 커질 때 문제가 생기는 듯!
// t_vec3	refract(t_vec3 *uv, t_vec3 *n, double etai_over_etat)
// {
// 	double	cos_theta;
// 	t_vec3	r_out_perp;
// 	t_vec3	r_out_parallel;

// 	dprintf(2, "들어온 광선 R:%f, %f, %f\n",uv->x, uv->y, uv->z);
// 	cos_theta = fmin(vdot(vmult(*uv, -1), *n), 1.0);
// 	dprintf(2, "cos_theta:%f\n", cos_theta);
// 	dprintf(2, "eta_o_eta:%f\n", etai_over_etat);
// 	dprintf(2, "n:%f, %f, %f\n", etai_over_etat);
// 	dprintf(2, "vmult(*n, cos_theta):%f, %f, %f\n", vmult(*n, cos_theta).x, vmult(*n, cos_theta).y, vmult(*n, cos_theta).z);
// 	r_out_perp = vmult(vplus(*uv, vmult(*n, cos_theta)), etai_over_etat); // 𝐑′⊥
// 	r_out_parallel = vmult(*n, -sqrt(fabs(1.0 - vlength2(r_out_perp)))); // 𝐑′∥
// 	dprintf(2, "-법선과 r' 내적값_cos_theta`:%f\n", vdot(vmult(*n,-1), vplus(r_out_perp, r_out_parallel)));
// 	dprintf(2, "-법선과 r' 수직 내적값:%f\n", vdot(vmult(*n,-1), r_out_perp));
// 	dprintf(2, "refracted의 길이:%f\n", vlength(vplus(r_out_perp, r_out_parallel)));

// 	return (vplus(r_out_perp, r_out_parallel));
// }

t_vec3	refract(t_vec3 *uv, t_vec3 *n, double etai_over_etat)
{
	double	cos_theta;
	t_vec3	r_out_perp;
	t_vec3	r_out_parallel;

	// r_out_perp = etai_over_etat * (uv + cos_theta*n);
	// r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	cos_theta = fmin(vdot(vmult(*uv, -1), *n), 1.0);
	r_out_perp = vmult(vplus(*uv, vmult(*n, cos_theta)), etai_over_etat); // 𝐑′⊥
	r_out_parallel = vmult(*n, -sqrt(fabs(1.0 - vlength2(r_out_perp)))); // 𝐑′∥
	return (vplus(r_out_perp, r_out_parallel));
}