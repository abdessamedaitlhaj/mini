/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:48:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/15 23:26:51 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(char const *str)
{
	int				sign;
	long long int	c;
	long long int	sum;

	sum = 0;
	sign = 1;
	while (*str && ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		c = sum;
		sum = sum * 10 + sign * (*str - 48);
		if (sum > c && sign < 0)
			return (0);
		if (sum < c && sign > 0)
			return (-1);
		str++;
	}
	return (sum);
}

void	ft_exit(char *number, char **args)
{
	int	len1;
	int	len2;
	int	status;

	if (args[1])
	{
		ft_putendl_fd("exit", 2);
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		exit(1);
	}
	if (!number[0])
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	len1 = ft_strlen(number);
	len2 = ft_strlen("9223372036854775807");

	if (len1 > len2 || (len1 == len2 && ft_strncmp(number, "9223372036854775807", len1) > 0))
	{
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(number, 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(255);
	}
	status = ft_atoi(number);
	if (status > 256)
		status = status % 256;
	ft_putendl_fd("exit", 1);
	exit(status);
}

void	get_status(int *status, t_data *data)
{
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = WTERMSIG(status) + 128;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
		printf("quit: %d\n", WTERMSIG(status));
}