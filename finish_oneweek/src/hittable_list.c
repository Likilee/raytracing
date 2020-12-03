#include "hittable_list.h"

t_hittable_list	*h_lstnew(int object_type, void *obj)
{
	t_hittable_list	*new;

	if (!(new = (t_hittable_list *)malloc(sizeof(t_hittable_list) * 1)))
		return (NULL);
	new->data = obj;
	new->object_type = object_type;
	new->next = NULL;
	return (new);
}

t_hittable_list	*h_lstlast(t_hittable_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	h_lstadd_back(t_hittable_list **lst, t_hittable_list *new)
{
	t_hittable_list	*cur;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

int		h_lstsize(t_hittable_list *lst)
{
	int		count;

	count = 0;
	while (lst)
	{
		++count;
		lst = lst->next;
	}
	return (count);
}

void	h_lstdelone(t_hittable_list *obj)
{
	if (obj == NULL)
		return ;
	free(obj);
	obj = NULL;
}

void	h_lstclear(t_hittable_list **lst)
{
	t_hittable_list	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		h_lstdelone(*lst);
		(*lst) = temp;
	}
}
