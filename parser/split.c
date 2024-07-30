/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:00:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 02:56:44 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	malloc_tab(char **tab, t_split_args *args, int i, t_data *data)
{
	int	k;
	int	j;

	k = 0;
	while (args->s[k] && !ft_strchr(args->delimiters, args->s[k]))
		k++;
	tab[i] = ft_malloc(sizeof(char) * (k + 1), &data->allocated);
	j = 0;
	while (j < k)
	{
		tab[i][j] = args->s[j];
		j++;
	}
	tab[i][j] = '\0';
	return (1);
}

char	**ft_split_str(char const *s, char const *delimiters, t_data *data)
{
	int				total_length;
	int				i;
	char			**tab;
	t_split_args	args;

	args = (t_split_args){s, delimiters};
	if (!s)
		return (NULL);
	total_length = count_words(s, delimiters);
	tab = ft_malloc(sizeof(char *) * (total_length + 1), &data->allocated);
	i = 0;
	while (i < total_length)
	{
		while (*args.s && ft_strchr(args.delimiters, *args.s))
			args.s++;
		malloc_tab(tab, &args, i, data);
		while (*args.s && !ft_strchr(args.delimiters, *args.s))
			args.s++;
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
