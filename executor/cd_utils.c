/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 20:24:18 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/05 21:48:06 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"



char	*oldpwd_set(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_getenv("OLDPWD", data->env);
	if (oldpwd)
		return (oldpwd);
	ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	return (NULL);
}
