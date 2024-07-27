/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:39:04 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/26 19:43:19 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_envs(char **envp, t_data *data)
{
	int		i;

	if (!*envp)
	{
		ft_setenv("PATH", "/usr/bin:/bin:/usr/sbin:/sbin", data);
		ft_setenv("SHLVL", "1", data);
		ft_setenv("_", "minishell", data);
	}
	else
	{
		i = -1;
		while (envp[++i])
			ft_lstadd_back(&data->env, ft_lstnew(envp[i]));
	}
}

int	ft_env(t_list *envp)
{
	while (envp)
	{
		ft_putendl_fd(envp->content, 1);
		envp = envp->next;
	}
	return (0);
}
