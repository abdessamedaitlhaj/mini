/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:16:42 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 15:17:47 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_var(char **pipe, int *i, t_data *data)
{
	char	*var_value;
	int		j;
	int		flag;
	int		z;

	j = *i + 1;
	while ((*pipe)[j] && (ft_isalnum((*pipe)[j]) || (*pipe)[j] == '_'))
		j++;
	var_value = ft_getenv(ft_substr(*pipe, *i + 1, j - *i - 1, data),
			data->env);
	data->help = var_value;
	if (var_value)
		expand_exist(pipe, i, j, data);
	else
	{
		flag = 0;
		z = *i - 1;
		while (z > 0 && ft_isspace((*pipe)[z]))
			z--;
		if ((*pipe)[z] == '>' || (*pipe)[z] == '<')
			flag = 1;
		if (!flag)
			expand_flag(pipe, i, j, data);
	}
}

int	expand_help(char **pipe, int *i, t_data *data)
{
	if ((*pipe)[*i + 1] && (*pipe)[*i + 1] == '?')
	{
		*pipe = expand_exit(*pipe, i, data);
	}
	else if (((*pipe)[*i + 1] && !((*pipe)[*i + 1] == '_'
		|| ft_isalnum((*pipe)[*i + 1]) || (*pipe)[*i + 1] == '"'
			|| (*pipe)[*i + 1] == '\'')) || !(*pipe)[*i + 1])
	{
		(*i)++;
		return (-1);
	}
	else if ((*pipe)[*i + 1] && ft_isnum((*pipe)[*i + 1]))
		expand_num(pipe, *i, data);
	else if ((*pipe)[*i + 1] && ((*pipe)[*i + 1] == '"'
		|| (*pipe)[*i + 1] == '\''))
		expand_case(pipe, i, data);
	else if ((*pipe)[*i + 1] && ft_isspace((*pipe)[*i + 1]))
		(*i)++;
	else
		expand_var(pipe, i, data);
	return (0);
}

char	*expanding_outside(char *pipe, t_data *data)
{
	int	i;

	i = -1;
	while (pipe[++i])
	{
		if ((pipe[i] == '<' || pipe[i] == '>') && pipe[i + 1]
			&& (pipe[i + 1] == '<' || pipe[i + 1] == '>'))
			i = skip_redirection(pipe, i, 0);
		else if (pipe[i] == '<' || pipe[i] == '>')
		{
			i = skip_redirection(pipe, i, 1);
		}
		else if (pipe[i] == '$')
		{
			if (expand_help(&pipe, &i, data) == -1)
			{
				continue ;
			}
		}
	}
	return (pipe);
}

void	inside_exit(char **pipe, int *i, t_data *data)
{
	char	*exit_status_str;
	char	*before;
	char	*after;
	char	*new_pipe;

	exit_status_str = ft_itoa(data->exit_status, data);
	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 2, ft_strlen(*pipe) - *i - 2, data);
	new_pipe = ft_strjoin(before, exit_status_str, data);
	*pipe = ft_strjoin(new_pipe, after, data);
	*i += ft_strlen(exit_status_str) - 1;
}

void	inside_number(char **pipe, int *i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 2, ft_strlen(*pipe) - (*i + 2), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}
