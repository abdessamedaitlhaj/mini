/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/23 15:20:58 by aait-lha         ###   ########.fr       */
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

int	ft_setenv(char *key, char *value, t_data *data)
{
	char	*new_var;
	char	*tmp;
	t_list	*new;

	tmp = ft_strjoin(key, "=", data);
	new_var = ft_strjoin(tmp, value, data);
	if (!new_var)
		free(tmp);
	free(tmp);
	new = ft_lstnew(new_var);
	if (!new)
		free(new_var);
	ft_lstadd_back(&data->env, new);
	return (0);
}

int	ft_export(char **args, t_data *data, char *cmd)
{
	int		i;
	char	*tmp;

	if (check_empty_args(data, args))
		return (1);
	i = -1;
	while (args[++i])
	{
		if (is_valid(args[i], cmd, data))
			continue ;
		if (ft_strchr(args[i], '='))
		{
			if (ft_strchr(args[i], '+'))
			{
				tmp = ft_strchr(args[i], '+');
				if (tmp[1] == '=')
				{
					if (append_value(data, args[i], cmd))
						return (1);
					return (0);
				}
			}
			overwrite_or_add(data, args[i], cmd);
		}
	}
	return (0);
}