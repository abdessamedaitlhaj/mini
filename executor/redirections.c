/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:29:45 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/13 12:25:02 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	init_fd_out(t_data *data, t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -2;
	while (cmd->files[i])
	{
		if (fd > 2)
			ft_close(fd);
		if (cmd->files[i]->expand_error)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			return (-2);
		}
		if (cmd->files[i]->type == OUTFILE)
			fd = open_files(cmd->files[i]->file, 0);
		else if (cmd->files[i]->type == APPEND)
			fd = open_files(cmd->files[i]->file, 1);
		if (fd == -1)
			return (error_two(cmd->files[i]->file));
		i++;
	}
	data->fd_out = fd;
	return (0);
}
int	init_fd_in(t_data *data, t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -2;
	while (cmd->files[i])
	{
		if (fd > 2)
			ft_close(fd);
		if (cmd->files[i]->expand_error)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(": ambiguous redirect", 2);
			return (-2);
		}
		if (cmd->files[i]->type == INFILE || cmd->files[i]->type == HEREDOC)
			fd = open_files(cmd->files[i]->file, 2);
		if (fd == -1)
			return (error_two(cmd->files[i]->file));
		i++;
	}
	data->fd_in = fd;
	return (0);
}