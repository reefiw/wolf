/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 16:13:11 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/07 17:19:58 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *first;
	t_list *temp;

	if (!lst)
		return (0);
	first = ft_lstnew(f(lst->content));
	if (!first->content)
	{
		ft_lstdelone(first, del);
		return (0);
	}
	temp = first;
	lst = lst->next;
	while (lst)
	{
		temp->next = ft_lstnew(f(lst->content));
		if (!temp->next || !temp->next->content)
		{
			ft_lstclear(&first, del);
			return (0);
		}
		temp = temp->next;
		lst = lst->next;
	}
	return (first);
}
