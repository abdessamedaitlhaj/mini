/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:19:43 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 03:03:04 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strncmp(char const *s1, char const *s2, size_t n)
{
	if (n == 0)
	{
		return (0);
	}
	while (*s1 == *s2 && *s1 && *s2 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return (((unsigned char)(*s1) - (unsigned char)(*s2)));
}

char	*get_env_value(char *key, t_list *env)
{
	char	*env_str;
	int		i;

	while (env != NULL)
	{
		env_str = (char *)env->content;
		i = 0;
		while (env_str[i] != '\0' && env_str[i] != '=')
			i++;
		if (env_str[i] == '=' && ft_strncmp(env_str, key, i) == 0
			&& key[i] == '\0')
		{
			return (env_str + i + 1);
		}
		env = env->next;
	}
	return (NULL);
}

int	ft_isnum(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalnum(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || ft_isnum(c))
		return (1);
	return (0);
}

int	skip_redirection(char *pipe, int i, int flag)
{
	if (!flag)
		i += 2;
	else
		i++;
	while (pipe[i] && ft_isspace(pipe[i]))
		i++;
	while (pipe[i] && !ft_isspace(pipe[i]))
		i++;
	return (i);
}
