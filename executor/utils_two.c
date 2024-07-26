/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:26:39 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/25 19:07:30 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_list **env)
{
	t_list	*tmp;

	while (*env)
	{
		tmp = (*env)->next;
		free(*env);
		*env = tmp;
	}
}

void	init_envs(char **envp, t_data *data)
{
	int		i;

	i = -1;
	while (envp[++i])
		ft_lstadd_back(&data->env, ft_lstnew(envp[i]));
}

char	*ft_strdup2(char *str, t_data *data)

{
	char	*new_str;
	int		i;

	i = 0;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		fail_error("malloc failed", &data->allocated);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
