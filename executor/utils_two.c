/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:26:39 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 11:00:03 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		tmp = *env;
		*env = (*env)->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

char	*ft_strdup2(char *str, t_data *data)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		fail_error("malloc failed", &data->allocated);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

char	*copy_cwd(char *cwd, t_data *data)
{
	char	*new_str;
	int		i;

	if (!cwd)
		return (NULL);
	new_str = malloc(sizeof(char) * (ft_strlen(cwd) + 1));
	if (!new_str)
		fail_error("malloc failed", &data->allocated);
	i = 0;
	while (cwd[i])
	{
		new_str[i] = cwd[i];
		i++;
	}
	new_str[i] = '\0';
	free(cwd);
	return (new_str);
}
