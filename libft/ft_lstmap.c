/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migugar2 <migugar2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:17:41 by migugar2          #+#    #+#             */
/*   Updated: 2024/09/25 17:55:36 by migugar2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*n;

	if (f == NULL)
		return (NULL);
	new_lst = NULL;
	while (lst)
	{
		n = ft_lstnew(lst->content);
		if (n == NULL)
		{
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		n->content = f(n->content);
		ft_lstadd_back(&new_lst, n);
		lst = lst->next;
	}
	return (new_lst);
}
