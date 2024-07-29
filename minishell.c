#include "includes/minishell.h"

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

// ctrl+c return empty line 
// ctrl+d exits bash if line is empty
// ctl+\ ignored

void	INT_HANDLER(int sig)
{
	if (sig == SIGINT)
	{
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}

void f()
{
	system("leaks minishell");
}


int main (int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	struct termios	term;
	t_data	data;

	data = (t_data){NULL, NULL, 0, NULL, NULL, \
		NULL, 0, NULL, NULL, 1, 0, -2, -2, envp, 0};
	init_envs(envp, &data);
	signal(SIGINT, INT_HANDLER);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	tcgetattr(0, &term);
	while (77)
	{
		data.allocated = NULL;
		data.fd_in = -2;
		data.fd_out = -2;
		tcgetattr(0, &term);
		line = readline("minishell$ ");
		if (!line)
		{
			rl_clear_history();
			write (2, "exit\n", 5);
			free_env(&data.env);
			free_allocated(&data.allocated);
			exit(0);
		}
		if (!parsing(line, &data))
			continue ;
		execute_cmds(&data);
		free(line);
		free_allocated(&data.allocated);
		tcsetattr(0, TCSANOW, &term);
	}
	free_env(&data.env);
	return (data.exit_status);
}
