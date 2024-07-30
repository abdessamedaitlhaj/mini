/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:25:30 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 02:58:20 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	inside_quote_ex(char **pipe, int i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, i , ft_strlen(*pipe) - i, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void	normal_notfound(char **pipe, int i, int j, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

char	*normal_expand(char *pipe, int i, t_data *data)
{
	int		j;
	char	*var_value;
	char	*before;
	char	*after;
	char	*tmp;

	j = i + 1;
	while (pipe[j] && (ft_isalnum(pipe[j]) || pipe[j] == '_'))
		j++;
	var_value = get_env_value(ft_substr(pipe, i + 1, j - i - 1, data),
			data->env);
	if (var_value)
	{
		before = ft_substr(pipe, 0, i, data);
		after = ft_substr(pipe, j, ft_strlen(pipe) - j, data);
		tmp = ft_strjoin(before, var_value, data);
		tmp = ft_strjoin(tmp, after, data);
		pipe = tmp;
	}
	else
	{
		normal_notfound(&pipe, i, j, data);
	}
	return (pipe);
}

int	inside_cases(char **pipe, int *i, t_data *data)
{
	if ((*pipe)[*i + 1] && (*pipe)[*i + 1] == '?')
		inside_exit(pipe, i, data);
	else if (((*pipe)[*i + 1] && !(ft_isalnum((*pipe)[*i + 1])
		|| (*pipe)[*i + 1] == '_')) || !(*pipe)[*i + 1])
	{
		(*i)++;
		return (-1);
	}
	else if ((*pipe)[*i + 1] && ft_isnum((*pipe)[*i + 1]))
		inside_number(pipe, i, data);
	else if ((*pipe)[*i + 1] && ((*pipe)[*i + 1] == '"'
		|| (*pipe)[*i + 1] == '\''))
		inside_quote_ex(pipe, *i, data);
	else if ((*pipe)[*i + 1] && ft_isspace((*pipe)[*i + 1]))
		(*i)++;
	else
	{
		*pipe = normal_expand(*pipe, *i, data);
		*i = *i - 1;
	}
	return (0);
}

char	*expanding_inside(char *pipe, t_data *data)
{
	int	i;

	i = -1;
	while (pipe[++i])
	{
		if (pipe[i] == '$')
		{
			if (inside_cases(&pipe, &i, data) == -1)
			{
				continue ;
			}
		}
	}
	return (pipe);
}
