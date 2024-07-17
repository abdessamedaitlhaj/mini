/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/16 16:51:10 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_list *env, t_data *data)
{
	t_list	*tmp;
	char	*key;
	char	*value;

	tmp = env;
	while (tmp)
	{
		key = ft_substr((char *)tmp->content, 0, \
		ft_strchr((char *)tmp->content, '=') - (char *)tmp->content, data);
		value = ft_strchr((char *)tmp->content, '=') + 1;
		printf("declare -x %s=\"%s\"\n", key, value);
		tmp = tmp->next;
	}
}

int	is_key_valid(char *key)
{
	if (!ft_isalpha(key[0]) && key[0] != '_')
	{
		ft_putstr_fd("export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	return (0);
}

int	ft_setenv(char *key, char *value, t_data *data)
{
	char	*new_var;
	char	*tmp;
	t_list	*new;

	tmp = ft_strjoin(key, "=", data);
	if (!tmp)
		error_three("malloc");
	new_var = ft_strjoin(tmp, value, data);
	if (!new_var)
	{
		free(tmp);
		error_three("malloc");
	}
	free(tmp);
	new = ft_lstnew(new_var);
	if (!new)
	{
		free(new_var);
		error_three("malloc");
	}
	ft_lstadd_back(&data->env, new);
	return (0);
}

int	ft_export(char **args, t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	char	*tmp;

	if (!args[0])
	{
		print_env(data->env, data);
		return (0);
	}
	i = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			if (ft_strchr(args[i], '+'))
			{
				tmp = ft_strchr(args[i], '+');
				if (tmp[1] == '=')
				{
					key = ft_substr(args[i], 0, ft_strchr(args[i], '+') - args[i], data);
					if (is_key_valid(key) == 0)
					{
						value = ft_strchr(args[i], '+') + 2;
						if (ft_get_key_index(key, data->env) != -1)
						{
							value = ft_strjoin(ft_getenv(key, data->env), value, data);
							ft_unsetenv(key, data);
							ft_setenv(key, value, data);
						}
						else
							return (1);
					}
				}
			}
			else
				key = ft_substr(args[i], 0, ft_strchr(args[i], '=') - args[i], data); 
			if (is_key_valid(key) == 0)
			{
				value = ft_strchr(args[i], '=') + 1;
			}
			else
				return (1);
			if (ft_get_key_index(key, data->env) != -1)
				ft_unsetenv(key, data);
			ft_setenv(key, value, data);
			free(key);
		}
		i++;
	}
	return (0);
}