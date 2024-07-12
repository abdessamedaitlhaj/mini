/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:29:45 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/12 00:56:43 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	setting_redir_out(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -2;
	while (cmd->files[i])
	{
		if (cmd->files[i]->expand_error)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(" : ambiguous redirect", 2);
			return (-2);
		}
		if (fd > 2)
			ft_close(fd);
		if (cmd->files[i]->type == OUTFILE)
			fd = open_files(cmd->files[i]->file, 0);
		else if (cmd->files[i]->type == APPEND)
			fd = open_files(cmd->files[i]->file, 1);
		if (fd == -1)
			return (error_two(cmd->files[i]->file));
		i++;
	}
	dup_file(cmd->files[i - 1]->type, fd);
	return (0);
}
int	setting_redir_in(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -2;
	while (cmd->files[i])
	{
		if (cmd->files[i]->expand_error)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->files[i]->file, 2);
			ft_putendl_fd(" : ambiguous redirect", 2);
			return (1);
		}
		if (cmd->files[i]->type == INFILE || cmd->files[i]->type == HEREDOC)
		{
			if (fd > 2)
				ft_close(fd);
			fd = open_files(cmd->files[i]->file, 2);
		}
		if (fd == -1)
			return (1);
		i++;
	}
	dup_file(cmd->files[i - 1]->type, fd);
	return (0);
}