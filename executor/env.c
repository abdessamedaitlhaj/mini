/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:39:04 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 12:44:54 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**get_env_array(t_env *env, t_data *data)
{
	int		i;
	t_env	*tmp;
	char	**env_array;

	i = 0;
	tmp = env;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	env_array = (char **)ft_malloc(sizeof(char *) * (i + 1), &data->allocated);
	i = 0;
	tmp = env;
	while (tmp)
	{
		env_array[i] = ft_strjoin2(tmp->key, "=", data);
		env_array[i] = ft_strjoin2(env_array[i], tmp->value, data);
		tmp = tmp->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*shell_level(char *value, t_data *data)
{
	char	*shlvl;
	int		new_shlvl;

	new_shlvl = ft_atoi(value, NULL);
	if (new_shlvl < 0)
		new_shlvl = 0;
	else if (new_shlvl > 999)
		new_shlvl = 1;
	else if (new_shlvl == 999)
		return ft_strdup2("", data);
	else
		new_shlvl++;
	shlvl = ft_itoa2(new_shlvl, data);
	return (shlvl);
}

void	handle_empty_env(t_data *data)
{
	char	*path;
	char	*pwd;
	char	*shlvl;
	char	*last_cmd;

	data->empty_env = 1;
	path = ft_strdup2("/usr/bin:/bin:/usr/sbin:/sbin:.", data);
	pwd = ft_strdup2(ft_getenv("PWD", data->env), data);
	shlvl = ft_strdup2("1", data);
	last_cmd = ft_strdup2("/usr/bin/env", data);
	ft_add_env(&data->env, ft_new_env(ft_strdup2("PWD", data), ft_getenv("PWD", data->env), data));
	ft_add_env(&data->env, ft_new_env(ft_strdup2("PATH", data), path, data));
	ft_add_env(&data->env, ft_new_env(ft_strdup2("SHLVL", data), shlvl, data));
	ft_add_env(&data->env, ft_new_env(ft_strdup2("_", data), last_cmd, data));
}

void	init_envs(char **envp, t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	int		j;

	if (!*envp)
		handle_empty_env(data);
	else
	{
		i = -1;
		while (envp[++i])
		{
			j = 0;
			while (envp[i][j] && envp[i][j] != '=')
				j++;
			key = ft_substr(envp[i], 0, j, data);
			value = ft_strdup2(ft_strchr(envp[i], '=') + 1, data);
			if (!key || !value)
				continue ;
			if (ft_strcmp(key, "SHLVL") == 0)
				value = shell_level(value, data);
			ft_add_env(&data->env, ft_new_env(key, value, data));
		}
	}
}

int	ft_env(t_env *env, int arg_number)
{
	if (arg_number)
		return (0);
	while (env)
	{
		ft_putstr_fd(env->key, 1);
		ft_putstr_fd("=", 1);
		ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	return (0);
}
