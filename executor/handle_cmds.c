/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/01 16:36:34 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_one_node(t_data *data)
{
	if (data->cmds[0].files)
	{
		if (init_fds(data, &data->cmds[0]))
		{
			close_streams(&data->fd_in, &data->fd_out, data);
			return (1);
		}
	}
	if (data->cmds[0].files)
	{
		if (data->cmds[0].infile || data->cmds[0].heredoc)
			dup_file(INFILE, data->fd_in, data);
		if (data->cmds[0].outfile || data->cmds[0].append)
			dup_file(OUTFILE, data->fd_out, data);
		close_streams(&data->fd_in, &data->fd_out, data);
	}
	if (data->cmds[0].flag_command)
		return (close_streams(&data->fd_in, &data->fd_out, data));
	if (is_builtin(&data->cmds[0]))
		data->exit_status = ft_exec_builtin(&data->cmds[0], data);
	else
		create_process(data, &data->cmds[0]);
	return (data->exit_status);
}

void	save_last_pipe(t_data *data, int i, int *fd, int *prev_fd)
{
	if (i > 0)
		ft_close(prev_fd, data);
	if (i < data->counter_command - 1)
	{
		*prev_fd = fd[0];
		ft_close(&fd[1], data);
	}
}

void	init(int *fd, t_data *data)
{
	fd[0] = -2;
	fd[1] = -2;
	data->fd_in = -2;
	data->fd_out = -2;
	data->exit_status = 0;
}

int	execute_multiple_nodes(t_data *data)
{
	int	i;
	int	fd[2];
	int	prev_fd;

	i = -1;
	prev_fd = -2;
	while (++i < data->counter_command)
	{
		init(fd, data);
		if (i < data->counter_command - 1)
		{
			if (pipe(fd) == -1)
			{
				close_streams(&data->fd_in, &data->fd_out, data);
				ft_close(&prev_fd, data);
				free_allocated(&data->allocated);
				exit(1);
			}
		}
		if (!init_fds(data, &data->cmds[i]) && !data->cmds[i].flag_command)
			fork_process(data, i, fd, &prev_fd);
		close_streams(&data->fd_in, &data->fd_out, data);
		save_last_pipe(data, i, fd, &prev_fd);
	}
	return (0);
}
