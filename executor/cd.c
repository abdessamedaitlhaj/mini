/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/06 20:54:01 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "executor.h"

int		ft_chdir(char *path)
{
	if (chdir(path) == -1)
	{
		write (2, "minishell: cd: ", 15);
		write (2, path, ft_strlen(path));
		write (2, ": ", 2);
		perror("");
		return (-1);
	}
	return (0);
}

int	set_home(char *home, char *pwd, char *oldpwd, t_data *data)
{
	if (home)
	{
		if (!home[0])
			return (-1);
		ft_setenv("PWD", home, data);
		ft_setenv("OLDPWD", pwd, data);
		if (ft_chdir(home) == -1)
			return (-1);
	}
	else
		write (2, "minishell: cd: HOME not set\n", 28);
	return (0);
}

int	set_prev(char *oldpwd, char *pwd, t_data *data)
{
	if (oldpwd)
	{
		if (ft_setenv("PWD", oldpwd, data) == -1 || \
			ft_setenv("OLDPWD", pwd, data) == -1 || ft_chdir(oldpwd) == -1)
			return (-1);
	}
	else
		write (2, "minishell: cd: OLDPWD not set", 12);
	return (0);
}

int	ft_cd(char *path, t_data *data)
{
	char	*pwd;
	char	*oldpwd;
	char	*home;

	pwd = ft_pwd(data);
	if (!pwd)
		return (-1);
	oldpwd = ft_getenv("OLDPWD", data->env);
	home = ft_getenv("HOME", data->env);
	if (!path || !path[0] || !ft_strcmp(path, "~") || \
		!ft_strcmp(path, "--"))
	{
		if (set_home(home, pwd, oldpwd, data) == -1)
			return (-1);
	}
	else if (path[0] == '-' && !path[1])
	{
		if (set_prev(oldpwd, pwd, data) == -1)
			return (-1);
	}
	else
	{
		if (ft_setenv("PWD", path, data) == -1 || \
			ft_setenv("OLDPWD", pwd, data) == -1 || ft_chdir(path) == -1)
			return (-1);
	}
	return (0);	
}