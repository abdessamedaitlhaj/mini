/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 23:13:15 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/05 21:01:33 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		exit(4);
		// g_signal_flag = 100;
	}
}

void	get_status(t_data *data, int status)
{
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
		{
			ft_putstr_fd("\n", 1);
			data->exit_status = 130;
		}
	}
	else
		ft_putstr_fd("\n", 2);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		data->exit_status = 131;
		ft_putstr_fd("Quit: ", STDOUT_FILENO);
		ft_putendl_fd(ft_itoa(WTERMSIG(status), data), STDOUT_FILENO);
	}
}
