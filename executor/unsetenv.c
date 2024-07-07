/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:25:27 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/06 20:11:47 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "executor.h"

void	ft_lstremove(t_list **list, int index)
{
	t_list	*tmp;
	t_list	*prev;
	int		i;

	i = 0;
	tmp = *list;
	prev = NULL;
	while (tmp && i < index)
	{
		prev = tmp;
		tmp = tmp->next;
		i++;
	}
	if (prev)
		prev->next = tmp->next;
	else
		*list = tmp->next;
	free(tmp->content);
	free(tmp);
}

int	ft_unsetenv(char *key, t_data *data)
{
	t_list	*tmp;
	int		i;

	tmp = data->env;
	i = 0;
	while (tmp)
	{
		if (!ft_strncmp(key, (char *)tmp->content, ft_strlen(key)) && \
		((char *)tmp->content)[ft_strlen(key)] == '=')
		{
			ft_lstremove(&data->env, i);
			return (0);
		}
		tmp = tmp->next; 
		i++;
	}
	return (0);
}