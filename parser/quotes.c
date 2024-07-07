/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 18:08:22 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 11:23:17 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_parsing_status	pair_quotes(char *line)
{
	int					i;
	char				quote;
	t_parsing_status	status ;

	(1) && (status.index = -1, status.error = 0);
	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				status.error = 1;
				status.index = i;
				return (status);
			}
			i++;
		}
		else
			i++;
	}
	return (status);
}

void	skip_quotes(int *i, char *str)
{
	char	c;

	c = str[*i];
	(*i)++;
	while (str[*i] && str[*i] != c)
		(*i)++;
}
