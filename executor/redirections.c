/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:29:45 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/15 16:40:18 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	init_fds(t_data *data, t_cmd *cmd)
{
	int	i;

	i = -1;
	while (cmd->files[++i])
	{
		if (cmd->files[i]->expand_error)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			return (1);
		}
		if (cmd->files[i]->type == INFILE || cmd->files[i]->type == HEREDOC)
		{
			if (data->fd_in > 2)
				close(data->fd_in);
			data->fd_in = open_files(cmd->files[i]->file, 2);
		}
		else if (cmd->files[i]->type == OUTFILE)
		{
			if (data->fd_out > 2)
				close(data->fd_out);
			data->fd_out = open_files(cmd->files[i]->file, 0);
		}
		else if (cmd->files[i]->type == APPEND)
		{
			if (data->fd_out > 2)
				close(data->fd_out);
			data->fd_out = open_files(cmd->files[i]->file, 1);
		}
		if (data->fd_in == -1 || data->fd_out == -1)
			return (error_two(cmd->files[i]->file));
	}
	return (0);
}