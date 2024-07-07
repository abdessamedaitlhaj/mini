#include "../includes/minishell.h"

void	init_envs(char **envp, t_data *data)
{
	int		i;
	char 	*var;

	i = -1;
	while (envp[++i])
	{
		var = ft_strdup(envp[i], &data->allocated);
		// if (!var)
		// 	return ;
		ft_lstadd_back(&data->env, ft_lstnew(var));
	}
}

void	f()
{
	system("leaks minishell");
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

int main (int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char	*line;
	t_data	data;
	data = (t_data){NULL, NULL, 0, NULL, NULL, NULL, 0, NULL, 0, envp, 0};
	init_envs(envp, &data);
	while (77)
	{
		line = readline("minishell$ ");
		if (!parsing(line, &data))
			continue ;
		execute_cmds(&data);
		free (line);
	}
	free_allocated(&data.allocated);
	free_env(&data.env);
}