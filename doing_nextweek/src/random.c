#include "random.h"

double		random_double(void)
{
	return (rand() / (RAND_MAX + 1.0));
}

double		random_double_(double min, double max)
{
	return (min + (max - min) * random_double());
}
