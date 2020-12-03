#include "bvh.h"

t_bvh_node		bvh_node(t_node_type type, void *objects, size_t start, size_t end, double time0, double time1)
{
	int		axis = random_int(0,2);
	t_bool (*compare)(void *, void *);

	
}