/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 01:38:34 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 01:47:55 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_exit(char *pipe, int *i, t_data *data)
{
	char	*exit_status_str;
	char	*before;
	char	*after;
	char	*new_pipe;

	exit_status_str = ft_itoa(data->exit_status, data);
	before = ft_substr(pipe, 0, *i, data);
	after = ft_substr(pipe, *i + 2, ft_strlen(pipe) - *i - 2, data);
	new_pipe = ft_strjoin(before, exit_status_str, data);
	pipe = ft_strjoin(new_pipe, after, data);
	*i += ft_strlen(exit_status_str) - 1;
	return (pipe);
}

void	expand_num(char **pipe, int i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, i, data);
	after = ft_substr(*pipe, i + 2, ft_strlen(*pipe) - (i + 2), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void	expand_case(char **pipe, int *i, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, *i + 1, ft_strlen(*pipe) - (*i + 1), data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
}

void	expand_exist(char **pipe, int *i, int j, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, data->help, data);
	tmp = ft_strjoin(tmp, after, data);
	*pipe = tmp;
}

void	expand_flag(char **pipe, int *i, int j, t_data *data)
{
	char	*before;
	char	*after;
	char	*tmp;

	before = ft_substr(*pipe, 0, *i, data);
	after = ft_substr(*pipe, j, ft_strlen(*pipe) - j, data);
	tmp = ft_strjoin(before, after, data);
	*pipe = tmp;
	(*i)--;
}
