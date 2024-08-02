/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:24:18 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 20:28:38 by ael-hara         ###   ########.fr       */
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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	ft_skip_spaces(int *sign, char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}
