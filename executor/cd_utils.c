/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:24:18 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/03 21:16:42 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*home_set(t_data *data)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (home)
		return (home);
	ft_putendl_fd("minishell: cd: HOME not set", 2);
	return (NULL);
}

char	*oldpwd_set(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_getenv("OLDPWD", data->env);
	if (oldpwd)
		return (oldpwd);
	ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	return (NULL);
}
