/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:20:01 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/19 07:18:51 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_args(t_data *data, int i, int n, char **args)
{
	while (i < n)
	{
		if (strcmp(args[i], "~") == 0)
			ft_putstr_fd(ft_getenv("HOME", data->env), 1);
		else
			ft_putstr_fd(args[i], 1);
		if (i + 1 < n)
			ft_putstr_fd(" ", 1);
		i++;
	}
}

int	ft_echo(t_data *data, char **args, int n)
{
	int	i;
	int	n_flag;
	int	j;

	i = 0;
	n_flag = 0;
	while (i < n && !ft_strncmp(args[i], "-n", 2))
	{
		j = 2;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		if (!args[i][j])
			n_flag = 1;
		else
			break ;
		i++;
	}
	print_args(data, i, n, args);
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}