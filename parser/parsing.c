/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:04:15 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/07 11:37:58 by ael-hara         ###   ########.fr       */
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

char	*ft_itoi(int n, t_data *data)
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

void	open_heredoc(t_data *data)
{
	int		i;
	int		j;
	int		fd;
	int		index;
	int		z;
	char	*index_str;
	char	*base_path;
	char	*path;

	i = 0;
	index =1;
	while (i < data->counter_command)
	{
		j = 0;
		z = 1;
		while (data->cmds[i].files[j])
		{
			if (data->cmds[i].files[j]->type == HEREDOC)
			{
				if (z < data->cmds[i].heredoc)
					empty_line(data->cmds[i].files[j]->file);
				else
				{
					index_str = ft_itoi(index, data);
					base_path = "/tmp/heredoc";
					path = ft_strjoin(base_path, index_str, data);
					index++;
					fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
					if (data->cmds[i].files[j]->expanding_heredoc == 0)
						push_line_expand(fd, data->cmds[i].files[j]->file,
							data);
					else
					{
						push_line(fd, data->cmds[i].files[j]->file, data);
					}
					close(fd);
					fd = open(path, O_RDONLY);
					data->cmds[i].files[j]->fd = fd;
				}
				z++;
			}
			j++;
		}
		i++;
	}
}

int	find_last_str(char *str1, char *str2, char **array)
{
	int	i;
	int	j;
	int	last_str1;
	int	last_str2;

	i = 0;
	last_str1 = -1;
	last_str2 = -1;
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

void print_heredoc(t_data *data)
{
	int i = 0;
	// int j = 0;
	printf("heredoc_error: %d\n", data->heredoc_error);
	while (i < data->counter_command)
	{
		printf("heredoc[%d]: %s\n", i, data->heredoc[i]);
		i++;
	}
}

void herdoc_delimiters(char *line, int min, t_data *data)
{
	int	i;
	int	j;
	int	start;
	int	delimiter_count;

	i = 0;
	j = 0;
	start = 0;
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
	j = 0;
	data->heredoc = malloc((delimiter_count + 1) * sizeof(char *));
	data->heredoc[delimiter_count] = NULL;
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
			{
				i++;
			}
			data->heredoc[j] = ft_strndup(&line[start], i - start);
			quote_replace(data->heredoc[j], -6, ' ');
			quote_replace(data->heredoc[j], -5, '|');
			quote_replace(data->heredoc[j], -7, '<');
			quote_replace(data->heredoc[j], -8, ' ');
			quote_replace(data->heredoc[j], -42, '\t');
			data->heredoc[j] = remove_q(data->heredoc[j], data);
			j++;
		}
		i++;
	}
}

int	parsing(char *line, t_data *data)
{
	char	**pipes;
	int		i;
	int		min;

	if (line[0] == '\0')
		return (free(line), 0);
	add_history(line);
	quote_replace(line, '|', -5);
	quote_replace(line, '>', -6);
	quote_replace(line, '<', -7);
	quote_replace(line, ' ', -8);
	quote_replace(line, '\t', -42);
	if (pair_quotes(line).error || parsing_pipe(line).error || parsing_redir(line).error)
	{
		if (pair_quotes(line).index != -1 || parsing_pipe(line).index != -1 || parsing_redir(line).index != -1)
		{
			min = ft_min(pair_quotes(line).index, parsing_pipe(line).index, parsing_redir(line).index);
			herdoc_delimiters(line, min, data);
		}
		i = 0;
		while (i < data->delimiter_count)
		{
			empty_line(data->heredoc[i]);
			i++;
		}
		write(2, "minishell: syntax error\n", 24);
		return (free(line), 0);
	}
	quote_replace(line, -8, ' ');
	quote_replace(line, -42, '\t');
	line = add_space_redir(line, data);
	quote_replace(line, -6, '>');
	quote_replace(line, -7, '<');
	quote_replace(line, '$', -11);
	pipes = ft_split_str(line, "|", data);
	if (!pipes)
		return (0);
	data->pipes = pipes;
	i = 0;
	while (pipes[i])
		i++;
	data->counter_command = i;
	i = 0;
	while (pipes[i])
	{
		quote_replace(pipes[i], -5, '|');
		quote_replace(pipes[i], ' ', -6);
		quote_replace(pipes[i], '\t', -42);
		pipes[i] = expanding_outside(pipes[i], data);
		i++;
	}
	fill_command(data);
	open_heredoc(data);
	return (1);
}
