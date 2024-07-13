/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_fds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:33:03 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/13 09:57:39 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	dup_file(t_aa file, int fd)
{
	if (file == OUTFILE || file == APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_one(NULL, "dup2", fd);
	}
	else if (file == INFILE || file == HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			error_one(NULL, "dup2", fd);
	}
	return (0);
}
int	dup_cmd_in(int *fd, int prev_fd)
{
	if (dup2(prev_fd, STDIN_FILENO) == -1)
		error_one(fd, "dup2", prev_fd);
	ft_close(prev_fd);
	return (0);
}

int	dup_cmd_out(int *fd)
{
	ft_close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_one(fd, "dup2", -1);
	ft_close(fd[1]);
	return (0);
}

int	dup_redir(t_data *data, int i, int *fd, int prev_fd)
{
	if ((data->cmds[i].infile && data->fd_in != -1) || data->cmds[i].heredoc)
		dup_file(INFILE, data->fd_in);
	else if (i > 0)
		dup_cmd_in(fd, prev_fd);
	if ((data->cmds[i].outfile || data->cmds[i].append) && data->fd_out != -1)
		dup_file(OUTFILE, data->fd_out);
	else if(i < data->counter_command - 1)
		dup_cmd_out(fd);
	return (0);
}