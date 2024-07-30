/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:40:21 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 02:57:31 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_last_str(char *str1, char *str2, char **array)
{
	int	i;
	int	j;
	int	last_str1;
	int	last_str2;

	(1) && (i = 0, last_str1 = -1, last_str2 = -1);
	while (array[i])
	{
		j = 0;
		while (array[i][j])
		{
			if (ft_strcmp(str1, array[i]) == 0)
				last_str1 = i;
			if (ft_strcmp(str2, array[i]) == 0)
				last_str2 = i;
			j++;
		}
		i++;
	}
	if (last_str1 == -1 && last_str2 == -1)
		return (0);
	if (last_str1 > last_str2)
		return (1);
	else
		return (2);
}

void	process_heredoc_quotes(char **heredoc, t_data *data)
{
	quote_replace(*heredoc, -6, ' ');
	quote_replace(*heredoc, -5, '|');
	quote_replace(*heredoc, -7, '<');
	quote_replace(*heredoc, -8, ' ');
	quote_replace(*heredoc, -42, '\t');
	*heredoc = remove_q(*heredoc, data);
}

void	process_heredoc_line(char *line, int min, t_data *data)
{
	int	i;
	int	j;
	int	start;

	j = 0;
	data->heredoc = malloc((data->delimiter_count + 1) * sizeof(char *));
	data->heredoc[data->delimiter_count] = NULL;
	i = 0;
	while (i < min)
	{
		if (line[i] == '<' && line[i + 1] == '<')
		{
			i += 2;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			start = i;
			while (line[i] != ' ' && line[i] != '\t'
				&& line[i] != '\n' && i < min)
				i++;
			data->heredoc[j] = ft_strndup(&line[start], i - start, data);
			process_heredoc_quotes(&data->heredoc[j], data);
			j++;
		}
		i++;
	}
}

void	herdoc_delimiters(char *line, int min, t_data *data)
{
	int	i;
	int	delimiter_count;

	i = 0;
	delimiter_count = 0;
	data->heredoc_error = 1;
	while (i < min)
	{
		if (line[i] == '<' && line[i + 1] == '<')
		{
			i += 2;
			while (line[i] == ' ' || line[i] == '\t')
				i++;
			if (line[i] == '\0' || line[i] == '<' || line[i] == '>')
				break ;
			delimiter_count++;
		}
		i++;
	}
	data->delimiter_count = delimiter_count;
	process_heredoc_line(line, min, data);
}
