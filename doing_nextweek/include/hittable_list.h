#ifndef HITTABLE_LIST_H
# define HITTABLE_LIST_H

# include "hittable.h"

t_hittable_list	*h_lstnew(int object_type, void *obj);
t_hittable_list	*h_lstlast(t_hittable_list *lst);
void	h_lstadd_back(t_hittable_list **lst, t_hittable_list *new);
int		h_lstsize(t_hittable_list *lst);
void	h_lstdelone(t_hittable_list *obj);
void	h_lstclear(t_hittable_list **lst);

#endif