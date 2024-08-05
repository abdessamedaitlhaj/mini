/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 21:55:20 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/05 20:23:17 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	common_error(char *str1, char *str2, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(": ", 2);
	if (str3)
		perror(str3);
	return (1);
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

void	ft_close(int *fd, t_data *data)
{
	if (*fd > 2)
	{
		if (close(*fd) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror("close");
			free_allocated(&data->allocated);
			exit(1);
		}
	}
	*fd = -2;
}

int	close_streams(int *fd1, int *fd2, t_data *data)
{
	ft_close(fd1, data);
	ft_close(fd2, data);
	return (0);
}

void	close_pipe(int *fd, t_data *data)
{
	ft_close(&fd[0], data);
	ft_close(&fd[1], data);
}
