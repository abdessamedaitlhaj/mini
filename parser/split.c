/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:00:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 09:46:53 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_tab(char **tab, int i)
{
	while (i > 0)
	{
		i--;
		free(tab[i]);
	}
	free(tab);
}

static int	count_words(char const *s, char const *delimiters)
{
	while (*s && ft_strchr(delimiters, *s))
		s++;
	if (!*s)
		return (0);
	while (*s && !ft_strchr(delimiters, *s))
		s++;
	return (1 + count_words(s, delimiters));
}

static int	malloc_tab(char **tab, char const *s, char const *delimiters, int i, t_data *data)
{
	int	k;
	int	j;

	k = 0;
	while (s[k] && !ft_strchr(delimiters, s[k]))
		k++;
	tab[i] = ft_malloc(sizeof(char) * (k + 1), &data->allocated);
	if (!tab[i])
	{
		free_tab(tab, i);
		return (0);
	}
	j = 0;
	while (j < k)
	{
		tab[i][j] = s[j];
		j++;
	}
	tab[i][j] = '\0';
	return (1);
}

char	**ft_split_str(char const *s, char const *delimiters, t_data *data)
{
	int		total_length;
	int		i;
	char	**tab;

	if (!s)
		return (NULL);
	total_length = count_words(s, delimiters);
	tab = ft_malloc(sizeof(char *) * (total_length + 1), &data->allocated);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < total_length)
	{
		while (*s && ft_strchr(delimiters, *s))
			s++;
		if (!malloc_tab(tab, s, delimiters, i, data))
			return (NULL);
		while (*s && !ft_strchr(delimiters, *s))
			s++;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}