/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:29:45 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/13 20:20:35 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	init_fds(t_data *data, t_cmd *cmd)
{
	int	i;
	int	fd_in;
	int	fd_out;

	i = -1;
	fd_in = -2;
	fd_out = -2;
	while (cmd->files[++i])
	{
		if (cmd->files[i]->expand_error)
		{
			ft_close_two(fd_in, fd_out);
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			return (1);
		}
		if (cmd->files[i]->type == INFILE || cmd->files[i]->type == HEREDOC)
		{
			if (fd_in > 2)
				ft_close(fd_in);
			fd_in = open_files(cmd->files[i]->file, 2);
		}
		else if (cmd->files[i]->type == OUTFILE)
		{
			if (fd_out > 2)
				ft_close(fd_out);
			fd_out = open_files(cmd->files[i]->file, 0);
		}
		else if (cmd->files[i]->type == APPEND)
		{
			if (fd_out > 2)
				ft_close(fd_out);
			fd_out = open_files(cmd->files[i]->file, 1);
		}
		if (fd_in == -1 || fd_out == -1)
		{
			ft_close_two(fd_in, fd_out);
			return (error_two(cmd->files[i]->file));
		}
	}
	if (fd_in > 2)
		data->fd_in = fd_in;
	if (fd_out > 2)
		data->fd_out = fd_out;
	return (0);
}