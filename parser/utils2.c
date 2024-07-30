/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:01:38 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 03:02:43 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_substr(char *s, unsigned int start, size_t len, t_data *data)
{
	char	*str;
	size_t	i;

	i = 0;
	str = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	if (!str)
		return (NULL);
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strjoin(char *s1, char *s2, t_data *data)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = ft_malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1),
			&data->allocated);
	if (!str)
		fail_error("malloc failed", &data->allocated);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i] = s2[j];
		i++;
		j++;
	}
	str[i] = '\0';
	return (str);
}

void	quote_replace(char *str, char find, char replace)
{
	int		i;
	char	c;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			c = str[i];
			i++;
			while (str[i] && str[i] != c)
			{
				if (str[i] == find)
					str[i] = replace;
				i++;
			}
		}
		i++;
	}
}

int	ft_min(int a, int b, int c)
{
	if (a == -1)
		a = 2147483647;
	if (b == -1)
		b = 2147483647;
	if (c == -1)
		c = 2147483647;
	if (a <= b && a <= c)
		return (a);
	if (b <= a && b <= c)
		return (b);
	return (c);
}

char	*ft_strndup( char *s, size_t n, t_data *data)
{
	size_t		i;
	size_t		len;
	char		*dup;

	i = 0;
	len = ft_strlen(s);
	if (n < len)
		len = n;
	dup = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}
