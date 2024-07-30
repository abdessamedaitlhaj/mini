/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:39:04 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 00:43:39 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*shell_level(char *value, t_data *data)
{
	char	*shlvl;
	int		new_shlvl;

	new_shlvl = ft_atoi(value, NULL);
	if (new_shlvl < 0)
		new_shlvl = 0;
	else if (new_shlvl > 999 || !is_num(value))
		new_shlvl = -1;
	else if (new_shlvl < 0)
		new_shlvl = 0;
	else
		new_shlvl++;
	if (new_shlvl == -1)
		shlvl = NULL;
	else
		shlvl = ft_itoa2(new_shlvl, data);
	return (shlvl);
}

char	*ft_getcwd()
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		perror("");
		return(NULL);
	}
	return (cwd);
}

void	handle_empty_env(t_data *data)
{
	char	*value;

	data->empty_env = 1;
	data->pwd = ft_getcwd();
	ft_add_env(&data->env, ft_new_env("PWD", data->pwd, data));
	ft_add_env(&data->env, ft_new_env("PATH", "/usr/bin:/bin:/usr/sbin:/sbin:.", data));
	value = shell_level("1", data);
	ft_add_env(&data->env, ft_new_env("SHLVL", value, data));
	ft_add_env(&data->env, ft_new_env("_", "/usr/bin/env", data));
}

void	init_envs(char **envp, t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	char	**args;

	if (!*envp)
		handle_empty_env(data);
	else
	{
		i = -1;
		data->pwd = ft_getcwd();
		while (envp[++i])
		{
			args = ft_split_str(envp[i], "=", data);
			key = ft_strdup2(args[0], data);
			value = ft_strdup2(args[1], data);
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
