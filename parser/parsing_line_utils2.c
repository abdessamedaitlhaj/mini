/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_line_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 02:51:00 by ael-hara          #+#    #+#             */
/*   Updated: 2024/08/01 01:37:17 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_quote_help(char *str, char *new_str, int *i, int *j)
{
	char	c;

	c = str[*i];
	if ((str[*i] == '"' || str[*i] == '\''))
	{
		c = str[*i];
		(*i)++;
		while (str[*i] && str[*i] != c)
		{
			new_str[*j] = str[*i];
			(*j)++;
			(*i)++;
		}
		if (str[*i] == c)
		{
			(*i)++;
		}
	}
	else
	{
		new_str[*j] = str[*i];
		(*j)++;
		(*i)++;
	}
}

char	*remove_q(char *str, t_data *data)
{
	int		len;
	char	*new_str;
	int		i;
	int		j;

	len = ft_strlen(str);
	new_str = ft_malloc(sizeof(char) * (len + 1), &data->allocated);
	i = 0;
	j = 0;
	while (i < len)
	{
		remove_quote_help(str, new_str, &i, &j);
	}
	new_str[j] = '\0';
	return (new_str);
}

void	ambiguous_file(t_cmd *cmd, int i, int j, t_data *data)
{
	int	expand;

	if (cmd[i].files[j]->type != HEREDOC)
	{
		expand = expanded(cmd[i].files[j]->file);
		cmd[i].files[j]->file = expanding_outside(cmd[i].files[j]->file, data);
	}
	if (cmd[i].files[j] && expand && check_null(cmd[i].files[j]->file) == 0
		&& cmd[i].files[j]->type != HEREDOC)
		cmd[i].files[j]->expand_error = 1;
	else
		cmd[i].files[j]->expand_error = 0;
	quote_replace(cmd[i].files[j]->file, -11, '$');
	if (cmd[i].files[j]->file && cmd[i].files[j]->type != HEREDOC)
	{
		cmd[i].files[j]->file = expanding_final(cmd[i].files[j]->file, data);
	}
}

void	ambiguous_file_args(t_cmd *cmd, int i, t_data *data)
{
	int	j;

	j = 0;
	while (j < cmd[i].outfile + cmd[i].infile + cmd[i].heredoc
		+ cmd[i].append)
	{
		ambiguous_file(cmd, i, j, data);
		j++;
	}
	j = 0;
	while (j < cmd[i].args_number)
	{
		quote_replace(cmd[i].args[j], -11, '$');
		if (cmd[i].args[j])
			cmd[i].args[j] = expanding_final(cmd[i].args[j], data);
		j++;
	}
}

void	ambigious(t_cmd *cmd, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->counter_command)
	{
		ambiguous_file_args(cmd, i, data);
		if (cmd[i].cmd)
		{
			quote_replace(cmd[i].cmd, -11, '$');
			if (cmd[i].cmd)
				cmd[i].cmd = expanding_final(cmd[i].cmd, data);
		}
		i++;
	}
}
