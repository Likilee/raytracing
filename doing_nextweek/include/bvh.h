#ifndef BVH_H
# define BVH_H

# include "hittable.h"
# include "random.h"
# define NODE 0
# define HITAB 1

typedef int	t_node_type;

typedef struct	s_bvh_node
{
	t_aabb				box;
	t_node_type 		l_type;
	t_node_type			r_type;
	void 				*left;
	void				*right;
}				t_bvh_node;

#endif