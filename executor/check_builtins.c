/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 23:31:20 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 11:13:05 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0 || \
	ft_strcmp(cmd->cmd, "pwd") == 0 || ft_strcmp(cmd->cmd, "echo") == 0 \
	|| ft_strcmp(cmd->cmd, "env") == 0 || ft_strcmp(cmd->cmd, "export") == 0 \
	|| ft_strcmp(cmd->cmd, "unset") == 0 || ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	return (0);
}

int	other_builtins(t_data *data, t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd, "env") == 0)
		return (ft_env(data->env, cmd->args_number));
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (ft_export(cmd->args, data, cmd->cmd));
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (ft_unset(cmd->args, data, cmd->cmd));
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		ft_exit(cmd->args, cmd->args_number);
	return (0);
}

int	ft_exec_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (ft_cd(cmd->args[0], data));
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
	{
		ft_putendl_fd(ft_getenv("PWD", data->env), 1);
		return (0);
	}
	else if (ft_strcmp(cmd->cmd, "echo") == 0)
		return (ft_echo(data, cmd->args, cmd->args_number));
	return (other_builtins(data, cmd));
}
