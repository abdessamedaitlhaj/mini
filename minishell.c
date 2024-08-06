/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 08:07:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/06 01:11:23 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_signal_flag = 0;

int	execute_cmds(t_data *data)
{
	int	stdin_copy;
	int	stdout_copy;
	int	i;

	if (data->counter_command == 1)
		data->exit_status = execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		i = -1;
		while (++i < data->counter_command)
		{
			if (data->status[i].pid != -1)
				waitpid(data->status[i].pid, &data->status[i].status, 0);
		}
		get_status(data, data->status[i - 1].status);
	}
	if (dup2(stdin_copy, 0) == -1 || \
		dup2(stdout_copy, 1) == -1)
		fail_error("dup2", &data->allocated);
	return (close_streams(&stdin_copy, &stdout_copy, data), 0);
}

void	int_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG) == -1)
		{
			g_signal_flag = 1;
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void	check_main(int ac, int isatty_result, char **av)
{
	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell: no arguments needed!\n", 2);
		exit(1);
	}
	if (!isatty_result)
	{
		ft_putstr_fd("minishell: is not a tty!\n", 2);
		exit(1);
	}
}

int	main(int ac, char **av, char **envp)
{
	char			*line;
	struct termios	term;
	t_data			data;

	check_main(ac, isatty(0), av);
	data = init_main(envp);
	while (77)
	{
		init_while(&data, &term);
		line = readline("minishell$ ");
		if (!line)
			line_exit(&data, line);
		check_signal(&data);
		if (!parsing(line, &data))
			continue ;
		if (parsing_signal(&data, line))
			continue ;
		execute_and_free(&data, line, &term);
	}
}
