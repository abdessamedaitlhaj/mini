/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:25:27 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/23 13:46:28 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	ft_isalnm(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
	(c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	is_valid(char *arg, char *cmd)
{
	int	i;
	char **key;

	i = 0;
	key = ft_split(arg, '=');
	if (!key)
		return (not_valid_identifier(arg, cmd));
	while (key[0][i])
	{
		if (ft_isdigit(key[0][0]) || \
			(!ft_isalnm(key[0][i]) && key[0][i] != '_'))
			return (free_array(key), not_valid_identifier(arg, cmd));
		i++;
	}
	return (free_array(key), 0);
}

int	ft_unset(char **args, t_data *data, char *cmd)
{
	t_list	*tmp;
	int		i;
	int		j;

	j = -1;
	while (args[++j])
	{
		if (is_valid(args[j], cmd))
			continue ;
		tmp = data->env;
		i = 0;
		while (tmp)
		{
			if (!ft_strncmp(args[j], (char *)tmp->content, ft_strlen(args[j])) && \
			((char *)tmp->content)[ft_strlen(args[j])] == '=')
				ft_lstremove(&data->env, i);
			tmp = tmp->next;
			i++;
		}
	}
	return (0);
}

int	ft_unsetenv(char *key, t_data *data, char *cmd)
{
	t_list	*tmp;
	int		i;

	tmp = data->env;
	i = 0;
	while (tmp)
	{
		if (is_valid(tmp->content, cmd))
			return (1);
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