/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:25:27 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/26 14:58:25 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_lstremove(t_list **env, int index)
{
	t_list	*tmp;
	t_list	*prev;
	int		i;

	i = 0;
	tmp = *env;
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
		*env = tmp->next;
}

int	is_valid(t_key_value *k_v, char *arg, t_data *data)
{
	int			i;

	k_v->key = extract_key(k_v, arg, data);
	i = -1;
	while (k_v->key[++i])
	{
		if (ft_isdigit(k_v->key[0]) || \
			(!ft_isalnm(k_v->key[i]) && k_v->key[i] != '_'))
		{
			free(k_v->key);
			k_v->err = 1;
			return (1);
		}
	}
	if (ft_strchr(arg, '='))
		k_v->value = ft_strdup2(ft_strchr(arg, '=') + 1, data);
	return (0);
}

int	is_key_valid(char *arg, char *cmd)
{
	int		i;

	i = -1;
	while (arg[++i])
	{
		if (ft_isdigit(arg[0]) || \
			(!ft_isalnm(arg[i]) && arg[i] != '_'))
			return (not_valid_identifier(arg, cmd));
	}
	return (0);
}

int	ft_unset(char **args, t_data *data, char *cmd)
{
	t_list	*tmp;
	int		i;
	int		j;

	j = -1;
	while (args[++j])
	{
		if (is_key_valid(args[j], cmd))
			continue ;
		tmp = data->env;
		i = 0;
		while (tmp)
		{
			if (!ft_strncmp(args[j], (char *)tmp->content, ft_strlen(args[j])))
			{
				ft_lstremove(&data->env, i);
				break ;
			}
			tmp = tmp->next;
			i++;
		}
	}
	return (0);
}

int	ft_unsetenv(char *key, t_data *data)
{
	t_list	*tmp;
	int		i;

	tmp = data->env;
	i = 0;
	while (tmp)
	{
		if (!ft_strncmp(key, (char *)tmp->content, ft_strlen(key)))
		{
			ft_lstremove(&data->env, i);
			return (0);
		}
		tmp = tmp->next;
		i++;
	}
	return (0);
}
