/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:04:15 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/18 09:34:38 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len = 1;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n, t_data *data)
{
	char	*str;
	long	nbr;
	int		len;

	nbr = n;
	len = ft_numlen(nbr);
	str = ft_malloc(len + 1, &data->allocated);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	if (nbr == 0)
		str[0] = '0';
	while (nbr > 0)
	{
		str[--len] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (str);
}

void	handle_heredoc(t_indexes indexes, t_data *data)
{
	char	*index_str;
	char	*base_path;
	char	*path;

	if (indexes.k < data->cmds[indexes.i].heredoc)
		empty_line(data->cmds[indexes.i].files[indexes.j]->file);
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
	while (indexes.i < data->counter_command)
	{
		indexes.j = 0;
		indexes.k = 1;
		while (data->cmds[indexes.i].files[indexes.j])
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

void process_heredoc_quotes(char **heredoc, t_data *data)
{
	quote_replace(*heredoc, -6, ' ');
	quote_replace(*heredoc, -5, '|');
	quote_replace(*heredoc, -7, '<');
	quote_replace(*heredoc, -8, ' ');
	quote_replace(*heredoc, -42, '\t');
	*heredoc = remove_q(*heredoc, data);
}

void process_heredoc_line(char *line, int min, t_data *data)
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
			data->heredoc[j] = ft_strndup(&line[start], i - start);
			process_heredoc_quotes(&data->heredoc[j], data);
			j++;
		}
		i++;
	}
}

void herdoc_delimiters(char *line, int min, t_data *data)
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

static	void	process_pipes(char **pipes, t_data *data)
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

static void	process_line(char **line, t_data *data)
{
	quote_replace(*line, -8, ' ');
	quote_replace(*line, -42, '\t');
	*line = add_space_redir(*line, data);
	quote_replace(*line, -6, '>');
	quote_replace(*line, -7, '<');
	quote_replace(*line, '$', -11);
}

static void	process_line_history(char *line)
{
	add_history(line);
	quote_replace(line, '|', -5);
	quote_replace(line, '>', -6);
	quote_replace(line, '<', -7);
	quote_replace(line, ' ', -8);
	quote_replace(line, '\t', -42);
}

static int process_errors(char *line, t_data *data)
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
	while (i < data->delimiter_count)
	{
		empty_line(data->heredoc[i]);
		i++;
	}
	data->exit_status = 258;
	write(2, "minishell: syntax error\n", 24);
	return (free(line), 0);
}

int	parsing(char *line, t_data *data)
{
	char	**pipes;
	int		i;

	if (line[0] == '\0')
		return (free(line), 0);
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
