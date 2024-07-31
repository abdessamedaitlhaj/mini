/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/31 09:38:30 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, "_") == 0)
		{
			env = env->next;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(env->key, 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(env->value, 1);
		ft_putendl_fd("\"", 1);
		env = env->next;
	}
}

void	ft_setenv(char *key, char *value, t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			free(tmp->key);
			ft_add_env(&data->env, ft_new_env(key, value, data));
			break ;
		}
		tmp = tmp->next;
	}
	if (!tmp)
		ft_add_env(&data->env, ft_new_env(key, value, data));
}

void	replace_env(t_key_value *k_v, t_data *data)
{
	t_env	*tmp;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, k_v->key) == 0)
		{
			if (k_v->append)
				tmp->value = ft_strjoin(tmp->value, k_v->value, data);
			else
			{
				free(tmp->value);
				tmp->value = k_v->value;
			}
			break ;
		}
		tmp = tmp->next;
	}
}

int	ft_export(char **args, t_data *data, char *cmd)
{
	int			i;
	t_key_value	k_v;

	if (check_empty_args(data, args))
		return (1);
	i = -1;
	while (args[++i])
	{
		k_v = (t_key_value){NULL, NULL, 0, 0};
		if (is_valid(&k_v, args[i], data))
		{
			if (k_v.err)
				not_valid_identifier(k_v.key, cmd);
			free(k_v.key);
			continue ;
		}
		if (ft_get_key_index(k_v.key, data->env) != -1)
			replace_env(&k_v, data);
		else
		{
			if (k_v.value)
				ft_add_env(&data->env, ft_new_env(k_v.key, k_v.value, data));
		}
	}
	return (0);
}
