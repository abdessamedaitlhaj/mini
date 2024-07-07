/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:09:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 11:32:49 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

t_parsing_status	parsing_pipe(char *str)
{
	int					i;
	t_parsing_status	status;

	(1) && (status.index = -1, status.error = 0);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '|')
		return ((t_parsing_status){1, i, 0, NULL});
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
			skip_quotes(&i, str);
		if (str[i] == '|')
		{
			i++;
			while (str[i] && ft_isspace(str[i]))
				i++;
			if (str[i] == '|' || str[i] == '\0')
				return ((t_parsing_status){1, i, 0, NULL});
		}
		i++;
	}
	return (status);
}
