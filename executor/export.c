/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/03 21:18:58 by aait-lha         ###   ########.fr       */
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
			free(tmp->key);
			free(tmp->value);
			tmp->key = key;
			tmp->value = value;
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
	char	*v;

	tmp = data->env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, k_v->key) == 0)
		{
			if (k_v->append)
			{
				v = tmp->value;
				tmp->value = ft_strjoin2(v, k_v->value, data);
				free(v);
				free(k_v->value);
			}
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

static	void	set_env(t_key_value *k_v, t_data *data)
{
	if (ft_get_key_index(k_v->key, data->env) != -1)
	{
		free(k_v->key);
		replace_env(k_v, data);
	}
	else
		ft_add_env(&data->env, ft_new_env(k_v->key, k_v->value, data));
}

int	ft_export(char **args, t_data *data, char *cmd)
{
	int			i;
	t_key_value	k_v;
	int			status;

	if (check_empty_args(data, args))
		return (1);
	i = -1;
	while (args[++i])
	{
		status = 0;
		k_v = (t_key_value){NULL, NULL, 0, 0};
		if (is_valid(&k_v, args[i], data))
		{
			if (k_v.err)
				not_valid_identifier(args[i], cmd);
			status = 1;
			continue ;
		}
		set_env(&k_v, data);
	}
	return (status);
}
