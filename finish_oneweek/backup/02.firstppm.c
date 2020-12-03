#include "main.h"

int	main(void)
{
	const int	image_width = 256;
	const int	image_height = 256;
	int			i;
	int			j;
	double		r;
	double		g;
	double		b;
	int			ir;
	int			ig;
	int			ib;

	printf("P3\n%d  %d\n255\n", image_width, image_height);
	j = image_width - 1;
	while (j >= 0)
	{
		ft_putstr_fd("\rScanlines remaining: ", 2);
		ft_putnbr_fd(j, 2);
		ft_putchar_fd('\n', 2);
		i = 0;
		while (i < image_width)
		{
			r = (double)i / (image_width - 1);
			g = (double)j / (image_height - 1);
			b = 0.25;
			// 255.999 로 하는 이유, 0.0 ~ 1.0 에 0 ~ 255 라는 정수를 구간별로 집어 넣어야 하니까. 정수 타입캐스팅할 때 소수 버려지는걸 이용해서 이렇게!!
			ir = (int)(255.999 * r);
			ig = (int)(255.999 * g);
			ib = (int)(255.999 * b);
			printf("%d %d %d\n",ir,ig,ib);
			++i;
		}
		--j;
	}
	ft_putstr_fd("Done.\n", 2);
}