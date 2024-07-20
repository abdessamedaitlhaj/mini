/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:33:03 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/20 16:56:04 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	open_files(char *file, int index)
{
	int fd;

	fd = -1;
	if (index == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (index == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (index == 2)
		fd = open(file, O_RDONLY);
	return (fd);
}

int	dup_file(t_aa file, int fd, t_data *data)
{
	if (file == OUTFILE || file == APPEND)
	{
		if (fd > 2)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				fail_error("dup2", data);
		}
	}
	else if (file == INFILE || file == HEREDOC)
	{
		if (fd > 2)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				fail_error("dup2", data);
		}
	}
	return (0);
}
int	dup_cmd_in(int prev_fd, t_data *data)
{
	if (prev_fd > 2)
	{
		if (dup2(prev_fd, STDIN_FILENO) == -1)
			fail_error("dup2", data);
	}
	ft_close(&prev_fd, data);
	return (0);
}

int	dup_cmd_out(int *fd, t_data *data)
{
	ft_close(&fd[0], data);
	if (fd[1] > 2)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			fail_error("dup2", data);
	}
	ft_close(&fd[1], data);
	return (0);
}

int	dup_redir(t_data *data, int i, int *fd, int prev_fd)
{
	if ((data->cmds[i].infile || data->cmds[i].heredoc) && data->fd_in != -1)
	{
		dup_file(INFILE, data->fd_in, data);
		ft_close(&data->fd_in, data);
	}
	else if (i > 0)
		dup_cmd_in(prev_fd, data);
	if ((data->cmds[i].outfile || data->cmds[i].append) && data->fd_out != -1)
	{
		dup_file(OUTFILE, data->fd_out, data);
		ft_close(&data->fd_out, data);
	}
	else if(i < data->counter_command - 1)
		dup_cmd_out(fd, data);
	return (0);
}