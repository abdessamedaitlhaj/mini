/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:39:58 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/04 01:44:12 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_pipes(char **pipes, t_data *data)
{
	int	i;

	i = 0;
	while (pipes[i])
	{
		quote_replace(pipes[i], -5, '|');
		quote_replace(pipes[i], ' ', -6);
		quote_replace(pipes[i], '\t', -42);
		pipes[i] = expanding_outside(pipes[i], data);
		i++;
	}
}

void	process_line(char **line, t_data *data)
{
	quote_replace(*line, -8, ' ');
	quote_replace(*line, -42, '\t');
	*line = add_space_redir(*line, data);
	quote_replace(*line, -6, '>');
	quote_replace(*line, -7, '<');
	quote_replace(*line, '$', -11);
}

void	process_line_history(char *line)
{
	add_history(line);
	quote_replace(line, '|', -5);
	quote_replace(line, '>', -6);
	quote_replace(line, '<', -7);
	quote_replace(line, ' ', -8);
	quote_replace(line, '\t', -42);
}

int	process_errors(char *line, t_data *data)
{
	int	min;
	int	i;

	if (pair_quotes(line).index != -1 || parsing_pipe(line).index != -1
		|| parsing_redir(line).index != -1)
	{
		min = ft_min(pair_quotes(line).index, parsing_pipe(line).index,
				parsing_redir(line).index);
		herdoc_delimiters(line, min, data);
	}
	i = 0;
	while (i < data->delimiter_count && !g_signal_flag)
	{
		empty_line(data->heredoc[i], data);
		i++;
	}
	data->exit_status = 258;
	write(2, "minishell: syntax error\n", 24);
	return (free_allocated(&data->allocated), free(line), 0);
}

int	empty_line_parse(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (!line[i])
		return (1);
	return (0);
}
