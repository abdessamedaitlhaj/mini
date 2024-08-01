/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 02:27:21 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 02:32:00 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_while(t_data *data, struct termios *term)
{
	data->allocated = NULL;
	data->fd_in = -2;
	data->fd_out = -2;
	tcgetattr(0, term);
}

void	execute_and_free(t_data *data, char *line, struct termios *term)
{
	execute_cmds(data);
	free(line);
	free_allocated(&data->allocated);
	tcsetattr(0, TCSANOW, term);
}

void	line_exit(t_data *data, char *line)
{
	ft_putstr_fd("exit\n", 1);
	free(line);
	free_allocated(&data->allocated);
	rl_clear_history();
}

void	check_signal(t_data *data)
{
	if (g_signal_flag == 1 && data->exit_status != 1)
	{
		data->exit_status = 1;
		g_signal_flag = 0;
	}
}

int	parsing_signal(void)
{
	if (g_signal_flag == 1)
	{
		g_signal_flag = 0;
		return (1);
	}
	return (0);
}
