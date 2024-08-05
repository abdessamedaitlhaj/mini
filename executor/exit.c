/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:48:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/04 21:39:42 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void ft_skip_spaces(int *sign, char **str)
{
	while (**str && ft_isspace(**str))
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			*sign = -1;
		(*str)++;
	}
}

int	ft_atoi(char *str, int *f)
{
	int				sign;
	long long int	c;
	long long int	sum;

	sum = 0;
	sign = 1;
	ft_skip_spaces(&sign, &str);
	while (*str && ft_isdigit(*str))
	{
		c = sum;
		sum = sum * 10 + sign * (*str - 48);
		if ((sum > c && sign < 0) || (sum < c && sign > 0))
		{
			if (f)
				*f = 1;
			return (-1);
		}
		str++;
	}
	return (sum);
}

static int	is_num(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	no_arg(t_data *data)
{
	if (data->counter_command == 1)
		ft_putendl_fd("exit", 1);
	free_allocated(&data->allocated);
	free_env(&data->env);
	exit(data->exit_status);
}

int	ft_exit(char **args, int arg_number, t_data *data)
{
	int	e;
	int	f;

	f = 0;
	if (!arg_number)
		no_arg(data);
	if (!is_num(args[0]) || !args[0][0])
		exit_error(args[0], data);
	e = ft_atoi(args[0], &f);
	if (f == 1)
		exit_error(args[0], data);
	if (args[1])
	{
		if (data->counter_command == 1)
			ft_putendl_fd("exit", 1);
		ft_putendl_fd("minishell: exit: too many arguments", 1);
		return (1);
	}
	free_allocated(&data->allocated);
	free_env(&data->env);
	if (data->counter_command == 1)
		ft_putendl_fd("exit", 1);
	exit(e % 256);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}