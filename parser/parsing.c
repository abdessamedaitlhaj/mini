/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 20:34:56 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 02:45:27 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int g_signal_flag;

void	handle_heredoc(t_indexes indexes, t_data *data)
{
	char	*index_str;
	char	*base_path;
	char	*path;

	if (indexes.k < data->cmds[indexes.i].heredoc)
		empty_line(data->cmds[indexes.i].files[indexes.j]->file, data);
	else
	{
		index_str = ft_itoa(indexes.index, data);
		base_path = "/tmp/heredoc";
		path = ft_strjoin(base_path, index_str, data);
		indexes.index++;
		indexes.l = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (data->cmds[indexes.i].files[indexes.j]->expanding_heredoc == 0)
			push_line_expand(indexes.l, data
				->cmds[indexes.i].files[indexes.j]->file, data);
		else
			push_line(indexes.l, data
				->cmds[indexes.i].files[indexes.j]->file, data);
		close(indexes.l);
		indexes.l = open(path, O_RDONLY);
		data->cmds[indexes.i].files[indexes.j]->fd = indexes.l;
		data->cmds[indexes.i].files[indexes.j]->file = path;
	}
}

void	open_heredoc(t_data *data)
{
	t_indexes	indexes;

	indexes = (t_indexes){0, 0, 0, 1, 1};
	while (indexes.i < data->counter_command && !g_signal_flag)
	{
		indexes.j = 0;
		indexes.k = 1;
		while (data->cmds[indexes.i].files[indexes.j] && !g_signal_flag)
		{
			if (data->cmds[indexes.i].files[indexes.j]->type == HEREDOC)
			{
				handle_heredoc(indexes, data);
				indexes.k++;
			}
			indexes.j++;
		}
		indexes.i++;
	}
}

int	parsing(char *line, t_data *data)
{
	char	**pipes;
	int		i;

	if (line[0] == '\0' || empty_line_parse(line))
		return (free(line), data->exit_status = 0, 0);
	process_line_history(line);
	if (pair_quotes(line).error || parsing_pipe(line).error
		|| parsing_redir(line).error)
	{
		return (process_errors(line, data));
	}
	process_line(&line, data);
	pipes = ft_split_str(line, "|", data);
	if (!pipes)
		return (0);
	data->pipes = pipes;
	i = 0;
	while (pipes[i])
		i++;
	data->counter_command = i;
	process_pipes(pipes, data);
	fill_command(data);
	open_heredoc(data);
	return (1);
}
