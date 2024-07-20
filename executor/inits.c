/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:29:45 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/20 16:59:38 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	init_fds(t_data *data, t_cmd *cmd)
{
	int	i;
	int z = 0;
	i = -1;
	while (cmd->files[++i])
	{

		if (cmd->files[i]->type == HEREDOC)
			z++;
		if (cmd->files[i]->expand_error)
			return (ambigious_error(cmd->files[i]->file));
		if (cmd->files[i]->type == OUTFILE)
		{
			ft_close(&data->fd_out, data);
			data->fd_out = open_files(cmd->files[i]->file, 0);
		}
		else if (cmd->files[i]->type == APPEND)
		{
			ft_close(&data->fd_out, data);
			data->fd_out = open_files(cmd->files[i]->file, 1);
		}
		else if (cmd->files[i]->type == INFILE || (cmd->files[i]->type == HEREDOC && z == cmd->heredoc))
		{
			ft_close(&data->fd_in, data);
			data->fd_in = open_files(cmd->files[i]->file, 2);
		}
		if (data->fd_in == -1 || data->fd_out == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(": No such file or directory", 2);
			return (1);
		}
	}
	return (0);
}

int	check_files(t_data *data, int i, int *fd, int *prev_fd)
{
	data->exit_status = 0;
	data->fd_in = -2;
	data->fd_out = -2;
	if (data->cmds[i].files)
	{
		if (init_fds(data, &data->cmds[i]))
		{
			close_streams(&data->fd_in, &data->fd_out, data);
			close_streams(&data->fd_in, &data->fd_out, data);
			if (i > 0)
				ft_close(prev_fd, data);
			*prev_fd = -2;
			return (1) ;
		}
	}
	if (data->cmds[i].flag_command)
	{
		close_streams(&data->fd_in, &data->fd_out, data);
		close_pipe(fd, prev_fd, data);
		if (i > 0)
				ft_close(prev_fd, data);
		*prev_fd = -2;
		return (1) ;
	}
	return (0);
}