/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:28:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/12 18:45:33 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	error_one(int *fd, char *str, int prev_fd)
{

	if (prev_fd != -1)
		ft_close(prev_fd);
	if (fd)
	{
		ft_close(fd[0]);
		ft_close(fd[1]);
	}
	if (str)
		perror(str);
	exit(1);
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

int	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		perror("close");
		exit (1);
	}
	return (0);
}