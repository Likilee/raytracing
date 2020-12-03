# include "main.h"
# include "bool.h"
# include "degree.h"
# include "ray.h"
# include "hittable.h"
# include "hittable_list.h"
# include "sphere.h"
# include "vec3.h"
# include "color.h"
# include "random.h"
# include "rtweekend.h"
# include "camera.h"
# include "material.h"


t_color			random_color(double min, double max)
{
	return (color(random_double_(min,max),random_double_(min,max),random_double_(min,max)));
}


// 메모리 관리 한개도 안해줌!! 메모리 릭 무조건 발생하는 상황
t_hittable_list *random_scene(void)
{
    t_hittable_list *world = (t_hittable_list *)malloc(sizeof(t_hittable_list));
	world = NULL;
	t_material *ground_material = make_material(LAMBERTIAN, color(0.5,0.5,0.5), 0);

    h_lstadd_back(&world, h_lstnew(SP, new_sphere(point(0,-1000,0), 1000, ground_material)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            t_point3 center = point(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((vlength(vminus(center, point(4, 0.2, 0)))> 0.9))
			{
				t_material *sphere_material = (t_material *)malloc(sizeof(t_material));

                if (choose_mat < 0.8) {
                    // diffuse
                    t_color albedo = vmult_(random_color(0,1), random_color(0,1));
                    sphere_material = make_material(LAMBERTIAN, albedo, 0);
                    h_lstadd_back(&world, h_lstnew(SP, new_sphere(center, 0.2, sphere_material)));
                } else if (choose_mat < 0.95) {
                    // metal
                    t_color albedo = random_color(0.5, 1);
                    double fuzz = random_double_(0, 0.5);
                    sphere_material = make_material(METAL, albedo, fuzz);
                    h_lstadd_back(&world, h_lstnew(SP, new_sphere(center, 0.2, sphere_material)));
                } else {
                    // glass
                    sphere_material = make_material(DIELECTRIC, color(0,0,0), 1.5);
                    h_lstadd_back(&world, h_lstnew(SP, new_sphere(center, 0.2, sphere_material)));
                }
            }
        }
    }

    t_material *material1 = make_material(DIELECTRIC, color(0,0,0), 1.5);
    h_lstadd_back(&world, h_lstnew(SP, new_sphere(point(0, 1, 0), 1.0, material1)));

    t_material *material2 = make_material(LAMBERTIAN, color(0.4, 0.2, 0.1), 0);
    h_lstadd_back(&world, h_lstnew(SP, new_sphere(point(-4, 1, 0), 1.0, material2)));

    t_material *material3 = make_material(METAL, color(0.7,0.6,0.5), 0.0);
    h_lstadd_back(&world, h_lstnew(SP, new_sphere(point(4, 1, 0), 1.0, material3)));
    return (world);
}


int	main(void)
{
	//Image
	const double	aspect_ratio = 3.0 / 2.0;
	const int		image_width = 1200;
	const int		image_height = (int)(image_width / aspect_ratio);
	const int		samples_per_pixel = 200;
	const int		max_depth = 50;

	//World
	t_hittable_list *world;
	world = random_scene();

	//Camera
	t_camera	cam;
	t_point3	lookfrom = point(13,2,3);
	t_point3	lookat = point(0,0,0);
	t_vec3		vup = vec3(0,1,0);
	double		dist_to_focus = 10.0;
	double		aperture = 0.1; // 조리개 크기
	camera(&cam, lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	int				i;
	int				j;
	int				k;
	t_color			pixel_color;
	double			u;
	double			v;
	t_ray			r;

	ft_printf("P3\n%d  %d\n255\n", image_width, image_height);
	// Render
	j = image_height - 1;
	while (j >= 0)
	{
		ft_putstr_fd("\rScanlines remaining: ", 2);
		ft_putnbr_fd(j, 2);
		ft_putchar_fd('\n', 2);
		i = 0;
		while (i < image_width)
		{
			k = 0;
			pixel_color = color(0, 0, 0);
			while (k++ < samples_per_pixel)
			{
				u = (i + random_double()) / (image_width - 1);
				v = (j + random_double()) / (image_height -1);
				r = get_ray(u, v, &cam);
				// if (i == 20 && j == 58)
				// {
					pixel_color = vplus(pixel_color, ray_color(&r, world, max_depth));
					// dprintf(2, "p:%f,%f,%f r:%f,%f,%f\n",pixel_color.x,pixel_color.y,pixel_color.z, ray_color(&r, world, max_depth).x,ray_color(&r, world, max_depth).y,ray_color(&r, world, max_depth).z);
				// }
			}
			write_color(pixel_color, samples_per_pixel);
			++i;
		}
		--j;
	}
	ft_putstr_fd("Done.\n", 2);
}
