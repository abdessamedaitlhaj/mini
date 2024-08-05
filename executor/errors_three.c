/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_three.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:30:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/04 21:39:08 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fail_error(char *str, t_list **garbage_collector)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	free_allocated(garbage_collector);
	exit(1);
}

void	exit_error(char *number, t_data *data)
{
	if (data->counter_command == 1)
		ft_putendl_fd("exit", 1);
	ft_putstr_fd("minishell: exit: ", 1);
	ft_putstr_fd(number, 1);
	ft_putendl_fd(": numeric argument required", 1);
	free_allocated(&data->allocated);
	free_env(&data->env);
	exit(255);
}
