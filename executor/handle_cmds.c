/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/18 12:32:04 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_one_node(t_data *data)
{
	if (data->cmds[0].files)
	{
		if (init_fds(data, &data->cmds[0]))
			return (ft_close_two(data->fd_in, data->fd_out));
	}
	if (data->cmds[0].files)
	{
		if (data->cmds[0].infile || data->cmds[0].heredoc)
			dup_file(INFILE, data->fd_in);
		if (data->cmds[0].outfile || data->cmds[0].append)
			dup_file(OUTFILE, data->fd_out);
		ft_close_two(data->fd_in, data->fd_out);
	}
	if (data->cmds[0].flag_command)
		return(ft_close_two(data->fd_in, data->fd_out));
	if (is_builtin(&data->cmds[0]))
		data->exit_status = ft_exec_builtin(&data->cmds[0], data);
	else
		create_process(data, &data->cmds[0]);
	return (0);
}

void	save_last_pipe(t_data *data, int i, int *fd, int prev_fd)
{
	if (i > 0)
		close(prev_fd);
	if (i < data->counter_command - 1)
	{
		prev_fd = fd[0];
		close(fd[1]);
	}
}

int	execute_multiple_nodes(t_data *data)
{
	int	i;
	int	fd[2];
	int	prev_fd;

	i = -1;
	prev_fd = -2;
	fd[0] = -2;
	fd[1] = -2;
	while (++i < data->counter_command)
	{
		if (check_files(data, i, fd, prev_fd))
			continue ;
		if (i < data->counter_command - 1)
		if (pipe(fd) == -1)
		{
			ft_close_two(data->fd_in, data->fd_out);
			error_one(fd, "pipe", prev_fd);
		}
		fork_process(data, i, fd, prev_fd);
		ft_close_two(data->fd_in, data->fd_out);
		save_last_pipe(data, i, fd, prev_fd);
	}
	return (0);
}