/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 08:45:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/23 15:21:16 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	append_value(t_data *data, char *str, char *cmd)
{
	char	*key;
	char	*value;

	key = ft_substr(str, 0, ft_strchr(str, '+') - str, data);
	if (is_valid(key, cmd, data) == 0)
	{
		value = ft_strchr(str, '+') + 2;
		if (ft_get_key_index(key, data->env) != -1)
		{
			value = ft_strjoin(ft_getenv(key, data->env), value, data);
			ft_unsetenv(key, data, cmd);
			ft_setenv(key, value, data);
		}
		else
			return (1);
	}
	return (0);
}

int	overwrite_or_add(t_data *data, char *str, char *cmd)
{
	char	*key;
	char	*value;

	key = ft_substr(str, 0, ft_strchr(str, '=') - str, data); 
	if (is_valid(key, cmd, data) == 0)
	{
		value = ft_strchr(str, '=') + 1;
	}
	else
		return (1);
	if (ft_get_key_index(key, data->env) != -1)
		ft_unsetenv(key, data, cmd);
	ft_setenv(key, value, data);
	return (0);
}

int	check_empty_args(t_data *data, char **args)
{
	if (!args[0])
	{
		print_env(data->env, data);
		return (1);
	}
	return (0);
}