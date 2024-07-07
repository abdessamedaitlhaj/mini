/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:13:16 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 09:46:53 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	repalce_char_in_linked_list(t_list *lst, char c, char new_c)
{
	char	*content;
	char	*ptr;

	while (lst)
	{
		content = lst->content;
		ptr = content;
		while (*ptr != '\0')
		{
			if (*ptr == c)
				*ptr = new_c;
			ptr++;
		}
		lst = lst->next;
	}
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	ft_strlen(char *str)
{
	int	i;
	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	count_str_in_2d_array(char *find, char **array)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (array[i])
	{
		if (ft_strcmp(find, array[i]) == 0)
			count++;
		i++;
	}
	return (count);
}

char	*ft_strdup(char *str, t_list **allocated)

{
	char	*new_str;
	int		i;

	i = 0;
	new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 1), allocated);
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
