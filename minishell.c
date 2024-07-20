#include "includes/minishell.h"

int	execute_cmds(t_data *data)
{
	int	status;
	int stdin_copy;
	int stdout_copy;

	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror("dup");
		return (1);
	}
	if (data->counter_command == 1)
		data->exit_status = execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = WTERMSIG(status) + 128;
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
			{
				ft_putstr_fd("Quit: ", STDOUT_FILENO);
				ft_putendl_fd(ft_itoa(WTERMSIG(status), data), STDOUT_FILENO);
			}
		}
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				perror("dup2");
				return (1);
			}
	close(stdin_copy);
	close(stdout_copy);
	return (0);
}

void	init_envs(char **envp, t_data *data)
{
	int		i;
	char 	*var;

	i = -1;
	while (envp[++i])
	{
		var = ft_strdup(envp[i], &data->allocated);
		if (!var)
			exit(EXIT_FAILURE);
		ft_lstadd_back(&data->env, ft_lstnew(var));
	}
}

void	free_env(t_list **env)
{
	t_list	*tmp;

	while (*env)
	{
		tmp = (*env)->next;
		free(*env);
		*env = tmp;
	}
}
// ctrl+c return empty line 

// ctrl+d exits bash if line is empty
// ctl+\ ignored

void	INT_HANDLER(int sig)
{
	if (sig == SIGINT)
	{
		//if (waitpid(-1, NULL, WNOHANG) == -1) {
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		//}
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
	
	atexit(f);
	t_data	data;
	data = (t_data){NULL, NULL, 0, NULL, NULL, NULL, 0, NULL, 0, -2, -2, envp, 0};
	init_envs(envp, &data);
	signal(SIGINT, INT_HANDLER);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	while (77)
	{
		data.fd_in = -2;
		data.fd_out = -2;
		tcgetattr(STDIN_FILENO, &term);
		line = readline("minishell$ ");
		if (!line)
		{
			rl_on_new_line();
			printf("\033[0A");
			rl_redisplay();
			ft_putstr_fd("exit", 1);
			free_env(&data.env);
			free_allocated(&data.allocated);
			break;
		}
		if (!parsing(line, &data))
			continue ;
		execute_cmds(&data);
		free(line);
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
		free_allocated(&data.allocated);
		break;
	}
	free_env(&data.env);
}
