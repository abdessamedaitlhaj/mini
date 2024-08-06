/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:39:04 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/06 01:06:59 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_empty_env(t_data *data)
{
	char	*path;
	char	*shlvl;
	char	*last_cmd;

	data->empty_env = 1;
	path = ft_strdup2("/usr/bin:/bin:/usr/sbin:/sbin:.", data);
	data->pwd = copy_cwd(getcwd(NULL, 0), data);
	shlvl = ft_strdup2("1", data);
	last_cmd = ft_strdup2("/usr/bin/env", data);
	ft_add_env(&data->env, ft_new_env(ft_strdup2("PWD", data), \
	data->pwd, data));
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
		data->pwd = copy_cwd(getcwd(NULL, 0), data);
		i = -1;
		while (envp[++i])
		{
			j = 0;
			while (envp[i][j] && envp[i][j] != '=')
				j++;
			key = ft_substr2(envp[i], 0, j, data);
			value = ft_strdup2(ft_strchr(envp[i], '=') + 1, data);
			if (!key || !value)
				continue ;
			ft_add_env(&data->env, ft_new_env(key, value, data));
		}
	}
}

int	ft_env(t_env *env, int arg_number)
{
	if (arg_number)
	{
		ft_putendl_fd("env: No such file or directory", 2);
		return (127);
	}
	if (!ft_getenv("PATH", env))
	{
		ft_putendl_fd("minishell: env: No such file or directory", 2);
		return (127);
	}
	while (env)
	{
		ft_putstr_fd(env->key, 1);
		ft_putstr_fd("=", 1);
		ft_putendl_fd(env->value, 1);
		env = env->next;
	}
	return (0);
}
