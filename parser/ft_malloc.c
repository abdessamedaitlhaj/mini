/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 17:51:32 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/10 00:42:50 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back(t_list **list, t_list *new)
{
	t_list	*last;

	if (!*list)
	{
		*list = new;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new;
}

t_list	*init_malloc(void)
{
	t_list	*garbage_collector;

	garbage_collector = NULL;
	return (garbage_collector);
}

void	free_allocated(t_list **garbage_collector)
{
	t_list	*tmp;

	while (*garbage_collector)
	{
		tmp = (*garbage_collector)->next;
		free((*garbage_collector)->content);
		free(*garbage_collector);
		*garbage_collector = tmp;
	}
}

void	*ft_malloc(size_t size, t_list **garbage_collector)
{
	void	*ptr;
	t_list	*new_node;

	ptr = malloc(size);
	if (!ptr)
	{
		free_allocated(garbage_collector);
		return (NULL);
	}
	new_node = ft_lstnew(ptr);
	if (!new_node)
	{
		free(ptr);
		free_allocated(garbage_collector);
		return (NULL);
	}
	ft_lstadd_back(garbage_collector, new_node);
	return (ptr);
}
