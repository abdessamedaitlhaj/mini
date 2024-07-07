/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_built.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 09:00:36 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/07 09:46:53 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(char *key, t_list *env)
{
	t_list	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		if (!ft_strncmp(key, (char *)tmp->content, ft_strlen(key)) && \
		((char *)tmp->content)[ft_strlen(key)] == '=')
			return ((char *)tmp->content + ft_strlen(key) + 1);
		tmp = tmp->next;
		i++;
	}
	return (NULL);
}

int	env_size(t_list *env)
{
	t_list	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

int	ft_get_key_index(char *key, t_list *env)
{
	t_list	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		if (!ft_strncmp(key, (char *)tmp->content, ft_strlen(key)) && \
		((char *)tmp->content)[ft_strlen(key)] == '=')
			return (i);
		tmp = tmp->next;
		i++;
	}
	return (-1);
}
