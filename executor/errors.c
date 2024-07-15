/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:28:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/15 18:30:32 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

void	perm_denied(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": Permission denied", 2);
	exit(126);
}

void	cmd_not_found(char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putendl_fd(": command not found", 2);
	exit(127);
}

int	error_one(int *fd, char *str, int prev_fd)
{

	if (prev_fd > 2)
		close(prev_fd);
	if (fd)
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (str)
		perror(str);
	return (1);
}

int	error_two(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	return (1);
}

int	error_three(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	exit(1);
}

int	ft_close_two(int fd1, int fd2)
{
	if (fd1 > 2)
		close(fd1);
	if (fd2 > 2)
		close(fd2);
	return (0);
}