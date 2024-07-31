/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 23:21:23 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/31 00:10:35 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_space_help(char **line_new, char *line, int *i, t_data *data)
{
	*line_new = ft_strjoin(*line_new, " ", data);
	*line_new = ft_strjoin(*line_new, ft_substr(line, *i, 1, data), data);
	(*i)++;
}

char	*add_space_redir(char *line, t_data *data)
{
	int		i;
	char	*line_new;

	i = 0;
	line_new = ft_strdup("", &data->allocated);
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
		{
			add_space_help(&line_new, line, &i, data);
			if (line[i] && line[i - 1] == line[i])
			{
				line_new = ft_strjoin(line_new,
						ft_substr(line, i, 1, data), data);
				i++;
			}
			line_new = ft_strjoin(line_new, " ", data);
		}
		else
		{
			line_new = ft_strjoin(line_new, ft_substr(line, i, 1, data), data);
			i++;
		}
	}
	return (line_new);
}

t_parsing_status	parsing_redir(char *line)
{
	int					i;
	t_parsing_status	status ;

	status.index = -1;
	status.error = 0;
	(1) && (status.index = -1, status.error = 0, i = 0);
	while (line[i])
	{
		if (line[i] == '>' || line[i] == '<')
		{
			i++;
			if (line[i] && line[i - 1] == line[i])
				i++;
			while (line[i] && (line[i] == ' ' || line[i] == '\t'))
				i++;
			if (line[i] == '\0' || line[i] == '|' || line[i] == '<'
				|| line[i] == '>')
			{
				(1) && (status.error = 1, status.index = i);
				return (status);
			}
		}
		i++;
	}
	return (status);
}
