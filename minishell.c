#include "includes/minishell.h"

int	execute_cmds(t_data *data)
{
	int	status;
	int stdin_copy;
	int stdout_copy;

	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
		return (error_two("dup"));
	if (data->counter_command == 1)
		execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
			get_status(data, &status);
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			return (error_two("dup2"));
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
		if (waitpid(-1, NULL, WNOHANG) == -1) {
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
}
int main (int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_data	data;
	data = (t_data){NULL, NULL, 0, NULL, NULL, NULL, 0, NULL, 0, -2, -2, envp, 0};
	init_envs(envp, &data);
	signal(SIGINT, INT_HANDLER);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
	while (77)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", 1);
			free_allocated(&data.allocated);
			exit(0);
		}
		if (!parsing(line, &data))
			continue ;
		execute_cmds(&data);
		free (line);
	}
	free_allocated(&data.allocated);
	free_env(&data.env);
}
