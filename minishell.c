#include "includes/minishell.h"

int g_signal_flag = 0;

int	execute_cmds(t_data *data)
{
	int	status;
	int stdin_copy;
	int stdout_copy;

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

void	INT_HANDLER(int sig)
{
	if (sig == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			g_signal_flag = 1;
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void ll()
{
	system("leaks minishell");
}

int main (int ac, char **av, char **envp)
{
	(void)av;
	char	*line;
	struct termios	term;
	t_data	data;

	atexit(ll);
	if (ac != 1)
	{
		ft_putstr_fd("minishell: no arguments needed!\n", 2);
		exit(1);
	}
	if (!isatty(0))
	{
		ft_putstr_fd("minishell: is not a tty!\n", 2);
		exit(1);
	}
	data = (t_data){NULL, NULL, 0, NULL, NULL, \
		NULL, 0, NULL, 0, 0, -2, -2, envp, 0, NULL};
	init_envs(envp, &data);
	signal(SIGINT, INT_HANDLER);
	signal(SIGQUIT, SIG_IGN);
	while (77)
	{
		data.allocated = NULL;
		data.fd_in = -2;
		data.fd_out = -2;
		tcgetattr(0, &term);
		line = readline("minishell$ ");
		if (!line)
		{
            ft_putstr_fd("exit\n", 1);
			free(line);
			free_allocated(&data.allocated);
            break;
		}
		if (g_signal_flag == 1 && data.exit_status != 1)
		{
			data.exit_status = 1;
			g_signal_flag = 0;
		}
		if (!parsing(line, &data))
			continue ;
		if (g_signal_flag == 1)
		{
			g_signal_flag = 0;
			continue ;
		}
		execute_cmds(&data);
		free(line);
		free_allocated(&data.allocated);
		tcsetattr(0, TCSANOW, &term);
		break ;
	}
	free_env(&data.env);
	return (data.exit_status);
}
