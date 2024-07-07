/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:20:01 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/06 20:15:29 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "executor.h"

int	ft_echo(char **args, int n, int status)
{
	int	i;
	int	n_flag;
	int	j;

	i = 0;
	n_flag = 0;
	if (ft_strcmp(args[0], "$?") == 0 && n == 1)
	{
		printf("%d\n", status);
		return (0);
	}
	while (n && !ft_strncmp(args[i], "-n", 2))
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
			n_flag = 1;
		i++;
	}
	while (i < n)
	{
		ft_putstr_fd(args[i], 1);
		if (i < n - 1)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", 1);
	return (0);
}