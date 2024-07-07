/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:55:14 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/06 20:56:15 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "executor.h"

int	is_key_valid(char *key)
{
	int	i;

	i = 0;
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
		return (-1);
	new_var = ft_strjoin(tmp, value, data);
	if (!new_var)
		return (free(tmp), -1);
	free(tmp);
	new = ft_lstnew(new_var);
	if (!new)
		return (free(new_var), -1);
	ft_lstadd_back(&data->env, new);
	return (0);
}

int	ft_export(char **args, t_data *data)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[0])
	{
		write (2, "export: not enough arguments\n", 29);
		return (-1);
	}
	i = 0;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			key = ft_substr(args[i], 0, ft_strchr(args[i], '=') - args[i], data); 
			if (is_key_valid(key))
			{
				value = ft_strchr(args[i], '=') + 1;
			}
			else
				return (1);
			if (ft_get_key_index(key, data->env) != -1)
				ft_unsetenv(key, data);
			if (ft_setenv(key, value, data) == -1)
				return (-1);
			free(key);
		}
		i++;
	}
	return (0);
}