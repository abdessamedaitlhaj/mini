/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 23:06:30 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 23:09:44 by ael-hara         ###   ########.fr       */
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
