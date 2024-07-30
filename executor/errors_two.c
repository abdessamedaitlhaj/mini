/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 21:55:20 by aait-lha          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/07/27 00:34:17 by ael-hara         ###   ########.fr       */
=======
/*   Updated: 2024/07/27 11:29:18 by aait-lha         ###   ########.fr       */
>>>>>>> 7a7ea8b2786f1d88151a7177374e7041338194de
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	common_error(char *str1, char *str2, char *str3)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd(": ", 2);
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

int	   close_streams(int *fd1, int *fd2, t_data *data)
{
	ft_close(fd1, data);
	ft_close(fd2, data);
	return (1);
}

void	close_pipe(int *fd, t_data *data)
{
	ft_close(&fd[0], data);
	ft_close(&fd[1], data);
}
