/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 21:55:20 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/19 08:52:36 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

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

int	not_valid_identifier(char *str, char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": '", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

