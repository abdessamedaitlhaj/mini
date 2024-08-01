/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:06:30 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 01:40:28 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fill_command_help(t_cmd *cmds, t_data *data)
{
	check_null_command(cmds, data);
	ambigious(cmds, data);
	heredoc_loop(cmds, data);
	remove_quotes(cmds, data);
	data->cmds = cmds;
}

void	fill_command_index(t_cmd *cmds, t_indexes *indexes,
		char **split, t_data *data)
{
	indexes->j = 0;
	handle_cmd_allocation(&cmds[indexes->i], &indexes->j, split, data);
	indexes->l = indexes->j;
	indexes->k = 0;
	count_cmd_args(&indexes->j, &indexes->k, split);
	cmds[indexes->i].args = ft_malloc((sizeof(char *)
				* (indexes->k + 1)), &data->allocated);
	cmds[indexes->i].args[indexes->k] = NULL;
}

int	expanded(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	ft_min(int a, int b, int c)
{
	if (a == -1)
		a = 2147483647;
	if (b == -1)
		b = 2147483647;
	if (c == -1)
		c = 2147483647;
	if (a <= b && a <= c)
		return (a);
	if (b <= a && b <= c)
		return (b);
	return (c);
}
