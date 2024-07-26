/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/26 01:12:23 by aait-lha         ###   ########.fr       */
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
	free(tmp);
	new = ft_lstnew(new_var);
	if (!new)
	{
		free(new_var);
		free(tmp);
		fail_error("malloc", &data->allocated);
	}
	ft_lstadd_back(&data->env, new);
	return (0);
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
			continue ;
		}
		if (ft_get_key_index(k_v.key, data->env) != -1)
		{
			if (k_v.append)
				k_v.value = ft_strjoin(ft_getenv(k_v.key, data->env), \
					k_v.value, data);
			ft_unsetenv(k_v.key, data);
			if (k_v.value)
				ft_setenv(k_v.key, k_v.value, data);
		}
		else
			if (k_v.value)
				ft_setenv(k_v.key, k_v.value, data);
	}
	return (0);
}
