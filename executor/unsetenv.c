/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:25:27 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/02 20:00:27 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_remove_env(t_env **env, char	*key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int	is_valid(t_key_value *k_v, char *arg, t_data *data)
{
	int			i;

	i = -1;
	while (arg[++i] && arg[i] != '=' && arg[i] != '+')
	{
		if (ft_isdigit(arg[0]) || \
			(!ft_isalnm(arg[i]) && arg[i] != '_'))
		{
			k_v->err = 1;
			return (1);
		}
	}
	if (i == 0 || !arg[0])
	{
		k_v->err = 1;
		return (1);
	}
	k_v->key = extract_key(k_v, arg, data);
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
	t_env	*tmp;
	int		i;

	i = -1;
	while (args[++i])
	{
		if (is_key_valid(args[i], cmd))
			continue ;
		tmp = data->env;
		while (tmp)
		{
			if (ft_strcmp(args[i], tmp->key) == 0 && ft_strcmp(args[i], "_"))
			{
				ft_remove_env(&data->env, args[i]);
				break ;
			}
			tmp = tmp->next;
		}
	}
	return (0);
}
