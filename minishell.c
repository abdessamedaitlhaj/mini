/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 08:07:08 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/01 02:30:08 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	g_signal_flag = 0;

int	execute_cmds(t_data *data)
{
	int	status;
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
		fail_error("dup", &data->allocated);
	if (data->counter_command == 1)
		data->exit_status = execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
			get_status(data, status);
	}
	if (dup2(stdin_copy, 0) == -1 || \
		dup2(stdout_copy, 1) == -1)
		fail_error("dup2", &data->allocated);
	close_streams(&stdin_copy, &stdout_copy, data);
	return (0);
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

t_data	init_main(char **envp)
{
	t_data	data;

	data = (t_data){NULL, NULL, 0, NULL, NULL, NULL, 0,
		NULL, 0, 0, -2, -2, envp, 0, NULL};
	init_envs(envp, &data);
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
	return (data);
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
		{
			line_exit(&data, line);
			break ;
		}
		check_signal(&data);
		if (!parsing(line, &data))
			continue ;
		if (parsing_signal())
			continue ;
		execute_and_free(&data, line, &term);
	}
	return (free_env(&data.env), data.exit_status);
}
